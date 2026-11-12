// NCreatureMode::CModeSitChairBored::GetType() const -- returns 8
// Leaf: ?GetType@CModeSitChairBored@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00938160, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeSitChairBored's vtable 0x01292354 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeSitChairBored or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeSitChairBored@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeSitChairBored {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeSitChairBored::GetType() const
{
    return 8;
}
}
