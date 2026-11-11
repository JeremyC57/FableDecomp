// NCreatureMode::CModeScorpionKingExhausted::GetType() const -- returns 0x38
// Leaf: ?GetType@CModeScorpionKingExhausted@NCreatureMode@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3800, thiscall, `ret`.
// Identity: slot 1 of NCreatureMode::CModeScorpionKingExhausted's vtable 0x012858A0 (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeScorpionKingExhausted or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CModeScorpionKingExhausted@NCreatureMode@@UBEKXZ; `ret` matches the signature.
namespace NCreatureMode {
class CModeScorpionKingExhausted {
public:
    virtual unsigned long GetType() const;
};
unsigned long CModeScorpionKingExhausted::GetType() const
{
    return 0x38;
}
}
