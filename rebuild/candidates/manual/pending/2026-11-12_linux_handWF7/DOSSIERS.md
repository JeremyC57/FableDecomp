## 0x008B0B60

# DOSSIER 0x008B0B60 CCombatSequenceGeneric::OnCreateSpecial (432 bytes)

## 1. Retail shape (xdis 008b0b60 0x1c0; ends 0x8b0d0f, next fn 0x8b0d10 = CCombatManager::RegisterCombatSequences)
- thiscall, `ret 4` (one arg: const map&). `push ebp; mov ebp,esp; and esp,-8; sub esp,1Ch`: VC7.1 dynamic 8-byte
  alignment for the 8-byte locals (CCountedPointer / CCombatAction). Not an /Oy- marker. No SEH frame (no EH state; the TU
  has no unwinding for these locals). Default /O2, no size idioms. Pushes ebx, esi(=this), edi.
- Frame (esp-relative after prologue): 0x14 = CCharString name (branch 1) / spilled loop iterator (branch 2);
  0x18/0x1C = CDefString temp (branch 1) / CCombatAction action (branch 2, ebx/edi cache it);
  0x20/0x24 = CCountedPointer<CCombatSequenceBase> (branch 1) / GetPCombatActionFromEnum result temp (branch 2);
  0x13 = the __false_type tag byte of _M_insert_overflow.
- Control flow: `if (Def->ActionList begin==end)` -> special path is the fall-through, loop path at 0x8b0c0f. Loop re-reads
  begin/end, `je end; jmp body` (pre-test + jump into body), latch reloads `this->Def->ActionList._M_finish` ([esi+28h]->+58h)
  every iteration and keeps the iterator in memory ([esp+14h]). `CreateCalled = true` (+0x24) is tail-duplicated in both exits.
- Branch 1 order: def vslot 10 (+0x28) into hidden temp @0x18 -> GDefStringTable.GetString(temp.TablePos) into name@0x14 ->
  map._M_find(name) (0x6F1360) compared with `[map]` (header = end()) -> creator(ecx=Creature +0x1C, edx=Target +0x20) ->
  CCountedPointer<CCombatSequenceBase>(T*) ctor 0x6EF6F0 into @0x20 -> `seq->vslot1()` (+4) -> CombatActions(+0x10) =
  seq->CombatActions (0x8B3180) -> NoAvailableActions(+8) = seq->NoAvailableActions -> release (dec [Info]; jne; flags, Info in edi; no zero
  stores) -> ~CCharString(name) 0x99EAE0 (also on the not-found path).
- Branch 2 per element: action{0,0} (ebx/edi zero, stored to @0x18/0x1C), push Target, Creature, ecx=&tmp@0x20, edx=*it,
  call 0x857430; action = tmp (ShareData with known-null old value: only `if (tmp.Info) {copy; ++RefCount}`); tmp dtor
  (dec [Info]; RELOAD Info; cmp [Info],0 -- the shared header's split `--RefCount; if (RefCount==0)`), tmp zeroed
  (Data=0, Info=0 stored after the release, interleaved with push_back); inline push_back (finish!=eos -> placement copy
  with `if (finish)` null test, `add [vec+4],8`; else _M_insert_overflow(finish, action, tag, 1, 1) 0x8B3350);
  `++NoAvailableActions`; action release via flags (Info in edi); action slots zeroed at the latch.

## 2. Name / signature (two sources: PDB + vtable)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x008EC0F0) and FableWin.pdb (_fablewin_api.tsv 0x017CAF29 / 0x02A38720) both:
  `?OnCreateSpecial@CCombatSequenceGeneric@@UAEXABV?$map@VCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@ZU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@P6IPAVCCombatSequenceBase@@AAVCThingCreatureBase@@0@Z@std@@@5@@std@@@Z`
- rtti-vtables.tsv: CCombatSequenceGeneric vtable 0x01261D74 slot 19 = 0x008B0B60 (last slot; base vtable 0x01278FEC has no
  slot 19 -> pure in base). xref: only reference is that vtable (.rdata 0x01261DC0). Manifest label is CORRECT.
- `public: virtual void __thiscall CCombatSequenceGeneric::OnCreateSpecial(const std::map<CCharString,
  CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)>&)` (U = public virtual, not const).

## 3. Callees (landed file / landed name / PDB name; DRAFT MISMATCH flagged)
- def vslot 10 (+0x28): `virtual CDefString CDefClassBase::GetInstantiationName() const` (engine/CDefClassBase.h:90,
  Ego_r ?GetInstantiationName@CDefClassBase@@UBE?AVCDefString@@XZ). DRAFT WRONG: `GetSpecialName`, `CDefString::ID`
  (real member is private `TablePos`; use engine/CDefString.h, inline `operator CCharString()` -> GetString ->
  GDefStringTable.GetString(TablePos)). Same pattern as landed GetName 0x008B0700 (which itself still calls it GetNameStringId).
- 0x009D49B0 rebuild/src/compiled/00/9d/CDefStringTable_GetString_009d49b0.cpp: `CCharString CDefStringTable::GetString(long) const`
  (?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z), object 0x013CA828 = `GDefStringTable` (?GDefStringTable@@3VCDefStringTable@@A).
  DRAFT WRONG: CTextTable / g_TextTable, non-const.
- 0x006F1360 rebuild/src/compiled/00/6f/stdRbtree_Mfind_006f1360.cpp: STLport `_Rb_tree<...>::_M_find(const CCharString&) const`
  (behind inline map::find; node compared with _M_header). DRAFT WRONG: CSpecialMap::Find. Opt-in FABLE_RETAIL_STLP_TREE_MFIND_DECL
  (stlport/fable_map.h:177, declaration only) -- or use stlport/fable_map.h map with find.
- creator (map value, +0x14 of node): `CCombatSequenceBase* (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)`, args `*Creature, *Target`. Draft OK.
- 0x006EF6F0 rebuild/src/compiled/00/6e/CCountedPointer006ee3d0_CCountedPointer006ee3d0_006ef6f0.cpp: landed placeholder name
  `CCountedPointer_006ee3d0::CCountedPointer_006ee3d0(void*)`; real = `CCountedPointer<CCombatSequenceBase>::CCountedPointer(CCombatSequenceBase*)`
  (FableWin has CCountedPointer<CCombatSequenceBase> instantiation: ?ShareData@?$CCountedPointer@VCCombatSequenceBase@@@@...).
  NAME_FIXES candidate (one-source: this call site + FableWin instantiation). Opt-in FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR (decl-only ctor).
- vslot 1 (+4) of CCombatSequenceBase = OnCreate (CCombatSequenceGeneric slot 1 = 0x006F0750, FableWin ?OnCreate@CCombatSequenceGeneric@@UAEXXZ;
  OnStop row 0x008B0870 slot table also says slot 1 OnCreate). DRAFT WRONG: `Initialise`.
- 0x008B3180 rebuild/src/compiled/00/8b/stdvectorCCombatAction_operator_008b3180.cpp: `std::vector<CCombatAction>::operator=(const vector&)`
  (??4?$vector@VCCombatAction@@...@QAEAAV01@ABV01@@Z). Opt-ins: FABLE_RETAIL_STLP_VECTOR_ASSIGN (decl) / FABLE_RETAIL_STLP_VECTOR_C (visible body, still called).
- 0x00857430 rebuild/src/compiled/00/85/CCombatActionBase_GetPCombatActionFromEnum_00857430.cpp:
  `static CCountedPointer<CCombatActionBase> __fastcall CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType, CThingCreatureBase&, CThingCreatureBase&)`
  (?GetPCombatActionFromEnum@CCombatActionBase@@SI?AV?$CCountedPointer@VCCombatActionBase@@@@W4EActionRegisteredType@@AAVCThingCreatureBase@@1@Z; `ret 8`).
  DRAFT WRONG: free function, `long`, pointer params. Retail passes `*it, *Creature, *Target`; element type of ActionList is EActionRegisteredType.
- 0x008B3350 rebuild/src/compiled/00/8b/stdvector_Minsertoverflow_008b3350.cpp: vector<CCombatAction>::_M_insert_overflow(pos, x, const __false_type&, 1, true).
  FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY exists; PushAction 0x8B0B10 uses a same-TU noinline stand-in so the tag homes right.
- 0x0099EAE0 CCharString::~CCharString (rebuild/src/compiled/00/99/CCharString_CCharString_0099eae0.cpp, fable_string.h).
- 0x00BFEA9BC operator delete (global_operatordelete_00bfe9bc.cpp); Info->DeleteFunc indirect (__fastcall, ecx=Info->Data).

## 4. Members (retail offsets; Ego_r offsets in struct_layouts_egor.tsv are +4 after the 16-byte debug vector)
- CCombatSequenceBase (NO shared header; model locally like landed 0x006F1770 / 0x008B0B10): Running bool +4, NoAvailableActions long +8,
  CurrentActionIndex long +0xC, CombatActions std::vector<CCombatAction> +0x10 (12 bytes STLport), Creature CThingCreatureBase* +0x1C,
  Target +0x20, CreateCalled bool +0x24; sizeof 0x28. OnCreate is vslot 1, OnCreateSpecial slot 19.
- CCombatSequenceGeneric: engine/CCombatSequenceGeneric.h with `#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC` (needs
  CCombatSequenceBase defined before and `#define FABLE_RETAIL_DEF_CLASS_BASE` + engine/CDefPointer.h): `CDefPointer<const CCombatSequenceDef> Def` +0x28.
  The header class has only the ctor declared -- OnCreateSpecial must be added (header is shared; probably needs a header
  edit or an hdr/ overlay as chunk 710 did).
- CCombatSequenceDef (Ego_r name; no shared header): ActionList std::vector<EActionRegisteredType> retail +0x54 (Ego_r +88). DRAFT WRONG name `Actions`.
  Base CDefClassBase (engine/CDefClassBase.h) gives GetInstantiationName slot 10.
- CCombatAction (Ego_r 8 bytes): `CCountedPointer<CCombatActionBase> CreatureAction` +0; no user members in either PDB -> implicit
  ctor/copy/dtor. engine/CCountedPointer.h: Data +0, Info +4; CCPPointerInfo RefCount +0, DeleteFunc +4, Data +8. The header's default
  dtor already is retail's split `--RefCount; if (RefCount == 0)` + `Data = 0; Info = 0`.
- map node: CCharString key +0x10, creator +0x14 (stlport/fable_map.h _Rb_tree_node).
- Locally modelled in draft though a shared header exists: CCountedPointer (engine/CCountedPointer.h), SCountBlock (=CCPPointerInfo),
  CCombatSequenceGeneric (engine/CCombatSequenceGeneric.h), CDefString (engine/CDefString.h), CDefStringTable (engine/CDefStringTable.h),
  CCharString (fable_string.h / FABLE_RETAIL_CCHARSTRING), std::map / vector (stlport/fable_map.h, fable_containers.h), CDefPointer (engine/CDefPointer.h).

## 5. Visibility
- CCombatSequenceBase::PushAction 0x008B0B10 (out of line, 78 bytes) sits immediately before this function = same TU, defined earlier.
  Its body (push_back + ++NoAvailableActions) is EXACTLY retail's loop tail -> retail likely wrote `PushAction(action);` and /Ob2
  auto-inlined it (or wrote the two lines). Try both.
- Out-of-line: _M_find 0x6F1360, vector::operator= 0x8B3180, _M_insert_overflow 0x8B3350, CCountedPointer<CCombatSequenceBase>(T*) 0x6EF6F0
  (all COMDATs at their own addresses; 0x8B3180/0x8B3350 are in this TU's COMDAT run after 0x8B32xx base vtable stubs).
- Inline (expanded): map::find, push_back, CCountedPointer copy/assign/dtor, CDefString->CCharString conversion, GDefStringTable.GetString call.
- CCombatSequenceBase ctors/dtor 0x008B0A60/0x008B0AB0, PushAction 0x008B0B10, OnCreateSpecial 0x008B0B60 contiguous -> one source file (CombatSequence*.cpp).

## 6. Siblings to copy idioms from (none carry `// Flags:`; default /O2)
- rebuild/src/compiled/00/8b/CCombatSequenceBase_PushAction_008b0b10.cpp: engine/CCountedPointer.h, local CCombatAction, local STLport
  vector with `#pragma auto_inline(off)` noinline _M_insert_overflow stand-in (tag homing). Closest model for branch 2.
- rebuild/src/compiled/00/6f/CCombatSequenceGeneric_CCombatSequenceGeneric_006f1770.cpp: FABLE_RETAIL_DEF_CLASS_BASE + engine/CDefPointer.h,
  local CCombatSequenceBase (0x28), FABLE_RETAIL_CCOMBATSEQUENCEGENERIC.
- rebuild/src/compiled/00/8b/CCombatSequenceGeneric_GetName_008b0700.cpp: def vslot 10 -> GDefStringTable.GetString idiom (branch 1 head).
- rebuild/src/compiled/00/8b/CCombatSequenceBase_CCombatSequenceBase_008b0ab0.cpp (auto_inline off/on), CCombatSequenceGeneric_OnStop_008b0870.cpp
  (def layout at retail offsets, ActionList +0x54, creature/target fastcall).
- rebuild/src/compiled/00/85/CCombatActionBase_GetPCombatActionFromEnum_00857430.cpp: the callee's exact declaration.

## 7. Old draft model errors (vs disassembly)
- Wrong names (GetSpecialName, Initialise, CTextTable/g_TextTable, CSpecialMap::Find, GetPCombatActionFromEnum as free fn with long+pointers,
  Def->Actions) -- see section 3.
- Two different counted-pointer types (CCountedPointer + non-clearing CSequencePointer): use the one shared engine/CCountedPointer.h; its
  zeroing stores are dead for the branch-1 local and survive (as retail) for the escaped branch-2 locals.
- Local action modelled as CCountedPointer<CCombatAction> (pointee wrong): retail element is CCombatAction{CCountedPointer<CCombatActionBase>}.
  Retail: `CCombatAction action; action.CreatureAction = CCombatActionBase::GetPCombatActionFromEnum(*it, *Creature, *Target);`
  (zero-init then ShareData) then PushAction/push_back.
- Loop end must be re-read through this->Def each iteration (Def is a CDefPointer member; draft's `const CCombatSequenceDef* Def` raw
  pointer + `long*` iterator; ActionList is vector<EActionRegisteredType>, iterate with its const_iterator).
- Residue reported: this in edi vs retail esi and zero hoisted into ebx -- consistent with the wrong local types/aliasing above; the
  map argument is loaded into edi only after GetString in retail (`mov edi,[ebp+8]` at 0x8b0b99).

## 0x009486D0

# DOSSIER 0x009486D0 -- CAIStateGroup_GoToPub::FreeSeat (432 bytes)

## 1. Retail shape (xdis 009486d0 0x1c0)
- __thiscall, plain `ret` (no args), void. No frame pointer (/Oy default; no `push ebp; mov ebp,esp`), no SEH prologue even
  though two CIntelligentPointer temporaries with dtors are built -> no EH state (or nothing between ctor and dtor counts as throwing). Siblings
  carry no `// Flags:` line and no `#pragma optimize`: default flags.
- Frame `sub esp,8` = one 8-byte slot shared by: the GetTC key temps (0xB8 / 0x46 / 0xBA), the miss-path reads of the uninitialised
  component out-vars (`mov eax,[esp+8]`, `mov ebp,[esp+10h]`, `mov eax,[esp+10h]`), and both CIntelligentPointer temporaries.
- Pushes ebx, esi at entry; ebp, edi only after the first lookup (0x94873A, shrink-wrapped, inside the outer `if`).
  The outer-if miss goes straight to `pop esi; pop ebx; add esp,8; ret` at 0x94887A.
- Register allocation: ebx=this, esi=&PTable then scratch, ebp=carrying component (live over the tankard block), edi=&PTankard (this+0x34).
- Constants: `cmp dword ptr [ebx+64h],-1` is an IMMEDIATE compare; the -1 for both stores is materialised once, LATE: `or esi,-1`
  at 0x948802 (after the tankard block, on the join), stored `mov [ebx+64h],esi` immediately and `mov [ebx+3Ch],esi` at the very
  end between `pop edi` and `pop ebp`. This is the only thing the old draft got wrong (it got ebp=-1 hoisted for compare + both stores).
- Control flow:
  if (PTable.GetPItem() && ActionPointIndex(+0x64) != -1) {
     table = PTable.GetPItem()->GetTC(0xB8)  (bit 24 of +0x34, LowerBound 0x40F020, `it==end || 0xB8<it->first ? end : it`)
     table->FreeSeat(SeatIndex(+0x3c), ActionPointIndex(+0x64))   // push [+64] then push [+3c] => arg1=+0x3c, arg2=+0x64
     carrying = PBrain(+4)->Creature(+0x18)->GetTC(0x46)  (bit 6 of byte +0x28)
     if (PTankard.GetPItem() && !(PTankard.GetPItem()->Dead /*+0x91 bit0*/)) {   // IS_THING_ALIVE, 2 GetPItem calls
        if (carrying->GetThingInPrimarySlot() == PTankard.GetPItem()) carrying->RemoveThingFromPrimarySlot(false);
        tankard = PTankard.GetPItem()->GetTC(0xBA) (bit 26 of +0x34); tankard->ReplaceTankard();
     }
     ActionPointIndex = -1;
     PTankard = CIntelligentPointer<CThing>(0);       // ctor 0xA01B10, vptr 0x01238C6C, SetPItem(0), GetPItem(temp), SetPItem(&PTankard), dtor 0xA01C10
     PTable   = CIntelligentPointer<CThingObject>(0); // same, vptr 0x0125A858, destination recomputed as `lea ecx,[ebx+24h]`
     SeatIndex = -1;
  }
  Temporaries' dtor is a direct call to 0xA01C10 with no derived vptr store.

## 2. Name / class / signature
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv, Ego_r 0x0096C400): `?FreeSeat@CAIStateGroup_GoToPub@@IAEXXZ`.
- FableWin.pdb (ghidra_out/_fablewin_api.tsv, 0x0180FCB4): same `?FreeSeat@CAIStateGroup_GoToPub@@IAEXXZ`.
- => `protected: void __thiscall CAIStateGroup_GoToPub::FreeSeat(void)`, non-const, non-virtual (IAE = protected). Manifest label
  correct. Declare it under `protected:` or the leaf mangles QAE. (Landed OnReplace 0x009498B0 local model declares it public --
  irrelevant to its own leaf, but do not copy that access.)
- Retail placement: between InitCreaturePosition 0x00948060 and PlayAnims 0x00948D40 -- the CAIStateGroup_GoToPub TU
  (Ego_r order GetSeat 0x96BF70, FreeSeat 0x96C400, Drink 0x96C5E0 ... same file).
- Callers (xref.py): 0x00949902 (landed OnReplace 0x009498B0, calls `FreeSeat();`), 0x00949EE3, 0x0094A0A9, 0x0094A0C5, 0x0094B267
  (unlanded GoToPub methods, likely Drink/ChooseAction/Update/WalktoSeatAndSit range).

## 3. Callees (landed file | landed name | PDB name | draft spelling)
- 0x00A01B50 rebuild/src/compiled/00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const |
  ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returned CThing* -- WRONG type, PDB returns CBaseObject*.
- 0x00A01B10 ..._CBaseIntelligentPointer_00a01b10.cpp | ??0CBaseIntelligentPointer@@QAE@XZ | ok.
- 0x00A01B90 ..._SetPItem_00a01b90.cpp | ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z | draft took CThing* -- WRONG.
- 0x00A01C10 ..._CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (VIRTUAL dtor) | draft non-virtual -- WRONG.
- 0x0040F020 rebuild/src/compiled/00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound
  (const ETCInterfaceType&) | draft local CInterfaceMap::LowerBound(const long*) -- use engine/CVectorMap.h via GetTC.
- 0x007B8AA0 rebuild/src/compiled/00/7b/CTCTavernTable_FreeSeat_007b8aa0.cpp | CTCTavernTable::FreeSeat(long seat, long point) |
  ?FreeSeat@CTCTavernTable@@QAEXJJ@Z (Ego_r 0x007B7A20) | draft "CTCSeatingArea::FreeSeat" -- WRONG CLASS. Interface 0xB8 = TCI_TAVERN_TABLE
  (rebuild/include/engine/ETCInterfaceType.h:189). engine/CTCTavernTable.h exists but does NOT declare FreeSeat (has StatGetInterfaceType,
  GetServingPoint, FreeServingPoint, opt-ins FABLE_RETAIL_CTCTAVERNTABLE_SEATS / _LAYOUT) -> declare it locally or add an opt-in.
- 0x006A1440 rebuild/src/compiled/00/6a/CTCCarrying_GetThingInPrimarySlot_006a1440.cpp | ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ
  (CONST) | draft non-const -- WRONG. engine/CTCCarrying.h:17 declares it const (unconditional).
- 0x006A1E00 .../CTCCarrying_RemoveThingFromPrimarySlot_006a1e00.cpp | ?RemoveThingFromPrimarySlot@CTCCarrying@@QAEPAVCThing@@_N@Z | draft ok
  (returns CThing*). engine/CTCCarrying.h:18. Interface 0x46 = TCI_CARRYING (ETCInterfaceType.h:75).
- 0x00947C30 rebuild/src/compiled/00/94/CTCTavernTankard_ReplaceTankard_00947c30.cpp | ?ReplaceTankard@CTCTavernTankard@@QAEXXZ | ok.
  0xBA = TCI_TAVERN_TANKARD (ETCInterfaceType.h:191). No engine/CTCTavernTankard.h -> local decl.
- Vtables in temporaries: 0x01238C6C = CIntelligentPointer<CThing>, 0x0125A858 = CIntelligentPointer<CThingObject> (also cited in landed OnReplace 0x009498B0).

## 4. Members (Ego_r struct_layouts_egor.tsv, CAIStateGroup_GoToPub size 168; no engine/CAIStateGroup_GoToPub.h)
- +0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- engine/CAIStateGroupBase.h (opt-ins FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS], _GETCREATURE).
- CAIBrain +0x18 = `CThingAICreature* Creature` (Ego_r; the draft's "PCreature" is a made-up spelling) -- engine/CAIBrain.h,
  inline GetCreature() under FABLE_RETAIL_CAIBRAIN_GETCREATURE; CAIStateGroupBase::GetCreature() under FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE.
- +0x24 `CIntelligentPointer<CThingObject> PTable` (draft "PSeating" -- WRONG name).
- +0x34 `CIntelligentPointer<CThing> PTankard` (ok).
- +0x3c `long SeatIndex` (draft "SeatAreaIndex" -- WRONG).
- +0x64 `long ActionPointIndex` (draft "SeatIndex" -- WRONG; the draft's HasSeat() is invented).
- Full layout (for a local model): PPub +0x14 <CThingBuilding>, PBar +0x1c <CThingObject>, PTable +0x24, PBarman +0x2c <CThingAICreature>,
  PTankard +0x34, SeatIndex +0x3c, Pos +0x40 C3DVector, RHSet +0x4c, ActionPointIndex +0x64, NextFramePub +0x68, FrameLeavePub +0x6c,
  Sitting +0x70, BarActionPointIndex +0x74, BarPos +0x78, BarRHSet +0x84, BarIndex +0x9c, HandsOffMode +0xa0, UnderScriptControl +0xa1,
  TimeBeforeAskingForBeer +0xa4.
- CThing: presence bitset +0x20 (UsingComponents), Components CVectorMap +0x44, Dead = +0x91 bit 0 (CThing::IsDead()/IsAlive() in engine/CThing.h:280/289).
  GetTC body: `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` before engine/CThing.h (template GetTC(ETCInterfaceType, T**), CThing.h:340) -- exactly
  the retail lookup shape incl. constant key compare `cmp [eax],0B8h ; jle`.
- Classes the draft models LOCALLY that have shared headers: CThing (engine/CThing.h), CThingObject (engine/CThingObject.h),
  CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER), CAIBrain (engine/CAIBrain.h),
  CAIStateGroupBase (engine/CAIStateGroupBase.h), CTCCarrying (engine/CTCCarrying.h), CTCBase (engine/CTCBase.h), the interface map
  (engine/CVectorMap.h), CTCTavernTable (engine/CTCTavernTable.h; draft called it CTCSeatingArea).

## 5. Callee visibility / opt-ins
- All real callees are out of line in retail (CBaseIntelligentPointer 4 members, LowerBound, TavernTable::FreeSeat, Carrying x2, ReplaceTankard).
- ReplaceTankard 0x00947C30 is in the SAME TU, earlier (just before SitIdle 0x00947CA0) -- defined earlier in the source file, so VC7.1 could
  inline it; retail called it out of line. If a body were visible, guard with auto_inline(off); a declaration-only model avoids the issue.
- Inline (expanded) in retail: GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED), IS_THING_ALIVE, CIntelligentPointer(T*) converting ctor
  (vptr store + SetPItem(0)), CIntelligentPointer::operator= (GetPItem then SetPItem: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN gives
  `CBaseObject* item = other.GetPItem(); SetPItem(item);`), implicit ~CIntelligentPointer -> 0xA01C10.
