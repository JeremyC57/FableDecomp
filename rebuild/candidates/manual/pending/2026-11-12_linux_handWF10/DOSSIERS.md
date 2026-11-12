## 0x00808E40

# DOSSIER 0x00808E40 -- (anon)::CThingFilter_IsValidTargetForPlayerCreature::operator()(const CThing&) const

## 1. Retail shape (python3 $S/w17/xdis.py 00808e40 0x1f8)
- 488 bytes, __thiscall, `ret 4`, bool in al. No frame pointer: `push ecx ; push ebp ; mov ebp,ecx` -- ebp = `this`
  for the whole body (NOT /Oy-; ebp is just a callee-saved register). No SEH prologue although two CCharString
  temporaries live here (no EH state; same as the sibling ctor 0x00805890) -> no /EHsc needed.
- Frame: ONE 4-byte local ([esp] after `push ecx`) = the GetClassTypeName() return CCharString; everything else uses
  the dead `thing` ARGUMENT SLOT: every GetTC key (`mov [arg],0Dh/54h/36h/09h`) and the
  "CTCActionUseWindow" CCharString (`lea ecx,[esp+20h]` after push ebx = arg slot). `thing` lives in edi.
  -> the old draft's "this spilled" residue = draft lost ebp-for-this; retail has 3 saves (ebp, esi, edi) + ebx only
  inside the class-name block (`push ebx` @808eb2 / `pop ebx` @808f1c).
- Early exit before esi/edi pushes: `if (PTargetOnlyThing) return PTargetOnlyThing == &thing;` (sete al).
- Order of tests (retail):
  1. PlayerIsChild(+1): [edi+24h]&40h (TCI_DOOR 38) -> false; [edi+24h]&200000h (TCI_BED 53) -> false (ONE load, two
     tests); [edi+20h]&2000h (TCI_ON_ACTION_USE 13) -> GetTC(13) expanded, then
     `GetClassTypeName() == CTCActionUseWindow::StatGetClassTypeName()`: the Stat literal string 0x0123A16C is built
     FIRST (call 0x0099EBF0 push -1), then vslot +0x58 (slot 22, CTCBase::GetClassTypeName, hidden-ret ptr = [esp+10h]),
     then inline CCharString::operator== (ptr-equal -> bl=1; either null -> 0; StringLength +4 differ -> 0; else
     `Compare(left->PData,right->PData)` 0x00411570 `neg;sbb;inc`), then dtor result, dtor literal; `test bl` -> false.
     This is exactly CThing::HasTC<CTCActionUseWindow>() (CThing.h line ~312, gated by FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT).
  2. [edi+28h]&100000h (TCI_STOCK_ITEM 84): GetTC(84) -> esi=stock; `GetShopTC()` 0x007F4FC0 nonzero ->
     if (!Stealable +0x0D) false; if (!AcceptStealableStockItems [ebp+0]) false.
  3. `thing.GetType()==13` (byte cmp [edi+10h]) && PlayerCreature([ebp+4])->HasTC(TCI_STEALTH 54: [eax+24h]&400000h)
     && stealth->InDangerCoolOffTimer(+0x34) >= 0 (`jge` -> false). Likely the inline CTCStealth::IsInDanger()
     (FableWin ?IsInDanger@CTCStealth@@QBE_NXZ; header declares it out of line, retail inlines it).
  4. `if (!byte_013B85F4 || !PlayerCarryingWeapon[+2]) return true;`
  5. ThingTypeFlags +0x6C & 8 -> `return NTargeting::IsThingVulnerableToMeleeStrikesFromThing(thing,*PlayerCreature)`
     (fastcall ecx=edi, edx=[ebp+4]; nonzero -> shared `mov al,1` tail; zero -> its own `pop;pop;pop;pop;ret 4` with al=0
     still from the call -- note a 3rd epilogue copy at 808fd9).
  6. else [edi+20h]&200h (TCI_TARGETED 9): GetTC(9)->TargetingFlags(+0x0C) & 1Ch == 0 -> false; else/no TC -> true.
- Two shared tails: `xor al,al` @809016 (all false returns), `mov al,1` @80901f (true).
- GetTC expansion each time: `lea esi,[thing+44h]; mov ecx,esi; push &key; call 0x004365B0; mov esi,[esi+4]` (end
  read AFTER the call through the map pointer), `cmp eax,esi; je; cmp [eax],key; jle; mov eax,esi`, `->second`
  = FABLE_RETAIL_CTHING_GETTC_EXPANDED shape (pointer-to-map local, key copy, `?:`).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00802660 and egor_signatures.tsv):
  `??RCThingFilter_IsValidTargetForPlayerCreature@?A0x58ed6966@@QBE_NABVCThing@@@Z`
  = `bool (anonymous namespace)::CThingFilter_IsValidTargetForPlayerCreature::operator()(const CThing&) const`.
  Also present in FableWin.exe.signatures.tsv (0x018284D0 / 0x024DB2F0).
- Landed caller GetAllThingsInRoughArea<..CFilter_Bind5<..,CThingFilter_IsValidTargetForPlayerCreature>> 0x00809A70
  (rebuild/src/compiled/00/80/GetAllThingsInRoughArea..._00809a70.cpp line 100) declares it
  `bool operator()(const CThing&)const; // 0x00808e40`. xref.py: called from 0x0080950F and 0x00809C14.
- Manifest label class/method is RIGHT; the draft's parameter is WRONG: `const CThing&`, not `CThing*` (same code,
  but `PTargetOnlyThing == &thing`). Class is in the anonymous namespace of the NPlayerTargeting/TCTargetingPlayer TU
  (_anon_58ED6966; same ns as CTargetPointFilter_IsStealableStockItem 0x00809270, CGetBestTargetInDirection 0x805A90).
- Layout (struct_layouts_egor.tsv, size 12): AcceptStealableStockItems bool +0, PlayerIsChild bool +1,
  PlayerCarryingWeapon bool +2, PlayerCreature CThingPlayerCreature* +4, PTargetOnlyThing CThing* +8 (PDB ctor takes
  `const CThingPlayerCreature&, const CThing*` -> the members are probably const pointers in source).

## 3. Callees
| addr | landed row | landed / PDB name | draft spelling |
|---|---|---|---|
| 0x004365B0 | rebuild/src/compiled/00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) const | draft `CInterfaceMap::LowerBound` -- local stand-in, use engine/CVectorMap.h |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long=-1) | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | ok |
| 0x00411570 | 00/41/CBasicStringchar_Compare_00411570.cpp | ?Compare@?$CBasicString@D@@KIJPBD0@Z (static protected __fastcall CBasicString<char>::Compare) | draft free `Compare` -- WRONG, use fable_string.h |
| 0x007F4FC0 | 00/7f/CTCStockItem_GetShopTC_007f4fc0.cpp | CTCStockItem::GetShopTC() const | ok |
| 0x007F59F0 | 00/7f/NTargeting_IsThingVulnerableToMeleeStrikesFromThing_007f59f0.cpp | ?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z (namespace NTargeting, const CThing& x2) | draft global with pointers -- WRONG namespace/types |
| vslot +0x58 (22) | -- | CTCBase::GetClassTypeName() const (CTCBase.h:101, pure virtual 22) | draft `CTCAction::GetClassName` -- WRONG |
| data 0x013B85F4 (bool) | no name found anywhere (grep ghidra_out/rebuild/refs/docs; xref: only this function reads it) | made-up needed; mark descriptive | draft g_bMeleeTargetCheck (made up) |
| literal 0x0123A16C | "CTCActionUseWindow" via CTCActionUseWindow::StatGetClassTypeName() (engine/CTCActionUseWindow.h) | | draft inline literal |

## 4. Members read (retail TLC layout, shared headers)
- this: the 5 filter members above (no shared header; local class in anon namespace, like siblings -- OK to define locally).
- CThing (engine/CThing.h, retail layout): Type +0x10 is CClass::Type (engine/CClass.h, `GetType()` inline FableWin
  ?GetType@CClass@@QBEKXZ); UsingComponents std::bitset<274> +0x20; Components CVectorMap +0x44; ThingTypeFlags
  unsigned long +0x6C (protected in header). Draft's local CThing (ThingType/m_presentBits/m_interfaces/ThingTypeFlags)
  must be replaced by engine/CThing.h.
  Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC/PeekTC(id,T**) expanded LowerBound form),
  FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (forceinline HasTC -> in-place `test dword ptr`),
  FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT (forceinline HasTC + template HasTC<T>() = presence+GetTC+GetClassTypeName==Stat),
  FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN, FABLE_RETAIL_CTHING (forceinline GetTC(type)).
- TC ids (engine/ETCInterfaceType.h): TCI_TARGETED 9, TCI_ON_ACTION_USE 13, TCI_DOOR 38, TCI_BED 53, TCI_STEALTH 54,
  TCI_STOCK_ITEM 84. Draft's "CTCHeroStats (0x36)" is WRONG -> CTCStealth; draft's "CTCPhysicsInfo (9)" WRONG -> CTCTargeted;
  draft's "CTCAction (0x0d)" -> CTCActionUseBase / HasTC<CTCActionUseWindow>.
- CTCStockItem (engine/CTCStockItem.h): Stealable bool +0x0D (ForSale +0x0C, Initialised +0x0E, Price +0x10).
- CTCStealth (engine/CTCStealth.h): InDangerCoolOffTimer long +0x34 (header line 121); IsInDanger() const declared
  (line 94) out-of-line only; opt-ins in header: FABLE_RETAIL_STEALTH_DRAW, FABLE_RETAIL_CTCSTEALTH_SETSNEAKING,
  FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE (none gives IsInDanger inline -- write the member test or add an opt-in).
- CTCTargeted (engine/CTCTargeted.h): TargetingFlags unsigned long +0x0C; FABLE_RETAIL_CTCTARGETED gives
  GetTargetingFlags() inline (also IsZTargetable two-return idiom note).
- CTCActionUseWindow.h: StatGetClassTypeName() FABLE_FASTCALL returns CCharString("CTCActionUseWindow",-1);
  CTCActionUseBase.h: StatGetInterfaceType() = TCI_ON_ACTION_USE.
- CCharString (rebuild/include/fable_string.h, FABLE_RETAIL_CCHARSTRING): member operator==(const CCharString&) line 341;
  FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ forces it AND the CBasicString<char> length-then-Compare body inline
  (documented as producing exactly `call Compare ; neg ; sbb ; inc`, chunk 795 row 0x005E78F0).
- Locally modelled in the draft but with shared headers: CThing, CThingPlayerCreature, CCharString/CCharStringData/
  CBasicString, CVectorMap (CInterfaceMap), CTCStockItem, CTCStealth (as CTCHeroStats), CTCTargeted (as CTCPhysicsInfo),
  CTCBase/CTCActionUseBase (as CTCAction).

## 5. Callee visibility
- LowerBound 0x004365B0: out of line in every caller (CVectorMap.h line 6); do NOT define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND.
- HasTC / GetTC / HasTC<T> / CCharString::operator== / CBasicString::operator== / Stat* / IsInDanger: all inline in
  retail here (no call). Need forceinline variants (macros above) -- 488-byte body may exhaust the inline budget.
- CCharString ctor/dtor, Compare, GetShopTC, IsThingVulnerable...: out of line (other TUs / library).
- Same TU, earlier addresses: ctor 0x00805890, CGetBestTargetInDirection 0x008059F0/0x00805A90,
  CThingFilter_ViableProjectileAimingTarget 0x008077B0 -- none called here.

## 6. Siblings (same anon namespace / neighbourhood)
- 00/80/CThingFilterIsValidTargetForPlayerCreature_CThingFilterIsValidTargetForPlayerCreature_00805890.cpp (ctor; same
  struct, local model, no pragmas; CCharString temp in the dead arg slot).
- 00/80/CTargetPointFilterIsStealableStockItem_operator_00809270.cpp -- SAME stock-item test: HasTC as
  `if (bit) return true; return false;`, GetTC with key homed in the dead arg slot, `it==end||type<it->first ? end : it`,
  end read after lookup; `#include "fable_compiler.h"`, local `namespace { }`.
- 00/80/CThingFilterNoiseCameFromThing_operator_00809210.cpp -- `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`,
  `#include "engine/CThing.h"`, engine/CVectorMap.h.
- 00/80/CThingFilterIsInTargetingRangeAndFOV_operator_008090e0.cpp (right after this row; `#include "engine/CThing.h"`, inline IS_THING_ALIVE).
- 00/80/CTCTargetingPlayer_GetPBestThingForMeleeAttack_008031e0.cpp: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCTARGETED,
  FABLE_RETAIL_CTHING, FABLE_RETAIL_PLAYER, FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_CTHING_PEEKPLAYER ...
- 00/80/CTCTargetingPlayer_UpdateTargetList_00804020.cpp (constructs this filter): `// Flags: /d2inlT68`,
  FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN, FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS.
- 00/80/CThingFilterViableProjectileAimingTarget_operator_008077b0.cpp uses `#pragma auto_inline(off/on)`.

## 7. Old draft model errors
- Parameter `CThing*` -> must be `const CThing&` (Ego_r mangled leaf); filter is in anonymous namespace.
- Wrong component classes (HeroStats/PhysicsInfo/CTCAction) and wrong callee spellings (free Compare,
  global IsThingVulnerable..., GetClassName) -- see tables.
- The class-name test should be HasTC<CTCActionUseWindow>() (or its spelled-out equivalent with the Stat literal
  constructed before the virtual call -- draft got the order by luck as a free operator==).
- Retail keeps `this` in ebp and uses only 1 local + the arg slot; draft spilled `this` to the frame (507 v 488).
  Likely fix: fewer named temporaries (key copies inside forceinline GetTC so they home into the dead `thing`
  slot), `this` member reads directly, and real header inlines; the draft's local `HasTC` returned `unsigned long`
  (masked) rather than bool.
- Melee branch: `return NTargeting::IsThingVulnerableToMeleeStrikesFromThing(thing, *PlayerCreature);` (draft OK in
  shape, wrong types). Stealth test probably `thing.GetType()==13 && PlayerCreature->HasTC(TCI_STEALTH) &&
  stealth->IsInDanger()` with IsInDanger inline as `InDangerCoolOffTimer >= 0` (unproven body; only FableWin name).
- Global 0x013B85F4 unnamed in every source; keep a descriptive made-up name and mark it in NOTES.

## 0x0070CE10

# DOSSIER 0x0070CE10 -- CAnimSet::UpdateDistanceMoved(float)  (495 bytes)

## 1. Retail shape (xdis 0070ce10 0x1ff)
- __thiscall (this in ecx -> esi), `ret 4`, one float arg. NO frame pointer (`sub esp,10h; push ebx/ebp/esi/edi`), no SEH
  (the float vector temp is not destroyed under EH -> no /EHsc unwind frame needed: body has no throwing callee after the ctor... retail
  simply has none). Default /O2-ish code (not /Os: loops use `jb` back-edges with per-iteration size reloads, fcom idioms).
- Frame: 0x10 bytes locals: [local+0] = the 0.0f fill-value temp (`mov [esp+1Ch],0` after 3 pushes), [local+4..0xF] = the
  vector<float> speeds (12 bytes). The ALLOCATOR temp is passed as `lea eax,[esp+24h]` = the ARG slot of `distance` (empty temp
  aliased onto the param slot). The arg slot is later reused as scratch for the new percentage (`fstp [esp+24h]; mov edx,[esp+24h];
  mov [esi+1Ch],edx`) and read as the inline callee's parameter (`fld [esp+24h]; fsub 1.0`).
- Call 0x004516E7 = out-of-line STLport vector<float>(size_type n, const float& value, const allocator&) (ret 0Ch) with
  n = Anims.size() (`(finish-start) sar 1`). So retail used the fill ctor COMDAT, not the inline body.
- After the ctor, speeds._M_start is cached in ebp for the whole function (including the dtor: `test ebp,ebp; je; push ebp;
  call free 0x00BFEA14; add esp,4`) -- the inline ~vector -> _Vector_base dtor -> allocator::deallocate -> free.
- Loop 1 (edi=i, bl=noMovement flag = 1): guarded `test eax,eax ; jbe` then `do{}while(i < size)` with size re-read each
  iteration ([esi+8]-[esi+4] sar 1). For Anims[i] != 0 (`mov ax,[ecx+edi*2]; test ax,ax`): global CMainGameComponent ->+0x24 World
  ->+0x3c PMainMeshBank -> PeekAnimation((long)(short)anim) 0x00A242C0; `add eax,18h` then fld Z,Y,X (a by-value C3DVector copy of
  MovementVector at +0x18), X*X+Y*Y+Z*Z, fsqrt, `* 0.01f` (0x01230A5C), `fstp [ebp+edi*4]` then RELOAD `fld [ebp+edi*4]` and
  `fcomp 0.0f` (0x0122DEDC): if speeds[i] > 0 -> bl = 0.
