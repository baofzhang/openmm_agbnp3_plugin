from distutils.core import setup
from distutils.extension import Extension
import os
import sys
import platform

openmm_dir = '@OPENMM_DIR@'
AGBNP3plugin_header_dir = '@AGBNP3PLUGIN_HEADER_DIR@'
AGBNP3plugin_library_dir = '@AGBNP3PLUGIN_LIBRARY_DIR@'
AGBNP3_dir = '@AGBNP3_DIR@'

# setup extra compile and link arguments on Mac
extra_compile_args = []
extra_link_args = []

openmm_lib_path = os.getenv('OPENMM_LIB_PATH')

if platform.system() == 'Darwin':
    extra_compile_args += ['-stdlib=libc++', '-mmacosx-version-min=10.7']
    extra_link_args += ['-stdlib=libc++', '-mmacosx-version-min=10.7', '-Wl', '-rpath', openmm_lib_path]

extension = Extension(name='_AGBNP3plugin',
                      sources=['AGBNP3PluginWrapper.cpp'],
                      libraries=['OpenMM', 'AGBNP3Plugin', 'gomp', 'nblist', 'agbnp3'],
                      include_dirs=[os.path.join(openmm_dir, 'include'), AGBNP3plugin_header_dir],
                      library_dirs=[os.path.join(openmm_dir, 'lib'), AGBNP3plugin_library_dir, AGBNP3_dir],
                      extra_compile_args=extra_compile_args,
                      extra_link_args=extra_link_args
                     )

setup(name='AGBNP3plugin',
      version='1.0',
      py_modules=['AGBNP3plugin'],
      ext_modules=[extension],
     )
