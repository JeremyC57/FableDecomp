## 0x00BDF100

# DOSSIER 0x00BDF100 (610 bytes)

## 1. Retail shape (xdis 00bdf100 0x272)
- __thiscall, plain `ret` (two exits 0xBDF358 / 0xBDF361), no args. No frame pointer (ebp = `this`; /Oy on). No SEH. Speed TU
  (`nop` / `mov edi,edi` loop-head alignment at 0xBDF15F / 0xBDF1FE).
- `sub esp,10h` frame: [esp+14h] (after push ebx/ebp) spills `this`; inside the inlined overflow [esp+14h]=1 (fill_len) and
  [esp+18h]=old_size (the `(max)` reference pair, `lea eax,[esp+14h] ; jb ; lea eax,[esp+18h]`); the out-of-line call's
  __false_type temporary is the byte at [esp+1Bh] (address passed, never stored = the shared header's `__false_type() { }` tag).
- ebx = 0 for the whole function (used as null for compares/stores); inside the inlined overflow ebx becomes new_start and is
  re-zeroed (`xor ebx,ebx` 0xBDF326) afterwards. esi = &Children[i] cursor, edi = countdown 4 then the collection pointer.
- Function-local static: guard 0x0143B984 bit0, storage 0x0143B978 (start/finish/eos zeroed via ebx), atexit(0x0122ADB0).
- Order: static init; `sphere_list.resize(0)` (push ebx -> 0x00BE0140, ret 4); MaxFadeDistance=0; PrimitiveTypeMask=0;
  child loop (4, test Radius>0 via `fcomp 0.0f [0x0122DEDC]; test ah,41h; jne skip`): push_back(child->BoundingSphere) FIRST
  (inline fast path; overflow = OUT-OF-LINE call 0x00BDFE70 with (finish, &x, &tag, 1, true)), then fade max (re-reads
  Children[i]), then mask |=. Collection loop (Next chain): Radius>0 test, fade max, mask |=, THEN push_back whose overflow is
  INLINED (incl. inlined __uninitialized_copy head loop, null-tested placement copies, no fill_n / tail branch = constants
  folded, `if(start) free(start)` = _M_clear, then _M_set). Final: `size()` (cached finish in ecx) unsigned `> 0` ->
  BoundingSphere.BuildFromSubSpheres(start, size) else BoundingSphere.Radius = 0.
- Copy-construct register patterns: child fast path edx,edx,edx,ecx; collection fast path edx/ecx alternating; head-copy loop
  edx only; the x construct after the loop ecx/edx alternating.

## 2. Identity
- Ego_r.pdb (egor_pdb_names.tsv 00a92600) and FableWin (_fablewin_api.tsv 01846881 / 02e39420, FableWin.exe.signatures.tsv):
  `?CalcBoundingSphereAndFadeDistanceFromChildren@CQuadTreeElement@CLocalDetailCacheMap@@QAEXXZ`
  = `void CLocalDetailCacheMap::CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren()` (public, non-const, void).
- Static: PDB `...::`2'::`dynamic atexit destructor for 'sphere_list'' (Ego_r 0101dd00, FableWin 04012220) -> retail 0x0122ADB0
  (landed rebuild/src/compiled/01/22/global_E2_0122adb0.cpp: `static std::vector<C3DBoundingSphere> sphere_list;`).
- Manifest label module `CQuadTreeElement` is right but NESTED class missing: the class is CLocalDetailCacheMap::CQuadTreeElement.
  Old draft models a top-level `struct CQuadTreeElement` -> wrong mangled name.
- Callers: xref.py reports 0 (no landed/direct caller found). Not virtual (no vtable slot).

## 3. Callees
| retail | landed file | landed name / PDB | draft spelling |
|---|---|---|---|
| 0x004012BC | 00/40/global_atexit_004012bc.cpp | atexit | ok (compiler-emitted) |
| 0x0122ADB0 | 01/22/global_E2_0122adb0.cpp | _$E2 atexit dtor of sphere_list | ok |
| 0x00BE0140 | 00/be/STLvector_resize_00be0140.cpp | `?resize@?$vector@VC3DBoundingSphere@@V?$allocator@VC3DBoundingSphere@@@std@@@std@@QAEXI@Z` (Ego_r 00a93fc0), thiscall ret 4 | draft local CSphereVector::resize -> should be std::vector<C3DBoundingSphere>::resize |
| 0x00BDFE70 | 00/bd/stdvector_Minsertoverflow_00bdfe70.cpp | `?_M_insert_overflow@?$vector@VC3DBoundingSphere@@...@std@@IAEXPAVC3DBoundingSphere@@ABV3@ABU__false_type@2@I_N@Z` (protected), ret 14h | draft local class; std::__false_type must be std-namespaced |
| 0x00ADDBE0 | 00/ad/C3DBoundingSphere_BuildFromSubSpheres_00addbe0.cpp | `?BuildFromSubSpheres@C3DBoundingSphere@@QAEXPBV1@J@Z` (Ego_r 00bfa6b0) = void BuildFromSubSpheres(const C3DBoundingSphere*, long), thiscall ret 8 | WRONG: draft `SetFromSpheres(const C3DBoundingSphere*, unsigned int)` |
| 0x00BFEA0E / 0x00BFEA14 | 00/bf/global_malloc / global_free | malloc / free (allocator) | ok |
| (0x00BDF880 not called here) | 00/bd/std_uninitializedcopy..._00bdf880.cpp | __uninitialized_copy<C3DBoundingSphere*> `// Flags: /Gr` | retail inlines its body at the collection site only |

## 4. Members
- this = CLocalDetailCacheMap::CQuadTreeElement (struct_layouts_egor.tsv, 76 bytes, CMovableResource base vptr +0):
  ObjectCacheGroupCollections +0x04 (CObjectCacheGroupCollection*), Children[4] +0x08 (CQuadTreeElement*[4]),
  BoundingSphere +0x18 (C3DBoundingSphere; Radius +0x24), MaxFadeDistance +0x2C (float), PrimitiveTypeMask +0x30 (ulong).
  Shared header rebuild/include/engine/CLocalDetailCacheMap.h, opt-in `FABLE_RETAIL_CLOCALDETAILCACHEMAP_QUADTREE` (pulls
  CMovableResource.h + C3DBoundingSphere.h; used by landed ctor 0x00BDDF30 with FABLE_RETAIL_CMOVABLERESOURCE, FABLE_RETAIL_C3DVECTOR).
  GAPS: the members are `private:` and the class declares no CalcBoundingSphereAndFadeDistanceFromChildren -> a member
  definition in the row needs the declaration added (new opt-in in the header). CObjectCacheGroupCollection is only
  forward-declared nested (`CLocalDetailCacheMap::CObjectCacheGroupCollection`) -- no shared definition.
- CObjectCacheGroupCollection (egor, 60 bytes, vptr +0): Next +0x04, BoundingSphere +0x08 (Radius +0x14), MaxFadeDistance +0x18
  (float), PrimitiveTypeMask +0x1C (ulong). (LoadStaticContents NOTES: retail collection is 0x38 bytes, PDB less SourceObjectCount.)
- C3DBoundingSphere (engine/C3DBoundingSphere.h): C3DVector Center +0, float Radius +0xC; inline GetRadius(); opt-in
  `FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES` declares BuildFromSubSpheres. NO user copy ctor in the header although
  FableWin.pdb has `??0C3DBoundingSphere@@QAE@ABV0@@Z` (018028f7 / 01b409b0); landed 0x00BDFE70 needed a memberwise user copy
  ctor (`Center(other.Center), Radius(other.Radius)`) to get alternating registers (207v209 without).
- Static vector: std::vector<C3DBoundingSphere> from rebuild/include/stlport/fable_containers.h (default `__false_type() { }`
  tag, `(max)` by reference, allocator malloc/free with `if(p)` deallocate, `_M_insert_overflow(__false_type)` body visible unless
  FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY; `FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL` makes resize(n) declaration-only as retail
  calls it out of line). C3DBoundingSphere needs __type_traits non-POD (is_POD/has_trivial_assignment = __false_type) to pick the
  __false_type overflow.
- Draft-local classes that have shared headers: CMovableResource (engine/CMovableResource.h), C3DBoundingSphere
  (engine/C3DBoundingSphere.h, and its Center should be C3DVector from engine/C3DVector.h, not X/Y/Z floats), CQuadTreeElement
  (engine/CLocalDetailCacheMap.h), the vector (stlport/fable_containers.h std::vector), __false_type (fable_containers.h).

## 5. Visibility
- Same TU (LocalDetailCacheMap source, 0x00BDD9A0..0x00BE0140): _M_insert_overflow 0x00BDFE70, __uninitialized_copy 0x00BDF880,
  __uninitialized_fill_n 0x00BDF8C0, erase 0x00BDFCF0, _M_fill_insert 0x00BDFF50, resize 0x00BE0140 are this TU's COMDATs at
  neighbouring addresses -> their bodies were visible. Retail auto-inlined the generic overflow ONCE (collection site) with
  fill_len=1/atend=true folded and its __uninitialized_copy also expanded, but called it out of line at the child site, so an
  /Ob2-style heuristic or two different spellings. Draft hack: hand-written `_M_insert_overflow_inline` __forceinline copy
  (= not the shared body). The STLport helpers in the TU are __fastcall (siblings 0x00BDF580/0x00BDF880/0x00BDF8C0 carry
  `// Flags: /Gr`) -> this TU is likely /Gr (irrelevant to the thiscall row, relevant to any non-inlined free helper).
- Relevant existing opt-ins: FABLE_STLP_OUTOFLINE_UNINIT_COPY (noinline __fastcall __uninitialized_copy -- WRONG here, retail
  inlined it at the collection site), FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL, FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
  (would forbid the inline expansion -- avoid), FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE/NOINLINE (not wanted: push_back is
  inline at both sites).

## 6. Siblings (rebuild/src/compiled/00/bd/)
- CLocalDetailCacheMapCQuadTreeElement_CQuadTreeElement_00bddf30.cpp: only sibling using the shared header
  (`#define FABLE_RETAIL_CLOCALDETAILCACHEMAP_QUADTREE`, `FABLE_RETAIL_CMOVABLERESOURCE`, `FABLE_RETAIL_C3DVECTOR`).
- CQuadTreeElement_Update_00bdebf0.cpp / CLocalDetailCacheMapCQuadTreeElement_RenderUpdate_00bdda80.cpp: `#pragma intrinsic(sqrt)`,
  local models of CObjectCacheGroupCollection (nested in CLocalDetailCacheMap in 00bdda80).
- CLocalDetailCacheMapCObjectTypeCollectionPalette_GetIndexFromType_00bdef40.cpp: same TU inline push_back with
  `#pragma auto_inline(off)` + `__declspec(noinline)` _M_insert_overflow (out-of-line overflow idiom), `#pragma warning(disable:4291)`.
- stdvector_Minsertoverflow_00bdfe70.cpp: the overflow body; user copy ctor on C3DBoundingSphere; noinline __fastcall helpers in TU.
- No sibling has `// Flags:` except the /Gr STLport helpers (and an empty one on 00bde080).

## 7. Old draft model errors
1. Class not nested -> wrong mangled name (must be CLocalDetailCacheMap::CQuadTreeElement; collection = CLocalDetailCacheMap::CObjectCacheGroupCollection).
2. Callee misnamed: SetFromSpheres(…, unsigned) -> BuildFromSubSpheres(const C3DBoundingSphere*, long).
3. Local CSphereVector/C3DBoundingSphere{X,Y,Z,Radius}/__false_type at global scope instead of std::vector<C3DBoundingSphere> +
   C3DVector Center + std::__false_type; placement new defined locally (shared header uses <new>, null-tested).
4. Overflow inlining modelled with a hand duplicated `_M_insert_overflow_inline` with plain cursors and `len ? malloc : 0`
   spelled directly -- retail is the STLport body (allocate(len), __uninitialized_copy, `if(fill_len==1)`, `if(!atend)`,
   _M_clear, _M_set) with (max) by reference into stack slots; the residue (+27: zero in esi instead of ebx, new_start and
   collection spilled, copy cursor form) points to the non-STLport inline body / missing user copy ctor (register alternation).
5. No user copy ctor for C3DBoundingSphere (retail copy patterns alternate edx/ecx at some sites).
Control flow, loop forms, statement order and the >0 / < comparisons in the draft agree with retail.

## 0x005F6BD6

# DOSSIER 0x005F6BD6 CTCInventoryAbilities::SetAbilityAsAvailable (618 bytes)

## 1. Retail shape (xdis 005f6bd6 0x27a)
- thiscall, `ret 0Ch` (3 stack args: EHeroAbility, bool, bool). Frame pointer `push ebp; mov ebp,esp; push ecx` (one 4-byte local
  [ebp-4]) -> `#pragma optimize("y", off)`. No SEH. Size idioms (`or dword ptr [eax],-1`, `and [ebp+0Ch],0`, `leave`) -> `optimize("s", on)`.
  Address 0x...BD6 unaligned = size-optimised TU, same as neighbours (OnInitialActivate 0x005F6B47 directly before).
- Register plan: esi=this, ebx=ability (reloaded from [ebp+8] after inner-loop reuse), edi=0 early; dead `available` slot [ebp+0Ch]
  is reused as the i+1 counter and later the button counter; dead `updateRow` slot [ebp+10h] reused for the Max temporary.
- Flow: entry=&Abilities[ability] (`lea eax,[eax+ebx*8]`); wasAvailable = entry.first>=0 (`setge dl`);
  if(available) entry.first = max(0L... ) as `Max(entry.first,0)`: temp 0 in [ebp-4], `jg` keeps &entry.first -> STLport max(a,b)=a<b?b:a
  with a=temp0? -> it picks &first when first>0 else &zero: `std::max(0L, entry.first)` (zero literal temp, first operand).
  else `entry.first = -1` (or -1).
  if(Initialised +0x141) { if(available && !wasAvailable){A} else if(wasAvailable && !available){B} }  (B re-tests `available`).
  A: (1) linear scan ReceivedAbilityOrder for 0 -> store ability, skip (2); (2) nested i / j=i+1 duplicate scan, on match RAO[j]=ability;
     both loops re-read size() ((finish-start)>>2) every iteration; outer of (2) guarded once by size!=0 then do-while (VC inversion).
     (3) n = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons() INLINE
        (call 0x00686D50; call 0x00449960; mov ecx,[eax+0Ch]; call 0x00445940) -- i.e. the body of GetNumAssignableSpecialMoveButtons
        expanded here, while later uses call 0x005EDCF0 out of line.
     (4) for(button=0;button<n;++button) if(AbilityButtons.Find(button)==AbilityButtons.end()){SetAbilityToButton(ability,button,true);break;}
        Find inlined: `mov ebx,[edi+4]` (end read BEFORE LowerBound call), call 0x005F7F4C, `cmp eax,ebx; je; cmp ecx,[eax]; jge` ->
        `i==end()||key<i->first ? end() : i`; then compare against re-read [esi+170h].
     (5) count = #non-zero RAO entries; if(count > GetNumAssignableSpecialMoveButtons() [0x005EDCF0]) WaitingToGiveTutorial=true.
  B: it = AbilityButtons value search (inline, cursor eax loaded before last edx, `jmp cond` for-loop, not-found -> eax=last) ;
     if(it != end() [re-read +0x170]) AbilityButtons.erase(it) [0x005F87C0, ecx still this+0x16C];
     r = std::find(RAO.begin(), RAO.end(), ability) [0x005F8150 fastcall ecx=first edx=last, push &ability = &param slot [ebp+8]];
     if(r != finish (ebx, read before call)) RAO.erase(r) [0x005F8750].
  if(updateRow) { CurrentAssignedAbilityLevel = Max(level - N(), -N()); CycleAssignedAbilities(true); }  -- `-N()` built FIRST
     ([ebp+0Ch]), then level-N() ([ebp+10h]), `jl` picks right; identical idiom to landed OnInitialActivate 0x005F6B47 and
     CycleAssignedAbilities 0x005F64DD (`std::max(CurrentAssignedAbilityLevel,-GetNumAssignableSpecialMoveButtons())`).

## 2. Name / signature (proven, >=2 sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv) 0x00585AD8 `?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z`,
  directly after ?OnInitialActivate@CTCInventoryAbilities 0x00585A49 = retail order (0x005F6B47 then 0x005F6BD6).
- FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv) same mangled name (0x01EBBCFB, 1858 bytes debug).
- engine/CTCInventoryAbilities.h FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN already declares
  `void SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign); // 0x005F6BD6`.