- if (bl) change = 0.1f (0x0122EE58)  [also taken directly when size == 0].
- else Loop 2: total = 0.0f (fld kept on x87), eax=i, ecx walks Anims pointer (+2 per iter, `cmp word [ecx],0`), size re-read each
  iteration; if (Anims[i]) total += InternalAlphas[i].CurrentAlpha ([esi+10h] reloaded each iter, `fld [edx+eax*8]`) * speeds[i]
  (`fmul [ebp+eax*4]`) -- alpha first, then speed.
- `fcom DELTA_NOTIONAL_ZERO (0x0129BA3C)`: if total > DELTA -> change = distance / (total * PAnimator->Scale) (`mov eax,[esi+3Ch];
  fmul [eax+88h]; fdivr [esp+24h]`) else change = 0.0f (`fstp st0; fld 0.0`).
- Tail (looks like an INLINED `UpdatePercentageCompleteTo(float)`): `mov ecx,[esi+1Ch]` (Current, int copy to Last [esi+20h]);
  `fst [esi+34h]` LastUpdateChange = change; `fadd [esi+1Ch]` -> new = change + Current stored to arg slot then int-copied to
  [esi+1Ch]; flags byte [esi+51h] tested (al&1 Looping):
  * Looping: if (Last > 1.0f) {Last -= 1; Current = new - 1;} else if (Last < 0.0f) {Last += 1; Current = new + 1;}
    (new is re-read from the arg slot = inline callee's parameter, NOT Current).
  * Not looping: if (Current < 0) Current = 0 (int store 0); else if (Current > 1) Current = 1.0f (int store 0x3F800000);
    then if (1.0f - DELTA_NOTIONAL_ZERO <= Current) `or byte [esi+51h],2` (Finished = true). Same idiom as landed UpdateTimePassed.

## 2. Identity (CONFIRMED, manifest label is right)
- Ego_r.pdb: ghidra_out/egor_pdb_names.tsv 0x00576690 `?UpdateDistanceMoved@CAnimSet@@AAEXM@Z`;
  FableWin.pdb: ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv 0x01E9BFF0 (debug, 1093 bytes).
- Retail address corroborated by caller CAnimSet::Update 0x0070D030 (landed rebuild/src/compiled/00/70/CAnimSet_Update_0070d030.cpp,
  call at 0x0070D065 is the only xref; mode 2 = distance arm) and Ego_r donor order (UpdateDistanceMoved 0x576690 just before
  Update 0x5768a0; retail 0x70CE10 before 0x70D030).
- Signature: `private: void __thiscall CAnimSet::UpdateDistanceMoved(float)` (AAEXM: private, non-const, void, float by value).
- Not virtual (no vtable slot).

## 3. Callees
| target | what | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|---|
| 0x004516E7 | vector<float> fill ctor (n,const float&,const alloc&) COMDAT | rebuild/src/compiled/00/45/stdvector_vector_004516e7.cpp | std::vector<float>::vector | ??0?$vector@MV?$allocator@M@std@@@std@@QAE@IABMABV?$allocator@M@1@@Z | draft uses a local CFloatVector -- WRONG model; use std::vector<float> + FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE |
| 0x00A242C0 | CMeshDataBank::PeekAnimation(unsigned long) const | (referenced by landed UpdateTimePassed 0x0070C330) | CMeshDataBank::PeekAnimation | ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z (as used by siblings) | OK |
| 0x00BFEA14 | free (MSVCR71 import) | rebuild/src/compiled/00/bf/global_free_00bfea14.cpp | free | CRT | via ~vector, OK |
Inline (no call in retail), PDB-named:
- `CAnimSet::PeekAnimInfo(short) const` (Ego_r 0x005758B0 `?PeekAnimInfo@CAnimSet@@ABEABVC3DAnimationInfo@@F@Z`, FableWin 0x01E9B410):
  `return *CMainGameComponent::Get()->World->PMainMeshBank->PeekAnimation(anim)`; landed UpdateTimePassed models it with the bank in a
  named local. Draft open-codes it and uses `g_PMainGameComponent` / `PWorld` -- respell to CMainGameComponent::Get() (protected static
  ?PMainGameComponent@CMainGameComponent@@1PAV1@A).
- `C3DAnimationInfo::GetMovementDistance() const` (FableWin `?GetMovementDistance@C3DAnimationInfo@@QBEMXZ` 0x01D8C5A0, 22 bytes; no
  Ego_r copy => inline) and `GetMovementVector() const` (`?GetMovementVector@C3DAnimationInfo@@QBE?AVC3DVector@@XZ`): retail loads a
  3-float copy (Z,Y,X order) => likely `GetMovementVector().GetMagnitude()`. Draft's `C3DVector::GetLength` does NOT exist in either PDB;
  the PDB name is `?GetMagnitude@C3DVector@@QBEMXZ` (engine/C3DVector.h line ~271/273, FABLE_VECTOR_INLINE).
- `CAnimateThing::GetThingScale() const` (FableWin `?GetThingScale@CAnimateThing@@QBEMXZ`) returns Scale at retail +0x88. Draft calls
  the member `DistancePerCycle` -- WRONG; it is CAnimateThing::Scale (private) -> use GetThingScale().
- `CAnimSet::UpdatePercentageCompleteTo(float)` (Ego_r 0x00575250 out of line `?UpdatePercentageCompleteTo@CAnimSet@@AAEXM@Z`, FableWin
  0x01E9BA00) -- retail tail strongly suggests it is inlined here (the wrap/clamp reads the parameter slot). NOTE the FableWin debug copy
  is only 34 bytes/14 instr, so in source it may be just `Last = Current; Current = p;` with the wrap/clamp in UpdateDistanceMoved itself
  (compare landed UpdateTimePassed, whose wrap/clamp is written inline in its own body). Either way: model the tail so that the wrap
  branches use the new value (param/local), e.g. `CurrentPercentageComplete = newval - 1.0f`.
- Globals: CMainGameComponent::PMainGameComponent 0x013B86A0; DELTA_NOTIONAL_ZERO 0x0129BA3C (?DELTA_NOTIONAL_ZERO@@3MB, 1e-4f);
  literals 0.01f 0x01230A5C, 0.1f 0x0122EE58, 0.0f 0x0122DEDC, 1.0f 0x0122DED8 (unnamed literal pool).

## 4. Members (retail offsets = PDB - 8 after the two 12-byte STLport vectors; struct_layouts_egor.tsv)
CAnimSet (shared header rebuild/include/engine/CAnimSet.h, retail 0x54, members PRIVATE, CAnimateThing friend):
- +0x04 Anims `std::vector<short>` (PDB +4); +0x10 InternalAlphas `std::vector<CTransitionBlendAlpha>` (PDB +0x14; elem 8 bytes,
  CurrentAlpha +0 used); +0x1C CurrentPercentageComplete float; +0x20 LastPercentageComplete; +0x34 LastUpdateChange; +0x3C PAnimator
  `CAnimateThing*`; +0x51 bitfield byte: Looping bit0, Finished bit1.
- Opt-ins in CAnimSet.h: only FABLE_RETAIL_LOOK_HEAD_ANIM / FABLE_RETAIL_CANIMSET_FRAME (GetFrame/GetAnimIndex) -- NONE declares
  UpdateDistanceMoved, UpdatePercentageCompleteTo or PeekAnimInfo. A new opt-in (declaration + inline PeekAnimInfo /
  UpdatePercentageCompleteTo) would be needed to use the shared header; all landed CAnimSet rows (0x0070C330, 0x0070CC70,
  0x0070CAD0, 0x0070C590, 0x0070D030) still model CAnimSet LOCALLY ("Layout: no engine/CAnimSet.h" comments are stale -- the header
  exists now with FABLE_STATIC_ASSERT(sizeof==0x54)).
CAnimateThing (rebuild/include/engine/CAnimateThing.h, retail 0xBC): Scale at retail +0x88 (PDB +0xB0; CCompleteAnimation 0x60 +
  list 4 + DelayedAnim 4 + 4 C3DAnimationBlendState*8 = 0x88). No GetThingScale opt-in in the header (needs adding).
C3DAnimationInfo (engine/C3DAnimationInfo.h): MovementVector C3DVector +0x18 (generated branch spells MovementVector_X/Y/Z; retail
  branch FABLE_RETAIL_C3DANIMATIONINFO(_BUILD) has GetMovementDistance()/GetMovementVector() DECLARED out of line -- would emit calls;
  needs an inline body opt-in). Landed UpdateTimePassed uses `#define C3DAnimationInfo C3DAnimationInfo_PDB` subclass trick.
CMeshDataBank.h: FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION exists (PeekAnimation decl). CWorld.h: PMainMeshBank via
  FABLE_RETAIL_CWORLD_MESH_BANK (GetMeshBank()). CMainGameComponent.h: Get() / PMainGameComponent, World is boost::scoped_ptr PWorld.
Draft models locally (all have shared headers): C3DVector, C3DAnimationInfo, CMeshDataBank, CWorld, CMainGameComponent, CAnimateThing,
  CAnimSet, CTransitionBlendAlpha (defined in CAnimSet.h), plus fake CFloatVector/CShortVector/CAlphaVector (use stlport vectors).

## 5. Callee-body visibility
- vector<float> fill ctor: retail CALLS the COMDAT 0x004516E7 -> `#define FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE` (fable_containers.h
  line ~374; precedent COpinionSet::COpinionSet 0x004538EB, which says "value and allocator temporaries in the caller's frame").
- ~vector, size(), operator[]: inline (expanded). PeekAnimation 0x00A242C0 is in another TU (mesh bank) -> plain decl.
- Same-TU neighbours (CAnimSet.cpp, defined earlier in retail order): SetAlpha 0x0070BFC0, SetInternalAlpha 0x0070C280,
  UpdateInternalAlphas 0x0070C2C0, UpdateTimePassed 0x0070C330, GetInterpolatedTime 0x0070C800, GetMeshAnim 0x0070C900, GetLastMeshAnim
  0x0070CAD0, GetStartMeshAnim 0x0070CC70; after: Update 0x0070D030, Init 0x0070D1A0, AddAnim 0x0070D2F0. UpdatePercentageCompleteTo
  has NO retail out-of-line copy found (none landed / not in manifest) -> expanded inline here, consistent with the disassembly.

## 6. Sibling rows (idioms)
- rebuild/src/compiled/00/70/CAnimSet_UpdateTimePassed_0070c330.cpp -- closest twin: same wrap/clamp tail, `float& current =
  CurrentPercentageComplete;` trick to get `or byte [esi+51h],2`, inline PeekAnimInfo with bank local, CMainGameComponent::Get(),
  extern const float DELTA_NOTIONAL_ZERO, local CAnimSet with bitfields. No pragmas.
- CAnimSet_Update_0070d030.cpp (caller; local CRetailVector model, no pragmas).
- CAnimSet_AddAnim_0070d2f0.cpp, CAnimSet_Init_0070d1a0.cpp: `#pragma auto_inline(off)` ... `(on)` around helper bodies.
- stdvector_vector_004516e7.cpp / COpinionSet_COpinionSet_004538eb.cpp: fill-ctor COMDAT model.

## 7. Old draft model errors
- Local CFloatVector with a 3-arg ctor declared + explicit `CFloatAllocator()` temp: the allocator temp got its own frame byte (475 vs
  495). Retail aliases it onto the param slot -> try real `std::vector<float> speeds(Anims.size(), 0.0f);` (default-arg allocator) with
  FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE and the real STLport _Vector_base/_STLP_alloc_proxy layout.
- `float* speedData = speeds._M_start` local: retail does store-then-reload `speeds[i] = ...; if (speeds[i] > 0.0f)` through operator[].
- Wrong names: GetLength (-> GetMagnitude / GetMovementDistance), DistancePerCycle (-> Scale via GetThingScale()), g_PMainGameComponent /
  PWorld (-> CMainGameComponent::Get()->World / PWorld scoped_ptr per header), Movement (-> MovementVector).
- Movement read: draft reads info->Movement directly; retail copies all 3 floats first (Z,Y,X) => by-value GetMovementVector() then
  GetMagnitude(), likely wrapped as inline GetMovementDistance().
- Tail: draft writes `CurrentPercentageComplete = current - 1.0f` with `current` local -- right in spirit; retail spills new value to the
  ARG slot (param of inlined UpdatePercentageCompleteTo, or the reused `distance` slot). Model as inline
  `UpdatePercentageCompleteTo(CurrentPercentageComplete + change)` after `LastUpdateChange = change;` (note retail evaluates
  `change + Current`: fst change then fadd [Current]). Non-looping clamp needs the UpdateTimePassed `float&` trick for `or byte`.
- Loop 2 in retail walks Anims by pointer (strength reduced) while indexing InternalAlphas/speeds by i -- plain indexed loop is fine.

## 0x00949380

# DOSSIER 0x00949380 CAIStateGroup_GoToPub::ThankBarmaid (498 bytes)

## 1. Retail shape (xdis.py 00949380 0x202)
- __thiscall, no args, plain `ret` (0x00949442 and 0x00949571). Returns bool (al).
- NO frame pointer (`sub esp,0C8h` first; ebp is a general register = tankard). No SEH prologue (temporaries
  CCharString / PlayAnimation are destroyed inline, no EH state). Frame 0xC8.
- Push order: sub esp ; push ebx ; mov ebx,ecx(this) ; load [ebx+4] ; push ebp ; push esi ; ... push edi (edi pushed AFTER
  the creature load `mov esi,[eax+18h]` and HasTC read). `this` also spilled to [esp+18h] (0x0094939B) and reloaded for
  IsStopping (0x00949558).
- Each HasTC result is MATERIALISED into a stack byte [esp+13h] (`shr ecx,0Fh ; and cl,1 ; mov [esp+13h],cl ; je`) -- the
  `const bool&` HasTC shape. All three lookups share that byte. Key temp at [esp+14h].
- Miss paths read the uninitialised out-pointer from its frame word [esp+18h] (same word as the `this` spill):
  `mov edi,[esp+18h]` (talk), `mov esi,[esp+18h]` (scratchpad), `mov ebp,[esp+18h]` (tankard) -> out-param GetTC whose
  pointer is left uninitialised on miss (same as landed TellStory 0x0094A2B0 / EatAtTable::Update 0x0096CD70 notes).
- Map handling per lookup: talk `lea edi,[esi+44h] ; mov ecx,edi ... mov edi,[edi+4]`; scratchpad `add esi,44h` (creature reg
  reused); tankard `lea esi,[eax+44h]`. Final component load `mov reg,[eax+4]` from the ternary (end-or-greater -> end).
- Speech loop: 0x00949412 loop head reloads ThankingPhrase `[esi+0Ch]` each iteration; result==2 -> DoNothing, ==4 -> loop,
  else return false; result!=0 -> return false.
- Alive test of PTankard: GetPItem called twice (null test, then `test byte [eax+91h],1`), then a 3rd GetPItem for GetTC.
- LookAtThing: `push 3B360A13h` (0.0027777f) ; `push 16h` ; by-value CIntelligentPointer<const CThing> built in the arg area
  (`sub esp,8`, base ctor 0x00A01B10, vtable 0x01237F1C store, SetPItem 0x00A01B90 with the barmaid); `push 2` (repeat);
  result==1 -> false.
- PerformActionAndWait: `push 0` (wait=false) FIRST, then ctor 8 args (creature, &anim, &CSoundPair{0,0}, 0,1,0,0,0); result
  compared ==1 via `dec;neg;sbb;inc` into bl (bool kept in bl across the two dtors: ~CCreatureActionBase 0x00693EF0 then
  ~CCharString 0x0099EAE0). So: action temp destroyed first, string second (string named in an enclosing scope / declared first).
- Tail: SetPBarmaid(0) on tankard (ecx=ebp) ; IsStopping(this from [esp+18h]) ; `neg al;sbb al,al;inc al` = !IsStopping().

## 2. Name / signature (two sources)
- Ego_r.pdb: ghidra_out/egor_pdb_names.tsv `?ThankBarmaid@CAIStateGroup_GoToPub@@IAE_NXZ` (Ego_r copy at 0x0096D200);
  FableWin.pdb: ghidra_out/_fablewin_api.tsv + naming_stragglers/FableWin.exe.signatures.tsv same mangled leaf.
- rebuild/manifest/functions.tsv: 00949380 ThankBarmaid CAIStateGroup_GoToPub __thiscall bool. Manifest label is CORRECT.
- Leaf: `?ThankBarmaid@CAIStateGroup_GoToPub@@IAE_NXZ` = protected, non-virtual, non-const, `bool ThankBarmaid()`.
- Only caller: 0x0094A525 inside CAIStateGroup_GoToPub::ChooseAction 0x0094A3B0 (not landed). No vtable slot.

## 3. Callees (landed file / landed name / PDB)
| addr | landed row | PDB/header spelling | draft disagreement |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (engine/CVectorMap.h) | draft uses local CInterfaceMap::LowerBound(const long*) -- wrong class/mangling |
| 0x006E3B60 | 00/6e/CTCTalk_PlayAISpeech_006e3b60.cpp | CTCTalk::ESpeechPlaybackResult PlayAISpeech(unsigned long, bool, NSpeechGainManager::EDialogueLayer, bool, bool) (engine/CTCTalk.h, FABLE_RETAIL_CTCTALK) | draft returns long, layer long |
| 0x0097E200 | 00/97/CAIStateGroupBase_AIActionDoNothing_0097e200.cpp | EAIActionReturn AIAction_DoNothing() (IAE) | draft returns long |
| 0x00A01B10/50/90 | 00/a0/CBaseIntelligentPointer_{CBaseIntelligentPointer,GetPItem,SetPItem}_*.cpp | GetPItem returns CBaseObject*, SetPItem(CBaseObject*) (engine/CIntelligentPointer.h) | draft GetPItem returns CThing*, SetPItem(const CThing*) -- wrong mangling |
| 0x0097D6F0 | not landed | EAIActionReturn AIAction_LookAtThing(EAIActionRepeat, CIntelligentPointer<const CThing>, long, float) (CAIStateGroupBase.h:146) | draft long repeat / long return |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*, long) / ~CCharString (FABLE_RETAIL_CCHARSTRING) | ok |
| 0x008424F0 | 00/84/CCreatureActionPlayAnimation_CCreatureActionPlayAnimation_008424f0.cpp (`ret 20h`) | CCreatureAction_PlayAnimation(CThingCreatureBase&, const CCharString&, const CSoundPair&, bool,bool,bool,long,long) | draft takes CThingCreatureBase* (PDB is a reference) |
| 0x00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ~CCreatureActionBase (implicit PlayAnimation dtor: FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR) | draft declares ~CCreatureAction_PlayAnimation() bound to 0x693EF0 -- wrong symbol |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) | draft takes pointer, returns long |
| 0x0094B400 | 00/94/CTCTavernTankard_SetPBarmaid_0094b400.cpp (comment calls it CClass_0094b400::AssignTarget; manifest CTCTavernTankard::SetPBarmaid, `ret 4`) | CTCTavernTankard::SetPBarmaid(CThingAICreature*) per manifest/draft; verify mangled leaf in egor_pdb_names before use | arg type: manifest says CThing* |
| 0x008FD010 | 00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp | bool IsStopping() const `?IsStopping@CAIStateGroupBase@@IBE_NXZ` | draft non-const -- WRONG mangling |

