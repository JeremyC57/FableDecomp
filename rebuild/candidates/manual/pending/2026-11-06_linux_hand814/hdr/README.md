# Chunk 814 header overlays

All four are additive. Each existing header was copied fresh from `rebuild/include/engine/` and re-diffed just before hand-back. Each edit
is a new `#ifdef` opt-in block: the forward declarations are inside it, existing lines are not reflowed, and `diff` shows only `>` lines.
The default path is therefore textually unchanged. No landed includer can change bytes, so no `INCLUDERS_*.txt` recheck was needed.

## engine/CV_ChapelOfEvilScript.h (+26 lines, opt-in `FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL`)
- **Adds:** the nested `CV_ChapelOfEvilScript::CCOE_AcolyteEvil : CEntityScriptBase`:
  - ParentClass `CV_ChapelOfEvilScript*` +0x14, PMasterData +0x18, `CTimer AcolytePrayerTimer` +0x1c.
  - Alloc / GetParentScript / Main / Init / OnPersist / OnPredicateFail.
  - Size assert 0x20.
- **Evidence:** the Ego_r.pdb layout (struct_layouts_egor.tsv); entity vtable 0x012E8468, 6 slots, stored by Alloc 0x00E6A500; the landed
  Init 0x00E6AAC0 and Main 0x00E6BC50 offsets. Both PDB name sets hold the members.
- **Model:** chunk 811's CTOL_AcolyteGood twin.
- **Includers:** 1 landed (default path, untouched). Used by differ/00e6bc50.cpp.

## engine/CV_AmbushScamScript.h (+15 lines, opt-in `FABLE_RETAIL_CV_AMBUSHSCAMSCRIPT_BUDDY`)
- **Adds:** `#include "engine/C3DVector.h"`, `#pragma pack(push,1)` / `pack(pop)` around CAS_Buddy, and `C3DVector BuddyStartPos` at +0x1f.
- **Evidence:** Ego_r.pdb offset 31; CAS_Buddy::Main 0x00E2FF00 reads three dwords at +0x1f/+0x23/+0x27 for SetWanderCentrePoint; the
  landed Alloc 0x00E2BEC0 models the 13-byte tail.
- **Includers:** 10 landed (default path, untouched). Used by differ/00e2ff00.cpp.

## engine/CQ_ArenaHoldingScriptScript.h (+25 lines, opt-in `FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ENTRANCEGUARD`)
- **Adds:** `class CPersistContext;` and `CQ_ArenaHoldingScriptScript::CEntranceGuard : CEntityScriptBase`, containing:
  - Alloc 0x00CF0B60 and GetParentScript 0x00CF0C00;
  - `CQ_ArenaHoldingScriptScript& ParentClass` +0x14 and PMasterData +0x18 (protected);
  - bool IntroDone +0x1c and DoneArena +0x1d (private);
  - Main / Init / OnPersist;
  - size assert 0x20.
- **Evidence:** entity vtable 0x012C6158 (0 ??_G 0x00CF0C20, 1 Main 0x00CF68C0, 2 Init 0x00CF0C10, 3 GetParentScript 0x00CF0C00, 4 OnPersist
  0x00CF6890); the Ego_r layout; the bools that the landed Init clears.
- **Includers:** 0 landed. Used by differ/00cf68c0.cpp.

## engine/CQ_GuildTrainingWoodsDepartureScript.h (NEW, behind `FABLE_RETAIL_NSCRIPT`, no #error branch)
- **Starting point:** chunk 810's pending draft (`rebuild/candidates/manual/pending/2026-11-06_linux_hand810/hdr/engine/`), copied.
- **Change:** the draft's forward declaration `class CFinalMaze;` is replaced by the full nested class (36 bytes), plus a size assert of 0x24:
  - CEntityScriptBase 0x14;
  - ParentClass* +0x14, PMasterData +0x18;
  - NotFighting +0x1c, NotBeaten +0x1d, BeenHit +0x20.
- **Evidence:** the Ego_r layout; the Alloc 0x00D62270 new size; entity vtable 0x012D16C0 (Main 0x00D647F0, Init 0x00D62320, GetParentScript
  0x00D62310, OnPredicateFail 0x00D62330); Main writes +0x1c/+0x1d/+0x20.
- **Merge note:** the file differs from 810's draft only by this class and its assert (plus the line-3 credit). Whoever lands second should
  merge, not overwrite. Used by differ/00d647f0.cpp.
