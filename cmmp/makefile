NAMES := test

CXX := g++-7
CXX_FLAGS := -std=c++11
MAKE := make

LIBS := -ldlib -lX11 
EXTERN_DEPEND :=
EXTERN := 
CEXTERN :=

INCLUDE := 
LIB_L := 
OBJ := 
SRC :=

INCLUDE := $(shell find . -type d -not -path '*/\.*') $(INCLUDE)
INCLUDE := $(INCLUDE) $(EXTERN) $(CEXTERN)
INCLUDE := $(patsubst %, -I%, $(INCLUDE))

LIB_L := $(shell find . -type d -not -path '*/\.*') $(LIB_L)
LIB_L := $(LIB_L) $(EXTERN) $(CEXTERN)
LIB_L := $(patsubst %, -L%, $(LIB_L))

_SRC := $(shell find . -type f -name '*.cpp' -not -path '*/\.*')
_NAMES := $(patsubst %, %.cpp, $(NAMES))
_NAMES := $(shell find . -name $(_NAMES) -not -path '*/\.*')
NAMES_O := $(patsubst %, %.o, $(NAMES))

SRC := $(filter-out $(_NAMES), $(_SRC)) $(SRC)
OBJ := $(patsubst %.cpp, %.o, $(SRC)) $(OBJ)

all: $(NAMES)

$(NAMES):%:%.cpp $(EXTERN_DEPEND) $(OBJ) $(NAMES_O)
	$(CXX) $(INCLUDE) $(LIB_L) $(OBJ) $(LIBS) $(CXX_FLAGS) $< -o $@

$(NAMES_O):

$(OBJ):%.o:%.cpp
	$(CXX) -c $(INCLUDE) $(CXX_FLAGS) $< -o $@

$(EXTERN_DEPEND):
	$(foreach extern, $(EXTERN), $(shell $(MAKE) -C $(extern)))

clean:
	rm -f $(OBJ)
	$(foreach extern, $(EXTERN), $(shell make -C $(extern) clean))

fclean:
	rm -f $(OBJ)
	rm -f $(NAMES)
	rm -f .depend2 .depend
	$(foreach extern, $(EXTERN), $(shell make -C $(extern) fclean))

re: clean all

print:
	@echo "NAMES: $(NAMES)"
	@echo "CXX: $(CXX)"
	@echo "CXX_FLAGS: $(CXX_FLAGS)"
	@echo "LIBS: $(LIBS)"
	@echo "EXTERN_DEPEND: $(EXTERN_DEPEND)"
	@echo "EXTERN: $(EXTERN)"
	@echo "INCLUDE: $(INCLUDE)"
	@echo "LIB_L: $(LIB_L)"
	@echo "SRC: $(SRC)"
	@echo "OBJ: $(OBJ)"

depend: .depend .depend2

.depend: $(SRC)
	rm -f ./.depend
	$(CXX) $(INCLUDE) $(CXX_FLAGS) -MM $^ -MF  ./.depend;

.depend2: $(_NAMES)
	rm -f ./.depend2
	$(CXX) $(INCLUDE) $(CXX_FLAGS) -MM $^ -MF  ./.depend2;

.PHONY: $(NAMES_O)

include .depend
include .depend2