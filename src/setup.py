from distutils.core import setup, Extension

pcapml_fe = Extension(name='pcapml_fe',
                     define_macros = [('MAJOR_VERSION', '0'),
                                      ('MINOR_VERSION', '1')],

                     include_dirs=['../include/', '../include/dir/', '../include/label',
                                   '../include/sample/', '../include/pcap/',
                                   '../include/pcapng/'],
                     libraries=['pcap', 'stdc++'],
                     sources=['pcapml_pymod.cpp', '../pcapML/src/pcapng/block_pcapng.cpp',
                              '../pcapML/src/pcapng/reader_pcapng.cpp', '../pcapML/src/pcapng/sorter_pcapng.cpp',
                              '../pcapML/src/util.cpp', '../pcapML/src/sample/sampler.cpp',
                              '../pcapML/src/sample/sample.cpp']
                    )

setup(name="pcapml_fe",
      version="0.0.1",
      description="Python interface for pcapML feature exploration",
      author="nPrint",
      author_email="nprint.pr@gmail.com",
      ext_modules=[pcapml_fe]
     )
