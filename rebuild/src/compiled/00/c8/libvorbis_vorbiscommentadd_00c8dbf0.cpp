// libvorbis::vorbis_comment_add(vorbis_comment* vc, char* comment) -- libvorbis info.c: grow the comment and length arrays by one, store a malloc'd copy of the comment and its length, and keep the list NULL-terminated
// Leaf: @vorbis_comment_add@8
// Module (sweep2, 2026-10-25): owner decision 1c unifies the statically linked Xiph code on libogg / libvorbis (vorbisfile.c
//   ov_* with libvorbis); was module _global. The extern "C" __fastcall symbol is unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DBF0, length 147 bytes (160 with padding).
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909"). No PDB/RTTI: libvorbis API name.
// Convention: LTCG __fastcall (vc in ecx, comment in edx). Callees: MSVCR71 realloc (IAT 0x01440054, cached in ebx),
// malloc (IAT 0x01440158). strlen and strcpy are the /O2 intrinsics (inline byte loops).
// Shape: the libvorbis body verbatim; every vc->comments use re-reads the member after the calls.
#include <string.h>
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int size);                 // IAT 0x01440158
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // IAT 0x01440054
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00 (comments + 1 entries, NULL-terminated)
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};

extern "C" void __fastcall vorbis_comment_add(vorbis_comment* vc, char* comment)
{
    vc->user_comments = (char**)realloc(vc->user_comments,
                            (vc->comments + 2) * sizeof(*vc->user_comments));
    vc->comment_lengths = (int*)realloc(vc->comment_lengths,
                            (vc->comments + 2) * sizeof(*vc->comment_lengths));
    vc->comment_lengths[vc->comments] = strlen(comment);
    vc->user_comments[vc->comments] = (char*)malloc(vc->comment_lengths[vc->comments] + 1);
    strcpy(vc->user_comments[vc->comments], comment);
    vc->comments++;
    vc->user_comments[vc->comments] = 0;
}