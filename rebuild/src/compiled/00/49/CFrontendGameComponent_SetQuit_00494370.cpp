// Byte-flag setter at this+8: `mov byte ptr [ecx+8],1; ret`. __fastcall this=ecx.
// Leaf: ?SetQuit@CFrontendGameComponent@@QAEXXZ
struct CFrontendGameComponent { char pad[8]; char quit; void SetQuit(); };
void CFrontendGameComponent::SetQuit() { this->quit = 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetQuit@CFrontendGameComponent@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
