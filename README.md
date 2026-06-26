# VaRLib

![C++](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-compiler-A42E2B?logo=gnu&logoColor=white)
![Make](https://img.shields.io/badge/Make-build-427819?logo=gnubash&logoColor=white)
![Status](https://img.shields.io/badge/status-WIP-orange)

A header-only, **zero-dependency** library for calculating **VaR** on a given data set. Written in C++23.
> [!WARNING]
> This library is incomplete and the API may change **without notice**. Active development is ongoing.

## Development Status

Features are still being decided.

## How to use 

There is still **no support** for the IMPLEMENTATION MACRO so the whole library gets pulled in.
Simply do `#include "VaRLib.h"` in your desired location after placing the downloaded VaRLib.h file in your project's directory. **If you place it elsewhere you will need to point your build system to it!**  

## Planned Features

Runtime VaR calculation supporting:

1. Historical VaR and CVaR (implemented, not tested)
2. Parametric (variance-covariance) VaR and CVaR
3. Monte Carlo VaR and CVaR

