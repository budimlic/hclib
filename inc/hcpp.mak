PROJECT_CXXFLAGS=-std=c++11 -I$(HCLIB_ROOT)/include -I$(HCPP_INSTALL)/include
PROJECT_LDFLAGS=-L$(HCLIB_ROOT)/lib -L$(OCR_INSTALL)/lib  -L$(HCPP_INSTALL)/lib
PROJECT_LDLIBS=-lhclib -locr -lhcpp
