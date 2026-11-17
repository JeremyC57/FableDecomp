// CTBaseSingleton<CFileInstaller>::Release() -- shut down the file installer singleton: drop the static counted pointer PClass (Reset(0) inline)
// Leaf: ?Release@?$CTBaseSingleton@VCFileInstaller@@@@SIXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404490, 57 bytes, plain `ret` (__fastcall, no arguments): the count block word 0x013CA81C
//   decremented, at zero DeleteFunc(Data) and delete 0x00BFE9BC, then Info (0x013CA81C) and Data (0x013CA818) nulled.
// Identity (two sources): Ego_r.pdb 0x00403CB0 + FableWin.pdb 0x01871060 ?Release@?$CTBaseSingleton@VCFileInstaller@@@@SIXXZ, right
//   after Get (Ego_r 0x00403C60 / retail 0x00404440: the same 0x50 gap); engine/CTBaseSingleton.h RELEASE opt-in names 0x00404490; the
//   landed caller 0x0042EC7C CNewFrontendGameComponent::Run binds this spelling. The data symbol 0x013CA818 is the private static
//   ?PClass@?$CTBaseSingleton@VCFileInstaller@@@@0V?$CCountedPointer@VCFileInstaller@@@@A (Ego_d.pdb PUB32, ghidra_out/egod_carved_names.tsv,
//   and the shared header's PClass member; NF76's 0x00599E3F binds it).
// NF79 (round 7, q10969): re-authored as the explicit instantiation of the header's Release (my overlay opt-in
//   CTBASESINGLETON_RELEASE_BODY); was the free _global::Release_013ca818 over a local CCountedPointerStatic g_pointer_013ca818 model.
#define FABLE_RETAIL_CTBASESINGLETON_RELEASE_BODY
#include "engine/CTBaseSingleton.h"
#include "engine/CFileInstaller.h"
template void FABLE_FASTCALL CTBaseSingleton<CFileInstaller>::Release();