- engine/CIntelligentPointer.h opt-ins: FABLE_RETAIL_CINTELLIGENTPOINTER (packed, real PData), _ASSIGN, _COPY, _CONVERT, _CTOR_OUTOFLINE,
  _ASSIGN_OUTOFLINE, _COMPARE_NULL_INT (operator==/!=(ptr,int) "used by IS_THING_ALIVE"), _GET_ORIGINAL_CHAIN. Get() = static_cast<T*>(GetPItem()).
  Note: the header's CIntelligentPointer has NO user-declared virtual of its own; retail's temporaries store a derived vptr (0x01238C6C /
  0x0125A858) so CIntelligentPointer<T> must have its own vtable -- the virtual dtor inherited from CBaseIntelligentPointer gives that.
- No STLport helpers involved.

## 6. Sibling landed rows (same TU, rebuild/src/compiled/00/94/)
- CAIStateGroupGoToPub_OnReplace_009498b0.cpp (497 B, the caller): FABLE_RETAIL_CCHARSTRING + FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CThing.h,
  engine/CVectorMap.h, LOCAL CBaseIntelligentPointer/CIntelligentPointer (FABLE_FORCEINLINE converting ctor + operator=), local GoToPub
  layout with the Ego_r names (PTable/PTankard/SeatIndex...); resets written `PTankard = 0; ... SeatIndex = -1;`.
- CAIStateGroupGoToPub_FreeBarPoint_00949320.cpp (93 B): GETTC_EXPANDED + `#define FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h;
  `m_pBar.Get()->GetTC(TCI_TAVERN_BAR, &p); p->FreeBarPoint(idx); idx = -1;` -- closest idiom for the table lookup.
- CAIStateGroupGoToPub_IsStaffed_00947fb0.cpp / FallAsleep_0094a0e0.cpp: `inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
  { if (p.GetPItem()) { if (!p.GetPItem()->IsDead()) return true; } return false; }` (IsStaffed with FABLE_RETAIL_CINTELLIGENTPOINTER).
- CAIStateGroupGoToPub_GetBar_009490d0.cpp: macro form `#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())`, engine/CAIStateGroupBase.h.
- CAIStateGroupGameMaster_OnReplace_0094ca80.cpp: same pointer/IS_THING_ALIVE local model; GameMaster derives the GoToPub seat base.
- None of these carry `// Flags:` or optimize pragmas (only auto_inline(off/on) around some helper bodies, pack(push,1) in InitCreaturePosition).

## 7. Old draft's model errors
- Control flow and loop form are right; the residue (436 v 432) is the -1 constant: VC hoisted -1 into ebp for the compare and both
  stores. Retail: compare immediate, -1 in esi only after the tankard block. Things to try: (a) write the stores as retail's member
  names ActionPointIndex/SeatIndex directly (no HasSeat inline); (b) real header types so GetPItem returns CBaseObject* and the
  CIntelligentPointer temporaries come from engine/CIntelligentPointer.h (virtual dtor, packed class) -- the changed register
  pressure / temp lifetimes (carrying must land in ebp, &PTankard in edi) can move the -1 CSE; (c) the order of the four resets
  (retail emits ActionPointIndex store first, SeatIndex store last, matching OnReplace's `... SeatIndex = -1;` tail idiom);
  (d) GetThingInPrimarySlot const.
- Wrong names: PSeating->PTable, SeatAreaIndex->SeatIndex, SeatIndex->ActionPointIndex, CTCSeatingArea->CTCTavernTable,
  PCreature->Creature (or GetCreature()), m_flags91->Dead/IsDead().
- Wrong types: GetPItem/SetPItem CThing* (PDB CBaseObject*), non-virtual ~CBaseIntelligentPointer (PDB UAE virtual),
  LowerBound(const long*) (PDB const ETCInterfaceType&), non-const GetThingInPrimarySlot.
- Local models of CThing/CIntelligentPointer/CAIBrain/CAIStateGroupBase/CTCCarrying where shared headers exist (verifier warns).

## 0x0094F480

# DOSSIER 0x0094F480 (437 bytes): CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable

## 1. Retail shape (xdis 0094f480 0x1c5)
- thiscall, no args, plain `ret` (0x94f629 / 0x94f634). Returns bool (al).
- NO frame pointer: `sub esp,48h ; push ebp ; mov ebp,ecx` -- ebp is `this` (FPO, /Oy on). No SEH (no /EH frame). Not /Os.
- ebx is pushed LATE (0x94f500, right before the first GetFacingVector call) and popped early (0x94f5c9, before the fcomp).
  ebx = PBrain->Creature, loaded once at 0x94f54b and reused for creature->PhysicsTC->GetFacingVector() (0x94f569) and
  creature->GetPos() (0x94f573). edi = enemy, esi = enemy->PhysicsTC then the facing result pointer.
- Two epilogues: success (pop edi/esi, mov al,1, pop ebp) and a SINGLE shared false epilogue at 0x94f62c (plus `fstp st(0)` at
  0x94f62a for the dist>=30.25 path). Every early-out jumps to 0x94f62c.
- One stack slot [F+0xc] (F = esp after push edi) is reused for: the GetTC key local (=8), the `approach` float, and the
  CIntelligentPointer temporary (8 bytes).
- Constants: [0x0122F59C]=0.5f, [0x01293350]=30.25f, [0x01293354]=9.0f.
- Flow: GetTC(TCI_TARGETING=8) (bitset test `mov ecx,[eax+20h]; test ch,1`; key store after the test; map via esi=&Components,
  end via `mov esi,[esi+4]`; `cmp eax,esi; je; cmp [eax],8; jle; mov eax,esi` = conditional-expression form; `mov ecx,[eax+4]`)
  -> enemy = targeting->vslot29 (+0x74) -> `if(!enemy) false` -> `if(enemy->Dead) false` (test byte [edi+91h],1)
  -> NCreatureAI::IsAwareOfThing(PBrain->Creature, enemy, false) (fastcall ecx,edx,push 0) -> movement = enemyPT->Position -
  enemyPT->OldPosition (X,Y,Z stored) -> enemyPT->GetFacingVector() (result at F+0x3c, NEVER read; call kept) ->
  myFacing = PBrain->Creature->PhysicsTC->GetFacingVector() (F+0x24) -> toEnemy = enemyPT->Position - creaturePT->Position
  (only X,Y stored: F+0x18,F+0x1c; Z dead) -> ahead = creature->GetPos() + creature->PhysicsTC->GetFacingVector()
  (GetFacingVector called BEFORE GetPos; Z,Y as `facing+pos` (fld [esi+8]; fadd [eax+8]), X as `pos+facing` (fld [eax]; fadd [esi]))
  -> approach = dot(myFacing, movement) computed Z,Y,X (fld f.Z fmul m.Z; fld f.Y fmul m.Y; faddp; fld f.X fmul m.X; faddp)
  -> enemy->vslot81(+0x144)(ahead) * 0.5f ; `if (!(approach > that)) false` (fcomp vs approach; test ah,5; jp)
  -> d2 = Y*Y + X*X of toEnemy (Y loaded first: default one-line `X*X+Y*Y` GetSquaredMagnitude2D does that)
  -> `if (d2 < 30.25f && d2 > 9.0f) { PTarget = enemy; return true; } return false;`
  -> PTarget = enemy: out-of-line ctor 0x00668B60 (push edi; ecx=temp), operator= 0x00668B80 (ecx=this+0x14), dtor 0x00A01C10 on temp.

## 2. Identity
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00973E00) and FableWin.pdb (_fablewin_api.tsv 0x0179E113 / 0x02BD7080,
  FableWin.exe.signatures.tsv): `?IsRunnable@CAIStateGroup_BalverineJumpAtFleeingEnemy@@MAE_NXZ` -> protected virtual
  `bool IsRunnable()` (non-const, MAE). Manifest label is RIGHT.
- Vtable: rebuild/manifest/rtti-vtables.tsv CAIStateGroup_BalverineJumpAtFleeingEnemy vtable 0x0128C0F8 slot 11 = 0x0094F480
  (slot 13 InitStateGroup 0x0094F170, slot 12 Update 0x0094F730, slot 8 GetStateGroupType 0x00900572); CAIStateGroupBase.h documents
  slot 11 = IsRunnable. xref: only reference is the vtable (.rdata 0x0128C124 = 0x0128C0F8+11*4). No direct callers.
