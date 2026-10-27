// libvorbis::floor0_unpack(vorbis_info* vi, oggpack_buffer* opb) -- libvorbis floor0.c: read a floor-0 setup header (order 8 bits, rate 16, barkmap 16, ampbits 6, ampdB 8, numbooks-1 4, then numbooks 8-bit book numbers) into a new vorbis_info_floor0; any order/rate/barkmap/numbooks below 1 or a book number outside [0, ci->books) frees it (floor0_free_info) and returns NULL
// Leaf: @floor0_unpack@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3600, 221 bytes. libpk's statically linked Xiph libvorbis 20030909 (lib/floor0.c); the
// static unpack takes vi in ecx and opb in edx (the LTCG link turned the libvorbis statics into __fastcall), plain `ret`.
// Identity: floor0.c's unpack -- it allocates sizeof(vorbis_info_floor0) = 0x60 bytes, reads exactly floor0's header fields, checks
// the book numbers against codec_setup_info::books (+0x18), and its error path calls the landed floor0_free_info 0x00CA3580, the
// function that precedes it in floor0.c. Catalogued as gap-scan sub_00ca3600.
// Callees: malloc (IAT 0x01440158, _ogg_malloc), oggpack_read @ 0x00C915A0 (__fastcall), floor0_free_info @ 0x00CA3580.
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // [0x01440158]
#define _ogg_malloc malloc

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

typedef struct codec_setup_info {      // codec_internal.h (leading members; the rest is not touched here)
    long blocksizes[2];
    int modes;
    int maps;
    int floors;
    int residues;
    int books;                          // +0x18
    int psys;
} codec_setup_info;

typedef void vorbis_info_floor;

typedef struct {
    int order;
    long rate;
    long barkmap;
    int ampbits;
    int ampdB;
    int numbooks;                       /* <= 16 */
    int books[16];
    float lessthan;                     /* encode-only config setting hacks for libvorbis */
    float greaterthan;                  /* encode-only config setting hacks for libvorbis */
} vorbis_info_floor0;                   // 96 bytes

extern "C" long __fastcall oggpack_read(oggpack_buffer* b, int bits);   // 0x00C915A0
extern "C" void __fastcall floor0_free_info(vorbis_info_floor* i);      // 0x00CA3580

extern "C" vorbis_info_floor* __fastcall floor0_unpack(vorbis_info* vi, oggpack_buffer* opb)
{
    codec_setup_info* ci = (codec_setup_info*)vi->codec_setup;
    int j;

    vorbis_info_floor0* info = (vorbis_info_floor0*)_ogg_malloc(sizeof(*info));
    info->order = oggpack_read(opb, 8);
    info->rate = oggpack_read(opb, 16);
    info->barkmap = oggpack_read(opb, 16);
    info->ampbits = oggpack_read(opb, 6);
    info->ampdB = oggpack_read(opb, 8);
    info->numbooks = oggpack_read(opb, 4) + 1;

    if (info->order < 1) goto err_out;
    if (info->rate < 1) goto err_out;
    if (info->barkmap < 1) goto err_out;
    if (info->numbooks < 1) goto err_out;

    for (j = 0; j < info->numbooks; j++) {
        info->books[j] = oggpack_read(opb, 8);
        if (info->books[j] < 0 || info->books[j] >= ci->books) goto err_out;
    }
    return (info);

err_out:
    floor0_free_info(info);
    return (0);
}