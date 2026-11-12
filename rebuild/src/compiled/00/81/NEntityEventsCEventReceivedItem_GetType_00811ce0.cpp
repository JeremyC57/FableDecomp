// NEntityEvents::CEventReceivedItem::GetType() const -- returns (EEventType)0xa
// Leaf: ?GetType@CEventReceivedItem@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811CE0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventReceivedItem's vtable 0x01272360 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventReceivedItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventReceivedItem@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventReceivedItem {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventReceivedItem::GetType() const
{
    return (EEventType)0xa;
}
}
