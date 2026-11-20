# NF160 notes (engine resource / texture / input naming, 22 entries)

Counts below come from APPLIED.tsv and STATUS.tsv.

Entries (22):
- 10 done: q4132, q4287, q4297, q4298, q4301, q4591, q4592, q4593, q6174, q11726.
- 2 done-already: q4286, q4603.
- 10 blocked: q4293, q6169, q4300, q6166, q6213, q11728, q11729, q11795, q11867, q11868.
- 0 rejected, deferred or open. REJECTED.tsv has one line: q6213's queue spelling (a stand-in) was rejected. The entry was then
  worked to the STLport spelling and is not a rejected entry.

Rows (54):
- 41 ported, all in rows/.
- 2 not-a-binder: 0x00BE6F70, 0x00BF4570.
- 11 blocked: 0x00499880, 0x007E5230, 0x00689130, 0x00B87680, 0x00B89A20, 0x005EC990, 0x00BEC000, 0x00BEC470, 0x00BEC950,
  0x009E15E0, 0x0122A600.

Grades: every ported row is MATCH or RELOCATION_MATCH. Tests PASS in batches b1 to b4, and the final recheck is recheck_final.log.

## Descriptive names
- I added no made-up names. Every new leaf, callee and data spelling is a PDB spelling, confirmed in both Ego_r.pdb and FableWin.pdb,
  or a library instantiation spelling (STLport `std::vector<CEnginePrimitiveHandle>::resize`, `std::list<CEngineMap*>`).
- Rows I touched keep the existing "(descriptive)" comments on callees that have no PDB name. Example: 0x00499880's own callees,
  which I did not change because that row is blocked.

## Self renames (each moves the row's file at landing)
No EH funclet includes any of these files. I grepped rebuild/src and rebuild/tests for each file stem and found nothing.

| Row | Leaf change | Current file |
|---|---|---|
| 0x009FA280 | leaf only, pool parameter `long` -> `ESurfacePool` | file stem unchanged: CTexture_Initialise_009fa280 |
| 0x00A04110 | -> CInputEvent::SetAsJoystickButtonReleased | 00/a0/CInputEvent_SetAsJoystickButton3_00a04110.cpp |
| 0x00B40230 | -> CEngineWorldMap::OnGameMapUnloaded | 00/b4/CEngineMapManager_MapStateSetUnloaded_00b40230.cpp |
| 0x00B42390 | -> CEngineWorldMap::SetMapVisibility | 00/b4/CEngineMapManager_SetMapVisibility_00b42390.cpp |
| 0x00B423B0 | -> CEngineWorldMap::SetMapVisibilityAllMaps | 00/b4/CMapOwner00b423b0_SetMapVisibility_00b423b0.cpp |
| 0x00B41DB0 | -> `?SetMapVisibility@CEngineMap@@QAEXAAV?$list@PAVCEngineMap@@V?$allocator@PAVCEngineMap@@@std@@@std@@_N@Z` | 00/b4/CEngineMap_SetMapVisibility_00b41db0.cpp; file stem unchanged |

Census for each referenced row, so no split is created:
- 0x00B40230: its only referencer 0x00B23E60 already binds the new leaf.
- 0x00B42390: 0x00B23E40 already binds it.
- 0x00B423B0: 0x00B23E50 already binds it.
- 0x00A04110: its only referencer 0x00AB6E40 already binds it.
- 0x009FA280: the 22 REFS rows all end on the new spelling. 16 already bound it (by probe; 0x009A3640 confirmed with objdump), and
  this hand-back respells the other 6.

## Out-of-entry self port: 0x00B41DB0 CEngineMap::SetMapVisibility
- 0x00B41DB0 is the callee family of q4592 and q4593. I claimed its lock first: the owner file read `released`, so the lock was free.
- The row was moved off the invented `CRetailList` and its local CEngineMap, CEngineLandscapeMap and CLocalDetailCacheMap models. It now
  uses STLport `std::list` (FABLE_RETAIL_STLP_LIST) and the shared headers, with hdr/ opt-ins.
- Exe scan (tools/refs1.py 00b41db0, which uses gate.scan): the only landed referencers are 0x00B42390 and 0x00B423B0. Both are in
  this hand-back, so the address ends on one spelling.
- Evidence: Ego_r.pdb 0x009E2440 and FableWin.pdb 0x02D5B370.

## New PDB spellings that replace stand-ins on callees
The spelling count does not grow at any of these addresses. RENAMES section 2 has the full list.
- 0x00443F90 STLport vector resize: replaces the CRetailHandleVector / CHandleVector stand-ins.
- 0x00A03B40 GetType, 0x00A03B70 GetKey, 0x00A043A0 GetJoystickPosition, 0x00A03B90 GetJoystickButton: replace the
  GetEventType / GetKind / GetKey_00a03b40 / GetAnalogueVector / MarkAsUsed / GetControllerIndex stand-ins.
- 0x00BDBFF0 CEngineLandscapeMap::OnGameMapUnloaded.
- 0x009DA9F0, 0x009DE3F0, 0x00437570 and 0x0049B3B0, from 0x00499D90.
- 0x009FA280, from 0x00F37440's CTextureNative stand-in.

