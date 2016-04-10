TRAVIS: [![Build Status](https://travis-ci.org/headupinclouds/local_laplacian_filters.svg?branch=hunter)](https://travis-ci.org/headupinclouds/local_laplacian_filters)
APPVEYOR: [![Build status](https://ci.appveyor.com/api/projects/status/9uo26abfh8tece9c/branch/hunter?svg=true)](https://ci.appveyor.com/project/headupinclouds/local-laplacian-filters/branch/hunter)


# README #

Implementation of the Local Laplacian Filters image processing algorithm in C++ using OpenCV. The algorithm is described here:

Paris, Sylvain, Samuel W. Hasinoff, and Jan Kautz. "Local Laplacian filters: edge-aware image processing with a Laplacian pyramid." ACM Trans. Graph. 30.4 (2011): 68.

The project is built using CMake and a C++11 compiler. I have developed the code on Mac OS X, so you may have to change the includes or add Windows includes to get it to compile.


```
#!bash
mkdir build && cd build
cmake ..
make
```

The code has currently been tested for detail enhancement and reduction. Tone mapping is untested, but will be soon.