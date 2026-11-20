# NF155 notes (round 10; PRIORITY pick list CThingCreatureBase / CThing / CThingManager; 18 entries)

Counts (from APPLIED.tsv / STATUS.tsv): 18 entries: 6 done (q4745, q5074, q5082, q11682, q11683, q11684), 3 done-already (q4980, q5073,
q6195), 6 blocked with rows listed (q4046, q4259, q4412, q4723, q5083, q5165), 3 open (q4314, q5085, q5206: no row attempted), 0 rejected.
Rows: 18 ported (3 MATCH + 15 RELOCATION_MATCH, every test PASS), 15 not-a-binder (the entry self rows, already on the claimed leaf),
32 blocked (31 port + 1 identity; 30 of the port rows were not attempted, see below), 4 open (not attempted: 0x00521AE0, 0x00903920,
0x00903970, 0x006A4D60), 2 deferred (held by NF153: 0x00935160, 0x00957F20).
Rows not attempted inside partial entries: q4259 1, q4412 23, q4723 5, q5083 1 (0x00834760 is shared by q4259 / q5083), q4046 1
(0x008F7BD0). Resume them rather than park them.

## Census (how every "left" list was built)
- Every REFS row of every entry except q4046 (300 rows) was compiled at HEAD (vland --recheck-landed, all 300 RELOCATION_MATCH PASS) and
  its (symbol, retail target) pairs taken with reloc_identity.check() (work/pairs2.py, read-only; work/base*_pairs.json, work/bind_base.txt).
- q4046 (879 rows): 686 from the newest cached object of each row under $S (work/census.py), 193 by a scope-aware source scan of the
  GetPos / GetPosition declarations (work/scope.py, getpos_scope.txt); the 24 suspects were compiled in the HEAD probe. 873 bind the claim.
- The table and cached objects were stale in places (0x005252B0's cached object showed the old CThingVector spelling), so only the compile
  probe decided.

## Self renames (RENAMES section 1)
- 0x006620E0 / 0x006620F0 / 0x00662100: CThingCreatureBase::GetDefHover{Slow,Medium,Fast}Speed (both PDBs), on engine/CThingCreatureBase.h
  with NF128's landed GETDEFHOVERSPEEDS opt-in and the 0x00662010 define set; tests rewritten on the shared headers (they link the row
  object; PassPatterns kept). Old leaves from HEAD objects. Only referencing row 0x008EA500 already binds the new spellings.
- 0x00692930 CCreatureActionBase::GetPlayer and 0x00692990 CCreatureActionBase::PeekThingSearchTools (both were the free `Dest_val`
  stand-in, the same fix NF30 made for the twin 0x00692950 PeekWorld). Exe scan of each address: 0x00692930 is called by 0x008424A0 (binds
  the new spelling) and 0x00846410 / 0x008424D0 / 0x00846100, which bound pointer stand-ins and are respelled in place here (their
  existing local CCreatureActionBase), so 0x00692930 ends on one spelling. 0x00692990 is called by 0x008BC1A0 (new spelling), 0x008C2240
  (the CCombatActionBase stand-in ?GetThingSearchTools@CCombatActionBase@@QBEPAVCThingSearchTools@@XZ; its local CCombatActionBase has
  no CCreatureActionBase base, so it is in the q4412 port family; NAME_FIXES) and 3 sites with no catalog row. The rename removes the
  Dest_val spelling and adds none (no split created). DECISION for the coordinator: 0x00692990's self rename while 0x008C2240 still binds
  a stand-in (the spelling count falls 3 -> 2, nothing grows).
  Their tests are standalone behaviour models of the 15-byte forwarder (no stand-in linked); kept unchanged, as NF30 did for 0x00692950.
- No EH funclets include any changed row. No other landed row defines any of the five new self leaves (source grep; the two hits are comments).
- Defined-symbol diff, HEAD vs hand-back objects (work/head_*.sym vs work/final_*.sym): no change in any binder row; the two self
  re-ports change only their own leaf (Dest_val -> the PDB spelling).