- Caller: landed CThingPlayerCreature::LearnAbility 0x006AC690 (calls at 0x006AC7D0 `(ability,true,true)`, 0x006AC842 `(ability,false,true)`);
  also 0x005F715E inside OnSerialise 0x005F6E40 (not landed). Not virtual. Manifest label is CORRECT.
- Signature: public, non-const, void(EHeroAbility, bool, bool). Mangled leaf: ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z

## 3. Callees (landed row / landed name / PDB name / draft spelling)
| addr | landed file | landed name | PDB name | draft |
|---|---|---|---|---|
| 0x00686D50 | compiled/00/68/CTCBase_PeekPlayerManager_00686d50.cpp | CTCBase::PeekPlayerManager() const (protected) | ?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ | WRONG: `GetPlayerManager()` non-const returning ptr |
| 0x00449960 | compiled/00/44/CPlayerManager_GetMainPlayer_00449960.cpp | const CPlayer& CPlayerManager::GetMainPlayer() const | same | ok |
| 0x00445940 | compiled/00/44/CGamePlayerInterface_GetNumberOfAssignableSpecialMoveButtons_00445940.cpp | CGamePlayerInterface::GetNumberOfAssignableSpecialMoveButtons() const | same | WRONG: `CPadController::GetNumButtons`, member `PController` (real: CPlayer::PlayerInterface +0x0C) |
| 0x005F7F4C | compiled/00/5f/CVectorMap_LowerBound_005f7f4c.cpp | CVectorMap<long,EHeroAbility,CKeyPairCompareLess<long,EHeroAbility>>::LowerBound(const long&) | same (FableWin also ?Find@.. inline) | draft `LowerBound(const long*)` pointer arg: wrong type |
| 0x005F6423 | compiled/00/5f/CTCInventoryAbilities_SetAbilityToButton_005f6423.cpp | SetAbilityToButton(EHeroAbility,long,bool) | ?SetAbilityToButton@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J_N@Z | WRONG: `AssignAbilityToButton` |
| 0x005EDCF0 | compiled/00/5e/CTCInventoryAbilities_GetNumAssignableSpecialMoveButtons_005edcf0.cpp | GetNumAssignableSpecialMoveButtons() const | ?GetNumAssignableSpecialMoveButtons@CTCInventoryAbilities@@QBEJXZ | WRONG: `GetNumAbilitiesPerRow` |
| 0x005F87C0 | compiled/00/5f/stdvector_erase_005f87c0.cpp | std::vector<std::pair<long,EHeroAbility>>::erase(iterator) | ?erase@?$vector@U?$pair@JW4EHeroAbility@@... | draft `CAbilityButtonMap::erase` (ok semantically, it is the CArray/vector base) |
| 0x005F8150 | compiled/00/5f/vectorEWeaponClass_UmoveEWeaponClass_005f8150.cpp | **std::vector<EWeaponClass>::_Umove<EWeaponClass*>  (WRONG landed name)** | real: STLport std::find<EHeroAbility*,EHeroAbility> forwarder (fastcall, push &val, call __find 0x005F7ACC, ret 4; header fable_algorithm.h FABLE_RETAIL_STLP_FIND comment + std_find_005f7acc.cpp identity; FableWin has ??$_Find@PAW4EHeroAbility@@W41@@std@@YI...) | draft `FindAbility` free function: WRONG; -> NAME_FIXES.tsv candidate for 0x005F8150 |
| 0x005F8750 | compiled/00/5f/stdvector_erase_005f8750.cpp | std::vector<EHeroAbility>::erase(iterator) | ?erase@?$vector@W4EHeroAbility@@...@@QAEPAW4EHeroAbility@@PAW43@@Z | ok |
| 0x005F64DD | compiled/00/5f/CTCInventoryAbilities_CycleAssignedAbilities_005f64dd.cpp | CycleAssignedAbilities(bool) | ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z | ok |
- Inline value search: FableWin.pdb has `??$ValFind@W4EHeroAbility@@@?$CVectorMap@JW4EHeroAbility@@...@@QAE?AV?$_Vector_iterator...` (0x018580E0)
  -> original spelling is the member template `CVectorMap::ValFind(const V&)`. Not in engine/CVectorMap.h (grep ValFind: nothing).
  Landed GetButtonAbilityIsSetTo 0x005EDF33 matched the same loop as a FABLE_FORCEINLINE free helper (cursor declared BEFORE last,
  early `return it`, `return last`). Draft calls it `FindValue` (cursor/last order ok, but name wrong; use ValFind).

## 4. Members (retail offsets; engine/CTCInventoryAbilities.h under FABLE_RETAIL_CTCINVENTORYABILITIES, sizeof 0x1D8)
- +0x140 bool WaitingToGiveTutorial (write 1); +0x141 bool Initialised (read); +0x144 long CurrentAssignedAbilityLevel (rw);
  +0x154 std::vector<EHeroAbility> ReceivedAbilityOrder; +0x160 std::vector<std::pair<long,CDefPointer<const CHeroAbilityDef> > > Abilities
  (8-byte elems; .first is the level, -1 = unavailable); +0x16C CVectorMap<long,EHeroAbility> AbilityButtons (0x10 bytes: vector + Compare +0xC + Dirty +0xD).
  All private in the header (fine: member function).
- CPlayer::PlayerInterface (+0x0C, CGamePlayerInterface*) -- engine/CPlayer.h with FABLE_RETAIL_PLAYER (see row 0x005EDCF0).
- Opt-ins to use (copy from 0x005EDCF0 + 0x005F64DD): FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE,
  _CTCBASE_PEEKPLAYERMANAGER, _CINTELLIGENTPOINTER, _PLAYER, _CPLAYERMANAGER, _CGAMEPLAYERINTERFACE,
  _CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS, _CTCINVENTORYBASE, _CTCINVENTORYABILITIES, _CTCINVENTORYABILITIES_ASSIGN (SetAbilityAsAvailable,
  CycleAssignedAbilities), _CTCINVENTORYABILITIES_BUTTONS (SetAbilityToButton, GetNumAssignableSpecialMoveButtons; header comment says
  it was made "for row 0x005F6BD6"), FABLE_RETAIL_STLP_FIND (fable_algorithm.h: "chunk 1008b, row 0x005F6BD6": fastcall find decl),
  FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE (fable_containers.h ~l.514: out-of-line erase(iterator)), std::max in fable_containers.h l.114.
- Draft models LOCALLY (all have shared headers): CTCInventoryAbilities, CPlayer, CPlayerManager, CVectorMap (as CAbilityButtonMap, 12 bytes:
  WRONG size, real 0x10), vector<EHeroAbility>, pair vector, CPadController (fictional; real CGamePlayerInterface).

## 5. Callee visibility
- LowerBound 0x005F7F4C: engine/CVectorMap.h default = same-TU `__declspec(noinline)` body -- explains `mov ebx,[edi+4]` end() read
  before the call (compiler knows callee does not write). Do NOT define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND.
  CVectorMap::Find is a plain inline in the header; SetAbilityToButton (same TU) needed FABLE_FORCEINLINE on Find/Insert in the size TU
  -> consider FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND if plain Find goes out of line.
- GetNumAssignableSpecialMoveButtons: retail expands its body ONCE (step 3) and calls 0x005EDCF0 elsewhere -> the original probably
  wrote the chain literally in step 3 (as the draft did); keep the call form for the two other uses.
- vector erase x2, std::find: out of line COMDATs (opt-ins above). std::__find 0x005F7ACC landed (callee of 0x005F8150 only).
- Same TU, defined earlier: GetButtonAbilityIsSetTo 0x005EDF33, GetNumAssignable 0x005EDCF0, SetAbilityToButton 0x005F6423,
  CycleAssignedAbilities 0x005F64DD, OnInitialActivate 0x005F6B47; OnSerialise 0x005F6E40 follows.

## 6. Siblings to copy idioms from
- 0x005F6B47 OnInitialActivate: `#pragma optimize("y",off)` + `("s",on)`, local `Max` ref template, `level = Max(level - perRow, -perRow)`.
- 0x005F64DD CycleAssignedAbilities: `// Flags: /O1 /Oy-`, shared header + opt-in set listed in section 4, std::max with -GetNum...().
- 0x005F6423 SetAbilityToButton: y-off/s-on, FABLE_FORCEINLINE Find, notes Find shape `(it==end()||key<it->first)?end():it`.
- 0x005EDF33 GetButtonAbilityIsSetTo: optimize("s"), forceinline value search (cursor before last).
- 0x005EDCF0 GetNumAssignableSpecialMoveButtons: shared-header opt-in list for the player chain.

## 7. Old draft model errors (DIFFER 588v618, 30 bytes short; NOTES: regalloc ability ebx / zero edi, level re-reads, i+1 in dead slot)
- Wrong names (GetPlayerManager, AssignAbilityToButton, GetNumAbilitiesPerRow, PController/GetNumButtons, FindAbility, FindValue) and
  local struct models (verifier rejects in strict mode). Use the shared header.
