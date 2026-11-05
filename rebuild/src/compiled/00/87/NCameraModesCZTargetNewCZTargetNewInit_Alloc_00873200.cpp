// NCameraModes::CZTargetNew::CZTargetNewInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate and construct a Z-targeting camera mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873200, 74 bytes, `ret 8`.
// Leaf: ?Alloc@CZTargetNewInit@CZTargetNew@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Name: vtable 0x0125C264 slot 0; FableWin.pdb ?Alloc@CZTargetNewInit@CZTargetNew@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: operator new @ 0x00BFEA1A (0xf0 bytes, the retail CZTargetNew size), NCameraModes::CBase::CBase @ 0x0084E420,
//          CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Shape: CZTargetNew's inline constructor: root ctor, its vtable 0x0127754C, then the first member, an empty
//        CIntelligentPointer<const CThingGameObject> at +0xa4 (base ctor + its vtable 0x01237F1C); null block returns 0.
class CGameCameraManager;
class CCameraModeDef;
class CBaseObject;
template <class T> class CDefPointer;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    float         DefaultFOV;                      // +0x04
    unsigned char _body[0x9c];                     // +0x08..+0xa3 (retail)
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};
struct CZTargetNew : CBase {                       // vtable 0x0127754C, 0xf0 bytes
    CIntelligentPointer<const CThing> PTargetedThing;            // +0xa4 (Ego_r.pdb name)
    unsigned char       _body[0xf0 - 0xac];
    virtual void v00();
    CZTargetNew(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) : CBase(manager, def) { }
    struct CZTargetNewInit {
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;
    };
};
CBase* CZTargetNew::CZTargetNewInit::Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const
{
    return new CZTargetNew(manager, def);
}
}