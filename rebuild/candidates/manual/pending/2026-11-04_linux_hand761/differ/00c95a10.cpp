// libvorbis::_best(codebook* book, float* a, int step) -- Xiph libvorbis 1.0 vq/codebook.c: find the codebook entry nearest the vector (threshold search, else exhaustive _dist)
// Grade: DIFFER(0v588) -- third-party: libvorbis (Xiph Ogg Vorbis, _best); not reconstructed per the brief
// Leaf: none
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95A10, 588 bytes. Evidence: calls _dist 0x00C95910 inside the exhaustive loop, walks the
//   encode_tthresh / thresh tables of the codebook's static description, inside the libvorbis block of the image.