- CAbilityButtonMap 12 bytes vs real CVectorMap 0x10 (no layout effect here since it is last used member, but wrong).
- LowerBound declared out-of-TU (`SEntry* LowerBound(const long*)`) -> compiler cannot know it does not clobber memory; retail reads
  end() before the call (header's same-TU noinline body gives that). Likely part of the "re-read" residue.
- std::find modelled as a free fastcall `FindAbility(long*,long*,const EHeroAbility&)` with long* iterators; should be std::find over
  EHeroAbility* with &ability (the parameter) -- ability must stay addressable (param slot), which drives ebx reload from [ebp+8].
- AddReceivedAbility / GetNumAssignedReceivedAbilities as __forceinline invented helpers (no PDB evidence); retail shape is consistent
  with straight-line code in the body; the size TU homes i+1 in the dead `available` slot: write the dup loop as
  `for(i=0;i<size();++i) for(j=i+1;j<size();++j)` with unsigned/long index matching `jb` (unsigned compares: size_type).
- Loop compares are `jb` (unsigned): indices compare against size() as unsigned (size_t / unsigned long), button loop is signed (`jl`, long).

## 0x00A60D40

# DOSSIER 0x00A60D40 CChecksumCacheFile::Open (623 bytes)

## 1. Retail shape (xdis 00a60d40 0x27f)
- __thiscall, `ret 4` (0x00A60FAC). NO frame pointer (sub esp,0x88; push ebx,ebp,esi,edi). NO SEH prologue (no fs:0)
  although locals with dtors exist -> TU without C++ EH unwind (same as siblings; `// Flags:` empty/none).
- Frame 0x88. Slots (esp after the 4 pushes): +0x10 long `val` (shared temp of every inline 4-byte Read: version, count,
  checksum); +0x14 CFileDataInputStream stream (0x24); +0x38 CWideString name; +0x3c CDiskFileWin32 file (0x1c, Opened +0x50,
  Pathname +0x54); +0x58 CCharString temp (ReadNullTerminatedString result); +0x5c CEntry entry (0x20, Checksum +0x78);
  +0x7c CDateAndTime temp (ReadDateAndTime result).
- Registers: esi=this then (after `NeedsSaving=0`) the CDiskFileWin32 vptr 0x0122D06C (mov esi,imm at 0xA60DA4, stored at
  0xA60DAD and at the dtor 0xA60F87; RE-loaded at 0xA60F75 after the loop because rep movsd clobbers esi); ebp=&Entries (this+4,
  lea at 0xA60D5C, used for clear and for operator[] this); ebx=0 then loop counter; edi=filename then the constant 4.
- Order: Filename=filename (0x0099B7D0); inline map clear (if size: _M_erase(head->parent) 0x00A60A00; head->left=head;
  head->parent=0; head->right=head; size=0); NeedsSaving(+0x14)=0; if (!CAFile::PathExists(filename)) return.
  CDiskFileWin32 file (inline default ctor: CBaseClassNonCopyable() 0x0099A2F0, vptr, Pathname() 0x0099AED0, Opened=0 -- the
  Opened store is scheduled after Open's 3 pushes); file.Open(filename,1,2) direct call 0x0099A6A0; CFileDataInputStream
  stream(file,0x4000) 0x00994700; if (ReadSLONG()==4) { count=ReadSLONG(); if count>0: ebx=count, `dec ebx; jne` loop:
  name = stream.ReadNullTerminatedString().ToWideString() (outer hidden ret pushed FIRST, then inner) ; ~CCharString 0x0099EAE0;
  CEntry entry (CDateAndTime ctor 0x009E9AB0 at +0x5c -- built AFTER name); entry.TimeStamp = stream.ReadDateAndTime()
  (temp +0x7c, rep movsd 7); entry.Checksum = ReadULONG() (via val slot +0x10 then to +0x78); Entries[name] (0x00A617E0,
  ecx=ebp) = entry (rep movsd 8); ~CWideString(name) 0x0099B510 }. Exit: ~CFileDataInputStream 0x00994780; inline
  ~CDiskFileWin32: vptr store, Close() direct 0x0099A920, ~CWideString(Pathname) 0x0099B510, ~CBaseClassNonCopyable 0x0099A300.
  Version-mismatch / count<=0 jump straight to 0xA60F7A (esi still holds vptr there).
- Inline Read: `__int64 np=(__int64)(StreamPos+4)`, range test hi/lo vs 0 and 0x7fffffff, then chunk copy or
  ReadWithSrcChunkOverflow 0x00993CA0 -- exactly CDataInputStream.h FABLE_FORCEINLINE Read (`size<=0` test folds).
- No /O1 size idioms beyond rep movsd struct copies (normal /O2 for 7/8-dword copies).

## 2. Name / signature
- `CChecksumCacheFile::Open(const CWideString& filename)`, returns void, non-const, public.
  Leaf `?Open@CChecksumCacheFile@@QAEXABVCWideString@@@Z`.
- Source 1: ghidra_out/egor_pdb_names.tsv Ego_r 0x00C00760 (and egor_signatures.tsv line 37444); Ego_r order Save c00530,
  Close c006d0, ~ c00700, Open c00760, GetChecksum c00980 == retail order Save a60aa0, Close a60c30, ~ a60ce0, Open a60d40,
  GetChecksum a60fb0.
- Source 2: ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv + _fablewin_api.tsv (FableWin 0x033A68E0 / 0x017F99EB same
  mangled name). Source 3: landed callers CChecksumCacheFile ctor 0x00A61150 (calls Open last), CDefinitionManager::CheckDependencies
  0x009B05F0, CBankFile::BeginUpdateEntries 0x009CE8D0 all declare `void Open(const CWideString&)` @0x00A60D40.
- Manifest label is CORRECT. Not virtual (no vtable). Param name: draft "filename" (PDB param names not in tsv; fine).

## 3. Callees (all LANDED)
| addr | landed file | PDB name | draft spelling ok? |
|---|---|---|---|
| 0099B7D0 | 00/99/CWideString_operatorassign_0099b7d0.cpp | CWideString::operator=(const CWideString&) | ok |
| 00A60A00 | 00/a6/stdRbtree_Merase_00a60a00.cpp | std::_Rb_tree<CWideString,pair<const CWideString,CChecksumCacheFile::CEntry>,...>::_M_erase (STLport) | ok (via inline clear) |
| 00999230 | 00/99/CAFile_PathExists_00999230.cpp | ?PathExists@CAFile@@SI_NABVCWideString@@@Z (static __fastcall) | ok |
| 0099A2F0 / 0099A300 | 00/99/CBaseClassNonCopyable_CBaseClassNonCopyable_0099a2f0/_0099a300.cpp | ??0CBaseClassNonCopyable@@IAE@XZ (protected) / ??1...@@MAE@XZ (protected virtual) | draft made both PUBLIC |
| 0099AED0 / 0099B510 | CWideString_CWideString_0099aed0 / CWideString_Destructor_0099b510 | ??0/??1CWideString | ok |
| 0099A6A0 | 00/99/CDiskFileWin32_Open_0099a6a0.cpp | ?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z | ok |
| 0099A920 | 00/99/CDiskFileWin32_Close_0099a920.cpp | ?Close@CDiskFileWin32@@UAEXXZ | ok |
| 00994700 | CFileDataInputStream_CFileDataInputStream_00994700.cpp | ??0CFileDataInputStream@@QAE@AAVCAFile@@K@Z | DRAFT WRONG: (CAFile*, long); real (CAFile&, unsigned long) |
| 00994780 | ..._00994780.cpp | ??1CFileDataInputStream@@UAE@XZ (virtual) | ok |
| 00993CA0 | CDataInputStream_ReadWithSrcChunkOverflow_00993ca0.cpp | ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z (private) | ok |
| 00994C80 | CDataInputStream_ReadNullTerminatedString_00994c80.cpp | ?ReadNullTerminatedString@CDataInputStream@@UAE?AVCCharString@@XZ (VIRTUAL, on CDataInputStream) | DRAFT WRONG: put on fake class CFileDataInputStreamX, non-virtual |
| 00995770 | CDataInputStream_ReadDateAndTime_00995770.cpp | ?ReadDateAndTime@CDataInputStream@@QAE?AVCDateAndTime@@XZ (non-virtual) | DRAFT WRONG class (CFileDataInputStreamX) |
| 0099E480 | CCharString_ToWideString_0099e480.cpp | ?ToWideString@CCharString@@QBE?AVCWideString@@XZ | ok |
| 0099EAE0 | CCharString_CCharString_0099eae0.cpp | ??1CCharString | ok |
| 009E9AB0 | 00/9e/CDateAndTime_CDateAndTime_009e9ab0.cpp | ??0CDateAndTime@@QAE@XZ | ok |
| 00A617E0 | 00/a6/STLmap_operator_00a617e0.cpp | ??A?$map@VCWideString@@UCEntry@CChecksumCacheFile@@...@@QAEAAUCEntry@CChecksumCacheFile@@ABVCWideString@@@Z | ok |
- The `CFileDataInputStreamX` class in the draft is made up -- no such retail class; ReadNullTerminatedString/ReadDateAndTime
  belong to CDataInputStream; ReadLong is not the header spelling (header: ReadSLONG / ReadULONG / ReadType<T>).

## 4. Members
- this: CChecksumCacheFile (no shared engine header; only rebuild/include/engine/CChecksum.h, unrelated). PDB
  (struct_layouts_egor.tsv): Filename CWideString +0, Entries map<CWideString,CEntry> +4 (PDB 16 bytes), BufferSize long +20,
  NeedsSaving bool +24. RETAIL: map is 12 bytes (STLport _Rb_tree: header +4, node_count +8, compare +0xc) -> BufferSize +0x10,
  NeedsSaving +0x14 (written at 0xA60D8A). Landed siblings model it locally with `std::map<CWideString,CEntry>` (GetChecksum
  0x00A60FB0 builds a local STLport map; Close/ctor use local structs).
- CEntry (struct_layouts_egor.tsv "CEntry 32"): **TimeStamp** CDateAndTime +0, **Checksum ulong** +0x1c. Draft says `Time` and
  `long Checksum` -> rename TimeStamp / unsigned long (GetChecksum 0x00A60FB0 already uses TimeStamp / unsigned long).
- CDiskFileWin32 (engine/CDiskFileWin32.h, opt-in FABLE_RETAIL_CDISKFILEWIN32 + FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR for the
  inline `CDiskFileWin32(){Opened=false;}`; base CAFile via FABLE_RETAIL_CAFILE): AccessType +4, Position +8, Length +0xc,
  WinFileHandle +0x10, Opened +0x14, Pathname +0x18; vtable 0x0122D06C; inline virtual dtor `{Close();}`.
- CDataInputStream (engine/CDataInputStream.h, FABLE_RETAIL_CDATAINPUTSTREAM): StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc,
  SourceChunkStreamPos +0x10, SourceChunkBytesRemaining +0x14; inline Read / ReadType / ReadULONG / ReadSLONG; virtual
  ReadNullTerminatedString declared. **ReadDateAndTime is NOT declared** in the header (no opt-in) -> must be added locally
  (e.g. a derived-free declaration is impossible; author needs a header opt-in or a local declaration route as the landed
  ReadDateAndTime row does with a CDataInputStreamReader stand-in -- prefer a new FABLE_RETAIL_CDATAINPUTSTREAM_* opt-in? read-only
  here, so note it).
- CFileDataInputStream (engine/CFileDataInputStream.h, FABLE_RETAIL_CFILEDATAINPUTSTREAM): File +0x18, Buffer +0x1c, BufferSize +0x20.
- CDateAndTime (engine/CDateAndTime.h, no opt-in): header declares NO ctor (trivial) but retail CALLS ??0CDateAndTime 0x009E9AB0 for
  entry -> the header as-is would drop that call; landed GetChecksum models CDateAndTime locally with `CDateAndTime();`.
- CWideString / CCharString: fable_string.h with FABLE_RETAIL_CWIDESTRING / FABLE_RETAIL_CCHARSTRING (ToWideString const at line 447).
- Draft models LOCALLY although shared headers exist: CWideString, CCharString (fable_string.h), CAFile, CBaseClassNonCopyable,
  CDiskFileWin32, CDataInputStream, CFileDataInputStream, CDateAndTime (partially -- needs ctor), plus a hand map (CEntryMap)
  where stlport/fable_map.h exists.

## 5. Callee visibility
- map::clear: inline in retail (expanded at 0xA60D65..0xA60D85) -- fable_map.h default branch (line 145) is the exact STLport
  body in retail order; do NOT define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE.
- map::operator[]: called out of line (0x00A617E0) -- fable_map.h default branch declares it (line 448); do NOT define
  FABLE_RETAIL_STLP_MAP (that one inlines it).
- _M_erase 0x00A60A00 sits just below Open in the same retail neighbourhood (same TU COMDAT block 0x00A608F0..0x00A61770: _M_find,
  _M_lower_bound, _M_erase, _M_insert, insert_unique, operator[]) -- out of line, called.
- CDiskFileWin32 default ctor/dtor: inline (expanded) -> FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR; dtor inline in header.
- CDataInputStream::Read inline (FABLE_FORCEINLINE). ReadNullTerminatedString / ReadDateAndTime / ToWideString called direct.
- Same-TU earlier functions (retail order): Save 0x00A60AA0, Close 0x00A60C30, ctor 0x00A60C90, ~ 0x00A60CE0; later GetChecksum
  0x00A60FB0, ctor(filename,size) 0x00A61150 (calls Open).

## 6. Sibling landed rows
- rebuild/src/compiled/00/a6/CChecksumCacheFile_GetChecksum_00a60fb0.cpp -- `// Flags:` empty; local STLport map model + local
  CDateAndTime with ctor + CAFile; includes engine/CDiskFileWin32.h via `#define CDiskFileWin32 CDiskFileWin32Fields` trick;
  CEntry {CDateAndTime TimeStamp; unsigned long Checksum;}; class fields Filename/Entries/BufferSize/NeedsSaving.
- .../00/a6/CChecksumCacheFile_Close_00a60c30.cpp -- inline STLport clear (same body as here), local structs.
- .../00/a6/CChecksumCacheFile_Save_00a60aa0.cpp -- FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM + engine/CFileDataOutputStream.h.
- .../00/a6/CChecksumCacheFile_CChecksumCacheFile_00a61150.cpp -- ctor that calls Open; documents 12-byte retail map.
- .../00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp (`// Flags: none`) -- best idiom donor: PathExists, CDiskFileWin32
  default ctor, `CFileDataInputStream stream(*disk_file, 0x4000)`, ReadULONG; defines FABLE_RETAIL_CCHARSTRING, _CWIDESTRING,
  _CAFILE, _CDISKFILEWIN32, _CDISKFILEWIN32_DEFAULTCTOR, _CDATAINPUTSTREAM, _CFILEDATAINPUTSTREAM (+ includes
  engine/CDiskFileWin32.h, engine/CFileDataInputStream.h). Note: no frame pointer there either.
- .../00/6f/NAnimationEventsCManager_LoadBinary_006f9f90.cpp -- inline ~CDiskFileWin32 + inline ReadSLONG helpers.

## 7. Old draft model errors
- Fake class CFileDataInputStreamX holding ReadNullTerminatedString (really VIRTUAL CDataInputStream member) and ReadDateAndTime.
- CDiskFileWin32 derives directly from CBaseClassNonCopyable with only 2 virtuals; real base is CAFile (full vtable); ctor/dtor of
  CBaseClassNonCopyable are protected in retail. The vptr-register residue (vptr 0x0122D06C in esi, constant 4 in edi) is the
  open problem: retail treats the vptr as a CSE'd constant across inline ctor+dtor. First try with the real shared headers
  (CAFile base + DEFAULTCTOR) -- GOTCHAS Q26: register-only residues often vanish once callee identities/headers are right.
- CFileDataInputStream ctor param types wrong (CAFile*, long) vs (CAFile&, unsigned long): pass `file` by reference.
- ReadLong() returning long for both version/count and checksum: Checksum is unsigned long -> ReadULONG; version/count: the
  `count<=0` test (jle) is signed -> ReadSLONG (or long). Version compare `== 4`.
- CEntry member spelled Time (PDB TimeStamp), Checksum typed long (PDB ulong).
- Hand map struct CEntryMap with `long colour` node (STLport color is bool) -- use stlport/fable_map.h std::map (default
  branch: clear inline, operator[] out of line) with the retail 12-byte tree.
- Control flow/loop otherwise matches (count-down loop from `for(i=0;i<count;++i)`, entry after name, single val temp slot).
- Retail stack: CCharString temp (+0x58) sits ABOVE the file (+0x3c..0x58) and below entry; draft got it below the file.

## 0x0091DDD0

# DOSSIER 0x0091DDD0 CAIStateGroup_HoverAllAroundTest::Update (626 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 0091ddd0 0x282)
- __thiscall, plain `ret` (0x0091E041), virtual. No SEH frame (no fs:0) even though a CIntelligentPointer local with a dtor exists -> no EH unwind needed / TU not EH-tracking here (same as neighbours).
- Prologue `push ebp; mov ebp,esp; and esp,-8; sub esp,0x8C; push ebp; push esi; push edi` = VC7.1 8-ALIGNED frame (not /Oy-: neighbours 0x0091E050 / 0x0091C3D0 have no frame pointer). The draft attributes it to a double local; landed 0x007AA6B0 says the alignment came "for this body as written (no double local needed)", 0x007AF490 attributes it to a temporary. Epilogue `pop edi/esi/ebp; mov esp,ebp; pop ebp; ret`.
- esi = this all along; ebp is reused as a scratch register (player item) inside the body.
- Constants: 0.0f @0x0122DEDC, 2^32 (double) @0x012387C8 (unsigned fixup), 65536.0 @0x012387D8, 1/65536 @0x012387C0, (double)6.2831855f @0x0124F2B8 (CTools 0x00875120 spells it `(double)TWO_PI` with `static const float TWO_PI = 6.28318548f`), 10.0f pushed as imm 0x41200000.

## 2. Identity
- Vtable 0x0128A2C0 (CAIStateGroup_HoverAllAroundTest) slot 12 = 0x0091DDD0 (rebuild/manifest/rtti-vtables.tsv); slot 12 in CAIStateGroupBase is `virtual void Update() = 0` (_purecall; CAIStateGroupBase.h header comment). Only xref is the vtable (xref.py: abs .rdata 0x0128A2F0 = 0x0128A2C0+0x30).
- Name: ?Update@CAIStateGroup_HoverAllAroundTest@@UAEXXZ -- Ego_r.pdb (egor_pdb_names.tsv 0x009415D0) + FableWin.pdb (fablewin_pdb_names.tsv 0x02C0DA90, 602 bytes, naming_stragglers signatures). Manifest label CORRECT. `void Update()`, public virtual (UAE), no params.
- Class: no engine/CAIStateGroup_HoverAllAroundTest.h and no struct_layouts_egor row. FableWin has ??0/??1/Alloc/GetName for it. The function touches no member of its own (only PBrain +4), so derive it from CAIStateGroupBase (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS). The ctor 0x00900CD5 is slot 0 neighbour of the vtable.

## 3. Callees (landed file / landed name / PDB name; X = the draft spelling disagrees)
| addr | landed row | PDB / header spelling | draft |
|---|---|---|---|
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ `CWorld& GetWorld() const` (protected) | X `CWorld* GetPWorld()` |
| 0x0049D9D0 | 00/49/CWorld_PeekMyPlayer_0049d9d0.cpp (body `mov ecx,[ecx+0Ch]; jmp 0x00449970` = PlayerManager->GetMainPlayer) | ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ `const CPlayer* PeekMyPlayer() const` (CWorld.h line 267) | X `CPlayerManager* GetPlayerManager()` |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ `CThingPlayerCreature* GetPControlledCreature() const` (CPlayer.h line 174) | X `CThing* GetMainPlayerCreature()` on CPlayerManager |
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | ??0CBaseIntelligentPointer@@QAE@XZ | ok |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z | X takes CThing* |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@..@@QBEPAVCBaseObject@@XZ | X returns CThing* |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ??1CBaseIntelligentPointer@@UAE@XZ (virtual) | X non-virtual in draft |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | `const C3DVector& CThing::GetPos() const` (CThing.h:411) | X `GetPosition` |
| 0x00A14440 | 00/a1/C3DVector_AccurateNormalise_00a14440.cpp | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ `C3DVector& AccurateNormalise()` (C3DVector.h:346, FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE) | X `void Normalise()` (Normalise is the different table-driven inline) |
| 0x008FCFD0 | 00/8f/CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp (legacy free-fn stub) | ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ `unsigned long* GetPWorldSeed() const` (CAIStateGroupBase.h:75, always under _MEMBERS) | X returns CRandomSeed* |
| 0x00497890 | 00/49/global_GFROR13_00497890.cpp | ?GFROR13@@YIJK@Z `long __fastcall GFROR13(unsigned long)` (engine/GFROR13.h / GFRandom.h) | X returns unsigned long |
| 0x00BFEB28 | 00/bf/global_CIfmod_00bfeb28.cpp (import thunk) | _CIfmod = `#pragma intrinsic(fmod)` | ok |
| 0x004978A0 | 00/49/global_GFFloatRandom_004978a0.cpp | ?GFFloatRandom@@YIMMPAK@Z `float __fastcall GFFloatRandom(float, unsigned long*)` (engine/GFFloatRandom.h) | X member of CRandomSeed |
| 0x008FCFF0 | 00/8f/CAIStateGroupBase_PeekWorldMap_008fcff0.cpp (legacy stub) | ?PeekWorldMap@CAIStateGroupBase@@IBEABVCWorldMap@@XZ `const CWorldMap& PeekWorldMap() const` (CAIStateGroupBase.h:156, needs _AIACTIONS + _SPEECH) | X `CWorldMap*`, non-const |
| 0x0063A300 | 00/63/CWorldMap_GetGroundSizeZAt_0063a300.cpp | `float CWorldMap::GetGroundSizeZAt(const C3DVector&) const` (CWorldMap.h:108) | X non-const |
| 0x0097EC00 | 00/97/CAIStateGroupBase_AIActionMoveToPos_0097ec00.cpp | `EAIActionReturn AIAction_MoveToPos(EAIActionRepeat,C3DVector,bool&,EMovementType,bool,bool,const CThing*,bool,float)` (CAIStateGroupBase.h:80, FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS) | X long/long types; args right |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ (public, header line 189) | ok |
Note: GetWorld + PeekWorldMap together are declared only under `_AIACTIONS && _SPEECH` (lines 148-156); GetWorld alone under `_GOTOBED_WORLD` (line 137). Need _MEALACTIONS for AIAction_MoveToPos.
Call sequence: GetWorld() -> PeekMyPlayer() -> GetPControlledCreature() -> SetPItem: i.e. `CIntelligentPointer<CThing> player(GetWorld().PeekMyPlayer()->GetPControlledCreature());` (vftable 0x01238C6C = ??_7?$CIntelligentPointer@VCThing@@@@6B@, rtti-vtables.tsv; inline ctor in CIntelligentPointer.h `CIntelligentPointer(T*){SetPointer}` without _CTOR_OUTOFLINE). Sibling 0x00929820 spells the same chain.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- CAIStateGroupBase.h.
- PBrain+0x18 CAIBrain::Creature (CThingAICreature*, struct_layouts_egor) -- engine/CAIBrain.h; inline accessors under FABLE_RETAIL_CAIBRAIN_GETCREATURE + FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE (`CThingAICreature& GetCreature() const`, both PDBs inline). Draft calls it PCreature / CThingCreatureBase* (X).
- thing+0x91 bit0 = CThing::Dead (bitfield, CThing.h:88; `bool IsDead() const` line 280). thing+0x60 -> +0xC position inside GetPos (out of line).
- CIntelligentPointer local at esp+0x18 (vptr, PData) -- engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER, packed).
- Locally modelled in the draft but HAVE shared headers: C3DVector, CThing, CThingCreatureBase, CBaseIntelligentPointer/CIntelligentPointer, CPlayerManager, CWorld, CWorldMap, CAIBrain, CAIStateGroupBase. (CRandomSeed is invented -- seed is `unsigned long*`.)

## 5. Visibility of callee bodies
- Inline in retail (bodies must be visible): CIntelligentPointer<CThing>(T*) ctor, Get(), the IS-ALIVE test (two GetPItem calls per test, same shape as landed 0x0091DDA0 IsThingDead_0091dda0 right before this row: `if (p.Get()!=0){ if(!p.Get()->IsDead()) ...}`), CAIStateGroupBase::GetCreature / CAIBrain::GetCreature, C3DVector ctor/operator-/operator*/operator+.
- GFFloatRandom: retail INLINES the first call (0x0091DEAF..0x0091DEF2: GetPWorldSeed, `imul 24A1h; add 24DFh; mov [edi],ecx; call GFROR13; mov [edi],eax`, unsigned fild fixup, `fmod(seed, 65536.0) * (1/65536)` -> float) = GFFloatRandom(1.0f, seed) with range*65536 and the `>0.0001` test constant-folded; but CALLS 0x004978A0 for the second, GFFloatRandom(10.0f, GetPWorldSeed()). Precedents for a row-local inline twin: 0x00934870 CAIStateGroup_MinionKidGroupTag::Update (byte-identical inline sequence at 0x00934883) and 0x00A146C0 C3DVector::GetRandomPointWithinUnitSphere. Open question for the author: why one inline and one call (two different spellings? inline budget?). engine/GFRandom.h only has GFRandom (integer); engine/GFFloatRandom.h is declaration only.
- Out of line in retail: everything in the callee table.

## 6. Sibling rows (same TU neighbourhood 0x0091C...-0x0091E...)
- 00/91/CAIStateGroupHoverAttack_Update_0091e050.cpp (RELOCATION_MATCH; next function): local class models, `#define FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h, engine/C3DVector.h, inline `IsAlive(p){return p.Get()!=0 && !p.Get()->IsDead();}`, GetCreature() inline via local CAIBrain, GetWorld() const & protected.
- 00/91/global_IsThingDead0091dda0_0091dda0.cpp: engine/CThing.h + FABLE_RETAIL_CINTELLIGENTPOINTER, nested-if liveness form.
- 00/91/CAIStateGroupHoverMovementBase_AIActionMoveToPosAndFacePos_0091d7f0.cpp, ..._0091d320, ..._0091d510 (HoverMovementBase TU).
- 00/91/CAIStateGroupNymphFloatAround_Update_0091c3d0.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CThing.h, CVectorMap.h, FABLE_RETAIL_CINTELLIGENTPOINTER, `#pragma pack(push,1)` locals; planar direction with Z = 0.
- 00/93/CAIStateGroupMinionKidGroupTag_Update_00934870.cpp: inline GFFloatRandom twin, `#pragma intrinsic(fmod)`.
- 00/87/CTools_IsPosInViewingFrustrum_00875120.cpp: `cos(x * (double)TWO_PI)` intrinsic with `static const float TWO_PI = 6.28318548f`.
None of these neighbours use a frame pointer; no `optimize` pragma seen.

## 7. Decoded body and the draft's model errors
Retail order:
1. `CIntelligentPointer<CThing> player(GetWorld().PeekMyPlayer()->GetPControlledCreature());`
2. `C3DVector start = GetCreature().GetPos();` (copied to esp+0x44/48/4C)
3. Loop top 0x0091DE30: alive test on player (two GetPItem) -> exit to dtor WITHOUT Terminate.
4. Order of loads: creature (edi = PBrain->Creature) first, then player.Get() (ebp), then GetPos(creature) THEN GetPos(player). dir = (them.X-me.X, them.Y-me.Y, 0.0f) -- Y difference computed first (ctor args right-to-left); `dir.AccurateNormalise()` (out of line).
5. perp = C3DVector(dir.Y, -dir.X, 0.0f) (dir.Y copied as an integer move edx, -X via fchs).
6. frac = inline GFFloatRandom(1.0f, GetPWorldSeed()) -> float spill [0x20]; dist = GFFloatRandom(10.0f, GetPWorldSeed()) kept in st(1); angle = frac * (double)TWO_PI (double, stays in st0).
7. fcos FIRST then fsin (no fsincos). x87 decode: cos-term components computed Z,Y,X: (0.0f*c)*d, (c*perp.Y)*d, (c*perp.X)*d (two temporaries: `perp*c` then `*d`); sin-term = up(0,0,1): Z = s*d (the *1 folded), X=Y=(0.0f*s)*d shared. Sums: (start + up*s*d) componentwise Z,Y,X, then + (perp*c*d). I.e. consistent with `target = start + up * (float)sin(angle) * dist + perp * (float)cos(angle) * dist` with right-to-left operand evaluation (cos operand evaluated first) -- a vertical circle in the perpendicular plane, NOT the draft's 2D rotation `x*c - y*s` (draft model error #1, the main cause of 509v626).
8. ground = PeekWorldMap().GetGroundSizeZAt(target); `if (target.Z < ground) target.Z = ground;` (fcomp; test ah,5; jp).
9. `if (AIAction_MoveToPos(1, target, moving, 3, true, true, 0, true, 0.0f) == 2) continue/loop; Terminate();` then fall to dtor. Draft arg order is right; draft types (long, CRandomSeed) wrong.
Draft errors: wrong getter chain (GetPWorld/GetPlayerManager/GetMainPlayerCreature vs GetWorld/PeekMyPlayer/GetPControlledCreature); Normalise vs AccurateNormalise; GFFloatRandom(1.0f) fraction written as GFRandom+fmod by hand (ok-ish but should be the GFFloatRandom inline); rotation formula wrong; sin/cos as float locals (retail keeps them in x87, fcos before fsin); all classes local instead of shared headers; CRandomSeed invented; GetCreature typed CThingCreatureBase*.

## 0x008DB810

# DOSSIER 0x008DB810 -- NCreatureMode::CModeDragonFireBeam::UpdateBeam (630 bytes)

## 1. Retail shape (xdis 008db810 0x286)
- __thiscall, no args, plain `ret` (0x008DBA85). No frame pointer (`sub esp,6Ch ; push edi` -> /Oy on), no SEH, no EH state.
- this in edi for the whole body; esi = def pointer (loaded [esp+8] after GetSubDef), later reused for the definition manager result.
- `push esi` happens AFTER the BeamActive test (shrink-wrapped); early-out jumps to `pop edi`.
- Frame (esp offsets after both pushes): CDefPointer def +08 (zeroed `mov [esp+0Ch],0` after the lea/push), CCharString temp +0C
  (also reused for the 0x41 interface key), const CGameCameraManager* +10, mouth C3DVector +14, beam end +20, hitPos +2C,
  middle +38, x87 scratch +40..+48, hitNormal +50 (also scratch for the middle Z sum), CRightHandedSet orientation +5C.
- Release at 0x008DBA74: `dec [esi+4] ; jne ; call [vtbl+4]` with NO null test (VC drops it after the def deref -- same as landed
  0x0094FFF0 notes). Reached both when the frame test fails and at the end.
- Constants: 0x0122DEE0 = -1.0f, 0x01252370 = 1000.0f, 0x0122F59C = 0.5f. Strings: 0x01285B4C "DRAGON_MOUTH_DUMMY",
  0x01285B84 "DRAGON_FIREBEAM_EXPLOSION", 0x01285B60 "DRAGON_FIREBEAM_EXPLOSION_INVISIBLE".

## 2. Identity
- Name PROVEN (2 sources): Ego_r egor_pdb_names.tsv 0x0081CD90 and FableWin _fablewin_api.tsv 0x017B714A both
  `?UpdateBeam@CModeDragonFireBeam@NCreatureMode@@AAEXXZ` -> private, void UpdateBeam(), non-const. Manifest label correct.
- Caller: 0x008E07BB inside landed CModeDragonFireBeam::FrameUpdate rebuild/src/compiled/00/8e/NCreatureModeCModeDragonFireBeam_FrameUpdate_008e0780.cpp
  (models the class as `struct CModeDragonFireBeam : CModeDragonHover`, vtable 0x01285900).
- Class layout (struct_layouts_egor.tsv, size 148): Timer +0x80 long, PBeamWeaponEffect +0x84 CIntelligentPointer<CThing>,
  LastExplodeFrame +0x8C long, BeamActive +0x90 bool. Base CTCCreatureModeManager::CModeBase: ParentThing +0x04 (CThing&).

## 3. Callees (address / landed row / PDB name; DRAFT spelling disagreements marked !!)
- 0x008E3300 CDefClassBase::GetSubDef<CJackDragonDef> (rebuild/src/compiled/00/8e/CDefClassBase_GetSubDefCJackDragonDef_008e3300.cpp;
  Ego_r `??$GetSubDef@VCJackDragonDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCJackDragonDef@@@@@Z`, const, takes CDefPointer<const CJackDragonDef>*).
  !! draft: `CDefClassBase::GetDragonDef(CDragonDefPointer*)`, invented CDragonDef. this = ParentThing.PBaseDef object (`mov ecx,[ecx+70h]`, no addref) -> ParentThing.PeekBaseDef().GetSubDef(&def).
- 0x0049D870 CWorld::GetFrame, static __fastcall long (CWorld_GetFrame_0049d870.cpp; `?GetFrame@CWorld@@SIJXZ`; CWorld.h:142). !! draft `GetWorldFrame`.
- 0x0099EBF0 / 0x0099EAE0 CCharString(const char*, long=-1) / ~CCharString (landed). Use fable_string.h + FABLE_RETAIL_CCHARSTRING (draft has local CCharString).
- 0x004C08C0 NGraphicAppearance::CTCGraphicAppearance::GetDummyObject -- NOT landed. PDB `?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z`:
  const, references (draft: non-const, pointers). Shared decl NGraphicAppearance_CTCGraphicAppearance.h:215 (FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT).
- 0x0073A680 CTCCreatureModeManager::CModeBase::GetWorld() const -> CWorld& (`?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ` Ego_r 0x00751030;
  CTCCreatureModeManager.h:37 under FABLE_RETAIL_CMODEBASE_VIRTUALS / _PARENTTHING). Folded body ([0x013B8A1C]+0x24); landed file is
  misnamed CViewBase_GetDisplayEngine_0073a680.cpp (header comment even says @0x004c7aa0) -- NAME_FIXES candidate. !! draft returns CWorld* with invented P34.
- vtable call `[eax+28h]` on CWorld::PCurrentGameCamera (+0x34): CAIGameCameraBase slot 10 `virtual bool GetCameraManager(const CGameCameraManager**) const`
  (CAIGameCameraBase.h:36, FABLE_RETAIL_CAIGAMECAMERABASE). Reached via `GetWorld().PeekGameCamera()` (CWorld.h ~line 213, FableWin ?PeekGameCamera@CWorld@@QBEPBVCAIGameCameraBase@@XZ).
  !! draft: CToolsSource::GetToolsHolder / CToolsHolder (invented).
- 0x00875550 NCameraModes::CTools::GetLandscapePointIntersectionFromTo (CTools_GetLandscapePointIntersectionFromTo_00875550.cpp;
  Ego_r `?GetLandscapePointIntersectionFromTo@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1_N@Z` = const; (from, to, hitPos&, hitNormal&, bool)).
  this = &manager->CamTools (CGameCameraManager +0x78, `GetCameraTools() const`, CGameCameraManager.h:220 under FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED).
  NOTE: NCameraModes_CTools.h does NOT declare this method -- author must declare it (const) locally or in the TU.
- 0x0044C6B0 CGameDefinitionManager::Get() static __fastcall -> CGameDefinitionManager& (`?Get@CGameDefinitionManager@@SIAAV1@XZ`).
- 0x009AD410 CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) CONST (`QBEJABVCCharString@@@Z`). !! draft non-const on CGameDefinitionManager.
- 0x0080C460 CTCDExplosion::Create(long, const C3DVector&) static __fastcall CThing* (`?Create@CTCDExplosion@@SIPAVCThing@@JABVC3DVector@@@Z`; CTCDExplosion.h:31).
  !! draft `CreateExplosionObject(long, const C3DVector*)`.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (landed, engine/CVectorMap.h noinline body) -- via CThing::GetTC.
  !! draft local CInterfaceMap::LowerBound(const long*).
