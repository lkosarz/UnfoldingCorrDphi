#!/bin/csh

setup 64b
setup ROOT 6.20.08

setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/gpfs01/star/pwg/lkosarz/photoNuclear/HadronicEESI/UnfoldingCorr/RooUnfold

./getResponseMatrixMain