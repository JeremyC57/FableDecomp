// NEntityEvents::CEventDied::GetType() const -- returns (EEventType)6
// Leaf: ?GetType@CEventDied@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00669350, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventDied's vtable 0x0125AAF4 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventDied or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventDied@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventDied {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventDied::GetType() const
{
    return (EEventType)6;
}
}
