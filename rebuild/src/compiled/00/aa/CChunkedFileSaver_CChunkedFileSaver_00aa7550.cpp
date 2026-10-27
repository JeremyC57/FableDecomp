// CChunkedFileSaver::CChunkedFileSaver(const char* type) -- set the vtable, zero the five-byte SupportedFileType and copy the four type characters in
// Leaf: ??0CChunkedFileSaver@@QAE@PBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA7550, 33 bytes, `ret 4`. Stores vtable 0x0129E46C (RTTI CChunkedFileSaver);
//   Ego_r.pdb `??0CChunkedFileSaver@@QAE@PBD@Z`. Twin of the landed CChunkedFileLoader(const char*) 0x00AA7510 (vtable
//   0x0129E464), which sits right before it; callers 0x00A98A08 / 0x00A9A878 (the mesh-file savers).
// Layout: Ego_r.pdb CChunkedFileSaver (12 bytes): vptr +0, char SupportedFileType[5] +4. The intrinsic memset of five
//   bytes is the dword + byte zero stores through esi, the four-byte memcpy one dword move.
#include <string.h>

class CChunkedFileSaver {
public:
    char SupportedFileType[5];                          // +0x04
    CChunkedFileSaver(const char* type);
    virtual ~CChunkedFileSaver();
};

CChunkedFileSaver::CChunkedFileSaver(const char* type)
{
    memset(SupportedFileType, 0, sizeof(SupportedFileType));
    memcpy(SupportedFileType, type, 4);
}