## 4. Members
- this: CAIStateGroupBase::PBrain +0x04 (engine/CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS); CAIBrain::PCreature
  +0x18 (engine/CAIBrain.h; GetCreature via FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE / FABLE_RETAIL_CAIBRAIN_GETCREATURE).
- CAIStateGroup_GoToPub (struct_layouts_egor.tsv, sizeof 168): PPub +0x14 CIntelligentPointer<CThingBuilding>, PBar +0x1c,
  PTable +0x24 <CThingObject>, PBarman +0x2c <CThingAICreature>, PTankard +0x34 CIntelligentPointer<CThing>, SeatIndex +0x3c ...
  TimeBeforeAskingForBeer +0xa4. NO shared header engine/CAIStateGroup_GoToPub.h -> local class (as all landed GoToPub rows do).
- CThing: UsingComponents +0x20 (bitset; TCI_TALK=15 -> bit 15 of +0x20; TCI_AI_SCRATCHPAD=170 -> bit 10 of +0x34;
  TCI_TAVERN_TANKARD=186 -> bit 26 of +0x34), Components +0x44 (CVectorMap), Dead flag bit0 of +0x91 (IsDead()). engine/CThing.h.
- CTCAIScratchpad::ThankingPhrase +0x0c unsigned long (egor: CTCAIScratchpad 132/12 ulong) -- engine/CTCAIScratchpad.h line 30 (public).
- CTCTavernTankard::PBarmaid +0x34 CIntelligentPointer<CThingAICreature> (egor: CTCTavernTankard 60/52). NO engine header -> local.
- Draft models LOCALLY classes that have shared headers: CThing (+CInterfaceMap), CBaseIntelligentPointer/CIntelligentPointer,
  CCharString, CSoundPair, CCreatureActionBase, CCreatureAction_PlayAnimation, CTCTalk, CTCAIScratchpad, CAIBrain, CAIStateGroupBase.

## 5. Callee visibility / opt-in macros
- LowerBound out of line (non-inline in CVectorMap.h). GetTC expansion: FABLE_RETAIL_CTHING_GETTC_EXPANDED (ternary end-or-greater,
  map via pointer local, key copied inside the test) -- used by every landed GoToPub GetTC row.
- Stack-byte HasTC: FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE (`const bool& found=UsingComponents.test(type)`) -- landed EatAtTable::Update
  0x0096CD70 uses it WITH GETTC_EXPANDED for exactly this retail shape. (FreeBarPoint 0x00949320 tests in place -> no byte there.)
- IS_THING_ALIVE: engine/ThingAlive.h with FABLE_RETAIL_IS_THING_ALIVE (template IS_THING_ALIVE(const T&)), used by landed Spawn 0x0095B820.
- CIntelligentPointer converting ctor (base ctor + vtable store + SetPItem, inline): FABLE_RETAIL_CINTELLIGENTPOINTER.
- PlayAnimation: FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION + _IMPLICIT_DTOR (dtor = 0x00693EF0 directly, chunk Q9 row 0x0092B900).
- CSoundPair: engine/CSoundPair.h (FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS available).
- CTCTalk: FABLE_RETAIL_CTCTALK (PlayAISpeech + ESpeechPlaybackResult {STARTED=0, FAILED_FINAL=1, FAILED_RETRY=2}).
- AIAction_* decls: FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS / _PERFORMACTION (check which gates LookAtThing at line 146).
- No STLport map::operator[]/tree helpers involved.

## 6. Siblings (rebuild/src/compiled/...)
- 00/94/CAIStateGroupGoToPub_TellStory_0094a2b0.cpp: CCHARSTRING + GETTC_EXPANDED + CINTELLIGENTPOINTER; `thing->GetTC(TCI_.., &out)`
  out-param, miss leaves pointer in a stale frame word (same as here).
- 00/94/CAIStateGroupGoToPub_OnReplace_009498b0.cpp: fable_compiler.h, CCHARSTRING, GETTC_EXPANDED; block-scoped CCharString with the
  call result in a bool assigned inside the block (exactly the PerformActionAndWait shape needed here).
- 00/94/CAIStateGroupGoToPub_Laugh_00947d30.cpp: `AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation,
  CSoundPair(), false, true, false, 0, 0)) != 2` -- same action ctor arguments.
- 00/92/CAIStateGroupDrunk_TalkToSelfUpdate_00922750.cpp: `PBrain->PCreature->GetTC(TCI_TALK,&talk)` then `for(;;){ result=PlayAISpeech(..,false,2,false,true);
  if(result==2){ if(AIAction_DoNothing()==4) continue; ...} ...}` -- the retry loop idiom (RELOCATION_MATCH).
- 00/96/CAIStateGroupEatAtTable_Update_0096cd70.cpp: GETTC_EXPANDED + CTCWEAPONTRAIL_UPDATE (stack-byte HasTC) + IMPLICIT action dtor family,
  full shared-header include set (fable_string.h, stlport/fable_containers.h, engine/fable_ai_action_enums.h, CAIStateGroupBase.h, CAIBrain.h).
- 00/95/CAIStateGroupHoverHornetQueen_Spawn_0095b820.cpp: was DIFFER in hand258 with "same gap as 00949380 (map pointer coalescing)";
  now LANDED on shared headers (GETTC_EXPANDED, IS_THING_ALIVE, CINTELLIGENTPOINTER, CAISTATEGROUPBASE_*). Strong hint the
  hand258 register gap closes by moving to the shared CThing::GetTC + CVectorMap models.
- No `#pragma optimize` / `// Flags:` in any GoToPub sibling: default /O2 /Oy.

## 7. Old draft model errors
- Local CThing::GetInterface with `long` key and `const unsigned char& present` + `CInterfaceMap*` -- replace by shared
  GetTC(ETCInterfaceType, T**) under GETTC_EXPANDED + CTCWEAPONTRAIL_UPDATE (ternary form, ETCInterfaceType key, real
  LowerBound mangling). The hand258 residue (creature edi vs esi; map lea/add) is exactly the map-pointer coalescing that
  the shared ternary model fixed for Spawn 0x0095B820.
- Wrong types/manglings: IsStopping must be const; GetPItem returns CBaseObject*; PlayAISpeech/AIAction_* return enums;
  PlayAnimation ctor takes CThingCreatureBase&; PerformActionAndWait takes const CCreatureActionBase&; the 0x693EF0 dtor must
  be ~CCreatureActionBase (implicit dtor), not a declared ~CCreatureAction_PlayAnimation.
- Control flow is otherwise right: for(;;) speech loop with phrase reloaded each iteration, alive test via GetPItem x2, third
  GetPItem for GetTC, block-scoped CCharString with `failed` bool, SetPBarmaid(0), `return !IsStopping()`.

## 0x006E3580

# DOSSIER 0x006E3580 -- CTCTalk::PlaySpeech (bank overload)

## 1. Retail shape (xdis 006e3580 0x206)
- thiscall, `ret 0x1c` (7 stack words), both exits. 502 bytes, ends 0x6E3775 (int3 pad from 0x6E3776).
- NO frame pointer (`sub esp,2Ch ; push esi`, esp-relative). NO SEH frame. So no `optimize("y",off)`, no /Os idioms seen.
- Frame map (F = esp after `sub esp,2Ch`; arg1 = F+0x30):
  - F+0x00: `mov [esp+4],0` dead `player = 0` store, then the GetTC key copy (0x10), then the found component (player). One word for all three.
  - F+0x04/+0x08: CSoundPair (sample, sample) (edi twice).
  - F+0x0C..+0x28: CSoundInit (Volume 1.0, Pitch 1.0, Pan 0, NoPlays 1, PlaybackType 1, GainFaderBits 0x8040, PitchFaderBits 0 (ebp), OverrideID -1).
    Store order: NoPlays, PlaybackType (eax=1, early), Volume, Pitch, Pan, PitchFaderBits, OverrideID, GainFaderBits, pair.First, pair.Second.
  - arg4 slot (name, F+0x3C) reused for the CDefPointer<const CEntitySoundDef> (init 0 via ebp) and then for the `hasPitch` bool byte.
  - arg7 slot (F+0x48) address is the __find random_access_iterator_tag temp (`lea ecx,[esp+58h]` with 4 pushes); arg7 is read later (UseLipSynching).
- Register roles: esi=this, ebx=player (from LowerBound), ebp=name (loaded at 0x6E35F1 BEFORE the IsPureAISpeaker test), edi=narrators.last, later edi=sample; after def: ebx=def object, ebp=PBaseDef copy.
- Control flow:
  1. GetTC(0x10) inline: `test [thing+20h],10000h ; je` -> return 1. map ptr `lea edi,[eax+44h]`, key local, LowerBound 0x40F020, end read `[edi+4]`, end-or-greater ternary.
  2. `GetCurrentServerFrame() - SampleStartedFrame < SampleDurationFrames` -> StopCurrentSpeech(player) 0x6E28D0  (= inline IsTalking()).
  3. if IsPureAISpeaker: CGameDefinitionManager::Get()->[+0xD0]->narrators; __find(first(ecx), last(edx=edi), name, tag); == last -> IsPureAISpeaker=false (= inline SetAsUsingNarrator(name)).
  4. LastNarrator = name (CCharString::operator= 0x99EFB0, `lea ecx,[esi+30h]`).
  5. SampleID = player->PlayAttachedSample(bank, pair, init, true, loop, in2D).
  6. if SampleID != *0x129D25C: thing=[esi+4] loaded into ebx FIRST, then GetWorld() (0x686A70, ecx=this), then __fastcall OnDialogueStart(world(ecx), thing(edx), SampleID, layer(arg5)).
  7. def word = 0; PBaseDef copy (`mov ebp,[thing+70h] ; test ; inc [ebp+4]`) -> GetSubDef<CEntitySoundDef>(&def) 0x65F604 on it; bool = ok && def->VoicePitchOverride(+0x4C) != 1.0f (int compare `cmp [ebx+4Ch],3F800000h`, byte set 1 then jne / set 0 on fail path); release PBaseDef copy (dec, vslot 1).
  8. if bool: player->SetSpecificSamplePitch(SampleID, def->VoicePitchOverride) 0x66AFA0 (pitch pushed first, then [esi+28h]; ecx=player from F+0).
  9. UseLipSynching(+0x36) = arg7 (dl loaded before the pushes, store after `mov ecx,esi`); PlayLipMovement(bank, sample) 0x6E29A0.
  10. SampleLoadedTime(+0x20) = -1.0f; SampleStartedFrame = GetCurrentServerFrame(); SampleDurationFrames = GetSpeechDurationInFrames(sample) 0x6E2940.
  11. def release (no reload: ebx), `xor eax,eax` return 0.

## 2. Name / signature (manifest label CTCTalk::PlaySpeech is RIGHT, the draft's parameter types are not)
- Mangled leaf (FableWin.pdb): `?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@AAVCASoundBank@@J_NABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@11@Z`
  Sources: ghidra_out/_fablewin_api.tsv (0x01828C78) + ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (same); declared already in
  rebuild/include/engine/CTCTalk.h under FABLE_RETAIL_CTCTALK + FABLE_RETAIL_CTCTALK_CONVERSATION (chunk 1026, caller 0x00716B30 pushes 7 words).
  Not in Ego_r (Ego_r has only the (K, const CCharString&, ...) and (const CCharString&, ...) overloads).
- `ESpeechPlaybackResult PlaySpeech(CASoundBank& bank, long sample, bool b, const CCharString& narrator, NSpeechGainManager::EDialogueLayer layer, bool in2D, bool c);`
  (header spelling; bank is a REFERENCE, not pointer). Public, non-const, non-virtual (no vtable).
  Return enum (header): SPEECH_PLAYBACK_STARTED=0, SPEECH_PLAYBACK_FAILED_FINAL=1 (draft: SPEECH_PLAYING/SPEECH_NO_PLAYER -- wrong).
  Arg use: b = PlayAttachedSample 5th arg (loop?), in2D = 6th, c = UseLipSynching.
- Callers (xref): 0x006E39D7 (inside 0x006E3890 PlaySpeech(K, const CCharString&, ...), not landed), 0x006E5DBA (not landed), 0x00716C5A
  (landed rebuild/src/compiled/00/71/NTCScriptedControlCActionTalkBase_SetUpTextAndSpeech_00716b30.cpp, calls `talk->PlaySpeech(*message.PSpeechBank, message.SpeechIndex, false, GetNarrator(), ...)`).

