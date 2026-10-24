// _global::lzo1x_999_compress(const lzo_byte* in, lzo_uint in_len, lzo_byte* out, lzo_uintp out_len, lzo_voidp wrkmem) -- LZO 1.08 lzo1x_999_compress: best-ratio LZO1X compression at level 8 through lzo1x_999_compress_internal
// Leaf: _lzo1x_999_compress
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C08130, 58 bytes. LZO 1.08 lzo1x_9x.c, C linkage, __cdecl (LZO_PUBLIC).
// Callee: lzo1x_999_compress_internal 0x00C07AF0 (__cdecl, 14 arguments; Ego_r PDB name set
// _lzo1x_999_compress_internal at the matching debug-build slot).
// Shape: the library's lzo1x_999_compress calls lzo1x_999_compress_level(..., NULL, 0, 0, 8), which looks level 8 up
// in its static table c[7] = { try_lazy 2, good_length 32, max_lazy 128, nice_length 0, max_chain 2048, flags 1 }.
// Retail has that call inlined and folded (whole-program build), so the body is the direct internal call with the
// table row as immediates: 14 pushes, `add esp,38h`. Written here in that folded form: defining
// lzo1x_999_compress_level in the TU would also make the `_lzo1x_999_compress` leaf ambiguous.
typedef unsigned char lzo_byte;
typedef unsigned int  lzo_uint;
typedef lzo_uint*     lzo_uintp;
typedef void*         lzo_voidp;
typedef unsigned long lzo_uint32;
typedef void (__cdecl *lzo_progress_callback_t)(lzo_uint, lzo_uint);

extern "C" int __cdecl lzo1x_999_compress_internal(const lzo_byte* in, lzo_uint in_len,
                                                    lzo_byte* out, lzo_uintp out_len,
                                                    lzo_voidp wrkmem,
                                                    const lzo_byte* dict, lzo_uint dict_len,
                                                    lzo_progress_callback_t cb,
                                                    int try_lazy,
                                                    lzo_uint good_length,
                                                    lzo_uint max_lazy,
                                                    lzo_uint nice_length,
                                                    lzo_uint max_chain,
                                                    lzo_uint32 flags);   // 0x00C07AF0

extern "C" int __cdecl lzo1x_999_compress(const lzo_byte* in, lzo_uint in_len,
                                          lzo_byte* out, lzo_uintp out_len,
                                          lzo_voidp wrkmem)
{
    /* level 8: c[7] = { 2, 32, 128, 0, 2048, 1 } */
    return lzo1x_999_compress_internal(in, in_len, out, out_len, wrkmem,
                                       0, 0, (lzo_progress_callback_t)0,
                                       2, 32, 128, 0, 2048, 1);
}