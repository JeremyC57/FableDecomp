// NEntityEvents::CEventSoldItem::GetType() const -- returns (EEventType)0xc
// Leaf: ?GetType@CEventSoldItem@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811CF0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventSoldItem's vtable 0x01272378 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventSoldItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventSoldItem@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventSoldItem {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventSoldItem::GetType() const
{
    return (EEventType)0xc;
}
}