- 0x0080CBF0 CTCDExplosion::SetThingAsImmune(CThing&) (`?SetThingAsImmune@CTCDExplosion@@QAEXAAVCThing@@@Z`; CTCDExplosion.h:33). Interface 0x41 = TCI_EXPLOSION (ETCInterfaceType.h:70).
  !! draft `CTCDependents::AddDependentThing(CThing*)`.
- [vtbl+4] def Delete (CDefPointer release, slot 1).

## 4. Members
- this+0x90 BeamActive, +0x8C LastExplodeFrame, +0x04 ParentThing: no shared CModeDragonFireBeam header; CModeBase in engine/CTCCreatureModeManager.h
  (FABLE_RETAIL_CMODEBASE_PARENTTHING: `CThing& ParentThing`, GetParentThing()). CModeDragonBase.h exists (FABLE_RETAIL_CMODEDRAGONBASE_VSLOTS).
- CThing (engine/CThing.h): UsingComponents bitset +0x20 (HasTC test `test byte [eax+28h],2` = bit 65), Components +0x44 (line 66),
  PTCGraphicAppearance +0x64 (line 71, GetTCGraphicAppearance()), PBaseDef CDefPointer<const CThingBaseDef> +0x70 (line 74, PeekBaseDef()).
  GetTC<T>(id, T**) expanded form under FABLE_RETAIL_CTHING_GETTC_EXPANDED (CThing.h:340) matches retail's key store / end-or-greater form.
- CJackDragonDef +0x5C = FramesBetweenFireBeamUpdate (long) (struct_layouts_egor.tsv; header engine/CJackDragonDef.h). Draft calls it CDragonDef::BeamExplodeInterval -- wrong.
- CDefPointer (engine/CDefPointer.h, FABLE_RETAIL_DEF_CLASS_BASE): default Release (null test, dropped by VC) fits; refcount +4, Delete slot 1.
- CRightHandedSet PDB: Up +0x00, Forward +0x0C (engine/CRightHandedSet.h). Retail scales orientation+0 -> it is `Up`, not Forward.
- CWorld +0x34 PCurrentGameCamera (CWorld.h:308). CGameCameraManager +0x78 CamTools.
- Draft models LOCALLY (all have shared headers): C3DVector, CRightHandedSet, CCharString, CDefBase/CDefPointer/CDefClassBase, CThing,
  CTCGraphicAppearance, CTools, CWorld, CGameDefinitionManager, the interface map (CVectorMap), CTCDExplosion.

## 5. Callee visibility
- GetSubDef<CJackDragonDef> 0x008E3300 is a template COMDAT emitted in the creature-mode code range (near CModeDragonHover ctor 0x008E31A0) -> visible:
  use engine/CDefClassBase_GetSubDef.h with FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (real body, noinline), as sibling 0x008DC720 does. Landed
  0x0094FFF0 also compiles a same-TU GetSubDef<CJackDragonDef> body.
- LowerBound 0x0040F020: engine/CVectorMap.h noinline body (StartBeamEffect 0x008DB620 uses it).
- CModeBase::GetWorld 0x0073A680 is out of line (folded) -- keep declaration only. CWorld::GetFrame, Create, SetThingAsImmune, GetDummyObject, CTools ray, CCharString: other TUs, decl only.
- Old NOTES "next" idea: a same-TU stand-in for the tools ray call (0x00875550) to change slot assignment of end/hitPos.