- Class layout (struct_layouts_egor.tsv / engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h): size 0x20, base CAIStateGroupBase
  (0x14: PBrain +4), PTarget +0x14 `CIntelligentPointer<CThingCreatureBase>`, WaitUntilWorldFrameForNextAttempt +0x1c (long).

## 3. Callees
| retail | landed row | landed name | PDB/real name | draft spelling |
|---|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (NON-const QAE) | same (Ego_r+FableWin) | `CInterfaceMap::LowerBound` local -- WRONG model, use engine/CVectorMap.h / CThing::GetTC |
| vslot 29 +0x74 on TCI_TARGETING(8) comp | 00/4d/CTCTargetingAI_vfunc29_004ddae5.cpp (CTCTargetingAI impl) | vfunc29 | `virtual CThing* GetPTargetedThing() const` (CTCTargetingBase, Ego_r ?GetPTargetedThing@CTCTargetingBase@@UBEPAVCThing@@XZ; engine/CTCTargetingBase.h:34/83; TrollBase_IsStillRunnable 0x0094D930 uses this name) | `CTCAttackTarget::GetTarget()` -- WRONG (class + name + missing const) |
| 0x008645E0 | 00/86/CMainGameComponent_BeginInputLoading_008645e0.cpp (WRONG landed name, a stub) | -- | `NCreatureAI::IsAwareOfThing` `?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z` = `bool __fastcall IsAwareOfThing(const CThingAICreature*, const CThing*, bool)` (name fix note in 00/93/CAIStateGroupMinionReactToEnemy_IsRunnable_00937f70.cpp:12) | `CanCreatureSeeCreature` -- WRONG (old name) |
| vslot 70 +0x118 on CTCPhysicsBase | -- | -- | `virtual C3DVector GetFacingVector() const` (engine/CTCPhysicsBase.h ~l.121, no-arg overload declared first -> slot 70) | `CThingPlacement::GetFacing` -- WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (`const C3DVector& GetPos() const`, engine/CThing.h:411) | GetPos | `GetPosition` -- respell GetPos |
| vslot 81 +0x144 on enemy | 00/6a/CThingPlayerCreature_GetMaxRunningSpeed_006a9890.cpp (an override) | GetMaxRunningSpeed | `virtual float GetMaxRunningSpeed(const C3DVector&) const` (CThingCreatureBase.h:49; ?GetMaxRunningSpeed@...@@UBEMABVC3DVector@@@Z) | `Slot81` -- WRONG |
| 0x00668B60 | 00/66/CIntelligentPointer_CThingCreatureBase_CIntelligentPointer_00668b60.cpp | CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(T*) (vtable 0x0123E7D8) | ??0?$CIntelligentPointer@VCThingCreatureBase@@@@QAE@PAVCThingCreatureBase@@@Z | `CIntelligentPointerConstThing(CThing*)` -- WRONG class |
| 0x00668B80 | 00/66/CIntelligentPointerCThingCreatureBase_operator_00668b80.cpp | CIntelligentPointer<CThingCreatureBase>::operator= | ??4?$CIntelligentPointer@VCThingCreatureBase@@@@QAEAAV0@ABV0@@Z | draft takes `const CIntelligentPointerConstThing&` -- WRONG |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | CBaseIntelligentPointer::~CBaseIntelligentPointer (virtual) | ??1CBaseIntelligentPointer@@UAE@XZ | ok |

## 4. Members
- this+0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE; the generated JumpAtFleeing header flattens it as _pad_0x04).
- this+0x14 PTarget CIntelligentPointer<CThingCreatureBase> (JumpAtFleeing.h: opaque `unsigned char PTarget[8]`).
- CAIBrain+0x18 `CThingAICreature* Creature` (struct_layouts_egor CAIBrain 176/24; engine/CAIBrain.h). Draft calls it CThing*.
- CThing+0x20 UsingComponents (std::bitset<274>), +0x44 Components (CVectorMap), +0x60 `CTCPhysicsBase* PhysicsTC`, +0x91 `Dead:1`
  (engine/CThing.h l.65-88; IsDead()/IsAlive() accessors l.280/289). Draft names: m_presentBits/m_interfaces/Placement/m_flags91 -- all wrong.
- CTCPhysicsBase +0x0C `C3DVector Position`, +0x18 `C3DVector OldPosition` (struct_layouts_egor CTCPhysicsBase 64/12, 64/24). Draft: LastPosition -- WRONG.
- Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (CThing.h ~l.324, conditional-expression GetTC<T>(id,T**) with map pointer local,
  non-const LowerBound -- exactly retail's bytes here), FABLE_RETAIL_STLP_BITSET_INDEX (TrollBase sibling uses with it),
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE, FABLE_RETAIL_CTCPHYSICSBASE (+FABLE_RETAIL_CTCBASE), FABLE_RETAIL_CAISTATEGROUPBASE(_MEMBERS),
  FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE + _ASSIGN_OUTOFLINE (out-of-line T* ctor and
  operator= -- matches the calls to 0x00668B60/0x00668B80; `PTarget = enemy;` then emits ctor/op=/~CBaseIntelligentPointer).
  C3DVector.h: default GetSquaredMagnitude2D (`X*X+Y*Y`, loads Y first -- matches); Dot variants FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
  / default one-line; operator+/- XY_GROUP variants exist (FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP / _SUBTRACT_XY_GROUP).
- Classes the draft models LOCALLY that have shared headers: C3DVector, CThing, CVectorMap (as CInterfaceMap), CTCPhysicsBase (as
  CThingPlacement), CTCTargetingBase (as CTCAttackTarget), CAIBrain, CBaseIntelligentPointer/CIntelligentPointer, CAIStateGroupBase
  (via _pad_0x04 cast), CThingCreatureBase (for slot 81).

## 5. Callee visibility
- CThing::GetPos 0x004C73D0 is an inline accessor emitted out of line (no Ego_r out-of-line name) -> declare only (CThing.h:411), call stays.
- CIntelligentPointer<CThingCreatureBase> ctor/op= are template inline members emitted as COMDATs at 0x00668B60/80 -> retail
  called them out of line: use the _CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE declarations (NOT inline bodies). Contrast
  MinionReactToEnemy 0x00937F70 which inlined them.
- LowerBound 0x0040F020 called out of line (declared in CVectorMap.h).
- Same TU neighbours (interleaved Balverine file): Cornered::GetLandPos 0x0094F090, JumpAtFleeing::InitStateGroup 0x0094F170,
  THIS 0x0094F480, Cornered::Update 0x0094F640, JumpAtFleeing::Update 0x0094F730. None are callees.

## 6. Sibling rows
- 00/93/CAIStateGroupMinionReactToEnemy_IsRunnable_00937f70.cpp -- same front half (TC 8, slot 29, dead bit, IsAwareOfThing, PEnemy=enemy); local model, NCreatureAI::IsAwareOfThing decl.
- 00/94/CAIStateGroupTrollBase_IsStillRunnable_0094d930.cpp -- `#define FABLE_RETAIL_STLP_BITSET_INDEX`, `FABLE_RETAIL_CTHING_GETTC_EXPANDED`, engine/CThing.h, FABLE_RETAIL_CINTELLIGENTPOINTER; names slot 29 GetPTargetedThing.
- 00/94/CAIStateGroupBalverineJumpAtFleeingEnemy_Update_0094f730.cpp / CAIStateGroupBalverineCornered_Update_0094f640.cpp -- same TU; Brain()/Target() accessors over the generated header, FABLE_RETAIL_CINTELLIGENTPOINTER.
- 00/94/CAIStateGroupJackDragon_IsTargetOnATower_0094fbb0.cpp -- engine/CThing.h + GetPos() + C3DVector.h.
- 00/94/CAIStateGroupBalverineCornered_GetLandPos_0094f090.cpp -- includes CTCPhysicsBase.h, C3DVector.h, CAIBrain.h, GetPos.
- None of these carry special pragmas; no `// Flags:` line found in them (default /O2 /Oy).

## 7. Old draft model errors (vs disassembly)
- Callee names/classes wrong (table above): CanCreatureSeeCreature->NCreatureAI::IsAwareOfThing(const CThingAICreature*,const CThing*,bool);
  GetTarget->GetPTargetedThing() const; GetFacing->GetFacingVector() const; Slot81->GetMaxRunningSpeed(const C3DVector&) const;
  GetPosition->GetPos; CIntelligentPointerConstThing->CIntelligentPointer<CThingCreatureBase>; LastPosition->OldPosition.
- GetInterface uses the `if` re-assignment form; retail is the conditional-expression form (CThing.h GETTC_EXPANDED notes this
  form changes register use: component loaded straight into ecx). Use CThing::GetTC<CTCTargetingBase>(TCI_TARGETING,&t).
- Residual (NOTES.md l.50, STATUS.tsv): DIFFER(449v437), retail this=ebp/creature=ebx vs ours swapped, duplicated false epilogue.
  Retail loads `PBrain->Creature` into ebx once at 0x94f54b and uses it for both the GetFacingVector and GetPos of `ahead`, i.e. a
  creature local scoped to (or first used in) the toEnemy/ahead section; all earlier uses re-read [ebp+4]->+0x18. Try a
  `CThingAICreature* creature = PBrain->Creature;` declared right before the `ahead`/toEnemy statements (not at function top),
  typed CThingAICreature (real Creature type), and a const enemy-PhysicsTC local.
- enemy type: GetPTargetedThing returns CThing*; PTarget wants CThingCreatureBase* -> a cast at `PTarget = (CThingCreatureBase*)enemy`
  (or enemy declared CThingCreatureBase*; GetMaxRunningSpeed is a CThingCreatureBase virtual, so enemy is probably CThingCreatureBase*).
- Draft's unused `enemyFacing` is correct (retail makes that call and discards the result).

## 0x005F6793

# DOSSIER 0x005F6793 CTCInventoryAbilities::CycleAssignedExpressions (442 bytes = 0x1BA; old draft said 440)

## 1. Retail shape (xdis 005f6793 0x1ca)
- __thiscall, `ret 4` (one bool). Frame pointer `push ebp; mov ebp,esp; sub esp,0x3c` ... `leave` -> `#pragma optimize("y",off)` + `optimize("s",on)` (or `// Flags: /O1 /Oy-` like twin 0x005F64DD). No SEH.
- Regs: this=edi, esi=0 then &CurrentAssignedExpressionLevel (lea esi,[edi+0x1b8]); ebx pushed LATE (0x5F67C7, after the empty test).
- Frame: vector<long> at ebp-0x1C (zeroed inline = STLport ctor), CSoundInit at ebp-0x3C (0x20), size at -0x10, iterator spill -0xC,
  original_level / max-min temp -8, counter / std::min temp -4. The CCharString temporary reuses the `forward` arg slot [ebp+8].
- Empty path: `cmp begin,end; jne; cmp eax,0; je exit; push eax; jmp <shared free call>` = inlined vector dtor, tail-merged into the final free.
- Flow (matches the Ego_r/FableWin-derived twin CycleAssignedAbilities 0x005F64DD, rebuild/src/compiled/00/5f/CTCInventoryAbilities_CycleAssignedAbilities_005f64dd.cpp):
  1. GetPrioritisedExpressionDefs(expressions); **if (expressions.empty()) return;** -- BEFORE the reset loop (the abilities twin resets first, then tests empty).
  2. for (button=0; button<GetNumAssignableSpecialMoveButtons(); ++button) SetExpressionToButton(0, button);  (push ebx=button, push esi=0 -> args (0, button))
  3. original_level = std::max(CurrentAssignedExpressionLevel, -GetNum...())  (by-ref temp at -8, `jl` picks temp).
  4. size hoisted once: (end-begin) sar 2 -> [ebp-0x10]; signed compare `jl` => `(long)expressions.size()` (twin spells it in the loop and VC hoists it).
  5. do { GetNum called once then `cmp forward; add/sub [esi]` (source: if(forward) +=GetNum(); else -=GetNum(); tail-merged);
     if (level >= (long)size) level = std::min(original_level, 0L);  (temp -4 =0, `jg`)
     else if (level < 0) level = size - GetNum();   -- NO clamp to 0 and NO `level += level % n` (both present in the abilities twin, absent here)
     it = begin; if (level > 0) it += level;
     c1=0; if (c1 < GetNum()) { p=it; do { if (p != end && *p > 0) -> valid; ++c1; ++p; } while (c1 < GetNum()); }
  } while (!valid && level != original_level);   (exit with level==original goes straight to the dtor: valid flag jump-threaded)
  6. valid: for (button=0; it != end; ++it) { SetExpressionToButton(*it > 0 ? *it : 0, button); if (++button == GetNum()) break; }
     (`mov eax,[ebx]; test; push esi; jle -> push 0 else push eax`).
  7. if (PSoundManager) { CSoundInit init; init.GainFaderBits = 0x20; CASoundBank& bank = GetParentThing().GetSampleBank();
     long sample = bank.GetSoundIndexFromSymbol("SND_MENU_04"); PSoundManager->Get2DEmitter().PlaySample(bank, sample, true, init, false); }
     (string 0x01256F54, CCharString(ptr,-1) via default arg; vtable +0x24 = slot 9, then +0x0C = slot 3, pushes 0,&init,1,sample,bank).
  8. ~vector: `if (_M_start) free(_M_start)` (0x00BFEA14).

## 2. Identity (proven, 3 sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x005856CD) and FableWin.pdb (ghidra_out/fablewin_pdb_names.tsv 0x01EBE7CD; FableWin.exe.signatures.tsv 1004 bytes):
  `?CycleAssignedExpressions@CTCInventoryAbilities@@QAEX_N@Z` -> `void CTCInventoryAbilities::CycleAssignedExpressions(bool)` public, non-const.
- Ego_r ordering CycleAssignedAbilities 0x585434 / SetExpressionToButton 0x5855C3 / CycleAssignedExpressions 0x5856CD mirrors retail 0x5F64DD / 0x5F66D9 / 0x5F6793 (same TU).
- Manifest label is CORRECT. Not a vtable slot. Header decl exists: engine/CTCInventoryAbilities.h FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN (line 77).
- Caller: 0x005F6BCD in OnInitialActivate 0x005F6B47 (rebuild/src/compiled/00/5f/CTCInventoryAbilities_OnInitialActivate_005f6b47.cpp) which
  declares it locally as `CycleAssignedExpressions(long direction)` -> NAME_FIXES candidate (PDB `_N` = bool; also CycleAssignedAbilities(long) there).

## 3. Callees (landed file / landed name / PDB name; draft spelling)
| addr | landed row | PDB | draft spelling (WRONG unless noted) |
|---|---|---|---|
| 0x005EED8C | 00/5e/CTCInventoryAbilities_GetPrioritisedExpressionDefs_005eed8c.cpp | ?GetPrioritisedExpressionDefs@CTCInventoryAbilities@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z (const, vector<long>&) | GetPrioritisedExpressionDefs(CLongVector*) non-const -- wrong type/const |
| 0x005EDCF0 | 00/5e/CTCInventoryAbilities_GetNumAssignableSpecialMoveButtons_005edcf0.cpp | ?GetNumAssignableSpecialMoveButtons@CTCInventoryAbilities@@QBEJXZ (long, const) | GetNumberOfAssignableSlots -- wrong |
| 0x005F66D9 | 00/5f/CTCInventoryAbilities_SetExpressionToButton_005f66d9.cpp | ?SetExpressionToButton@CTCInventoryAbilities@@QAEXJJ@Z (long expression, long button) | SetAssignedExpression -- wrong |
| 0x0041C930 | 00/41/CSoundInit_CSoundInit_0041c930.cpp | ??0CSoundInit@@QAE@XZ (inline in Ego_r; retail COMDAT, called out of line here) | ok |
| 0x004C7A80 | 00/4c/CAnimateThing_GetMeshBank_004c7a80.cpp (ICF-shared body; functions.tsv names it CThing::GetSampleBank) | ?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ | ok (CThing::GetSampleBank const) |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) (fable_string.h:290) | ok |
| 0x00A01920 | 00/a0/CASoundBank_GetSoundIndexFromSymbol_00a01920.cpp | ?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z (const) | draft non-const |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | ok |
| [0x013B8394] | global | ?PSoundManager@@3PAVCASoundManager@@A (engine/CASoundManager.h:249) | g_pGameState / CGameState -- wrong |
| vslot 9 (+0x24) | CASoundManager::Get2DEmitter() (descriptive, CASoundManager.h:141) | - | GetSoundPlayer -- wrong |
| vslot 3 (+0x0C) | CASoundEmitter::PlaySample(CASoundBank&, long, bool, const CSoundInit&, bool) (engine/CASoundEmitter.h) | - | Play(bank,long,bool,CSoundInit*,long) -- wrong |

