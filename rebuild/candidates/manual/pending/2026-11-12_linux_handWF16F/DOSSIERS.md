## 0x006D8F10

# DOSSIER 0x006D8F10 -- CHeroExperienceDef::Transfer (692 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 006d8f10 0x2c4)
- __thiscall virtual, `ret 4` (one arg: CPersistContext&). No frame pointer, no SEH, no `sub esp`. Prologue push ebx/esi/edi;
  esi = context (arg), edi = this. **ebp is shrink-wrapped**: `push ebp` at 0x6D9064 (just before the 8th transfer, the
  first StatIncreasePerHitType) and `pop ebp` at 0x6D91A3 (inside the last transfer, between `sub eax,2` and `mov edx,edi`).
  Two epilogues (pop edi/esi/ebx ; ret 4) -- one per arm of the final vector transfer (tail duplicated).
- 14 inline transfers, each `push 0x0122D70E ("") ; mov ecx,esi ; lea ebx,[edi+off] ; call CheckCRC 0x404500`, then
  `mov eax,[esi+0x18] ; sub eax,2`:
  - vectors (+0x28,+0x34,+0x40 vector<long>; +0xa8 vector<CAttackHistoryCombo>): `mov edx,ebx` BEFORE `je` (common to both
    arms), je -> In(ecx=[esi+0x24]) else Out(ecx=[esi+0x28]) => two-way `Mode==2 ? In : Out` (TransferVector body).
  - maps and floats: `je load ; dec eax ; jne done` => switch {case 2 load; case 3 save}; edx loaded inside each arm.
  - floats (+0x64,+0x68): save = `mov eax,[ebx] ; mov ecx,[esi+0x28] ; push eax ; call WriteFloat 0x993EE0` (integer copy of
    the float, no x87); load = `push ebx(&value) ; push [esi+0x24](stream) ; lea ecx,[esp+0x18] ; call 0x40EFB0` -- the
    CPersistTraits<float> traits temporary lives in the dead `context` argument slot.
- Transfer order: Thresholds +28, HeroLevelTitles +34, FallbackMultipliers +40, SoundIntensityThresholds +4c, ComboMultiplierMap +58,
  float +64, float +68, StatIncreasePerHitType +9c (ebp), ComboMultiplierIncrease +6c, Lock +78, Lost +84,
  RunningHitsIncrease +90, StatIncreasePerHitType AGAIN (reuses ebp, 0x6D915E: no lea), AttackHistoryCombos +a8 (`add edi,0xa8`).
  The double transfer of +0x9c is real (same register, same address); keep it.
- Size idioms: none of the `optimize("s")` kind (per-member `lea ebx,[edi+off]` reused across arms) -- plain /O2. No /Oy-.

## 2. Name / signature (manifest label `CHeroExperienceDef::CHeroExperienceDef::Transfer` has a doubled class; real name below)
- Vtable: rebuild/manifest/rtti-vtables.tsv `CHeroExperienceDef 0124390c 18 006d8f10`; xref.py: only reference is the .rdata
  slot 0x01243954 (= 0x0124390C + 18*4). CDefClassBase.h: slot 18 = `virtual void Transfer(CPersistContext&) = 0`.
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0058a900) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 017a57c4/01ef6b20,
  naming_stragglers FableWin.exe.signatures.tsv 01ef6b20, 317 bytes): `?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z`
  => `public: virtual void __thiscall CHeroExperienceDef::Transfer(class CPersistContext &)`. Two sources + vtable slot.

