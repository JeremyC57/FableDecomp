// NEntityEvents::CEventItemUsed::GetType() const -- returns (EEventType)5
// Leaf: ?GetType@CEventItemUsed@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0790, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventItemUsed's vtable 0x0125C820 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventItemUsed or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventItemUsed@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventItemUsed {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventItemUsed::GetType() const
{
    return (EEventType)5;
}
}