## 4. Members
- +0x04 CTCBase::ParentThing (CThing*) -> use inline `GetParentThing()` (engine/CTCBase.h:143, FABLE_RETAIL_CTCBASE).
- +0x1B8 `long CurrentAssignedExpressionLevel` (Ego_r member; engine/CTCInventoryAbilities.h retail branch line 173, private -- fine inside a member; static_assert 0x1b8 in default branch line 330). Draft called it FirstAssignedExpression -- WRONG.
- CSoundInit +0x14 `GainFaderBits` (engine/CSoundInit.h:20). Draft `Flags` -- wrong. Keep the ctor out of line (do NOT define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR; retail calls 0x0041C930).
- Locally modelled classes that have shared headers: CTCInventoryAbilities, CCharString (fable_string.h), CLongVector (= std::vector<long>, stlport/fable_containers.h), CSoundInit, CASoundBank, CThing, CSoundPlayer2D (= CASoundEmitter), CGameState (= CASoundManager).

## 5. Visibility / opt-in macros
- Copy the twin 0x005F64DD macro set exactly (it matched the same TU): FABLE_RETAIL_CCHARSTRING, _CTCBASE, _DEF_CLASS_BASE, _CTCINVENTORYBASE,
  _CTCINVENTORYABILITIES, _CTCINVENTORYABILITIES_ASSIGN, _BUTTONS, _PRIORITISED, _MAGIC, FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX, FABLE_RETAIL_STLP_VECTOR_C,
  _CTHING, _CTHING_MOREACCESS, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTHINGCREATUREBASE_CHILD, _CINTELLIGENTPOINTER, _CCLASS, _BASE_OBJECT;
  includes engine/CTCInventoryAbilities.h, CThingCreatureBase.h (or CThing.h), CASoundBank.h, CSoundInit.h, CASoundManager.h.
- All CTCInventoryAbilities callees are out-of-line members (PRIORITISED/BUTTONS decls). vector<long> ctor/empty/size/begin/end/dtor are inline STLport (inlined in retail); no out-of-line STLport helper is called.
- Same TU (retail address order 0x5F6423 SetAbilityToButton, 0x5F64DD CycleAssignedAbilities, 0x5F66D9 SetExpressionToButton, 0x5F6793 this, 0x5F694D ResetAssignedAbilities). The twin defined GetPrioritisedAbilityDefs body in its row; SetExpressionToButton was defined earlier in the TU but retail calls it (not inlined; size TU).

## 6. Siblings
- 0x005F64DD CycleAssignedAbilities: `// Flags: /O1 /Oy-`, no pragmas, shared headers, std::max/std::min, bool valid_set do/while -- THE template.
- 0x005F66D9 SetExpressionToButton, 0x005F6423 SetAbilityToButton, 0x005F694D ResetAssignedAbilities, 0x005F6A6F ResetAssignedAbilitiesPC,
  0x005F6B47 OnInitialActivate: `#pragma optimize("y",off)` + `optimize("s",on)` with local models (older style).

## 7. Old-draft model errors
- Wrong names everywhere (table 3/4); local models instead of headers; pointer param instead of const vector<long>&; non-const GetNum.
- `goto found` / `for(;;)` with `return` on exhaust -> instead use the twin's `bool valid_set` do/while + `if (valid_set)` (VC jump-threads it).
- Hand-rolled Max/Min templates -> std::max / std::min (by-ref temps match retail).
- `long count = defs.size()` hoisted by hand; twin writes `(long)abilities.size()` inline in the compare/assign and VC hoists it.
- Register pick this=edi/zero=esi: twin row has this=ebx because its reset loop runs first; here the early empty-return with the late ebx push is retail -- write `if (expressions.empty()) return;` first, then the reset loop.

## 0x00847DF0

# DOSSIER 0x00847DF0 -- CCreatureAction_UseLamp::Initialise (442 bytes)

## 1. Retail shape (xdis 00847df0 0x1ca)
- __thiscall, `ret` (no args), returns void. No frame pointer (`sub esp,14h ; push ebx/ebp/esi/edi`), ebp used as a GPR
  (holds the `new` result). No SEH prologue (no /EHsc frame), frame 0x14 bytes of locals.
- Locals: [esp+10] animGroup CCharString; [esp+14] shared slot: key 0xd for LowerBound, then the returned-name temp of
  GetActionUseAnimationGroupName, then the "LIGHT_LAMP" CCharString, then the CGameCameraManager* out param;
  [esp+18..20] CAnimFilterOfGroup filter {vptr 0x01267568, Group +1c, GroupCRC +20}; at end vptr reset to CAnimFilterBase 0x0122FFCC
  (inline ~CAnimFilterOfGroup -> ~CAnimFilterBase), then animGroup dtor.
- ebx = this; esi = &PObjectToUse (+0xa8) during the guard chain, later esi = &PCreature (+8) kept across.
- No size-opt idioms visible; default /O2-style.

## 2. Identity
- Vtable: rtti-vtables.tsv `CCreatureAction_UseLamp 0127486c slot 12 = 00847df0` (only xref: .rdata 0127489c).
- Ego_r.pdb: egor_signatures.tsv `0051f2b0 ?Initialise@CCreatureAction_UseLamp@@UAEXXZ` (class has ctor 0051dcc0,
  IsToHaveEmptyHands, PerformAction, Clone). Landed sibling 0x00847FB0 = CCreatureAction_UseLamp::PerformAction (same TU).
  Manifest label is CORRECT: `?Initialise@CCreatureAction_UseLamp@@UAEXXZ` (public virtual void Initialise()).
- NOTE: egor_pdb_names.tsv row `00847df0 ?PeekWorld@CAIStateGroupBase...` is an Ego_r-address collision, irrelevant.

## 3. Callees (draft spelling vs PDB / landed)
| addr | landed file | real name (PDB) | draft spelling |
|---|---|---|---|
| 0099E4B0 | 00/99/CCharString_CCharString_0099e4b0.cpp | CCharString::CCharString() | ok |
| 00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const (CBaseObject*) | ok (draft returns CThing*) |
| 004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound(const ETCInterfaceType&) **const** | draft "CTCMap" local -- the CONST overload => lookup on a const map (PeekTC), NOT 0x0040F020 used by sibling PerformAction |
| 007EABA0 | 00/7e/CTCActionUseBase_GetActionUseAnimationGroupName_007eaba0.cpp | ?GetActionUseAnimationGroupName@CTCActionUseBase@@QBE?AVCCharString@@XZ (**const**) | draft non-const -- wrong |
| 0099EFB0 | 00/99/CCharString_operator_0099efb0.cpp | CCharString::operator=(const CCharString&) | ok |
| 0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| 004014A0 | 00/40/CCRC_Calc_004014a0.cpp | CCRC::Calc __fastcall(ulong,const void*,ulong) | ok |
| 0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | ok |
| 00662EF0 | 00/66/CThingCreatureBase_GetRandomAnimFromNameWithFilter_00662ef0.cpp | ?GetRandomAnimFromNameWithFilter@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@@Z: `const CAnimationEntry& (const CCharString&, const CAnimFilterBase&) const` | draft: returns pointer, takes CAnimFilterBase* -- wrong types (ABI same) |
| 00693030 | 00/69/CCreatureActionBase_SetAnimation_00693030.cpp | CCreatureActionBase::SetAnimation(const CAnimationEntry&, float, float) | draft pointer -- wrong type |
| vslot 77 (+0x134) on PCreature item | -- | **CThingCreatureBase::IsPlayerCharacter() const** (pure; CThingPlayerCreature 0x0052ABE0, CThingAICreature 0x00835940; CThingCreatureBase.h:45, CThingAICreature.h:246) | draft "IsPlayerControlled" on CThing -- WRONG NAME |
| 00692940 | 00/69/CCreatureActionBase_GetWorld_00692940.cpp | ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ, returns CWorld& | draft "GetGameContext"/CActionGameContext -- WRONG |
| [world+0x34] | -- | CWorld::PCurrentGameCamera (CAIGameCameraBase*) (struct_layouts_egor CWorld +52) | draft "PCameraOwner" -- wrong |
| vslot 9 (+0x24) | 00/69/CAIGameCameraBase_GetCameraManager_006970c0.cpp | CAIGameCameraBase::GetCameraManager(CGameCameraManager**) non-const (slot 9; const overload is slot 10) | draft "CCameraOwner::GetCameraManager" -- class wrong |
| 00BFEA1A | 00/bf/global_operatornew_00bfea1a.cpp | operator new(0xa0) | ok |
| 006925B0 | 00/69/CCreatureActionBase_GetTotalTimeWorldFrames_006925b0.cpp | ?GetTotalTimeWorldFrames@CCreatureActionBase@@QBEJXZ (`mov eax,[ecx+10h]`, TotalTime) | draft "GetCameraEnvironment" -- WRONG |
| 00853120 | 00/85/NCameraModesCScriptModuleUseLamp_CScriptModuleUseLamp_00853120.cpp | **NCameraModes::CScriptModuleUseLamp**::CScriptModuleUseLamp(const CThing& creature, const CThing& lamp, long duration) (??0CScriptModuleUseLamp@NCameraModes@@QAE@ABVCThing@@0J@Z, Ego_r 00828250 + FableWin straggler) | draft "CScriptModuleOpenChest" -- WRONG CLASS; PDB params are references (landed row declares pointers) |
| 005ED270 | 00/5e/CCountedPointer005ecf50_..._005ed270.cpp | CCountedPointer<T>::CCountedPointer(T*) out-of-line COMDAT (landed under made-up name CCountedPointer_005ecf50) | called, not inlined |
| 00699E60 | NOT LANDED | ?SetScriptedCameraMode@CGameCameraManager@@QAEXV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z | ok (namespace NCameraModes missing in draft) |
| 00694D10 | 00/69/CCreatureActionBase_Initialise_00694d10.cpp | CCreatureActionBase::Initialise() (direct qualified call) | ok |

## 4. Members
- this+0x08 PCreature: CIntelligentPointer<CThingCreatureBase> (Ego_r +8). Header engine/CCreatureActionBase.h,
  opt-ins FABLE_RETAIL_CCREATUREACTIONBASE + _PCREATURE (protected PCreature at +8, opaque to 0xa8) or _FULL_MEMBERS;
  _ACCESSORS declares GetWorld (0x00692940). TotalTime +0x10 for GetTotalTimeWorldFrames -- NO header opt-in declares
  GetTotalTimeWorldFrames (grep rebuild/include: none); declare it locally / in a derived view.
- this+0xa8 PObjectToUse: CIntelligentPointer<CThing> (Ego_r +0xb4; retail base is 0xa8). engine/CCreatureAction_UseLamp.h is
  a flat generated header (pads, no opt-ins); sibling 0x00847FB0 models it locally on a local 0xa8 base.
- CThing +0x20 UsingComponents bitset (test bit 13 = TCI_ON_ACTION_USE=13, ETCInterfaceType.h:18), +0x44 Components
  (CVectorMap), +0x91 flag bit0 (Dead). engine/CThing.h: FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED gives
  `PeekTC(id, T**) const` that calls the const LowerBound 0x004365B0 with the pointer-local map and ternary end pick -- matches retail.
  IsDead() exists (CThing.h:280).
- CWorld +0x34 PCurrentGameCamera (engine/CWorld.h).
- CAnimFilterOfGroup (engine/CAnimFilterOfGroup.h: Group +4, GroupCRC +8, size 12; virtual operator(), Clone, dtor).
  Header has NO inline ctor; retail INLINES the (const CCharString&) ctor here although an out-of-line copy exists at 0x0073E8F0.
  Retail inline form = single shared null test (if/else into data/length locals), like landed 0x007409C0 lines 39-53 -- not
  the two-accessor form of 0x0073E8F0. The CRC reads the SOURCE string ([esp+10], animGroup), not Group.
- CAIGameCameraBase.h: FABLE_RETAIL_CAIGAMECAMERABASE (full 25-slot polymorphic class, GetCameraManager slot 9).
- CGameCameraManager.h: FABLE_RETAIL_CGAMECAMERAMANAGER declares SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>).
- CCountedPointer.h: FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR (decl-only T* ctor -> call 0x005ED270, used by landed 0x008C1A10/0x008C1900/0x0096DBC0).
- CThingCreatureBase.h: FABLE_RETAIL_CTHINGCREATUREBASE + _ANIMFILTER (GetRandomAnimFromNameWithFilter const& sig, line 286-290).
- CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER (+ _DEREFERENCE for operator*/->).
- Classes the draft models LOCALLY that have shared headers: CCharString (fable_string.h + FABLE_RETAIL_CCHARSTRING),
  CThing, CThingCreatureBase, CTCActionUseBase, CBaseIntelligentPointer/CIntelligentPointer, CCountedPointer,
  CCreatureActionBase, CCreatureAction_UseLamp, CAnimFilterBase/CAnimFilterOfGroup (draft calls it CAnimFilterGroupName -- wrong
  name), CGameCameraManager, CAIGameCameraBase (draft: CCameraOwner), CVectorMap (draft: CTCMap), CScriptModuleUseLamp (draft: OpenChest).

## 5. Visibility
- Out of line in retail (call): LowerBound const 0x004365B0, GetPItem, all CCharString members, CCRC::Calc,
  GetTotalTimeWorldFrames 0x006925B0 (4-byte body called -> not visible), GetWorld 0x00692940, CCountedPointer(T*) 0x005ED270
  (called COMDAT -> CALLED_CTOR opt-in), CScriptModuleUseLamp ctor 0x00853120, SetScriptedCameraMode.
- Inline in retail: CAnimFilterOfGroup(const CCharString&) ctor and its dtor (vptr reset to 0x0122FFCC, Group dtor), HasTC bit
  test, PeekTC map logic, CIntelligentPointer accessors.
- Same TU: PerformAction 0x00847FB0 follows (defined later); the neighbourhood 0x00847470..0x00848FD0 are other CCreatureAction_*
  rows (likely one CreatureActions*.cpp TU).

## 6. Siblings
- 00/84/CCreatureActionUseLamp_PerformAction_00847fb0.cpp: local CThing model (HasTC + ternary GetTC), local 0xa8 base,
  CIntelligentPointer<CThing> PObjectToUse +0xa8; no pragmas.
- 00/84/CCreatureActionDrink_Initialise_00847ac0.cpp: FABLE_RETAIL_CCHARSTRING etc., CCreatureActionBase via #define-rename trick.
- 00/84/CCreatureActionGetInBed_PerformAction_00848db0.cpp: FABLE_RETAIL_CTHING + _CCHARSTRING + _CTHING_GETTC_EXPANDED; counted
  pointer built in arg area.
- 00/74/NCreatureModeCModeBerserk_CModeBerserk_007409c0.cpp: inline CAnimFilterOfGroup ctor form with the single null test.
- 00/8c/CCombatActionGuildmasterHealFollowersInto_Initialise_008c1900.cpp / 008c1a10: CCOUNTEDPOINTER_CALLED_CTOR usage.

## 7. Old draft model errors (vs disassembly)
- Wrong names: IsPlayerControlled->IsPlayerCharacter, GetGameContext->GetWorld (CWorld&), PCameraOwner->PCurrentGameCamera,
  CCameraOwner->CAIGameCameraBase, GetCameraEnvironment->GetTotalTimeWorldFrames, CScriptModuleOpenChest->NCameraModes::CScriptModuleUseLamp,
  CAnimFilterGroupName->CAnimFilterOfGroup, GetActionUseAnimationGroupName must be const, GetRandomAnimFromNameWithFilter returns const&.
- Residue (436 v 442): retail evaluates the module ctor args as lamp (edi = PObjectToUse via fresh `lea ecx,[ebx+0a8h]`),
  creature (esi = PCreature, esi=&PCreature already), duration (eax = GetTotalTimeWorldFrames) and pushes eax,edi,esi.
  Now that the ctor's real params are `const CThing&` refs and arg3 is GetTotalTimeWorldFrames(), retry with
  `new NCameraModes::CScriptModuleUseLamp(*PCreature, *PObjectToUse, GetTotalTimeWorldFrames())` (operator* deref form,
  FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE) -- reference conversions may change VC7.1's eval order. Also the camera manager
  object: retail loads `mov ecx,[eax+34h]` (world ref -> PCurrentGameCamera) straight into ecx, then vptr via eax.
- Guard chain: retail calls GetPItem three times (null test, flag test, then the thing), HasTC test on [eax+20h] bit 13 BEFORE
  computing &Components -> PeekTC(13, &use) const form (key 0xd stored after test, const LowerBound).
