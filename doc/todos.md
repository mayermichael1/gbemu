# Todos

- [x]   make window resizable while aspect ratio has to stay the same
- [x]   when resizing fetch from the right texel from texture 
- [x]   memory arenas instead of mallocs
- [x]   implement test suite into this project
- [ ]   memory viewer for current gbzstate
- [ ]   general memory arena for general purposes
- [ ]   very simple font renderer

## CPU

- [x]   research registers
- [x]   research memory
- [x]	write down cpu and memory findings digitally
- [x]   find a documentation showing and explaining the instruction encoding
- [x]   extend operand type with address, etc... for determining what to do in 
        operations
- [x]   instead of storing a dispatcher in the instruction struct switch on a type
- [x]	parse a bit stream to the cpu instructions
    - [x]    missing parser for bigger instruction size (currently pc is skipped forward)
- [ ]	implement the cpu instructions
    - [ ]   flags are not set yet
- [ ]   should instruction always zero initialize to something?

## Graphics

- [ ]	how did graphics work in general on the game boy?

## Cartridges 

- [ ]	how where cartridges actually read on the game boy. 
        Probably the cartridge was not pre-loaded into the memory as a whole
        (actually it is mapped to memory so it was not loaded at all. Direclty 
        read by the cpu)

## Audio 

- [ ]	find a librabry or way to write audio in linux

## Platform Layer

- [ ]	implement a platform layer for windows as well
- [ ]	maybe use wayland on linux as a challenge
