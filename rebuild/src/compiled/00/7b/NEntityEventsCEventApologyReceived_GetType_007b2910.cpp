// NEntityEvents::CEventApologyReceived::GetType() const -- returns (EEventType)4
// Leaf: ?GetType@CEventApologyReceived@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B2910, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventApologyReceived's vtable 0x0126D7D4 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventApologyReceived or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventApologyReceived@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventApologyReceived {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventApologyReceived::GetType() const
{
    return (EEventType)4;
}
}
