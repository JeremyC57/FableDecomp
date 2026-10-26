// Behaviour test for 0x00458833 CPersistContext::Transfer<T>: CheckCRC(""), then a CPersistTraits<T> local loads (mode 2) or saves (mode 3) the value; any other mode does neither; the traits are destroyed on every path.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#endif
static int g_crc = 0, g_in = 0, g_out = 0, g_ctor = 0, g_dtor = 0; static const void* g_stream = 0; static const void* g_value = 0;

class CDataOutputStream;
class CMemoryDataInputStream;

namespace NHitLocations { class CPhysicalPrimitiveBase; }
struct SCountBlock;

template <class T> class CCountedPointer { public:
    T*           Data;                  // +0
    SCountBlock* Count;                 // +4
    CCountedPointer() : Data(0), Count(0) {}
    ~CCountedPointer();                 // 0x0044F3B2 for T = NHitLocations::CPhysicalPrimitiveBase
};

class CPhysicalPrimitiveInit {
public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CPhysicalPrimitiveInit> {
    CPhysicalPrimitiveInit DefaultVal;                                                   // +0x00
    void TransferBinaryOut(CDataOutputStream* stream, const CPhysicalPrimitiveInit* value); // 0x0067F7D0
    void TransferBinaryIn(CMemoryDataInputStream* stream, CPhysicalPrimitiveInit* value);   // 0x0067F870
};

class CPersistContext { public:
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

template void CPersistContext::Transfer<CPhysicalPrimitiveInit>(CPhysicalPrimitiveInit& value);
template <> CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>::~CCountedPointer() { ++g_dtor; if (Data || Count) g_dtor += 100; }
void CPersistContext::CheckCRC(const char* name) { if (name && name[0] == 0) ++g_crc; }
void CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryOut(CDataOutputStream* stream, const CPhysicalPrimitiveInit* value) { ++g_out; g_stream = stream; g_value = value; if (DefaultVal.PPrimitive.Data) g_out += 100; }
void CPersistTraits<CPhysicalPrimitiveInit>::TransferBinaryIn(CMemoryDataInputStream* stream, CPhysicalPrimitiveInit* value) { ++g_in; g_stream = stream; g_value = value; if (DefaultVal.PPrimitive.Count) g_in += 100; }

int main()
{
    CPersistContext ctx; memset(&ctx, 0, sizeof(ctx));
    int inTag = 0, outTag = 0;
    ctx.PLoadStream = (CMemoryDataInputStream*)&inTag; ctx.PSaveStream = (CDataOutputStream*)&outTag;
    int ok = 1;
    {
        CPhysicalPrimitiveInit v;
        g_dtor = 0;
        ctx.Mode = 2; ctx.Transfer(v);                      // the traits' default value is a zeroed counted pointer
        if (g_crc != 1 || g_in != 1 || g_out != 0 || g_stream != &inTag || g_value != &v || g_dtor != 1) ok = 0;
        ctx.Mode = 3; ctx.Transfer(v);
        if (g_crc != 2 || g_in != 1 || g_out != 1 || g_stream != &outTag || g_value != &v || g_dtor != 2) ok = 0;
        ctx.Mode = 0; ctx.Transfer(v);
        if (g_crc != 3 || g_in != 1 || g_out != 1 || g_dtor != 3) ok = 0;
    }
    printf(ok ? "00458833_TEST PASS\n" : "00458833_TEST FAIL\n");
    return ok ? 0 : 1;
}