- Draft's filter ctor uses two accessors; retail inline form is the single null-test if/else (0x007409C0 model).

## 0x0097D6F0

# DOSSIER 0x0097D6F0 (444 bytes) CAIStateGroupBase::AIAction_LookAtThing

## 1. Retail shape (xdis 0097d6f0 0x1cc)
- __thiscall, `ret 14h` (5 dwords: repeat, CIntelligentPointer<const CThing> by value = 8 bytes, long, float). No frame pointer
  (`sub esp,60h ; push edi`), no SEH frame, default /O2-style. Callee-dtor of the by-value target on every exit (`lea ecx,[esp+..] ; call 0xA01C10`).
- Locals 0x60 (offsets after the 4 pushes edi/ebx/ebp/esi): pos C3DVector +0x10, GetFocalPos hidden-result temp +0x1c, CBrainOutput +0x28 (0x48 bytes,
  LookAtThing at +0x68 = output+0x40, PAction count block at +0x4c = output+0x24).
- Args: repeat [esp+74h], target [esp+78h], lookFlags -> ebx (loaded once, before loop), tolerance -> ebp (loaded once).
- Flow: IsStopping() first (`test al,al`; bool) -> dtor target, return 1. Loop head 0x0097D722:
  IS_THING_ALIVE(target) = GetPItem()!=0 && !(GetPItem()->[+0x91]&1) else result 3.
  pos: `if (thing->PTCGraphicAppearance /*+0x64*/) pos = PTCGraphicAppearance->GetFocalPos(1.0f); else pos = thing->GetPos();` --
  each branch has its OWN 12-byte copy into pos (branch A edx/ecx/edx from temp; branch B ecx/edx/eax from the GetPos ref) and a jmp.
  Then `esi = PBrain->Creature` loaded BEFORE the GetPItem call, `cmp eax,esi` (thing == creature -> result 2);
  then PBrain->Creature re-read into ecx, `test bl,4 ; setg al` / `test bl,2 ; setg dl`, push al(4), dl(2), ebp, &pos ->
  call 0x006650E0 (pos, tolerance, (flags&2)>0, (flags&4)>0); true -> result 2.
  Else: inline CBrainOutput ctor (call 0x00835620 is the OUT-OF-LINE ctor here, not inlined!), SetAsLookAtThing(thing, lookFlags) 0x0093E760 (push ebx then GetPItem),
  PBrain->SetOutput(output) 0x0088B8F0, `mov esi,4` scheduled before the inline ~CBrainOutput: LookAtThing dtor call 0xA01C10 then the inline
  CCountedPointer release (dec [cnt]; ==0 -> call [cnt+4] with ecx=[cnt+8]; operator delete 0xBFE9BC) and `PAction = {0,0}` stores.
  After: PBrain vtable slot 2 (+8) call; IsStopping() -> dtor, return 1; `repeat==1 && result==4` -> jump to loop head; else dtor, return result.
- Result codes used: 1 stopping, 2 done, 3 failed (dead/null target), 4 running.

## 2. Name / signature (manifest label is CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r 0x008484A0) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x01796251, 0x02891910):
  `?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z`
  = protected, non-const, `EAIActionReturn AIAction_LookAtThing(EAIActionRepeat, CIntelligentPointer<const CThing>, long, float)`.
- Shared header already declares it: rebuild/include/engine/CAIStateGroupBase.h line ~130 under FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS + _AIACTIONS
  (param names there "frames, speed" are wrong semantically: they are look flags and the view-angle tolerance; names do not affect bytes).
- 75 call sites (xref.py); landed callers declare it locally as `long AIAction_LookAtThing(long, CIntelligentPointer<const CThing>/CIntelligentPointerConstThing, long, float)`.

## 3. Callees (landed file -> landed name -> PDB name)
| addr | landed row | landed name | PDB / verdict |
|---|---|---|---|
| 0x008FD010 | compiled/00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp | `long IsStopping() const` | `?IsStopping@CAIStateGroupBase@@IBE_NXZ` = protected **bool** const (header agrees: bool). Draft: non-const -> wrong; caller tests `al` so use bool const. |
| 0x00A01B50 | compiled/00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (returns CBaseObject*; draft returned CThing*). Use CIntelligentPointer<const CThing>::Get()/operator-> from engine/CIntelligentPointer.h. |
| 0x00A01C10 | .../CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | virtual ~CBaseIntelligentPointer | ok |
| 0x004C1060 | compiled/00/4c/NGraphicAppearanceCTCGraphicAppearance_GetFocalPos_004c1060.cpp | CTCGraphicAppearance::GetFocalPos(float) const | `?GetFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@M@Z`. **Draft spelling GetHelperPointOrCentre is WRONG.** Header NGraphicAppearance_CTCGraphicAppearance.h:134 declares it. |
| 0x004C73D0 | compiled/00/4c/CThing_GetPosition_004c73d0.cpp | `const C3DVector& CThing::GetPos() const` | `?GetPos@CThing@@QBEABVC3DVector@@XZ` (out of line, 7 bytes). **Draft GetPosition returning pointer is WRONG** (CThing.h:411 declares GetPos). |
| 0x006650E0 | compiled/00/66/CThingCreatureBase_CanSeeTarget_006650e0.cpp | CThingCreatureBase::CanSeeTarget(const C3DVector&, float, bool, bool) | NOT a PDB name (only CanSeeTarget in PDB is CAIStateGroup_CreatureUseCoverBase::CanSeeTarget()). Best PDB fit: `?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z` (Ego_r 0x0053F1B0 + FableWin) -- same (const C3DVector&, float, bool, bool) const -> bool, body forwards to CTCLook::IsPosInViewRange then does the view-angle test. Sibling 0x0097D580 calls it IsLookingAtPos (also made up). NAME_FIXES candidate: 0x006650E0 -> CThingCreatureBase::IsPosInViewAngle (const). Draft put it on CThingCreature (wrong class). |
| 0x00835620 | compiled/00/83/CBrainOutput_CBrainOutput_00835620.cpp | CBrainOutput::CBrainOutput() | ok; CALLED out of line here (sibling 0x0097D580 inlines it; this row must not). |
| 0x0093E760 | compiled/00/93/CBrainOutput_SetAsLookAtThing_0093e760.cpp | SetAsLookAtThing(const CThing*, long) | PDB `?SetAsLookAtThing@CBrainOutput@@QAEXABVCThing@@J@Z` = **const CThing&** (same bytes). Draft passes pointer -> respell as reference (`*target`). |
| 0x0088B8F0 | compiled/00/88/CAIBrain_SetOutput_0088b8f0.cpp | CAIBrain::SetOutput | `?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z`. **Draft SetBrainOutput is WRONG** (CAIBrain.h opt-in FABLE_RETAIL_CAIBRAIN_SETOUTPUT). |
| vtbl+8 on PBrain | -- | draft "Yield" (made up) | CAIBrain : CSuspendableProcessMicrothread (CAIBrain.h FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE); slot 2 = `?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ` (CSuspendableProcessMicrothread.h:18, bool, result ignored). Sibling 0x0097D8B0 still uses `Yield`. |
| 0x00BFE9BC | compiled/00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | inline CCountedPointer release |

## 4. Members
- CAIStateGroupBase +0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE; _MEMBERS gives IsStopping bool const; _AIACTIONS gives this decl).
- CAIBrain +0x18 Creature (CThingAICreature*; reference under FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE, GetCreature() under _GETCREATURE). Draft typed it CThingCreature* -- real CThingAICreature (Ego_r layout).
- CThing +0x64 PTCGraphicAppearance (NGraphicAppearance::CTCGraphicAppearance*; Ego_r +0x74 = retail +0x64 after the -0x10 shift; CThing.h:71, inline GetTCGraphicAppearance() const CThing.h:433), +0x91 bit0 Dead (CThing.h:88, IsDead() :280, IsAlive() :289). Draft local `m_flags91`/`PGraphicAppearance` -> use CThing.h.
- CBrainOutput (no shared header; CAIBrain.h only forward-declares, Output[0x48] at brain +0x44): Types +0, LookFlags +4, NavigationInput +8, PAction (CCountedPointer<CCreatureActionBase>) +0x20, MoveToPos +0x28, LookAtPos +0x34, LookAtThing +0x40, size 0x48. Model as in sibling 0x0097D580 but ctor NOT inline (declare `CBrainOutput();`), dtor inline (LookAtThing dtor + PAction release + zero stores). engine/CCountedPointer.h offers FABLE_RETAIL_CCOUNTEDPOINTER (+ _RELEASEDATA, _VISIBLE_CALLED_DTOR...).
- IS_THING_ALIVE: engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (template `if(thing!=0){ if(!thing->IsDead()) return true;} return false;`) -- needs CIntelligentPointer != 0 / operator->; siblings define a local IS_THING_ALIVE(const CBaseIntelligentPointer&) instead.
- Locally modelled in the draft though a shared header exists: C3DVector (C3DVector.h), CThing (CThing.h), CTCGraphicAppearance (NGraphicAppearance_CTCGraphicAppearance.h), CBaseIntelligentPointer/CIntelligentPointer (CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER), CAIBrain (CAIBrain.h), CAIStateGroupBase (CAIStateGroupBase.h), CThingCreatureBase (CThingCreatureBase.h), CCountedPointer (CCountedPointer.h).

## 5. Visibility
- Out-of-line calls retail makes: CBrainOutput ctor 0x00835620 (so its body NOT visible), GetFocalPos, GetPos (7-byte body out of line -> not visible/inline), GetPItem, ~CBaseIntelligentPointer, SetAsLookAtThing, SetOutput, IsStopping, 0x006650E0.
- Inline in retail: CBrainOutput dtor (incl. CIntelligentPointer dtor call + CCountedPointer release), IS_THING_ALIVE, GetTCGraphicAppearance, IsDead.
- Neighbours in the same TU (CAIStateGroupBase AIActions, 0x0097D360..0x0097FBE0) all landed; 0x0097D580 LookAtPos is defined just before.

## 6. Siblings
- compiled/00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp (same loop skeleton, no pragmas/Flags; uses FABLE_RETAIL_CINTELLIGENTPOINTER + engine/C3DVector.h, local CBrainOutput with FORCEINLINE ctor/dtor, `(lookFlags & 2) > 0, (lookFlags & 4) > 0` in the call).
- ..._AIActionPerformActionTargetFace_0097df60.cpp (includes engine/CThing.h, CIntelligentPointer.h; local IS_THING_ALIVE on CBaseIntelligentPointer using IsDead(); SetAsLookAtThing(ptr,0x23)).
- ..._AIActionPerformActionTargetFaceAndWait_0097dcb0.cpp, ..._AIActionPerformAction_0097d8b0.cpp (uses SetOutput PDB spelling; ~CBrainOutput out of line 0x006D60C0 there), ..._AIActionWaitForMessage_0097d360.cpp (shared CAIStateGroupBase.h/CAIBrain.h).
- None carries `// Flags:` or `#pragma optimize` -> default lane flags.

## 7. Old draft model errors
- Residue DIFFER(422v444): VC tail-merged the two pos copies. Retail keeps separate copy+jmp per branch, and branch A copies out of a hidden-result temp (+0x1c) while branch B copies from the GetPos reference. Draft's `GetPosition()` returning a pointer and dereferencing gives the same shape in both arms -> merge. Fix: GetPos returning `const C3DVector&`; try a by-value helper with two returns, or `C3DVector pos = g ? g->GetFocalPos(1.0f) : thing->GetPos();`, or an inlined accessor; the per-branch registers (edx/ecx/edx vs ecx/edx/eax) are consistent with separate copy-ctor calls per arm.
- Compare: retail loads PBrain->Creature into esi BEFORE GetPItem (left operand first: `PBrain->Creature != target.Get()` style or a named creature local), then re-reads PBrain->Creature for the call (draft's note says draft did not reload: make the call go through `PBrain->Creature->`/GetCreature() after the call, not a cached local).
- Wrong names/types: GetHelperPointOrCentre->GetFocalPos, GetPosition->GetPos (ref), SetBrainOutput->SetOutput, Yield->SuspendSuspendableProcess, CThingCreature::CanSeeTarget->CThingCreatureBase::IsPosInViewAngle const (PDB best fit), SetAsLookAtThing(const CThing&), IsStopping bool const, Creature is CThingAICreature*, target type CIntelligentPointer<const CThing>.
- Control flow/loop form (do-while with IsStopping return inside, repeat==1 && result==4) matches retail; enum values 1/2/3/4 right.

## 0x006AD3C0

# DOSSIER 0x006AD3C0 CThingPlayerCreature::ApplyRelativeMovementAcceleration (447 B)

## 1. Retail shape (xdis 006ad3c0 0x1cf)
- `sub esp,68h ; push ebx ; push esi ; push edi ; mov edi,ecx`. NO frame pointer (no push ebp/mov ebp,esp; /Oy on), NO SEH.
  `ret 8` (thiscall, 2 args). Two epilogues (found / not-found arms each end `mov [edi+124h],0 ; pop.. ; ret 8`).
- `push ebp` at 0x6AD417 only around the camera/speed block, `pop ebp` at 0x6AD4B6 (ebp = PeekWorld() result, then PhysicsTC).
- Frame map (offsets from frame base = esp after the 3 pushes, i.e. `[esp+0Ch]` = 0): scale float 0x00 (=1.0f, stored
  before the je); TC key 0x04 (0x31, stored INSIDE the test); direction C3DVector 0x10..0x18 (copied from *acceleration);
  CCamera by-value temp 0x30..0x67 (0x38 bytes, GetCurrentRenderCamera hidden result) -- this is why the frame is 0x68;
  GetFacingVector() temp reuses 0x04; Z of the scaled vector 0x0C; pair<const EGameAction,pair<C3DVector,long>> 0x1C..0x2C;
  insert result 0x04. Dead ARG slots reused: arg1 (acceleration, esp+0x78/0x7c) holds the heading float, then the mode
  vtable, then the speed, then find's hidden iterator; arg2 (action) is read by address for find and by value for insert.
- Float compare: `fst [arg1] ; fcomp DELTA_NOTIONAL_ZERO(0x0129BA3C, 1e-4f) ; fnstsw ; test ah,41h ; jne` = `if (speed > DELTA_NOTIONAL_ZERO)`.
  `call [eax+15Ch]` (this slot 87) then `fdivr [arg1]` = scale = speed / GetMaxAcceleration().
- Scaled vector: `fld scale ; fmul [esi]` x3 = acceleration * scale components (scale on the left, i.e. scale*X).

## 2. Identity (two sources)
- Ego_r.pdb `egor_pdb_names.tsv` 0x0054ACB0 and FableWin `_fablewin_api.tsv` 0x01DEE070/0x0184EF90:
  `?ApplyRelativeMovementAcceleration@CThingPlayerCreature@@QAEXABVC3DVector@@W4EGameAction@@@Z`
  = public non-virtual `void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action)`.
- labels_landing_2026-11-10ai.tsv binds it to 0x006AD3C0; engine_api.tsv thiscall void (C3DVector*, EGameAction).
- Manifest label is RIGHT. Already declared in rebuild/include/engine/CThingPlayerCreature.h under
  `FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT` (needs FABLE_RETAIL_CTHINGPLAYERCREATURE + FABLE_RETAIL_CTHINGCREATUREBASE;
  EGameAction MS forward enum from engine/fable_nscript_forward.h).
- Callers: 0x0062968D (landed CWorld::EAMoveCreature 0x006295C0, rebuild/src/compiled/00/62/CWorld_EAMoveCreature_006295c0.cpp)
  and 0x0062BECB (in 0x0062BD80 CWorld::EAControlledCreatureStrafe, chunk 1012).

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | PDB / header name | old draft | verdict |
|---|---|---|---|---|
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap::LowerBound const (CThing.h PeekTC expanded) | CInterfaceMap::LowerBound via GetInterface | use CThing::PeekTC (const overload => PeekTC, not GetTC) |
| 0x004C7AB0 | 00/4c/CViewBase_GetDisplayEngine_004c7ab0.cpp (propagated label) | Ego_r ?PeekWorld@CThing@@IBEABVCWorld@@XZ (GetWorld is byte-identical twin 0x004C7AA0); CThing.h `const CWorld& PeekWorld() const` | "GetCameraManager" | WRONG in draft |
| 0x0049D9D0 | 00/49/CWorld_PeekMyPlayer_0049d9d0.cpp | ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ (CWorld.h:267) | "CCameraManager::GetMode" | WRONG |
| [world vtbl+0x18] slot 6 | - | ?GetCurrentRenderCamera@CWorld@@UBE?AVCCamera@@XZ (CWorld.h:104), CCamera 0x38 bytes by hidden ptr | "GetDirection() returns C3DVector" | WRONG type (CCamera, not C3DVector) |
| 0x00A0C380 | 00/a0/CCamera_GetAngleXY_00a0c380.cpp | ?GetAngleXY@CCamera@@QBEMXZ (CCamera.h, FABLE_RETAIL_CCAMERA_ANGLES) | C3DVector::GetAngleXY | WRONG class |
| [cam vtbl+0x1c] slot 7 | - | ?GetControlAngleXY@CAIGameCameraBase@@UBEMXZ (CAIGameCameraBase.h:34) on CWorld::PCurrentGameCamera +0x34 (inline GetGameCamera()/PeekGameCamera()) | "Heading->GetHeading()" | WRONG names |
| 0x00A14040 | 00/a1/C3DVector_RotateXY_00a14040.cpp | ?RotateXY@C3DVector@@QAEXM@Z (C3DVector.h:336, unconditional) | RotateAboutZ | WRONG spelling |
| 0x00A14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE) | Normalise | WRONG spelling |
| 0x0073A6E0 | 00/73/CTCCreatureModeManager_GetCurrentMode_0073a6e0.cpp | ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ (CTCCreatureModeManager.h:54) | CTCPlayerMovement::GetController | WRONG |
| [mode vtbl+0x1c] slot 7 | - | CModeBase::GetMovementSpeed(const C3DVector& direction, const C3DVector& facing, EMovementType) const (FABLE_RETAIL_CMODEBASE_VIRTUALS) | GetRelativeSpeed(dir, vel, bool) | WRONG name and 3rd type (EMovementType, pushed 0 = AI_MOVEMENT_NULL, engine/EMovementType.h) |
| [PhysicsTC vtbl+0x118] slot 70 | - | CTCPhysicsBase::GetFacingVector() const, `ret 4` hidden result only (CTCPhysicsBase.h:125) | GetVelocity() | WRONG: it is the facing vector |
| [this vtbl+0x15c] slot 87 | 00/6a/CThingPlayerCreature_GetMaxAcceleration_006a9c10.cpp | ?GetMaxAcceleration@CThingPlayerCreature@@UBEMXZ (Ego_r 0x00546BF0; header VSLOTS2 slot 87) | GetMaxSpeed | WRONG |
| 0x006AE3E0 | 00/6a/stdRbtree...find_006ae3e0.cpp | STLport _Rb_tree<EGameAction,...>::find(const key&) non-const, `ret 8` (hidden iterator + key ref) | CAccelMap::find (local stub) | use std map |
| 0x006AE9F0 | 00/6a/unattributed_InsertUniqueCRetailTree006ae9f0_006ae9f0.cpp | STLport _Rb_tree::insert_unique(const value_type&) (map::insert), returns pair<iterator,bool> by hidden ptr | CAccelMap::insert | use std map |
Global: DELTA_NOTIONAL_ZERO 0x0129BA3C (`extern const float`, engine/C3DVector.h under FABLE_RETAIL_C3DVECTOR); draft used literal 0.0001f.

