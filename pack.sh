#!/bin/bash
set -xe
cd build
cpack -G DEB -B build