## 6. Sibling rows (same TU neighbourhood, 0x008DB240..0x008DC720)
- 00/8d/NCreatureModeCModeDragonStrafeBurninate_StartBeamEffect_008dc720.cpp -- best template: defines CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING,
  CTHING_GETTC_EXPANDED, CINTELLIGENTPOINTER(_ASSIGN), CRESOURCELIST, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CDEFSTRINGTABLE, CWORLD,
  CDEFCLASSBASE_GETSUBDEF_NOINLINE, CTCGRAPHICAPPEARANCE_DUMMYOBJECT, CMODEBASE_VIRTUALS, CMODEBASE_PARENTTHING; includes fable_string.h,
  stlport/fable_containers.h, CThing.h, CDefPointer.h, C3DVector.h, CRightHandedSet.h, CGameDefinitionManager.h, CDefClassBase_GetSubDef.h, CThingBaseDef.h,
  CWorld.h, NGraphicAppearance_CTCGraphicAppearance.h, CTCCreatureModeManager.h.
- 00/8d/CModeDragonFireBeam_StartBeamEffect_008db620.cpp -- older local-model style, same mouth-dummy call (CRightHandedSet {Up, Forward}).
- 00/8d/NCreatureModeCModeDragonFireBeam_KillBeamEffect_008dba90.cpp (FABLE_RETAIL_CTHING_GETTC_EXPANDED, CINTELLIGENTPOINTER).
- 00/8e/NCreatureModeCModeDragonFireBeam_FrameUpdate_008e0780.cpp (caller), 00/8d/NCreatureModeCModeDragonFireBeam_OnDeactivate_008dbb00.cpp.
- 00/94/CAIStateGroupJackDragon_ChooseHoverOrStrafeState_0094fff0.cpp -- same GetSubDef<CJackDragonDef> + release-without-null-test shape.
- No `#pragma optimize` needed (no ebp frame).

## 7. Draft model errors vs disassembly
- Invented types/names everywhere (CDragonDef, GetDragonDef, CToolsSource/Holder, CTCDependents, CreateExplosionObject, GetWorldFrame) -- see section 3.
- `orientation.Forward * -1.0f` with a {Forward,Up} struct: right offset, wrong member; PDB says Up at +0 -> write `orientation.Up * -1.0f` with shared CRightHandedSet.
- Tools path: should be `const CGameCameraManager* mgr; if (GetWorld().PeekGameCamera()->GetCameraManager(&mgr)) if (mgr->GetCameraTools().GetLandscapePointIntersectionFromTo(mouth, end, hitPos, hitNormal, true))`.
  Retail loads ecx=mgr ([esp+10h]) between the pushes (after hitPos push, before end push) -- the residue the old draft could not hit; const-correct
  const method/const-ref chain may change the schedule.
- Slot residue: retail end +20 / hitPos +2C (draft swapped). Consider declaring hitPos/hitNormal before the GetCameraManager test or end before mouth math
  (declaration order levers); middle reuses +38, hitNormal +50 reused as scratch.
- Create takes const C3DVector& (draft pointer); GetDummyObject references + const; GetDefGlobalIndexFromName const on CDefinitionManager.
- Control flow of draft is otherwise right: explosion null -> skip both immune+frame store; no TCI_EXPLOSION -> skip immune but still `LastExplodeFrame = CWorld::GetFrame()`;
  invisible explosion: immune only, no frame store; def released on all paths after GetSubDef (not on the BeamActive early-out).

## 0x005FBAA0

