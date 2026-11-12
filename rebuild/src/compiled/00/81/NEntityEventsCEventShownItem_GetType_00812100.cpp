// NEntityEvents::CEventShownItem::GetType() const -- returns (EEventType)0xb
// Leaf: ?GetType@CEventShownItem@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00812100, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventShownItem's vtable 0x012723D8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventShownItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventShownItem@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventShownItem {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventShownItem::GetType() const
{
    return (EEventType)0xb;
}
}
