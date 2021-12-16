/*
 * Copyright 2020 nPrint
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at https://www.apache.org/licenses/LICENSE-2.0
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "sampler.hpp"

/* SamplerState - pcapML sample group instance.
 *
 * sampler: PcapMLSampler handle
 *
*/
typedef struct {
    PyObject_HEAD
    Sampler sampler;
} SamplerState;

/* Fields contained in a PcapmlSampleType 
 *
 * sid: Sample ID for the pcapml sample
 * metadata: metadata field, often used for labels
 * packets: list of packets in the sample
 *
*/
static PyStructSequence_Field pcapml_sample_fields[] = {
    {"sid", "Sample ID"},
    {"metadata", "Sample metadata field"},
    {"packets", "Packets in sample"},
    {NULL}
};

/* PcapmlSample description. Points at fields from pcapml_sample_fields */
static PyStructSequence_Desc pcapml_sample_desc = {
    "pcapml.sample",
    "pcapML sample structsequence", // docstring can't be NULL!! Will SEGFAULT
    pcapml_sample_fields,
    3
};

/* Fields contained in a PcapmlPacketType 
 *
 * ts: packet timestamp
 * raw_bytes: packet raw bytes
 *
*/
static PyStructSequence_Field pcapml_packet_fields[] = {
    {"ts", "Packet timestamp"},
    {"raw_bytes", "Packet bytes"},
    {NULL}
};

/* PcapmlPacket description. Points at fields from pcapml_packet_fields */
static PyStructSequence_Desc pcapml_packet_desc = {
    "pcapml.packet",
    "pcapML packet structsequence", // docstring can't be NULL!! Will SEGFAULT
    pcapml_packet_fields,
    2
};

/* Instantiate a new sampler */
static PyObject *
sampler_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    
    char *filename = NULL;
    Sampler s;
    
    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    SamplerState *sstate = (SamplerState *)type->tp_alloc(type, 0);
    
    if (!sstate)
        return NULL;
    
    s.open_pcapng(filename);
    sstate->sampler = s;

    return (PyObject *)sstate;    
}

static void
sampler_dealloc(SamplerState *sstate)
{
    Py_TYPE(sstate)->tp_free(sstate);
}

/* Sampler iterator */
static PyObject *
sampler_next(SamplerState *sstate)
{
    Sample *s;
    uint64_t i;

    while(1) {
            
            s = sstate->sampler.get_next_sample();
            if (s == NULL) {
                
                return NULL;
            }

            PyObject *packetlst = PyList_New(s->get_pkts().size());

            PyTypeObject *PcapmlSampleType = PyStructSequence_NewType(&pcapml_sample_desc);
            PyTypeObject *PcapmlPacketType = PyStructSequence_NewType(&pcapml_packet_desc);

            /* Create a pcapmlsample to eventually return */
            PyObject *pcapmlsample = PyStructSequence_New(PcapmlSampleType);

            /* Set the sid and metadata field values */
            PyStructSequence_SET_ITEM(pcapmlsample, 0, Py_BuildValue("k", s->get_sid()));
            PyStructSequence_SET_ITEM(pcapmlsample, 1, Py_BuildValue("s", s->get_label().c_str()));

            /* Build a list of packets to be inserted into the pcapmlsample */
            for (i = 0; i < s->get_pkts().size(); ++i) {
                PyObject *pcapmlpacket = PyStructSequence_New(PcapmlPacketType);

                /* For each packet set the ts and raw bytes */
                PyStructSequence_SET_ITEM(pcapmlpacket, 0, Py_BuildValue("k", s->get_pkt_ts()[i]));
                PyStructSequence_SET_ITEM(pcapmlpacket, 1, Py_BuildValue("y#", s->get_pkts()[i], s->get_pkt_lens()[i]));

                /* Place the packet in the sample list */
                PyList_SET_ITEM(packetlst, i, pcapmlpacket);
            }

            /* Place the packet list in the sample */
            PyStructSequence_SET_ITEM(pcapmlsample, 2, packetlst);
            
            return pcapmlsample;
    }

}

PyTypeObject PySampler_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "pcapml_fe",                     /* tp_name */
    sizeof(SamplerState),            /* tp_basicsize */
    0,                               /* tp_itemsize */
    (destructor)sampler_dealloc,     /* tp_dealloc */
    0,                               /* tp_print */
    0,                               /* tp_getattr */
    0,                               /* tp_setattr */
    0,                               /* tp_reserved */
    0,                               /* tp_repr */
    0,                               /* tp_as_number */
    0,                               /* tp_as_sequence */
    0,                               /* tp_as_mapping */
    0,                               /* tp_hash */
    0,                               /* tp_call */
    0,                               /* tp_str */
    0,                               /* tp_getattro */
    0,                               /* tp_setattro */
    0,                               /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,              /* tp_flags */
    0,                               /* tp_doc */
    0,                               /* tp_traverse */
    0,                               /* tp_clear */
    0,                               /* tp_richcompare */
    0,                               /* tp_weaklistoffset */
    PyObject_SelfIter,               /* tp_iter */
    (iternextfunc)sampler_next,      /* tp_iternext */
    0,                               /* tp_methods */
    0,                               /* tp_members */
    0,                               /* tp_getset */
    0,                               /* tp_base */
    0,                               /* tp_dict */
    0,                               /* tp_descr_get */
    0,                               /* tp_descr_set */
    0,                               /* tp_dictoffset */
    0,                               /* tp_init */
    PyType_GenericAlloc,             /* tp_alloc */
    sampler_new,                     /* tp_new */
};


static struct PyModuleDef pcapmlmodule = {
    PyModuleDef_HEAD_INIT,
    "pcapml_fe",
    "Python module for the pcapML feature exploration library",
    -1,
};

PyMODINIT_FUNC
PyInit_pcapml_fe(void)
{
    PyObject *module = PyModule_Create(&pcapmlmodule);

    if (!module)
        return NULL;

    if (PyType_Ready(&PySampler_Type) < 0)
        return NULL;
    Py_INCREF((PyObject *)&PySampler_Type);
    PyModule_AddObject(module, "sampler", (PyObject *)&PySampler_Type);

    return module;
}