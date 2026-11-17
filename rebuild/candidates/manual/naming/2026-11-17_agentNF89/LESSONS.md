# NF89 lessons (one line each)
- A landed row may already carry a header-based same-TU body of your target (0x005A4CDD carries InitialiseBuyableItems); start a port there, but it is ungraded and may not byte-match.
- Before porting an introsort loop, list its partition / median / swap callees' own leaves (oldprobe): the partitions here were PAV stand-ins while the swaps were already right.
- An implicit destructor called out of line in one retail function is an inline-budget split: sweep /d2inlT DOWN from 74 and take the largest matching value.
- An opt-in placed inside another opt-in's block (CTCInventoryQuests MEMBERS) is invisible without that macro: put non-layout declarations right after the class head.
- WORKLIST binder claims go stale: probe every listed binder first (two of 23 here already bound the PDB spelling).
- A stand-in caller often names a real PDB member under a made-up name (0x005BD328 GetNumberOfSlotsInCategory = GetNumberOfSlotsForCategory): grep the PDB sets by signature before keeping a descriptive name.
- When a binder's port seems to need a header class that does not exist yet, do not block on the probe. Look for a sibling agent's overlay or quarantine header (CInventoryCategoryDef: NF90's) or add the retail-class opt-in yourself (CResurrectionItemDef over CSubDefClassBase, from rtti_bases.tsv), then grade real forms. Both rows got there: one matched, one is down to a one-definition residue.
- Never put backticks in a double-quoted note: write notes with a quoted heredoc and pass them through python.
- The PASS regex in a vland log must allow several spaces after MATCH (`MATCH            PASS`).
- A by-value `GetPDef()->Flag` read that grades with only the Release `dec [def+4]` scheduled before the flag load (retail loads first):
  define the existing `FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE` (engine/CDefPointer.h). It keeps the may-alias edge that orders the
  load first. 0x005BD054: DIFFER(163v163) -> RELOCATION_MATCH with that one define.
- For a row calling `CDefinitionManager::GetDef<T>(long, ...)` / `CDefClassBase::GetSubDef<T>`, the header template bodies
  (`..._GETDEF_GLOBAL_BODY`, `..._GETSUBDEF_BODY`, both noinline) are what retail's TU saw. With them VC7.1 knows the out-handle stays
  local (0x005BF77D: 359 -> 324 bytes against 334). Include CThingBaseDef.h (which pulls CDefClassBase.h) BEFORE any header that
  includes CDefinitionManager.h, or the GetSubDef body fails with C2039 "not a member".