## 3. Callees (landed file / landed name / PDB name)
| addr | landed file | PDB name (egor_pdb_names) | draft spelling |
|---|---|---|---|
| 0x00404500 | 00/40/CPersistContext_CheckCRC_00404500.cpp | CPersistContext::CheckCRC(const char*) | ok |
| 0x00466A75 | 00/46/global_GFSerialiseVectorBinaryOutfloat_00466a75.cpp (shared <float>/<long> folded body) | ??$GFSerialiseVectorBinaryOut@M@@YIXAAVCDataOutputStream@@AAV?$vector@M...@Z (no <J> Out in PDB: COMDAT-folded) | draft `GFSerialiseVectorBinaryOutLong(CDataOutputStream*,CLongVector*)` WRONG: template GFSerialiseVectorBinaryOut<long>(CDataOutputStream&, vector<long>&) |
| 0x00466AA7 | 00/46/global_GFSerialiseVectorBinaryIn_00466aa7.cpp, Leaf ??$GFSerialiseVectorBinaryIn@J@@YIXAAVCMemoryDataInputStream@@AAV?$vector@JV?$allocator@J@std@@@std@@@Z | same | draft `...InLong(ptr,ptr)` WRONG (refs, template) |
| 0x006DAE40 | 00/6d/global_GFSerialiseMapBinaryOut_006dae40.cpp | ??$GFSerialiseMapBinaryOut@VCDefString@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@ABV?$map@JVCDefString@@...@Z (map by CONST ref) | draft `WriteDefStringMap` WRONG |
| 0x006DBA50 | 00/6d/global_GFSerialiseMapBinaryIn_006dba50.cpp | ??$GFSerialiseMapBinaryIn@VCDefString@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@JVCDefString@@...@Z | draft `ReadDefStringMap` WRONG |
| 0x006DC350 | 00/6d/global_GFSerialiseMapBinaryOut_006dc350.cpp | ??$GFSerialiseMapBinaryOut@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCDataOutputStream@@ABV?$map@... | draft `WriteComboMultiplierMap` WRONG |
| 0x006DCFA0 | 00/6d/global_GFSerialiseMapBinaryIn_006dcfa0.cpp | ??$GFSerialiseMapBinaryIn@VCComboMultiplierData@@JU?$less@J@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@... | draft `ReadComboMultiplierMap` WRONG |
| 0x006DC450 | 00/6d/GFSerialiseMapBinaryOutCHeroStatIncreaseDataEDamageAttribute_WriteToStream_006dc450.cpp (file says "Actual GFSerialiseMapBinaryOut<CHeroStatIncreaseData,EDamageAttribute>, not manifest WriteToStream") | ??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCDataOutputStream@@ABV?$map@... | draft `WriteStatIncreaseMap` WRONG; landed filename leaf also wrong (NAME_FIXES candidate) |
| 0x006DD1C0 | 00/6d/CHeroExperienceDef_ReadFromStream_006dd1c0.cpp (Leaf: ReadFromStream -- WRONG) | ??$GFSerialiseMapBinaryIn@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIXAAVCMemoryDataInputStream@@AAV?$map@... | draft `ReadStatIncreaseMap` WRONG; landed name wrong too (NAME_FIXES: CHeroExperienceDef::ReadFromStream -> GFSerialiseMapBinaryIn<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>>) |
| 0x006DC550 | 00/6d/global_GFSerialiseVectorBinaryOutCAttackHistoryCombo_006dc550.cpp | ??$GFSerialiseVectorBinaryOut@VCAttackHistoryCombo@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCAttackHistoryCombo@@...@Z | draft `...OutCombo(ptr,ptr)` WRONG |
| 0x006DD580 | 00/6d/global_GFSerialiseVectorBinaryInCAttackHistoryCombo_006dd580.cpp | ??$GFSerialiseVectorBinaryIn@VCAttackHistoryCombo@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@... | draft `...InCombo` WRONG |
| 0x00993EE0 | 00/99/CDataOutputStream_WriteFloat_00993ee0.cpp | ?WriteFloat@CDataOutputStream@@QAEXM@Z | ok |
| 0x0040EFB0 | 00/40/CPersistTraitsfloat_TransferBinaryIn_0040efb0.cpp | ?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z (refs!) | draft takes pointers (stream*, float*) -- PDB is (CMemoryDataInputStream&, float&) |
All GFSerialise* are `__fastcall` (YI) free function templates taking references (ecx=stream, edx=container).

### PDB names of the INLINE transfer templates the source called (egor_pdb_names.tsv) -- retail inlined all of them:
- `??$TransferVector@J@CPersistContext@@QAEXPBDAAV?$vector@J...@Z` and `??$TransferVector@VCAttackHistoryCombo@@@CPersistContext@@QAEXPBDAAV?$vector@...@Z`
  => `context.TransferVector("", vec)` (named 2-arg), not `Transfer`.
