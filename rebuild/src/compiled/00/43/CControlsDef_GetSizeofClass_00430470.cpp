// Constant-return member: returns 0x50. __fastcall this=ecx.
// Leaf: ?GetSizeofClass@CControlsDef@@QAEHXZ
struct CControlsDef { int GetSizeofClass(); };
int CControlsDef::GetSizeofClass() { return 0x50; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetSizeofClass@CControlsDef@@UAEKXZ); evidence: RTTI vtable slot + PDB name set.
