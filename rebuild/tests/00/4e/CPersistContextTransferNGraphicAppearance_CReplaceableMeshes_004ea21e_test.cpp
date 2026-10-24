// Behaviour test for 0x004EA21E CPersistContext::Transfer<T>: CheckCRC(""), then a CPersistTraits<T> local loads (mode 2) or saves (mode 3) the value; any other mode does neither; the traits are destroyed on every path.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#endif
static int g_crc = 0, g_in = 0, g_out = 0, g_ctor = 0, g_dtor = 0; static const void* g_stream = 0; static const void* g_value = 0;

struct CDataOutputStream;
struct CMemoryDataInputStream;

namespace NGraphicAppearance {
struct CEngineGraphicVector {                   // CArray<CEngineGraphic> = STLport vector, 12 bytes
    void* _M_start;                             // +0x00
    void* _M_finish;                            // +0x04
    void* _M_end_of_storage;                    // +0x08
};
class CReplaceableMeshes {
public:
    CEngineGraphicVector Vector;                // +0x00
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<NGraphicAppearance::CReplaceableMeshes> {
    NGraphicAppearance::CReplaceableMeshes DefaultVal;                                                  // +0x00
    CPersistTraits();                                                                                   // 0x004EA2AE
    ~CPersistTraits();                                                                                  // 0x004EA2BB
    void TransferBinaryOut(CDataOutputStream* stream, const NGraphicAppearance::CReplaceableMeshes* value);   // 0x004EA26B
    void TransferBinaryIn(CMemoryDataInputStream* stream, NGraphicAppearance::CReplaceableMeshes* value);     // 0x004EA2C9
};

struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    Mode;                           // +0x18 (2 = binary load, 3 = binary save)
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;                    // +0x24
    CDataOutputStream*      PSaveStream;                    // +0x28
    void CheckCRC(const char* name);                        // 0x00404500
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case 2:
        traits.TransferBinaryIn(PLoadStream, &value);
        return;
    case 3:
        traits.TransferBinaryOut(PSaveStream, &value);
        return;
    }
}

template void CPersistContext::Transfer<NGraphicAppearance::CReplaceableMeshes>(NGraphicAppearance::CReplaceableMeshes& value);
void CPersistContext::CheckCRC(const char* name) { if (name && name[0] == 0) ++g_crc; }
CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::CPersistTraits() { ++g_ctor; }
CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::~CPersistTraits() { ++g_dtor; }
void CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryOut(CDataOutputStream* stream, const NGraphicAppearance::CReplaceableMeshes* value) { ++g_out; g_stream = stream; g_value = value; }
void CPersistTraits<NGraphicAppearance::CReplaceableMeshes>::TransferBinaryIn(CMemoryDataInputStream* stream, NGraphicAppearance::CReplaceableMeshes* value) { ++g_in; g_stream = stream; g_value = value; }

int main()
{
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx));
    int inTag = 0, outTag = 0;
    ctx.PLoadStream = (CMemoryDataInputStream*)&inTag; ctx.PSaveStream = (CDataOutputStream*)&outTag;
    NGraphicAppearance::CReplaceableMeshes v;
    int ok = 1;
    ctx.Mode = 2; ctx.Transfer(v);
    if (g_crc != 1 || g_in != 1 || g_out != 0 || g_stream != &inTag || g_value != &v || g_ctor != 1 || g_dtor != 1) ok = 0;
    ctx.Mode = 3; ctx.Transfer(v);
    if (g_crc != 2 || g_in != 1 || g_out != 1 || g_stream != &outTag || g_value != &v || g_ctor != 2 || g_dtor != 2) ok = 0;
    ctx.Mode = 1; ctx.Transfer(v);
    if (g_crc != 3 || g_in != 1 || g_out != 1 || g_ctor != 3 || g_dtor != 3) ok = 0;
    printf(ok ? "004ea21e_TEST PASS\n" : "004ea21e_TEST FAIL\n");
    return ok ? 0 : 1;
}