## 3. Callees (landed file / landed name / PDB) -- draft spellings flagged
| addr | landed row | correct name | draft spelling |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (inlined GetTC) | CInterfaceMap::LowerBound -- WRONG |
| 0x00416225 | 00/41/CWorld_DrawGetEnvironment_00416225.cpp (placeholder `T::Get`) | CMainGameComponent::GetCurrentServerFrame() const (reads +0x48 = Ego_r CurrentServerFrame; landed siblings 006e2e60 use this name) on CMainGameComponent::PMainGameComponent (0x013B86A0) | CWorld::GetFrame / g_pWorld -- WRONG |
| 0x006E28D0 | 00/6e/CTCTalk_StopCurrentSpeech_006e28d0.cpp | ?StopCurrentSpeech@CTCTalk@@AAEXPAVCTCSoundPlayer@@@Z (private) | ok |
| 0x0044C6B0 | 00/44/CGameDefinitionManager_Get_0044c6b0.cpp | ?Get@CGameDefinitionManager@@SIAAV1@XZ (static __fastcall, returns &) | GetDefinitionManager -- WRONG |
| 0x004B8FF0 | 00/4b/std_findconstCCharStringCCharString_004b8ff0.cpp | std::__find<const CCharString*,CCharString> (__fastcall, ret 8) | ok-ish (CSpeechEntry type wrong: element is CCharString, 4 bytes? see note) |
| 0x0099EFB0 | 00/99/CCharString_operator_0099efb0.cpp | ??4CCharString@@QAEABV0@ABV0@@Z | ok |
| 0x0066B300 | 00/66/CTCSoundPlayer_PlayAttachedSample_0066b300.cpp | ?PlayAttachedSample@CTCSoundPlayer@@QAEKAAVCASoundBank@@ABVCSoundPair@@ABVCSoundInit@@_N22@Z (bank&, const pair&, const init&, bool,bool,bool) | pointer params -- WRONG types |
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (protected const, returns CWorld&) | returns CWorld* -- WRONG |
| 0x006E31E0 | not landed | ?OnDialogueStart@NSpeechGainManager@@YI_NABVCWorld@@AAVCThing@@KW4EDialogueLayer@1@@Z (Ego_r name set) | free OnDialogueStart(CWorld*,CThing*,...) -- WRONG namespace/types |
| 0x0065F604 | 00/65/CDefClassBase_GetSubDefCEntitySoundDef_0065f604.cpp | ??$GetSubDef@VCEntitySoundDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCEntitySoundDef@@@@@Z | GetSubDef(CDefPointer<CTalkDef>*) -- WRONG (CTalkDef invented) |
| 0x0066AFA0 | 00/66/CTCSoundPlayer_SetSpecificSamplePitch_0066afa0.cpp | ?SetSpecificSamplePitch@CTCSoundPlayer@@QAEXKM@Z | SetSampleVolume -- WRONG |
| 0x006E29A0 | 00/6e/CTCTalk_PlayLipMovement_006e29a0.cpp | ?PlayLipMovement@CTCTalk@@QAEXAAVCASoundBank@@J@Z | SetMeshBankAndSymbol -- WRONG |
| 0x006E2940 | 00/6e/CTCTalk_GetSpeechDurationInFrames_006e2940.cpp | ?GetSpeechDurationInFrames@CTCTalk@@ABEJJ@Z (private const) | FindAndAddMeshSymbol -- WRONG |
| vslot 1 (+4) | -- | CDefClassBase release (refcount hit 0) via CDefPointer dtor | ok |
Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent (protected static; read via inline static Get()); 0x0129D25C = g_InvalidSoundHandle (established landed spelling, descriptive).
Note on __find element: the vector is CGlobalDef::PermittedAINarrators (vector<CCharString>, Ego_r +0x108 -> retail +0x100 STLport 12-byte vector: _M_start +0x100, _M_finish +0x104), elements are CCharString (the draft's 16-byte CSpeechEntry is wrong; only matters for typing).

## 4. Members
CTCTalk (rebuild/include/engine/CTCTalk.h; Ego_r layout = retail, sizeof 0x3C; all private):
- +0x04 CTCBase::ParentThing (CTCBase.h)
- +0x10 SampleStartedFrame (draft SpeechStartFrame), +0x14 SampleDurationFrames (draft SpeechLengthFrames)
- +0x20 SampleLoadedTime float (draft SpeechTimer), +0x28 SampleID unsigned long (draft SpeechHandle)
- +0x30 LastNarrator CCharString (draft SpeechName), +0x34 IsPureAISpeaker bool (draft CheckSpeechBank), +0x36 UseLipSynching bool (draft LipSync)
- Opt-ins: FABLE_RETAIL_CTCTALK + FABLE_RETAIL_CTCTALK_CONVERSATION (declares PlaySpeech), FABLE_RETAIL_CTCTALK_LIPSYNC (GetSpeechBank/GetPLipSyncBank; needs FABLE_RETAIL_CTCBASE_PEEKSAMPLEBANK). The header does NOT yet declare PlayLipMovement, GetSpeechDurationInFrames, StopCurrentSpeech(CTCSoundPlayer*), SetAsUsingNarrator -- the row needs a new nested opt-in (or the author proposes one) declaring them; IsTalking() is declared (body needed inline).
CThing (rebuild/include/engine/CThing.h): +0x20 presence bits, +0x44 Components (CVectorMap), +0x70 PBaseDef CDefPointer<const CThingBaseDef> (draft "DefClass"); GetPBaseDef() const returns by value (the `inc [ebp+4]` copy). GetTC(id,&ptc) expanded form under FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED (key local inside test, map pointer local, ternary end-or-greater) -- exactly the retail shape.
CEntitySoundDef (rebuild/include/engine/CEntitySoundDef.h, needs FABLE_RETAIL_DEF_CLASS_BASE): +0x4C VoicePitchOverride float (draft "CTalkDef::SpeechVolume").
CGameDefinitionManager (CGameDefinitionManager.h): +0xD0 PGlobalDef (descriptive) under FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF, PeekGlobalDef() inline.
CGlobalDef (CGlobalDef.h:57): PermittedAINarrators std::vector<CCharString> at +0x100.
CMainGameComponent.h: CurrentServerFrame +0x48; inline GetCurrentServerFrame() exists only under FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES -- but retail CALLS 0x416225 out of line, so it must be declared non-inline (the IsTalking 006e28a0 / StopCurrentSpeech 006e2e60 rows model it as an out-of-line getter).
CSoundInit.h: Volume, Pitch, Pan, NoPlays, PlaybackType, GainFaderBits(+0x14 = 0x8040), PitchFaderBits, OverrideID; opt-ins FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR / _INLINE_CTOR_TYPE_LAST (inline ctor; out-of-line COMDAT 0x41C930 not called here -> must be inline).
CSoundPair.h: FirstSound/LastSound; FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS gives inline `explicit CSoundPair(long sound)` (sample,sample) -- matches the edi,edi stores.
CTCSoundPlayer.h: the bank overload 0x66B300 is NOT declared (only the pairless-bank 0x66B280 under FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE); SetSpecificSamplePitch also needs declaring.
NSpeechGainManager.h: no OnDialogueStart declaration yet.
Draft-local classes that have shared headers: CCharString (fable_string.h), CThing, CTCBase/CTCTalk, CDefClassBase, CDefPointer, CRefBase-ish defs, CSoundPair, CSoundInit, CTCSoundPlayer, CGameDefinitionManager, CMainGameComponent ("CWorld"), CGlobalDef ("CSoundManager"), CVectorMap ("CInterfaceMap"), NSpeechGainManager.

## 5. Callee visibility (same TU = CTCTalk source; neighbours defined earlier)
- INLINED in retail, bodies visible: CTCTalk::IsTalking() (out-of-line copy 0x6E28A0, landed) and CTCTalk::SetAsUsingNarrator(const CCharString&) (out-of-line copy 0x6E2CF0, landed, RELOCATION_MATCH; its body is literally step 3). Write `if (IsTalking()) StopCurrentSpeech(player);` and `SetAsUsingNarrator(narrator);` with those bodies visible (inline) -- this probably fixes the ebp(name)/edi(last) assignment and the tag-in-arg-slot gap the old draft could not reach.
- Out of line (call): StopCurrentSpeech(CTCSoundPlayer*) 0x6E28D0, GetSpeechDurationInFrames 0x6E2940, PlayLipMovement 0x6E29A0 (all same TU, earlier), OnDialogueStart 0x6E31E0 (NSpeechGainManager, same object region), GetCurrentServerFrame 0x416225 (out-of-line COMDAT), CGameDefinitionManager::Get 0x44C6B0, GetSubDef<CEntitySoundDef> 0x65F604 (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE in CDefClassBase_GetSubDef.h: visible noinline body lets the caller keep the def in a register -- retail keeps it in ebx and releases without reload), LowerBound 0x40F020, std::__find 0x4B8FF0 (fable_algorithm.h: FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH declares __find out of line + inline find() building random_access_iterator_tag()).
- SetAsUsingNarrator 006e2cf0 note: matched with the 4-arg __find called directly (no wrapper), `last` a named local; the tag homed in the narrator's own arg slot.

## 6. Siblings (same class / neighbourhood)
- 00/6e/CTCTalk_SetAsUsingNarrator_006e2cf0.cpp -- local model; direct `FindString(first,last,narrator,SRandomAccessTag())`, named `last`. No pragmas.
- 00/6e/CTCTalk_StopCurrentSpeech_006e2e60.cpp -- out-param GetInterface(T**,long) form (key copied inside the test, map pointer local, ternary) RELOCATION_MATCH; CMainGameComponent::Get()->GetCurrentServerFrame out of line. No pragmas.
- 00/6e/CTCTalk_IsTalking_006e28a0.cpp -- body `return GetFrame() - SampleStartedFrame < SampleDurationFrames;`.
- 00/6e/CTCTalk_PlayLipMovement_006e29a0.cpp, CTCTalk_GetSpeechDurationInFrames_006e2940.cpp -- `#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP`, engine/CASoundManager.h.
- 00/71/NTCScriptedControlCActionTalkBase_SetUpTextAndSpeech_00716b30.cpp -- the shared-header idiom set: FABLE_RETAIL_CCHARSTRING, _CTBASE, _CTHING, _CTHING_GETTC_EXPANDED, _CWORLD, _CTCTALK, _CTCTALK_CONVERSATION, _STLP_VECTOR, _DEF_CLASS_BASE ...
- 00/66/CTCSoundPlayer_GetPotentialCriteriaStrings (0x66BC10, chunk 1021) -- the GetSubDef<CEntitySoundDef> NOINLINE + CEntitySoundDef + HasBaseDef pattern.
- No CTCTalk row uses optimize pragmas; none has an EH frame.

## 7. Old draft model errors
- Every member/callee name above (8 callee misspellings, 7 member misspellings, invented CTalkDef/CDefClassPointer/CSoundManager/CDefinitionManager/CWorld types).
- bank as pointer; PlayAttachedSample/OnDialogueStart/GetWorld pointer vs reference types; OnDialogueStart not in NSpeechGainManager.
- IsTalking and SetAsUsingNarrator hand-expanded instead of called inline -> register assignment (name in ebp loaded before the bool test, end in edi) and the tag's arg-slot home differ.
- `CDefClassPointer defClass(ParentThing->DefClass)` should be the by-value temporary `ParentThing->GetPBaseDef()` (CDefPointer<const CThingBaseDef>, copy ctor with null test) used in the condition: `if (ParentThing->GetPBaseDef()->GetSubDef(&sound_def) && sound_def->VoicePitchOverride != 1.0f)` -- retail materialises a bool (set 1/0) in the reused arg4 slot, released before the if body, consistent with a full-expression temporary.
- Draft's GetSubDef is a local auto_inline(off) stand-in with an invented FindTalkDef -- use the shared CDefClassBase_GetSubDef.h NOINLINE opt-in.
- CSoundInit: write `CSoundInit init; init.GainFaderBits = 0x8040;` with the shared inline ctor; check which inline-ctor opt-in reproduces the NoPlays/PlaybackType-first store order.
- The dead `player = 0` store suggests `CTCSoundPlayer* player = NULL;` before `if (!ParentThing->GetTC(0x10/TCI_SOUND_PLAYER?, &player)) return SPEECH_PLAYBACK_FAILED_FINAL;` (address-taken init kept). Draft already has it but its ternary used `interfaceType < it->type` on the param, not the key local.

## 0x00966AF0

# DOSSIER 0x00966AF0 CAIStateGroup_TavernServer::GetBar (502 B)

## 1. Retail shape (xdis 00966af0 0x206)
- __thiscall, `ret` (no args), returns bool in al (`xor al,al` / `mov al,1`). Four exits: 0x966BF0 (empty list, false),
  0x966C94 (pick loop ran out, true), 0x966CC9 (picked / list empty at pick, true), 0x966CDC (empty after PBar=0, false).
- NO frame pointer, NO `and esp,-8` (sibling GetServingPoint 0x966DA0 has the aligned ebp frame; GetBar does not): `sub esp,0x10; push ebx/ebp/esi/edi`.
- NO SEH/EH frame even though locals have dtors (no-EH TU, like GetSeat 0x0094CB40 "no EH frame").
- Speed-optimised (loop-top alignment fillers `lea ecx,[ecx]`, `nop`) -> NOT optimize("s") (WanderWithinTavern 0x966AD0 is "s"; GetBar isn't).
- Register plan: this in edi then spilled to [esp+0x10]; list header node in esi for the WHOLE function; building/owner TC in ebx
  (loop 1), then ebx = &PBar (this+0x1c) for the pick; ebp = loop node / count.
- Flow: GetTC(0x21) on PPub.GetPItem() (inlined HasTC bit test `[eax+0x24]&2` + LowerBound 0x40F020) BEFORE the list ctor
  (malloc 0xc, self-link); loop 1 over owner->ObjectList (+0x0c sentinel): GetPItem x6 per node = IS_THING_ALIVE twice
  (0x91 bit0), HasTC(TCI_TAVERN_BAR=185: `test dword [eax+0x34],0x2000000`), GetPItem for push_back (malloc 0xc, placement
  store with `test ecx,ecx` null check, link before m_node). Then `if empty -> inlined dtor (self-link + free) ; return false`.
  Then PBar = 0 INLINE (ctor 0xA01B10, vptr 0x0125A858 = CIntelligentPointer<CThingObject>, SetPItem(0), GetPItem, SetPItem on
  this+0x1c, dtor 0xA01C10). Then `if empty -> ~list 0x792240 ; return false`. Count loop (size()), GetPWorldSeed 0x8FCFD0,
  inline GFRandom (imul 0x24A1, add 0x24DF, GFROR13 0x497890, `range? seed%range : 0`). Pick loop: `if (index==0) {PBar = *it; break;} --index`
  with the assignment as OUT-OF-LINE ctor 0x0065E133(push [node+8]) + operator= 0x007B82C0 (ecx=&PBar) + dtor 0xA01C10, then ~list 0x792240, return true.

## 2. Identity (name proven by two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv) 0x0098B360 `?GetBar@CAIStateGroup_TavernServer@@QAE_NXZ`, between WanderWithinTavern
  0x0098B340 and IsCarryingJug 0x0098B5B0 -- retail order WanderWithinTavern 0x966AD0 / GetBar 0x966AF0 / IsCarryingJug 0x966CF0 (landed rows).
- FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv) 0x018091D9 same mangled name.
- Caller: IsRunnable 0x00967650 (call at 0x0096772C), landed rebuild/src/compiled/00/96/CAIStateGroupTavernServer_IsRunnable_00967650.cpp
  declares `bool GetBar(); // 0x00966AF0`. Manifest label is CORRECT. Signature: `public: bool CAIStateGroup_TavernServer::GetBar()` (non-const, non-virtual).
- Shared header engine/CAIStateGroup_TavernServer.h FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_METHODS block does NOT yet declare GetBar
  (it has WanderWithinTavern/IsCarryingJug/GetServingPoint/.../FillTankard). Author must add `bool GetBar(); // 0x00966AF0` (header edit) or use a header row-local opt-in.

## 3. Callees (landed file / name / PDB)
- 0x00A01B50 GetPItem, 0x00A01B10 ctor, 0x00A01B90 SetPItem, 0x00A01C10 dtor: rebuild/src/compiled/00/a0/CBaseIntelligentPointer_*_00a01b*.cpp; PDB CBaseIntelligentPointer (shared engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER).
- 0x0040F020 CVectorMap::LowerBound (00/40/CVectorMap_LowerBound_0040f020.cpp) -- inlined GetTC via FABLE_RETAIL_CTHING_GETTC_EXPANDED / FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND.
- 0x00BFEA0E malloc / 0x00BFEA14 free (00/bf/global_malloc_00bfea0e.cpp, global_free_00bfea14.cpp): STLport list node allocator.
- 0x008FCFD0 CAIStateGroupBase::GetPWorldSeed (00/8f/CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp; engine/CAIStateGroupBase.h line 75, `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` -- protected CONST; draft declares it non-const: mismatch).
- 0x00497890 GFROR13 (00/49/global_GFROR13_00497890.cpp; engine/GFRandom.h `GFRandom(range, seed)` inline = exactly retail's sequence).
- 0x0065E133 CIntelligentPointer<CThingObject>::CIntelligentPointer(CThingObject*) (00/65/CIntelligentPointer_CThingObject_CIntelligentPointer_0065e133.cpp; RTTI vtable 0x0125A858). Draft calls it "CIntelligentPointerThing" -- wrong pointee; it is <CThingObject>.
- 0x007B82C0 CIntelligentPointer<T>::operator= (00/7b/CIntelligentPointerCThingCreatureBase_operator_007b82c0.cpp, 24 folded bodies; here = <CThingObject>).
- Draft's spellings `CInterfaceMap::LowerBound`, `CThingList`, `CTCBuilding` (interface 0x21) are wrong: 0x21 = TCI_THING_OWNER = CTCThingOwner (ETCInterfaceType.h:38), list member ObjectList (+0x0c, std::list<CIntelligentPointer<CThing> >, GetObjectList()).

## 4. Members
- this+0x1c PBar, this+0x34 PPub: CIntelligentPointer<CThingObject> / CIntelligentPointer<CThingBuilding>, private members of engine/CAIStateGroup_TavernServer.h (FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER) -- member function has access.
- CThing (retail offsets via engine/CThing.h, FABLE_RETAIL_CTHING): UsingComponents bitset +0x20 (bit 0x21 -> byte +0x24 bit1; bit 185 -> dword +0x34 0x2000000), Components CVectorMap +0x44, Dead bit +0x91 bit0 (IsDead()). (Ego_r layout is shifted by the 8-byte UniqueID: 0x48/0xA1 -- do not use the egor offsets.)
- CTCThingOwner::ObjectList +0x0c (engine/CTCThingOwner.h, FABLE_RETAIL_CTCTHINGOWNER).
- Local list = std::list<CThingObject*> (STLport, stlport/fable_containers.h FABLE_RETAIL_STLP_LIST): node {next,prev,data}, 0xc bytes.
- Classes the old draft models LOCALLY that have shared headers: CThing, CInterfaceMap(CVectorMap), CBaseIntelligentPointer/CIntelligentPointer, CAIStateGroupBase, CAIStateGroup_TavernServer, CTCThingOwner (as CTCBuilding), std::list (CThingList), GFRandom (WorldRandomRange), IS_THING_ALIVE (engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE).

## 5. Callee-body visibility
- ~list 0x792240: retail inlines it at the FIRST return only (0x966BDE: `cmp [esi],esi; jne; self-link; free(esi)`, clear loop folded because known empty) and calls it out of line at the three later exits. Same for the IP assignment: `PBar = 0` inlined, the pick's `PBar = *it` out of line (0x65E133 + 0x7B82C0). Both look like VC7.1's per-function inline budget running out, not a header choice -> prefer plain inline bodies; whole-TU declaration-only opt-ins (FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (noinline, visible)) would also out-of-line the first sites -- wrong unless the first sites are spelled differently (e.g. PBar = 0 via a different overload).
- Sentinel-in-esi: GetSeat 0x0094CB40 (RELOCATION_MATCH) fixed exactly this residue by giving ~list<CThing*> 0x00792240 a same-TU body under `#pragma auto_inline(off)` so VC sees the list address never escapes. FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (visible noinline body, fable_containers.h:859) is the shared-header equivalent; FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE (line 813) also exists.
- Same-TU visible: GetServingPoint 0x966DA0 is the twin function (same TU, defined after GetBar): copy its body form.

## 6. Siblings
- 00/96/CAIStateGroupTavernServer_GetServingPoint_00966da0.cpp -- near-identical body (owner list -> list<CThingObject*> -> PTable = 0 -> GFRandom pick). Defines: FABLE_RETAIL_CAISTATEGROUPBASE(_MEMBERS,_GETCREATURE,_AIACTIONS,_MEALACTIONS), FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER(_METHODS), FABLE_RETAIL_CAIBRAIN_GETCREATURE, _CREATURE_REFERENCE, FABLE_RETAIL_CINTELLIGENTPOINTER(_GET_ORIGINAL_CHAIN,_COMPARE_NULL_INT,_ASSIGN), FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTHING(_GETTC_EXPANDED), FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND, FABLE_RETAIL_CTHINGPHYSICAL/GAMEOBJECT/CREATUREBASE/AICREATURE, FABLE_RETAIL_CTCTHINGOWNER, FABLE_RETAIL_IS_THING_ALIVE, FABLE_RETAIL_STLP_LIST; includes engine/CThingObject.h, ThingAlive.h, CTCThingOwner.h, GFRandom.h, stlport/fable_containers.h. Body: `CTCThingOwner* owner; PPub->GetTC(&owner); std::list<CThingObject*> tables; for(...) if (IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_TAVERN_TABLE)) tables.push_back((CThingObject*)(*it).Get()); if (tables.empty()) return false; PTable = 0; ... GFRandom(tables.size(), GetPWorldSeed())`.
- 00/94/CAIStateGroupGameMaster_GetSeat_0094cb40.cpp -- the auto_inline(off) ~list 0x792240 trick (local models though).
- 00/96/CAIStateGroupTavernServer_IsRunnable_00967650.cpp (caller), _WalkToPoint_009677a0.cpp (VERIFIED_MATCH, `// Flags: none`), _WanderWithinTavern_00966ad0.cpp (`#pragma optimize("s",on)` -- tiny row only).

## 7. Old draft model errors
- Bar loop calls IS_THING_ALIVE twice -- CORRECT (retail has 4 GetPItem+tests before HasTC; GetServingPoint has only 2). Keep `IS_THING_ALIVE(*it) && IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_TAVERN_BAR)`.
- Draft returns false via `count == 0` after a manual count; retail does a second `empty()` test (0x966C33 `cmp [esi],esi` on head->next) then size() count -- spell `if (bars.empty()) return false;` (or while(!empty()) {...; return true;}) then `GFRandom(bars.size(), GetPWorldSeed())`.
- Pick loop: retail `for(it=begin;it!=end;++it){ if(index==0){PBar=*it;break;} --index; } return true;` (decrement in body, as GetServingPoint). Draft's `--index` in the for-increment is a different form.
- Wrong types: interface 0x21 is CTCThingOwner not CTCBuilding; ctor 0x65E133 is CIntelligentPointer<CThingObject>; list is list<CThingObject*>; GetPWorldSeed is const; everything modelled locally instead of shared headers.
- Draft's residue (sentinel reloaded, this kept in ebx) = the GetSeat residue; fix via visible-noinline / auto_inline(off) ~list body.

## 0x006992A0

# DOSSIER 0x006992A0 -- CGameCameraManager::PreparePrimitivesForRendering (506 bytes)

## 1. Retail shape (xdis 006992a0 0x20a)
- thiscall, `ret 4`, `sub esp,64h`, NO frame pointer (no push ebp/mov ebp,esp), NO SEH prologue even though a local
  std::list with a dtor lives across calls -> TU like the other CGameCameraManager rows (ResetFlags 0x00699130: `// Flags: none`).
- Register roles: esi=this (spilled once at [esp+10h] after 4 pushes, reloaded after loop 1 and each loop-2 iteration),
  edi=&info (loaded from the arg slot [esp+70h] before `je` on mode 1), ebp = local list header node, ebx = list cursor.
- Push order: esi, (test DrawDebugGraphics==0 -> early exit) edi, ... ebx, ebp only inside the `==2||==3` block (shrink-wrapped).
- Control flow: `if(DrawDebugGraphics==0) return;` `if(==1||==3) CurrentMode->slot5(info)` (vtbl+14h);
  DrawDebugGraphics is RE-READ from [esi+154h] after that call; `if(==2||==3) {...}`.
- Local list: malloc(0Ch) header, next=prev=self (STLport list<CThing*> ctor inlined); the list object is homed in the DEAD
  info-parameter slot ([esp+78h] base frame) -- `lea ecx,[esp+78h]` passed as GetAllThings' 2nd arg; header ptr also copied to
  [esp+18h] (reloaded into ebp for the dtor). The CThingFilter_HasTC temp (Type=0x3E) sits at [esp+14h].
- Later the TC key 0x44 for LowerBound is written into the same [esp+78h] slot (key temp shares the dead slot).
- Loop 1 (`lea ebx,[ebx]` align pad): thing=node->data; `push 0.25f(3E800000)`; GetPos(thing) 0x004C73D0;
  `mov edx,eax ; mov ecx,edi ; call 0x008764E0` (__fastcall DrawPyramid(info,pos,0.25f)); `test byte [esi+28h],10h`
  (UsingComponents bit 68 = TCI_SHAPE_MANAGER); LowerBound(&key) on [thing+44h]; end read `mov esi,[esi+4]` via the map
  pointer register; `cmp eax,esi ; je ; cmp [eax],44h ; jle` (conditional-expression form) -> `mov ecx,[eax+4]` component,
  vtbl+2Ch (slot 11) with push edi.
- After loop 1: `C3DVector pos = CurrentMode->GetPosition()` read ONCE: `mov eax,[this+98h]; ecx=[eax+54h]; edx=[eax+58h];
  add eax,54h; eax=[eax+8]` -> GoTo.Position (+0x54 = CBase::GoTo CPhysicsState, Position at +0) copied as 3 ints to [1Ch..24h].
- Loop 2 over DebugPoints (+0x15C, list<pair<C3DVector,CRGBColour>>): end re-read every iteration as `cmp esi,[this+15Ch]`
  (this reloaded from [esp+10h]); begin read once. Per node: End X,Y via FPU (fld +8, fld +0C, fxch, fstp), End Z via int
  (ecx=[+10h] spilled to [esp+30h]); colour bytes loaded B(+14h), A(+17h, spilled byte [esp+37h]), R(+16h), G(+15h); stored
  B->+5C, G->+5D, A->+5F, R->+5E. => an 8-byte temp at [30h..37h] = the copied element tail (Z + colour): the source very
  likely copies the pair (or the C3DVector/CRGBColour) into a local before building the line.
- CEnginePrimitiveLine built in place at [esp+38h] (0x3C bytes): Type=6, InterpolationSet=0 (ebx), 3 channel bytes 0,
  Start=pos (+44h..4Ch), End (+50h..58h), Colour (+5Ch), Width=0.05f (3D4CCCCD) +60h, BankIndex/BlendMode/BlendOp=0
  (+64h..6Ch), UseZBuffer=1 (+70h). Engine=[info+34h], call vtbl+50h (slot 20 AddPrimitive) with push &line.
  Same as sibling 0x007617A0: `CEnginePrimitiveLine(0, start, end, colour, 0.05f, 0, 0, true, 0, 0)`.
- Epilogue: inline list dtor (free each node, `lea esp,[esp]` pad, reset header, free header) then pops.

## 2. Name (two+ sources) -- manifest label is CORRECT
- rtti-vtables.tsv: CGameCameraManager vtable 0x0125C754 slot 2 = 0x006992A0 (only xref: .rdata 0x0125C75C).
- labels_rtti_port.tsv + naming_stragglers/Fable.exe.signatures.tsv: ?PreparePrimitivesForRendering@CGameCameraManager@@UBEXABVCEnginePrimitivePrepareInfo@@@Z
- egor_pdb_names.tsv 0x006C0240 and fablewin_pdb_names.tsv 0x02202DA0: same mangled name.
- Signature: `virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) const;` (public virtual, const).
  It overrides CAIGameCameraBase slot 2 (header LOOKAROUND branch derives from CAIGameCameraBase).

## 3. Callees
| retail | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00BFEA0E | 00/bf global_malloc_00bfea0e.cpp | malloc | MSVCR71 malloc | malloc OK |
| 0x00BFEA14 | global_free_00bfea14.cpp | free | free | OK |
| 0x004ACD70 | 00/4a CThingSearchTools_GetAllThings_004acd70.cpp | CThingSearchTools::GetAllThings<CThing,list<CThing*>,CThingFilter_HasTC> (filter, result) | Ego_r 004b3490 ??$GetAllThings@VCThing@@V?$list@PAVCThing@@...@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVCThingFilter_HasTC@@AAV?$list... (CONST member, filter first) | WRONG: draft `CThingManager::GetAllThings(const long&, list&)`; `this` is CWorld+0x20 = PThingSearchTools_ptr (CWorld.h: PeekThingSearchTools() const / GetThingSearchTools()) |
| 0x004C73D0 | 00/4c CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r 004c9070) | WRONG: draft `GetPosition` |
| 0x008764E0 | NOT landed | manifest label NCameraModes::DrawPyramid | ?DrawPyramid@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@M@Z (Ego_r 0085d1f0, FableWin 029c4cb0) -- __fastcall, (const info&, const C3DVector&, float) | WRONG: draft `DrawDebugSphere`; sibling 0x007617A0 calls it `EditorDrawCross` (also wrong -> NAME_FIXES candidate) |
| 0x0040F020 | 00/40 CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const) | same | draft local ComponentMap::LowerBound |
| vtbl+14h on CurrentMode | -- | NCameraModes::CBase slot 5 PreparePrimitivesForRendering(const info&) const | NCameraModes_CBase.h CYLINDER branch declares it at slot 5 | draft OK name |
| vtbl+2Ch on component | -- | CTCBase slot 11 | ?DrawPreparePrimitivesForRendering@CTCBase@@UAEXABVCEnginePrimitivePrepareInfo@@@Z (NON-const; CTCShapeManager overrides, Ego_r 00772ea0) | WRONG: draft calls it `PreparePrimitivesForRendering ... const` |
| vtbl+50h on info.Engine | -- | CIEngine slot 20 AddPrimitive(const CEnginePrimitive&) (CIEngine.h line 94) | ?AddPrimitive@CIEngine@@ | draft `CRenderer::AddPrimitive` WRONG class |
- TC ids (ETCInterfaceType.h): 0x3E = TCI_DRIVER_CAMERA_POINTS (filter), 0x44 = TCI_SHAPE_MANAGER (component).

