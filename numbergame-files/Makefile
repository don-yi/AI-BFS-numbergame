PRG=gnu.exe

GCC=g++
GCCFLAGS=-O2 -Wall -Wextra -std=c++11 -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wsign-promo  -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder

VALGRIND_OPTIONS=-q --leak-check=full
DIFFLAGS=--strip-trailing-cr -y --suppress-common-lines

OBJECTS0=numbergame.cpp
DRIVER0=driver.cpp

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
0 1 2 3 :
	@echo "running test$@"
	watchdog 1000 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFFLAGS) > difference$@
4 5:
	@echo "running test$@"
	watchdog 5000 ./$(PRG) $@ >studentout$@
	diff out$@ studentout$@ $(DIFFLAGS) > difference$@
clean:
	rm -f *.exe *.o student* difference*
