// NEntityEvents::CEventChildHurtGoToMother::GetType() const -- returns (EEventType)0x18
// Leaf: ?GetType@CEventChildHurtGoToMother@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00915700, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventChildHurtGoToMother's vtable 0x0128FA80 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventChildHurtGoToMother or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventChildHurtGoToMother@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventChildHurtGoToMother {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventChildHurtGoToMother::GetType() const
{
    return (EEventType)0x18;
}
}
