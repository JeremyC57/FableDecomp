// NEntityEvents::CEventExpressionPerformedTo::GetType() const -- returns (EEventType)3
// Leaf: ?GetType@CEventExpressionPerformedTo@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B070, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventExpressionPerformedTo's vtable 0x01274240 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventExpressionPerformedTo or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventExpressionPerformedTo@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventExpressionPerformedTo {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventExpressionPerformedTo::GetType() const
{
    return (EEventType)3;
}
}
