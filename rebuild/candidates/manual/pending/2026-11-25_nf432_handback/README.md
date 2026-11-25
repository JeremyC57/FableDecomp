# NF432 hand-back (2026-11-25): the placeholder classes NF430 identified

NF432's own summary is `AGENT_README.md`. `NOTES.md` has the method, sources per name, every gate and the pilot findings.

## Landed: 10 rows

### `batches/2026-11-24_landing_nf432_n1` (7 rows)
- **Five NPlayerGui draw constructors** now sit on `engine/NPlayerGui_CDrawBase.h` (CTOR + VSLOTS) and carry their PDB constructor names:
  - 0x0064D2A9 CDrawSpiritsHealth;
  - 0x0064D335 CDrawReceivedItems;
  - 0x0064D351 CDrawFishingGame;
  - 0x0064D389 CDrawSpiritScores;
  - 0x0064DA6E CDrawScriptScreenText.

  Each name has three sources: retail's RTTI type locator, FableWin's `??_7`, and the Ego_r / FableWin constructor names. Their five `??_7CVtable_0125a5xx` table lines are replaced by the real `??_7CDraw<X>@NPlayerGui@@6B@`.
- **0x00442760** is renamed to `CDrawBase::ClearPrimitives`, NF430's name fix.
- **0x00644627** is ported to its PDB name `ClearPrimitivesIfNecessary`. Its real address had been landed under a non-namespaced local name, and the rename above would otherwise leave slot 0 with no definer.
- **Headers:**
  - four new class headers (`NPlayerGui_CDraw{SpiritsHealth,FishingGame,SpiritScores,ScriptScreenText}.h`, from FableWin field lists and struct_layouts_egor, each with a 0x28 size assert);
  - one add-only opt-in, `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY`. Without it, 0x00644627 would emit a reached `Clear` copy that differs from 0x0041BE70.
- **Vftables:** every emitted slot agrees with retail (10 slots, 0 wrong).
- **Copies:** 0x00644627's four added pick-any copies have no retail address. Each is object-equal to landed 0x0064ED4F's copy.

### `batches/2026-11-24_landing_nf432_g1` (3 rows)
- `CScriptBase::RegisterMain / Main / Init` at 0x00CBD4B0 / C0 / D0 replace the shared gapscan stand-in `?Hook@T@@QAEXXZ`.
- Sources: retail vftable 0x012C1648, stored by the landed `??0CScriptBase@@QAE@XZ`, and FableWin `??_7CScriptBase@@6B@` slots 1-3.
- This stand-in, not the rows themselves, caused most of the "wrong slots" NF430 measured: every script vftable inherits these slots. /FORCE duplicates go from 3 to 1; the remaining one is an unrelated pre-existing pair.

### Checks
- NF432 ran the full gate set (NOTES.md).
- The coordinator reran equiv_check copy against copy on the tree at landing time.

## Held
| Row | Class | Why held |
|---|---|---|
| 0x0064D3DA | CDrawDamage | The header's STLport list ctor copy differs from 0x00655B18; it needs one unit with that row (NF390 R5). |
| 0x00C3C840 | `??_GCCodaObjectBase@@UAEPAXI@Z` | R-NF432-1, below. The batch lacks CHECKS / SELF_SYMBOLS / UNITS, so it is rebuilt before landing. |
| 0x00CBD4E0 | CScriptBase::OnPersist | 0x00DC3EA0 and 0x00DCC000 emit the same public symbol as pick-any copies. A strong definition would add an LNK4006 (NF382 b3). |

## Ruling R-NF432-1 (coordinator, 2026-11-25): granted, as R-NF430-2
A row that moves onto the D3-settled `??_7CCodaObjectBase@@6B@` may land while 0x012B5868 stays split by other rows' placeholders. Three reasons:
- that spelling is already bound there (witness 0x00C3E2A0);
- the row adds no spelling and does not widen the split;
- the name gate fails only because `is_std` treats every `??_` spelling as std, so a reused vftable spelling at a split address can never read as DEBT.

The split stays on the queue (nf432 0x012B5868; the 13 derived libpk binders under R-NF430-1).

## Not done
- **CEntityScriptBase (task 2):** not attempted. The nf328 line on 0x00DEF6A0 is untouched, and CSP_UndeadTrigger was skipped.
- **The 102 NF430 debt rows (task 3):** not ported. After g1, the remaining wrong slots are overrides the local script models do not declare. The generator plan is in `T3.txt`.
- **0x0043B570:** its CDrawNPCName vftable has two swapped slots (CTOR without VSLOTS). The fix is one macro; it is queued.

## Pilots for R-NF430-1 (findings only)
- **(a)** No CODA class is in FableWin or Ego_r publics or types. The landed `??0CCoda3DSource@@QAE@XZ` came from Codex pass 23:
  - CODAApi.lib CodaContext.obj relocations name the constructors `CCodaContext::Create` calls;
  - CODAApiDbg.lib gave class keys.

  That is a structural tie of the kind R-NF430-1 asks for.
- **(b)** The debug lib CODAApiDbg.lib is on disk in the coordinator's scratch SDK tree, with 77 `??_7` names. The slot-body match has not been run. No SDK file enters the repo.

## Tool fix
NF430's `vt430.py` read only the first relocation block of a section name, so it mis-checked objects that emit several vftables, and NF430's wrong-slot totals were taken with it. The fixed copy is `tools/vt432.py` + `coffload.py`. Re-measured per t1 batch (t1a / t1b / t1c):

| | before (vt430) | after g1 (vt432) |
|---|---|---|
| agreeing slots | 54 / 43 / 0 | 143 / 115 / 113 |
| wrong slots | 193 / 131 / 178 | 179 / 146 / 138 |
