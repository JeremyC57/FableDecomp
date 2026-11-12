// NEntityEvents::CEventAttackedWithBareHandsWithoutHittingAnything::GetType() const -- returns (EEventType)0x1f
// Leaf: ?GetType@CEventAttackedWithBareHandsWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085A450, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventAttackedWithBareHandsWithoutHittingAnything's vtable 0x0127649C (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventAttackedWithBareHandsWithoutHittingAnything or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventAttackedWithBareHandsWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventAttackedWithBareHandsWithoutHittingAnything {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventAttackedWithBareHandsWithoutHittingAnything::GetType() const
{
    return (EEventType)0x1f;
}
}
