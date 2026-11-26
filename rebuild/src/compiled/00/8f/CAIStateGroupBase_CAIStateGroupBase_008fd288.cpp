// CAIStateGroupBase::~CAIStateGroupBase() -- the implicit destructor: destroys Name (CCharString at +0x0C), then chains
// to the base CBaseClass destructor (tail jump); no vtable store (retail: push esi ; mov esi,ecx ; lea ecx,[esi+0Ch] ;
// call 0x0099EAE0 ; mov ecx,esi ; pop esi ; jmp 0x0099A2E0).
// Leaf: ??1CAIStateGroupBase@@UAE@XZ
// Retail: Fable.exe @ 0x008FD288, 19 bytes. Ego_r.pdb and FableWin.pdb type records flag ~CAIStateGroupBase `public virtual
//   compiler-generated` (controls ~CBaseClass / ~CCharString are not); FableWin.pdb puts it, the base ctor and 214 CAIStateGroup_* ctors
//   / dtors in one object (debug\ai_brain_tables.obj), and retail keeps it in that TU's COMDAT run after the base ctor 0x008FD255.
// TU (NF469, r467a; was a strong out-of-line definition on a local __declspec(novtable) model): VC7.1 emits the implicit destructor as a
//   pick-any COMDAT in every TU that destroys a CAIStateGroupBase out of line. 0x008FE862 ??0CAIStateGroup_ApprenticeHero@@QAE@XZ, a ctor of
//   the same retail TU on the shared header, emits it byte- and relocation-equal to retail, so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern). The base ctor 0x008FD255
//   cannot own it: with one caller in the TU VC7.1 inlines it into ??_GCAIStateGroupBase (NF469 R467A.md).
#include "../src/compiled/00/8f/CAIStateGroupApprenticeHero_CAIStateGroupApprenticeHero_008fe862.cpp"
