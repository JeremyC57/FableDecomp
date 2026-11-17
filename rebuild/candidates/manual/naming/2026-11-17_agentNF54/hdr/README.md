# NF54 header overlay

Each file here is a copy of the repo file with the same path under `rebuild/include/` plus `#ifdef` opt-ins only. With none of the
opt-ins defined, every file preprocesses token-identically to the repo copy (re-diffed against the repo before the final recheck).
Apply by copying the opt-in blocks into the repo headers.

| Header | Opt-in | What it adds | Rows |
|---|---|---|---|
| `engine/NGameText_CDataBank.h` | `FABLE_RETAIL_NGAMETEXT_TEXTENTRY_SPEECH` (only without `..._GROUPTEXT`) | `CTextEntry` gains `_pad_0x08[0x14]`, `CCharString SpeechBank` (+0x1C), `Narrator` (+0x20), `SoundName` (+0x24), the Ego_r.pdb layout | 0x0065C781 |
| `engine/CPersistTraits.h` | `FABLE_RETAIL_CPERSISTTRAITS_CFLOATRANGE_BINARYOUT_FORCEINLINE` | `FABLE_FORCEINLINE` before `CPersistTraits<CFloatRange>::TransferBinaryOut` (retail expands it in the Transfer instance) | 0x00458F2B |
| `engine/CPersistTraits.h` | `FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT` | `CPersistTraits() : DefaultVal(T()) {}` instead of `DefaultVal()`. The `#else` keeps the original. It works around VC7.1 C2065 on value-initialising a class-nested enum | 0x0049F4C0 |
| `engine/CWorld.h` | `FABLE_RETAIL_CWORLD_NESTED_ESAVETYPE` | `#ifndef` around the global `enum ESaveType;`, and `enum ESaveType { };` nested in `class CWorld` (both PDBs spell `CWorld::ESaveType`) | 0x004A21F0, 0x0049F4C0, 0x004AB7F0 |
| `boost/scoped_ptr.h` | `FABLE_RETAIL_SCOPED_PTR_CHECKED_DELETE_FASTCALL` (only without `..._RESET`) | Declares `template<class T> void FABLE_FASTCALL checked_delete(T*)` and `~scoped_ptr() { checked_delete(ptr); }`: the out-of-line /Gr `checked_delete<CScriptConversationManager>` 0x004AE810 | 0x004A6AB0 |
| `engine/CWorldMap.h` | `FABLE_RETAIL_CWORLDMAP_LEVELHEADER` | Declarations of `unsigned long GetLevelStaticMapChecksum(const CCharString&) const` (public, 0x00506C40) and `C2DBoxI GetLevelDimensions(CAFile&) const` (protected, 0x00506BB0), both from Ego_r.pdb and FableWin.pdb | 0x00506BB0, 0x00506C40 |
| `engine/CAnimInit.h` | `FABLE_RETAIL_CANIMINIT_CTOR_OUTOFLINE` (nested in `..._CTORS`) | The argument constructor `CAnimInit(unsigned char, unsigned long, long, EAnimUpdateMode, bool, unsigned long, long)` declaration only; the `#else` keeps the inline body. Retail emits it out of line at 0x005B33F4 | 0x005B33F4, 0x00728FF0, 0x00862780 |
| `stlport/fable_containers.h` | `FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE` | `#include "fable_compiler.h"`, and the two `#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES` lines in front of the range `__destroy` / `_Destroy` wrappers become `#if defined(FABLE_RETAIL_AUGMENTATION_PARTICLES) \|\| defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE)`. So the wrappers are `FABLE_FORCEINLINE`, and /Os still calls only `__destroy_aux` | 0x005BC0D3 |

Notes:
- The `fable_containers.h` change rewrites two preprocessor condition lines. With the new macro undefined, the condition has the
  same value as before, so the preprocessed text is unchanged.
- No overlay touches a row-visible default.
