#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test copy of the 0x0082C5E0 model: CMarkerInfo as engine/CMarkerInfo.h lays it out (PThing opaque bytes, GraphicIndex,
// AutoMarker), with PThing sized for the host pointer width so the placement-built intelligent pointer fits.
static int g_links = 0;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; ++g_links; }
};
struct CThingConstPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CThingConstPointer() { }
    CThingConstPointer(const CThingConstPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CMarkerInfo {
    union { unsigned char PThing[sizeof(CThingConstPointer)]; void* _align; };
    long GraphicIndex;
    bool AutoMarker;
};
void __fastcall _Construct(CMarkerInfo* p, const CMarkerInfo& value)
{
    if (p != 0) {
        new (p->PThing) CThingConstPointer(*reinterpret_cast<const CThingConstPointer*>(value.PThing));
        p->GraphicIndex = value.GraphicIndex;
        p->AutoMarker = value.AutoMarker;
    }
}
static double g_storage[16];
int main()
{
    CMarkerInfo src;
    CThingConstPointer* srcPtr = new (src.PThing) CThingConstPointer();
    srcPtr->m_pItem = reinterpret_cast<CBaseObject*>(0x44);
    g_links = 0;
    src.GraphicIndex = 31;
    src.AutoMarker = true;
    CMarkerInfo* slot = reinterpret_cast<CMarkerInfo*>(g_storage);
    std::memset(g_storage, 0, sizeof(g_storage));
    _Construct(slot, src);
    const CThingConstPointer* got = reinterpret_cast<const CThingConstPointer*>(slot->PThing);
    bool ok = g_links == 1 && got->GetPItem() == reinterpret_cast<CBaseObject*>(0x44) && slot->GraphicIndex == 31 && slot->AutoMarker;
    _Construct(0, src);
    if (g_links != 1) ok = false;
    if (!ok) { std::printf("0082c5e0_TEST FAIL\n"); return 1; }
    std::printf("0082c5e0_TEST PASS\n");
    return 0;
}