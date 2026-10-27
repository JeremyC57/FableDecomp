// _global::z_compressBound(uLong sourceLen) -- zlib 1.2.1 compressBound: the worst-case compress() output size for sourceLen input bytes
// Leaf: _z_compressBound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C03EB0, 21 bytes. zlib 1.2.1 compress.c, Z_PREFIX build, C linkage, __cdecl. No callees.
// Identity (proven): the zlib 1.2.1 compress.c body, byte for byte (`sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11`,
//   the 1.2.1 bound; 1.2.2+ changed it), inside the 0x00BFEC30..0x00C08170 zlib 1.2.1 + LZO 1.08 run whose other members
//   are landed as _z_inflateReset / _z_inflateInit2_ / _z_inflateEnd.
typedef unsigned long uLong;

extern "C" uLong __cdecl z_compressBound(uLong sourceLen)
{
    return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11;
}