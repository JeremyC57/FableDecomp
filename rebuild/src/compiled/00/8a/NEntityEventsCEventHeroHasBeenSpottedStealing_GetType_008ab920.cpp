// NEntityEvents::CEventHeroHasBeenSpottedStealing::GetType() const -- returns (EEventType)0x15
// Leaf: ?GetType@CEventHeroHasBeenSpottedStealing@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB920, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHeroHasBeenSpottedStealing's vtable 0x012781E0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHeroHasBeenSpottedStealing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHeroHasBeenSpottedStealing@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHeroHasBeenSpottedStealing {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHeroHasBeenSpottedStealing::GetType() const
{
    return (EEventType)0x15;
}
}
