// NEntityEvents::CEventHeroHasBeenSpottedPickingLock::GetType() const -- returns (EEventType)0x14
// Leaf: ?GetType@CEventHeroHasBeenSpottedPickingLock@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB890, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHeroHasBeenSpottedPickingLock's vtable 0x012781C8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHeroHasBeenSpottedPickingLock or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHeroHasBeenSpottedPickingLock@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHeroHasBeenSpottedPickingLock {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHeroHasBeenSpottedPickingLock::GetType() const
{
    return (EEventType)0x14;
}
}
