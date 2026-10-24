// NCameraModes::CZTargetNew::CZTargetNewInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate and construct a Z-targeting camera mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873200, 74 bytes, `ret 8`.
// Leaf: ?Alloc@CZTargetNewInit
// Name: vtable 0x0125C264 slot 0; FableWin.pdb ?Alloc@CZTargetNewInit@CZTargetNew@NCameraModes@@UBEPAVCBase@3@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z.
// Callees: operator new @ 0x00BFEA1A (0xf0 bytes, the retail CZTargetNew size), NCameraModes::CBase::CBase @ 0x0084E420,
//          CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Shape: CZTargetNew's inline constructor: root ctor, its vtable 0x0127754C, then the first member, an empty
//        CIntelligentPointer<const CThingGameObject> at +0xa4 (base ctor + its vtable 0x01237F1C); null block returns 0.
struct CGameCameraManager;
struct CCameraModeDef;
struct CBaseObject;
template <class T> struct CDefPointer;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() { }
};
namespace NCameraModes {
struct CBase {
    virtual void v00();
    float         DefaultFOV;                      // +0x04
    unsigned char _body[0x9c];                     // +0x08..+0xa3 (retail)
    CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
};
struct CZTargetNew : CBase {                       // vtable 0x0127754C, 0xf0 bytes
    CIntelligentPointer PTargetedThing;            // +0xa4 (Ego_r.pdb name)
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