# Naming pass wave 30 (2026-11-02): 46 landed rows respelled + 0x007F0B10 by the coordinator -- ~CScriptThing binders, the CGameScriptInterface script-API rows, PeekPThingWithUID, CTCBase::OnDie, STLport list dtors, LHNetworkLib

Inputs: brief `naming/briefs/BRIEF_W30.md`, QUEUE_W30_input.tsv (122 rows: wave 29 deferred / partial rows plus the c700 / c701 rows appended
mid-wave), wave 29 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w30` (AGENT_README.md; scripts/ here; own recheck
`logs/recheck_final.log`, 46 rows, after chunks 700 / 701). No shared header edited. The coordinator copied ov_final (48 files), applied the 9 RENAMES
(git mv + build_candidates.ps1), BINDINGS (18 RETIRE / 6 ADD / 7 KEEP, 0 CONFLICT; the table equals the agent's after-table) and LABELS (3), respelled
the landed 0x007F0B10 itself (DECISIONS_W30 D4: KillRippleFX private, Leaf ?DoDieFX@CTCKrakenTentacle@@QAEXXZ, binding -> AAE, so 0x007F07A0 has one
spelling), and rechecked the 47 rows against HEAD (`logs/coordinator_recheck.log`: 47/47 = 1 MATCH + 46 RELOCATION_MATCH, all PASS). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): cst 15 (~CScriptThing 0x004AA840 binders on engine/CScriptThing.h's retail branch: the local
QAE stand-ins and the invented ScriptThingCopy / NScript::ScriptThing / Row...::Handle views), csm 18 (the CTCAIScratchpad / script-API rows on the
landed 0x008A1E50 model: the header's CScriptThing, CThing for CThingNative, own rows as the retail virtual CGameScriptInterface members in place of
the invented CGameScriptInterfaceMethods), c699 7 (CThingManager::PeekPThingWithUID 0x0051ED60 const, was "CStopWatch::GetTicks"; CTCBase::OnDie
0x006868E0 with CTCScriptedControl::OnDie 0x00713740 and CTCKrakenTentacle::OnDie 0x007F0890; ~C3DMeshFilePrimitiveChunk 0x00A9D3A0, was
CTCInventoryItem::OnDie), stl 4 (STLport ~_List_base for EChangingStateComponentState / CEngineMap* / CSpriteTrailEntry, were byte-identical
SortTreeRecursively stand-ins; ??_G 0x0052D770's list dtor), lhn 2 (~LHHttpHeaders 0x00569C30, STLport ~vector<std::string> 0x00569940), plus the
coordinator's kraken row 0x007F0B10.

Queue outcomes (QUEUE_W30.tsv): 8 respelled, 8 partial, 106 deferred (33 of 65 ~CScriptThing binders done; ~CTCBase, most c697-c701 rows, scopes 2-3
not reached). Decisions (`DECISIONS_W30_RESOLVED.tsv`): the 17 two-view CScriptThing rows wait on the owner's inline_depth question (the same
per-call-site inline split); fable_string.h gains the PDB CCharString copy ctor in wave 31; the five transitional invented spellings are accepted and
queued for their PDB forms; engine/CTCBase.h gets a retail branch as a header family; libpk keeps the vtable-address convention -- 0x00C74450's class is
CVtable_012bf050 (virtual dtor), 0x012BF0C8's CVtable_012bf0c8.
