// NEntityEvents::CEventHeroHasBeenSpottedPickpocketing::GetType() const -- returns (EEventType)0x13
// Leaf: ?GetType@CEventHeroHasBeenSpottedPickpocketing@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB800, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHeroHasBeenSpottedPickpocketing's vtable 0x012781B0 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHeroHasBeenSpottedPickpocketing or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHeroHasBeenSpottedPickpocketing@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHeroHasBeenSpottedPickpocketing {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHeroHasBeenSpottedPickpocketing::GetType() const
{
    return (EEventType)0x13;
}
}