## 4. Members
- this (retail CThing via CThingCreatureBase): UsingComponents bitset<274> +0x20 (test [edi+24h],20000h = bit 49),
  Components CVectorMap +0x44, PhysicsTC +0x60 -> engine/CThing.h (FABLE_RETAIL_CTHING; expanded PeekTC body = FABLE_RETAIL_CTHING_GETTC_EXPANDED).
- IdleCounter long +0x124 -> engine/CThingCreatureBase.h:435 (asserted 0x124).
- RelativeMovementAccelerationComponents +0x1A8 -> engine/CThingPlayerCreature.h, BUT only as `unsigned char[0xc]`
  (Ego_r type map<EGameAction,pair<C3DVector,long>>, Ego_r offset 464). Author needs a typed STLport map there
  (stlport/fable_map.h; header change or reinterpret through a reference). Node value at +0x10: key, C3DVector +0x14, long +0x20.
- CPlayer +0x220 bool: CPlayer.h:558 retail = `UsingFreeCamera` (assert line 745; SpellMode is retail +0x218, CPlayer.h:546/163).
  Sibling 0x006A9690 calls +0x220 "SpellMode (Ego_r layout)" -- the shared header's retail name is UsingFreeCamera.
- CWorld::PCurrentGameCamera +0x34 (CWorld.h:308, GetGameCamera/PeekGameCamera inline at CWorld.h:212-213).
- Classes the draft models LOCALLY that have shared headers: C3DVector, CThingPlayerCreature (+CThing bits), CTCPhysicsBase,
  CVectorMap (CInterfaceMap), CTCCreatureModeManager/CModeBase (as CTCPlayerMovement/CMovementController), CWorld (as
  CCameraManager), CPlayer (as CCameraMode), CAIGameCameraBase (as CCameraHeading), CCamera, std::pair/map.

## 5. Visibility
- All callees are out of line in retail: LowerBound const (CThing.h notes it is out of line), PeekWorld, PeekMyPlayer,
  GetAngleXY, RotateXY, AccurateNormalise, GetCurrentMode, map find/insert_unique (STLport, called out of line).
- Inline expanded in retail: PeekTC body (UsingComponents test + LowerBound + end-or-greater ternary), CWorld::PeekGameCamera
  (`mov ecx,[ebp+34h]`), C3DVector copy and scale, map::end() (`mov ecx,[ebx]`), iterator->second.first += / ++second.second.
- map find/insert: fable_map.h has FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (find), FABLE_RETAIL_STLP_TREE_MFIND_DECL; check how
  0x006AE3E0's caller-side TUs keep find/insert_unique out of line (no ctor/dtor needed here).
- Neighbours: 0x006AD260 UpdateWalkingControlForces, 0x006AD580 OnCollision -- same CThingPlayerCreature TU, defined around it.

## 6. Siblings
- 00/6a/CThingPlayerCreature_ResolveRelativeMovementAcceleration_006a9690.cpp: SAME camera block (PeekWorld, PeekMyPlayer()->[+0x220],
  GetCurrentRenderCamera().GetAngleXY() else PCurrentGameCamera->GetControlAngleXY(), RotateXY via a `float&` reference, slot 87,
  frame 0x6C). Local models; key lesson: float angle local in both arms, `float& turn = angle; v.RotateXY(turn);` fixes lea order.
- 00/6a/CThingPlayerCreature_SetActionForMelee_006aabe0.cpp: shared-header idiom set (#define FABLE_RETAIL_CTHING,
  _CTHINGCREATUREBASE, _CTHINGPLAYERCREATURE, _C3DVECTOR, _CTCPHYSICSBASE, _CWORLD, _CAIGAMECAMERABASE, _PLAYER, _CVECTORMAP_FORCEINLINE_FIND...;
  `PeekTC(&x)`, `PhysicsTC->GetFacingVector()`, `GetWorld().GetGameCamera()`, `direction.RotateXY(angle)`, extern DELTA_NOTIONAL_ZERO).
- 00/6a/CThingPlayerCreature_UpdateWalkingControlForces_006ad260.cpp (#pragma intrinsic(sqrt)), 00/6a/CThingPlayerCreature_OnCollision_006ad580.cpp
  (pack(1) around C3DVector, #pragma auto_inline(off/on) helpers), 00/62/CWorld_EAMoveCreature_006295c0.cpp (caller, Flags: /GS).

## 7. Old draft model errors
- Every callee misnamed (see table); the local struct zoo should be the shared headers.
- Mode slot 7 args: (direction, PhysicsTC->GetFacingVector(), (EMovementType)0) -- draft passed velocity and `false`.
- Camera arm: world.GetCurrentRenderCamera() returns CCamera (0x38) -- the draft's C3DVector return is why VC packed the frame to
  0x1c instead of 0x68 (the "lane gap" in NOTES.md line 49 is actually this model error).
- Use PeekTC (const) -> `const CTCCreatureModeManager*`, GetCurrentMode() const, GetMovementSpeed const.
- Literal 0.0001f should be DELTA_NOTIONAL_ZERO; GetMaxSpeed -> GetMaxAcceleration.
- Failure branch of the TC test loads ebx from the action arg slot (`mov ebx,[esp+7Ch]`): the TC pointer is uninitialised when the
  bit is clear (PeekTC return value ignored); expect VC to emit something here only with the right local/flow.
- Map: write it as STLport map find/end/insert(value_type(action, pair<C3DVector,long>(scaled,1))) with
  `it->second.first += scaled; ++it->second.second;` (draft logic right, types local). Scaled vector built before the end() compare.

## 0x007735D0

# DOSSIER 0x007735D0 -- CTCShop::GetAttitudeBuyPriceMultiplier() const (448 bytes)

## 1. Retail shape (xdis 007735d0 0x1d0)
- __thiscall, plain `ret` (no args), float result in st0. No frame pointer (`sub esp,10h ; push ebp ; push esi ; mov ebp,ecx ; push edi`), so /Oy is ON: no `optimize("y",off)`. No SEH prologue, although two CDefPointer dtors exist. Not size-optimised (no `optimize("s")` idioms).
- Registers: this=ebp, ParentThing=edi, zero/shopDef=ebx (pushed late at 0x77362A, only on the def path), thingDef=esi from [esp+10h]. Frame words: [esp+10h]=thingDef out slot, [esp+14h]=shopDef out slot, reused for `multiplier` after it is read into ebx; [esp+18h]/[esp+1Ch] attitude key and find result; [esp+1Ch] key 4 for the hero LowerBound.
- Flow: (a) `test [edi+38h],200000h` = HasTC(TCI_BUYABLE_HOUSE=213) in place, then const LowerBound 0x004365B0 with key 0xD5 in [esp+10h] (+end-or-greater select), `[comp+11h]` OwnedByPlayer -> `fld 1.0f [0x0122DED8]` early return (pops only edi/esi/ebp: ebx not yet pushed).
  (b) GetDef(ParentThing->DefGlobalIndex movzx word +0x8C, &thingDef) 0x0042AF3C (result ignored), thingDef->GetSubDef(&shopDef) 0x005130A0; on failure: release shopDef (null-tested), release thingDef (null-tested), return 1.0f.
  (c) found: ebx=shopDef (read from slot, then NO null test on its release at 0x773768), multiplier=[ebx+60h] (AttitudeBuyPriceMultiplierDefault); `lea edi,[ebp+10h]` (&PShopKeeper); GetPItem() then GetPItem() again `mov ecx,[eax+28h]; test ch,ch; jns` = HasTC(79 TCI_CREATURE_OPINION_OF_HERO) (inline bitset test); PeekWorld() 0x00686A80 ->[+74h] (POpinionReactionManager_Data); 3rd GetPItem pushed; GetThingAttitudeToPlayer 0x00701840; attitude stored to [esp+18h]; const find 0x00701FC0 (hidden result [esp+1Ch], key by ref) on `lea esi,[ebx+64h]` (AttitudeBuyPriceMultiplier map); compare to [esi] (end = header) -> multiplier=[node+14h].
  (d) PeekPlayerManager() 0x00686D50 -> GetMainPlayer() const 0x00449960 -> GetPControlledCreature() 0x00487DC0; if non-null and `test byte [eax+20h],10h` (TCI_HERO_STATS=4) -> non-const LowerBound 0x0040F020 key 4 -> GetTradeSkill 0x0057C239 -> multiplier = multiplier + (1.0f - multiplier) * (skill * K) where K = float @0x01382998 (draft: 0.84f; unnamed in labels). x87 order: `fld K; fmul st(1)` (K*skill), `fld 1.0; fsub m; fmulp; fadd m; fstp m; fstp st0` -> skill stays on stack and is popped by `fstp st(0)`.
  (e) release shopDef (no null test), release thingDef (null test), `fld m`.

## 2. Identity
- Name/class: CTCShop::GetAttitudeBuyPriceMultiplier, mangled `?GetAttitudeBuyPriceMultiplier@CTCShop@@QBEMXZ` = `public: float __thiscall CTCShop::GetAttitudeBuyPriceMultiplier(void) const`.
  Sources: Ego_r ghidra_out/egor_pdb_names.tsv 0x007807A0 (Sell twin 0x00780980, same Buy->Sell order as retail 0x007735D0/0x00773790); FableWin ghidra_out/_fablewin_api.tsv 0x017E7769 + naming_stragglers/FableWin.exe.signatures.tsv; landed caller CTCShop_GetCurrentBuyPrice_007742c0.cpp calls it (call at 0x00774340) via engine/CTCShop.h FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE, which already declares it `// 0x007735D0`. Manifest label is CORRECT. Not virtual. Other caller 0x0079936A (xref.py).

## 3. Callees (landed file / landed name / PDB) -- draft disagreements flagged
| addr | landed row | landed/header name | PDB | draft spelling |
|---|---|---|---|---|
| 004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound const | same (FableWin) | WRONG: local CInterfaceMap::LowerBound + same-TU body |
| 0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<..>::LowerBound (non-const) | Ego_r | WRONG: local CInterfaceMap |
| 006869D0 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | `const CGameDefinitionManager& CTCBase::GetDefinitionManager() const` | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ | WRONG: CTCShop member returning CDefinitionManager* |
| 0042AF3C | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp | CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const T>*) const (header body FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, noinline template) | ??$GetDef@V..@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBV..@@@@@Z family | WRONG: GetDef_0042af3c, reference param, CDefClassBase pointee, local same-TU body |
| 005130A0 | 00/51/CDefClassBase_GetSubDefCShopDef_005130a0.cpp | `bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>*) const` | ??$GetSubDef@VCShopDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCShopDef@@@@@Z (Ego_r 0x004E1340) | WRONG: GetSubDef_005130a0, non-const, reference |
| 00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const -> CBaseObject* | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | WRONG return type CThing* |
| 00686A80 | 00/68/CTCBase_PeekWorld_00686a80.cpp | `const CWorld& CTCBase::PeekWorld() const` | ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ | WRONG: GetWorld, pointer |
| 00701840 | 00/70/COpinionReactionManager_GetThingAttitudeToPlayer_00701840.cpp | landed `long ...(const CThing*)` (non-const) | ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z (Ego_r 0x00551920): const, `const CThing&` | draft (pointer) WRONG; header COpinionReactionManager.h declares the PDB form only under FABLE_RETAIL_CTCWIFE_MARRY |
| 00701FC0 | 00/70/CRetailTreelongkey_find_00701fc0.cpp | CRetailTree_longkey::find (a generated/descriptive name) | std map<EOpinionAttitudeType,float>::find const (no Ego_r hit found; STLport hidden-iterator `ret 8`) | draft local CAttitudeMap::find; use the shared std::map in CShopDef.h (fable_map.h const find is declaration-only by default = out of line, matches retail) |
| 00686D50 | 00/68/CTCBase_PeekPlayerManager_00686d50.cpp | `const CPlayerManager& CTCBase::PeekPlayerManager() const` | ?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ | WRONG: GetPlayerManager, pointer |
| 00449960 | 00/44/CPlayerManager_GetMainPlayer_00449960.cpp | `const CPlayer& CPlayerManager::GetMainPlayer() const` | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ | WRONG: pointer return |
| 00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | `CThingPlayerCreature* CPlayer::GetPControlledCreature() const` | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ | WRONG: GetHeroThing returning CThing* |
| 0057C239 | 00/57/CTCHeroStats_GetTradeSkill_0057c239.cpp | float CTCHeroStats::GetTradeSkill() const | ?GetTradeSkill@CTCHeroStats@@QBEMXZ | OK (but engine/CTCHeroStats.h has NO GetTradeSkill declaration: author must add/opt-in or declare) |
| vtbl+4 | -- | CDefPointer release: `dec [p+4]; jne; call [vtbl+4]` | -- | use FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE + FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES (as GetCurrentBuyPrice) |

## 4. Members
- CTCShop (engine/CTCShop.h): ParentThing +0x04 (CTCBase), PShopKeeper +0x10 `CIntelligentPointer<CThingCreatureBase>` (PRIVATE; this is a member function so fine). Opt-ins: FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE (declares this row), FABLE_RETAIL_CTCSHOP_SHOPKEEPER (inline GetShopKeeper() = PShopKeeper.Get(); needs complete CThingCreatureBase).
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset +0x20 (bits 213 at +0x38 mask 0x200000, 79 at +0x28 bit15, 4 at +0x20 bit4), Components CVectorMap +0x44, DefGlobalIndex unsigned short +0x8C (GetDefGlobalIndex()). HasTC; FABLE_RETAIL_CTHING_HASTC_FORCEINLINE; GetTC/PeekTC(id, T**) expanded form under FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC uses const LowerBound 0x004365B0 = the house lookup; GetTC non-const 0x0040F020 = hero stats lookup).
- CTCBuyableHouse (engine/CTCBuyableHouse.h, FABLE_RETAIL_CTCBUYABLEHOUSE): OwnedByPlayer bool +0x11; inline GetOwnedByPlayer() under FABLE_RETAIL_CTCWIFE_MARRY or FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE; StatGetInterfaceType = TCI_BUYABLE_HOUSE.
- CShopDef (engine/CShopDef.h): AttitudeBuyPriceMultiplierDefault float retail +0x60, AttitudeBuyPriceMultiplier std::map<EOpinionAttitudeType,float> +0x64 (12-byte STLport; node value +0x10 key/+0x14 float). Sell twin: +0x70/+0x74.
- CWorld (engine/CWorld.h, FABLE_RETAIL_CWORLD): POpinionReactionManager_Data +0x74; inline PeekOpinionReactionManager() const returns `*POpinionReactionManager_Data`.
- CTCHeroStats: TCI_HERO_STATS=4 (FABLE_RETAIL_CTCHEROSTATS; StatGetInterfaceType under FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT).
- Locally modelled in the draft though shared headers exist: CVectorMap (as CInterfaceMap), CThing, CBaseIntelligentPointer (CIntelligentPointer.h), CTCBuyableHouse, CTCHeroStats, CDefPointer/CDefBase (CDefPointer.h), CShopDef, CDefClassBase, CDefinitionManager, COpinionReactionManager, CWorld, CPlayer, CPlayerManager, CTCShop, plus the map/iterator stand-ins (fable_map.h).

