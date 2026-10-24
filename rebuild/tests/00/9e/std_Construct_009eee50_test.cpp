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
namespace N009eee50 {
struct CCharString {
    void* m_pData;                               // ref-counted string body
    CCharString() { std::memset(this, 0, sizeof(CCharString)); }
    CCharString(const CCharString& other) { std::memcpy(this, &other, sizeof(CCharString)); ++g_extCopies; }
};
struct CCPPointerInfo { long RefCount; };
template <class T> struct CCountedPointer {      // shared pointer: object + shared count block
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct CMeshDataBank;
struct CMeshDataBankInfo {                       // CMeshBankManager::CMeshDataBankInfo (PDB, sizeof 0xC)
    CCharString                     BankHandle;      // +0x00
    CCountedPointer<CMeshDataBank>  MeshDataBank;    // +0x04 Data, +0x08 Info
};

void __fastcall _Construct(CMeshDataBankInfo* p, const CMeshDataBankInfo& value)
{
    if (p != 0)
        new (p) CMeshDataBankInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N009eee50;
    CCPPointerInfo info; info.RefCount = 1;
    CMeshDataBankInfo src;
    src.BankHandle.m_pData = reinterpret_cast<void*>(17);
    src.MeshDataBank.Data = reinterpret_cast<CMeshDataBank*>(0x40);
    src.MeshDataBank.Info = &info;
    CMeshDataBankInfo* slot = reinterpret_cast<CMeshDataBankInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->BankHandle.m_pData == src.BankHandle.m_pData)) ok = false;
    if (!(slot->MeshDataBank.Data == src.MeshDataBank.Data)) ok = false;
    if (!(slot->MeshDataBank.Info == src.MeshDataBank.Info)) ok = false;
    if (info.RefCount != 2) ok = false;   // the counted pointer copy took a reference
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("009eee50_TEST FAIL\n"); return 1; }
    std::printf("009eee50_TEST PASS\n");
    return 0;
}