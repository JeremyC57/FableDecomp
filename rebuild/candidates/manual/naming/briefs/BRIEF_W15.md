# Naming pass, wave 15 (one agent beside two chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it** -- a proven wrong spelling in a landed source is respelled in this wave, not held. The resolved decisions are in
`naming/2026-11-01_wave14/DECISIONS_W14_RESOLVED.tsv` (+ wave 13's); canonical globals / members in `naming/ESTABLISHED_SPELLINGS.tsv`
(0x013B8BA8 is now the CTCInventoryWeapons TU's anonymous-namespace NEWLINE). Rules, mechanics and hand-back as `briefs/BRIEF_W13.md` /
`BRIEF_W14.md` (read both, the wave 14 README, DEFERRED.tsv and the wave 14 `scripts/` -- compose14.py, fam_*.py, hand14/ -- reused as
compose15.py etc.). Labels the chunks already proved are in `ghidra_out/labels_chunk651..665_2026-10-27.tsv` (the catalog and manifest
already carry them; the landed SOURCES do not -- that is this wave's work).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family)
1. **The NAME_FIXES_QUEUE rows c651-c665** (`naming/NAME_FIXES_QUEUE.tsv`, ~260 rows; a row's wrong_spellings_seen names the landed
   sources to respell). Group by target symbol into families; the big ones first:
   - base destructors with stand-in families: 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ` (landed source says ??1CBaseClass: respell
     the row itself too), 0x0099A430 `??1CBaseObject@@MAE@XZ`, 0x00686830 `??1CTCBase@@UAE@XZ`, 0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ`
     (~27 stand-ins), 0x00693EF0 `??1CCreatureActionBase@@UAE@XZ`, 0x0062E900 `??1CCombatActionBase@@UAE@XZ`, 0x00CBD510 `??1CScriptBase@@UAE@XZ`;
   - 0x00833A10 `?SuspendAI@CThingAICreature@@QAEX_NPAX@Z` (17), 0x00861D00 `?OnDeactivate@CModeStandBase@@UAEXXZ` (8);
   - the ??_G scalar deleting destructors landed under stand-in names (c657, c664, c665 rows) and their callee spellings;
   - the STLport respells: `_M_erase` private AAE instantiations (c663, c664, c665), `_List_base` / `list` thunk pairs, `remove_if`,
     `_Destroy<T>` rows (0x00CD4FD0, 0x00781540, 0x007699C0, 0x004B86B0), the `??_G?$pair` rows;
   - the per-row items (GetTankard / GetFood swap, PeekStaticMesh / PeekAnimatedMesh, CTCBase::GetWorld 0x00686A70, FadeOrKillEffect
     0x0072AC40 / 0x0072ABD0, GetXString / IsExpression / GetUIInventoryCategory statics, CPhysicsCollisionPointsObject ctor, the 41
     anonymous-namespace CCharString globals of c661 under the lane's TU hash, the Kraken ctor members Interrupt / LoopNum, ...).
   0x00440730 stays on HOLD (its row says why). A row whose respell moves bytes -> DEFERRED with the diff (re-authoring work).
2. **DECISIONS_W14_RESOLVED**: IS_THING_ALIVE per-caller instantiation for 0x0097A630 (binding row with an `ICF` note), the
   CCreatureActionBase friend model for 00692940 / 00692c10 (`// Access:` line), the audio slot-5 spellings unified on ReleaseBuffer.
3. **The 0x013B8BA8 NEWLINE callers** (g_NewLineWideString binding) to the anonymous-namespace spelling.
4. The wave 14 DEFERRED items still name-only (the five KEEP spellings, 00972590 gw).
Not in this wave: the 10 vcs byte-changers and LowerBound's shared CVectorMap model (re-authoring; a chunk), VIRTUAL_SCREEN_* /
PMainGameComponent (gated), the engine header changes (migration wave 10).

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W15.tsv (every queue row with its outcome: respelled / deferred /
hold), the per-family rows files, logs and scripts, as wave 14 did.
