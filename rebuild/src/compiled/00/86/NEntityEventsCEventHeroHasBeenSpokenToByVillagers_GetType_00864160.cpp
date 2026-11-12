// NEntityEvents::CEventHeroHasBeenSpokenToByVillagers::GetType() const -- returns (EEventType)0x12
// Leaf: ?GetType@CEventHeroHasBeenSpokenToByVillagers@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00864160, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventHeroHasBeenSpokenToByVillagers's vtable 0x01276E00 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventHeroHasBeenSpokenToByVillagers or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventHeroHasBeenSpokenToByVillagers@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventHeroHasBeenSpokenToByVillagers {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventHeroHasBeenSpokenToByVillagers::GetType() const
{
    return (EEventType)0x12;
}
}
