# NF430 hand-back (2026-11-25): the `CVtable_<va>` placeholder classes

NF430 worked the placeholder classes that landed code names after a vftable address. `NOTES.md` has the method and the gates.

## The brief's counts were off
- There are 1,118 open addresses over 431 vftable VAs. About 1,000 of them are libpk / CODA code (0x00C0xxxx..0x00C8xxxx, vftables 0x012B1xxx..0x012BFxxx). Neither PDB covers that code, and retail has no RTTI for it.
- The "227-address `??0C3DAnimationUtil@@QAE@XZ`" stand-in is really NF424's CLandscapeBackgroundPatch deleting-destructor family. `??0C3DAnimationUtil` sits on only 14 libpk rows.

## How classes were identified
NScript script classes, two sources each:
1. FableWin.pdb's `??_7<C>`, whose own-override slots sit in rows whose module or self is that class: 395 slots agree, 0 disagree.
2. The PDB leaf of the row that stores the vptr.

The 6 NPlayerGui draw classes have retail RTTI plus FableWin. The hit rate is 100 of 108 Fable-side VAs and 0 of 317 libpk VAs.

## Landed: 102 table-only deletes over 99 VAs
The batches are `batches/2026-11-25_landing_nf430_t1{a,b,c}`.
- **What was deleted.** Each is a stale placeholder line `??_7CVtable_<va>@@6B@`. Every landed binder of that VA already emits the real class's `??_7` (HEAD census).
- **What did not change.** Rows, tests and objects equal HEAD, so the batch folders keep only the tables and evidence.
- **The tracker** shows exactly 99 addresses moving from split_unknown to agree_std, and nothing else.
- **Coordinator checks:**
  - **Leftover users.** No landed row names a deleted placeholder class in code. 16 tests still model one locally (debt below), and deleting a table line does not change their checks.
  - **Multi-target bindings.** `bindings_multi_check` stays at 1.
- **What this is.** These are stale placeholder lines removed, not newly named classes. The class names were already emitted.

## Rulings (advisor decision, owner delegated, 2026-11-25)
- **R-NF430-2: granted.** A table-only delete may land over witness rows that are unchanged from HEAD but already fail name_gate.
  - The landing adds no debt. The name_gate failures (local class models, old splits at 0x00D2F6B0 / 0x00EB2000) and the wrong vftable slots (151 / 120 / 178 per batch) are identical at HEAD.
  - The landing asserts no new name.
  - The debt is queued as the nf430 `debt_header` family line (102 rows).
- **R-NF430-1: no blanket grant; the libpk / CODA residue stays held.** This follows R-NF428-1.
  - A GameCODA SDK name counts as one source for a class only when something structural ties it to retail: slot count and order against the SDK class declaration, or a body match. The second source must come from retail or a PDB.
  - The only evidence so far cuts the other way. Release `CODAApi.lib` matched only 1-4 trivial slots per vftable.
  - Two cheap pilots are open:
    - look for CODA classes (`CCoda*`, `PKC*`) in FableWin.publics / Ego_r publics, and find where the landed `??0CCoda3DSource` name came from;
    - match against the debug CODA lib.
  - SDK names may be cited like PDB names, but no SDK file enters the repo.

## Residue (`T3.txt`, 1,121 lines; T3 queue family lines `nf430`)
| Group | Lines | Next step |
|---|---:|---|
| libpk_hold | 1,000 | R-NF430-1 |
| debt_header | 102 | local class models and wrong slots on the t1 witness rows |
| identified_port | 15 | see the list below |
| d3_port | 4 | the CCodaObjectBase port EXTRA_RULES D3 names (13 rows spell it 15 ways) |

The 15 identified ports are:
- 6 NPlayerGui draw constructors, to go onto `engine/NPlayerGui_CDrawBase.h`;
- CEntityScriptBase (0x00EB2000, 0x00DEF6A0 model it locally);
- CSP_UndeadTrigger (waits on nf328 for 0x00DEF6A0);
- CFeatDef / CScriptBase (nf303);
- strmbase CEnumMediaTypes (vendor-library route);
- 0x00DA0130 (R-NF424-2).

## Name fix
- 0x00442760 should be `?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ`, not `ClearPrimitivesIfNecessary`, which is bound at 0x00644627. FableWin's vftable puts it in slot 1, and its body is empty, matching retail's bare `ret`.
- Apply it **only with the NPlayerGui port**. Applied alone, it creates a split at 0x00442760.