## 5. Callee-body visibility
- All callees are called out of line in retail. GetDef / GetSubDef: header noinline template bodies (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY) -- GetCurrentBuyPrice notes these make VC7.1 know the shop def is non-null after success, giving the un-null-tested release at 0x773768 (draft reproduced this with local same-TU bodies).
- LowerBound 0x0040F020/0x004365B0: out-of-line; CVectorMap.h has the same-TU noinline LowerBound (see IsShopKeeperAvailable note). 0x004365B0 is the const twin (separate body; GOTCHAS: don't route one through the other).
- map find 0x00701FC0: STLport const find, out of line; fable_map.h default `const_iterator find(const K&) const;` declaration-only. FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY would make it visible (chunk 1016 idiom: caller keeps node in a register) -- retail here re-reads the slot ([esp+1Ch]) so declaration-only is right.
- PeekWorld/PeekPlayerManager/GetDefinitionManager 0x00686xxx are CTCBase out-of-line protected const getters (other TU). GetMainPlayer, GetPControlledCreature, GetTradeSkill (0x0057C239 unaligned -> COMDAT/other TU), GetThingAttitudeToPlayer: other TUs, opaque.
- Same-TU neighbours (CTCShop.cpp, defined earlier): 0x007729A0 CalcIsStallLike, 0x00772790 GetDaysTillRestock ... 0x00773950 IsOpeningHours is AFTER this row. GetCurrentBasicPrice 0x007727E0 earlier but not called.

## 6. Siblings (rebuild/src/compiled/00/77/)
- CTCShop_GetCurrentBuyPrice_007742c0.cpp (caller; byte-exact): macros FABLE_RETAIL_BASE_OBJECT, _CTCBASE, _DEF_CLASS_BASE, _CDEFPOINTER_RELEASE_SELFDELETING_POINTEE, _CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, _CTHING, _CCHARSTRING, _CDEFSTRINGTABLE, _STLP_LIST, _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY, _CGAMEDEFINITIONMANAGER, _CRESOURCELIST, _CRESOURCE, _CTCSHOP_ATTITUDEPRICE; includes CThing.h, CThingBaseDef.h, CShopDef.h, CTCShop.h, CGameDefinitionManager.h; no pragmas. Body: `CDefPointer<const CThingBaseDef> thingDef; GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef); CDefPointer<const CShopDef> shopDef; if (!thingDef->GetSubDef(&shopDef)) return 0;` -- copy this prefix verbatim.
- CTCShop_GetCurrentSellPrice_007743b0.cpp: same macro set.
- CTCShop_IsShopKeeperAvailable_00773aa0.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED, CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), IS_THING_ALIVE, GetTC(id,&ptr).
- CTCShop_IsOpeningHours_00773950.cpp: lever note -- statement order (early `return true`) flipped VC7.1 IR-numbering tie in the inline GetTC; relevant to this row's register-web tie.
- CTCShop_SetShopKeeper_007751a0.cpp (Flags: none): FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CINTELLIGENTPOINTER(_ASSIGN).
- Flags in CTCShop rows: none (CalcIsStallLike, FrameUpdate, SetShopKeeper), /GS only on OpenShoppingInterface. Twin row 0x00773790 (Sell, +0x70/+0x74) is parked with the same 443v448.

## 7. Draft model errors (vs disasm)
- Residue: 443 v 448, register web only (draft this=edi/thing=ebp/zero=esi vs retail this=ebp/thing=edi/zero=ebx). Note retail pushes ebx LATE (after the house early return), so the house path is a separate early return before the def code; draft has that.
- Retail calls GetPItem() TWICE for `GetPItem() && GetPItem()->HasTC(79)` and a 3rd time for the attitude arg, then the HasTC is an inline bitset test (`test ch,ch; jns`) -- draft matches; but GetPItem returns CBaseObject*: needs a CThing cast (or CIntelligentPointer<CThingCreatureBase>::Get()).
- Draft's `__forceinline BuyMultiplierFromDefs` helper is invented; retail has one function. Writing it as a single body with all locals (thingDef/shopDef CDefPointers, float multiplier) in the shared-header spelling is the obvious untried variable -- all ~26 probes used the local stand-ins.
- Def/world/player getters wrong shapes (pointer vs reference; GetWorld/GetPlayerManager/GetHeroThing vs PeekWorld/PeekPlayerManager/GetPControlledCreature) and GetThingAttitudeToPlayer non-const pointer vs PDB const reference: these change calling declarations, possibly the IR order.
- thingDef pointee CDefClassBase in draft vs CThingBaseDef (landed caller); GetSubDef by reference vs pointer (`&shopDef`).
- Map find: draft local CAttitudeMap; retail std::map const find (hidden iterator result), header map from CShopDef.h. `end()` read as `[esi]` after the call through the map pointer kept in esi.
- Hero lookup uses NON-const LowerBound 0x0040F020 (GetTC on non-const CThingPlayerCreature*), house lookup the CONST one 0x004365B0 (ParentThing reached via const this) -- draft matches; keep that const split.

## 0x00773790

# DOSSIER 0x00773790 CTCShop::GetAttitudeSellPriceMultiplier (448 bytes)

## 1. Retail shape (xdis 00773790 0x1d0)
- __thiscall, no args, plain `ret` (three exits: 0x7737e2, 0x77384c, 0x77394f). Float return in st0.
- NO frame pointer (`sub esp,10h ; push ebp ; push esi ; mov ebp,ecx ; push edi`): ebp = this. No SEH, no /GS cookie. Default /O2-style TU (no optimize("y",off)).
- Registers: this=ebp, ParentThing=edi, zero/shopDef=ebx (ebx pushed lazily after the house test), thingDef=esi.
- Frame: [esp+10h] thingDef slot (CDefPointer), [esp+14h] shopDef slot, later reused as `multiplier` float; [esp+18h]/[esp+1Ch] find key/result, hero key 4.
- Flow:
  1. `test [edi+38h],200000h` = ParentThing HasTC(0xD5 TCI buyable house) in place; then expanded PeekTC (const LowerBound 0x004365B0, key 0xD5 stored after the test, map via `lea esi,[edi+44h]`, end via `[esi+4]`, `cmp [eax],0D5h ; jle`), `mov eax,[eax+4] ; mov cl,[eax+11h] ; test cl,cl` -> OwnedByPlayer -> `fld 1.0f [0x0122DED8]` return.
  2. `movzx edx,word [edi+8Ch]` (DefGlobalIndex) ; GetDefinitionManager 0x006869D0 ; GetDef 0x0042AF3C(index,&thingDef) (result ignored); GetSubDef<CShopDef> 0x005130A0 on thingDef (esi=[esp+10h]) -> false: release shopDef (null-checked), release thingDef (null-checked), return 1.0f.
  3. found: `mov ebx,[esp+14h]` (shopDef read once), multiplier = shopDef->AttitudeSellPriceMultiplierDefault (+0x70) into [esp+14h].
  4. `lea edi,[ebp+10h]` PShopKeeper; GetPItem 0x00A01B50 test; GetPItem again `mov ecx,[eax+28h]; test ch,ch; jns` = HasTC(0x4F) in place (word 2 bit 15); PeekWorld 0x00686A80 -> `[eax+74h]` (PeekOpinionReactionManager, inline); GetPItem a 3rd time pushed; GetThingAttitudeToPlayer 0x00701840; find 0x00701FC0 on `lea esi,[ebx+74h]` (AttitudeSellPriceMultiplier) with hidden result ptr + &key; `cmp eax,[esi]` (end = header); found -> multiplier = node+0x14 (second).
  5. PeekPlayerManager 0x00686D50 -> GetMainPlayer const 0x00449960 -> GetPControlledCreature 0x00487DC0; null test; `test byte [eax+20h],10h` HasTC(4 hero stats) in place; expanded GetTC NON-const LowerBound 0x0040F020 key 4; GetTradeSkill 0x0057C239.
     FPU: `fld [0x01382998] ; fmul st,st(1)` (scale*skill, skill left at st1), `fld 1.0 ; fsub [m] ; fmulp st(1) ; fadd [m] ; fstp [m] ; fstp st(0)` => m = (1-m)*(skill*scale) + m  (skill popped last).
  6. shopDef release WITHOUT null test (`dec [ebx+4]`), thingDef release with null test (`test esi,esi`), `fld [m]`.

## 2. Name / class / signature
- Name: `?GetAttitudeSellPriceMultiplier@CTCShop@@QBEMXZ` = `float CTCShop::GetAttitudeSellPriceMultiplier() const`.
  - Source 1: ghidra_out/egor_pdb_names.tsv 0x00780980 (Ego_r; Buy twin 0x007807A0 precedes it by 0x1E0, retail Buy 0x007735D0 precedes by 0x1C0 - same order).
  - Source 2: _fablewin_api.tsv / FableWin signatures 0x01854CF6 / 0x026219D0 (FableWin.pdb).
  - Source 3 (body): reads CShopDef +0x70 / +0x74 = AttitudeSellPriceMultiplierDefault / AttitudeSellPriceMultiplier (engine/CShopDef.h order, retail 12-byte maps).
  - Landed caller CTCShop::GetCurrentSellPrice 0x007743B0 (rebuild/src/compiled/00/77/CTCShop_GetCurrentSellPrice_007743B0.cpp) binds it via the call at 0x00774430; engine/CTCShop.h already declares it under FABLE_RETAIL_CTCSHOP_ATTITUDEPRICE (`// 0x00773790`).
  - labels_chunk272_corrections_2026-10-21.tsv / labels_callee_canon: 00773790 CTCShop::GetAttitudeSellPriceMultiplier (catalog's GetAttitudeBuyPriceMultiplier label was the propagated twin; manifest hint as given in the task is correct).
- NAME_FIXES: landed caller rebuild/src/compiled/00/79/CTCShopKeeper_GetFriendlynessIndicator_00799350.cpp declares 0x00773790 as local `CTCShop::GetBasePriceMultiplier()` (wrong; PDB is GetAttitudeSellPriceMultiplier const) and 0x007735D0 as GetAttitudeBuyPriceMultiplier (right). Report `00773790 GetBasePriceMultiplier -> GetAttitudeSellPriceMultiplier` for that caller.
- Twin 0x007735D0 GetAttitudeBuyPriceMultiplier (same body, +0x68/+0x6C, Buy map) is NOT landed either.

## 3. Callees (landed file / landed name / PDB) - draft spelling disagreements flagged
| addr | landed row | PDB / header spelling | draft spelling |
|---|---|---|---|
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) const (engine/CVectorMap.h) | local CInterfaceMap::LowerBound(const long*) const -- WRONG |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | same, non-const | local -- WRONG |
| 0x006869D0 | 00/68/CTCBase_GetDefinitionManager_006869d0.cpp | ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (returns const CGameDefinitionManager&) | CTCShop::GetDefinitionManager() returning CDefinitionManager* -- WRONG class+return |
| 0x0042AF3C | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp | CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (sibling uses GetDef(ParentThing->GetDefGlobalIndex(), &thingDef) with CDefPointer<const CThingBaseDef>) | GetDef_0042af3c(long, CDefPointer&) -- WRONG |
| 0x005130A0 | 00/51/CDefClassBase_GetSubDefCShopDef_005130a0.cpp | ??$GetSubDef@VCShopDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCShopDef@@@@@Z (const, takes CDefPointer<const CShopDef>*) | GetSubDef_005130a0(CDefPointer<CShopDef>&) -- WRONG |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | returns CThing* -- WRONG return |
| 0x00686A80 | 00/68/CTCBase_PeekWorld_00686a80.cpp | ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (const CWorld&) | CTCShop::GetWorld() const CWorld* -- WRONG |
| (inline) | CWorld.h:204 | CWorld::PeekOpinionReactionManager() const, reads POpinionReactionManager_Data +0x74 | raw member OpinionReactionManager |
| 0x00701840 | 00/70/COpinionReactionManager_GetThingAttitudeToPlayer_00701840.cpp | ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z (Ego_r 0x00551920; takes const CThing&; header COpinionReactionManager.h:22 has it) | (const CThing*) -- WRONG param (landed row itself also spells it (const CThing*) returning long, non-const: NAME_FIXES candidate) |
| 0x00701FC0 | 00/70/CRetailTreelongkey_find_00701fc0.cpp (generic folded body "CRetailTree_longkey::find", one of 29) | std::map<EOpinionAttitudeType,float>::find (STLport _Rb_tree find; folded COMDAT) | CAttitudeMap::find -- local |
| 0x00686D50 | 00/68/CTCBase_PeekPlayerManager_00686d50.cpp | ?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ (CTCBase.h:177 under FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER) | CTCShop::GetPlayerManager() -- WRONG (GetPlayerManager is 0x00686D40) |
| 0x00449960 | 00/44/CPlayerManager_GetMainPlayer_00449960.cpp | ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (const CPlayer&; CPlayerManager.h:23 under FABLE_RETAIL_CPLAYERMANAGER) | returns CPlayer* |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ (CPlayer.h:174) | CPlayer::GetHeroThing -- WRONG name |
| 0x0057C239 | 00/57/CTCHeroStats_GetTradeSkill_0057c239.cpp | ?GetTradeSkill@CTCHeroStats@@QBEMXZ (Ego_r 0x0056E5CA) | matches; NOT declared in engine/CTCHeroStats.h (needs a declaration; no opt-in macro exists) |
| [vtbl+4] | - | CDefPointer release: slot 1 of the def pointee (header CDefPointer, FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE) | local CDefBase::Destroy |
| globals | 0x0122DED8 = 1.0f literal; 0x01382998 trade scale (draft g_TradeSkillAttitudeScale, 0.84f; name unproven -- grep landed users before naming) |

## 4. Members
- CTCBase::ParentThing +0x04 (engine/CTCBase.h; CTCShop derives CTCBase); use GetParentThing() const.
- CTCShop::PShopKeeper +0x10 CIntelligentPointer<CThingCreatureBase> (private in engine/CTCShop.h; member function has access). Header GetShopKeeper() inline under FABLE_RETAIL_CTCSHOP_SHOPKEEPER (needs complete CThingCreatureBase); retail calls GetPItem three times (the draft's PShopKeeper.GetPItem() x3 pattern).
- CThing::UsingComponents (bitset<274>) +0x20, Components (CVectorMap) +0x44, DefGlobalIndex (unsigned short) +0x8C via GetDefGlobalIndex() (CThing.h:267). Use FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC(id,&p) const for the house [const LowerBound], GetTC(id,&p) for the hero [non-const LowerBound]) -- matches retail's key-store-after-test / map-pointer-local idiom.
- CTCBuyableHouse::OwnedByPlayer +0x11 bool (struct_layouts_egor.tsv offset 17; engine/CTCBuyableHouse.h under FABLE_RETAIL_CTCBUYABLEHOUSE, inline GetOwnedByPlayer() const under FABLE_RETAIL_CTCWIFE_MARRY || FABLE_RETAIL_CTCBUYABLEHOUSE_WIFE). Retail `mov cl,[eax+11h]; test cl,cl` suggests the inline getter.
- CShopDef::AttitudeSellPriceMultiplierDefault +0x70, AttitudeSellPriceMultiplier std::map<EOpinionAttitudeType,float> +0x74 (engine/CShopDef.h, sizeof 0xD0 retail; struct_layouts_egor has +124/+128 in the Ego_r 16-byte-container layout). Node value `second` at node+0x14.
- CWorld::POpinionReactionManager_Data +0x74 (CWorld.h:328) via PeekOpinionReactionManager().
- Hero stats interface 4 = TCI_HERO_STATS; shopkeeper interface 0x4F (opinion source? check ETCInterfaceType.h for the enumerator); house 0xD5.
- Classes the draft models LOCALLY that have shared headers: CThing, CVectorMap (CInterfaceMap), CBaseIntelligentPointer (CIntelligentPointer.h), CTCBuyableHouse, CTCHeroStats, CDefPointer/CDefBase (CDefPointer.h), CShopDef, CDefClassBase, CDefinitionManager/CGameDefinitionManager, COpinionReactionManager, CWorld, CPlayer, CPlayerManager, CTCShop, std::map (stlport/fable_map.h).

