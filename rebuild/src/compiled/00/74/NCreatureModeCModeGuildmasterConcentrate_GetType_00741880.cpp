// NCreatureMode::CModeGuildmasterConcentrate::GetType() const -- returns 0x3b
// Leaf: ?GetType@CModeGuildmasterConcentrate@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741880, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeGuildmasterConcentrate's vtable 0x01268368 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeGuildmasterConcentrate or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeGuildmasterConcentrate@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeGuildmasterConcentrate {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeGuildmasterConcentrate::GetType() const
{
    return 0x3b;
}
}
