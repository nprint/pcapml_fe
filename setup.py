from setuptools import setup, Extension

pcapml_fe = Extension(name='pcapml_fe',
                     include_dirs=['./src/pcapml/include/', './src/pcapml/include/dir/', './src/pcapml/include/label',
                                   './src/pcapml/include/sample/', './src/pcapml/include/pcap/',
                                   './src/pcapml/include/pcapng/'],
                     extra_compile_args = ['-std=c++11'],
                     libraries=['pcap', 'stdc++'],
                     sources=['./src/sampler/pcapml_fe_sampler.cpp', './src/pcapml/src/pcapng/block_pcapng.cpp',
                              './src/pcapml/src/pcapng/reader_pcapng.cpp', './src/pcapml/src/pcapng/sorter_pcapng.cpp',
                              './src/pcapml/src/util.cpp', './src/pcapml/src/sample/sampler.cpp',
                              './src/pcapml/src/sample/sample.cpp']
                    )

try:
    import pypandoc
    long_description = pypandoc.convert('README.md', 'rst')
except(IOError, ImportError):
    print('EEE')
    long_description = open('README.md').read()

setup(name="pcapml_fe",
      version="0.0.3",
      description="Python interface for pcapML feature exploration library",
      author="The nPrint team",
      license='Apache 2.0 license',
      url="https://nprint.github.io/pcapml_fe",
      author_email="nprint.pr@gmail.com",
      platforms = ["Linux", "MacOS"],
      package_dir = {'': 'src',},
      long_description=long_description,
      packages = ['pcapml_fe_helpers'],
      install_requires=['scapy',
                        'dpkt',
                       ],
      ext_modules=[pcapml_fe]
     )

