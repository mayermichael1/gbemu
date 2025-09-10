# Todos

[Todos](./doc/todos.md)

# Documentation

Information from [GBDEV.io](https://gbdev.io)

## GBZ80 CPU

### Registers

|16|H|L|Name                      |
|--|-|-|--------------------------|
|AF|A|-|Accumulator and Flags     |
|BC|B|C|BC (General Purpose)      |
|DE|D|E|DE (General Purpose)      |
|HL|H|L|HL (General Purpose)      |
|SP|-|-|Stack Pointer             |
|PC|-|-|Program Counter / Pointer |

Following Flags exist in the lower 8 bits of the AF register:

|Bit|Purpose                   |
|---|--------------------------|
|  7|Zero Flag (z)             |
|  6|Subtraction Flag (N, BCD) |
|  5|Half Carry Flag (H, BCD)  |
|  4|Cary Flag (C, Cy)         |
|  3|always zero               |
|  2|always zero               |
|  1|always zero               |
|  0|always zero               |

- z ... is set when the previous instruction results in 0
- C,Cy ... is set when:
    - Result of 8 bit add is > 0xFF
    - Result of 16 bit add is > 0xFF
    - Result of subtract or cmp is < 0
    - When rotate or shift operation shifts out a 1
- BCD Flags:
    - N ... is set when the last operation is subtraction 
    - H ... carry bit of the lower 4 bits of the last operation

### Memory

The memory is 16 bit addressable, divided into different segments:

|Start|End |Description             |Note                               |
|-----|----|------------------------|-----------------------------------|
| 0000|3FFF|16 KiB ROM bank 0       |                                   |
| 4000|7FFF|16 KiB ROM bank 01-N    |switchable via mapper if exists?   |
| 8000|9FFF|8 KiB Video RAM         |switchable on GBC                  |
| A000|BFFF|8 KiB External RAM      |on cartridge, switchable           |
| C000|CFFF|4 KiB Work RAM          |                                   |
| D000|EFFF|4 KiB Work RAM          |Switchable in GBC                  |
| E000|FDFF|Echo Ram                |use is prohibited                  |
| FE00|FE9F|Object attribute memory |?                                  |
| FEA0|FEFF|not useable             |use is prohibited                  |
| FF00|FF7F|I/O Registers           |                                   |
| FF80|FFFE|High RAM                |                                   |
| FFFE|FFFF|Interput Enable Register|                                   |

#### VRAM

The VRAM Bank first contains: 

384 **tiles** of 16 bytes each
(commonly thought of as 3 groups )

After that each bank contains two maps of:

32 x 32 (=1024)

in bank 0 these are **tilemaps**, in bank 1 these are **attributes**

#### Jump Vectors

Following adresses are reserverd for jump vectors.
If few or no `rst` instructions are used these addresses may be used as wanted.

RST:        0000, 0008, 0010, 0018, 0020, 0028, 0030, 0038
Interupts:  0040, 0048, 0050, 0058, 0060 

#### Cartridge Header

The memory area 0100 - 014F contains the cartridge header

#### External Memory

Can be used normally as RAM.
Often used to control a MBC (Memory Board Controller)
Often used to save data persistently (As long as the on cartridge battery supplies power)

#### Echo RAM

Echos another RAM region

#### FFA0 - FEFF

prohibited but has special uses on certain game boy revisions

