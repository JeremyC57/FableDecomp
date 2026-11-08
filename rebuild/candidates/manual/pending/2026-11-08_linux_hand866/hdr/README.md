# Header changes -- none

Chunk 866 adds no header overlay. `engine/Vorbis.h` is used unchanged by 0x00C8E880 (as a C TU, `/TC /Gr`, because the header's
member named `class` at line 390 makes it C-only). The other rows model their Xiph structures the way the landed libvorbis /
libogg neighbours in rebuild/src/compiled/00/c8 and 00/c9 do.
