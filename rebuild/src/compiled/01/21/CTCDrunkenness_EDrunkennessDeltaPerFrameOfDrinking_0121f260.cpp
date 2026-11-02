// CTCDrunkenness::$E_DrunkennessDeltaPerFrameOfDrinking() -- dynamic initializer of the static float CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking: the drunkenness a drinking creature gains per frame (0.1f / CMainGameComponent::GetConstantFPS())
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121F260, 19 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374618, plain `ret`: `fild [ConstantFPS] ; fdivr [0.1f] ; fstp [0x013BAE70]`.
// Identity (proven, two sources): (1) Ego_r.pdb 0x01011A70 and FableWin.pdb 0x03FF5740 name
//   `dynamic initializer for 'CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking'' right after the VALUE_DISPLAY_DURATION /
//   REACTION_MANAGER_DEF_INSTANTIATION_NAME TU and before CTCOracleMinigame::SymbolStrings; retail runs 0x0121F220 (VALUE_DISPLAY_
//   DURATION) -> 0x0121F240 ("OPINION_REACTION_MANAGER_INSTANCE") -> this -> 0x0121F280 (the CTCOracleMinigame string table) ->
//   0x0121F2D0 (CTCFireheartMinigame::BlockNames); Ego_r's copy is 19 bytes like this one; (2) its readers multiply drinking frames
//   by it and add the product to a drunkenness float: 0x0093A4E0 (`fild [esp+4] ; fmul [0x013BAE70] ; fadd [ecx+10h] ; fstp`, an
//   out-of-line CTCDrunkenness adder) and CAIStateGroup_BanditSitRoundCampfire::Drink 0x0093A8C0 (0x0093AA07).
//   Catalog row "_gapscan::sub_0121f260" carried no name; the landed 0x0093A8C0 binds 0x013BAE70 as an invented
//   `?g_DrunkennessPerFrame@@3MA` (NAME_FIXES.tsv).
// Data symbol: ?DrunkennessDeltaPerFrameOfDrinking@CTCDrunkenness@@2MB -- public (read from the AI state group TU), static float
//   (fstp / fmul dword); `const` by convention (retail never writes it after this initializer); no PDB data-symbol dump proves
//   access or qualifier. CTCDrunkenness is declared here for its static only (no instance, no member access).
// Constant: 0x0126DEAC = 0.1f (const_check: 1 constant, 0 mismatches).
// Data: CMainGameComponent::ConstantFPS 0x01375550 (?ConstantFPS@CMainGameComponent@@1JA) through the inline GetConstantFPS() (naming wave 18).
// Shape: `0.1f / fps` -- the long is loaded first (`fild`), so the constant becomes the reversed divide's memory operand (`fdivr`).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCDrunkenness {                                   // class key V (pdb_class_keys.tsv)
public:
    static const float DrunkennessDeltaPerFrameOfDrinking;   // 0x013BAE70
};

const float CTCDrunkenness::DrunkennessDeltaPerFrameOfDrinking = 0.1f / CMainGameComponent::GetConstantFPS();