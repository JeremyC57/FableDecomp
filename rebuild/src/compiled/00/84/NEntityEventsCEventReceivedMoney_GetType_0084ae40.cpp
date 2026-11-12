// NEntityEvents::CEventReceivedMoney::GetType() const -- returns (EEventType)9
// Leaf: ?GetType@CEventReceivedMoney@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084AE40, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventReceivedMoney's vtable 0x01274210 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventReceivedMoney or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventReceivedMoney@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventReceivedMoney {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventReceivedMoney::GetType() const
{
    return (EEventType)9;
}
}
