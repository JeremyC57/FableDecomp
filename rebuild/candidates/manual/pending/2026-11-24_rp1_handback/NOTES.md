# RP1 notes (round 10, header re-port: CVsyncCallbackTimer option A + the two held IDirect3D9 rows)

Counts (STATUS.tsv / APPLIED.tsv / recheck_final.log): 31 STATUS lines.
- 29 rows ported. Every grade is unchanged (1 MATCH 0x009F77B0, 28 RELOCATION_MATCH) and every test PASSes.
- 1 row blocked (0x009C0E50).
- 1 not-a-binder: 0x009BF7E0, the header's only landed includer, regraded read-only.
- Entry rp1: blocked (rows left: 0x009C0E50). The 29 ported rows are finished and can land on their own.
- recheck_final.log is one vland `--recheck-landed` run over all 29 changed rows: 29/29 PASS. No SHARED_HEADER_AVAILABLE warning for
  CVsyncCallbackTimer or IDirect3D9 is left in any of them.

## Per-row before / after (baseline: work/base.log, a vland run of the HEAD text of all 29 rows + 0x009F7020 + 0x009BF7E0; symbols: work/base_syms vs work/fin_syms)
- Grade: identical for every row (see STATUS.tsv).
- Defined-symbol set (objdump -t, external, defined): identical to HEAD for all 29 rows. No local-model symbol was lost, and no COMDAT
  was gained.
- Bound (undefined) set: identical to HEAD for 28 rows. In 0x009BEEB0 SwapScreens, `?OnPresent@CVsyncCallbackTimer@@QAEXXZ` became
  `?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ` (0x009F73C0). This is the one binding change: RENAMES s2 and NAME_FIXES (the address
  has no catalog row).
  - Exe scan (work/scan_REFS.tsv): 0x009F73C0 is referenced only by 0x009BEEB0 and 0x009BF2D0, and 0x009BF2D0 already bound
    OnPostSwapScreens. The respell removes an existing split and creates none.
  - Two sources: both PDB name sets put OnPostSwapScreens right after OnPreDeviceReset, the same 0x20 step as retail
    0x009F73A0 -> 0x009F73C0. The body is a timer-TU member (/Od frame; `this`+8 size()). Both callers call it after a successful Present.

