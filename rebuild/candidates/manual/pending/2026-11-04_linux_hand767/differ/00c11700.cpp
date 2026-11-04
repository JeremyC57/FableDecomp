// CUnproven_00c11700::GetFaderOutput() const -- libpk forwarder: the output of the fader embedded at +0x7D8 (CFader::GetOutput, tail call)
// Grade: DIFFER(name) -- name unproven: libpk has no PDB / RTTI; the only landed spelling bound to this address is the placeholder ?cons_ctor@@YINPAUPair@@@Z (a free __fastcall function, wrong shape)
// Leaf: ?GetFaderOutput@CUnproven_00c11700@@QBEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11700, 11 bytes: `add ecx,7D8h ; jmp 0x00C25F00` (the landed ?GetOutput@CFader@@QBEMXZ). Caller
//   0x00C271E0 (landed as global Cons_val, a propagated label). The bytes reproduce under this placeholder; the owner class and the
//   member name are unknown, so the row is parked (one-source names are not applied).
// Next idea: name the owner from the caller 0x00C271E0's real class (the panel / sound-event object whose +0x7D8 is a CFader), then
//   respell ?cons_ctor@@YINPAUPair@@@Z in that landed caller.
class CFader { public: float GetOutput() const; };        // 0x00C25F00
class CUnproven_00c11700 {
public:
    float GetFaderOutput() const;
private:
    unsigned char Unmodeled_0x000[0x7d8];
    CFader Fader;                                         // +0x7d8
};
float CUnproven_00c11700::GetFaderOutput() const
{
    return Fader.GetOutput();
}