# DOSSIER 0x005FBAA0 CTCInventoryMap::GetRelativePosOnMiniMap (634 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 005fbaa0 0x28a)
- thiscall, `ret 0Ch` (3 args). NO frame pointer: `sub esp,24h ; push ebx,ebp,esi,edi` (esp frame, speed TU; no optimize("y",off), no ("s",on)). No SEH prologue (no /EHsc frame, the vector local's dtor is a plain trailing `push ebx ; call free`).
- Args at entry+0x28/+0x2C/+0x30 (after pushes +0x38/+0x3C/+0x40): region, position, relative. The position/relative arg slots are reused as float temps (width/height floats, graphic-bank pointer) -- normal VC7.1 dead-arg-slot reuse.
- Constants: 0x0122DED8 = 1.0f, 0x0122F59C = 0.5f, 0x0122DCB4 = 4294967296.0f (the `fild ; jge ; fadd 2^32` = unsigned long -> float of the GetGraphicFrameWidth/Height results).
- Body order: vector<long> copy ctor 0x00514160 on local at [esp+0x28] (source = region, i.e. region.Maps at +0) -> 3x PeekWorld()->vslot13->vslot19(maps[0]) (WorldPos X/Y; WorldSizeX + tl.X; WorldSizeY + tl.Y) -> loop i=1..size-1 (size cached in esi as (finish-start)>>2, unsigned compare `jbe`/`jb`; i lives in [esp+10h]; maps._M_start cached in ebx from 0x005FBAC5 to the end, never reloaded) -> FP normalise/aspect -> GetEngineGraphics bank (re-read from 0x013B86A0 +0x161C8 each time) slot 21 (+0x54) GetGraphicFrameWidth(GetMiniMapGraphic(&region), 0), then slot 22 (+0x58) Height -> final scale/offset -> free(maps._M_start) UNCONDITIONAL (no null test).

## 2. Name / signature (two sources)
- ghidra_out/egor_pdb_names.tsv (Ego_r 0x0064CBE0) and ghidra_out/_fablewin_api.tsv (FableWin 0x0184C5A1):
  `?GetRelativePosOnMiniMap@CTCInventoryMap@@IBEXAAVCRegion@@ABVC3DVector@@AAVC2DVector@@@Z`
  = protected, const, thiscall, void (CRegion&, const C3DVector&, C2DVector&). Manifest label is CORRECT.
- Already declared in rebuild/include/engine/CTCInventoryMap.h under FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION (line ~131, protected: `void GetRelativePosOnMiniMap(CRegion&,const C3DVector&,C2DVector&)const;`), comment names this row at 0x005FBAA0.
- Callers (python3 $S/w23/xref.py): 12 call sites in 0x005FEC41..0x00605338 (CTCInventoryMap region-description / marker builders).

## 3. Callees (landed file / landed name / PDB name; draft spelling flags)
- 0x00514160 vector<long> copy ctor: rebuild/src/compiled/00/51/stdvector_vector_00514160.cpp, STLport `vector(const _Self&)`. Draft models a local `CLongVector` -> use std::vector<long> (`std::vector<long> maps = region.GetMaps();` as landed 0x006001E0 line 105 does).
- 0x00686A80: rebuild/src/compiled/00/68/CTCBase_PeekWorld_00686a80.cpp, `?PeekWorld@CTCBase@@IBEABVCWorld@@XZ` returns const CWorld&. DRAFT WRONG: `GetWorld()` returning pointer (GetWorld is 0x00686A70, non-const). Declared in engine/CTCBase.h (FABLE_RETAIL_CTCBASE).
- CWorld vslot 13 (+0x34): engine/CWorld.h line 112 `virtual const CWorldMap& PeekWorldMap() const; // 0x004AE8C0` (FABLE_RETAIL_CWORLD). DRAFT WRONG: `GetWorldMap()` (that is slot 12, non-const) returning pointer.
- CWorldMap vslot 19 (+0x4C): engine/CWorldMap.h `virtual const CMap& PeekMap(long) const;` (slot order: dtor0..PeekMapAt15, GetMapNumberAt 16/17, GetMap 18, PeekMap 19; FABLE_RETAIL_CWORLDMAP). DRAFT WRONG: `GetMap` (slot 18 non-const).
- 0x005BEB57: rebuild/src/compiled/00/5b/CTCInventoryBase_GetMiniMapGraphic_005beb57.cpp, `?GetMiniMapGraphic@CTCInventoryBase@NInventory@@QAEJPBVCRegion@@@Z` (NON-const, takes const CRegion*; needs const_cast on this; this unadjusted: CTCInventoryBase is primary base). Opt-in FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC in engine/NInventory_CTCInventoryBase.h. Draft spelling OK but class must be NInventory::CTCInventoryBase.
- CGraphicDataBank vslot 21/22 (+0x54/+0x58): engine/CGraphicDataBank.h `GetGraphicFrameWidth/Height(unsigned long,unsigned long) const` under FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE + nested FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE (else they're _vslot21/22). Returns unsigned long (-> the 2^32 fixup). Draft names OK.
- Graphic bank read: rebuild/include/fable_game.h FABLE_RETAIL_CMAINGAMECOMPONENT + FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS (added "chunk 1011, row 0x005FBAA0"): `CCountedPointer<CGraphicDataBank> PMainGraphicBank; // +0x161C8` and inline `const CGraphicDataBank& PeekEngineGraphics() const` (FableWin ?PeekEngineGraphics@CMainGameComponent@@QBEABVCGraphicDataBank@@XZ). Static `CMainGameComponent::PMainGameComponent` (0x013B86A0) or `CMainGameComponent::Get()`. DRAFT WRONG: local `g_pMainGameComponent` / `GetGraphicDataBank()` / member `GraphicDataBank`.
- 0x00BFEA14 free import thunk (toolchain_library) = vector<long> dtor.

## 4. Members
- CRegion (engine/CRegion.h, FABLE_RETAIL_CREGION; protected members, public inline accessors): Maps +0x00 (vector<long>, accessor `GetMaps()`), MiniMapScale +0x2C (`GetMiniMapScale()`), MiniMapOffset +0x30 C2DVector (`GetMiniMapOffset()`). Draft models CRegion LOCALLY -> use header + accessors (members are protected!).
- CMap (engine/CMap.h): WorldSizeX +0x1D0C, WorldSizeY +0x1D10, WorldPos C2DCoordI +0x1D24 (Ego_r is +0x10 higher: 7452/7456/7476). Inline accessors under FABLE_RETAIL_CMAP_WORLDACCESSORS (written for THIS row, chunk 1011): `GetWorldSizeX()`, `GetWorldSizeY()`, `const C2DCoordI& GetWorldPosition()` (FableWin ?GetWorldSizeX@CMap@@QBEJXZ etc). Draft models CMap LOCALLY.
- C2DCoordI / C2DVector / C3DVector: engine/C2DCoordI.h, engine/C2DVector.h, engine/C3DVector.h (draft has local copies of all three).
- Locally-modelled classes that have shared headers: C2DCoordI, C3DVector, C2DVector, CRegion, CMap, CWorldMap, CWorld, CGraphicDataBank, CMainGameComponent (fable_game.h / engine/CMainGameComponent.h), CTCInventoryBase (engine/NInventory_CTCInventoryBase.h), CTCInventoryMap (engine/CTCInventoryMap.h), CLongVector (stlport/fable_containers.h std::vector<long>).

## 5. Callee visibility / levers
- KEY LEVER: FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE (stlport/fable_containers.h lines 401 / 605, chunk 1012): copy ctor body visible but __declspec(noinline), so VC7.1 knows the new vector is not captured and keeps start/finish in registers across virtual calls ("0x008A838C mov ebp,[esp+18h] ; mov edi,[esp+14h], never reloaded"). This is exactly the parked residue ("vector start/size cached across virtual calls", 632v634, NOTES.md line 44). The old draft only declared the copy ctor (no body) -> address escapes -> reloads. Used by landed CTCShop_GetSaleItemsToPlace_00773ee0.cpp and CVertexGroupBlockList_RemoveAllSubsets_00ad9770.cpp.
- WATCH: retail's vector dtor is `push ebx ; call free` with NO null test, while the shared _Vector_base dtor is `if (_M_start!=0) deallocate` and allocator::deallocate `if(p) free(p)`; other callers of 0x514160 (0x006001E0 at 0x00600B45, 0x00774299, 0x008A8412) DO show the `je` test. Possibly VC folded it because ebx was dereferenced (`mov ecx,[ebx]`) earlier with the copy ctor visible -- verify on first compile; if the test survives it is a 4-byte residue to study.
- PeekWorld 0x00686A80 is out of line (tiny accessor, different TU 0x686xxx) -- declaration only. PeekWorldMap/PeekMap/GetGraphicFrame* are virtual (always calls). PeekEngineGraphics, CMap world accessors, CRegion accessors are INLINE (in place reads).
- GetMiniMapGraphic 0x005BEB57 is in another TU (pragma y off / s on TU) -> declaration only.

## 6. Sibling rows
- rebuild/src/compiled/00/5f/CTCInventoryMap_GetRelativePosOnMap_005fb630.cpp (sister function, 429 bytes, `ret 0Ch`, esp frame speed TU, no pragmas; old local-struct style; calls `PeekWorld().GetWorldMap()` slot 13 -- note its slot-13 spelling is the stale GetWorldMap, PDB = PeekWorldMap).
- rebuild/src/compiled/00/60/CTCInventoryMap_ConstructRegionDescriptionShops_006001e0.cpp (same class, same vector<long> copy of region Maps): `std::vector<long> maps = region.GetMaps();`, `PeekWorld().PeekWorldMap()`; macros incl. FABLE_RETAIL_CREGION, FABLE_RETAIL_CWORLD, FABLE_RETAIL_CWORLDMAP, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CGAMECOMPONENT, FABLE_RETAIL_CMAINGAMECOMPONENT(_TEXTBANK), FABLE_RETAIL_SCOPED_PTR, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION; includes stlport/fable_containers.h, engine/CRegion.h, engine/CWorldMap.h, engine/CWorld.h, engine/CMainGameComponent.h.
- rebuild/src/compiled/00/5f/CTCInventoryMap_GetRegionTeleporter_005fbea0.cpp (neighbour, `ret 4`): defines FABLE_RETAIL_CTCBASE, CTHING, CWORLD, CWORLDMAP, CREGION, CTCINVENTORYMAP_TELEPORT, CTCINVENTORYBASE(_VTABLE); includes engine/CTCInventoryMap.h first.
- rebuild/src/compiled/00/5f/CTCInventoryMap_PanMap_005fb140.cpp (neighbour, `ret 8`, real STLport map text).
- rebuild/src/compiled/00/77/CTCShop_GetSaleItemsToPlace_00773ee0.cpp: FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE idiom for vector<long>.
- Suggested macro set: FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CWORLD, FABLE_RETAIL_CWORLDMAP, FABLE_RETAIL_CMAP_WORLDACCESSORS, FABLE_RETAIL_CREGION, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CGAMECOMPONENT, FABLE_RETAIL_CMAINGAMECOMPONENT, FABLE_RETAIL_CMAINGAMECOMPONENT_ENGINEGRAPHICS, FABLE_RETAIL_SCOPED_PTR, FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE + _FRAMESIZE, FABLE_RETAIL_CTCINVENTORYBASE(_MINIMAPGRAPHIC), FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION (check each header's prerequisites).

## 7. Old-draft model errors
- Callee names: GetWorld->PeekWorld (const CWorld&), GetWorldMap->PeekWorldMap (const CWorldMap&, slot 13), GetMap->PeekMap (const CMap&, slot 19), GetGraphicDataBank/g_pMainGameComponent->CMainGameComponent::PMainGameComponent->PeekEngineGraphics().
- Copy ctor declared without body -> the parked residue; use the COPY_OUTOFLINE lever (body visible, noinline).
- Logic/control flow otherwise matches the disassembly: 3 re-fetches of PeekMap(ids[0]) (WorldPos, then WorldSizeX + tl.X, then WorldSizeY + tl.Y); loop `for (unsigned i = 1; i < maps.size(); ++i)` (unsigned compare jbe/jb; size hoisted only if the vector is known uncaptured); in-loop order tl.X, tl.Y (Y kept in a temp [esp+14h]), br.X = X + SizeX, br.Y = SizeY + Y; ints tl/br are signed (jle/jge). FP: scaleX in FPU register (1.0 const), scaleY in memory [esp+24h]=1.0; width>height -> scaleY=h/w, else width<height -> scaleX=w/h; Width call before Height call; bank re-read before each call; final `(x*scale-0.5)*w`, `((1-y)*scale-0.5)*h`, then `+= off*scale`.
- The draft reads members directly (region.Maps, region.MiniMapScale) -- protected in the shared header; go through GetMaps()/GetMiniMapScale()/GetMiniMapOffset().

## 0x007CD9A0

# DOSSIER 0x007CD9A0 CTCBurntEffect::FrameUpdate (634 B = 0x27A; task's 0x28a window includes 6 int3 pad)

## 1. Retail shape (xdis 007cd9a0)
- __thiscall (`mov esi,ecx`), `ret` (no args), NO frame pointer (`sub esp,14h; push ebx,ebp,esi,edi`), no SEH, no /GS.
  Frame 0x14 (draft got 0x10): [esp+10h]/[esp+14h] byte ftol spills, [18h] CRGBColour / CurTime int temp, [1Ch] FadeInTime/white int temp, [20h] parent spill.
- `ParentThing` (+4) loaded into ebp at entry and spilled to [esp+20h]; ebp is reused as i*8 in the kill loop and reloaded
  (`mov ebp,[esp+20h]` 0x7CDAFF) before the fade-out tint -> `CThing* parent = ParentThing;` local at top, used for both tints and RemoveTC.
- Control flow: `if (CurTime <= FadeInTime)` (jg) {fade-in tint} `else if (CurTime >= FadeOutTimeStart)` (jl skip) {
  `if (CurTime == FadeOutTimeStart)` kill loop; fade-out tint }; then `++CurTime; if (CurTime > FadeOutTime + FadeOutTimeStart) parent->RemoveTC(0x89)`
  (reads +0x10, +0x1c, +0x18 in that order; `add edx(FadeOutTime),edi(FadeOutTimeStart)`).
- amount: fild CurTime / fidiv FadeInTime computed unconditionally, then `if (FadeInTime==0)` fstp + fld 1.0 (compute-then-override,
  divisor as int -> `(float)CurTime / FadeInTime` gives fidiv). Fade-out: fild(CurTime-FadeOutTimeStart) fidiv FadeOutTime, fsubr 1.0.
- white = ftol((1.0f - amount) * 255.0f) (consts 1.0f 0x0122DED8, 255.0f 0x01230014), stored and fild-ed back (a long, used as float).
- Per channel: fild c[i]; fmul amount; fadd white; ftol. ORDER = c[3], c[2], c[1], c[0] (def colour at [PDef+0x48] = vector<long>::_M_start).
  Byte targets: c[3]->A(+3), c[2]->B(+0), c[1]->G(+1), c[0]->R(+2). Reverse order = right-to-left evaluation of a 4-arg call
  (r=c0,g=c1,b=c2,a=c3). The landed sibling CTCCreatureTintEffect::FrameUpdate 0x7CEE40 (per-field stores) converts c0..c3 in
  forward order, so Burnt is NOT the same spelling: use the PDB ctor `CRGBColour(r,g,b,a)` (??0CRGBColour@@QAE@EEEE@Z) or
  `Initialise(r,g,b,a)` (?Initialise@CRGBColour@@QAEXEEEE@Z, FABLE_RETAIL_CRGBCOLOUR) with the four expressions as arguments.
- Parent tint: `mov ecx,[ebp+64h]; test; je; push esi(this); lea eax,colour; push eax; call 4C00B0` (appearance read once).
- Tint loop (both branches): pointer iterator over TintList (+0x2c begin in edi, +0x30 end in ebx cached, `add edi,8`), ONE GetPItem
  per element, `test byte [eax+91h],1` (Dead), `[eax+64h]` read once into a local -> `CThing* t = it->Get(); if (t && !t->IsDead())
  { appearance = t->GetTCGraphicAppearance(); if (appearance) appearance->SetTintColour(colour,this); }` (IS_THING_ALIVE<CThing*> on a raw
  pointer is in FableWin and would also give one GetPItem). Loop is rotated (`cmp edi,ebx; je` guard, then bottom test).
- Kill loop: index `unsigned i` (`xor ebx,ebx`, `jbe`/`jb` unsigned), size() = (end-begin)>>3 re-read every pass, `Effects[i]`
  recomputed from [esi+20h]+i*8 for each use; GetPItem called TWICE (null test, then Dead test) = IS_THING_ALIVE(Effects[i]) on the
  CIntelligentPointer; then `CTCSpecialAbilities::FadeOrKillEffect(Effects[i])` (ecx = element address, __fastcall static).
  This is byte-for-byte the landed dtor 0x007CDC20 loop: `for (unsigned int i = 0; i < Effects.size(); ++i) if (IS_THING_ALIVE(Effects[i])) CTCSpecialAbilities::FadeOrKillEffect(Effects[i]);`
- No helper: FableWin.pdb lists every CTCBurntEffect member (no SetTint/UpdateTint); the tint block is written twice inline (or a macro).
  The draft's invented `SetTint` __forceinline is not a retail name (ok only as a non-emitted forceinline, but cannot be a landed name).

## 2. Identity
- Name: CTCBurntEffect::FrameUpdate, mangled `?FrameUpdate@CTCBurntEffect@@UAEXXZ` (public virtual void (void)).
  Sources: rtti-vtables.tsv `CTCBurntEffect 0126efbc slot 9 -> 007cd9a0` (only xref: .rdata 0x0126EFE0 = vtable+0x24);
  Ego_r.pdb egor_pdb_names.tsv 007cbe20 ?FrameUpdate@CTCBurntEffect@@UAEXXZ (Ego_r build, same member order FinalCleanup/OnKill/OnRemove);
  FableWin _fablewin_api.tsv has ?FrameUpdate@CTCBurntEffect@@UAEXXZ; sibling CTCCreatureTintEffect::FrameUpdate is also slot 9.
  Manifest label is CORRECT. Leaf: `?FrameUpdate@CTCBurntEffect@@UAEXXZ`.
- Same TU neighbours (retail order): FinalCleanup 7CD930, OnKill 7CD980, OnRemove 7CD990 (jmp forwarder), FrameUpdate 7CD9A0,
  ~CTCBurntEffect 7CDC20, Reset 7CDD10, ctor 7CDFE0, GetDef<..> 7CE100, GetInterfaceType 7CE170, GetClassTypeName 7CE180,
  HasFrameUpdate 7CE1A0, scalar dtor 7CE1B0.

## 3. Callees
| addr | landed file | landed name / PDB | draft spelling issue |
|---|---|---|---|
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | __ftol2 (compiler) | - |
| 0x004C00B0 | 00/4c/NGraphicAppearanceCTCGraphicAppearance_SetTintColour_004c00b0.cpp | ?SetTintColour@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCRGBColour@@PAX@Z | ok (param name `key`) |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | draft returns CThing* - WRONG (returns CBaseObject*); call via CIntelligentPointer<CThing>::Get() |
| 0x0072ABD0 | 00/72/CTCSpecialAbilities_FadeOrKillEffect_0072abd0.cpp | ?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z (static __fastcall) | draft has a FREE function `FadeOrKillEffect(CBaseIntelligentPointer&)` - WRONG class and param type |
| 0x004C9840 | 00/4c/CThing_RemoveTC_004c9840.cpp | ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z | draft `RemoveTC(long)` - WRONG, param is ETCInterfaceType (0x89 = TCI for CTCBurntEffect; cast `(ETCInterfaceType)0x89` or the enum name from ETCInterfaceType.h) |
No vtable calls.

## 4. Members
- this (CTCBurntEffect, NO shared header; layout per landed ctor 7CDFE0 / dtor 7CDC20, Ego_r adjusted for 12-byte STLport vectors):
  +4 ParentThing (CTCBase.h, FABLE_RETAIL_CTCBASE), +0xc PDef CDefPointer<const CSpecialAbilitiesBurntEffectDef> (CDefPointer.h; Get()/operator-> = static_cast<T*>(Object)),
  +0x10 CurTime, +0x14 FadeInTime, +0x18 FadeOutTimeStart, +0x1c FadeOutTime (long), +0x20 Effects, +0x2c TintList
  (std::vector<CIntelligentPointer<CThing> >). Retail sizeof 0x38 (Ego_r 0x40). Draft's PDef as raw pointer works byte-wise but CDefPointer is the real type.
- CSpecialAbilitiesBurntEffectDef (no header): Ego_r Color `vector<long>` +76 (16-byte vector); retail +0x48. FadeIn/OutTimeSecs floats before it. Must derive (first base) from CDefPointeeBase so static_cast does not adjust.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_STATEACCESS): PTCGraphicAppearance +0x64 via inline
  GetTCGraphicAppearance() (FableWin ?GetTCGraphicAppearance@CThing@@QAE...); Dead bitfield +0x91 bit0 via IsDead() const; RemoveTC(ETCInterfaceType) declared line 466.
- CRGBColour (engine/CRGBColour.h): B,G,R,A at 0..3; 4-arg ctor always present; FABLE_RETAIL_CRGBCOLOUR adds Initialise(r,g,b,a).
- CIntelligentPointer (engine/CIntelligentPointer.h): FABLE_RETAIL_CINTELLIGENTPOINTER (+ _GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT for IS_THING_ALIVE).
- NGraphicAppearance::CTCGraphicAppearance (engine/NGraphicAppearance_CTCGraphicAppearance.h): FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT declares SetTintColour.
- CTCSpecialAbilities (engine/CTCSpecialAbilities.h): FadeOrKillEffect declared under FABLE_RETAIL_DIVINE_FINALCLEANUP or FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE.
- engine/ThingAlive.h: IS_THING_ALIVE under FABLE_RETAIL_IS_THING_ALIVE.
- Draft models LOCALLY (headers exist): CRGBColour, NGraphicAppearance::CTCGraphicAppearance, CThing, CBaseIntelligentPointer, (CTCBase is included). Only CTCBurntEffect and the def lack headers.

## 5. Callee visibility
- All five callees are out-of-line calls in retail; SetTintColour/GetPItem/RemoveTC/FadeOrKillEffect are in other TUs (addresses far away) -> declarations only.
- Inline (must be visible, not called): CIntelligentPointer::Get / operator->, CThing::IsDead, CThing::GetTCGraphicAppearance, CDefPointer::operator->,
  vector::size/operator[]/begin/end, IS_THING_ALIVE (FableWin template ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_N...), CRGBColour ctor/Initialise.
- No STLport out-of-line helpers called.

## 6. Sibling rows
- 00/7c/CTCBurntEffect_CTCBurntEffect_007cdc20.cpp (dtor, same TU, right after): the exact kill-loop spelling with IS_THING_ALIVE + FadeOrKillEffect (local models, no flags).
- 00/7c/CTCBurntEffect_FinalCleanup_007cd930.cpp: TintList pointer loop `CThing* thing=p->GetPItem(); if(thing&&!thing->Dead){ appearance=thing->PTCGraphicAppearance; if(appearance) ...}`.
- 00/7c/CTCCreatureTintEffect_FrameUpdate_007cee40.cpp: template for fade math (FABLE_RETAIL_CTHING, _STATEACCESS, FABLE_RETAIL_CINTELLIGENTPOINTER; local CRGBColour; FABLE_FORCEINLINE SetTint) - but forward channel order.
- 00/78/CTCDivineWrath_FinalCleanup_0078faf0.cpp: the shared-header macro set for IS_THING_ALIVE/FadeOrKillEffect:
  `// Flags: /d2inlT62`, FABLE_RETAIL_CTCBASE, _CTHING, _CINTELLIGENTPOINTER, _COPY, _COPY_ORIGINAL_CHAIN, _GET_ORIGINAL_CHAIN,
  _COMPARE_NULL_INT, FABLE_RETAIL_IS_THING_ALIVE, FABLE_RETAIL_DIVINE_FINALCLEANUP, FABLE_RETAIL_CTCSPECIALABILITIES_FULL; includes ThingAlive.h, CTCSpecialAbilities.h.
- No CTCBurntEffect row uses pragmas/flags (default /O2 /Oy, no optimize("s")).

## 7. Draft model errors
1. Colour channel order: draft got c3,c1,c0,c2 (local ctor with init-list B(b),G(g),R(r),A(a)); retail c3,c2,c1,c0 -> shared CRGBColour.h ctor `R=r;G=g;B=b;A=a` or Initialise, args (c0..c3 exprs).
2. Kill loop must be the dtor's form: IS_THING_ALIVE(Effects[i]) then CTCSpecialAbilities::FadeOrKillEffect(Effects[i]) (draft calls Get twice by hand: equivalent, keep).
3. Frame 0x14 v 0x10 and parent spill: retail keeps parent in ebp across the whole function and needs a 5th frame slot; the draft's
   forceinline SetTint(parent,amount) may shift allocation - try writing the tint block inline twice (no helper) with `parent` local.
4. Wrong callee types: GetPItem returns CBaseObject*, FadeOrKillEffect is CTCSpecialAbilities static __fastcall on CIntelligentPointer<CThing>&, RemoveTC takes ETCInterfaceType.
5. Local CThing/CRGBColour/CTCGraphicAppearance/CBaseIntelligentPointer stand-ins -> shared headers (verifier warns); PDef should be CDefPointer<const CSpecialAbilitiesBurntEffectDef>.

## 0x00B211B0

# DOSSIER 0x00B211B0 -- NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo (634 B)

## 1. Retail shape (xdis 00b211b0 0x28a)
- __thiscall, `ret 8` (0x00B21427): args `const CSplineRandomOffsetData& other` [esp+4], `float t` [esp+8].
- NO frame pointer (`sub esp,34h` first; no push ebp/mov ebp,esp) -> default /Oy. NO SEH/EH frame even though CArray
  temporaries are destroyed (free 0x00BFEA14 after `test eax,eax`) -> same EH-less shape as the landed parent 0x00B22310.
- Not size-optimised (/O2-style: inline CArray dtors, imul-0x2AAAAAAB divides by 12).
- Register map: esi=this, ebx=&other (loaded 0x00B211BE BEFORE pushes), ebp=&ThisSpline.ControlPoints (this+4, 0x00B21202),
  edi=&PreviousSpline.ControlPoints (this+0x18, 0x00B2129F; pushed late at 0x00B2126A, popped 0x00B213CC).
  Later ebp is reused for `t` (0x00B21387) and edi for &SplinePosOffsetLimits (+0x30); esi+=0x3C / ebx+=0x3C for the Speed arrays.
- `1.0f - t` (1.0 @0x0122DED8) computed once and spilled to [esp+0xc] (named float). 0.5 @0x0122F59C; `t > 0.5f` takes other's.
- Count compare: ThisSpline count computed first, spilled into the dead `other` arg slot [esp+48h]; then PreviousSpline count;
  `cmp ecx(This),eax(Prev); je skip` -> on mismatch: operator=(0x0075B8D0) with ecx=edi (Prev.ControlPoints), push ebp
  (This.ControlPoints), then dword +0xC (Tension) and byte +0x10 (EndPointsConnected) copied = compiler-generated
  CCardinalSpline::operator= inline (`PreviousSpline = ThisSpline`).
- Each spline block: zero C3DVector default (3 zero stores), copy-ctor 0x006F0810 of other's points (pushed first, ecx=temp),
  then copy-ctor of own points, GFInterpolateArray (__fastcall: ecx=result, edx=own copy, stack: other copy, t, &zero),
  operator= into own ControlPoints, then 3 inline dtors (result, own copy, other copy order: [esp+34],[esp+1c],[esp+28]).
- Offset arrays (+0x30,+0x3C): NO copies -- passed by reference directly to GFInterpolateArray, 1 dtor each.

## 2. Name / signature (2 sources)
- FableWin.pdb: `?InterpolateTo@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z`
  (ghidra_out/_fablewin_api.tsv 0x02F5D5E0, naming_stragglers/FableWin.exe.signatures.tsv 0x02F5D5E0, 1368 B debug body).
- Ego_r.pdb: same mangled name (egor_pdb_names.tsv / egor_signatures.tsv @0x00AE3000).
- Caller: only 0x00B223D6 inside landed CPSCSplineBase::InterpolateTo 0x00B22310
  (rebuild/src/compiled/00/b2/NParticleEngineCPSCSplineBase_InterpolateTo_00b22310.cpp), which declares
  `void InterpolateTo(const CSplineRandomOffsetData& other, float t); // 0x00B211B0`.
- Manifest label CSplineRandomOffsetData::InterpolateTo is CORRECT. Signature: `void InterpolateTo(const CSplineRandomOffsetData&, float)` non-const, public, non-virtual.
- Leaf for header: `// Leaf: ?InterpolateTo@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z` (old draft wrote only `InterpolateTo@CSplineRandomOffsetData`).

## 3. Callees
| addr | landed file | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x006F0810 | rebuild/src/compiled/00/6f/STLvectorC3DVector_vector_006f0810.cpp | std::vector<C3DVector>::vector(const vector&) (STLport copy ctor; Leaf ??0?$vector@) | draft: `CArray<C3DVector>(const CArray&)` -- WRONG owner: it is the base std::vector copy ctor, not a CArray member |
| 0x0075B8D0 | rebuild/src/compiled/00/75/stdvector_operator_0075b8d0.cpp | std::vector<C3DVector>::operator= (Leaf ??4?$vector@VC3DVector@@) | draft: `CArray::operator=` -- same issue (CArray has no own operator=; it's vector's) |
| 0x00B15BD0 | rebuild/src/compiled/00/b1/NParticleEngine_GFInterpolateArrayC3DVector_00b15bd0.cpp | `??$GFInterpolateArray@VC3DVector@@@NParticleEngine@@YI?AV?$CArray@VC3DVector@@@@ABV1@0MABVC3DVector@@@Z` -- NParticleEngine::GFInterpolateArray<C3DVector>, __fastcall | draft declares it GLOBAL (outside NParticleEngine) -- wrong mangling; the parent row was respelled in "Naming wave 35c" to the namespace version |
| 0x00BFEA14 | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free (inline ~vector -> deallocate) | ok |

## 4. Members (this and other)
- PDB (struct_layouts_egor.tsv, debug 16-byte CArray so offsets differ): CSplineRandomOffsetData { CCardinalSpline* InitialSpline;
  CCardinalSpline ThisSpline; CCardinalSpline PreviousSpline; bool OffsetOnlyAtStartup; bool ResetEachFrame; CArray<C3DVector>
  SplinePosOffsetLimits; CArray<C3DVector> SplinePosOffsetSpeed; }. Retail offsets (12-byte CArray): +0, +4, +0x18, +0x2C, +0x2D, +0x30, +0x3C (0x48 total; ctor 0x00B21DD0 / dtor 0x00B22EE0 agree).
- CCardinalSpline (PDB): CArray<C3DVector> ControlPoints +0, float Tension +0xC, bool EndPointsConnected +0x10 -- ALL PRIVATE.
  Shared header rebuild/include/engine/CCardinalSpline.h (size 0x14 asserted) offers the original inline accessors:
  GetTension()/SetTension(const float)/GetControlPointCount() (long, ControlPoints.size())/GetControlPoints() (by value)/
  SetControlPoints(const CArray<C3DVector>&). No FABLE_RETAIL_* macros in it.
- rebuild/include/engine/CArray.h: CArray<T> : public std::vector<T> (stlport/fable_containers.h); macros FABLE_RETAIL_CARRAY,
  _AT, _SIZE_CTOR_*, _GETASCARRAY_START, _DEALLOCATEUNUSEDMEMORY, _PUSHBACK(_NOINLINE).
- rebuild/include/engine/C3DVector.h: public X/Y/Z (ctor (x,y,z)).
- No shared header for CPSCSplineBase / CSplineRandomOffsetData (only engine/CPSCBase.h) -> those stay local.
- Draft models LOCALLY, though shared headers exist: C3DVector, CArray, CCardinalSpline (with PUBLIC members -- PDB says private).

## 5. Callee visibility (Q32 lesson) -- the most likely cure for the ebx/ebp colouring residue
- 0x006F0810 copy ctor and 0x0075B8D0 operator= are STLport inline bodies emitted as COMDATs (called out of line). Retail's TU
  saw the bodies. Opt-ins in rebuild/include/stlport/fable_containers.h:
  * FABLE_RETAIL_STLP_VECTOR_C (line ~436): operator= body visible from _vector.c but still called out of line; its comment says
    seeing that operator= never captures its argument changed the caller's register use (row 0x00B149D0 CPSCEmitterGeneric::ReadBinary, same NParticleEngine area, vector<C3DVector> 0x0075B8D0).
  * FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE (line 401/605): copy ctor body visible but __declspec(noinline) -- same
    non-capture effect for the caller (chunk 1012; also used by 0x00AD9770, 0x00773EE0).
  * FABLE_RETAIL_STLP_VECTOR, FABLE_RETAIL_CARRAY (used together with _VECTOR_C by 0x00B13530 CPSCEmitterGeneric::DatabaseSerialise, which includes engine/CCardinalSpline.h).
  * FABLE_RETAIL_STLP_VECTOR_ASSIGN: declaration-only operator= (alternative).
- The old draft declared both as body-less (opaque) -> compiler must assume `other`/ThisSpline addresses escape; the residue is
  exactly which of {other, &ThisSpline.ControlPoints} gets ebx vs ebp. Try shared CArray.h + _VECTOR_C + _VECTOR_COPY_OUTOFLINE (+ _STLP_VECTOR, _CARRAY) first.
- GFInterpolateArray 0x00B15BD0 lives in another TU region (0x00B15xxx, CPSCEmitterGeneric area) -- declaration only.

## 6. Sibling landed rows (no `// Flags:`, no pragmas -> default flags)
- 0x00B22310 CPSCSplineBase::InterpolateTo (parent, same TU likely): local CArray/CCardinalSpline stand-ins, `#include "fable_compiler.h"`,
  `#include "engine/C3DVector.h"`, NParticleEngine::GFInterpolateArray declared in namespace, `(1.0f - t) * Spline.Tension + t * source->Spline.Tension`, by-value GetControlPoints() copies, `C3DVector(0.0f,0.0f,0.0f)` default.
- 0x00B21160 CSplineRandomOffsetData::Update: `PreviousSpline = ThisSpline;` (compiler-generated CCardinalSpline operator=).
- 0x00B21DD0 ctor, 0x00B22EE0 dtor (uses shared engine/CArray.h + engine/C3DVector.h), 0x00B21E90 DatabaseSerialise / 0x00B21A50 (defines FABLE_RETAIL_STLP_VECTOR, _CPERSISTCONTEXT, _CCHARSTRING...), 0x00B22100 ReadBinary (`#pragma intrinsic(memcpy)`).
- 0x00B13530 CPSCEmitterGeneric::DatabaseSerialise: shared engine/CCardinalSpline.h + FABLE_RETAIL_CARRAY + FABLE_RETAIL_STLP_VECTOR_C + FABLE_RETAIL_STLP_VECTOR.

## 7. Old draft model errors (draft = residue file, DIFFER 633v634, hand298 NOTES line 70)
- Control flow, loop-free shape, FP order, copies vs references, count compare, 0.5 ternaries all MATCH (only 1 byte off).
- Only residue: ebx<->ebp swap (retail ebx=other, ebp=&ThisSpline.ControlPoints then t). Tried before: compare order, named count,
  const& view of other, forceinline getter. NOT tried: visible STLport bodies (section 5), shared CCardinalSpline.h with its
  private members + GetTension/SetTension/GetControlPoints/SetControlPoints/GetControlPointCount (original accessor spelling,
  e.g. `ThisSpline.SetControlPoints(GFInterpolateArray(ThisSpline.GetControlPoints(), other.ThisSpline.GetControlPoints(), t, C3DVector(0,0,0)))`).
- Naming defects: GFInterpolateArray must be in namespace NParticleEngine; copy ctor/operator= belong to std::vector<C3DVector>
  (via shared CArray.h); CCardinalSpline members must be private (shared header); Leaf line incomplete.

## 0x007BF510

# DOSSIER 0x007BF510 -- NThingShot::CTCShotDefinitionBase::OnCollision (635 bytes)

## 1. Retail shape (xdis 007bf510 0x28b)
- thiscall, `ret 0x10` (4 dword args), esp frame (`sub esp,18h`; NO push ebp/mov ebp,esp -> no optimize("y",off)), no SEH/EH frame
  (temporaries destroyed with plain calls; the `bl` flag 0x7BF533/0x7BF572 is the conditional-temporary-destruction flag of a
  temp built inside a `&&`, not EH). Pushes ebx, ebp, esi, (edi later at 0x7BF58C, popped at 0x7BF781). Frame 0x18.
- Dead parameter slots reused: the find bool lands in the `thing` slot ([esp+34h]@0x7BF567 = entry+0x10, thing kept in ebp);
  the iterator tag address is the `type` slot (lea [esp+28h]@0x7BF555 = entry+4; empty tag, no store);
  the 0x4C GetTC key is stored into the `thing` slot (mov [esp+3Ch],4Ch @0x7BF5A6 after push edi/push ecx = entry+0x10).
  `position` read at [esp+30h]@0x7BF610 (entry+8), `normal` at [esp+34h]@0x7BF692 (entry+0xC), `type` at [esp+2Ch]@0x7BF6D4/0x7BF71C.
- Not size-optimised (no optimize("s") idioms; fpu straight-line). Siblings in this TU use no #pragma optimize.

## 2. Identity (manifest label CORRECT in name; class namespace NThingShot)
- vtable slot: rtti-vtables.tsv `NThingShot::CTCShotDefinitionThrow 0123cdc4 28 007bf510`; header
  engine/NThingShot_CTCShotDefinitionBase.h (FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE block) names slot 28 = OnCollision, slot 29 = OnHitByMelee.
- Ego_r name set (egor_pdb_names.tsv / egor_signatures.tsv, Ego_r addr 0x007bf7d0):
  `?OnCollision@CTCShotDefinitionBase@NThingShot@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z`
- Callers: QueenHornetShot::OnCollision 0x007BBB36 (landed, `jmp` = CTCShotDefinitionBase::OnCollision(type,position,normal,thing)),
  NymphFun::OnCollision 0x007BDB66 (landed, calls base 0x007BF510), plus 0x007BDCF3/0x007BDE4C/0x007BE3D5.
- Signature: `public: virtual void OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing);`
  First param is ECollideType (stored into BestCollisionType +0x90), NOT `long frame` as the draft says.

## 3. Callees
| addr | landed file | landed name | PDB / note | draft spelling issue |
|---|---|---|---|---|
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer() | Ego_r ??0CBaseIntelligentPointer | ok |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem(CBaseObject*) | ok | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const | ok | ok |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ~CBaseIntelligentPointer (virtual in shared header) | ok | draft has non-virtual dtor + fake v00 |
| 0x007C2980 | 00/7c/std_find_007c2980.cpp | std::__find<CIntelligentPointer<const CThing>*,...>(first,last,val,const random_access_iterator_tag&) __fastcall, ret 8 | STLport __find | draft "StdVector_Find" with CIntelligentPointer<CThing>* elements -- WRONG element type |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound(const ETCInterfaceType&) | PDB-exact | draft CInterfaceMap local |
| 0x0076E4A0 | 00/76/CTCWeaponTrail_HasWeaponTrailPoints_0076e4a0.cpp | CTCWeaponTrail::HasWeaponTrailPoints() const | Ego_r ?HasWeaponTrailPoints@CTCWeaponTrail@@QBE_NXZ | draft non-const |
| 0x0076E5D0 | 00/76/CTCWeaponTrail_GetLastProjectileWeaponTrailPoints_0076e5d0.cpp | GetLastProjectileWeaponTrailPoints(vector<pair<C3DVector,C3DVector> >&) const | FableWin ?GetLastProjectileWeaponTrailPoints@CTCWeaponTrail@@QBE_NAAV?$vector@U?$pair@VC3DVector@@V1@@std@@... | draft "GetWeaponTrailPoints(CTrailPointVector*)" WRONG name, pointer vs reference, element type |
| 0x004A96F0 | 00/4a/CIntelligentPointer_CThing_CIntelligentPointer_004a96f0.cpp | CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) out of line | FableWin ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z, vtable 0x01238C6C | draft calls it CIntelligentPointer<const CThing> -- WRONG, it is <CThing> |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (CThing.h:411 `const C3DVector& GetPos() const;`) | catalog GetPos; inline-in-PDB GetPosition | draft "GetPosition" |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | STLport vector dealloc (points dtor) | ok |

## 4. Members (shared header engine/NThingShot_CTCShotDefinitionBase.h, needs FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT)
- +0x04 CTCBase::ParentThing (CThing*)
- +0x14/+0x18 `std::vector<CIntelligentPointer<const CThing> > HitThingsList` (8-byte elements) -- draft had <CThing>.
- +0x70 `C3DVector BestCollisionPos`; +0x7C `C3DVector BestCollisionNormal`; +0x88 `CIntelligentPointer<CThing> PBestCollisionThing`;
  +0x90 `ECollideType BestCollisionType` (draft: CollisionPos/CollisionNormal/PCollisionThing as <const CThing>/long CollisionFrame -- all wrong names, two wrong types).
  These are `private:` under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION (fine, member function).
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): +0x91 bit0 = `Dead` bitfield (CThing.h:88; IsDead() line 280) -> draft `Flags91 & 1`
  is `thing->IsDead()`; with the null test it is `IS_THING_ALIVE(thing)` (engine/ThingAlive.h, FABLE_RETAIL_IS_THING_ALIVE: `if(thing!=0){if(!thing->IsDead())return true;}return false;`).
  +0x20 UsingComponents bitset (0x4C>>5 = word +0x28, bit 12 -> `test ah,10h`), +0x44 Components CVectorMap.
