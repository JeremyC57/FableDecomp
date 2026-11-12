// NEntityEvents::CEventTalkedTo::GetType() const -- returns (EEventType)2
// Leaf: ?GetType@CEventTalkedTo@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084AFD0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventTalkedTo's vtable 0x01274228 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventTalkedTo or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventTalkedTo@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventTalkedTo {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventTalkedTo::GetType() const
{
    return (EEventType)2;
}
}
