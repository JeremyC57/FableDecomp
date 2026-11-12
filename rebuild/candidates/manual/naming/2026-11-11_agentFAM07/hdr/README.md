# agentFAM07 header overlay

Full copies of the current repo headers with `#ifdef`/`#ifndef` opt-in additions only; with no opt-in macro defined each
header compiles exactly as the repo copy (only comments and guarded lines added). Grade with `FABLE_HDR_OVERLAY=<this dir>`.

| Header | Opt-in macro | Effect | Used by |
|---|---|---|---|
| engine/NUISystem_CList.h | `FABLE_RETAIL_CLIST_CLEARPREVIOUSSELECTED` | declares `public: static void __fastcall ClearPreviousSelected();` (FableWin.pdb `?ClearPreviousSelected@CList@NUISystem@@SIXXZ`) | 0x0048573D |
| engine/CArenaRoundDef.h | `FABLE_RETAIL_CARENAROUNDDEF_IMPLICIT_DTOR` | drops the user-declared `virtual ~CArenaRoundDef();` so the compiler-generated dtor is used (it stores no vtable of its own; retail sdd 0x00F25B40) | 0x00F25B40 |
| engine/CHeroSoulsRoundDef.h | `FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR` | same for CHeroSoulsRoundDef (retail sdd 0x00F26DF0) | 0x00F26DF0 |
| engine/CHeroSoulsWaveDef.h | `FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR` | same for CHeroSoulsWaveDef (retail sdd 0x00F26F40) | 0x00F26F40 |
| engine/CHeroSoulsWaveDef.h | `FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS` | declares the def virtuals Copy / GetSizeofClass / Transfer (Ego_r.pdb names; vtable 0x012FB498) so the class is not abstract | 0x00F26F40, 0x00F26DF0 (vector<CHeroSoulsWaveDef> needs a concrete T) |

Evidence for IMPLICIT_DTOR (precedent: `FABLE_RETAIL_CTCINTERESTINGTOVILLAGERS_IMPLICIT_DTOR`, family K050): each retail scalar
deleting destructor calls the member vector's destructor first and only then stores the CDefPointeeBase vptr 0x01230BA0 before
~CResource 0x009FC550. A user-declared destructor would store the class's own vtable at entry, before the out-of-line member
destructor call, which retail does not do.
