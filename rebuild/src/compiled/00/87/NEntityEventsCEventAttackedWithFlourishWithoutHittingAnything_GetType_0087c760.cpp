// NEntityEvents::CEventAttackedWithFlourishWithoutHittingAnything::GetType() const -- returns (EEventType)0x20
// Leaf: ?GetType@CEventAttackedWithFlourishWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087C760, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventAttackedWithFlourishWithoutHittingAnything's vtable 0x01277874 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventAttackedWithFlourishWithoutHittingAnything or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventAttackedWithFlourishWithoutHittingAnything@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventAttackedWithFlourishWithoutHittingAnything {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventAttackedWithFlourishWithoutHittingAnything::GetType() const
{
    return (EEventType)0x20;
}
}
