// NEntityEvents::CEventFiredShot::GetType() const -- returns (EEventType)0x26
// Leaf: ?GetType@CEventFiredShot@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BFE60, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventFiredShot's vtable 0x0127F2C0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventFiredShot or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventFiredShot@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventFiredShot {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventFiredShot::GetType() const
{
    return (EEventType)0x26;
}
}
