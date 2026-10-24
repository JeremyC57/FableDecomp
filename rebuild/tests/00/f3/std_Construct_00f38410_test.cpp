#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N00f38410 {
struct CArrayLong {                                 // retail 12-byte STLport-vector body (long)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CArrayLong() { std::memset(this, 0, sizeof(CArrayLong)); }
    CArrayLong(const CArrayLong& other) { std::memcpy(this, &other, sizeof(CArrayLong)); ++g_extCopies; }
};
struct CResourcePage {                           // CGPUScratchBufferTextureManager::CResourcePage (retail sizeof 0x1C; PDB 0x20 with a 16-byte CArray)
    long           ScratchPool;                  // +0x00
    CArrayLong     ResourceList;                 // +0x04 CArray<long>
    unsigned char* NextFreeBufferAddress;        // +0x10
    long           FreeSpaceSize;                // +0x14
    bool           EnableAutoRestore;            // +0x18
};

void __fastcall _Construct(CResourcePage* p, const CResourcePage& value)
{
    if (p != 0)
        new (p) CResourcePage(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00f38410;
    CResourcePage src;
    src.ScratchPool = 2;
    src.ResourceList._M_start = reinterpret_cast<long*>(0x70);
    src.NextFreeBufferAddress = reinterpret_cast<unsigned char*>(0x80);
    src.FreeSpaceSize = 4096;
    src.EnableAutoRestore = true;
    CResourcePage* slot = reinterpret_cast<CResourcePage*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->ScratchPool == src.ScratchPool)) ok = false;
    if (!(slot->ResourceList._M_start == src.ResourceList._M_start)) ok = false;
    if (!(slot->NextFreeBufferAddress == src.NextFreeBufferAddress)) ok = false;
    if (!(slot->FreeSpaceSize == src.FreeSpaceSize)) ok = false;
    if (!(slot->EnableAutoRestore == src.EnableAutoRestore)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("00f38410_TEST FAIL\n"); return 1; }
    std::printf("00f38410_TEST PASS\n");
    return 0;
}