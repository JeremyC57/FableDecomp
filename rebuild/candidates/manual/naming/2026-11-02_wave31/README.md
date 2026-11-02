# Naming pass wave 31 (2026-11-02): 94 landed rows respelled + 3 shared headers -- CCharString copy ctor, CTCBase / CTCDoor retail branches, ~CTCBase binders, CDrawBase::GetPlayer, libpk, the c702 / c703 rows, SetAsRenderedThisFrame

Inputs: brief `naming/briefs/BRIEF_W31.md`, QUEUE_W31_input.tsv (152 rows: wave 30 deferred / partial rows plus the c702 / c703 rows appended
mid-wave), wave 30 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w31` (AGENT_README.md; scripts/ here; own recheck
`logs/recheck_final.log`). Shared headers edited: fable_string.h (the PDB copy ctor ??0CCharString@@QAE@ABV0@@Z 0x0099EC30), engine/CTCBase.h (opt-in
FABLE_RETAIL_CTCBASE, 28-slot vtable 0x0125BE0C), engine/CTCDoor.h (opt-in FABLE_RETAIL_CTCDOOR); both engine headers carry the RETAIL_MARK, so
gen_class_headers.py keeps them. The coordinator recomposed the overlay against the current tree (identical to ov_final, 97 files), copied it,
applied the 25 RENAMES (git mv + build_candidates.ps1), BINDINGS (42 RETIRE / 10 ADD / 13 KEEP, 0 CONFLICT; the table equals the agent's
after-table) and LABELS (22; the 0x004C7590 row corrected to CThing::SetAsRenderedThisFrame, D9), and rechecked 1,628 rows (the 94 family rows + every landed includer of the three headers, transitively; 2 more than the agent's 1,626) against HEAD
(`logs/coordinator_recheck.log`: 1,624 = 27 MATCH + 1,597 RELOCATION_MATCH, all PASS; the 4 failures fail identically without this wave, D8). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): fs (fable_string.h + two includer tests' copy-ctor stubs, D6), csm 4 (the blocked setters as
CGameScriptInterface virtuals taking CScriptThing / CCharString by value), tcb (engine/CTCBase.h retail branch; 0x006868E0 / 0x00713740 / 0x007F0890 off
local models), tcd 13 (~CTCBase binders as the implicit ??1 of their CTC class through DestroyOne, D2), dgp 4 (NPlayerGui::CDrawBase::GetPlayer
0x00643D7C / 0x00643D84 and callers), c702 1 (CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef 0x00643FC8), door (engine/CTCDoor.h;
CTCDoor::Copy 0x007305A0, ~CTCDoor 0x00731330), pek 3 (NParticleEngine::CParticleEmitter), hos 3 (ClearScoreboard private), d3 3 (protected
CTCBase::GetThingManager, GetPMaritalHome, CTCCarrying::GetThingInPrimarySlot), libpk 23 (CVtable_012bf050 with a virtual dtor, CVtable_012bf0c8),
c703 9 (virtual CThing::WorldUpdateBulletTime, protected ClearAllPrimitives, CTCGraphicAppearance own rows as members, the class-keyed STLport
allocator), srtf 24 (0x004C7590 = ?SetAsRenderedThisFrame@CThing@@QAEXK@Z, seven old spellings retired; 0x007CE370 = CTCElectrocutedEffect::OnInitialActivate).

Queue outcomes (QUEUE_W31.tsv): 11 respelled, 10 partial, 17 done, 114 deferred. Decisions (`DECISIONS_W31_RESOLVED.tsv`): D1 / D3 header branches
accepted; D2 implicit dtors accepted; D9 SetAsRenderedThisFrame verified (Ego_r 0x004C9150 between SetCurrentMapNumber and ClearRenderedThisFrame;
FableWin.pdb); D8 bisected by the coordinator -- 0x00692230 / 0x006922C0 broke in 84edb11e4 (engine/CBaseClass.h's virtual dtor adds an EH frame,
40 bytes vs 30), 0x004D9861 / 0x009E1440 fail since 2026-10-28; all four become chunk 705 repairs; D10 / D11 / D12 / D13 carried to wave 32.