## 4. Members
- this+0x74 World: CGameCameraManager.h `CWorld* World` (default) / `const CWorld& World` under FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD (with GetWorld()).
- this+0x98 CurrentMode: `CurrentMode_Data` (default) / `CCountedPointer<NCameraModes::CBase> CurrentMode` under FABLE_RETAIL_CGAMECAMERAMANAGER.
- this+0x154 `long DrawDebugGraphics` (draft calls it DebugMode -- wrong).
- this+0x15C `DebugPoints`: header `unsigned char DebugPoints[4]` // list<std::pair<C3DVector,CRGBColour>> (draft: DebugLines list<CDebugLine> -- wrong name/type). No opt-in types it yet (CAMPOINTS opt-in types only AvailableCamPoints +0xAC); the LOOKAROUND full-class branch has it as `unsigned char DebugPoints[4]` too and DrawDebugGraphics private.
- CWorld+0x20 PThingSearchTools_ptr (CWorld.h line 303/426; accessors line 190-191).
- NCameraModes::CBase+0x54 GoTo (CPhysicsState, Position at +0; struct_layouts_egor CPhysicsState) ; GetPosition() const
  declared in CBASE_POSITION / CYLINDER branches (out of line declaration; retail inlines it here -> author needs an inline body
  `return GoTo.Position;` -- CYLINDER branch has typed GoTo under TYPED_COMPOSITES).
- CThing+0x20 UsingComponents (bitset), +0x44 Components (CVectorMap) -- CThing.h, use FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC<T>(id,&p) conditional-expression form, LowerBound out of line 0x0040F020) + HasTC.
- CEnginePrimitivePrepareInfo+0x34 Engine (CIEngine&) -- CEnginePrimitivePrepareInfo.h under FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO.
- CEnginePrimitiveLine (0x3C) -- CEnginePrimitiveLine.h; inline ctor body only under FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT (inline, wanted here) or
  FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE (noinline -- NOT wanted: retail expands it here).
- Locally modelled in the draft but shared headers exist: CGameCameraManager, CThing, CWorld, CThingManager(wrong class anyway),
  CTCBase, NCameraModes::CBase (draft CCameraModeBase), CEnginePrimitivePrepareInfo, CEnginePrimitiveLine (draft CPrimitiveLine),
  CIEngine (draft CRenderer), C3DVector, CRGBColour, CVectorMap, std::list.

## 5. Visibility
- GetAllThings 0x004ACD70: template, out of line (called). GetPos 0x004C73D0 out of line (7-byte COMDAT). LowerBound 0x0040F020 out of line.
- DrawPyramid 0x008764E0 is in another TU (NCameraModes, after DrawSquare 0x00875FB0? sibling calls 0x00875FB0 too) -> declare only.
- list<CThing*> ctor/dtor and the DebugPoints iteration all INLINE (malloc/free direct) -> plain std::list, no STLP_LIST_*_OUTOFLINE opt-ins.
- CEnginePrimitiveLine ctor INLINE (record built on stack); CBase::GetPosition INLINE (add eax,54h).
- Neighbours in same TU (CGameCameraManager.cpp): 0x006991F0 AddCameraPoint, 0x00699130 ResetFlags, 0x006994F0 IsObservedThingInMelee.

## 6. Siblings
- rebuild/src/compiled/00/69/CGameCameraManager_ResetFlags_00699130.cpp: `// Flags: none`; defines FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CINTELLIGENTPOINTER, CGAMECAMERAMANAGER_CAMPOINTS; includes engine/CGameCameraManager.h; iterates std::list with iterator for-loop.
- CGameCameraManager_IsObservedThingInMelee_006994f0.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CVectorMap.h + CIntelligentPointer; local CGameCameraManager (padding).
- CGameCameraManager_AddCameraPoint_006991f0.cpp / RemoveCameraPoint_006984a0.cpp: `#pragma auto_inline(off)`..`(on)` around helpers; std::list<CIntelligentPointer<CThing>>.
- rebuild/src/compiled/00/76/NShapeCShape_EditorDrawHighlightPoints_007617a0.cpp: the SAME DrawPyramid (0x008764E0) fastcall + inline CEnginePrimitiveLine(0,start,end,colour,0.05f,0,0,true,0,0) + Engine->AddPrimitive; CRGBColour copy ctor `R=c.R;G=c.G;B=c.B;A=c.A;`, C3DVector user copy ctor -- copy its line/colour idioms.
- CGameCameraManager_SetEarthquakeAt_00699560.cpp includes fable_compiler.h, CGameCameraManager.h, CWorld.h.

