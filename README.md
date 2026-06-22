# VaRLib

![C++](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-compiler-A42E2B?logo=gnu&logoColor=white)
![Make](https://img.shields.io/badge/Make-build-427819?logo=gnubash&logoColor=white)
![Status](https://img.shields.io/badge/status-WIP-orange)

A header-only, zero-dependency library for calculating **VaR** on a given data set. Written in C++23.

> [!WARNING]
> This library is incomplete and the API may change without notice. Active development is ongoing.

## Development Status

Not yet single-header and features are still being decided. Contributions are welcome but please open an issue first to discuss.

## Planned Features

Runtime VaR calculation supporting:

1. Historical VaR and CVaR (implemented , not tested)
2. Parametric (variance-covariance) VaR and CVaR
3. Monte Carlo VaR and CVaR

**Experimental:** a compile-time API for backtesting on known data sets.
