# Phase 8 brief (from phase 7, 2026-11-06)

## State after phase 7 (counts in each wave's NOTES.txt / PASS.tsv / DEFERRED.tsv)
- w1_respells: 87 PASS, 8 deferred.
  * Old-spelling binders of the PDB-spelled targets of plan_p6_w2/CALLERS.tsv, respelled QAE->QBE and into NGraphicAppearance.
  * CScriptThing::NullCoord 0x0143E8E0 now has 36 + 3 binders (new sub-macro FABLE_RETAIL_CSCRIPTTHING_NULLCOORD).
  * 0x004C7A10 = CThing::PeekPlayer: 2 binders respelled; queue lines 4451 / 5150 closed, 12 narrowed.
- w2_cthing: 123 PASS (119 header migrations + 4 operator-new access respells), 2,308 deferred.
  * New sub-macros in engine/CThing.h: FABLE_RETAIL_CTHING_MOREACCESS (14 members), FABLE_RETAIL_CTHING_OPERATORNEW.
  * Queue line 220 is narrowed: ??2CThing@@SIPAXI_N@Z has no binder left; 2 placeholder stand-ins remain.
- w3_cintelligentpointer: 388 PASS of 418 rewritten (519 candidates); 131 deferred; no header change.
- Not started: wave 4 (CTCBase 1,223, CThingCreatureBase 1,071). The changed-row count stayed far under the 5,000 budget: the
  CThing leftovers are long-tail by-hand classes, not one rule.

## Wave 1 leftovers (naming decisions, two sources needed)
- 0x004BFFC0: the landed definer and Ego_r say ClearHighlighted (the PDB majority), but row 007cc120 calls it ClearMeshEffect.
  The namespace respell would bind ClearMeshEffect at two addresses (0x004BFE20 is ClearMeshEffect). Decide 007cc120's callee.
- Semantic multi-spelling targets: one target carries two declarations, and a respell changes a call's const-ness / argument
  type, so each needs the callee decided first:
  GetCurrentMode 0x0073A6E0, GetPerceivedThing 0x007446A0, AddTC 0x004C9D60, 0x00811340, 0x006D6520, 0x006E3180, 0x005DA980,
  ICF 0x007448F0.
- NullCoord: 4 reads outside a CScriptThing member (00d6b240, 00d701c0, 00d7f880, 00e0f4e0, an expanded GetPos) still bind
  g_ZeroVector. They need either the CScriptThing GetPos inline from the header (FABLE_RETAIL_CSCRIPTTHING_NULLCOORD plus a
  public accessor) or a friend: decide from the retail TU (queue 4223 / 6222 stay narrowed until then).
- 004C7A10 / 004C7A30:
  * 00733100 needs CPlayer::HasMode const (PDB QBE_NW4EPlayerMode@@@Z);
  * 007b1eb0 needs CPlayer::AddMode(EPlayerMode, bool);
  * 00692920 is a destructor-value row that binds 0x004C7A10 as a free stand-in.
- 0x004C7380 CThing::operator new (queue 220, narrowed): 006ac910 (?Allocate@NFactory006ac910@@YIPAXI_N@Z) and 00835c80
  (?operator_new@ExternalRetry_00835c80@@YIPAXI_N@Z) still bind free-function stand-ins. Each models its CThing-derived class
  inside a placeholder namespace; rebuild the models on engine/CThing.h with FABLE_RETAIL_CTHING_OPERATORNEW.
- 0x00639E40 (from phase 6): the binding GiveGold@CTCHeroMoney, the interface number 0x8E (TCI_REGION_DISPLAY) and the catalog
  label CTCRegionDisplay::GlobalDraw disagree.

## CThing: what is left (w2_cthing/DEFERRED.tsv `no_rule`, by phase 6 class)
1. local_std 567 (counts from w2_cthing/DEFERRED.tsv): a local namespace std model clashing with STLport. Port row by row to stlport/fable_containers.h.
2. header_lacks 525 (008385b0 left the class via the operator-new respell), a long tail:
   Components@0x44 19, GetInterface 16, FindEntry 16, GetModeManager 15, PDef@0x70 12, SetCurrentAction 11,
   ScriptName@0x74 8, PBaseDef@0x70 8, SetAsDrawable 8, GetUID 8, ctor 7, PTCDriver@0x5c 7, GetTCConst 7, GetStateFlags 7,
   GetPhysicsRadius 6, GetInterfaceUnchecked 6, ...
   Most are derived-class members declared on the CThing stand-in. They need the derived class's header (CThingPhysical,
   CThingCreatureBase), not more CThing members. Bitfield WRITES (C2106 x7) need FableWin's inline setters.
3. closure_model 461: CDefPointer pointees need engine/CDefPointeeBase.h models.
4. quarantine_model 104, searchtools_template 103, own_definition 39, template_instance_row 38 (+7 `$L` rows), unresolved_call 34,
   closure_derived 31, nested_in_namespace 25, local_base 25.
   searchtools_template and template_instance_row were NOT attempted in phase 7 (header_lacks took the wave). Leads as in
   phase 6 NEXT.md items 5 / 6: a second IsIncludedInThingScans expression form, and an unchecked bitset test (_Unchecked_test)
   for HasTC.
5. Gate failures of the w2 rewrites:
   - compile_fail 177: C2440 47, C2039 27, C2662 17 const propagation, C2065 8, C2106 7, C2027 7, ...;
   - bytes_differ 94, targets_differ 13;
   - consistency 47, mostly CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound: QAE / QBE are bound at several addresses
     (0x0040F020, 0x004365B0 and hashes). Decide one spelling per address first; that unblocks those rows and the 146 + 28
     NAME_CLASH notes the gate prints on every CThing row;
   - grade 9: DIFFER(40v39) on the landed text as well, a catalog-length question; they can pass as grade-equal.

## CIntelligentPointer: what is left
- 131 of the w3 candidates are deferred (w3_cintelligentpointer/DEFERRED.tsv):
  * 21 static_cast failures where T does not derive CBaseObject in the row's TU (migrate T's model first);
  * 58 non-canonical base models;
  * 29 local templates with operator T* / operator->.
- 1,677 rows with a local CIntelligentPointer model do not include engine/CThing.h: the same rewrite once their T is complete
  (CBaseObject.h + the T header).

## Wave 4 (not started): CTCBase 1,223 / CThingCreatureBase 1,071
Same method as CThing: a hdrmig7-style rewriter with the class's FIELD / BITACC / MACRO_OF tables. Do CThingCreatureBase after
CThing's header_lacks, because many header_lacks members are CThingCreatureBase's.

## Tooling (handback/tools; each documented in its header)
Phase 6 tools, plus:
- hdrmig7.py: --base, bitfield positions, the MOREACCESS / OPERATORNEW rules;
- w7_targets.py: per-target spelling survey;
- w7_respell.py: mechanical const / namespace respells;
- w7_nullcoord.py;
- includers.py: the landed includers of a header;
- ipmig7.py: the CIntelligentPointer rewrite with the ASSIGN / COPY opt-ins;
- handback.py --prior-hb: do not repeat an earlier wave's CALLERS / headers.
vlandwave.py derives its work dir from --tag: give every concurrent run its own tag.