- `??$TransferMap@VCDefString@@J@CPersistContext@@QAEXPBDAAV?$map@...@ABVCDefString@@@Z`, same for CComboMultiplierData and
  CHeroStatIncreaseData,EDamageAttribute: ONLY the 3-arg form (name, map&, const V& default) exists for these V -> source was
  `TransferMap(name, map, default)` or an inline 2-arg wrapper forwarding a default (cf. CPersistContext.h NAMED_DEFAULTS
  `Transfer(name,value){Transfer(name,value,GetDefaultVal<T>());}`).
- `??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z` and `...AAMABM@Z` (float, 2- and 3-arg).
- Draft uses `context.Transfer("", x)` overloads for everything and local CPersistTraits<map> specialisations: wrong spellings.

## 4. Members (Ego_r struct_layouts_egor.tsv offsets are PDB +0x10-per-container; retail re-based)
Shared header: rebuild/include/engine/CHeroExperienceDef.h (EXISTS -- the draft's "No generated engine header" is stale).
`class CHeroExperienceDef : public CSubDefClassBase` (CSubDefClassBase.h needs `#define FABLE_RETAIL_DEF_CLASS_BASE`; base 0x25
packed -> first member at 0x28), sizeof 0xb4. Members: ExperienceLevelThresholds +28, HeroLevelTitles +34, FallbackMultipliers +40
(vector<long>), SoundIntensityThresholds +4c (map<long,CDefString>), ComboMultiplierMap +58 (map<long,CComboMultiplierData>),
ExperienceMultiplierParamPerUnitComboMultiplier +64, ExperienceMultiplierRegionChangeMultiplier +68 (float),
ComboMultiplierIncrease +6c, ComboMultiplierLock +78, ComboMultiplierLost +84, ComboMultiplierRunningHitsIncrease +90
(map<long,CDefString>), StatIncreasePerHitType +9c (map<EDamageAttribute,CHeroStatIncreaseData>), AttackHistoryCombos +a8
(vector<CAttackHistoryCombo>). Names match the draft's.
- Header models all containers as `unsigned char X[12]` except opt-in FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT which makes
  StatIncreasePerHitType a real std::map<EDamageAttribute,CHeroStatIncreaseData> (includes CHeroStatIncreaseData.h,
  EDamageAttribute.h). No opt-in for real vector/map types of the other members, and NO Transfer declaration in the
  header: the author must add an opt-in (e.g. FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER declaring `virtual void Transfer(CPersistContext&)`
  and real STLport member types) -- not a local struct.
- CPersistContext: rebuild/include/engine/CPersistContext.h (opt-in FABLE_RETAIL_CPERSISTCONTEXT; fields Mode +0x18 (EMode,
  MODE_LOAD_BINARY=2, MODE_SAVE_BINARY=3), PLoadStream +0x24, PSaveStream +0x28; CheckCRC(const char*)). Opt-ins relevant:
  FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY (bodies of one-arg Transfer<T>(T&) [traits local, switch 2/3] and
  TransferVector<T>(vector&) [2 / default] -- exactly retail's arm shapes, but unnamed one-arg forms; no landed user yet),
  FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (named Transfer -> 3-arg; declares 3-arg TransferMap, declaration only),
  FABLE_RETAIL_GFSERIALISEMAPBINARY_DECL (map serialiser declarations; In body under FABLE_RETAIL_GFSERIALISEMAPBINARYIN in
  engine/CPersistTraits.h). Streams: FABLE_RETAIL_CMEMORYDATAINPUTSTREAM / CDATAINPUTSTREAM / CDATAOUTPUTSTREAM.
- Classes the draft models LOCALLY that have shared headers: CPersistContext, CHeroExperienceDef (+ base CSubDefClassBase /
  CDefClassBase), CDataOutputStream (engine/CDataOutputStream.h), CMemoryDataInputStream, CPersistTraits (engine/CPersistTraits.h),
  CHeroStatIncreaseData; the fake CLongVector / CDefStringMap / ... should be std::vector / std::map (stlport/fable_containers.h).

## 5. Callee visibility
- Every callee is out of line in retail (calls, no inlined bodies). Template bodies GFSerialise*<...> sit right after this
  function (0x6DAE40..0x6DD580, same TU COMDATs) -- they are instantiated in this TU, so their bodies are visible; they stay
  called because they're big. CheckCRC (0x404500), WriteFloat (0x993EE0), CPersistTraits<float>::TransferBinaryIn (0x40EFB0)
  live in other TUs (low addresses / CDataOutputStream module).
- TU = CTCHeroExperience's .cpp: neighbours 0x6D7120..0x6D8E30 are CTCHeroExperience members (OnSerialise 0x6D8E30 precedes),
  plus CDefClassBase::GetSubDef<CHeroExperienceDef> 0x6DAA90 and the CPersistTraits<CAttackHistoryCombo/CComboMultiplierData/
  CHeroStatIncreaseData> TransferBinaryIn/Out rows (0x6D9EE0, 0x6DB060, 0x6DB0F0, 0x6DBE00, 0x6DC070, 0x6DC6A0) after it.
- Per GOTCHAS Q24: visible same-TU noinline callee bodies taking a container by reference tell c2 the address does not
  escape -- declaring the GFSerialise* templates with bodies (or not) may change register/save decisions; worth a try for the ebp
  shrink-wrap.

## 6. Siblings (idioms)
- rebuild/src/compiled/00/74/CCreatureGeneratorDef_Transfer_0074c5f0.cpp -- the draft's template: local CPersistContext with
  inline `Transfer(name,T&)` switch 2/3 using `CPersistTraits<T>().TransferBinaryIn(...)` temp, vector overload 2/default. No pragmas.
- rebuild/src/compiled/00/47/CHeroStatsDef_Transfer_00479cba.cpp -- `#pragma optimize("s", on)`, local models, one-arg Transfer.
- rebuild/src/compiled/00/46/CArmourDef_Transfer_004615fb.cpp -- `#pragma optimize("s", on)`, `#define FABLE_RETAIL_DEF_CLASS_BASE`,
  `#include "engine/CParentDefClassBase.h"`, `context.TransferMap(member)` (out-of-line there).
- rebuild/src/compiled/00/6d/CDefClassBase_GetSubDefCHeroExperienceDef_006daa90.cpp -- same TU; `#define FABLE_RETAIL_DEF_CLASS_BASE`,
  engine/CSubDefClassBase.h, engine/CDefPointer.h.
- Shrink-wrap precedents (comment text): 00/84/CCreatureActionDrink_FinishAction_00847c90.cpp (goto shared tail),
  00/54/NUISystemCTextSlider_Initialise_00549230.cpp ("flattened push_back shrink-wraps the ebp push").

## 7. Old draft's likely model errors
- Only diff (693v692) is ebp saved in the prologue instead of around transfers 8..13. Ideas not yet tried (NOTES tried only the
  CCreatureGenerator template and ordering): (a) PDB-true spellings -- TransferMap(name, map, default) 3-arg with a real default
  (GetDefaultVal / static), TransferVector(name, vec), Transfer<float>(name, f) -- the extra (dead) default-value argument and real
  std::map/std::vector types change c2's flow graph; (b) the float load traits temp: retail homes it in the dead arg slot (the
  draft's `CPersistTraits<T>()` temp reproduces that already); (c) visible GFSerialise* bodies (Q24); (d) the second +0x9c
  transfer is a real source line -- keep it.
- Wrong callee spellings / pointer-vs-reference parameter types (all GFSerialise* take refs; CPersistTraits<float>::TransferBinaryIn
  takes (CMemoryDataInputStream&, float&)); local structs instead of shared headers; header comment "no engine header" stale.
- Control flow and member order in the draft are already right (vectors 2/default, maps+floats 2/3).

## 8. Scout re-check (2026-11-12) -- facts verified again + new evidence
- Re-verified: xdis 0x2c4 bytes (ret 4, no frame, no SEH, push ebp at 0x6D9064 / pop ebp at 0x6D91A3, two tail epilogues);
  xref.py: sole ref = vtable slot .rdata 0x01243954; callee row files all exist at the paths in section 3; the repo header
  rebuild/include/engine/CHeroExperienceDef.h still has NO Transfer opt-in -- the opt-in FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER
  exists only in the overlay $W/hdr/engine/CHeroExperienceDef.h (plus FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY in
  $W/hdr/engine/CPersistContext.h and FABLE_RETAIL_CPERSISTTRAITS_FLOAT_BINARYOUT in $W/hdr/engine/CPersistTraits.h); no landed row
  uses FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY yet (grep rebuild/src/compiled: 0 hits).
- NEW, source ORDER proven by a third source: refs/transfer_field_orders.json `defs.CHeroExperienceDef` (Ego_r Transfer 0x58a900,
  "fable_defs_order_match": "exact") and ghidra_out/def_schema.json (FableWin 01ef6b20) both list 14 transfers in exactly retail's
  order, StatIncreasePerHitType TWICE (8th and 13th). So the double transfer is a real source line in every build -- keep it.
- NEW, the transfer KEY of +0x64 is "ExperienceMultiplierPerUnitComboMultiplierParam" (both json sources) while the Ego_r MEMBER is
  ExperienceMultiplierParamPerUnitComboMultiplier -- irrelevant to bytes (retail CRC key is "" 0x0122D70E everywhere) but use the
  member name for the member and, if a named key is written, that key string.
- NEW, Ego_r neighbourhood (egor_pdb_names.tsv 0x58a010..0x58ab00): Transfer sits after the CTCHeroExperience members
  (SetAsHit .. AddSuccessfulStrikeWithHitParams 0x58a860) and right before GetSubDef<CHeroExperienceDef> 0x58ab00 -- same order as
  retail (CTCHeroExperience::* 0x6D80E0..0x6D8E30, Transfer 0x6D8F10, GetSubDef 0x6DAA90). TU = CTCHeroExperience's .cpp, Transfer
  is the LAST non-template function of the TU. Ego_r Transfer is 0x1e0 bytes (it calls out-of-line TransferMap 3-arg + TransferVector).
- Ego_r instantiates GetDefaultVal<CComboMultiplierData> (0x58cdd0) and GetDefaultVal<CHeroStatIncreaseData> (0x58d0d0) but NO
  GetDefaultVal<CDefString>: the dev source passed `GetDefaultVal<V>()` defaults for the two struct maps only (CDefString maps got some
  other default). Retail shows no default construction at all (no sub esp, no ctor/dtor), so the retail binary path never builds it --
  pass-2 probes with V() temporaries grew the body (931/1043); a null/unused reference default is byte-neutral.
- Siblings in the same TU (no pragmas, no // Flags): 00/6d/CTCHeroExperience_OnSerialise_006d8e30.cpp, CTCHeroExperience_Copy_006d8ca0.cpp,
  CDefClassBase_GetSubDefCHeroExperienceDef_006daa90.cpp (`#define FABLE_RETAIL_DEF_CLASS_BASE`, engine/CSubDefClassBase.h,
  engine/CDefPointer.h), CPersistTraitsCHeroStatIncreaseData_TransferBinaryOut_006db0f0.cpp (CDataOutputStream via engine header).
  So the TU was plain /O2, no optimize pragma; the residue is not a flag.
- NAME_FIXES candidates (unchanged, PDB-proven): 0x006DD1C0 landed leaf "ReadFromStream" (file CHeroExperienceDef_ReadFromStream_006dd1c0.cpp)
  = ??$GFSerialiseMapBinaryIn@VCHeroStatIncreaseData@@W4EDamageAttribute@@U?$less@W4EDamageAttribute@@@std@@@@YIX...;
  0x006DC450 file "..._WriteToStream_006dc450.cpp" = ??$GFSerialiseMapBinaryOut@VCHeroStatIncreaseData@@W4EDamageAttribute@@...;
  0x006DBA50 leaf is the bare "GFSerialiseMapBinaryIn" (needs the full <CDefString,long,less<long>> leaf).
- Status: best source $W/differ/006d8f10.cpp = DIFFER(693v692), residue ONLY ebp save placement (prologue vs retail's shrink-wrap
  around transfers 8..13). ~165 probes over 3 author passes (NOTES.md, ATTEMPTS.log) exhausted flags, pragmas, compiler builds, EH,
  inline layering, body spellings and callee visibility. Untested levers (from NOTES pass 3): c2 allocation order via the
  combo-map temps held in ebx inside the ebp range; preceding the function with the TU's real earlier content (CTCHeroExperience
  members 0x6D80E0..0x6D8E30 are landed -- compile them into the same probe TU before Transfer, in retail order).

