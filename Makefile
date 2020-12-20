IDIR = -I./include -I/usr/include/opus/
SRC=./src/

LIN-ODIR=obj/lin
LIN-LDIR =-L/usr/lib/ -L./lib/

LIN-CXX=g++
LIN-CFLAGS=-pedantic -Wall $(IDIR)v -std=c++11
LIN-LIBS=$(LIN-LDIR) -lopenal -lpthread

WIN-ODIR=obj/win
WIN-LDIR =-L./lib/

WIN-CXX=i686-w64-mingw32-g++-posix
WIN-CFLAGS=-pedantic -Wall $(IDIR) -Wl,--subsystem,windows -std=c++11 -static-libgcc
WIN-LIBS=$(WIN-LDIR) ./lib/OpenAL32.lib -lpthread

_DEPS = 
_OBJ = main.o soundManager.o sound.o

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
LIN-OBJ = $(patsubst %,$(LIN-ODIR)/%,$(_OBJ))
WIN-OBJ = $(patsubst %,$(WIN-ODIR)/%,$(_OBJ))

$(LIN-ODIR)/%.o: $(SRC)%.cpp $(DEPS)
	$(LIN-CXX) -c -o $@ $< $(LIN-CFLAGS)
main-lin: $(LIN-OBJ)
	$(LIN-CXX) -o ./bin/lin/$@ $^ $(LIN-CFLAGS) $(LIN-LIBS)	
	
	
$(WIN-ODIR)/%.o: $(SRC)%.cpp $(DEPS)
	$(WIN-CXX) -c -o $@ $< $(WIN-CFLAGS)
main.exe: $(WIN-OBJ)
	$(WIN-CXX) -o ./bin/win/$@ $^ $(WIN-CFLAGS) $(WIN-LIBS)


.PHONY: clean all

all: main-lin main.exe
clean:
	rm -f $(LIN-ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f $(WIN-ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f ./bin/lin/*
	rm -f ./bin/win/*
