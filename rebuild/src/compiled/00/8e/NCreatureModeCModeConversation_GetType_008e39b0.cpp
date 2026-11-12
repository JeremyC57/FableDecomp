// NCreatureMode::CModeConversation::GetType() const -- returns 0x1b
// Leaf: ?GetType@CModeConversation@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E39B0, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeConversation's vtable 0x01285990 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeConversation or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeConversation@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeConversation {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeConversation::GetType() const
{
    return 0x1b;
}
}
