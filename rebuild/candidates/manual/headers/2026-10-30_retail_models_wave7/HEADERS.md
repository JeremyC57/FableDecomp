# Wave 7 headers: what changed and the evidence

No new class headers. Two wave 1 NScript headers changed one member each: a declared-only constructor became the
inline constructor retail expands. Layout, vtables and every slot are untouched.

| header | change | evidence | includers checked |
|---|---|---|---|
| engine/CLevelScriptBase.h | `CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}` (was a declaration) | below | 104 migrated rows that construct it (78 non-template + 26 template inline-ctor rows) mcheck SAME; 46 unchanged includers `--all` SAME |
| engine/CEntityScriptBase.h | `CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) {}` (was a declaration) | below | ~600 entity Alloc rows (family ESB) mcheck SAME; all 9 unchanged includers `--all` SAME |

Both headers also pass `g++ -m32 -w -std=c++98 -fsyntax-only` with a derived class constructed through each ctor.

## CLevelScriptBase<T> constructor: inline

- **Retail.** Every level-script constructor and every script Alloc expands it. The sequence is `call 0x00CB8110` (CScriptBase())
  followed at once by `mov [esi+40h],Game` and `mov [esi+44h],data`. The intermediate CLevelScriptBase<T> vtable store is
  dead and dropped. Examples: 0x00D61C30 (CQ_GuildTrainingWoodsDepartureScript ctor, 34 bytes) and the Alloc family
  0x00EED220 (`call 0x00CB8110; mov [esi+40h],edi; mov [esi+44h],ebx; mov [esi],vtable`).
- **PDBs.** Ego_r.pdb (release) has no `??0?$CLevelScriptBase` COMDAT, only `??1?$CLevelScriptBase<CQ_SunnyvaleMasterData>`.
  FableWin.pdb (debug) has the five out-of-line ctors.
- **The out-of-line copies.** Retail keeps five unreferenced out-of-line ctors, 0x00CDD160..0x00CDD220, one per T (vtables
  0x012C2EEC..0x012C2F3C, bound in reloc_bindings.tsv). Their shape is that of an explicit instantiation of the inline
  member. **Unproven:** they have no caller and there is no TU evidence. Those 5 landed rows keep their local model.
- **The cast.** The parameter is `CScriptDataBase*` (PDB) and the member is `T*` (PDB). T derives from CScriptDataBase at
  offset 0, so the C-style cast emits no code. It also compiles for an incomplete T.

### T: the master-data class per level script (scripts/lsb_T.py, regscripts.py, regmap.py)

Three sources:

1. **Ego_r.pdb entity layouts.** The `PMasterData` member type of each nested entity class gives T: 595 entity classes have
   CQ_SunnyvaleMasterData*, 35 CEmptyData*, and 3 each the three test data classes. The nesting comes from the
   `@<Entity>@<Script>@NScript@@` names in both PDBs.
2. **Retail CScriptInfoManager::RegisterScripts 0x00CD52D0.** Each AddScript 0x00CB5C90 gets a CScriptInfo with the script
   Alloc at +0 and the master-data Alloc at +4:

   | master-data Alloc | allocates | stores data vtable | T | scripts |
   |---|---|---|---|---|
   | 0x00CDBD20 | 0x144 bytes | 0x012C2748 | CQ_SunnyvaleMasterData | 122 |
   | 0x00CD4A60 | 4 bytes | 0x012C2718 | CalextestData | 2 |
   | 0x00CD4AC0 | 4 bytes | 0x012C2724 | CEmptyData | 11 |
   | 0x00CD4B20 | 4 bytes | 0x012C2730 | Cjoss_test_scriptsData | 3 |
   | 0x00CD4B80 | 4 bytes | 0x012C273C | CDanyalTestScriptsData | 6 |
   | `ebx` = 0 (no master data) | | | | 12 (CGlobal* / CExpression*) |

   The data vtable order matches the five retail CLevelScriptBase<T> vtables 0x012C2EEC (Sunnyvale), 0x012C2F00 (alextest),
   0x012C2F14 (Empty), 0x012C2F28 (joss), 0x012C2F3C (Danyal), which are bound in reloc_bindings.tsv. The script Alloc's
   vtable store names the script.
3. **The landed template rows.**

A row takes T when two sources agree, or when one source and the landed rows agree. The registration of the row's own
cited vtable counts as a source for the rows the PDBs do not cover.

**Conflicts found (not changed):**
- The landed template rows say CQ_SunnyvaleMasterData, but retail registers these scripts with another data class:
  CEmptyScript (00eee800, CEmptyData), CTestTheInventoryScript (00eeec00, CEmptyData) and CHeroTurningDebuggerScript
  (00efc300 / 00efc260, CDanyalTestScriptsData).
- CQ_HeroSoulsArenaScript's Alloc row 00d80ba0 says CEmptyData; every other row and the PDB entities say Sunnyvale.
- CExpression_FollowScript (00eea020) is registered with no master data.

T enters no symbol of these rows, so fixing them is a respelling, not a byte change (NEXT.md).

## CEntityScriptBase constructor: inline

- **Retail.** Every entity Alloc expands it. 0x00F1B800 (CQ_ArenaScript::CWhisperAlly::Alloc): `operator new(0x1C)`, then
  `mov [esi+4],[parent+40h]` (Game), `mov [esi],0x012C3224` (the CEntityScriptBase vtable, kept because a call follows),
  then `call 0x004ABE90` (the out-of-line CScriptThing copy into Me +0x08), then the derived vtable and members.
- **PDBs.** Ego_r.pdb has no `??0CEntityScriptBase`. FableWin.pdb has `??0CEntityScriptBase@@QAE@AAVCGameScriptInterfaceBase@@ABVCScriptThing@@@Z`
  at 0x035E6BC0 (debug, out of line).
- **Unproven / unchanged.** Main's access in the base (every override is EAE, private). Rows whose entity class is
  instantiated now declare the PDB overrides it lacked: `private: virtual void Main();` and
  `virtual CScriptBase* GetParentScript() const;`. Every entity class has `?Main@<entity>@..@@EAEXXZ` and
  `?GetParentScript@<entity>@..@@UBEPAVCScriptBase@@XZ` in the PDBs. Without them the class stays abstract under the
  header's pure slots.

## Headers used unchanged (first bulk use)

- **engine/CScriptThing.h** (wave 1, 79 slots), on 1,300-odd rows.
  - The slot names the rows used are checked against the header slot by slot: the row's virtual declaration order, with
    the dtor taking its own position.
  - Invented names were renamed at the call sites only when the row's signature equals the header's at that slot (same
    vtable offset, same call). Rows changed: IsValid -> IsAlive (slot 75) in 117, GetThing -> GetPThing (slot 11) in 163,
    and one row each for IsValid -> IsNull (77), GetPosition -> GetPos (6), Refresh -> UpdateThingAttachment (71) and
    Accepts -> MsgIsKilledBy (18).
  - When the row's stand-in returned another pointee type (`CThing_Retail*` / `CThingBuilding*`), the call is cast:
    `((CThing_Retail*)thing.GetPThing())` (80 rows). CThing has no shared header yet.
- **engine/CIsThingAlive.h, engine/CIsThingConscious.h, engine/CPredicateBase.h** (wave 5), on 8 predicate rows.
