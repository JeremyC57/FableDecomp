// libvorbis::_vorbis_unpack_books(vorbis_info* vi, oggpack_buffer* opb) -- Xiph libvorbis 1.0 info.c: read the setup header (codebooks, time/floor/residue/mapping/mode tables) from the packet
// Grade: DIFFER(0v663) -- third-party: libvorbis (Xiph Ogg Vorbis, info.c _vorbis_unpack_books); not reconstructed per the brief
// Leaf: none
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8E2E0, 663 bytes. Evidence: 15 calls to oggpack_read 0x00C915A0 with the setup-header
//   field widths, the error exit through vorbis_info_clear 0x00C8DFB0 (OV_EBADHEADER), placed between _vorbis_unpack_comment and the
//   synthesis code in the libvorbis block 0x00C8C730.. of the image; the repo already lands libvorbis neighbours (libvorbis_* rows
//   0x00C89C10..0x00C89E80) under that module name.
