// NEntityEvents::CEventOpenedChest::GetType() const -- returns (EEventType)0x1c
// Leaf: ?GetType@CEventOpenedChest@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084B0C0, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventOpenedChest's vtable 0x01274258 (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventOpenedChest or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventOpenedChest@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventOpenedChest {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventOpenedChest::GetType() const
{
    return (EEventType)0x1c;
}
}