## Binder ports (RENAMES section 2)
- In-place respells inside local models the rows already had: 0x007038D0, 0x008330D0 (GetPWorldSeed, protected const, moved into the row's
  existing local CThing), 0x006AC140 (GetThingSearchTools IAE + reference into the local CThing; a forward `class CThingSearchTools;` was
  needed), 0x009473E0 / 0x00948060 (const building, PBV), 0x0084A7E0 (local CThingManager::GetThingSearchTools; the static
  AlertAllNearby declared on the row's header-less local CCreatureAction_AlertAllNearbyToThing, binding the landed self spelling of
  0x0084A720: 0x0084A720's spelling count falls 2 -> 1), 0x008C5490 / 0x008C74F0 (the existing local CCreatureActionBase model re-keyed
  struct -> class with `public:`; their emitted vtable now also names Clone with the PDB V key, ?Clone@CCreatureAction_RockTrollShakeAndExplode@@UBEPAVCCreatureActionBase@@XZ = Ego_r 0x008FF450).
- 0x00846410 / 0x00846100 / 0x008424D0: the existing local CCreatureActionBase model's `CPlayer* GetPlayer() [const]` respelled in place to
  the protected `CPlayer& GetPlayer()` (Ego_r + FableWin IAEAAV), uses `GetPlayer().X`.
- Shared-header ports: 0x00744730 (engine/CThing.h GetPos; local Vec3 -> C3DVector; a `// Leaf:` line added, the row had none),
  0x00927C80 (quarantine CThing.h opt-in; Leaf line added; the pointer locals became `const C3DVector*`).
- 0x00692930 also moved 0x00A01B50 from the stand-in ?dv_h1@@YIPAXPAX@Z to the common ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ.

## Blocked rows (not attempted: each needs a port that also moves the row's own leaf or a dozen local classes)
- q4412, 17 rows bind ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCombatActionBase@@@Z: their local CCombatActionBase model has no
  CCreatureActionBase base and re-basing it is not allowed; they must move onto engine/CCombatActionBase.h (its retail branch is behind
  per-family macros) + CCreatureActionBase.h + the derived action headers. 4 rows (0x007ED0A0..0x007ED870) declare it on a local
  CThingAICreature : CThing; 2 (0x007C9180, 0x00920960) on the namespaced stand-in N_0062e900::CCreatureActionBase.
- q4723 family (0x00917330, 0x009429C0, 0x00966A90, 0x009681A0, 0x0090FC50): the port (template 0x00946400) also changes the self leaf
  to the PDB ?InitCreaturePosition@<class>@@MAE_NXZ (NAME_FIXES); retail returns `mov eax,1`, so the bool-return form is a codegen risk.
  Hand these over as one family.
- 0x00871D30 (q4046): identity, not CTCLook: it calls NCameraModes::CBase::GetObservedThing 0x0084F1B0 on this (5 spellings already).
- 0x005252B0 (q5165): NF108 measured a declaration-only RuntimeSaveEntities call at DIFFER(314v312); the nf96a same-TU strong copy must
  carry the owner 0x005239C0 body verbatim, and this row's copy is a made-up body.
- 0x00903920 / 0x00903970 (q5085): CThing_Retail stand-in and _Methods stand-in self leaves; template 0x008916A0. 0x006633A0 is
  ClearCurrentAction (NAME_FIXES).
- Every blocked row's reason is in STATUS.tsv.

## Flags lines
- None added.

## Descriptive names
- None new.

## Header claims needed
- None (add-only opt-ins; hdr/README.md).

## Decisions for the coordinator
- 0x00692990's self rename with 0x008C2240 still on a stand-in (above).
- 0x00957F20: apply work/00957f20_suggested.diff on top of NF153's version (two hunks).
- The q4723 family needs its self leaves moved to ?InitCreaturePosition@<class>@@MAE_NXZ (NAME_FIXES) together with the port.
- Locks I claimed beyond rows.txt: 00667310 007f04c0 00858cc0 008c52c0 008c8f70 008c91e0 008c92c0 008cf610 008cf700 008cf7d0 008df510
  008ec700 00924670 00924720 00924880 00924a90 007ed0a0 007ed170 007ed560 007ed870 00920960 009429c0 00966a90 009681a0 00903970 (left
  untouched; free them), and 00948060 008c5490 00692930 00846410 008424d0 00846100 (changed, in rows/).

## Rule slips
- 0x00957F20 is locked by NF153 (it is not in my rows.txt); I edited and graded it before checking the lock. The change is withdrawn from
  rows/. The draft (RELOCATION_MATCH PASS in recheck_final.log) and its diff are in work/ for the coordinator to apply on top of
  NF153's version. recheck_final.log was regenerated over exactly the 18 rows in rows/.
- 0x006AC140 spent one grade on a missing forward declaration (SRC_FAIL in vland b1).
- The q4046 census used cached objects for 686 rows rather than compiling them (time); see "Census".
