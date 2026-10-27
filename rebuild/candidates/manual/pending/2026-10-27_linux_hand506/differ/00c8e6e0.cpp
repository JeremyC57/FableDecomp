// libvorbis::_vorbis_pack_info(oggpack_buffer* opb, vorbis_info* vi) -- libvorbis info.c: write the identification header body (packet type 1, "vorbis", version 0, channels, rate, the three bitrates, ilog2 of both block sizes, framing bit); OV_EFAULT when the codec setup is missing
// Grade: DIFFER(179v175) -- LTCG private register convention: retail takes opb in EAX and vi in ESI (body never loads them) and calls _v_writestring with opb in EBX / string in ECX / length in EAX; VC7.1 __fastcall moves ecx/edx and stack args instead (lane gap: LTCG)
// Leaf: @_vorbis_pack_info@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C8E6E0, 175 bytes. libpk's statically linked Xiph libvorbis 20030909 (lib/info.c). The LTCG link
// gave this static a private register convention: opb arrives in EAX and vi in ESI (neither is loaded in the body), and it calls
// _v_writestring 0x00C8DB80 with opb in EBX, the string in ECX and the length in EAX -- conventions VC7.1 cannot be asked for.
// Identity: the "vorbis" literal 0x012BF238 with length 6, the 8/32/8/32/32/32/32/4/4/1 field widths and OV_EFAULT (-129) are
// info.c _vorbis_pack_info exactly; ilog2 0x00C8DB60 and _v_writestring 0x00C8DB80 are info.c's statics. Catalogued as gap-scan.
// Callees: oggpack_write @ 0x00C90FC0, _v_writestring @ 0x00C8DB80, ilog2 @ 0x00C8DB60.
typedef struct {
    long endbyte;
    int endbit;
    unsigned char* buffer;
    unsigned char* ptr;
    long storage;
} oggpack_buffer;
typedef struct vorbis_info {
    int version;
    int channels;
    long rate;
    long bitrate_upper;
    long bitrate_nominal;
    long bitrate_lower;
    long bitrate_window;
    void* codec_setup;
} vorbis_info;

typedef struct codec_setup_info {      // codec_internal.h (leading members)
    long blocksizes[2];
} codec_setup_info;

extern "C" void __fastcall oggpack_write(oggpack_buffer* b, unsigned long value, int bits);   // 0x00C90FC0
extern "C" void __fastcall _v_writestring(oggpack_buffer* o, const char* s, int bytes);       // 0x00C8DB80
extern "C" int __fastcall ilog2(unsigned int v);                                              // 0x00C8DB60

extern "C" int __fastcall _vorbis_pack_info(oggpack_buffer* opb, vorbis_info* vi)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    if (!ci) return (-129);   // OV_EFAULT

    /* preamble */
    oggpack_write(opb, 0x01, 8);
    _v_writestring(opb, "vorbis", 6);

    /* basic information about the stream */
    oggpack_write(opb, 0x00, 32);
    oggpack_write(opb, vi->channels, 8);
    oggpack_write(opb, vi->rate, 32);

    oggpack_write(opb, vi->bitrate_upper, 32);
    oggpack_write(opb, vi->bitrate_nominal, 32);
    oggpack_write(opb, vi->bitrate_lower, 32);

    oggpack_write(opb, ilog2(ci->blocksizes[0]), 4);
    oggpack_write(opb, ilog2(ci->blocksizes[1]), 4);
    oggpack_write(opb, 1, 1);

    return (0);
}
