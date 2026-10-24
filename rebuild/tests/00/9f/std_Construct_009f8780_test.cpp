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
namespace N009f8780 {
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
struct CGraphicDataBankInit { unsigned long Words[11]; };   // 0x2C bytes, trivially copied
struct CGraphicDataBank;
struct CGraphicDataBankInfo {                    // CGraphicBankManager::CGraphicDataBankInfo (PDB, sizeof 0x38)
    CGraphicDataBankInit              Init;             // +0x00
    CCharString                       BankHandle;       // +0x2C
    CCountedPointer<CGraphicDataBank> GraphicDataBank;  // +0x30
    CGraphicDataBankInfo() { }
    __forceinline CGraphicDataBankInfo(const CGraphicDataBankInfo& o)   // the implicit memberwise copy, spelled out so VC7.1 inlines it
        : Init(o.Init), BankHandle(o.BankHandle), GraphicDataBank(o.GraphicDataBank) { }
};

void __fastcall _Construct(CGraphicDataBankInfo* p, const CGraphicDataBankInfo& value)
{
    if (p != 0)
        new (p) CGraphicDataBankInfo(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N009f8780;
    CCPPointerInfo info; info.RefCount = 1;
    CGraphicDataBankInfo src;
    src.Init.Words[0] = 1u;
    src.Init.Words[10] = 11u;
    src.BankHandle.m_pData = reinterpret_cast<void*>(17);
    src.GraphicDataBank.Data = reinterpret_cast<CGraphicDataBank*>(0x90);
    src.GraphicDataBank.Info = &info;
    CGraphicDataBankInfo* slot = reinterpret_cast<CGraphicDataBankInfo*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 1 && g_links == 0;
    if (!(slot->Init.Words[0] == src.Init.Words[0])) ok = false;
    if (!(slot->Init.Words[10] == src.Init.Words[10])) ok = false;
    if (!(slot->BankHandle.m_pData == src.BankHandle.m_pData)) ok = false;
    if (!(slot->GraphicDataBank.Data == src.GraphicDataBank.Data)) ok = false;
    if (!(slot->GraphicDataBank.Info == src.GraphicDataBank.Info)) ok = false;
    if (info.RefCount != 2) ok = false;   // the counted pointer copy took a reference
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 1 || g_links != 0) ok = false;
    if (!ok) { std::printf("009f8780_TEST FAIL\n"); return 1; }
    std::printf("009f8780_TEST PASS\n");
    return 0;
}