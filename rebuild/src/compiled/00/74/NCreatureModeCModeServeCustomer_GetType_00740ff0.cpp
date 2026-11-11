// NCreatureMode::CModeServeCustomer::GetType() const -- returns 0x2a
// Leaf: ?GetType@CModeServeCustomer@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740FF0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeServeCustomer's vtable 0x012680E8 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeServeCustomer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeServeCustomer@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeServeCustomer {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeServeCustomer::GetType() const
{
    return 0x2a;
}
}