- TCI_WEAPON_TRAIL = 76 = 0x4C (engine/ETCInterfaceType.h:81).
- CTCWeaponTrail: engine/CTCWeaponTrail.h; under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION it declares
  `bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector> >&)const;` (+ HasWeaponTrailPoints).
- Draft models LOCALLY classes that have shared headers: C3DVector (engine/C3DVector.h), CBaseIntelligentPointer/CIntelligentPointer
  (engine/CIntelligentPointer.h), CTCWeaponTrail (engine/CTCWeaponTrail.h), CThing (engine/CThing.h), CVectorMap/"CInterfaceMap"
  (engine/CVectorMap.h), CTCShotDefinitionBase (engine/NThingShot_CTCShotDefinitionBase.h), vector (stlport/fable_containers.h).

## 5. Callee visibility / opt-ins
- GetTC: retail is exactly CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED `GetTC(ETCInterfaceType id, T** ptc)` (presence test, key local
  stored after the test, map pointer local `add edi,44h`, end read `mov edi,[edi+4]`, conditional-expression end-or-greater, ->second).
  Write `CTCWeaponTrail* trail; if (ParentThing->GetTC((ETCInterfaceType)0x4c, &trail)) { std::vector<std::pair<C3DVector,C3DVector> > points; ... }`
  (je at 0x7BF596 goes to 0x7BF781 which skips the vector dtor -> vector declared INSIDE the GetTC block; both trail branches join at 0x7BF770 dtor -> declared before the HasWeaponTrailPoints if).
