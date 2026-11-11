// NEntityEvents::CEventKillLocked::GetType() const -- returns (EEventType)0xd
// Leaf: ?GetType@CEventKillLocked@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087FF10, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventKillLocked's vtable 0x01277CA8 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventKillLocked or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventKillLocked@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventKillLocked {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventKillLocked::GetType() const
{
    return (EEventType)0xd;
}
}
