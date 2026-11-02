// NPlayerGui::`dynamic initializer for 'VALUE_DISPLAY_DURATION''() -- set the GUI value-display duration (a float) to one second's worth of frames, (float)CMainGameComponent::ConstantFPS
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121DF1C, 13 bytes (`fild [ConstantFPS] ; fstp [0x013BA860] ; ret`), .CRT$XCU entry
//   0x013743F8; it starts right after the `ret` of the atexit-only initializer 0x0121DF10 (same TU .text$yc piece, no padding:
//   a real start) and is followed by int3 padding and the next TU's REACTION_MANAGER_DEF_INSTANTIATION_NAME initializer 0x0121DF30.
// Identity (proven): Ego_r.pdb 0x01010A15 and FableWin.pdb 0x03FE4B5E both name `NPlayerGui::`dynamic initializer for
//   'VALUE_DISPLAY_DURATION''; in Ego_r it is likewise unaligned and immediately followed by the
//   `anonymous namespace'::REACTION_MANAGER_DEF_INSTANTIATION_NAME initializer (0x01010A30), the retail order. The float's readers are
//   NPlayerGui code (CDrawDamage::AddDamageChange 0x006472B2: a damage value shown for one second's worth of frames; the
//   CDrawPerceivers ctor 0x0064ED06). The catalog row was sub_0121df1c; the landed AddDamageChange binds the float as the stand-in
//   g_fFramesPerSecond_013ba860 (NAME_FIXES.tsv).
// Constants: no float pool; 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (ESTABLISHED, protected static long) read through
//   the PDB inline accessor GetConstantFPS() (naming wave 18; the landed g_scaled initializers 0x0121E3D0 ff. use the same model).
// Linkage: VALUE_DISPLAY_DURATION is declared non-const here so the reader rows in other sources can bind it; whether retail spelled
//   it `const float` (internal, `_VALUE_DISPLAY_DURATION`) is unproven (same bytes: probe 2) -- coordinator decision in NOTES.md.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace NPlayerGui {
float VALUE_DISPLAY_DURATION = (float)CMainGameComponent::GetConstantFPS();   // 0x013BA860
}