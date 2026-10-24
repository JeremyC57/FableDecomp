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
namespace N00a6fdc0 {
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
struct CContainmentVolume;
struct C3DMeshVolume {                           // PDB sizeof 0xC
    CCharString                         Name;          // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;  // +0x04
};
struct CNamedMeshVolume {                        // std::pair<CCharString, C3DMeshVolume>
    CCharString   first;                         // +0x00
    C3DMeshVolume second;                        // +0x04
};

void __fastcall _Construct(CNamedMeshVolume* p, const CNamedMeshVolume& value)
{
    if (p != 0)
        new (p) CNamedMeshVolume(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00a6fdc0;
    CCPPointerInfo info; info.RefCount = 1;
    CNamedMeshVolume src;
    src.first.m_pData = reinterpret_cast<void*>(17);
    src.second.Name.m_pData = reinterpret_cast<void*>(18);
    src.second.ConvexVolume.Data = reinterpret_cast<CContainmentVolume*>(0x50);
    src.second.ConvexVolume.Info = &info;
    CNamedMeshVolume* slot = reinterpret_cast<CNamedMeshVolume*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 2 && g_links == 0;
    if (!(slot->first.m_pData == src.first.m_pData)) ok = false;
    if (!(slot->second.Name.m_pData == src.second.Name.m_pData)) ok = false;
    if (!(slot->second.ConvexVolume.Data == src.second.ConvexVolume.Data)) ok = false;
    if (!(slot->second.ConvexVolume.Info == src.second.ConvexVolume.Info)) ok = false;
    if (info.RefCount != 2) ok = false;   // the counted pointer copy took a reference
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 2 || g_links != 0) ok = false;
    if (!ok) { std::printf("00a6fdc0_TEST FAIL\n"); return 1; }
    std::printf("00a6fdc0_TEST PASS\n");
    return 0;
}