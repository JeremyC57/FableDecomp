// NEntityEvents::CEventKilledCreature::GetType() const -- returns (EEventType)0x22
// Leaf: ?GetType@CEventKilledCreature@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00811EB0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventKilledCreature's vtable 0x012723A8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventKilledCreature or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventKilledCreature@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventKilledCreature {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventKilledCreature::GetType() const
{
    return (EEventType)0x22;
}
}