## 7. Old-draft model errors
- Wrong names: DebugMode->DrawDebugGraphics, DebugLines->DebugPoints (pair<C3DVector,CRGBColour>), GetPosition->GetPos,
  DrawDebugSphere->NCameraModes::DrawPyramid, CThingManager::GetAllThings(long) -> CThingSearchTools::GetAllThings<CThing>(CThingFilter_HasTC(TCI_DRIVER_CAMERA_POINTS), list),
  component slot 11 is CTCBase::DrawPreparePrimitivesForRendering (non-const), CRenderer->CIEngine.
- Element copy: retail spills End.Z and Colour.A into an 8-byte temp at [esp+30h..37h] -> copy the pair/element into a local
  (e.g. `std::pair<C3DVector,CRGBColour> p = *it;` or by-value colour) rather than passing node refs directly; draft's 0x4C frame lacks it.
- List object should live where VC7.1 can home it in the dead `info` arg slot: info must be cached in a register (edi) -- keep it used only by value of pointer.
- Loop 2 end() must be re-read from this each iteration (it is, via DebugPoints.end()); camera pos read once before loop (draft OK).
- Retail re-reads DrawDebugGraphics after the mode call (draft OK: separate ifs).
- GetComponent template in draft returns uninitialised on false path; retail simply skips the call (bit test guards whole call) -> `if (thing->HasTC(TCI_SHAPE_MANAGER)) GetTC(...)->DrawPreparePrimitivesForRendering(info)` style using GETTC_EXPANDED.

## 0x008F0EB0

# DOSSIER 0x008F0EB0 -- CTCTavernGameCardBase::CardsTurning (506 bytes)

## 1. Retail shape (xdis 008f0eb0 0x20a)
- __thiscall (this in ecx -> esi), plain `ret` (3 copies: 0x8F0F20, 0x8F1087, 0x8F10A9), no frame pointer (`sub esp,0x18`, push ebx/esi/edi), NO SEH frame although a CCharString and a CDefPointer with dtors live in it -> TU without EH, like sibling ShuffleCards ("Flags: none"). Loop pads `mov edi,edi` / `lea esp,[esp]` = normal /O2, no optimize("s").
- Frame: def CDefPointer at [esp+0xc] (frame word 0) null-initialised AFTER its address is pushed; GetCardDef(&def) call 0x8F0B20.
- Loop 1: `for it in Cards (+0x128..+0x12c)`: `cmp byte [card+0x24], dl(=1)` -> explicit `Turning == true`; hit -> its OWN release (`mov ecx,[esp+0xc]; test; dec [ecx+4]; jne; call [edx+4]`) + own epilogue/ret (0x8F0F00..0x8F0F20).
- State (+0xC0): ==0x16 -> `push 0x17; jmp 0x8F108A`; !=0x19 -> `push 0` at 0x8F1088 then shared `mov ecx,esi; call SetState; release def from [esp+0xc]; ret` (if/else chain cross-jumps the two SetState calls).
- State 0x19: two x87 sums of CurrentPos.X (+0x3f) / CurrentPos.Y (+0x43) from 0.0f (const 0x122DEDC), divided by (float)Cards.size() computed TWICE (unsigned->float: `test; jge; fadd 2^32` [0x122DCB4]) -> avgX spilled [esp+0x14], avgY stays in st0.
- Then `push ebp; mov ebp,[esp+0x10]` = def.Object loaded into EBP and the def frame word reused as the height counter: `mov [esp+0x10],0x35` stored AFTER the empty-loop guard and after the hoisted stackPos.X/Y stores, decremented each pass, read by `fild` (signed long). This is the strength-reduced `53 - i` form (exactly what CardsMoving 0x008F2280 notes: "its 53 is stored after the hoisted copies; a plain height=53;--height counter is stored before the loop guard").
- Loop body: named CCharString("TAVERN_GAME_CARD_DEAL" @0x0126F568, -1) ctor 0x99EBF0 at [esp+0x14] (reuses avgX slot; `lea ecx` recomputed for PlaySoundW push = named local, not temp), PlaySoundW 0x5EA950, ~CCharString 0x99EAE0; Z = fild(cnt)*def->CardSeparation(+0x5c)+def->TableSeparation(+0x60); then the INLINED CCard::Move(C3DVector(avgX,avgY,Z), false) (Moving +0x26 test; LocalPos=CurrentPos / else CurrentPos=LocalPos, Moving=1; NewLocalPos=pos; MoveFraction(+0x20)=0). Loop end re-reads Cards end [esi+0x12c] each pass, iterator in edi.
- After: SetState(0x1a); release via ebp (`dec [ebp+4]; mov edx,[ebp]; mov ecx,ebp; call [edx+4]`), pop ebp, shared epilogue.

## 2. Identity
- Name: CTCTavernGameCardBase::CardsTurning, leaf `?CardsTurning@CTCTavernGameCardBase@@UAEXXZ` (public virtual void(), non-const). Sources: ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv (Ego_r 0x009166A0), _fablewin_api.tsv + FableWin.exe.signatures.tsv (FableWin 0x01824222/0x02AF2460); vtable slot 54: xref.py hits 0x0128836C = CTCTavernGameCardBase vtable 0x01288294 + 0xD8 (slot 54), and rebuild/manifest/rtti-vtables.tsv CTCTavernGameCardSorting 0x0126F5B4 slot 54 = 0x008F0EB0 (also 0x0126F68C / 0x0126F7D4 / 0x0126F9CC: other derived vtables). Header engine/card_sorting_types.h declares `virtual void CardsTurning();` in slot order (DealCards, CardsMoving[52], CardsFinishedMoving, CardsTurning[54], TurnCardsBack[55]). Manifest label is CORRECT. No direct callers (vtable only).

## 3. Callees
| addr | landed row | landed/PDB name | draft spelling |
|---|---|---|---|
| 0x008F0B20 | rebuild/src/compiled/00/8f/CTCTavernGameCardBase_GetCardDef_008f0b20.cpp | `?GetCardDef@CTCTavernGameCardBase@@QAEXPAV?$CDefPointer@$$CBVCTavernGameCardBaseDef@@@@@Z` (Ego_r 0x00916260) -- takes CDefPointer<CONST CTavernGameCardBaseDef>* | WRONG: non-const CDefPointer<CTavernGameCardBaseDef> |
| 0x005EA950 | 00/5e/CTCTavern­Game_PlaySoundW_005ea950.cpp | `?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z` (protected, returns unsigned long) | WRONG: `PlaySound` on CTCTavernGameCardBase |
| 0x005E9B20 | 00/5e/CTCTavernGame_SetState_005e9b20.cpp | `?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z` (enum param) | WRONG: `SetState(long)` on own class |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long) | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
| def vslot 1 (+4) | CDefPointer.h | CDefPointeeBase::Delete (shared engine/CDefPointer.h) | draft local `CDefBase::Destroy` |
| inlined CCard::Move 0x008F0660 | 00/8f/CCard_Move_008f0660.cpp | `?Move@CCard@@QAEXABVC3DVector@@_N@Z` (Ego_r 0x00915D30) | draft hand-expands it (no Move call) |

## 4. Members (Ego_r layout via ghidra_out/struct_layouts_egor.tsv; retail offsets)
- CTCTavernGame::TavernGameState +0xC0 (ETavernGameStates) -- engine/CTCTavernGame.h under FABLE_RETAIL_CTCTAVERNGAME (offsetof assert 0xc0); retail sizeof 0xF0. Draft calls it `State` (wrong).
- CTCTavernGameCardBase::Cards +0x128 (std::vector<CCard*>, STLport 0xc) -- engine/card_sorting_types.h (NO engine/CTCTavernGameCardBase.h exists; the class lives in card_sorting_types.h; opt-ins FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET, FABLE_RETAIL_CCARD_POSITIONS).
- CCard (card_sorting_types.h; size 0x58): MoveFraction +0x20 FLOAT (draft: `long TurnFrame` -- wrong name/type), Turning +0x24 bool, Moving +0x26 bool (draft `HasRestPosition`), LocalPos +0x27 (draft RestPosition), NewLocalPos +0x33 (draft TargetPosition), CurrentPos +0x3f (draft Position). C3DVector members need FABLE_RETAIL_CCARD_POSITIONS + FABLE_RETAIL_C3DVECTOR_PACKED.
- CTavernGameCardBaseDef (engine/CTavernGameCardBaseDef.h, FABLE_RETAIL_CTAVERNGAMECARDBASEDEF): CardSeparation +0x5C, TableSeparation +0x60 (draft: made-up CardStackHeightStep/CardStackBaseHeight -- wrong).
- Def refcount +4, Delete vslot 1: engine/CDefPointer.h (CDefPointeeBase::IVRefCount).
- Locally modelled in draft though shared headers exist: CDefBase/CDefPointer (engine/CDefPointer.h, fable_game.h), CTavernGameCardBaseDef, C3DVector (engine/C3DVector.h), CCard + CTCTavernGameCardBase + CCardVector (engine/card_sorting_types.h, std::vector from stlport/fable_containers.h), CCharString (fable_string.h + FABLE_RETAIL_CCHARSTRING), CTCTavernGame state/SetState/PlaySoundW (engine/CTCTavernGame.h).

