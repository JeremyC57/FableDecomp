// vorbis_comment_add_tag(vorbis_comment* vc, char* tag, char* contents) -- libvorbis info.c: build "TAG=contents" in an alloca buffer and add it as a comment
// Leaf: vorbis_comment_add_tag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DC90, length 156 bytes (160 with padding); `ret 4`.
// Library: libpk's statically linked Xiph libvorbis ("Xiph.Org libVorbis I 20030909"). No PDB/RTTI: libvorbis API name.
// Convention: LTCG __fastcall (vc in ecx, tag in edx, contents on the stack). Callees: __chkstk / _alloca_probe
// 0x00BFEA30 (the alloca, size in eax), vorbis_comment_add 0x00C8DBF0 (LTCG __fastcall). The "=" literal @ 0x0129C1F8
// is copied as one word. strlen/strcpy/strcat are the /O2 intrinsics.
// Shape: the libvorbis body verbatim; vc survives the alloca in the ebp frame slot [ebp-4].
#include <string.h>
#include <malloc.h>
struct vorbis_comment {                 // 0x10 bytes
    char** user_comments;               // +0x00 (comments + 1 entries, NULL-terminated)
    int*   comment_lengths;             // +0x04
    int    comments;                    // +0x08
    char*  vendor;                      // +0x0c
};

extern "C" void __fastcall vorbis_comment_add(vorbis_comment* vc, char* comment);   // 0x00C8DBF0

extern "C" void __fastcall vorbis_comment_add_tag(vorbis_comment* vc, char* tag, char* contents)
{
    char* comment = (char*)_alloca(strlen(tag) + strlen(contents) + 2); /* +2 for = and \0 */
    strcpy(comment, tag);
    strcat(comment, "=");
    strcat(comment, contents);
    vorbis_comment_add(vc, comment);
}