## 5. Callee visibility
- 0x0042AF3C (unaligned, COMDAT template GetDef<T>) and 0x005130A0 GetSubDef<CShopDef>: visible inline templates retail emitted out of line. Opt-ins FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (used by both price siblings; sibling note: "the headers' noinline GetDef / GetSubDef bodies keep the thing def in ebp and re-read the shop def once on the found path"). The missing null test on the final shopDef release (0x773928) says the compiler knew shopDef non-null on the found path => GetSubDef body visible.
- 0x0057C239 GetTradeSkill: unaligned => an inline/COMDAT emitted out of line, frame-pointer size TU; call stays out of line.
- 0x00701FC0 map::find: STLport template, called out of line (folded COMDAT shared by 29 map instantiations).
- Same-TU neighbours (CTCShop.cpp, defined earlier): 0x007735D0 Buy twin (unlanded); landed 0x007729A0 CalcIsStallLike, 0x00772790 GetDaysTillRestock etc. Callees PeekWorld/PeekPlayerManager/GetDefinitionManager are CTCBase.cpp, out of line.

## 6. Sibling landed rows
- 00/77/CTCShop_GetCurrentSellPrice_007743b0.cpp (direct caller, same TU): macros FABLE_RETAIL_BASE_OBJECT, _CTCBASE, _DEF_CLASS_BASE, _CDEFPOINTER_RELEASE_SELFDELETING_POINTEE, _CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES, _CTHING, _CCHARSTRING, _CDEFSTRINGTABLE, _STLP_LIST, _CDEFINITIONMANAGER, _CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY, _CGAMEDEFINITIONMANAGER, _CRESOURCELIST, _CRESOURCE, _CTCSHOP_SELLPRICE, _CTCSHOP_ATTITUDEPRICE; includes CThing.h, CThingBaseDef.h, CShopDef.h, CTCShop.h, CGameDefinitionManager.h. Body: `CDefPointer<const CThingBaseDef> thingDef; GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef); CDefPointer<const CShopDef> shopDef; if (!thingDef->GetSubDef(&shopDef)) return ...;` -- copy this prologue verbatim.
- 00/77/CTCShop_GetCurrentBuyPrice_007742c0.cpp: same macro set (twin).
- 00/77/CTCShop_IsShopKeeperAvailable_00773aa0.cpp, CTCShop_SetStockState_00773c00.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER).
- 00/77/CTCShop_IsOpeningHours_00773950.cpp (next function in retail): still local-model; lever note on if-ordering moving the GetTC key/map lea tie-break.
- 00/70/COpinionReactionManager_GetThingAttitudeToPlayer_00701840.cpp, 00/79/CTCShopKeeper_GetFriendlynessIndicator_00799350.cpp (caller).
- No `// Flags:` lines in these siblings (OpenShoppingInterface 0x00773B20 has `// Flags: /GS` but this row has no cookie).

## 7. Old draft's likely model errors
- __forceinline helper SellMultiplierFromDefs is invented; write one flat body (house test, then defs) -- the residue was a register tie-break (ours this=edi/thing=ebp/zero=esi vs retail ebp/edi/ebx); shared-header bodies + exact callee signatures (references, const) change IR and are the first lever.
- Local noinline stand-in GetDef/GetSubDef bodies with by-reference out params instead of the header's CDefPointer<const T>* out-pointer templates.
- GetPItem returning CThing* (should be CBaseObject*, cast); GetThingAttitudeToPlayer by pointer (PDB by const reference); GetWorld/GetPlayerManager/GetHeroThing wrong names; GetMainPlayer pointer vs const CPlayer&.
- House: draft HasTC + GetTC const roughly right, but should use header PeekTC/GetTC expanded forms and CTCBuyableHouse header.
- Trade formula: retail computes scale*skill with skill kept on the FPU stack (fstp st(0) at the end); draft `(1-m)*(skill*scale)` then `+=` -- check operand order: `fld scale ; fmul st,st(1)` then `(1-m)` times that, plus m.
- Final shopDef release without null test (needs visible GetSubDef body), thingDef release with null test.

## 0x007FD650

# DOSSIER 0x007FD650 CTCDPhysicalObstruction::FrameUpdate (448 bytes)

## 1. Retail shape (xdis 007fd650 0x1d0)
- thiscall, plain `ret` (0x007FD80F), no args. No frame pointer: `sub esp,0xd8 ; push ebx ; push ebp ; mov ebp,ecx` (ebp = this, NOT /Oy-). No SEH prologue (no destructors run: CEnginePrimitiveLine/C3DVector/CRGBColour temporaries are trivially destroyed). Plain /O2 idioms.
- `xor ebx,ebx` zero register used for all 0 pushes/stores; esi/edi pushed only inside the debug block (push esi at 0x7FD683, push edi 0x7FD688; popped at 0x7FD7E5 before the shared tail).
- Body: `if (GetThingManager().<byte +0x78>) { 3 x SetPersistentPrimitive(line,CEnginePrimitiveLine(...),0) } UpdateAdditionalImpulse(); if (LifeTimer > 0 && --LifeTimer == 0) ParentThing->Kill(true);` (tail: `cmp eax,ebx ; jle ; dec eax ; cmp ; mov [ebp+24h],eax ; jne ; push 1 ; mov ecx,[ebp+4] ; call 0x004C9B80`).
- Engine is re-read from the global EVERY call: `mov eax,[0x013B86A0] ; mov ecx,[eax+28h] ; mov eax,[ecx+2Ch]` = CMainGameComponent::PMainGameComponent->PDisplayEngine(+0x28)->Engine3D(+0x2C), read BEFORE each ctor; the engine ptr and its vtable are saved to frame slots ([esp+..]) and reloaded after the ctor (`mov ecx,[esp+30h]; call [edi+5Ch]`).
- Each of the 3 calls: pushes `0` FIRST (=SetPersistentPrimitive's 3rd arg `unsigned long`), then ctor args right-to-left: blendop 0, interp 0, use_z 1, bank 0, blend 0, width 0x3D4CCCCD (0.05f), &colour, &endVec, &startVec, id hi 0, id lo 0; `call 0x004C3090` (returns this), `push eax ; lea ecx,[ebp+38h/40h/48h] ; push ecx ; call [vt+5Ch]`.
- Colour bytes: [+2]=0 (R), [+1]=0 (G), [+0]=0xFF (B), [+3]=0xFF (A) stored in R,G,B,A order -> CRGBColour(0,0,255,255) via the R,G,B,A-storing ctor. In call 3 the 0xFF comes from `mov cl,0FFh` and colour stores are interleaved with the pushes.
- Vectors: line1 end = (StartPos.X, StartPos.Y, StartPos.Z + 2.0f) [2.0f @ 0x0122DCDC, `fadd`]; line2 end = EndPos analog; line3 = (Start lifted, End lifted). In line3 the "EndPos lifted" vector (edi=&EndPos) is computed first (fld [edi+8]) then StartPos lifted (fld [esi+8]); start arg = &start-lifted ([esp+4Ch] region), end arg = &end-lifted.
- FRAME: each CEnginePrimitiveLine temporary has its own 0x3C slot (lea ecx,[esp+0xA0] / [esp+0x64] / [esp+0xDC] with different push depths) -> retail did NOT share temporaries across the 3 statements: total 0xD8.

## 2. Identity (two sources)
- Ego_r.pdb `?FrameUpdate@CTCDPhysicalObstruction@@UAEXXZ` (Ego_r 0x007F8900, right after UpdateAdditionalImpulse 0x007F8820 -- same order as retail 0x007FD580 -> 0x007FD650) [ghidra_out/egor_pdb_names.tsv].
- rtti-vtables.tsv: CTCDPhysicalObstruction vtable 0x01241C0C slot 9 = 0x007FD650; xref: only reference is .rdata 0x01241C30 (=0x01241C0C+0x24 = slot 9). CTCBase.h slot 9 = `virtual void FrameUpdate();` (0x006868A0). FableWin also has ?HasFrameUpdate@CTCDPhysicalObstruction@@UBE_NXZ (slot 8, 0x004DF1C9).
- Mangled leaf: `?FrameUpdate@CTCDPhysicalObstruction@@UAEXXZ` -- public virtual void FrameUpdate(), non-const. Manifest label CORRECT.

## 3. Callees
| retail | landed row | landed name | PDB | draft |
|---|---|---|---|---|
| 0x00686AE0 | rebuild/src/compiled/00/68/CTCBase_GetThingManager_00686ae0.cpp | CTCBase::GetThingManager | ?GetThingManager@CTCBase@@IAEAAVCThingManager@@XZ (protected, returns CThingManager&) | draft returns CThingManager* -- WRONG type (reference) |
| 0x004C3090 | rebuild/src/compiled/00/4c/CEnginePrimitiveLine_CEnginePrimitiveLine_004c3090.cpp | CEnginePrimitiveLine ctor | ??0CEnginePrimitiveLine@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MW4EEnginePrimitiveBlendMode@@K_NW4EEngineInterpolationSet@@W4EEnginePrimitiveBlendOp@@@Z | draft uses `long` for the 3 enums -- wrong spelling |
| vslot 23 (+0x5C) | CEngine 0x00B23BC0 | CIEngine::SetPersistentPrimitive | ?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z (3 args!) | draft declares 2 args -- WRONG: retail pushes the 3rd arg 0 |
| 0x007FD580 | rebuild/src/compiled/00/7f/CTCDPhysicalObstruction_UpdateAdditionalImpulse_007fd580.cpp | CTCDPhysicalObstruction::UpdateAdditionalImpulse | ?UpdateAdditionalImpulse@CTCDPhysicalObstruction@@AAEXXZ (private) | ok |
| 0x004C9B80 | rebuild/src/compiled/00/4c/CThing_Kill_004c9b80.cpp | CThing::Kill | ?Kill@CThing@@QAEX_N@Z; CThing.h:248 `void Kill(bool);` | ok |
Inline accessors (expanded in place): CMainGameComponent::GetDisplayEngine() (FableWin ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ), CDisplayEngine::Get3DEngine() (?Get3DEngine@CDisplayEngine@@QAEAAVCIEngine@@XZ), CThingManager::IsDrawCombatCollisionDebug() (?IsDrawCombatCollisionDebug@CThingManager@@QBE_NXZ, FableWin + Ego_r).

## 4. Members
- this (CTCDPhysicalObstruction, rebuild/include/engine/CTCDPhysicalObstruction.h, includes CTCBase with FABLE_RETAIL_CTCBASE forced): ParentThing +0x04 (CTCBase), StartPos +0x0C C3DVector, EndPos +0x18 C3DVector, LifeTimer +0x24 long. line1/line2/line3 +0x38/+0x40/+0x48 (Ego_r CEnginePrimitiveHandle each, Ego_r offsets 60/68/76 -> retail -4 because retail vector is 12 bytes): header models them as `unsigned char Lines[0x18]` -- NO opt-in names them; author must add a retail opt-in (e.g. FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_LINES typing `CEnginePrimitiveHandle line1, line2, line3;`, size stays 0x18; CEnginePrimitiveHandle.h = CBaseClass vptr + CIVCountedPointer Data, 8 bytes) or cast. Existing opt-ins: _CREATE, _VSLOTS, _DRIVERSLOTS (none needed).
- CThingManager +0x78 = `bool DrawCombatCollisionDebug` (retail layout in CThingManager.h: map +0x58..0x64, CurrentUniqueID +0x68, CurrentLocalThingUniqueID +0x70, DrawCombatCollisionDebug +0x78; Ego_r has it at 144 due to its bigger map). Accessor `bool IsDrawCombatCollisionDebug() const` under FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT (CThingManager.h:23-26). WARNING: that macro also switches CEnginePrimitiveLine.h to the plain-inline ctor body (#ifdef branch wins over CTOR_VISIBLE_OUTOFLINE) and includes CEnginePrimitiveEnums/CEngineRenderEnums -- check the ctor stays out-of-line (retail calls 0x004C3090), else read the member another way. The draft's "DebugDisplayObstructions" is a made-up name: the real member is DrawCombatCollisionDebug.
- CMainGameComponent::PMainGameComponent (static, 0x013B86A0) +0x28 PDisplayEngine (scoped_ptr) -> GetDisplayEngine() under FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE (needs FABLE_RETAIL_CMAINGAMECOMPONENT); CDisplayEngine +0x2C Engine3D (struct_layouts_egor CDisplayEngine 44 `CIEngine* Engine3D`) -> Get3DEngine() under FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE (+FABLE_RETAIL_CIENGINE for polymorphic CIEngine; CIEngine.h:97 slot 23 `SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) = 0`). Users of GET3DENGINE: 0056F30B CTCInventoryExperience_CloseInventory, 007C9160 CTCFireballSpell_OnKill.
- Draft LOCAL models that have shared headers: C3DVector, CRGBColour, CEnginePrimitiveHandle, CEnginePrimitive, CEnginePrimitiveLine, CIEngine, CDisplayEngine, "CEngineGlobals" (=CMainGameComponent), CThingManager, CThing, CTCDPhysicalObstruction (all exist in rebuild/include/engine/). CRGBColour.h:12 ctor stores R,G,B,A (matches retail).

## 5. Visibility
- CEnginePrimitiveLine ctor 0x004C3090: inline in original header but called out of line; opt-in FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE (noinline, body visible) -- used by DrawShapes 0x00761980. Body visibility may be what changes temporary-slot sharing (hypothesis for the 0xD8 frame).
- UpdateAdditionalImpulse 0x007FD580 is the same TU, defined just before (Ego_r order UpdateAdditionalImpulse then FrameUpdate) -- its body was visible to retail's compiler (not inlined: called). Landed row compiles its own TU stand-ins.
- GetThingManager 0x00686AE0 (CTCBase TU) and Kill 0x004C9B80 (CThing TU): other TUs, not visible.
- No STLport helpers called.

## 6. Siblings
- 0x007FD3C0 OnInitialActivate: `// Flags: none`, `#include "engine/CTCDPhysicalObstruction.h"` (shared header idiom).
- 0x007FD430 Construct: FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCLASS, engine/CTCBase.h, CTCPhysicsBase.h.
- 0x007FD580 UpdateAdditionalImpulse: local models + `#pragma auto_inline(off)` stand-in bodies for same-TU callees.
- 0x00761980 CTCShapeManager::DrawShapes (closest idiom: line temporaries): Flags none; defines FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CIENGINE(+_RENDER), FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE; spells `CEnginePrimitiveLine(0, a, b, CRGBColour(0xff,0,0,0xff), 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0)`.
- 0x007DCE40 CTCCreatureFlyOnSpline::DrawPreparePrimitivesForRendering: SetPersistentPrimitive with 3 args (handle, line, flags) through slot 23.

## 7. Old draft model errors
1. SetPersistentPrimitive declared with 2 params; retail passes 3 (third = 0, the first `push ebx` before the ctor args). This alone shifts every push/frame offset.
2. GetThingManager returns CThingManager& (not *); member name DrawCombatCollisionDebug via IsDrawCombatCollisionDebug().
3. Engine accessor: should be CMainGameComponent::PMainGameComponent->GetDisplayEngine().Get3DEngine() (reference chain); draft uses made-up g_pEngineGlobals.
4. Enum params spelled `long`; CEnginePrimitiveHandle modelled as POD (real one is CBaseClass-derived polymorphic, 8 bytes) -- handle type only affects the reference argument.
5. Frame residue (0xD8 vs shared slots) recorded as lane gap; try the shared headers + visible ctor + 3-arg call before blaming frame sharing. In line3 the end-lifted vector is evaluated before start-lifted (args right-to-left -> natural if both are temporaries in the call).

