// NCameraModes::CZTargetNew::CZTargetNewInit::Alloc(CGameCameraManager&, const CDefPointer<const CCameraModeDef>&) const -- allocate and construct a Z-targeting camera mode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873200, 74 bytes, `ret 8`.
// Leaf: ?Alloc@CZTargetNewInit
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
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
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