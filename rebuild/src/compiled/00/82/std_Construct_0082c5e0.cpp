// std::_Construct(CMiniMapDisplay::CMarkerInfo*, const CMiniMapDisplay::CMarkerInfo&) -- placement copy of a minimap marker: the thing intelligent pointer (re-linked), graphic index, auto-marker flag
// Leaf: _Construct
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe @ 0x0082C5E0, 51 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (inline CIntelligentPointer<const CThing>
//   copy constructor, vtable 0x01237F1C).
// Layout: engine/CMarkerInfo.h (Ego_r.pdb, sizeof 0x10): PThing +0 (opaque 8 bytes, a CIntelligentPointer<CThing const>),
//   GraphicIndex +8, AutoMarker +0xC. The header keeps PThing opaque, so the pointer copy is placement-built on its bytes and the
//   two scalars are copied after it -- the same order as the implicit memberwise copy constructor.
#include "engine/CMarkerInfo.h"
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0082c5e0 {
struct CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CThingConstPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    virtual void v00();
    CThingConstPointer(const CThingConstPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
// STLport _Construct: placement-copy one element (null-checked placement new, then the memberwise copy)
void __fastcall _Construct(CMarkerInfo* p, const CMarkerInfo& value)
{
    if (p != 0) {
        new (p->PThing) CThingConstPointer(*reinterpret_cast<const CThingConstPointer*>(value.PThing));
        p->GraphicIndex = value.GraphicIndex;
        p->AutoMarker = value.AutoMarker;
    }
}
}