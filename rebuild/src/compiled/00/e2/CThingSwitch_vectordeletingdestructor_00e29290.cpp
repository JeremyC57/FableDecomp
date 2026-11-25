// CActiveEntityScript<NScript::CQS_MeetSisterScript>::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CActiveEntityScript<T>
//   (~CActiveEntityScriptBase expanded: Entity +0x48, PEntityScript +0x34, then ~CSpawnedFuncBase), then operator delete on flag bit 0
// Leaf: ??_G?$CActiveEntityScript@VCQS_MeetSisterScript@NScript@@@@UAEPAXI@Z
// Flags: /d2inlS20
// Inline-budget: /d2inlS20 + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY (defined in the included owner 0x00E29110) -- this ??_G expands the implicit ~CActiveEntityScriptBase as retail does; evidence in the owner's marker (17-19 fail, 85/120 at 20)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E29290, 48 bytes, thiscall, `ret 4`; slot 0 of the RTTI-less vftable 0x012E2268.
// Identity (NF424 r2 S0, V+P+C): (V) slot 4 of the same vftable, referenced only there, is the landed 4:00e29220;
//   (P) FableWin.pdb / Ego_r name sets: E:fwin,D:fwin; (C) the vptr store sits in the owner below,
//   CEntityScriptBinding<T>::CreateActiveEntityScript 0x00E29110 (engine/CActiveEntityScript.h: slot 0 = this implicit scalar deleting dtor).
// TU: VC7.1 emits this pick-any COMDAT beside ??_7 in the TU that stores the vptr: the owner, included with no code of its own
//   (nf159 TU-emitter fold, nf251 pattern). The owner carries the /d2inlS20 + DTOR_DECL_ONLY edit of this hand-back (owners/).
#include "../src/compiled/00/e2/CEntityScriptBindingNScriptCQSMeetSisterScript_CreateActiveEntityScript_00e29110.cpp"
