// NCreatureMode::CModeScorpionKingTailPlunge::GetType() const -- returns 0x2f
// Leaf: ?GetType@CModeScorpionKingTailPlunge@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3780, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScorpionKingTailPlunge's vtable 0x01285860 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScorpionKingTailPlunge or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScorpionKingTailPlunge@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScorpionKingTailPlunge {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScorpionKingTailPlunge::GetType() const
{
    return 0x2f;
}
}
