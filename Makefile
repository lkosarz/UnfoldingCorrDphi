#===============================================================================
#   File: Makefile
#
#   Example Makefile for Pythia8 written for HF in STAR
#   Author: Thomas Ullrich
#   Last modified: September 9, 2008
#
#   This needs 3 packages installed: Pythia8, LHAPDF, and ROOT
#
#   This is setup for Mac OS X 10.5.4 but should be easy to 
#   adapt for other (Unix) platforms. In principle changing the
#   program name (PROGRAM), the location of PYTHIA (PYTHIAPATH),
#   and the LHAPDF libraries (LHAPDFPATH) should do the job.
#   Note that the environment variable ROOTSYS needs to be set.
#   Otherwise define it here in the makefile.
#===============================================================================
OBJS  =  getResponseMatrixMain.C DictOutput.cxx
#OBJS  =  getResponseMatrixMain.C
PROGRAM  =  getResponseMatrixMain

PYTHIAPATH   = /gpfs01/star/pwg/lkosarz/Upsilon_pp500GeV_2011/UnfoldingCorr/UnfoldPythiaQian/pythia8142
LHAPDFPATH   = /gpfs01/star/pwg/lkosarz/Upsilon_pp500GeV_2011/UnfoldingCorr/UnfoldPythiaQian/lhapdf-5.7.0-install/lib
#ROOTSYS  = /afs/rhic.bnl.gov/star/ROOT/5.34.30/.sl64_gcc482/rootdeb
ROOTLIBS = `root-config --cflags --libs`

PYTHIALIBS = -lLHAPDF -llhapdfdummy -lpythia8
DICTHEADERS = Event.h TNtuple.h LinkDef.h

ROOUNFOLD = RooUnfold
ROOUNFOLDLIB = /gpfs01/star/pwg/lkosarz/photoNuclear/HadronicEESI/UnfoldingCorr

CXX      =  g++
CXXFLAGS =  -m32 -fno-inline -O  -W -Wall -g
CPPFLAGS = -I$(PYTHIAPATH)/include -I$(ROOTSYS)/include -I$(ROOUNFOLD)/src
LDFLAGS  = -L$(PYTHIAPATH)/lib/archive -L$(ROOTSYS)/lib -L$(LHAPDFPATH) -L$(ROOUNFOLDLIB) -lRooUnfold -L$(ROOTSYS)/lib $(ROOTLIBS)

all: $(PROGRAM)

$(PROGRAM):	$(OBJS) Makefile
		$(CXX) $(CXXFLAGS) $(PROGRAM).C $(CPPFLAGS) $(LDFLAGS) -o $(PROGRAM) 

DictOutput.cxx :
	rootcint -f $@ -c $(DICTHEADERS)
			
clean:
		rm -f $(PROGRAM) DictOutput.cxx DictOutput.h
