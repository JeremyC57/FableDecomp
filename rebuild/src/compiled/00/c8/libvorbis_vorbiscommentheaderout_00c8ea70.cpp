// libvorbis::vorbis_commentheader_out(vorbis_comment* vc, ogg_packet* op) -- libvorbis info.c: pack the comment header into a local bit-packer and hand a malloc'd copy of its bytes out as a (non-BOS, non-EOS, granule 0) packet; OV_EIMPL when packing fails
// Leaf: @vorbis_commentheader_out@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8EA70, length 127 bytes.
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909"). No PDB/RTTI: libvorbis API name.
// Convention: LTCG __fastcall (vc in ecx, op in edx). Callees: oggpack_writeinit 0x00C90F10 and oggpack_bytes
// 0x00C917B0 (libogg bitwise.c, LTCG __fastcall, b in ecx); _vorbis_pack_comment 0x00C8E790 (LTCG static: opb in ecx,
// vc on the stack, callee-clean -- the thiscall shape, so it is modelled as a member of the packer); MSVCR71 malloc
// (IAT 0x01440158). The memcpy is the /O2 intrinsic (`rep movsd ; rep movsb`).
// Shape: the libvorbis 1.0.x body verbatim (no oggpack_writeclear: 1.0.x leaks the packer buffer here); oggpack_bytes
// is called three times; the 64-bit granulepos store is the last two of the four zero stores.
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);   // IAT 0x01440158
typedef __int64 ogg_int64_t;
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00 (comments + 1 entries, NULL-terminated)
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};
struct oggpack_buffer {                 // 0x14 bytes (libogg bitwise.c)
    long           endbyte;             // +0x00
    int            endbit;              // +0x04
    unsigned char* buffer;              // +0x08
    unsigned char* ptr;                 // +0x0c
    long           storage;             // +0x10
    int _vorbis_pack_comment(vorbis_comment* vc);   // 0x00C8E790 (opb in ecx, vc on the stack)
};
struct ogg_packet {                     // 0x20 bytes
    unsigned char* packet;              // +0x00
    long           bytes;               // +0x04
    long           b_o_s;               // +0x08
    long           e_o_s;               // +0x0c
    ogg_int64_t    granulepos;          // +0x10
    ogg_int64_t    packetno;            // +0x18
};

#define OV_EIMPL -130

extern "C" void __fastcall oggpack_writeinit(oggpack_buffer* b);   // 0x00C90F10
extern "C" long __fastcall oggpack_bytes(oggpack_buffer* b);       // 0x00C917B0

extern "C" int __fastcall vorbis_commentheader_out(vorbis_comment* vc, ogg_packet* op)
{
    oggpack_buffer opb;

    oggpack_writeinit(&opb);
    if (opb._vorbis_pack_comment(vc)) return OV_EIMPL;

    op->packet = (unsigned char*)malloc(oggpack_bytes(&opb));
    memcpy(op->packet, opb.buffer, oggpack_bytes(&opb));

    op->bytes = oggpack_bytes(&opb);
    op->b_o_s = 0;
    op->e_o_s = 0;
    op->granulepos = 0;

    return 0;
}