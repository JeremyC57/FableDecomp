// $E_VALUE_DISPLAY_DURATION() -- dynamic initializer of the anonymous-namespace float VALUE_DISPLAY_DURATION of the CTCOverheadDisplay TU: half a second in frames (0.5f * CMainGameComponent::GetConstantFPS())
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F220, 19 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374610, plain `ret`: `fild [ConstantFPS] ; fmul [0.5f] ; fstp [0x013BAE68]`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x01011A30 and FableWin.pdb 0x03FF56E0 run, in one TU,
//   `anonymous namespace'::`dynamic initializer for 'VALUE_DISPLAY_DURATION'' -> REACTION_MANAGER_DEF_INSTANTIATION_NAME, and then
//   `dynamic initializer for 'CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking'' (the next TU); retail's .CRT$XCU runs this
//   0x0121F220 -> 0x0121F240 (CCharString "OPINION_REACTION_MANAGER_INSTANCE") -> 0x0121F260 (the 0.1f / fps float), and Ego_r's
//   copy is 19 bytes like this one; (2) its only readers are the CTCOverheadDisplay members at 0x007B7A70 / 0x007B7C30
//   (AddDamageChange) / 0x007B7C80 / 0x007B7CD0 and the function at 0x007B74A0, which `fld [0x013BAE68] ; call __ftol2` it as the
//   frame count an overhead value change stays on screen (one `fdiv` at 0x007B75B9). The NPlayerGui::VALUE_DISPLAY_DURATION of both PDBs is another object (another TU).
//   Catalog row "_gapscan::sub_0121f220" carried no name; the landed 0x007B7A70 binds 0x013BAE68 as an invented
//   `?g_OverheadValueExpiryFrames@@3MA` (NAME_FIXES.tsv).
// Type: float (fstp dword, the readers' fld dword + __ftol2). `const` is the Lionhead upper-case constant convention: retail
//   never writes the object after this initializer (xref: this store, seven fld and one fdiv); no PDB data-symbol dump proves the qualifier.
// Constant: 0x0122F59C = 0.5f (const_check: 1 constant, 0 mismatches).
// Callees: none. Data: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA, protected static) read
//   through the inline static accessor GetConstantFPS() (Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ), as retail
//   code outside the class does (ESTABLISHED: naming wave 18; same model as the landed 0x0121F7D0 / 0x0121F800).
// Shape: an int static times a float constant: VC7.1 loads the long with `fild` straight from memory and multiplies by the pooled 0.5f.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

namespace {
const float VALUE_DISPLAY_DURATION = 0.5f * CMainGameComponent::GetConstantFPS();   // 0x013BAE68
}