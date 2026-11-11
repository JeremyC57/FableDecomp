// NEntityEvents::CEventHitBy::GetType() const -- returns (EEventType)1
// Leaf: ?GetType@CEventHitBy@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083AF50, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHitBy's vtable 0x01272FA8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHitBy or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHitBy@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHitBy {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHitBy::GetType() const
{
    return (EEventType)1;
}
}