## 5. Callee-body visibility (same TU = CTCTavernGameCardBase source file, retail 0x008F0620..0x008F2AC0)
- CCard::Move 0x008F0660 (defined earlier, out-of-line copy exists) -> retail INLINED it here: define its real body in the row TU before CardsTurning (as CardsMoving 0x008F2280 does; CardsMoving's Move body is the reference text).
- GetCardDef 0x008F0B20 (earlier, same TU) -> kept a CALL here, but its body (and GetSubDef template from engine/CDefClassBase_GetSubDef.h with FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE) being visible is what lets VC7.1 know &def does not escape -> def in EBP + def slot reused for the counter. This is exactly the ShuffleCards 0x008F10B0 recipe (its comment says so). If VC inlines GetCardDef here, use auto_inline/noinline control (CardsMoving uses `#pragma auto_inline(off/on)` around a stand-in).
- CCharString ctor/dtor, PlaySoundW, SetState: out of line (other TUs).
- No STLport out-of-line helpers involved (only begin/end/size inlines).

## 6. Sibling landed rows (rebuild/src/compiled/00/8f/)
- CTCTavernGameCardBase_ShuffleCards_008f10b0.cpp (next function, closest idiom, SHARED-HEADER model): defines FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CDEFSTRINGTABLE, _DEF_CLASS_BASE, _CDEFPOINTER_RELEASE_POINTEE, _CTCTAVERNGAME, _CTCTAVERNGAME_GETDEF_OUTOFLINE, _CTCTAVERNGAME_POSITIONMESH, _CTCTAVERNGAME_HIDEMESH, _CTAVERNGAMECARDBASEDEF, _CTAVERNGAMECARDBASEDEF_CARDPACK, _CTAVERNGAMECARDBASEDEF_TYPENAME, _CRESOURCELIST, _CDEFINITIONMANAGER, _CDEFCLASSBASE_GETSUBDEF_NOINLINE, _CGAMECOMPONENT, _CCLASSFACTORY, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CMAINGAMECOMPONENT, _SCOPED_PTR, _C3DVECTOR_PACKED, _CCARD_POSITIONS, _CTCTAVERNGAMECARDBASE_RHSET; includes stlport/fable_containers.h, fable_game.h, engine/CThingObject.h, CRightHandedSet.h, CCardDef.h, CTavernGameDef.h, CTavernGameCardBaseDef.h, card_sorting_types.h, CDefClassBase_GetSubDef.h; then real GetCardDef body `{ CDefPointer<const CTavernGameCardBaseDef> def; ParentThing->PeekBaseDef().GetSubDef(&def); *pdef = def; }`. Flags: none. Uses `(float)(53 - (int)i) * pdef->CardSeparation + pdef->TableSeparation`.
- CTCTavernGameCardBase_CardsMoving_008f2280.cpp (slot 52, same structure: settle loop `Moving == true` early return, state if-chain, `long i=0; for(it..; ++card, ++i) Move(C3DVector(x, y, (53 - i)*def->CardSeparation + def->TableSeparation), false)`, each branch `return`); local model (older style), Move body defined in TU.
- CTCTavernGameCardBase_TurnCardsBack_008f17d0.cpp (slot 55): named CCharString in its own block, pointer walk re-reading Cards.end().
- CTCTavernGameCardBase_TurnAllCards_008f09e0.cpp: named CCharString (temp reuses eax), explicit `== true` bool test.
- CCard_Move_008f0660.cpp: Move body (#pragma pack(1) model).

## 7. Old draft model errors (draft_original.cpp; DIFFER 487v506)
- GetCardDef declared without body / non-const type -> &def escapes -> def not held in ebp and its slot not reused (the main 19-byte gap). Fix: ShuffleCards recipe (real GetCardDef body + GetSubDef header in TU).
- Counter as `long height=0x35; --height` -> stored before the loop guard; retail needs `53 - i` with `++i` in the for header (store after hoisted X/Y copies).
- Hand-expanded Move instead of the inline CCard::Move(C3DVector(avgX,avgY,z), false) call (stackPos local vs ctor temp; retail builds Z per-iter into [esp+0x24], X/Y hoisted).
- Still-turning early return merged with common tail: retail has 3 release sites; the still-turning return must stay distinct (try: return inside loop with def dtor = natural; CardsMoving got duplicated per-branch tails by `return` in each branch -- but here 0x16/default SHARE one SetState call via cross-jump, while 0x19 has its own, so likely `if (==0x16) SetState(0x17); else if (==0x19){...; SetState(0x1a);} else SetState(0);` with def release in ebp only on the 0x19 path).
- Wrong names: State/PlaySound/SetState(long)/TurnFrame(long)/HasRestPosition/RestPosition/TargetPosition/Position/CardStackHeightStep/CardStackBaseHeight -> TavernGameState/PlaySoundW/SetState(ETavernGameStates)/MoveFraction(float)/Moving/LocalPos/NewLocalPos/CurrentPos/CardSeparation/TableSeparation.
- Local struct models for classes with shared headers (strict verifier rejects).

## 0x00929340

# DOSSIER 0x00929340 -- CAIStateGroup_BanditPlayDice::Update (507 bytes)

## 1. Retail shape (xdis 00929340 0x20b)
- __thiscall, `ret` (no args), NO frame pointer (sub esp,0x120; push ebx/ebp/esi/edi), no SEH/EH frame (no try state), so /Oy on, no /EH unwinding needed.
- Frame 0x120 locals: [esp+0x10] = conditional-temporary flag word (bit0 Sheathe temp, bit1 Unsheathe temp), byte `reached` at locals+7 (lea edx,[esp+0x2f] after 6 pushes),
  action temporary at locals+8 ([esp+0x18]) => temp size 0x118 = 0x120-8. Both temps share the slot.
- Two exits each `call 0x8FCF40 (Terminate)` then epilogue (duplicated epilogue at 0x9293c3 / 0x929530).
- Followed at 0x929540 by `add ecx,0x10; jmp GetPItem` (no xrefs) = an out-of-line copy of CTCDiceGame::GetCurrentPlayer() (FableWin ?GetCurrentPlayer@CTCDiceGame@@QAEPAVCThing@@XZ), same TU; then 0x929550 SetCurrentPlayer.

## 2. Identity
- Name: CAIStateGroup_BanditPlayDice::Update, leaf `?Update@CAIStateGroup_BanditPlayDice@@MAEXXZ` (protected virtual void Update(), non-const).
  Sources: Ego_r egor_pdb_names.tsv 0x0094c5a0; FableWin _fablewin_api.tsv 0x02c65cb0 / naming_stragglers; rtti-vtables.tsv vtable 0x0128AD54 slot 12 = 0x00929340
  (base CAIStateGroupBase slot 12 = pure Update). Manifest label (doubled class) is right in substance; the draft's `DiceMethods` stand-in is not the class.
- Vtable 0x0128AD54: 10 OnReplace 0x928E20, 11 IsRunnable 0x929020, 12 Update, 13 InitStateGroup 0x9284B0, 14 GetDiceGame 0x929220, 15 GetMarkerType 0x928550,
  16 PutWeaponAway 0x8FE7D5 (`return true`; FableWin ?PutWeaponAway@CAIStateGroup_BanditPlayDice@@MAE_NXZ), 17 Act 0x9285D0.
  Retail calls slots 16 (+0x40) and 17 (+0x44) VIRTUALLY -> declare `virtual bool PutWeaponAway();` and `virtual EAIActionReturn Act();` in the class in that order (after GetDiceGame, GetMarkerType).

## 3. Callees (landed file / PDB spelling)
| addr | landed row | PDB spelling | draft issue |
|---|---|---|---|
| 0xA01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (const, returns CBaseObject*) | draft returns CThing* |
| 0x916640 | 00/91/CTCExclusiveUse_SetUser_00916640.cpp | ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z | OK (out of line here; OnReplace row has it inline -- here it MUST be a call) |
| 0x4C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ (FableWin) | draft spells GetPosition -> use GetPos (engine/CThing.h declares `const C3DVector& GetPos() const`) |
| 0x97EC00 | 00/97/CAIStateGroupBase_AIActionMoveToPos_0097ec00.cpp | ?AIAction_MoveToPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@AA_NW4EMovementType@@_N4PBVCThing@@4M@Z | draft: long repeat, bool* reached, long movement -> must be EAIActionRepeat, bool&, EMovementType, const CThing* |
| 0x97D580 | 00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp | ?AIAction_LookAtPos@...W4EAIActionRepeat@@VC3DVector@@JM@Z | draft long repeat -> EAIActionRepeat |
| vslot 72 (+0x120) of PhysicsTC | (CTCPhysicsRigidBody_GetRHSet_0075bf70.cpp proves slot 72) | ?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ | draft spells GetOrientation on fake CThingPlacement -> GetRHSet() const; object = CThing::PhysicsTC (+0x60 retail) |
| 0x6A1540 | 00/6a/CTCCarrying_IsThingCarryingWeapon_006a1540.cpp | ?IsThingCarryingWeapon@CTCCarrying@@SI_NABVCThing@@@Z (static __fastcall, const CThing&) | draft free function taking CThing* |
| 0x8CC370 | 00/8c/CCreatureActionSheatheWeapons_..._008cc370.cpp | ??0CCreatureAction_SheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z | draft takes pointer -> reference |
| 0x8B8590 | 00/8b/CCreatureActionUnsheatheWeapons_..._008b8590.cpp (`ret 4`) | ??0CCreatureAction_UnsheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z | draft pointer -> reference |
| 0x97DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z | draft takes pointer (`&temp`) -> const CCreatureActionBase& |
| 0x62E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | ??1CCombatActionBase@@UAE@XZ (Sheathe/Unsheathe derive CCombatActionBase; dtor called direct, non-virtual on a temp) | draft calls it ~Sheathe (folded) |
| 0x929550 | 00/92/CTCDiceGame_SetCurrentPlayer_00929550.cpp | ?SetCurrentPlayer@CTCDiceGame@@QAEXPAVCThing@@@Z | OK |
| 0x8FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ (public) | OK |
- Enum: EAIActionReturn from engine/EAIAction.h: AI_ACTION_NULL 0, TERMINATE_PROCESS 1, AI_ACTION_FINISHED 2, FAILED 3, IN_PROGRESS 4. Draft's AI_ACTION_DONE is not a PDB name.
- Args: MoveToPos(1, pos, reached, 3 /*movement*/, true, true, 0, true, 0.0f) (pushes: 0,1,0,1,1,3,&reached,vec,1). LookAtPos(1, vec, 0x11, 0.0027777f /*0x3B360A13*/). PerformActionAndWait(temp, false).

## 4. Members
- this: engine/CAIStateGroup_BanditPlayDice.h under FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE (needs FABLE_RETAIL_CAISTATEGROUPBASE [+ _MEMBERS for Terminate/virtuals, _MEALACTIONS for MoveToPos/LookAtPos, _PERFORMACTION for PerformActionAndWait, _GETCREATURE + CAIBrain.h FABLE_RETAIL_CAIBRAIN_GETCREATURE for GetCreature()]).
  +0x04 PBrain (CAIBrain*, brain +0x18 Creature), +0x14 DiceGamePosition (CTCExclusiveUse*), +0x18 DiceGame (CTCDiceGame*), +0x24 WasCarryingWeapon (bool). The retail header declares only Act: Update / PutWeaponAway (and GetDiceGame/GetMarkerType slots 14/15) are NOT in it -- the row must add them (stand-in or header edit by coordinator).
- CTCExclusiveUse (Ego_r sizeof 0x14): CTCBase ParentThing +0x04 (engine/CTCBase.h `GetParentThing()` inline, FableWin), User CIntelligentPointer<CThing> +0x0C. No engine/CTCExclusiveUse.h exists.
- CTCDiceGame (Ego_r 0x18): NumberOfPlayers long +0x0C, CurrentPlayer CIntelligentPointer<CThing> +0x10. No engine header. FableWin inline accessors: GetNumberOfPlayers() (QAEJXZ), SetNumberOfPlayers(long), GetCurrentPlayer() (QAEPAVCThing@@XZ, non-const).
- CThing: PhysicsTC CTCPhysicsBase* at retail +0x60 (engine/CThing.h CheckRetailLayout asserts; GetPhysicsTC() const/non-const accessors present). CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE) declares `virtual const CRightHandedSet& GetRHSet() const`. CRightHandedSet.h / C3DVector.h exist (Forward at +0x0C: fld [edi+0xc..0x14]).
- Locally modelled in draft though shared headers exist: C3DVector, CRightHandedSet, CThing, CAIBrain, CBaseIntelligentPointer (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER), CCreatureActionBase, CAIStateGroupBase methods (DiceMethods), CCreatureAction_UnsheatheWeapons.h (flat, retail sizeof 0x118 noted in header).

## 5. Visibility
- GetPItem, SetUser, GetPos, SetCurrentPlayer, ctors, ~CCombatActionBase, AIActions all called out of line (declare only).
- CTCDiceGame::GetCurrentPlayer was visible inline (inlined at 0x9294ae: `lea ecx,[eax+0x10]; call GetPItem`) AND emitted out of line at 0x929540 right after this function => defined in this TU (inline member, header-visible). Use `DiceGame->GetCurrentPlayer() == GetCreature()` (OnReplace row 0x928E20 found this extra inline level matters).
- ++players: retail `mov ecx,[eax+0xc]; inc ecx; mov [eax+0xc],ecx` = load/inc/store -> strongly suggests `DiceGame->SetNumberOfPlayers(DiceGame->GetNumberOfPlayers() + 1)` (FableWin accessors). The decrement is `dec dword ptr [eax+0xc]` (memory form) -> try `--` direct or Set(Get()-1); test both.
- GetCreature: inline FableWin ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ -> PBrain->GetCreature() (reference).

## 6. Siblings
- 00/93/CAIStateGroupBanditSitRoundCampfire_Update_0093b1d0.cpp -- SAME pattern (MoveToPos(1,pos,arrived,3,true,true,0,true,0.0f) != 2; LookAtPos(1, lookAt, 0x11, 0.0027777f); `AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()), false) != 2`; CCombatActionBase sizeof 0x114, Sheathe adds bool at +0x114 => 0x118). Its LookAt: `C3DVector forward = orientation.Forward; C3DVector lookAt = forward + GetCreature().PeekPos();` (named locals). No pragmas.
- 00/92/CAIStateGroupBanditPlayDice_OnReplace_00928e20.cpp (DIFF notes on GetCurrentPlayer inline level), _Act_009285d0.cpp (AI_ACTION_FINISHED, engine/EAIAction.h, CIntelligentPointer retail macros), _PutWeaponAway_008fe7d5.cpp (stand-in class), _GetDiceGame_00929220.cpp. None use pragmas/Flags beyond defaults.

## 7. Draft model errors
1. Action temporaries: retail slot is 0x118 (CCombatActionBase 0x114 + bool/enum at +0x114); draft 0xa8+0x58+__int64 = 0x108 -> frame 0x110 v 0x120. Model CCombatActionBase as 0x114 with virtual dtor 0x62E900, Sheathe/Unsheathe adding one 4-byte member (bool PlayedSound / EUnsheatheWeaponType WeaponType); drop the __int64 hack.
2. LookAt order: retail calls GetRHSet (vslot 72) BEFORE pushing 0x3B360A13/0x11, then GetPos after `sub esp,0xc; mov ebp,esp`, and the sum is Forward(left, fld [edi]) + Pos(right, fadd [eax]). => hoist the orientation into a named reference local before the call (e.g. `const CRightHandedSet& rh = DiceGamePosition->GetParentThing().GetPhysicsTC()->GetRHSet();` or `const C3DVector& forward = ...Forward;`) and write `forward + DiceGamePosition->GetParentThing().GetPos()` inline in the argument (the C3DVector operator+ result constructed straight into the arg slot).
3. ++NumberOfPlayers -> Set/GetNumberOfPlayers accessors (see 5).
4. Signatures: references (bool& reached, const CCreatureActionBase&, CThingCreatureBase&, const CThing&), enums EAIActionRepeat/EMovementType, EAIActionReturn names.
5. Control flow otherwise matches retail: early `if (DiceGamePosition->User.GetPItem()) { DiceGame = 0; Terminate(); return; }` (shares the 0x9293b9 tail with the move-failure path which first SetUser(0)); WasCarryingWeapon = PutWeaponAway() && IsThingCarryingWeapon(GetCreature()) (materialised 1/0); `if (WasCarryingWeapon && PerformActionAndWait(Sheathe)!=2) {Terminate(); return;}`; Act(); --players; current-player clear; SetUser(0); DiceGame=0; `if (!(WasCarryingWeapon && PerformActionAndWait(Unsheathe)!=2)) WasCarryingWeapon = false;` Terminate().
   Note `WasCarryingWeapon = false` at entry is stored from ebx=0 before the first GetPItem.

## 0x009F4390

# DOSSIER 0x009F4390 CSurface::CopyFromMemory (507 bytes, ret 0x18)

## 1. Retail shape (python3 $S/w17/xdis.py 009f4390 0x20b)
- __thiscall, 6 stack args, `ret 0x18`. NO frame pointer (sub esp,0x3c ; push ebx/ebp/esi/edi), no SEH, no /Oy- pragma.
- Three epilogues (CopyFromUnsignedDataToSignedSurface path, CopyFromMemory(4-arg) path, D3DX path) -> flat early returns.
- Registers: esi=this, ebx=dstBox (loaded right after `push ebx`, before ebp/esi/edi pushes), edi=srcBox, ebp=&srcFormat
  (loaded at 0x9f4434 only after the first IsSigned call).
- Frame (offsets after all 4 pushes): [0x10]=dstH, [0x14]=srcH, [0x18]=1st GetPixelFormat temp, [0x1c..0x2b]=dst RECT,
  [0x2c..0x4b]=D3DSURFACE_DESC (shared by all GetDesc) and src RECT [0x2c..0x3b]. Arg slots: 0x50 dstBox, 0x54 data,
  0x58 srcBox, 0x5c srcFormat, 0x60 pitch, 0x64 arg6.
- DEAD-ARG-SLOT HOMING: dstW -> dstBox slot [0x50]; srcW -> srcBox slot [0x58]; 2nd/3rd GetPixelFormat temporaries built
  in the srcFormat slot [0x5c] (ebp already holds &srcFormat).
- Extents: dstBox ? (BRX-TLX, BRY-TLY) : two inline GetDesc (Width [desc+0x18], then a second GetDesc for Height [desc+0x1c]).
- srcBox != 0 falls through (width -> [0x58], height -> [0x14]). srcBox == 0 is an OUT-OF-LINE block at 0x9f44a3 (after the
  first epilogue): `mov [esp+0x58],ecx ; jmp 0x9f4409` -- it stores ONLY srcW = dstW. [0x14] (srcH) is NEVER written on that
  path, yet is compared later (0x9f44e6 cmp [0x14],[0x10]). => the original source did not assign srcSize.Y in the
  null-srcBox branch (e.g. `srcSize.X = dstSize.X;` only, or a C2DExtentsI with a user default ctor left half-filled).
  The old draft's `srcSize = dstSize;` stores both -> probable main residue cause (500v507 was blamed on regalloc).
- Signed test: tmp=GetPixelFormat() (inline: GetDesc + Initialise 0x9E3830, NO -1 store => ctor(_D3DFORMAT) form);
  `if (tmp.IsSigned() || srcFormat.IsSigned())` then `if (!(GetPixelFormat().IsSigned() && srcFormat.IsSigned()))`
  then `if (CanDoProperSignedConversion(dstBox, srcBox, srcFormat)) { CopyFromUnsignedDataToSignedSurface(data, srcFormat, pitch); return; }`.
  Any failure -> 0x9f44ac.
- 0x9f44ac: `if (srcFormat.IsEquivalent(GetPixelFormat()) && srcW==dstW && srcH==dstH) { CopyFromMemory(dstBox,data,srcBox,pitch); return; }`
  (ecx=ebp=srcFormat, arg = temp in [0x5c]; compares [0x58] vs [0x50], then [0x14] vs [0x10]).
- Tail: `xor ecx,ecx ; xor eax,eax` (pDst, pSrc zeroed up front), RECT copies in store order TLY->top, BRY->bottom,
  TLX->left, BRX->right (same as landed sibling CopyFromSurface 0x9F37E0), then
  D3DXLoadSurfaceFromMemory(PD3DSurface, 0, pDst, data, GD3DFormats[srcFormat.FormatIndex].D3DFormat (table 0x0129BA40, *36),
  pitch, 0, pSrc, arg6, 0). arg6 is passed as the D3DX *Filter*, ColorKey is literal 0.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv + egor_signatures.tsv, debug-build addr 0x00B48090; also FableWin stragglers 017b2a3c):
  `?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z`
  = public `void CSurface::CopyFromMemory(const C2DBoxI*, const void*, const C2DBoxI*, const CPixelFormat&, long, unsigned long)`.
- Second source: rebuild/include/engine/CSurface.h (FABLE_RETAIL_CSURFACE_COPYFROMMEMORY) declares exactly this @0x009F4390;
  landed caller CStaticFontBank::CStaticFontBank 0x00AB8E10 (rebuild/src/compiled/00/ab/CStaticFontBank_CStaticFontBank_00ab8e10.cpp:118)
  calls it with 6 args (..., sourceFormat, TextureSize.X*4, 0xFFFFFFFF). Manifest label is CORRECT.
- Param names in header: destRect, source, sourceRect, sourceFormat, sourcePitch, colourKey (but retail forwards arg6 as D3DX Filter).
- Not virtual (CSurface vtable 0x0122F84C has only the dtor).

## 3. Callees
| retail | PDB name | landed row | draft spelling |
|---|---|---|---|
| 0x009E3830 | ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z | 00/9e/CPixelFormat_InitialiseD3DFormat_009e3830.cpp | Initialise(unsigned long) -- WRONG param type, must be _D3DFORMAT |
| 0x009E3970 | ?IsSigned@CPixelFormat@@QBE_NXZ | 00/9e/CPixelFormat_IsSigned_009e3970.cpp | IsSigned() const OK |
| 0x009E37F0 | ?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z (bool IsEquivalent(const CPixelFormat&) const) | 00/9e/CPixelFormat_IsEquivalent_009e37f0.cpp (stub `S::IsEquivalent`) | `operator==` -- WRONG NAME |
| 0x009F35D0 | ?CanDoProperSignedConversion@CSurface@@AAE_NPBVC2DBoxI@@0ABVCPixelFormat@@@Z (PRIVATE; const CPixelFormat&) | 00/9f/CSurface_CanDoProperSignedConversion_009f35d0.cpp | OK (sibling CopyFromSurface uses a pointer; PDB says reference) |
| 0x009F36E0 | ?CopyFromUnsignedDataToSignedSurface@CSurface@@AAEXPBXABVCPixelFormat@@J@Z (PRIVATE; const void*, const CPixelFormat&, long) | 00/9f/CSurface_CopyFromUnsignedDataToSignedSurface_009f36e0.cpp (landed spells const unsigned char*, unsigned long -> NAME_FIXES candidate) | draft OK vs PDB |
| 0x009F3B00 | ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0J@Z (4-arg OVERLOAD; manifest functions.tsv: CSurface::CopyFromMemory) | NOT landed | `CopyFromMemoryDirect` -- WRONG NAME, it is the CopyFromMemory overload |
| 0x00BFEFC8 | _D3DXLoadSurfaceFromMemory@40 import thunk | 00/bf/global_D3DXLoadSurfaceFromMemory_00bfefc8.cpp (toolchain_library) | OK |
| COM | IDirect3DSurface9::GetDesc slot 12 (`call [ecx+0x30]`, this pushed) | - | OK |

## 4. Members
- CSurface +0x04 PD3DSurface (IDirect3DSurface9*) -- struct_layouts_egor.tsv:34706; engine/CSurface.h (private in RETAIL path).
- C2DBoxI +0/+4/+8/+0xC = TLX, TLY, BRX, BRY (engine/C2DBoxI.h, Ego_r TPI). Draft uses left/top/right/bottom -> wrong names.
  Header opt-in FABLE_RETAIL_C2DBOXI offers inline GetExtents() (C2DExtentsI(BRX-TLX,BRY-TLY)).
