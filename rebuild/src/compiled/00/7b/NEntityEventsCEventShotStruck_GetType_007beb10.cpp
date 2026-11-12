// NEntityEvents::CEventShotStruck::GetType() const -- returns (EEventType)0x27
// Leaf: ?GetType@CEventShotStruck@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BEB10, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventShotStruck's vtable 0x0126E188 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventShotStruck or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventShotStruck@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventShotStruck {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventShotStruck::GetType() const
{
    return (EEventType)0x27;
}
}