## Splits that already existed and are left as they are (none created)
- 0x00499880 RenderProgress:
  - reloc_bindings.tsv already bound two spellings at HEAD: QAE (from 0x00499C50 and the self row) and AAE (from 0x00499AA0).
  - 0x00499D90 now includes engine/CProgressDisplay.h, which declares the member private, so it moved from QAE to AAE.
  - The count stays at 2. The split closes when the self row 0x00499880 is ported. That port is blocked, see STATUS.
- 0x00BDBFF0: the landed self row (`?M@C00bdbff0@@QAEXXZ`) and 0x00B3EDC0 keep their stand-ins. They are outside this slice. NAME_FIXES
  has the fix.

## Decision for the coordinator: the `??1CDrawBase@NPlayerGui@@QAE@XZ` COMDAT
- Seven q6213 rows now also define the implicit destructor of their row-local `NPlayerGui::CDrawBase` model: 0x0064E24F,
  0x0064E83F, 0x0064E8DC, 0x00651904, 0x00654258, 0x006542A4, 0x00658240.
- Cause: in each row, the local CDrawBase model's `PrimitiveHandles` member (+0x1C) changed in place. It was the trivial
  `CRetailHandleVector` stand-in and is now `std::vector<CEnginePrimitiveHandle>`. Its non-trivial destructor makes the constructor's
  EH unwind emit `~CDrawBase`. The class token is unchanged, so this is an in-place respell of a local model the row already had.
- The landed recipe row 0x0064E1E4 emits the same COMDAT.
- Ownership checks:
  - reloc_bindings.tsv has no `??1CDrawBase@NPlayerGui` binder.
  - No landed row has it as its Leaf.
  - engine/NPlayerGui_CDrawBase.h only declares `~CDrawBase();` (out of line), so the header defines nothing inline.
  - So the hold condition of the in-place rule (another landed row owns the symbol, or the header defines it inline) does not apply.
- Latent clash:
  - FableWin.pdb has a real out-of-line `??1CDrawBase@NPlayerGui@@QAE@XZ`: player_gui.obj S_GPROC32 at 0x01A7B030 (also listed at
    0x01831904 in _fablewin_api.tsv).
  - If a strong retail row for that destructor ever lands, these pick-any COMDATs (and 0x0064E1E4's) would hit LNK2005, as NF92's
    link proof showed for selection-2 against a strong owner.
- Choices for the coordinator:
  - accept it now, as with 0x0064E1E4;
  - or hold the seven rows until they are ported onto NPlayerGui_CDrawBase.h. Its CDrawBase modes sit behind
    FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22 and related macros. I did not attempt that port.

## Needs header claim
- rebuild/include/candidates/preload_bump_texture_compiled.h, lines 75-76:
  - Its local CResource declares public `ListRemove()` and `ListInsert(CResource*, void*)`.
  - Changing these to the PDB private forms `?ListInsert@CResource@@AAEXPAV1@0@Z` and `?ListRemove@CResource@@AAEXXZ` alters an existing
    member's access and signature in a shared header.
  - This unblocks 0x00BEC000, 0x00BEC470 and 0x00BEC950 (entries q11728, q11729, q11867, q11868). Those rows are July multi-block
    rows with no Leaf line.

## Blocked rows that need other work first
- 0x00499880 RenderProgress, entries q4293 and q6169:
  - Draft: work/00499880_blocked.cpp. Best result was DIFFER(476v489) after 3 forms.
  - The only remaining gap is the two inline `~CSurface` vtable stores.
  - Porting it closes the RenderProgress split and the 0x00A0AA60 / 0x00A0AA90 / 0x009DA9F0 stand-ins. NAME_FIXES has the names.
- 0x007E5230, entry q4300:
  - Draft: work/007e5230_blocked.cpp, DIFFER(817v817) with one scheduling swap.
  - It still binds `?GetEventType@CInputEvent@@QBEJXZ`.
- 0x00689130, entry q4300: the row itself is the NativeInput::TargetSelect stand-in, so it needs re-authoring.
- 0x00B87680 and 0x00B89A20, entry q6166:
  - Both still bind `?Clear@CRenderManagerCore@@QAEXABVCRGBColour@@K@Z`.
  - Their local CSurface, CRenderTarget, CDisplayManager, CTexture and CRenderStateManager models must move onto the shared headers
    first. That is re-authoring work, not a respell.
- 0x005EC990, entry q6213: porting it also respells the DrawBox callee's parameter type, a family that goes beyond this entry.
- 0x009E15E0 and 0x0122A600, entry q11795, as NF139 found:
  - 0x009E15E0 is the CTextureEraseView stand-in with no Leaf line.
  - 0x0122A600 is the AutoTinyThunkTarget atexit thunk of the anonymous-namespace GFontTextureCache. Its identity must be settled
    together with the registrar 0x01223EE0.

## Flags lines
- None added or changed. The `// Flags:` lines in 0x0049124E, 0x0055FC90, 0x0068C7F0, 0x0068C880 and 0x00690480 match HEAD exactly.

## Rule slips
- One Bash call exited non-zero: a `diff` loop without `; true`.
- One stray scratch file was written to /tmp/x1, outside the scratchpad. It has been deleted.
- STATUS lines for 0x009E15E0, 0x0122A600, 0x00BE6F70, 0x00BEC000, 0x00BEC470, 0x00BEC950 and 0x00BF4570 were recorded before their
  locks were claimed. The locks were claimed afterwards: two read `released`, five did not exist. None reads `acctB`, and none of these
  rows was changed.