- std::find: inline front end, out-of-line `__find` (0x007C2980, fastcall, value = CIntelligentPointer<CThing> temp with vtable 0x01238C6C
  constructed INLINE: a01b10 + vtable store + SetPItem). No shared std::find/__find in rebuild/include/stlport (only __find_if in fable_string.h) --
  declare the STLport `__find` template locally (copy std_find_007c2980.cpp's declaration: `template<class _RandomAccessIter,class _Tp> _RandomAccessIter __fastcall __find(..., const random_access_iterator_tag&)`), call as `std::find(HitThingsList.begin(), HitThingsList.end(), CIntelligentPointer<CThing>(thing))`.
  random_access_iterator_tag is in stlport/fable_containers.h (empty ctor).
- CIntelligentPointer<CThing>(T*) ctor: inline at the find temp (0x7BF533..) and at the NULL temp (0x7BF6E3..), but OUT OF LINE (0x004A96F0)
  for the `PBestCollisionThing = CIntelligentPointer<CThing>(thing)` temp at 0x7BF6B1. One header cannot give both via
  FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE (declaration-only); likely VC7.1 per-caller inline budget (see CThing.h:301-305 note) -- open question for the author.
- operator=: inline GetPItem then SetPItem -> FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (default spelling `CBaseObject* item = other.GetPItem(); SetPItem(item);`).
- Squared length: Z*Z+Y*Y+X*X order for BOTH vectors (0x7BF647 `fld st(2); fmul st(3)` = dz first, then dy, then dx; second from the stored z)
  -> engine/C3DVector.h FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX; PDB name GetSquaredMagnitude (draft GetLengthSquared).
  Compare: update when (position - tip).GetSquaredMagnitude() < (BestCollisionPos - tip).GetSquaredMagnitude() (fcompp, test ah,41h, jne skip).

## 6. Sibling rows (same class / TU neighbourhood)
- 0x007C1000 NThingShotCTCShotDefinitionBase_OnCollisionProper_007c1000.cpp (RELOCATION_MATCH, same class; no pragmas). Defines
  FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION/_MOVEMENT/_ONHITBYMELEE, CINTELLIGENTPOINTER(+_COPY,_CONVERT_MUTABLE,_ASSIGN), C3DVECTOR,
  CTHING, CTHING_GETTC_EXPANDED, IS_THING_ALIVE ...; includes engine/NThingShot_CTCShotDefinitionBase.h, engine/CTCWeaponTrail.h, engine/ThingAlive.h.
  Uses `ParentThing->GetTC((ETCInterfaceType)0x4c, &trail); std::vector<std::pair<C3DVector, C3DVector> > points;` and `(points.end() - 2)->first`.
- 0x007BBB36 QueenHornetShot::OnCollision (pragma optimize("s",on) -- different TU, size-opt; NOT this one).
- 0x007BDB66 NymphFun::OnCollision (optimize("y",off)+("s",on); different TU).
- 0x007C0FB0 AddHitThing (push_back(CIntelligentPointer<const CThing>(thing)); "module built without /GX").
- 0x007BF3D0 HasPassedTarget, 0x007BF460 ctor, 0x007BF350 OnRemove, 0x007C04A0 UpdateCreatureAwarenessOfMe (auto_inline(off) region) -- same file neighbourhood.

## 7. Old draft model errors
1. `long frame` -> `ECollideType type`; member +0x90 is BestCollisionType.
2. HitThingsList elements are CIntelligentPointer<const CThing>; the find VALUE temp is CIntelligentPointer<CThing>(thing) (vtable 0x01238C6C).
3. `thing->Flags91 & 1` is the Dead bit: IS_THING_ALIVE(thing).
4. Trail points: vector<pair<C3DVector,C3DVector>> (stride 0x18); retail reads [finish-0x30] = `(points.end()-2)->first`, NOT back() of a
   0x30-stride struct. Copied into a local C3DVector (3 dwords to [esp+10h..18h]).
5. Draft split HasTC + GetTC (two inline helpers); use GETTC_EXPANDED GetTC(id,&trail).
6. Update branch: PBestCollisionThing is CIntelligentPointer<CThing> (draft used <const CThing> spec. + out-of-line ctor of the wrong type).
   Store order: BestCollisionPos = position; BestCollisionNormal = normal; PBestCollisionThing = CIntelligentPointer<CThing>(thing); BestCollisionType = type.
7. Else branch order: PBestCollisionThing = CIntelligentPointer<CThing>(NULL) (inline ctor); BestCollisionType = type; BestCollisionPos = ParentThing->GetPos();
   BestCollisionNormal = C3DVector(0.0f,0.0f,1.0f) (temp materialised in the frame then copied).
8. GetLengthSquared X,Y,Z -> GetSquaredMagnitude Z,Y,X.
9. Residue recorded (hand305 NOTES): frame 0x34 v 0x18, parameter-slot reuse; the local stand-ins (non-packed pointer, wrong types) are the likely cause.

## 0x00972CC0 (not landed)

# DOSSIER 0x00972CC0 -- CAIStateGroup_JackOfBladesStage1::MeleeCombat (612 bytes)

## 1. Retail shape (xdis 00972cc0 0x274)
- thiscall, plain `ret` (no args). this -> edi. Returns `mov eax,2` = AI_ACTION_FINISHED (engine/EAIAction.h: NULL0,TERMINATE_PROCESS1,FINISHED2,FAILED3,IN_PROGRESS4).
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0x58; push ebx,ebp,esi,edi` -- the and-esp/-8 frame comes from an 8-aligned
  local (same as landed sibling 0x0093DDE0 AttackEnemy: "the 8-byte look-at temporary inside the loop gives the and esp,-8 frame").
  ebp is then reused as a general register. No SEH, no /GS.
- Locals (esp after pushes): +0x14 float distance, +0x18 temp CIntelligentPointer<const CThing> (8), +0x20 CBrainOutput (0x48:
  Types+0x20, LookFlags+0x24, Nav +0x28 (Proximity+0x34 imm 0, MovementType+0x38 esi, Speed+0x3c esi), PAction +0x40/+0x44, LookAtThing +0x60).
  Local+0x10..+0x14 is unused (frame 0x58 vs draft 0x54).
- Flow: prologue `if (creature->IsInMeleeAsAttacker()) GetWorld().PThingCombatManager->SetAttackerAsReadyToAttack(*creature,true)`
  (creature reloaded via [[edi+4]+0x18] twice). Loop top 0x972d00 (aligned by `lea esp,[esp]` filler):
  inline IsStillRunnable (PTargetThing alive (GetPItem!=0 && !(+0x91&1)), !IsStopping(), PArenaCentre alive) -- fail -> 0x972f10 Terminate(); return 2.
  Body: CBrainOutput ctor inline (LookAtThing ctor 0xA01B10 + vptr 0x01237F1C store, inline PAction release), Types|=0x20 (in memory `or`),
  SetAsLookAtThing inline (GetPItem(PTargetThing)->esi, Types|=4 via eax, temp ptr ctor+vptr(ebp)+SetPItem, GetPItem, LookAtThing.SetPItem, temp dtor),
  LookFlags=0x11, PBrain->SetOutput(output), PBrain->vslot2 (SuspendSuspendableProcess, result ignored);
  then distance: creature=PBrain->Creature, target=PTargetThing.GetPItem() (ebp), GetPos(creature)->esi, GetPos(target)->eax,
  dx=t.X-c.X, dy=t.Y-c.Y, fsqrt(dx*dx+dy*dy) (2D magnitude of target-creature), stored to [esp+0x14] BEFORE output dtor;
  output dtor inline: LookAtThing dtor call 0xA01C10, then PAction count release (dec/destroy/operator delete 0xBFE9BC; zero obj+count).
  Latch: inline IsStillRunnable again (fail -> return 2 WITHOUT Terminate, shared epilogue 0x972f17),
  `distance < GetCreature().GetCombatTypeDef().FrontMiddleLimit` (fcomp [eax+0x70]; test ah,5; jp exit) else return 2,
  HasHitsCausedRush() false -> jmp top; true -> return 2 via a SEPARATE epilogue 0x972f03 (pop order edi,esi,ebp,ebx vs 0x972f17 interleaves mov eax,2 after pop edi).
- The vptr constant 0x01237F1C is materialised in ebp once per iteration (`mov ebp,0x1237f1c` at 0x972d81) and stored twice (LookAtThing +0x60 and temp +0x18).

## 2. Identity (two sources) -- manifest label class is right, details wrong in draft
- Ego_r.pdb (egor_pdb_names.tsv 0x00997D30) and FableWin.pdb (_fablewin_api / naming_stragglers 0x02BEAC30, 0x017C8E95):
  `?MeleeCombat@CAIStateGroup_JackOfBladesStage1@@IAE?AW4EAIActionReturn@@XZ` = protected, non-const, non-virtual, returns EAIActionReturn, no params.
- Caller: only 0x00976769 inside CAIStateGroup_JackOfBladesStage1::Update 0x00975BB0 (vtable 0x0128D6B4 slot 12; parked draft
  rebuild/candidates/manual/pending/2026-11-11_codex_pass21/return/parked/00975bb0/00975bb0.cpp calls `MeleeCombat();`).
- Shared header rebuild/include/engine/CAIStateGroup_JackOfBladesStage1.h already declares `EAIActionReturn MeleeCombat();` (protected)
  under FABLE_RETAIL_JACK_STAGE1_UPDATE. Draft returns `long` -> WRONG.
- PDB order (Ego_r: HasHitsCausedRush 0x997290 < MeleeCombat 0x997d30) matches retail order (0x972340 < 0x972CC0): same TU.

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | PDB / landed name | draft | flag |
|---|---|---|---|---|
| 0x00662410 | 00/66/CThingCreatureBase_IsInMeleeAsAttacker_00662410.cpp | ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (const) | CanBeReadyToAttack | WRONG |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (returns CWorld&, const) | GetPWorld returning CWorld* | WRONG |
| 0x006EA450 | 00/6e/CCombatManager_SetAttackerAsReadyToAttack_006ea450.cpp | ?SetAttackerAsReadyToAttack@CCombatManager@@QAEXABVCThingCreatureBase@@_N@Z (const CThingCreatureBase&, bool), ret 8 | takes CThingCreatureBase* | WRONG type |
| 0x00A01B10/1B50/1B90/1C10 | 00/a0/CBaseIntelligentPointer_* | ctor / GetPItem (QBE..PAVCBaseObject) / SetPItem(CBaseObject*) / virtual dtor | ok names | via engine/CIntelligentPointer.h |
| 0x008FD010 | 00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp | ?IsStopping@CAIStateGroupBase@@IBE_NXZ (protected const) | non-const | minor |
| 0x0088B8F0 | 00/88/CAIBrain_SetOutput_0088b8f0.cpp | ?SetOutput@CAIBrain@@QAEXABVCBrainOutput@@@Z | SetBrainOutput | WRONG |
| vslot 2 (+8) of PBrain | 0x00A44840 | ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (bool) | Yield (void) | WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition | WRONG |
| 0x00BFE9BC | 00/bf/global_operatordelete_00bfe9bc.cpp | operator delete | ok | |
| 0x00666D10 | 00/66/CThingCreatureBase_GetCombatTypeDef_00666d10.cpp | GetCombatTypeDef() const -> const CCombatTypeDef& (CThingCreatureBase.h:98 under FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE) | returns pointer, non-const | WRONG |
| 0x00972340 | 00/97/CAIStateGroupJackOfBladesStage1_HasHitsCausedRush_00972340.cpp | ?HasHitsCausedRush@CAIStateGroup_JackOfBladesStage1@@IAE_NXZ (protected) | ok | not declared in the Stage1 shared header |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | ok | |
| (inlined) 0x00971B00 | 00/97/CAIStateGroupJackOfBladesBase_IsStillRunnable_00971b00.cpp | ?IsStillRunnable@CAIStateGroup_JackOfBladesBase@@QAE_NXZ | open-coded in draft | retail INLINES it twice |
| (inlined) 0x0093E760 | 00/93/CBrainOutput_SetAsLookAtThing_0093e760.cpp | CBrainOutput::SetAsLookAtThing(const CThing&,long) | (CThing*,long) | param type |

## 4. Members
- this +0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS); +0x10 Terminated (via Terminate).
- +0x1c PArenaCentre CIntelligentPointer<CThing>, +0x24 PTargetThing CIntelligentPointer<CThingCreatureBase>
  (CAIStateGroup_JackOfBladesBase.h, FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE). Draft models PTargetThing as <CThing> -- wrong template arg.
- CAIBrain +0x18 `Creature` (CThingAICreature*) -- engine/CAIBrain.h, GetCreature via FABLE_RETAIL_CAIBRAIN_GETCREATURE; SetOutput via
  FABLE_RETAIL_CAIBRAIN_SETOUTPUT; vslot 2 via FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE (base CSuspendableProcessMicrothread). Draft calls it PCreature.
  CAIStateGroupBase::GetCreature() inline (CAIBrain.h:121, FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE).
- CWorld +0x4c `PThingCombatManager` (scoped_ptr<CCombatManager>, struct_layouts_egor) -- draft "CombatManager". engine/CWorld.h + FABLE_RETAIL_CWORLD.
- CThing +0x91 bit0 = Dead (CThing.h:280 IsDead()); use IS_THING_ALIVE from engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE).
- CCombatTypeDef +0x70 = `FrontMiddleLimit` (engine/CCombatTypeDef.h:45, FABLE_RETAIL_CCOMBATTYPEDEF) -- draft "Range" WRONG.
- CBrainOutput (Ego_r 0x48): Types, LookFlags, NavigationInput, PAction CCountedPointer<CCreatureActionBase>, MoveToPos, LookAtPos,
  LookAtThing CIntelligentPointer<const CThing>. Only declaration-level shared spelling is engine/campfire_types.h (private members,
  SetAsPumpCombatActions = Types|=0x20 per PDB method list); every landed user (0x0093DDE0, 0x0097D6F0) models CBrainOutput/CNavigationInput LOCALLY.
- Draft-local classes that HAVE shared headers: C3DVector (engine/C3DVector.h), CThing, CThingCreatureBase, CCombatTypeDef, CBaseIntelligentPointer /
  CIntelligentPointer (engine/CIntelligentPointer.h), CCountedPointer (engine/CCountedPointer.h, opt-ins FABLE_RETAIL_CCOUNTEDPOINTER,
  _RELEASEDATA, _VISIBLE_CALLED_DTOR ...), CCombatManager, CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_JackOfBladesBase/Stage1.

## 5. Visibility
- IsStillRunnable (out-of-line copy 0x971B00) is expanded inline here twice -> same TU, visible. The parked Update draft already
  defines `inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable(){return IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre);}`
  ("native971b00, also expanded at the combat-loop latch"). Header declares it under FABLE_RETAIL_JACK_STAGE1_UPDATE (protected; PDB says public QAE).
- HasHitsCausedRush 0x972340 is same TU, defined earlier, but called OUT of line (142 bytes; not inlined).
- CBrainOutput ctor / SetAsLookAtThing / dtor all inline here (unlike 0x0097D6F0 which calls 0x835620/0x93E760 out of line).
- CIntelligentPointer ctor/dtor/GetPItem/SetPItem out of line (0xA01Bxx) -- default FABLE_RETAIL_CINTELLIGENTPOINTER behaviour.
- 2D distance: C3DVector.h offers GetMagnitude2D under FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D (`GFFastSqrt(X*X+Y*Y)`, needs `#pragma intrinsic(sqrt)`)
  or FABLE_RETAIL_UPDATE_NEAREST_INTERESTING (running-statement form); operator- has FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP. Likely source:
  `(PTargetThing->GetPos() - GetCreature().GetPos()).GetMagnitude2D()`; the unused 8-byte-aligned frame slot (local+0x10, and esp,-8) may be the dead C3DVector/temp.

## 6. Siblings
- 0x0093DDE0 CAIStateGroup_MeleeCombatNearby2::AttackEnemy (00/93/...AttackEnemy_0093dde0.cpp) -- landed, near-identical shape (same and esp,-8 frame,
  IsInMeleeAsAttacker + SetAttackerAsReadyToAttack prologue, output Types|=0x20, SetAsLookAtThing(...,0x11), SetOutput, SuspendSuspendableProcess);
  local models of CBaseIntelligentPointer/CCountedPointer (FABLE_FORCEINLINE dtor + Reset)/CNavigationInput/CBrainOutput; no pragmas. COPY ITS CBrainOutput MODEL.
- 0x00971ED0 Stage1::GetRushToPosForLunge (Flags: none): shared-header idiom -- FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GOTOBED_WORLD,
  _CAISTATEGROUP_JACKOFBLADESBASE, CINTELLIGENTPOINTER, _GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT, CTHING, CTHINGPHYSICAL, CTHINGGAMEOBJECT,
  CTHINGCREATUREBASE, CWORLD, IS_THING_ALIVE; uses GetWorld(), IS_THING_ALIVE(PTargetThing), PTargetThing->GetPos().
- 0x00972340 Stage1::HasHitsCausedRush (CVectorMap.h, `#pragma auto_inline(off/on)`), 0x00972280 Stage1::IsRunnable, 0x00971B00 Base::IsStillRunnable (local models).
- Parked 0x00975BB0 Update: full FABLE_RETAIL_JACK_STAGE1_UPDATE define list incl. CAIBRAIN_GETCREATURE, CTHINGCREATUREBASE_COMBATTYPE, CCOMBATTYPEDEF,
  C3DVECTOR_MAGNITUDE_SQRTF, C3DVECTOR_NORMALISE_SQUARE_HELPER, `#pragma intrinsic(sqrt)`.

## 7. Draft model errors
- Names: CanBeReadyToAttack->IsInMeleeAsAttacker, GetPWorld->GetWorld (CWorld&), SetBrainOutput->SetOutput, Yield->SuspendSuspendableProcess (bool),
  GetPosition->GetPos, Range->FrontMiddleLimit, CombatManager->PThingCombatManager, PCreature->Creature; return type long -> EAIActionReturn (AI_ACTION_FINISHED).
- SetAttackerAsReadyToAttack takes const CThingCreatureBase& (draft pointer: same bytes but wrong spelling); GetCombatTypeDef returns const&.
- IsStillRunnable should be called (inline def), not open-coded IS_THING_ALIVE chains; PTargetThing is CIntelligentPointer<CThingCreatureBase>.
- Retail residue (notes, 3 tries): vptr 0x01237F1C held in ebp, frame 0x58 (+4 bytes: an extra dead temp, likely a C3DVector difference / 8-aligned temp),
  Types |= 0x20 as memory `or`, this/zero register swap. Draft's `__declspec(align(8))` on CBrainOutput is a hack: Ego_r has no such alignment;
  sibling 0x93DDE0 gets and esp,-8 without it (from its 8-byte look-at temporary).
- The separate return-2 epilogue for the HasHitsCausedRush-true exit vs the shared one for the other exits suggests the rush exit is a distinct `return` statement.

