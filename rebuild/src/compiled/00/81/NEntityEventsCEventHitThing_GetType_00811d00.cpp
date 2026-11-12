// NEntityEvents::CEventHitThing::GetType() const -- returns (EEventType)0x1d
// Leaf: ?GetType@CEventHitThing@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811D00, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHitThing's vtable 0x01272390 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHitThing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHitThing@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHitThing {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHitThing::GetType() const
{
    return (EEventType)0x1d;
}
}
