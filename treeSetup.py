from distutils.core import setup, Extension

module = Extension("Tree", sources=["twoThreeTree.c"]);

setup(name="twoThreeTree",
  version="1.0",
  description="2-3 tree for integer storage in C",
  ext_modules=[module])
