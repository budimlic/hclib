---
layout: default
title: Installation
permalink: /installation/
---

Current build status of HCLib:
[![Build Status](https://travis-ci.org/habanero-rice/hclib.svg?branch=master)](https://travis-ci.org/habanero-rice/hclib)

Downloading
---------------------------------------------

HCLib source code can be checked out from GitHub here:
[HCLib](https://github.com/habanero-rice/hclib)

`git clone https://github.com/habanero-rice/hclib.git`

Installation
---------------------------------------------

HClib follows your standard bootstrap, configure, and make installation
procedure. An install.sh script is provided for your convenience that will
build and install HClib. Simply run the script to install:

    ./install.sh

By default, HClib will be installed to `$PWD/hclib-install`. If you want to use
a different installation location, you can override the `INSTALL_PREFIX`
environment variable:

    INSTALL_PREFIX=/opt/local ./install.sh

Likewise, if you would like to use C/C++ compiler other than the
system defaults, then you can specify them using the `CC` and `CXX` environment
variables. For example, if you want to use the Intel compilers:

    CC=icc CXX=icpc ./install.sh

You will need to set the `HCLIB_ROOT` environment variable to point to your
HClib installation directory. You can automatically set this variable after
installation by sourcing the `hclib_setup_env.sh` script. For example, assuming
HClib was installed with `INSTALL_PREFIX=/opt/local`:

    source /opt/local/bin/hclib_setup_env.sh


Dependencies
---------------------------------------------

* automake
* gcc >= 4.8.4, or clang >= 3.5
  (must support -std=c11 and -std=c++11)
* libxml2 (with development headers)
* jsmn JSON parser. You can get it from https://github.com/zserge/jsmn.git, install it using the instructions below, then set the environment variable JSMN_HOME to the installation folder.
* OpenSHMEM library. Installation details are below.

Installing jsmn
---------------------------------------------
* Get jsmn from https://github.com/zserge/jsmn.git
* cd to the jsmn directory
* run `CFLAGS=-fPIC make`
* set the JSMN_HOME variable to the directory where jsmn is installed

Installing OpenSHMEM
---------------------------------------------
There are several versions of OpenSHMEM that you can install on your system, depending on what kind of hardware you have and what do you want to use as the communication layer. You can find out a lot more about it at http://www.openshmem.org. Here we describe the simplest installation process of OpenSHMEM on top of OpenMPI.


Testing
---------------------------------------------

The main regression tests for HClib are in the test/c and test/cpp folders. The
`test_all.sh` scripts in each of those folders will automatically build and run
all test cases.


Static Checks
---------------------------------------------

As part of the development workflow for HClib, any newly committed code should
be checked using standard static checking tools.

In particular, run cppcheck on all modified files. cppcheck is available online
at [1]. cppcheck should be run by cd-ing to tools/cppcheck and executing the
run.sh script from there (this assumes cppcheck is on your path). Any new errors
printed by cppcheck should be addressed before committing.

You should also run astyle on all modified files. astyle is a source code
auto-formatter. Simply cd to tools/astyle and execute the run.sh script from
there. This assumes you have astyle installed and it is on your path.

[1] https://sourceforge.net/projects/cppcheck/

