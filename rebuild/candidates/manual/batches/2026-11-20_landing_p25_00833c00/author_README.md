# 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering (p25N, 2026-11-21)

**Grade:** RELOCATION_MATCH (2300/2300), no flag.

**Strict vland passes** (`vland.log`, 1 WIN, behaviour PASS, no warnings). The run used LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/` and
a fresh LANDVERIFY_WORK. The oracle is in `oracle.tsv`.

**Header change:** add-only overlay (`hdr/`, `hdr.diff`: 34 lines added, 0 removed), with four opt-ins:

| header | opt-in | adds |
|---|---|---|
| CEnginePrimitiveLine.h | `FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR` | the constructor body, plain inline (a new `#elif` branch; same body as the landed VISIBLE_OUTOFLINE opt-in, without `noinline`, on the original three-argument base ctor) |
| CEnginePrimitiveText.h | `FABLE_RETAIL_CENGINEPRIMITIVETEXT_INLINE_CTOR` | the constructor body, plain inline (the STEALTH_DRAW branch's body). Under the same opt-in `PFont` is `const CFontBank*`, the Ego_r.pdb type: an `#ifdef/#else/#endif` around the existing line |
| CThing.h | `FABLE_RETAIL_CTHING_GETTHINGMANAGER` | the protected inline `GetThingManager()` alone. The ONHIT branch already carries it; the new block is guarded off when ONHIT is defined |
| CThingManager.h | `FABLE_RETAIL_CTHINGMANAGER_DEBUGDRAWFLAGS` | the private statics DrawPerceptionDebug / DrawLinkedBuildingsDebug (Ego_d.pdb) and their inline accessors (FableWin.pdb). Guarded off under STEALTH_DRAW, which declares the first one |

## What closed it (from 164 difflines)
- **The residue.** Every `pos + vect` X / Y copy was x87 in ours and an integer move through ecx / edx in retail, with retail storing
  Z first.
- **What fixed it.** Two changes together:
  1. The Line and Text constructor bodies are visible. They are inline header functions in the original:
     - Ego_d.pdb lists each one once, under tc_graphic_appearance.obj;
     - FableWin.pdb emits their COMDATs in several objects;
     - retail has one out-of-line copy of each, 0x004C3090 / 0x004C3120 (both landed).

     c2 still calls them out of line at every site, as retail does: 84 calls, the same count.
  2. `FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP` is dropped, so operator+ is the header's default `C3DVector(X+o.X, Y+o.Y, Z+o.Z)`.
- **Ablation** (`evidence/ablation.txt`): without either body, or with the XY grouping back, the grade is DIFFER again. The p25N measurements:

  | variant | difflines |
  |---|---|
  | neither body | 164 (+ XY group) / 350 (without it) |
  | Line body only, noinline (VISIBLE_OUTOFLINE) | 174 |
  | Line body only, noinline (VISIBLE_OUTOFLINE), without the XY group | 202 |
  | both bodies, no XY group | MATCH |
- **Mini repro** (`evidence/mini_operator_plus.cpp/.txt`): with a visible consumer constructor, the plain operator+ gives retail's exact
  `fld [eax+8] ; mov edx,[eax+4] ; fadd ; mov ecx,[eax]`. The grouped form, forceinline or not, gives x87 copies. The `__forceinline` itself
  is neutral.
- **Also checked.** PeekPos 0x006A4D00 / GetPos 0x004C73D0 are real calls in retail, eax is kept the same way in both, and nothing changed
  in the call handling.

## Other text changes since the 164 draft (all byte-identical before the fix)
- **Interpolant.** `info.Interpolate.Current.WFInterpolate` with the reference `info.Engine.` (FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO)
  replaces the `*(const float*)&info.Interpolate[4]` cast.
- **IS_THING_ALIVE** comes from the shared engine/ThingAlive.h; the local template is gone.
- **Stealth / buildings switches.** They are `GetThingManager().IsDrawingPerceptionDebug()` / `IsDrawingLinkedBuildingsDebug()`. The
  current-manager load is dead and drops out, so retail reads only the static. Evidence:
  - the Ego_d.pdb statics;
  - FableWin.pdb's accessors and setters;
  - the retail setters 0x0051E4A0 / 0x0051E4B0;
  - the landed CTCStealth 0x007471F0 reads 0x013B8A80 through the same accessor.
- **The other two switches keep their descriptive names.**
  - 0x013B86E8 (creature names) and 0x013B8640 (navigation level; as in landed 0x006AA2E0).
  - Ego_d.pdb candidates are GDisplayScriptNames / GDisplayNavigationLevels, but there is one source each and retail has no console strings.
- **GetHomeBuilding / GetWorkBuilding** are the header's "body in the includer" definitions, now marked `inline` (they are inline in
  retail). The `inline` keyword is byte-neutral.

## Defines
- The block came from a greedy minimisation (`evidence/define_greedy.txt`). 11 defines dropped as neutral.
- Every kept define fails without it (SRC_FAIL, or DIFFER for the two INLINE_CTOR opt-ins).
- One exception: removing CDEFSTRINGTABLE makes the compile hang, so it is kept.

## Test (`test/00833c00.cpp`, passes in vland)
- **Setup.**
  - The test links the real object: the list clear, LowerBound and the Line / Text constructor COMDATs run for real.
  - Every external callee is an `/alternatename` stand-in that logs.
  - The engine, the perception TC and the tracker's thing and def are fake objects with vtables.
- **17 scenarios:**
  - the base declines;
  - all switches off;
  - names (no brain, both texts, name only);
  - buildings (home == work, both, work only, none);
  - perception (with / without the TC);
  - navigation (no tracker, live thing, dead thing, empty tracker, an unheld def destroyed by each temporary reference);
  - everything on.
- **Checked:**
  - every field of every primitive handed to the engine;
  - the persistent handles, which are the list nodes just inserted;
  - wide-string construction and destruction balance;
  - the def reference counts.
- **Mutants: 22/23 KILLED** (`mutants/`). The one survivor is `GetUniqueID()` -> `0` on a Text. The original base constructor ignores the id
  (FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR: "id is unused"; retail 0x004C3120 never stores it).

## NAME_FIXES.tsv
- 0x013B8A80 is CThingManager::DrawPerceptionDebug. The landed 0x006AA2E0 spells it `g_DebugDrawStealth`; respell that row to
  `GetThingManager().IsDrawingPerceptionDebug()` (needs the CThing.h / CThingManager.h opt-ins here).
- 0x013B8A81 is CThingManager::DrawLinkedBuildingsDebug.
