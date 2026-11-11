// NEntityEvents::CEventVillagerInteraction::GetType() const -- returns (EEventType)0x19
// Leaf: ?GetType@CEventVillagerInteraction@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00982380, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventVillagerInteraction's vtable 0x012995A8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventVillagerInteraction or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventVillagerInteraction@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventVillagerInteraction {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventVillagerInteraction::GetType() const
{
    return (EEventType)0x19;
}
}
