// Constant-return member: returns 0x54. __fastcall this=ecx.
// Leaf: ?GetSizeofClass@CFrontEndDef@@QAEHXZ
struct CFrontEndDef { int GetSizeofClass(); };
int CFrontEndDef::GetSizeofClass() { return 0x54; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetSizeofClass@CFrontEndDef@@UAEKXZ); evidence: RTTI vtable slot + PDB name set.
