OPT=-O0
DEBUG=-g
LIBS=-lglfw -lGL
FLAGS=-std=c99

BUILDDIR=./build
BINARY=$(BUILDDIR)/gbemu
INCLUDEDIRS=-I. 

.PHONY: buildandrun
buildandrun: binary
	$(BINARY)

.PHONY: binary
binary: $(BUILDDIR)
	gcc main.c $(OPT) $(DEBUG) $(LIBS) $(FLAGS) $(INCLUDEDIRS) -o $(BINARY) 

.PHONY: test
test: $(BUILDDIR) testbin
	gcc $(OPT) $(DEBUG) $(LIBS) $(FLAGS) $(INCLUDEDIRS) -o -fpic -shared -o build/gbtest.so test/gbtest.c 
	$(BUILDDIR)/testbin $(BUILDDIR)/gbtest.so

.PHONY: testbin
testbin: 
	gcc lib/c_test_suite/test.c $(OPT) $(DEBUG) $(LIBS) $(FLAGS) $(INCLUDEDIRS) -o $(BUILDDIR)/testbin 


$(BUILDDIR):
	@mkdir $(BUILDDIR)
	@echo "*" > $(BUILDDIR)/.gitignore
	@echo "!.gitignore" >> $(BUILDDIR)/.gitignore

.PHONY : clean
clean: 
	rm -rf $(BUILDDIR)
	rm -f compile_commands.json
	rm -rf .ccls-cache 