## Group 1 (20 STL rows over CPendingQuery)
- Transformer: tools/g1port.py, proven by hand on three different tails first:
  - 0x009F7A30 (`template class`);
  - 0x009F7890 (/Od; its local CPendingQuery was four dwords, the header's has `double TimeStamp`: the trivial copy is still the dword
    block copy);
  - 0x009F7B60 (typedef'd const_iterator).
- What the transformer does: the row-local `struct IDirect3DQuery9 { ... }` (or forward declaration) and `class CVsyncCallbackTimer { ... }`
  block becomes `#define FABLE_RETAIL_CVSYNCCALLBACKTIMER_PENDINGQUERY` + `#include "engine/CVsyncCallbackTimer.h"`. Removed lines are
  left blank, so every row keeps its line count. Each row's own `namespace std` STLport model is untouched (brief point 2).
- 0x009F7BB0 instantiates `_List_base<SListEntry16>` and names no timer type, so its dead local model is just deleted (no include).
- 0x009F7890's Layout comment was updated (it called CPendingQuery four dwords with descriptive names).
- Tests: unchanged (the 0x009BEEB0 test, a group-3 row, is the exception; see below). verify_and_land.py runs `shared_header_redefinitions` on the row source only, not on the test. B's b2 / b3 precedent
  left tests as landed. No respell broke a test, so none was rewritten. Their row-local timer models remain test-only debt.

## Group 3 and the IDirect3D9 rows
- **0x004166E2:** the pad-model CVsyncCallbackTimerInfo becomes the header record. Its members are respelled to the Ego_r names
  (VsyncCount -> CPUFrameCount +0x10, LastVsyncTime -> GPUTimeStamp +0x18, PresentedVsyncCount -> GPUFrameCount +0x28; same offsets).
  The header's declared ctor makes `info` non-POD, which moved no byte. The local `CDisplayDevice` cast stand-in for CDisplayManager is
  untouched (out of scope; debt).
- **0x009F7020 (extra row, claimed RP1):** CVsyncCallbackTimerInfo moved to the header, so its own row moved too. Count members were
  `unsigned long`; Ego_r says `long`. Byte-neutral.
- **0x009BE870:** exactly B's b2 recipe (the twin of 0x009BE830): IDirect3D9 and the colliding D3D types come from the header; the local
  `enum _D3DRESOURCETYPE` stays.
- **0x009BFB30 (landed nf140; started from the tree file):** b3-style changes:
  - the local IDirect3D9, `_D3DFORMAT`, `_D3DDISPLAYMODE` and `_D3DPRESENT_PARAMETERS_` give way to engine/IDirect3D9.h;
  - SDK-enum type changes: DeviceType becomes D3DDEVTYPE, WindowHandle becomes HWND; `(D3DMULTISAMPLE_TYPE)` twice;
    D3DSWAPEFFECT_DISCARD; `(_D3DFORMAT)` on two local GetD3DFormat() results;
  - the local timer `class CVsyncCallbackTimer { IDirect3DDevice9* Device; }` becomes a plain header include.
  - A first form retyped the local inline `CPixelFormat::GetD3DFormat()` to `_D3DFORMAT`. That renamed a defined COMDAT
    (`?GetD3DFormat@CPixelFormat@@QBEKXZ` -> `...QBE?AW4_D3DFORMAT@@XZ`), so it was reverted to casts. The defined set is now identical.
  - Other local models (IDirect3DDevice9, CPixelFormat, CSurface, CTexture, C2DExtentsI, C2DBoxI, CDisplayManager) are untouched (scope).

## Blocked: 0x009C0E50 CDisplayManager::Init (decision needed; FIRST item for the coordinator)
- I deviated from the brief here ("fix them here, a rename"), on the advisor's call. The row is blocked by analysis, with 0 grades.
  Retail: 0x009C0EB5 `push 14h` ; 0x009C0EBA `call 0x00BFEA1A` (operator new), then the ctor 0x009F70E0.
- The landB1120b10 queue lines for 0x009F70E0 / 0x009F7330 (the CDynamicTextureList respell) therefore stay open.
- It does `PDynamicTextures = new CDynamicTextureList` with operator new(0x14), then calls 0x009F70E0. The header class is 4 bytes and
  "must not be constructed". Option A declares no layout, so the row cannot use the header without a size.
- Choices for the coordinator:
  - (a) option B: the real layout with STLport lists, after the calibration probe on the group-2 rows;
  - (b) an explicit size-only opt-in. I did not add one: it is a layout claim, which option A excludes.
- The row is unchanged. It still binds 0x009F70E0 / 0x009F7330 as `??0/??1CDynamicTextureList@@QAE@XZ`. That existing split stays
  (self rows 0x009F70E0 / 0x009F7330 and 0x009C01C0 bind the CVsyncCallbackTimer spelling).
- The member names B listed (PD3D / PDevice / Caps / PDynamicTextures) are left for the same pass.

- 0x009BEEB0's test is a standalone host model with its own `CVsyncCallbackTimer::OnPresent`. It was respelled in place to
  OnPostSwapScreens (3 lines; PassPattern unchanged; rows/009beeb0_test.cpp), so the test agrees with the row. The 0x004166E2 test
  simulates the arithmetic on its own `struct Info` (VsyncCount / LastVsyncTime / PresentedVsyncCount, no CVsyncCallbackTimerInfo).
  It is left as landed and noted as a descriptive test-only spelling.
- EH funclets / includers: no file in rebuild/src/compiled or rebuild/tests includes any of the 29 changed row files (basename grep),
  so there are no funclets to regrade. A self rename moves no file: no self row was renamed.

## Header choices
- Ctor and ClearAllQueries are not declared: no finished row uses them. Their users are 0x009C0E50 (blocked) and group 2.
- OnPostSwapScreens was added beyond the brief's method list, for 0x009BF2D0 / 0x009BEEB0.
- The destructor has its own macro (`_DTOR`).
- No FABLE_FORCEINLINE. No existing member was changed, so no "needs header claim".

## Other rows seen (not touched; for the coordinator)
- Group 2 (0x009F70E0, 0x009F7330, 0x009F7250, 0x009F73A0): option B, not in scope. 0x0041C780 / 0x0041C7A0 / 0x0041C7C0 are NF146's.
- 0x009F7330 is also referenced by 0x009C1810 (`?Dtor_009f7330@T@@`) and 0x009C1E70 (`?Cleanup_009f7330@T@@`): stand-in spellings
  on the same address. This is an existing split, not mine.
- 0x00CABE00 (catalog "CVsyncCallbackTimer::CVsyncCallbackTimer") is a CEnumPins byte twin, per 0x00CAC890's comment. Its local
  `CVsyncCallbackTimerInfo_00cabe00` / `CVsyncCallbackTimer_00cabe00` are address-suffixed stand-ins, not this class. Catalog debt.
- The tests of 21 of the 29 changed rows still define CVsyncCallbackTimer / CVsyncCallbackTimerInfo / IDirect3D9 locally (test-only debt; grep of the landed tests).

## Descriptive names
None new. 0x009F7050's `pRecord` / SVsyncGPUTimeRecord / g_Vsync* globals were already descriptive (unchanged).

## Flags lines
None added.

## Rule slips
- One Bash call exited 1: a `cat` of a lock owner file that did not exist (0x009F7020, before claiming it). Every later call ends `; true`.
- 0x009F7890's and 0x009BF500's comment lines were edited after their first grade. Comments only; both are covered by recheck_final.log.
