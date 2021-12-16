from distutils.core import setup, Extension

pcapml_fe = Extension(name='pcapml_fe',
                     define_macros = [('MAJOR_VERSION', '0'),
                                      ('MINOR_VERSION', '1')],

                     include_dirs=['../pcapml/include/', '../pcapml/include/dir/', '../pcapml/include/label',
                                   '../pcapml/include/sample/', '../pcapml/include/pcap/',
                                   '../pcapml/include/pcapng/'],
                     extra_compile_args = ['-std=c++11'],
                     libraries=['pcap', 'stdc++'],
                     sources=['pcapml_pymod.cpp', '../pcapml/src/pcapng/block_pcapng.cpp',
                              '../pcapml/src/pcapng/reader_pcapng.cpp', '../pcapml/src/pcapng/sorter_pcapng.cpp',
                              '../pcapml/src/util.cpp', '../pcapml/src/sample/sampler.cpp',
                              '../pcapml/src/sample/sample.cpp']
                    )

setup(name="pcapml_fe",
      version="0.0.1",
      description="Python interface for pcapML feature exploration",
      author="nPrint",
      author_email="nprint.pr@gmail.com",
      ext_modules=[pcapml_fe]
     )