- C2DExtentsI X,Y with a USER default ctor `C2DExtentsI() {}` (engine/C2DExtentsI.h) -- lets `C2DExtentsI srcSize;` be left
  half-assigned legitimately; FABLE_RETAIL_C2DEXTENTSI_INITIALISE gives Initialise(x,y) + copy ctor.
- CPixelFormat FormatIndex (+0, protected in engine/CPixelFormat.h); GD3DFormats[] CD3DFormatInfo table 0x0129BA40
  (engine/CD3DFormatInfo.h), D3DFormat field +0.
- D3DSURFACE_DESC Width +0x18, Height +0x1c (engine/Direct3DSurface9.h / Direct3D9Types.h, used by Clear 0x9F40A0).
- CSurface.h opt-ins: FABLE_RETAIL_CSURFACE (master), _COPYFROMMEMORY (inline dtor + this decl), _PIXELFORMAT (GetPixelFormat
  DECL only, out of line -- retail here expands it inline, so the author must supply an inline body/derived class),
  _BITMAP (GetDimensions decl + inline GetWidth/GetHeight via GetDimensions), _CLEAR, _SWIZZLE. No decl for
  CanDo/CopyFromUnsigned/4-arg CopyFromMemory -> add in a derived class or local wrapper as siblings do.
- Draft models LOCALLY (shared headers exist): CSurface, CPixelFormat, C2DBoxI, C2DExtentsI, CSurfaceLockedRect,
  IDirect3DSurface9/D3DSURFACE_DESC (engine/Direct3DSurface9.h), CRGBColour.

## 5. Visibility
- GetPixelFormat (out of line copy 0x009F2F60) and GetWidth/GetHeight/GetDimensions (0x00415BD0/0x00415BF0/0x009F2F90) are ALL
  expanded inline here (3 GetPixelFormat + 2 GetDesc for size, no calls) -> provide inline bodies (not the header decls).
- Same TU (CSurface.cpp band 0x009F2D60..0x009F45xx): Lock 0x9F33E0, CanDo 0x9F35D0, CopyFromUnsigned 0x9F36E0, CopyFromSurface
  0x9F37E0, CopyFromMemory(4) 0x9F3B00 all defined EARLIER in the same TU; all called out of line here. Landed CopyFromUnsigned
  needed Lock's body visible (its retry 10). Here only the callee-reads-args question matters: if retail had CanDo/CopyFromMemory(4)
  bodies visible, VC7.1 might know they don't alias the extents -- try `__declspec(noinline)` same-TU bodies only if the
  frame/homing still differs.
- No STLport involvement.

## 6. Siblings (rebuild/src/compiled/00/9f/)
- CSurface_CopyFromSurface_009f37e0.cpp -- matched twin of the D3DX tail; local classes, `_D3DFORMAT` enum, RECT pointers zeroed
  up front, ctor CPixelFormat(_D3DFORMAT){Initialise(f);}. No pragmas.
- CSurface_CanDoProperSignedConversion_009f35d0.cpp -- `#define CSurface CSurfaceLayout / #include engine/CSurface.h / #undef`,
  derived class with method decls; "inline-once band" notes; temporaries in dead sourceFormat slot. No pragmas.
- CSurface_Clear_009f40a0.cpp / CSurface_Swizzle_009f4330.cpp -- shared headers via FABLE_RETAIL_CPIXELFORMAT,
  _CPIXELFORMAT_COLOURDEPTH, FABLE_RETAIL_CSURFACE, _CSURFACE_PIXELFORMAT, _CSURFACE_CLEAR (+_SWIZZLE).
- CSurface_ClearArea_009f4190.cpp -- local-class model, inline GetWidth/GetHeight GetDesc reads, counter in dead arg slot.
- CSurface_CopyFromMemoryAutoSwizzle_009f3470.cpp -- CPixelFormatLayout derived class, `#pragma auto_inline(off)` around a
  same-TU noinline Lock body.
- None use `// Flags:` or optimize pragmas (default /O2 lane flags).

## 7. Old draft model errors
1. `else { srcSize = dstSize; }` -- retail stores ONLY srcSize.X (=dstW); srcSize.Y is left unassigned on the null-srcBox
   path. Write `srcSize.X = dstSize.X;` only (C2DExtentsI with user default ctor). Most likely cause of the 7-byte miss and of the
   register/homing differences.
2. operator== must be IsEquivalent(const CPixelFormat&) const (PDB). Call shape: srcFormat.IsEquivalent(GetPixelFormat()).
3. CopyFromMemoryDirect must be the overload CopyFromMemory(const C2DBoxI*, const void*, const C2DBoxI*, long).
4. Initialise(unsigned long) -> Initialise(_D3DFORMAT); D3DSURFACE_DESC.Format typed _D3DFORMAT.
5. C2DBoxI members TLX/TLY/BRX/BRY (not left/top/...); use engine headers or the CanDo-row derived-class idiom.
6. CanDo/CopyFromUnsigned are private in PDB (AAE) -- harmless for codegen.
7. arg6 named `filter` in the draft, `colourKey` in the header; retail passes it as D3DX Filter (keep header name or document).

## 0x00641C30 (not landed)

# DOSSIER 0x00641C30 -- CTCLight::OnAppearanceDraw (490 bytes)

## 1. Retail shape (xdis 00641c30 0x1fa)
- thiscall, `ret 8` (2 args), returns bool (`mov al,1` / early `pop;...;ret 8` with al=0 from IsResourcesLoaded).
- NO frame pointer: `sub esp,0x40 ; push ebp ; mov ebp,ecx` -- ebp holds `this`. No SEH prologue. No /Os idioms. Plain /O2: no pragma.
- `this` = the CAGraphicAppearanceComponent subobject at CTCLight+0x0c (vtable 0x0125A208 slot 0; slot 1 = OnAppearanceClear 0x00641210).
  Evidence: `lea ecx,[ebp-0xc]` (CTCLight this for DrawLight/DrawFlare), `[ebp+0x10]` = CTCLight+0x1c Active, `[ebp-8]` = CTCLight+0x04 ParentThing.
- Dead arg slots reused: lightsDrawn counter homed in the appearance arg slot ([esp+0x54]; appearance itself lives in esi),
  flare count homed in the info arg slot ([esp+0x58]; info in edi). Light count at [esp+0x10]. Loop index in ebx (both loops).
- Fallback: ONE DrawLight(0, tmp, appearance, info) call at 0x00641DF1..0x00641E09 reached from both branches: class-2 branch computes
  X,Y,Z into ecx/edx/eax then `jmp 0x641df1`; else branch `call 0x4c73d0` (CThing::GetPos, out of line) then loads [eax],[eax+4],[eax+8]
  into ecx/edx/eax; merge stores them into a 12-byte local [esp+0x14] and passes its address. => source shape is a single
  `C3DVector pos = cond ? <class2 expr> : GetPos();` (or if/else assigning one local) followed by ONE DrawLight -- old draft had two copies.
- Class-2 expression order in retail: (1) virtual `PhysicsTC->vtbl[+0x114]` (slot 69) with hidden return ptr + float info.Interpolate, result *0.35f
  (const 0x0125A264 = 0.35f) componentwise; (2) ParentThing reloaded ([ebp-8]) and PhysicsTC reloaded; (3) inline (Position-OldPosition)*t+OldPosition
  (+0x0c/+0x18 of physics); (4) add facing*0.35; (5) Z += 1.0f (const 0x0122DED8 = 1.0f). Z+1 applied last, after the add.
- Ego_r.pdb size for comparison: 0x00683430..0x00683610 = 480.

## 2. Identity
- Name: CTCLight::OnAppearanceDraw. Leaf `?OnAppearanceDraw@CTCLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z`.
  Sources: (a) Ego_r.pdb egor_pdb_names.tsv 0x00683430 (same source order: DrawLight 682980, DrawFlare 683090, AttachParticles, SetActive, OnAppearanceDraw);
  (b) rtti-vtables.tsv CTCLight 0x0125A208 slot 0 = 0x00641C30, slot 1 = 0x00641210 (landed OnAppearanceClear) -- matches
  CAGraphicAppearanceComponent's slot order (OnAppearanceDraw, OnAppearanceClear) in rebuild/include/engine/NGraphicAppearance_CAGraphicAppearanceComponent.h.
  Manifest label is CORRECT. Only reference: the vtable (xref.py: 0x0125A208 .rdata).
- Signature: `virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance&, const NGraphicAppearance::CGraphicAppearanceDrawInfo&)` (non-const method).
- Secondary base REAL name: NGraphicAppearance::CAGraphicAppearanceComponent (rebuild/include/engine/CTCLight.h: `class CTCLight:public CTCBase,public NGraphicAppearance::CAGraphicAppearanceComponent`). Draft's "CAppearanceListener" is invented.

## 3. Callees
| addr | role | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|---|
| 0x004BE3E0 | IsResourcesLoaded(true) | rebuild/src/compiled/00/4b/NGraphicAppearanceCTCGraphicAppearance_IsResourcesLoaded_004be3e0.cpp | CTCGraphicAppearance::IsResourcesLoaded(bool) **const** | ?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z | ok name, draft missing const |
| 0x004BDE90 | count helpers, args (name, 1.0f) | rebuild/src/compiled/00/4b/NGraphicAppearanceCTCGraphicAppearance_GetNoHelperPoints_004bde90.cpp | GetNoHelperPoints(const CCharString&, float) const | ?GetNoHelperPoints@CTCGraphicAppearance@NGraphicAppearance@@QBEJABVCCharString@@M@Z | **WRONG in draft: GetNoDummyObjects** (that is the byte twin 0x004BE000) |
| 0x004C0560 | GetHelperPoint(name, i, 0, pos, info.Interpolate) | not landed | -- | ?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z (Ego_r 0x004C1780) | draft takes `C3DVector*` and non-const: PDB is `C3DVector&`, const |
| 0x006414A0 | DrawLight(i, pos, appearance, info) | not landed (hand326 NOTES: DIFFER(-v1596)) | -- | ?DrawLight@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z | ok |
| 0x00641AE0 | DrawFlare | rebuild/src/compiled/00/64/CTCLight_DrawFlare_00641ae0.cpp | CTCLight::DrawFlare | ?DrawFlare@CTCLight@@IAEX...@Z | ok |
| 0x004C73D0 | thing position (out of line) | rebuild/src/compiled/00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const -> const C3DVector& | ?GetPos@CThing@@QBEABVC3DVector@@XZ (declared in CThing.h line ~411) | **WRONG in draft: GetPosition** |
| vtbl+0x114 on PhysicsTC | slot 69 | 0x006B1430 rebuild/src/compiled/00/6b/CTCPhysicsBase_GetFacingVector_006b1430.cpp | CTCPhysicsBase::GetFacingVector(float) const | ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@M@Z | **WRONG in draft: "GetInterpolatedVelocity"** -- it is the facing vector (rtti CTCPhysicsBase 0x0125D2C4 slot 69 = 0x006B1430, CTCPhysicsStandard slot 69 = 0x00723C20 GetFacingVector) |
- Globals: 0x013BACC8 = `MESH_LIGHT_POINT_NAME`, 0x013BACCC = `MESH_FLARE_POINT_NAME` (const CCharString; rebuild/src/compiled/01/21/staticinit_MESHLIGHTPOINTNAME_0121eac0.cpp / staticinit_MESHFLAREPOINTNAME_0121eae0.cpp, Ego_r + FableWin + Anniv PDB `?MESH_LIGHT_POINT_NAME@@3VCCharString@@B`). Draft's g_LightHelperName/g_FlareHelperName are wrong; no shared header declares them -> `extern const CCharString MESH_LIGHT_POINT_NAME;` locally.
- Float consts 0x0125A264 = 0.35f, 0x0122DED8 = 1.0f (read from Fable.exe).

## 4. Members
- CTCLight (rebuild/include/engine/CTCLight.h, size 68 retail): Active (bool, +0x1c; struct_layouts_egor CTCLight +28 Active). Accessed at subobject+0x10.
  Header declares DrawLight/DrawFlare (protected), IsActive() (declared, no body). DrawFlare row uses this header; copy its defines.
- CTCBase::ParentThing +0x04 (CTCBase.h; FABLE_RETAIL_CTCBASE; GetParentThing() inline accessor available, see CTCBase.h ~line 138).
- CThing (CThing.h, FABLE_RETAIL_CTHING): CClass::Type uchar +0x10 (CClass.h, FABLE_RETAIL_CCLASS; inline `unsigned long GetType() const`); retail compares `cmp byte ptr [ecx+0x10],2`. PhysicsTC +0x60 (protected; GetPhysicsTC() inline); GetPos() const out of line 0x004C73D0.
- CTCPhysicsBase (CTCPhysicsBase.h): FABLE_RETAIL_CTCPHYSICSBASE gives virtual GetFacingVector(float) const (slot 69, header line 126); Position +0x0c, OldPosition +0x18;
  FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION + _BODY give inline `C3DVector GetPosition(float) const {return (Position-OldPosition)*t+OldPosition;}` (exactly retail's fsub/fmul/fadd).
  Model: landed rebuild/src/compiled/00/7e/NTargeting_GetTargetingPositionOnThing_007ea2f0.cpp (`thing.GetPhysicsTC()->GetPosition(scale) + C3DVector(...)`).
- CGraphicAppearanceDrawInfo::Interpolate float +0 (struct_layouts_egor; size 36).
- CTCGraphicAppearance header opt-ins: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES (IsResourcesLoaded, GetNoHelperPoints), FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL (GetHelperPoint, native 004c0560 signature), _MOVEMENT/_FLARE (used by DrawFlare row).
- C3DVector: engine/C3DVector.h with FABLE_RETAIL_C3DVECTOR.
- Draft models LOCALLY with shared headers available: C3DVector, CCharString (fable_string.h), CTCGraphicAppearance, CGraphicAppearanceDrawInfo, CTCPhysicsBase, CThing, CTCBase, CTCLight, CAGraphicAppearanceComponent ("CAppearanceListener").

## 5. Visibility
- GetPos 0x004C73D0 is called out of line in retail (the declaration in CThing.h has no body -- keep it so).
- GetPosition(float) is expanded inline (retail inlines the interp math) -> needs _INTERPOLATED_POSITION_BODY.
- GetFacingVector is a true virtual call (+0x114).
- DrawLight 0x006414A0 / DrawFlare 0x00641AE0 precede this function in the same TU (CTCLight.cpp; retail order DrawLight, DrawFlare, OnAppearanceDraw) but are called, not inlined (large).
- No STLport helpers called. IsResourcesLoaded/GetNoHelperPoints/GetHelperPoint are other-TU (0x004Bxxxx) calls.

## 6. Siblings (rebuild/src/compiled/00/64/)
- CTCLight_DrawFlare_00641ae0.cpp: `// Flags: none`; defines FABLE_RETAIL_BASE_OBJECT, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CLIGHTDEF, _C3DVECTOR,
  _CENGINEPERSISTENTPRIMITIVEBASE, _CIVCOUNTEDPOINTER, _CTCGRAPHICAPPEARANCE_MOVEMENT, _CTCGRAPHICAPPEARANCE_FLARE, ...; includes engine/CThing.h, engine/CTCLight.h,
  engine/NGraphicAppearance_CTCGraphicAppearance.h. BEST template (uses shared CTCLight.h, GetParentThing()).
- CTCLight_OnAppearanceClear_00641210.cpp: local model (pre-header), documents the +0x0c subobject `this`.
- CTCLight_SetActive_006412b0.cpp (`#pragma auto_inline(off/on)` around a helper), CTCLight_AttachParticles_00641260.cpp (FABLE_RETAIL_CTHING), CTCLight_ApplyDefActiveState_00641480.cpp.
- 0x007EA2F0 NTargeting_GetTargetingPositionOnThing: interpolated-position opt-in idiom.

## 7. Old draft model errors
- Two DrawLight calls in effect (separate copy C3DVector(position.X,...) after an if/else) -> retail has one tail-merged call; write one local assigned in both branches then one DrawLight, or a `?:`.
- Wrong callee names: GetNoDummyObjects->GetNoHelperPoints, GetPosition->GetPos, GetInterpolatedVelocity->GetFacingVector(float), helper-name globals -> MESH_LIGHT_POINT_NAME / MESH_FLARE_POINT_NAME.
- GetHelperPoint takes C3DVector& (not pointer); IsResourcesLoaded/GetNoHelperPoints/GetHelperPoint are const.
- Invented base "CAppearanceListener"; local CThing/CTCLight/CTCPhysicsBase layouts instead of shared headers.
- Class-2 computation: retail calls GetFacingVector first, scales by 0.35, then reloads ParentThing->PhysicsTC for the inline GetPosition(t), adds, then Z+1.0 last.
  `ThingClass` is CClass::Type (GetType()).
- Loop form fine (for, count cached, index ebx, `i<count` test at bottom with a top guard `jle`).

