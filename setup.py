from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
import numpy
import os

def find_version(path):
    import re
    # path shall be a plain ascii text file.
    s = open(path, 'rt').read()
    version_match = re.search(r"^__version__ = ['\"]([^'\"]*)['\"]",
                              s, re.M)
    if version_match:
        return version_match.group(1)
    raise RuntimeError("Version not found")


extensions = [
        Extension("pmesh._domain", ["pmesh/_domain.pyx"], include_dirs=["./", numpy.get_include()]),
        Extension("pmesh._window", ["pmesh/_window.pyx", "pmesh/_window_imp.c"],
                depends=["pmesh/_window_imp.h", "pmesh/_window_generics.h", "pmesh/_window_wavelets.h", "pmesh/_window_lanczos.h"], include_dirs=["./", numpy.get_include()])
        ]
print(find_version("pmesh/version.py"))

setup(
    name="pmesh", version=find_version("pmesh/version.py"),
    author="Yu Feng",
    description="Particle Mesh in Python",
    package_dir = {'pmesh': 'pmesh'},
    packages= ['pmesh', 'pmesh.tests'],
    install_requires=['cython', 'numpy', 'mpi4py', 'mpsort', 'pfft-python'],
    ext_modules = cythonize(extensions)
)

