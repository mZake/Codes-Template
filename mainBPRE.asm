.gba
.thumb
.open "BPRE.gba","build/rom_BPRE.gba", 0x08000000
//---------------------------------------------------

//hooks
//None

.align 4
.org insertinto
.importobj "build/linked.o"
.close
