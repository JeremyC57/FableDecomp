// NEntityEvents::CEventTalkedToAnyone::GetType() const -- returns (EEventType)0x21
// Leaf: ?GetType@CEventTalkedToAnyone@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B140, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventTalkedToAnyone's vtable 0x01274270 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventTalkedToAnyone or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventTalkedToAnyone@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventTalkedToAnyone {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventTalkedToAnyone::GetType() const
{
    return (EEventType)0x21;
}
}
