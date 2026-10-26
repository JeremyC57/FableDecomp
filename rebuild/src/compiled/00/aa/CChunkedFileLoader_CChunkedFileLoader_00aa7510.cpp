// CChunkedFileLoader::CChunkedFileLoader(const char* type) -- vtable 0x0129E464 (RTTI: CChunkedFileLoader),
// the five-byte SupportedFileType zeroed then the four type bytes copied in, ReadUnknownChunkTypes = 0.
// Not CBase-derived (no base constructor call). PDB layout: ReadUnknownChunkTypes +4, SupportedFileType[5] +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7510, length 40 bytes. No callees (intrinsic memset/memcpy).
#include <string.h>
class CChunkedFileLoader { public:
    virtual void VSlot0();
    long ReadUnknownChunkTypes;                 // +0x04
    char SupportedFileType[5];                  // +0x08
    CChunkedFileLoader(const char* type);
};

CChunkedFileLoader::CChunkedFileLoader(const char* type)
{
    memset(SupportedFileType, 0, sizeof(SupportedFileType));
    memcpy(SupportedFileType, type, 4);
    ReadUnknownChunkTypes = 0;
}