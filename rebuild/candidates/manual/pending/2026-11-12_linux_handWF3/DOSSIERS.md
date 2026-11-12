## 0x00931200

# DOSSIER 0x00931200 -- CAIStateGroup_MinionReactToCriminal::IsStillValid() const

## 1. Retail shape (xdis 00931200 0x193)
- 387 bytes (0x931200..0x931382) + int3. thiscall, no params, plain `ret`. NO frame pointer (FPO), NO SEH. `sub esp,8` = two
  frame dwords: [esp+8]/[esp+0x10] = spill of `this` (stored at entry, `mov [esp+8],ebp`), [esp+0x14] = GetTC key (0x23).
- Register plan: ebp=this, ebx=&PHome (+0x1c) for the first block; esi/edi pushed only AFTER the home-alive test (the early
  `!alive` exit 0x931376 pops only ebp/ebx). Inside the guard loop ebp is reused for PHome.GetPItem() and reloaded from [esp+0x10].
- Two tails, both `mov ecx,ebp ; call 0x930EE0 (IsValid) ; return its al`: list-end exit 0x93135D, and break/no-village/no-member
  exit 0x931374 (pop edi/esi) / 0x931376 (home dead). So source = `if (IS_THING_ALIVE(PHome)) {...} return IsValid();`.
- Flow (exact order):
  1. PHome.GetPItem() != 0 && !(GetPItem()->Dead +0x91 bit0)  (GetPItem called twice, inline IS_THING_ALIVE shape).
  2. creature = PBrain(+4)->PCreature(+0x18) loaded into esi FIRST, then PHome.GetPItem() -> edi, then creature->GetPos()
     (0x4C73D0), then edi->IsPointInside(pos) (0x82D000) -> `mov [ebp+0x2E],al`  MeInsideHouse.
  3. HeroInsideHouse (+0x2D) = int-valued && (`mov eax,1 / xor eax,eax`, stored as al): PHero(+0x14) alive (GetPItem x2,
     +0x91 bit0) && PHome.GetPItem() [called BEFORE] ->IsPointInside(PHero.GetPItem()->GetPos()).
  4. `mov byte [ebp+0x2F],0` GuardInsideHouse = false (after HeroInsideHouse store).
  5. PBrain->PCreature reloaded; `test byte [eax+24h],8` = HasTC(TCI_VILLAGE_MEMBER=35=0x23); key store 0x23 inside the test;
     `lea esi,[eax+44h]` map pointer, LowerBound 0x40F020, `mov esi,[esi+4]` end via pointer; `cmp eax,esi ; je` /
     `cmp [eax],23h ; jle` / `mov eax,esi` -> component `mov esi,[eax+4]` = CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED GetTC body
     (ternary pick). Result bool is NOT tested (no branch on it): `CTCVillageMember* m; if (GetTC(TCI_VILLAGE_MEMBER,&m) && ...)`
     -- the true-return is folded. Miss path jumps straight to tail 0x931374.
  6. `if (m->GetPVillage())` then GetPVillage() AGAIN (0x7EE540 twice) -> ebx = village; list node word at village+0x2C
     (CTCVillage::VillagePeople), `ebx += 0x2c` (pointer to list), first test `cmp edi,eax` (head), loop test `cmp edi,[ebx]`
     re-reads end() through the list pointer every iteration => `const list& people = ...; for (it=people.begin(); it!=people.end(); ++it)`.
  7. Loop body: esi = &node->value (+8, CIntelligentPointer<CThingAICreature>); alive (GetPItem x2, +0x91 bit0); then
     GetPItem() again `test dword [eax+34h],100000h` = HasTC(TCI_GUARD=180) as a DWORD test (word 5 bit 20); then
     PHome.GetPItem() FIRST (`lea ecx,[ebp+1Ch]` -> ebp), then p.GetPItem()->GetPos(), then IsPointInside. On true:
     `mov ebp,[esp+10h]` (before jne) then at 0x93136C `mov eax,[esp+10h] ; mov byte [eax+2Fh],1` (GuardInsideHouse=true; break).

## 2. Identity (two+ sources) -- manifest label is CORRECT
- Ego_r egor_pdb_names.tsv 0x009544F0 `?IsStillValid@CAIStateGroup_MinionReactToCriminal@@UBE_NXZ`; FableWin _fablewin_api.tsv
  0x02B5A2F0 same leaf; vtable 0x0128B1F4 slot 5 = 0x00931200 (rtti-vtables.tsv), base slot 5 = CAIStateGroupBase::IsStillValid()
  const 0x008FD276 (`mov al,1`, engine/CAIStateGroupBase.h:56). Ego_r order IsValid 0x954190 < IsRunnable 0x9542E0 < IsStillValid
  0x9544F0 matches retail 0x930EE0 < 0x931010 < 0x931200.
- Signature: `public: virtual bool IsStillValid() const` (UBE_NXZ). It WRITES HeroInsideHouse/MeInsideHouse/GuardInsideHouse
  although const -> needs `mutable` members or a const_cast of this (unproven which; a named non-const `self` local could also
  explain the entry spill of this -- hypothesis only). No xrefs (virtual only; xref.py lists only the vtable .rdata 0x0128B208).
- IsValid: `?IsValid@CAIStateGroup_MinionReactToCriminal@@ABE_NXZ` = PRIVATE CONST non-virtual (Ego_r 0x954190, FableWin 0x02B59DB0).

## 3. Callees
| retail | landed row | PDB / landed name | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `CBaseObject* CBaseIntelligentPointer::GetPItem() const` | draft returns CThing* (wrong type; use CIntelligentPointer<T>::GetPItem via engine/CIntelligentPointer.h) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | **`CThing::GetPos() const`** `?GetPos@CThing@@QBEABVC3DVector@@XZ` (engine/CThing.h:411) | draft `GetPosition()` non-const -- WRONG name (manifest Ego_r label at that VA is a propagated vector::_Ufill) |
| 0x0082D000 | 00/82/CThingBuilding_IsPointInside_0082d000.cpp | `bool CThingBuilding::IsPointInside(const C3DVector&) const` (engine/CThingBuilding.h:43) | draft on CThing, non-const -- wrong class/const |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | `CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&)` non-const | draft local CInterfaceMap::LowerBound(long*) -- wrong |
| 0x007EE540 | 00/7e/CTCVillageMember_GetPVillage_007ee540.cpp | `CTCVillage* CTCVillageMember::GetPVillage() const` | draft returns local `CVillage*` -- wrong class |
| 0x00930EE0 | 00/93/CAIStateGroupMinionReactToCriminal_IsValid_00930ee0.cpp | `bool IsValid() const` private | draft non-const; the landed IsValid row also declares it non-const (IsRunnable row 0x931010 declares `bool IsValid() const`) |

## 4. Members
- this: engine/CAIStateGroup_MinionReactToCriminal.h (generated, flat, no FABLE_RETAIL_* opt-ins; PHero/PHome are opaque
  `unsigned char[8]`, base flattened). Ego_r struct_layouts: PHero +0x14 CIntelligentPointer<CThingPlayerCreature>, PHome +0x1c
  CIntelligentPointer<CThingBuilding>, FrameLastLeftStategroup +0x24, FrameToNextSpeak +0x28, OverrodeLocking +0x2c bool,
  HeroInsideHouse +0x2d bool, MeInsideHouse +0x2e bool, GuardInsideHouse +0x2f bool. The landed siblings (0x930EE0, 0x931010)
  model the class locally deriving a local CAIStateGroupBase because the header flattens the base.
- CAIStateGroupBase::PBrain +0x04 (engine/CAIStateGroupBase.h, opt-ins FABLE_RETAIL_CAISTATEGROUPBASE /
  _MEMBERS for virtual slots); CAIBrain PCreature +0x18 (engine/CAIBrain.h; IsGuardOnLevel row calls it `PBrain->Creature`).
- CThing (engine/CThing.h): UsingComponents std::bitset<274> +0x20, Components CVectorMap +0x44, Dead:1 at +0x91 (IsDead()/
  IsAlive()); GetTC(ETCInterfaceType, T**) expanded body under FABLE_RETAIL_CTHING_GETTC_EXPANDED (matches step 5 exactly);
  HasTC forceinline variant FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- CTCVillage (engine/CTCVillage.h, FABLE_RETAIL_CTCVILLAGE): VillageBuildings +0x28, **VillagePeople +0x2c**
  std::list<CIntelligentPointer<CThingAICreature>> (retail list = one node word); inline `PeekVillagePeople() const` under
  FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE (FableWin.pdb ?PeekVillagePeople@CTCVillage@@QBEABV?$list...). Draft's
  "CVillage::Members +0x2c" = VillagePeople.
- Interface ids (engine/ETCInterfaceType.h): TCI_VILLAGE_MEMBER = 35 (0x23), TCI_GUARD = 180 (0xb4).
- IS_THING_ALIVE: engine/ThingAlive.h under FABLE_RETAIL_IS_THING_ALIVE (template IS_THING_ALIVE<T>(const T&): `thing!=0 &&
  !thing->IsDead()`), FASTCALL inline.
- Draft models LOCALLY (all have shared headers): CThing, C3DVector (engine/C3DVector.h), CBaseIntelligentPointer
  (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER), CVectorMap (CInterfaceMap -> engine/CVectorMap.h),
  CTCVillage ("CVillage"), CTCVillageMember (engine/CTCVillageMember.h), CAIBrain, list (stlport/fable_containers.h via CTCVillage.h).

## 5. Visibility
- All callees out of line in retail: GetPItem, GetPos (7-byte inline-looking accessor emitted out of line at 0x4C73D0 -- must be
  only declared, not defined), IsPointInside, LowerBound (non-const 0x40F020), GetPVillage, IsValid. IsValid (0x930EE0) is the same
  TU, defined EARLIER, but called, not inlined (300 bytes). The GetTC/HasTC/IS_THING_ALIVE/list iterator are inlined.
- STLport list iteration fully inline (no out-of-line list helpers).

## 6. Siblings
- 00/93/CAIStateGroupMinionReactToCriminal_IsValid_00930ee0.cpp (same class/TU): local CThing with GetInterfaceX ternary form,
  local IS_THING_ALIVE inline, includes engine/ETCInterfaceType.h + engine/CVectorMap.h.
- 00/93/CAIStateGroupMinionReactToCriminal_IsRunnable_00931010.cpp: FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h,
  local class deriving local CAIStateGroupBase, `bool IsValid() const`.
- 00/93/CAIStateGroupMinionReactToCriminal_OnReplace_00930e20.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CThing.h.
- 00/95/CAIStateGroupMinionReactToCrime_IsGuardOnLevel_00958c00.cpp: same village/guard loop (VillagePeople +0x2c, UsingComponents[5]
  & 0x100000, GetPVillage twice) -- copy its loop idiom.
- 00/6c/CTCBuyableHouse_FrameUpdate_006c1250.cpp uses PeekVillagePeople; 00/95/CAIStateGroupMinionReactToBurglary_IsRunnable_00957f20.cpp
  uses GetPos(0x4C73D0)+IsPointInside on a home building. 00/93/CAIStateGroupMinionGuardReactToBribe_IsStillValid_00933470.cpp
  (same slot-5 override; FABLE_RETAIL_CTHING + FABLE_RETAIL_CINTELLIGENTPOINTER). No `// Flags:` / pragmas in these siblings.

## 7. Old draft model errors
- Non-const IsStillValid/IsValid (PDB: both const). Wrong names GetPosition (GetPos), CVillage/Members (CTCVillage/VillagePeople),
  IsPointInside on CThing (CThingBuilding, const), GetPItem typed CThing*.
- Loop end: draft's iterator end() from `m_pHead` of a reference -- OK in shape, but retail re-reads `[ebx]` each iteration via
  list pointer; first comparison uses the already loaded head.
- Guard test: draft `HasTC(0xb4)` returns the masked dword -- matches `test dword` (keep the dword mask form; bitset::test emits shr/and).
- Hero flag must be int-valued `&&` (draft OK). Home GetPItem must be evaluated before the hero/guard GetPItem+GetPos (draft writes
  `Home().GetPItem()->IsPointInside(Hero...GetPos())` -- VC evaluates object first: OK).
- Register residue (358v387): retail keeps this in ebp and spills it at entry; draft's helper struct `...Methods` derived from the
  flat header may change the this-pointer plan; first block `creature` must be loaded before PHome.GetPItem().

## 0x009DC6E0

# DOSSIER 0x009DC6E0 CRenderManager2D::Draw2DTexture (box overload), 388 bytes

## 1. Retail shape (xdis 009dc6e0 0x194)
- __thiscall, `ret 10h` (4 stack args), returns `mov al,1` (bool true). No frame pointer (`sub esp,90h`, esp-relative), no SEH, no pragmas needed.
- Frame 0x90: box out at [esp+0Ch..18h] (TLX,TLY,BRX,BRY); vertex array v[4] at [esp+1Ch] (v0), 3Ch (v1), 5Ch (v2), 7Ch (v3), 32 bytes each, ascending = a `CTVertexRHWColSpecTex1 v[4]` array (LESSONS 0x009DDFF0: array gives ascending order).
- Order: colour packed FIRST (arg2): `movzx edx,[c+1](G); xor ecx,ecx; mov ch,[c+3](A); mov cl,[c+2](R); shl 8; or G; shl 8; or B` -> esi.
  Then `GFVBoxToABox(box=arg1 in ecx, &out in edx)` (fastcall).
- Store order after the call (byte offset in vertex): v0.X,v0.Y,v1.X,v2.X,v2.Y (copy chain through ecx/edx/eax), v0.RHW (eax = [0139652C]), v1.Y, v3.X;
  then eax=0 (xor) used for: v0.Spec, v1.Z, v1.Spec, v2.Z, v2.Spec, v3.Z, v3.Spec; then `mov al,[flags]; test al,2` interleaved;
  v3.Y; IMMEDIATES: v0.U=0, v0.V=0, v1.U=1.0, v1.V=0, v2.U=1, v2.V=1, v3.U=0, v3.V=1, then v0.Z=0 (immediate, late, after eax clobbered by flags);
  then v0.Diffuse=esi, v1.RHW=ecx, v1.Diff, v2.RHW=edx, v2.Diff, v3.RHW=ecx, v3.Diff.
  RHW global read TWICE (mov eax,[0139652C] and mov ecx,[0139652C]) and copied.
- Mode: `test al,2; je; esi=2 / else esi = ((flags&1)<<1)|1` -> branchless `(flags&1)?3:1` inner (movzx/and/shl/or). Equivalent of draft's if/else-if/else.
- Two `Add2DTri(v0,v1,v2,mode,texture)` / `(v0,v2,v3,mode,texture)` calls; texture = arg3 loaded once into ebx (`mov ebx,[esp+0A8h]`), pushed as pointer.

## 2. Name / signature (two sources + callers)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00B302F0) and FableWin (ghidra_out/_fablewin_api.tsv 0x01835AEA):
  `?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@AAVCTexture@@K@Z`
  = `bool CRenderManager2D::Draw2DTexture(const C2DBoxF&, const CRGBColour&, CTexture&, unsigned long)` public.
- Ego_r order: 0x00B301C0 (C2DVector* overload) < 0x00B302F0 (THIS) < 0x00B30450 (CUVBox overload = retail 0x009DC870, landed). Retail neighbour 0x009DC870 confirms.
- Callers (xref): 0x00494AD7 (CFrontendGameComponent::Output, landed, line 139), 0x004989C2..0x00499145 (CProgressDisplay::DrawRetailDisplay 0x00498490, landed), 0x00B4D356. Both landed callers use `#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX`.
- Manifest label is CORRECT. Old draft's `CTexture* texture` is WRONG: PDB says `CTexture&` (pass `&texture` to Add2DTri, like the landed UV sibling).

## 3. Callees
- 0x009E1E70 GFVBoxToABox: landed rebuild/src/compiled/00/9e/global_GFVBoxToABox_009e1e70.cpp as `void __fastcall GFVBoxToABox(const C2DBoxF& v, C2DBoxF& a)`; PDB `?GFVBoxToABox@@YIXABVC2DBoxF@@AAV1@@Z` (egor 0x00B34C30, FableWin 0x017B1079). Draft spells out param as `C2DBoxF*` -> wrong (should be `C2DBoxF&`). No header declares it; declare locally with FABLE_FASTCALL.
- 0x009DC030 CRenderManager2D::Add2DTri: landed rebuild/src/compiled/00/9d/CRenderManager2D_Add2DTri_009dc030.cpp; PDB `?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z` (PRIVATE, long mode, CTexture*). Declared in engine/CRenderManager2D.h under FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ (private).
- Global 0x0139652C: landed spelling `g_ScreenRHW` (extern float, declared in engine/CTVertexRHWColSpecTex1.h line 53 inside the SETRHWFROMZ block, else declare locally). Possibly per-TU const BIGGEST_RHW_VERTEX_VALUE (unproven; keep g_ScreenRHW).
- Inline colour pack: PDB `?GetD3DCOLOR@CRGBColour@@QBEKXZ` (FableWin 0x017C9372). Draft spells it `GetARGB` -> WRONG name; use `FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR` + `colour.GetD3DCOLOR()`.

## 4. Members / headers (struct_layouts_egor.tsv line 35486)
- CTVertexRHWColSpecTex1Base (32): X+0, Y+4, Z+8, RHW+0C (float), DiffuseColour+10, SpecularColour+14 (ulong), U1+18, V1+1C (float). Header engine/CTVertexRHWColSpecTex1.h (CTVertexRHWColSpecTex1 : CGenericVertex<Base>, with declared-only setters SetColour, SetU1V1, SetXYZ, SetRHW...; only opt-in FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ). Draft field names Diffuse/Specular/U/V are WRONG -> DiffuseColour/SpecularColour/U1/V1.
- C2DBoxF {TLX,TLY,BRX,BRY}: engine/C2DBoxF.h (has user default ctor C2DBoxF(){}; opt-ins FABLE_RETAIL_C2DBOXF, _COPY, ...).
- CRGBColour {B,G,R,A}: engine/CRGBColour.h (opt-in FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR).
- CRenderManager2D: engine/CRenderManager2D.h; opt-ins FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX (declares this overload, line 67-72) + FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ (private Add2DTri).
- CTexture: engine/CTexture.h (forward use only).
- Draft models LOCALLY (all have shared headers): C2DBoxF, CRGBColour, CTexture, CTVertexRHWColSpecTex1, CRenderManager2D.

## 5. Visibility
- No out-of-line inline helpers: only GFVBoxToABox (separate TU, 0x009E1E70, global GF* file) and Add2DTri (same TU, earlier address 0x009DC030, 645 bytes, not inlined in retail -> a declaration suffices). No STL calls. GetD3DCOLOR inline.

## 6. Siblings (same TU, CRenderManager2D 2D file)
- 0x009DC870 Draw2DTexture(UV) rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DTexture_009dc870.cpp: local types, no pragmas/flags; lever = per-vertex store ORDER (Diffuse,X,Y first; U,V,Z,RHW after). Copy its param/`&texture`/mode idioms.
- 0x009DD960 Draw2DLine (..._Draw2DLine_009dd960.cpp): THE shared-header idiom: `#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR`, `#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DLINE`, `#include "fable_compiler.h"`, engine/C2DVector.h, CRGBColour.h, CTVertexRHWColSpecTex1.h, CRenderManager2D.h. Lever: two plain reads of g_ScreenRHW AFTER the Z zeroes gives one load + register copy; reading before gives two loads. There retail used ecx-zero for all Spec/Z/U/V zeroes.
- 0x009DDA50 Draw2DBoxColours / 0x009DDFF0 Draw2DTriangle: store order of field assignments is the lever; vertex ARRAY for ascending frame.
- 0x009DE3F0 Draw2DBox (forwarder to Draw2DBoxWithZ).

## 7. Old draft's likely model errors
- Residue (hand249 NOTES): DIFFER 384v388: zero-constant register choice (retail: Spec and v1..v3 Z via xor-eax zero, U/V and v0.Z immediates) + copy-chain scheduling. Draft's statement order (per vertex X,Y,Z,RHW,Diffuse,Spec,U,V) is not retail's schedule; reorder per section 1: X/Y copies + RHW, then Spec/Z zeroes (v0.Spec, v1.Z, v1.Spec, ... ; v0.Z separate/late), then U/V constants, then Diffuse/RHW. Note v0.Z is the odd one out (immediate, stored after U/V): try writing v0.Z after the U/V block, or Spec before Z for v0.
- The RHW global is read twice in retail (two loads, eax and ecx) -> at least two separate reads at different points in source (Draw2DLine lever).
- Colour packed before GFVBoxToABox (draft already does) -- keep.
- Wrong spellings: GetARGB->GetD3DCOLOR, Diffuse/Specular/U/V->DiffuseColour/SpecularColour/U1/V1, CTexture*->CTexture&, GFVBoxToABox out param ref not pointer; local types -> shared headers.
- Mode: draft's if/else-if/else is fine semantically; retail inner is branchless (flags&1)?3:1.

## 0x00761980

# DOSSIER 0x00761980 CTCShapeManager::DrawShapes (389 bytes, ret 8)

## 1. Retail shape (xdis 00761980 0x195)
- __thiscall, `ret 8` (2 args), NO frame pointer (sub esp,0x64 then push ebx/ebp/esi/edi), NO SEH, no /Oy- pragma needed.
- Frame 0x64 exactly = locals (base offsets after the 4 pushes): shapeColour +0x10, selectedColour +0x14, ITERATOR +0x18 (spilled),
  map (const CWorldMap*) +0x1c, first point copy +0x20..0x28, lifted point +0x2c..0x34, CEnginePrimitiveLine temp +0x38..0x73 (0x3c bytes).
- Registers: edi=this, ebp=&info (loaded once), esi=shape (per iteration), bl=0xff and al=0x50 (CSE'd byte constants), viewPos re-read
  from its arg slot every iteration ([esp+0x7c]); in the tail edi is reused for &viewPos.
- Colour byte stores go R,G,B,A order ([+2],[+1],[+0],[+3]) -> CRGBColour(r,g,b,a){R=r;G=g;B=b;A=a;} (shared header default ctor; also
  matches FABLE_RETAIL_CRGBCOLOUR Initialise order). shape = (0x50,0x50,0xff,0xff) blue; selected = (0xb4,0xa0,0x50,0xff);
  line red (0xff,0,0,0xff) built in the dead `info` arg slot; highlight white (ff,ff,ff,ff) built in the dead `viewPos` arg slot.
- Loop: `it=begin; mov [esp+18],it; je tail; jmp body; top: mov eax,[esp+18]; jmp body; (align) body:` ... `it+=8; cmp it,[edi+10h]; jne top`
  -> for-loop, end (_M_finish +0x10) re-read each iteration (after virtual calls), iterator lives in memory.
- shape pointer cached in esi across the vcall and reused for Points (+8/+0xc) without reload of *it -> source names a local
  (`NShape::CShape* shape = it->Get()` / `NShape::CShape& shape = **it`), not repeated `(*it)->`.
- Point lift: memberwise copy *Points.begin() into a frame C3DVector (+0x20), then C3DVector(first.X, first.Y, first.Z + 0.15f) in a
  separate slot (+0x2c); 0.15f = [0x0126A1F4] (same constant as landed 0x007617A0), 0.05f pushed as immediate 0x3d4ccccd.
- Engine: `mov edx,[ebp+34h]; mov esi,[edx]` (vtable fetched BEFORE building args), ctor, then `mov ecx,[ebp+34h]` reload, `call [esi+50h]`.
- `ParentThing` (+0x04) loaded into ecx before the red colour stores, then call 0x004C73D0.
- Tail: `cmp [edi+18h],[edi+10h]` = SelectedShape != PersistantShapes.end().

## 2. Identity (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00772C30) AND FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x017E6206 / 0x025D6F60):
  `?DrawShapes@CTCShapeManager@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@@Z`
  = private: void __thiscall CTCShapeManager::DrawShapes(const CEnginePrimitivePrepareInfo&, const C3DVector&). Label is RIGHT.
- Ego_r neighbour order DrawShapes 0x772C30 -> EditorDrawPreparePrimitivesForRendering 0x772E20 -> DrawPreparePrimitivesForRendering
  0x772EA0; retail DrawShapes 0x761980 -> DrawPreparePrimitivesForRendering 0x761B10 (only caller, call at 0x761B38; vtable 0x0126A2A4 slot 11,
  rebuild/manifest/rtti-vtables.tsv). Ego_r body 0x1F0 bytes vs retail 389.
- DRAFT ERRORS in signature: draft has non-const `CEnginePrimitivePrepareInfo&` (PDB: const&). Landed caller
  rebuild/src/compiled/00/76/CTCShapeManager_DrawPreparePrimitivesForRendering_00761b10.cpp declares DrawShapes(CEnginePrimitivePrepareInfo*,
  const C3DVector*) (pointer types, public) -> NAME_FIXES candidate (PDB: AAE private, const refs).

## 3. Callees
| retail | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00686A80 | rebuild/src/compiled/00/68/CTCBase_PeekWorld_00686a80.cpp | CTCBase::PeekWorld() const -> const CWorld& | ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (Ego_r+FableWin) | WRONG: draft `GetWorld()` (GetWorld is 0x00686A70) |
| CWorld vslot 13 (+0x34) | engine/CWorld.h | CWorld::PeekWorldMap() const -> const CWorldMap& (0x004AE8C0) | same | WRONG: draft `GetWorldMap()` returning CWorldMap* (slot 12 is GetWorldMap) |
| NShape::CShape vslot 3 (+0x0c) | engine/NShape_CClosedShapeBase.h | EditorDrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&, const CWorldMap&, const C3DVector&, const CRGBColour&) | ?EditorDrawPreparePrimitivesForRendering@CClosedShapeBase@NShape@@UAEX...@Z (landed 0x00761560) | WRONG: draft `EditorDraw` (descriptive) |
| 0x007617A0 | rebuild/src/compiled/00/76/CShape_EditorDrawHighlightPoints_007617a0.cpp | CShape::EditorDrawHighlightPoints (pointer params, non-namespaced CShape) | ?EditorDrawHighlightPoints@CShape@NShape@@QAEXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@ABVCRGBColour@@@Z (Ego_r+FableWin) | draft name OK but params non-const; NOT declared in shared NShape_CClosedShapeBase.h (needs an opt-in decl) |
| 0x004C73D0 | rebuild/src/compiled/00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const -> const C3DVector& (engine/CThing.h:411) | inline accessor, no out-of-line PDB name; catalog CThing::GetPos | WRONG: draft `GetPosition` |
| 0x004C3090 | rebuild/src/compiled/00/4c/CEnginePrimitiveLine_CEnginePrimitiveLine_004c3090.cpp | CEnginePrimitiveLine ctor (10 params, ret 2Ch) | ??0CEnginePrimitiveLine | OK (out of line: use engine/CEnginePrimitiveLine.h WITHOUT FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT so it stays a declaration) |
| CIEngine vslot 20 (+0x50) | engine/CIEngine.h line ~94 | CIEngine::AddPrimitive(const CEnginePrimitive&) | ?AddPrimitive@CIEngine (header chunk 731) | WRONG: draft `AddPrimitive(CEnginePrimitiveLine*)` (pointer); landed 0x7617A0 calls it `SetPrimitive` (also wrong) |

Ctor arg order (pushes): id 0 (two dword 0), &top, GetPos(), &red, 0.05f, blendMode 0, flags 0, depthTest 1, interp 0, blendOp 0 -> as in draft
and landed 0x00761560 (`(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0`).

## 4. Members
- CTCBase::ParentThing +0x04 (CThing*): engine/CTCBase.h:199 (also inline GetParentThing() const, line 144). CTCBase.h declares PeekWorld protected unguarded.
- CTCShapeManager (struct_layouts_egor: PersistantShapes +12 vector<CCountedPointer<NShape::CShape>>, SelectedShape +28 iterator; retail
  +0x0c and +0x18 since STLport vector is 12 bytes, iterator = raw pointer): engine/CTCShapeManager.h models them as PRIVATE OPAQUE
  `unsigned char PersistantShapes[12]; unsigned char SelectedShape[4];`. Opt-ins offered: FABLE_RETAIL_CTCSHAPEMANAGER (+ _TRACK,
  _GETINSTANTIATIONFROMENUM, _VSLOTS) -- NONE types these members and DrawShapes is not declared. The author needs a new nested opt-in
  (e.g. FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES) that declares DrawShapes and types the two members as
  `std::vector<CCountedPointer<NShape::CShape> > PersistantShapes; std::vector<...>::iterator SelectedShape;` (stlport/fable_containers.h:
  vector::iterator is `_Tp*`, empty() / front() / begin() exist; CCountedPointer is {Data,Info} 8 bytes, engine/CCountedPointer.h operator-> / Get()).
- NShape::CShape (engine/NShape_CClosedShapeBase.h, sizeof 32 retail): Owner (CTCShapeManager&) +4, Points std::vector<C3DVector> +8/+0xc,
  SelectedPoints +0x14. vslot order: ~CShape 0, GetType 1, OnSerialise 2, EditorDrawPreparePrimitivesForRendering 3, IsPosInShape 4.
- CEnginePrimitivePrepareInfo::Engine +0x34: engine/CEnginePrimitivePrepareInfo.h default = `CIEngine* Engine` (static-asserted);
  FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO gives PDB form `CIEngine& Engine` (pulls CInterpolationInfo/CCamera).
- CWorld vtable: engine/CWorld.h (slot 13 PeekWorldMap unguarded). CIEngine slot 20 AddPrimitive: engine/CIEngine.h (sibling 0x761560 defines
  FABLE_RETAIL_CIENGINE, _RENDER, _CONVERTSCREENPOS, _CONVERTWORLDPOS, _SCREENPOS to get the full table).
- CRGBColour: engine/CRGBColour.h 4-arg ctor (R,G,B,A store order matches retail); FABLE_RETAIL_CRGBCOLOUR adds Initialise + 3-arg ctor (A=255).
- Draft LOCALLY models (all have shared headers): C3DVector, CRGBColour, CEnginePrimitiveLine, CIEngine, CEnginePrimitivePrepareInfo,
  CWorld, NShape::CShape, CThing, CTCShapeManager (+ local CCountedPointer/vector/iterator stand-ins).

## 5. Callee visibility
- 0x007617A0 EditorDrawHighlightPoints: same TU, defined earlier (retail order ..0x761560 CClosedShapeBase::EditorDraw.., 0x761620 CTrack slot3,
  0x761740, 0x7617A0, 0x761980 DrawShapes, 0x761B10 caller, 0x761B50 GetBestTrack); called out of line (467 bytes, non-inline member).
- 0x004C73D0 CThing::GetPos: inline accessor in PDBs, emitted out of line in retail (aligned in CThing range) -> keep declaration-only (engine/CThing.h).
- 0x00686A80 PeekWorld, 0x004C3090 line ctor: out-of-line, declaration only (do NOT enable FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT, that inlines the ctor).
- No STLport helper calls: vector begin/end/empty all inline.

## 6. Siblings (same TU neighbourhood, rebuild/src/compiled/00/76/)
- NShapeCClosedShapeBase_EditorDrawPreparePrimitivesForRendering_00761560.cpp: `// Flags: none`; shared headers only; defines FABLE_RETAIL_CIENGINE_SCREENPOS,
  FABLE_RETAIL_C3DVECTOR_MAGNITUDE_DECLARATION, FABLE_RETAIL_CIENGINE, _RENDER, _CONVERTSCREENPOS, _CONVERTWORLDPOS, FABLE_RETAIL_CWORLDMAP; includes
  NShape_CClosedShapeBase.h, CIEngine.h, CEnginePrimitivePrepareInfo.h, CEnginePrimitiveLine.h, CWorldMap.h; spells
  `info.Engine->AddPrimitive(CEnginePrimitiveLine(0,a,b,colour,0.05f,(EEnginePrimitiveBlendMode)0,0,true,(EEngineInterpolationSet)0,(EEnginePrimitiveBlendOp)0));`
  BEST TEMPLATE.
- CTCShapeManager_DrawPreparePrimitivesForRendering_00761b10.cpp (caller; local models, pointer-param DrawShapes decl).
- CTCShapeManager_GetBestTrack_00761b50.cpp (engine/CCountedPointer.h + local vector; `#pragma auto_inline(off)` around same-TU __find_if).
- CTCShapeManager_FrameUpdate_00760990.cpp, CTCShapeManager_IsPosInAnyShape_00760870.cpp (local models; layout +0x0c/+0x10/+0x18 confirmed).
- CShape_EditorDrawHighlightPoints_007617a0.cpp (local models; same 0.15f constant).

## 7. Old draft model errors (vs disassembly)
- Retail spills the iterator (+0x18) and keeps 0xff in bl / 0x50 in al; draft got iterator in ebx (frame 0x60). Retail frame 0x64 = exactly the
  locals above, so retail has the iterator as a stack local with no other extra local. Candidates (untested): typed STLport vector
  `std::vector<CCountedPointer<NShape::CShape> >::iterator it`, `const CWorldMap& map = PeekWorld().PeekWorldMap();` (reference, real header),
  real PDB const-ref types everywhere, the CShape pointer local, `Points.front()`/`*Points.begin()` with `if (!shape->Points.empty())`.
- Wrong callee spellings: GetWorld (should be PeekWorld), GetWorldMap (PeekWorldMap, const CWorldMap&), EditorDraw (EditorDrawPreparePrimitivesForRendering),
  GetPosition (GetPos), AddPrimitive(CEnginePrimitiveLine*) (AddPrimitive(const CEnginePrimitive&) taking the temp by reference).
- `info` non-const in draft; PDB const&. `&CEnginePrimitiveLine(...)` (address of temporary) should be a by-reference temporary.
- Local struct models for 9 classes with shared headers (strict verifier rejects).

## 0x00A89740

# DOSSIER 0x00A89740 C3DMesh2::GetHelperPointPositionAnimated (one-state overload), 389 bytes

## 1. Retail shape (xdis 00a89740 0x195)
- __thiscall, `ret 0x10` = hidden C3DVector* result + 3 args. No frame pointer (no push ebp), no SEH, `sub esp,0x58; push esi`.
  Plain /O2-style; no optimize("s") idioms. Bytes end 0x00A898C4 (389), int3 pad to 0x00A898D0 (next vtable slot 4 fn).
- esi = helper.BoneIndex (+0xC); `cmp esi,-1; je tail` -> tail (0x00A898AA) copies helper.Position to *result with 3 integer moves.
  Fall-through = animated path (so `if (bone != -1) {...} return helper.Position;` or early-return form; the -1 path is placed last).
- Animated path, in order:
  a. pos copy: x fld then spilled `fstp [arg2 slot]` (dead helper-ptr slot), y `fst [esp+4]`, z integer-moved to pos.Z (+0x10).
     = `C3DVector pos(helper.Position)` + TransformPoint's xx/yy/zz copies (zz aliases pos.Z).
  b. `lea edx,&scale; push edx` (last call arg pushed BEFORE the transform maths).
  c. &Bones[bone].LocalizationMatrix: `Bones` read at this+0x9C, `imul 0x3C`, `+0xC`.
  d. three rows, each term order E3k*z, E2k*y, E1k*x, + E4k (X: 0x18,0x0C,0x00,+0x24; Y: 0x1C,0x10,0x04,+0x28; Z: 0x20,0x14,0x08,+0x2C)
     results stored to pos (+8,+0xC,+0x10). This is EXACTLY the shared header's `FABLE_RETAIL_CMATRIX3X4` default (#else)
     `TransformPoint(C3DVector&)`: `E1k*xx+(E2k*yy+E3k*zz)+E4k` for all three rows (CMatrix3x4.h ~l.164).
  e. push &transform(+0x2C), push bone(esi), push this, push state(arg3), ecx=manager(arg4); call 0x00AA0FA0.
  f. scaled = scale.X*pos.X ... (`fld scale.k ; fmul pos.k` -> scale is LEFT operand), each product stored to a temp
     (+0x14/+0x18/+0x1C) then integer-copied to pos (+8/+0xC/+0x10) AND x,y to the arg2/arg3 slots (xx/yy of 2nd TransformPoint).
     = `pos = scale.<GetScaled>(pos);` (temp + assignment) then in-place TransformPoint (xx,yy spilled, zz aliases pos.Z).
  g. second transform with the local matrix, same E3k,E2k,E1k order; final sums stay on x87, `fxch st(2)` + 3 fstp to *result
     -> `return pos;` after the in-place TransformPoint (same epilogue idiom as landed 0x00A35F80 / 0x00A11AE0).
- Frame map (pre-push esp): +4 yy spill, +8..+0x10 pos, +0x14..+0x1C scale*pos temp, +0x20..+0x28 scale, +0x2C..+0x58 transform.

## 2. Identity (label is RIGHT)
- vtable C3DMesh2 0x0129DEF0 slot 5 = 0x00A89740 (rebuild/manifest/rtti-vtables.tsv); slot 4 = 0x00A898D0 (two-state overload).
  Only xref: .rdata 0x0129DF04 (the vtable). No direct callers.
- Mangled leaf (Ego_r egor_pdb_names.tsv @00bf68b0 AND FableWin _fablewin_api.tsv / FableWin.exe.signatures.tsv @017a0c4c/03364b20):
  `?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@@Z`
  = public virtual const; returns C3DVector; (const C3DHelperPoint&, const C3DAnimationBlendState&, C3DAnimationManager&).
  ret 0x10 rules out the 5-arg overload (`...1MAAV...`, two blend states + float).
- FableWin debug copy 03364b20 is 210 bytes (/Od), consistent with the short source above.

## 3. Callees
- 0x00AA0FA0 C3DAnimationManager::CalcBoneTransformation -- NOT landed (only ctor 0x00AA0F60 landed in 00/aa). No shared header
  C3DAnimationManager.h. PDB (Ego_r + FableWin): `?CalcBoneTransformation@C3DAnimationManager@@QAEXABVC3DAnimationBlendState@@ABVC3DMesh2@@KAAVCMatrix3x4@@AAVC3DVector@@@Z`
  = `void CalcBoneTransformation(const C3DAnimationBlendState&, const C3DMesh2&, unsigned long, CMatrix3x4&, C3DVector&)`.
  **Draft spelling WRONG: draft uses pointers (const C3DAnimationBlendState*, const C3DMesh2*, ..., CMatrix3x4*, C3DVector*)** -- PDB says references (same bytes, wrong spelling).
- Inline (no call emitted): CMatrix3x4::TransformPoint(C3DVector&) x2, C3DVector copy ctor, C3DVector::GetScaled (FableWin
  `?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z`, returns const C3DVector; NO shared-header body exists -- grep GetScaled in rebuild/include = none).
  Possibly C3DMesh2::GetBone (FABLE_RETAIL_C3DMESH2_BONES: `const C3DBone2* GetBone(ulong) const {return &Bones[index];}`).
  No componentwise C3DVector operator*(C3DVector) in either PDB catalog: the draft's operator* is invented; use GetScaled.

## 4. Members
- C3DMesh2::Bones: retail +0x9C (PDB Ego_r +0xA4; retail C3DMesh2 is 8 bytes smaller). Shared header rebuild/include/engine/C3DMesh2.h
  names `C3DBone2* Bones` at +0x9C (UnmodelledBase[0x7c] then handles...); opt-ins: FABLE_RETAIL_C3DMESH2_BONES (GetBone/GetBoneCount),
  FABLE_RETAIL_C3DMESH2_STATS, FABLE_RETAIL_C3DANIMATIONINFO_BUILD. Header has NO base class and NO virtual declarations, and no
  GetHelperPointPositionAnimated declaration -- an opt-in would be needed to declare the member (or follow C3DMeshStats.h's
  FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS, which declares the 6 pure virtuals on C3DMeshStats (needs FABLE_RETAIL_CMOVABLERESOURCE);
  C3DMesh2.h does not derive from it -- sizeof(C3DMeshStats)=124=0x7c=UnmodelledBase).
- C3DBone2 (0x3C): NameCRC +0, Parent +4, OriginalNoChildren +8, CMatrix3x4 LocalizationMatrix +0xC (struct_layouts_egor.tsv). No shared header.
- C3DHelperPoint (0x10): C3DVector Position +0, long BoneIndex +0xC. No shared header.
- CMatrix3x4: shared rebuild/include/engine/CMatrix3x4.h, flat E11..E43 (row = axis; E1k=XAxis +0, E2k +0xC, E3k +0x18, E4k=Position +0x24).
  Opt-in FABLE_RETAIL_CMATRIX3X4 (default branch) gives the exact row form needed. Draft models XAxis/YAxis/ZAxis/Position locally -> use header.
- C3DVector: shared engine/C3DVector.h (X,Y,Z), FABLE_RETAIL_C3DVECTOR.
- Draft models LOCALLY classes that have shared headers: C3DVector, CMatrix3x4, C3DMesh2 (also C3DAnimationBlendState.h exists, forward-decl only needed).

## 5. Visibility
- CalcBoneTransformation: other TU (C3DAnimationManager, 0x00AA0FA0), called out of line -- declaration only.
- TransformPoint / GetScaled: no out-of-line retail copy (CMatrix3x4.h comment: no retail ?TransformPoint copy); inline/forceinline.
- No STLport helpers involved.

## 6. Siblings
- rebuild/src/compiled/00/a1/CPhysicsMeshObject_GetBoundingBoxCentre_00a11ae0.cpp: `// Flags: none`; defines FABLE_RETAIL_CMATRIX3X4,
  FABLE_RETAIL_C3DVECTOR, ...; writes TransformPoint's xx/yy/zz body on a named local then `return centre;` (same fxch st(2)+3 fstp).
- rebuild/src/compiled/00/a3/C3DMeshObject_GetBoundingCentre_00a35f80.cpp (named result, memberwise copy ctors).
- rebuild/src/compiled/00/b3/CEngineInternalAttachedPrimitive_GetParentTransformAnimatedMesh_00b39080.cpp: uses
  FABLE_RETAIL_CMOVABLERESOURCE + FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS + FABLE_RETAIL_CMATRIX3X4 (+_SETMULTIPLY).
- Same-class rows 00/a8/C3DMesh2_*.cpp (CalculateBoneKeyframes 0x00A8A9D0, GetUsedBones 0x00A8B5F0, RelocateData 0x00A8B4C0) all model C3DMesh2 locally.
- Twins parked: 0x00A898D0 (slot 4, DIFFER 393v402), 0x00A89A70/0x00A89E40/0x00A8A220 (x87 operand order) -- same TU, same problem class.

## 7. Old draft model errors
- Uses a by-value `CMatrix3x4::operator*(C3DVector)` returning a fresh vector: retail is in-place TransformPoint with xx/yy/zz
  copies (spills: xx->arg slot, yy fst, zz = pos.Z integer copy) -- that is exactly what the draft's residue note describes as
  "spill" differences. Use `C3DVector pos(helper.Position); Bones[bone].LocalizationMatrix.TransformPoint(pos);`.
- Invented componentwise operator*; retail = temp then copy (`pos = scale.GetScaled(pos)`, scale left operand), then
  `transform.TransformPoint(pos); return pos;`.
- CalcBoneTransformation spelled with pointers; PDB uses references (state, *this, bone, transform, scale).
- Local CMatrix3x4/C3DVector/C3DMesh2 instead of shared headers; local C3DMesh2 with `virtual v00` + pad (fine for bytes, flagged by verifier).

## 0x009122A0

# DOSSIER 0x009122A0 CBezierCurve::GetPointOnCurve (390 bytes)

## 1. Retail shape (xdis 009122a0 0x196)
- __thiscall, `ret 8`: hidden result pointer (C3DVector*, read late at 0x009123BF `mov eax,[esp+0x3c]` after pop esi) + float t.
- No frame pointer (`sub esp,0x34 ; push esi`), no SEH, no /Oy- pragma. Frame 0x34 + esi. Ends 0x00912426 (int3 pad).
- Constants: double 3.0 @0x012368F0 (pow exponent, both calls), float 3.0f @0x0122DED4, float 1.0f @0x0122DED8.
- Callee: 0x00BFEB40 `_CIpow` twice (x87-stack args: fld base ; fld 3.0 ; call) -> intrinsic pow.
- Evaluation order (rightmost term first, then left-assoc sum):
  1. pow(t,3.0) * Point4 (+0x24/+0x28/+0x2c) -> frame [0x20..0x28].
  2. Point3*3.0f (+0x18..), then fld t; fmul t -> t*t stored to [esp+4]; x,y,z * (t*t) (memory operand);
     then 1.0f - t -> `fst [esp+4]` (reuses the t*t slot), x,y,z * (1-t) -> [0x14,0x18,0x1c].
  3. fld [esp+4] (1-t reloaded onto x87 BEFORE Point2 loads); Point2*3.0f (+0xc..), each *t (t read from arg slot [esp+0x40]);
     then (1-t)*(1-t) computed as `fld st1; fmul st2` and SPILLED to the dead t argument slot [esp+0x40]; x,y,z * [esp+0x40].
  4. fld (1-t) [esp+4]; fld 3.0; call _CIpow -> pow(1-t,3) * Point1 (+0,+4,+8).
  5. Sum ((T1+T2)+T3)+T4, stores straight into *result (eax).
- Semantics: Point1*(1-t)^3 + Point2*3*t*(1-t)^2 + Point3*3*t^2*(1-t) + Point4*t^3. Draft arithmetic/order is right;
  only the term-2 x87 schedule differs (358 v 390).

## 2. Name / signature (two sources, manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00898c60, Ego_r copy 0x1D0 = 464 bytes) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x01793e1b):
  `?GetPointOnCurve@CBezierCurve@@QBE?AVC3DVector@@M@Z` = `C3DVector CBezierCurve::GetPointOnCurve(float) const` (public, non-const return).
- Manifest rebuild/manifest/functions.tsv: 009122a0 GetPointOnCurve CBezierCurve __thiscall C3DVector (agrees).
- Not virtual (CBezierCurve has no vtable). Caller: only 0x009132F2 inside CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos (0x009130D0, unlanded).
- In Ego_r it sits between vector<CThingBrushInfo>::push_back and CEditControlCentre (an editor TU) => it is an INLINE member
  defined in the CBezierCurve header, emitted as COMDAT per TU. In retail it is emitted at the start of the
  CAIStateGroup_FlyingThingBase TU block (0x009126C0 ctor, 0x00912700, 0x00912840, 0x00912BA0, 0x009130D0 follow).
- Other CBezierCurve members (FableWin): ctor(const C3DVector&x4) ??0CBezierCurve@@QAE@ABVC3DVector@@000@Z, copy ctor,
  GetPoint1/3/4() const -> const C3DVector&, SetPoint1..4(const C3DVector&). None needed here (retail reads members by offset).

## 3. Callees
- 0x00BFEB40 `_CIpow`: landed rebuild/src/compiled/00/bf/global_CIpow_00bfeb40.cpp (toolchain_library import thunk; name MSVCR71 _CIpow).
  Spell as `extern "C" double __cdecl pow(double, double);` (+ optional `#pragma intrinsic(pow)`, as std_pow_00c40e50.cpp does);
  22 landed rows use that exact extern line with comment `// _CIpow 0x00BFEB40`.
- C3DVector::operator*(float) const: FableWin ??DC3DVector@@QBE?BV0@M@Z (returns CONST C3DVector) -- header has it (FABLE_VECTOR_INLINE = __forceinline).
- C3DVector::operator+(const C3DVector&) const: FableWin ??HC3DVector@@QBE?BV0@ABV0@@Z (const return) -- header has it.
- pow note: engine/C3DVector.h includes <math.h>; VC7.1 C++ math.h adds float pow(float,float)/pow(float,int) overloads.
  `pow(1.0f - t, 3.0)` (float,double) is ambiguous between double pow(double,double) and float pow(float,float) under those
  overloads -- check it compiles; retail likely wrote `pow(x, 3.0f)` (float overload -> powf -> (float)pow((double)x,(double)3.0f),
  giving the same double 3.0 constant) or cast the base to double. pow(t,3) (int) would emit _Pow_int, not _CIpow -> excluded.
  FableWin has ?pow@@YAMMM@Z and ?pow@@YAMMH@Z (global float overloads, i.e. math.h's), retail std::pow(float,float) is 0x00C40E50.
  Draft's draft-local `extern "C" pow` + header math.h is the same declaration, OK.

## 4. Members
- CBezierCurve (ghidra_out/struct_layouts_egor.tsv, 48 bytes): Point1 +0x00, Point2 +0x0C, Point3 +0x18, Point4 +0x24, all C3DVector.
  Shared header rebuild/include/engine/CBezierCurve.h names exactly these; it has NO opt-in macros and NO GetPointOnCurve declaration
  (author must add the inline member there, e.g. under a new `FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE`, or declare it and define
  out of line in the row).
- C3DVector X/Y/Z +0/4/8: rebuild/include/engine/C3DVector.h. Relevant opt-ins: FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
  (operator* groups (X*scale): "adds one no-code scheduler tuple" -- a known x87-scheduling lever, try it), FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
  (operator+ parenthesised (X+o.X),(Y+o.Y)), FABLE_RETAIL_C3DVECTOR (unary minus/cross), copy-ctor spellings under
  FABLE_RETAIL_CTCTARGETINGAI_ATTACK / _CTCTARGETINGPLAYER_UPDATE_LIST (member-init vs body assignment).
- Draft models LOCALLY: struct C3DVector (header exists: engine/C3DVector.h) and struct CBezierCurve with members P0..P3
  (header exists: engine/CBezierCurve.h, real names Point1..Point4). Both must go.

## 5. Callee visibility
- Only callee is the _CIpow import (intrinsic, never inlined). The C3DVector operators/ctors are __forceinline in the header (no out-of-line copies
  in either PDB or Fable.exe). GetPointOnCurve itself is a header-inline emitted out of line (16-aligned COMDAT at 0x009122A0 before the
  FlyingThingBase TU); no STLport helpers involved.

## 6. Siblings (same TU / neighbourhood), all with no `// Flags:` and no #pragma (default lane flags)
- rebuild/src/compiled/00/91/CAIStateGroupFlyingThingBase_CAIStateGroupFlyingThingBase_009126c0.cpp
- rebuild/src/compiled/00/91/CAIStateGroupFlyingThingBase_GetRandomFlightPos_00912700.cpp (includes fable_compiler.h, engine/C3DVector.h, CAIStateGroupBase.h, CVectorMap.h)
- rebuild/src/compiled/00/91/CAIStateGroupFlyingThingBase_AddRandomBezierCurveToPath_00912840.cpp (uses engine/CBezierCurve.h + Point3/Point4;
  defines FABLE_RETAIL_CCHARSTRING..CWORLDMAP, FABLE_STLPORT_DEQUE_PUSH_BACK(_AUX_NOINLINE); graded with FABLE_HDR_OVERLAY=agentQ6/hdr)
- rebuild/src/compiled/00/91/STLdeque_pushback_00913e70.cpp, stddeque_popfront_00913990.cpp (deque<CBezierCurve>).
- rebuild/src/compiled/00/c4/std_pow_00c40e50.cpp: pow idiom (`extern "C" double __cdecl pow(double,double); #pragma intrinsic(pow)`; named float result local).

## 7. Old draft model errors
- Local C3DVector / CBezierCurve stand-ins (strict verifier rejects); member names P0..P3 wrong (Point1..Point4); operator* / operator+ return
  non-const and are plain inline (header: const return, __forceinline) -- inlining strength can change x87 scheduling.
- Arithmetic, term order, rightmost-first evaluation and the two _CIpow calls are right.
- Residue: term 2 (Point2*3*t*(1-t)^2). Retail keeps (1-t) on the x87 stack from before Point2's loads and stores (1-t)^2 into the dead t slot,
  reading it via memory operands. Lane lesson elsewhere: such dead-argument-slot spills come from a NAMED float local (e.g.
  `float oneMinusT2 = ...` or a named float in its own block) that the allocator homes in the dead parameter. Untried levers:
  named float for (1-t)*(1-t) / for (1-t) at block scope; float pow overload `pow(1.0f - t, 3.0f)`; FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN;
  using the shared header's forceinline operators (not yet tried at all). Ego_r copy is 74 bytes longer (464) so Ego_r source/flags differ; not usable as a template.

## 0x00BE7BE0

# DOSSIER 0x00BE7BE0 (391 bytes) -- CLandscapeBackgroundPatch::Render(bool, long&, bool&)

## 1. Retail shape (xdis 00be7be0 0x197)
- __thiscall, `ret 0Ch` (3 stack args), NO frame pointer (`push esi ; mov esi,ecx`), no SEH, no /Os idioms -> speed TU, Flags: none.
- `push ebx` only after the WaterOnlyPatch test (+0x11A) -- the water-only path (0xBE7D10) pops only esi.
- arg0 (bool) true -> `RenderWithOcclusion([esp+10],[esp+14])` 0xBE79D0 (refs passed straight through, bl=0);
  false -> `bl = Render(false)` 0xBE6F70 (`push 0`).
- WaterSubPatch(+0xF4) -> edx; null / PolyCount(+0x30, `cmp word,0 ; jbe`) -> return. edx stored to the dead arg0 slot
  [esp+0Ch] TWICE (0xBE7C2D before the jbe, 0xBE7C55 inside the 3/4/5 arm) = the local whose address is the push_back arg.
- `mov esi,4` hoisted: used as `cmp eax,esi` (type==4) and as the inline push_back step `add eax,esi`.
- Type (+0x40) 3/4/5 arm laid out FIRST: ecx = g(+0x1436E54) ; bl ? +0x268 : +0x25C (the +0x25C add is out of line at 0xBE7C95
  jumping back) ; INLINE push_back (`cmp [ecx+4],[ecx+8]`; null test; store; `add`) else `_M_insert_overflow(finish,&x,&tag,1,1)`
  0xBE8F10 (tag = [esp+14] 1-byte temp, x = [esp+18] i.e. the arg0 slot).
- type==8: bl ? +0x238 : +0x22C, OUT-OF-LINE push_back 0xBE8FF0(&local). Else: bl ? +0x214 : +0x208, out-of-line 0xBE8FF0.
  Each arm has its own `pop ebx ; pop esi ; ret 0Ch` tail; the null/PolyCount exit shares 0xBE7CBC.
- Water-only (0xBE7D10): esi = WaterSubPatch, null/PolyCount test, store esi to [esp+8] (arg0 slot), INLINE push_back on +0x25C
  (`add dword ptr [ecx+4],4` -- no esi constant here), overflow 0xBE8F10.

## 2. Identity (manifest label `_global::Render` is wrong module)
- FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv, _fablewin_api.tsv) + Ego_r.pdb (egor_pdb_names.tsv 0x009C9D60):
  `?Render@CLandscapeBackgroundPatch@@QAEX_NAAJAA_N@Z` = public `void CLandscapeBackgroundPatch::Render(bool, long&, bool&)`.
- Callers (xref): CLandscapeBackgroundTreeNode::Render 0x00BF71D0 (call 0xBF72CD, landed) and CEngineLandscapeRenderer::RenderSortedPatches
  0x00B68DA0 (call 0xB68F59; Ego_r ?RenderSortedPatches@CEngineLandscapeRenderer@@AAEXXZ).
- NAME_FIX: landed caller 0x00BF71D0 declares `void Render(bool force, long* count, bool* changed)` (pointers, "manifest parameter
  types") -- PDB says `long&, bool&` (AAJ / AA_N). Param names unknown; the old draft's "withOcclusion, a, b" by value is wrong
  (they are references forwarded to RenderWithOcclusion(long&, bool&)).
- Not a vtable slot (no rtti-vtables hit).
- FableWin has `?HasWater@CLandscapeBackgroundPatch@@QAE_NXZ` (28 bytes) -- likely `WaterSubPatch && WaterSubPatch->PolyCount > 0`;
  plausible inline used here (descriptive inference, not in Ego_r).

## 3. Callees
| addr | landed file | landed name | PDB name |
|---|---|---|---|
| 0x00BE79D0 | 00/be/CLandscapeBackgroundPatch_RenderWithOcclusion_00be79d0.cpp | RenderWithOcclusion(long& queryHandle, bool& occluded) | ?RenderWithOcclusion@CLandscapeBackgroundPatch@@AAEXAAJAA_N@Z (private) -- draft spells (long a, long b): WRONG |
| 0x00BE6F70 | 00/be/CLandscapeBackgroundPatch_Render_00be6f70.cpp | bool Render(bool) | ?Render@CLandscapeBackgroundPatch@@AAE_N_N@Z (private) -- draft spells `bool RenderPatch(long flags)`: WRONG |
| 0x00BE8FF0 | 00/be/global_PushBackdword00be8ff0_00be8ff0.cpp | CRetailVector_dword::PushBack_dword_00be8ff0 (generic) | best evidence: ?push_back@?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@QAEXABQAVCEngineWaterBackgroundSubPatch@@@Z (FableWin 0x02CE8750; the only push_back for this element type in either PDB; lists are CArray<...> in Ego_r). Could equally be std::vector<..>::push_back -- bytes identical. NAME_FIX candidate. |
| 0x00BE8F10 | 00/be/stdvector_Minsertoverflow_00be8f10.cpp | std::vector<CUnprovenElement_00be8f10*>::_M_insert_overflow | element is CEngineWaterBackgroundSubPatch* (this row proves it) -- NAME_FIX: respell the element type |
- Global 0x01436E54 = `extern CEngineWaterRenderer* g_pEngineWaterRenderer;` (3 landed rows incl. 0x00BF44A0; one row calls it g_pVideoOptionsListener -- outlier).

## 4. Members
- CLandscapeBackgroundPatch (engine/CLandscapeBackgroundPatch.h; opt-in FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH gives the retail
  class : CLandscapeBackgroundSubNodeBase, sizeof 0x11C; declares only GetEdgeStrip -- Render/RenderWithOcclusion/Render(bool) must be
  added (header opt-in or derived shell as the old draft did)): WaterSubPatch +0xF4 (CEngineWaterBackgroundSubPatch*), WaterOnlyPatch +0x11A (bool).
- CEngineWaterBackgroundSubPatch (engine/CEngineWaterBackgroundSubPatch.h, generated, no opt-ins): PolyCount +0x30 ushort;
  WaterType +0x40 is `unsigned char WaterType[4]` in the header (PDB type EWaterType; enumerators unproven -- landed rows use
  `enum EWaterType { WATER_TYPE_NONE };`). Header needs a cast or local enum use.
- CEngineWaterRenderer (engine/CEngineWaterRenderer.h, opt-in FABLE_RETAIL_CENGINEWATERRENDERER [pulls CEngineComponent etc.];
  members are 12-byte STLport CArray<CEngineWaterBackgroundSubPatch*>):
  +0x208 BackgroundRenderList, +0x214 AlphaedBackgroundRenderList, +0x22C IceBackgroundRenderList, +0x238 IceAlphaedBackgroundRenderList,
  +0x25C SeaMidgroundRenderList, +0x268 SeaAlphaedMidgroundRenderList. Mapping: Render(false) result true -> "Alphaed" list;
  type 3/4/5 = sea, 8 = ice, other = plain; water-only -> SeaMidgroundRenderList. (Also FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND for methods.)
- Old draft models LOCALLY: CEngineWaterBackgroundSubPatch, CEngineWaterRenderer, the vector (CRetailVector) -- all have shared headers
  (CArray.h + stlport/fable_containers.h).

## 5. Visibility
- CArray<T>::push_back: engine/CArray.h FABLE_RETAIL_CARRAY_PUSHBACK (Lionhead wrapper -> std::vector::push_back body) or
  FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE. Retail inlined it at the FIRST two textual sites (sea arm, water-only?) and called 0xBE8FF0
  at the others -- VC7.1 inline-budget split, same as landed 0x00BF44A0 (which used a forceinline/noinline pair locally).
- _M_insert_overflow 0xBE8F10 out of line: FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY (used by sibling 0x00BE4F90), or the body
  visible (FABLE_RETAIL_STLP_VECTOR). STLport push_back opt-ins: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE / _OUTOFLINE.
  Tag type: retail never zero-stores the 1-byte tag ([esp+14]) -> __true_type with user ctor (landed 0xBE8FF0 SScalarTag; check
  FABLE_RETAIL_STLP_FALSE_TAG_POD semantics for the __true_type path).
- Same TU, earlier addresses: Render(bool) 0xBE6F70, RenderWithOcclusion 0xBE79D0 (both landed, out of line here -- calls are real).

## 6. Siblings
- 00/bf/CEngineLandscapePatch_RenderForegroundWater_00bf44a0.cpp -- same water-renderer lists, inline-then-out-of-line push_back split,
  named water local whose address is the arg; local CRetailVector with FABLE_FORCEINLINE push_back_inline + noinline push_back.
- 00/be/CArrayCTVertexWaterBackground_pushback_00be4f90.cpp -- defines FABLE_RETAIL_STLP_FALSE_TAG_POD, _VECTOR_OVERFLOW_DECL_ONLY,
  _STLP_VECTOR, _CARRAY_PUSHBACK; shared CArray.h.
- 00/be/CLandscapeBackgroundPatch_RenderWithOcclusion_00be79d0.cpp (local patch model; fable_compiler.h), 00/be/CLandscapeBackgroundPatch_GetEdgeStrip_00be5c60.cpp
  (uses FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH), 00/b6/CEngineWaterRenderer_WorldFrameUpdate_00b6d6d0.cpp (`// Flags: none`, FABLE_RETAIL_CENGINEWATERRENDERER).

## 7. Old draft model errors
- Wrong signature: (bool, long, long) by value + RenderWithOcclusion(long,long) + RenderPatch(long) -> must be (bool, long&, bool&),
  RenderWithOcclusion(long&, bool&), Render(bool) called with `false`.
- Local stand-ins for three classes with shared headers; fake GrowWaterList body for _M_insert_overflow (should be decl-only / STLport).
- Residue: water pointer in esi vs retail edx with esi=4 constant, and 3/4/5 arm layout. Retail stores the pointer to the arg slot
  twice -> two copies of the local (e.g. `if (HasWater())`-style test reading WaterSubPatch, then a separate `water` local / switch
  per arm), suggesting the test and the queued pointer are separate expressions. `this` is dead after the WaterSubPatch load in retail
  (esi reused) -- so nothing after may read members. Try: switch on WaterType with case 3/4/5 first; CArray::push_back (Lionhead
  wrapper) rather than a direct vector::push_back so the inline budget falls as in retail; the lists named per the header.

## 0x0081F240

# DOSSIER 0x0081F240 CMap::CopyPos (392 bytes, ret 0Ch)

## 1. Retail shape (python3 $S/w17/xdis.py 0081f240 0x198)
- __thiscall, `ret 0Ch` (3 ptr args), NO frame pointer (no `push ebp; mov ebp,esp`; ebp used as a GP reg = `source`), no SEH.
- Prologue `push ecx` = one 4-byte local: `this` is SPILLED to [esp+0xc] at 0x81F270 and reloaded from it 3x in the
  EngineBlend[0] copy (0x81F313, 0x81F329, 0x81F34C) and once in EngineBlend[1] (0x81F375). Push order ecx,ebx,ebp,esi,edi
  (edi pushed late at 0x81F280).
- Body: (a) 12-byte cell copy, source cell address computed FIRST (edx, 0x81F247-0x81F26D) then dest (eax), copied as 3 dwords;
  (b) `xor ebx,ebx; nop` + loop ebx=0..2 (`cmp ebx,3; jl`): each iteration recomputes source cell from source.HeightMap
  ([ebp+1A7Ch]), movzx EngineThemePaletteIndex[ebx] (`[edx+ebx+4]`), source.ThemePaletteEntryList start [ebp+1A6Ch],
  `[ecx+eax*8]` = ThemeDefIndex (no zero test), pushes (ebx, themeDef, to.Y, to.X), `mov ecx,[esp+20h]` (spilled this),
  call 0x0081EF30; (c) two blend byte copies, NOT a loop: byte +7 then byte +8 (constant offsets); no further calls.
- (c) detail: EngineBlend[0]: source.HeightMap loaded once (ecx), this->HeightMap loaded THREE times (once for SizeY, once for
  SizeX, once for Cells), source cells base spilled to [esp+18h] (the dead `source` arg slot). EngineBlend[1]: each HeightMap
  loaded once (ebp <- source.HeightMap reused for SizeY/SizeX/Cells; ecx <- this->HeightMap).
- No optimize("s") idioms; normal /O2 code. Cell index everywhere = Cells[(SizeY - y) * (SizeX + 1) + x] (`sub; inc; imul; add; lea*3; lea*4`).

## 2. Identity (manifest label CMap::CopyPos is CORRECT)
- Ego_r.pdb ?CopyPos@CMap@@IAEXABV1@ABVC2DCoordI@@1@Z = `protected: void __thiscall CMap::CopyPos(const CMap&, const C2DCoordI&, const C2DCoordI&)`
  (ghidra_out/egor_pdb_names.tsv @0x00679E50; FableWin.pdb same leaf in _fablewin_api.tsv / FableWin.exe.signatures.tsv @0x017D41E6/0x02238310).
  NOTE 0x0181F240 in the FableWin tables is CTCSmokeGenerator::GetInterfaceType - a different binary's address, ignore.
- Source 2: Ego_r donor order vs retail order (same object file):
  Ego_r 679A70 SetEngineThemeAt / 679B50 SetEngineThemeAtIncludingExtraCells / 679BD0 SetEngineBlendAt / 679C60 SetEngineBlendAtIncludingExtraCells /
  679CA0 GetEngineThemeAt / 679D40 GetEngineThemeAtIncludingExtraCells / 679D80 GetEngineBlendAt / 679E10 GetEngineBlendAtIncludingExtraCells /
  679E50 CopyPos (0x190 bytes) / 679FE0 IsPassableAt.
  Retail 81EE60 SetEngineThemeAt / 81EF30 SetEngineThemeAtIncludingExtraCells / 81EFC0 SetEngineBlendAt / 81F090 GetEngineThemeAt /
  81F170 GetEngineBlendAt / 81F240 CopyPos / 81F3D0 IsPassableAt. Retail simply drops the three small *IncludingExtraCells helpers
  (inlined everywhere). Ego_r CopyPos 400 bytes ~ retail 392.
- Source 3: callers xref.py: 0x0063E127, 0x0063E22B, 0x0063E298 all in landed CWorldMap::UpdateAllEdges
  (rebuild/src/compiled/00/63/CWorldMap_UpdateAllEdges_0063dfd0.cpp, which cites CopyPos 0x0081F240).
  FLAG: that caller declares `void CopyPos(CMap& src, ...)` (non-const) - PDB says `const CMap&` (ABV1). NAME_FIXES candidate.
- Signature: void CMap::CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to); protected (IAE) - callers in
  CWorldMap need friendship (the landed caller uses a local model).

## 3. Callees
- call 0x0081EF30 only: landed rebuild/src/compiled/00/81/CMap_SetEngineThemeAtIncludingExtraCells_0081ef30.cpp,
  name CMap::SetEngineThemeAtIncludingExtraCells(long,long,unsigned long,long) - PDB `?SetEngineThemeAtIncludingExtraCells@CMap@@IAEXJJKK@Z`
  = (long x, long y, unsigned long, unsigned long layer): the LAYER IS unsigned long (K), landed row and old draft both spell it `long`.
  Protected. Not in engine/CMap.h (declare it; no opt-in macro exists for it).
- Inline (no out-of-line copy in retail, names from FableWin.pdb, all CMap protected const/non-const):
  ?GetEngineThemeAtIncludingExtraCells@CMap@@IBEKJJK@Z  unsigned long (long x, long y, unsigned long layer) const
  ?GetEngineBlendAtIncludingExtraCells@CMap@@IBEEJJK@Z  unsigned char (long x, long y, unsigned long index) const
  ?SetEngineBlendAtIncludingExtraCells@CMap@@IAEXJJEK@Z void (long x, long y, unsigned char blend, unsigned long index)
  CHeightMapCell inlines: ?GetEngineThemePaletteIndex@CHeightMapCell@@QBEEJ@Z, ?GetEngineBlend@CHeightMapCell@@QBEEJ@Z,
  ?SetEngineBlend@CHeightMapCell@@QAEXEJ@Z, ?SetEngineThemePaletteIndex@CHeightMapCell@@QAEXEJ@Z;
  ?GetCell@CHeightMap@@QAEAAVCHeightMapCell@@ABVC2DCoordI@@@Z (+ ?PeekCell@CHeightMap@@ABEABVCHeightMapCell@@ABVC2DCoordI@@@Z private const).
  None of these CMap/CHeightMapCell accessors exist in any header yet (grep rebuild/include: no hit) - the author must add them
  under a new nested opt-in. Old draft's `GetCellIncludingExtraCells(long,long)` is a MADE-UP name: the PDB name with that
  formula is GetCell(const C2DCoordI&) (engine/CHeightMap.h FABLE_RETAIL_CHEIGHTMAP_GETCELL); GetCellIncludingExtraCellsAt is a
  DIFFERENT formula (SizeXIncludingExtraCells*y+x) - do not use it.
- Retail evidence the Get/Set*IncludingExtraCells(x,y,...) helpers have the same `(SizeY - y)*(SizeX+1)+x` cell + `+7+index`
  byte: GetEngineBlendAt 0x0081F170 default arm and SetEngineBlendAt 0x0081EFC0 default arm (`mov al,[edx+eax+7]`).

## 4. Members (retail offsets; Ego_r PDB offsets are +0xC for CMap, +4 for CHeightMap after Cells)
- CMap::ThemePaletteEntryList +0x1A6C std::vector<CThemePaletteEntry> (PDB 6776=0x1A78), CMap::HeightMap +0x1A7C
  boost::scoped_ptr<CHeightMap> (PDB 0x1A8C): engine/CMap.h, opt-in FABLE_RETAIL_CMAP_THEMEPALETTE (needs stlport/fable_containers.h,
  boost/scoped_ptr.h -> define FABLE_RETAIL_SCOPED_PTR). CThemePaletteEntry::ThemeDefIndex +0 (8-byte entry) in same opt-in.
- CHeightMap::Cells +0x10 (std::vector<CHeightMapCell>), SizeX +0x204, SizeY +0x208: engine/CHeightMap.h (private members!);
  FABLE_RETAIL_CHEIGHTMAP_GETCELL gives public GetCell(const C2DCoordI&) AND `friend class CMap` (so CMap may read SizeX/SizeY/Cells).
  CHeightMap.h requires FABLE_RETAIL_CHEIGHTMAPCELL and FABLE_RETAIL_C2DBOXI defined before include.
- CHeightMapCell (12 bytes) EngineThemePaletteIndex[3] +4, EngineBlend[2] +7: engine/CHeightMapCell.h (FABLE_RETAIL_CHEIGHTMAPCELL
  adds the flag bitfields + inline Clear). Whole-cell copy = implicit operator=.
- C2DCoordI X/Y: engine/C2DCoordI.h.
- Locally modelled in old draft though shared headers exist: C2DCoordI, CHeightMapCell, CThemePaletteEntry (in CMap.h),
  CHeightMap, CMap, scoped_ptr (boost/scoped_ptr.h), CRetailVector (stlport vector).

## 5. Callee-body visibility
- SetEngineThemeAtIncludingExtraCells 0x0081EF30 is in the same TU, defined earlier, out of line in retail (Ego_r: also out of line).
  It is NOT inlined into CopyPos (real call) - if the author defines its body in the TU, keep it out of line (#pragma auto_inline(off)
  around it, or declaration only).
- Get/SetEngineBlendAtIncludingExtraCells, GetEngineThemeAtIncludingExtraCells: inline class members (no retail copy) -> must be
  visible and inlined. Likely the reason this->HeightMap is reloaded per access in the blend copy: SetEngineBlendAtIncludingExtraCells
  inlined on `this` with HeightMap->... expressions, source side via GetEngineBlendAtIncludingExtraCells on `source`.
- STLport: vector operator[] / scoped_ptr operator-> inline only; nothing called out of line.

## 6. Siblings
- rebuild/src/compiled/00/8f/CMap_AddThemeDefIndexToPalette_008f70f0.cpp: the ONLY CMap row on shared headers. Uses
  #define FABLE_RETAIL_CMAP_THEMEPALETTE, FABLE_RETAIL_CMAP_THEMEPALETTE_GETINDEX_NONCONST, FABLE_RETAIL_CHEIGHTMAPCELL,
  FABLE_RETAIL_C2DBOXI, FABLE_RETAIL_CHEIGHTMAP_GETCELL, FABLE_RETAIL_SCOPED_PTR; #include "engine/CMap.h", "engine/CHeightMap.h";
  `#pragma auto_inline(off)/(on)` around an out-of-line same-TU body; uses `HeightMap->GetCell(C2DCoordI(x, y)).EngineThemePaletteIndex[i]`
  and `ThemePaletteEntryList[...]`. Copy this idiom.
- 0x0081EF30 SetEngineThemeAtIncludingExtraCells (local model, no flags), 0x0081EFC0 SetEngineBlendAt / 0x0081F170 GetEngineBlendAt
  / 0x0081F090 GetEngineThemeAt (local raw-offset models with misnamed `quarterGrid`/`gameGrid` at 0x1A78/0x1A7C - those are
  really GameMap / HeightMap), 0x0081F3D0 IsPassableAt (quarantine header). No `// Flags:` lines, no optimize pragmas in this
  neighbourhood (only CMap_GetScriptedMapBrush_0081e9c0 has `// Flags: /EHsc`).

## 7. Old-draft model errors
- Made-up accessor GetCellIncludingExtraCells on a local CHeightMap; all types modelled locally despite shared headers.
- The 3 theme-layer copies and the blend copies are spelled with raw expressions instead of the PDB inline helpers
  `SetEngineThemeAtIncludingExtraCells(to.X, to.Y, source.GetEngineThemeAtIncludingExtraCells(from.X, from.Y, i), i)` and
  `SetEngineBlendAtIncludingExtraCells(to.X, to.Y, source.GetEngineBlendAtIncludingExtraCells(from.X, from.Y, i), i)`.
  The remaining DIFFER (368v392) is exactly the blend part (retail reloads this->HeightMap thrice + spills source cells base):
  the inline-helper spelling (value passed through an unsigned char parameter, index unsigned long) is the untried form.
- Blend part: try both two explicit calls (index 0, 1) and `for (unsigned long i = 0; i < 2; ++i)` (VC7.1 may fully unroll a
  2-trip loop; retail has constant +7/+8 offsets and no loop). Theme loop must stay a real loop (retail `cmp ebx,3; jl`).
- Layer/index types: PDB uses unsigned long (K) for layer/index params, draft uses long.
- 6 prior forms (named/unnamed bases, blend helper inline/out of line) are recorded in
  rebuild/candidates/manual/pending/2026-10-21_linux_hand247/NOTES.md:29 / STATUS.tsv:13.

## 0x00646580

# DOSSIER 0x00646580 -- NPlayerGui::CDrawMeleeActionButtonEffects::Draw (393 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 00646580 0x199)
- thiscall, `ret 4` (one arg), frame pointer `push ebp; mov ebp,esp; sub esp,0x94`, `leave` -> `#pragma optimize("y", off)` + `#pragma optimize("s", on)`
  (the NPlayerGui HUD TU mode; same as DrawButton 0x00645099, DrawEffects 0x006451DD, Update 0x0064BE45). No SEH frame (no /EHsc unwinding: sprite
  and CRightHandedSet have no dtors; the CIntelligentPointer temp dtor is called directly, no state).
- Registers: esi=this, ebx=0 held all function, edi=&PeekGuiDef() result. Frame: sprite at [ebp-0x94] (0x54 bytes), CRightHandedSet temp [ebp-0x40],
  C3DVectors [ebp-0x28] (0,0,1) and [ebp-0x1c] (0,1,0), anchored pos [ebp-0x10], CIntelligentPointer temp / anchored pos / uv_end reuse [ebp-8].
- Control flow (exact):
  ```
  if (PlayerGui->ShowSpecialAbilityButtons)            // [esi+4]->+0x250, jne -> 0x6466f2
      FlourishEffect = 0;                              // [esi+0x40] = ebx
  else if ((FlashTimer >= 0 || ComboLoadingIconTimer >= 0) && FlourishEffect > 0) {   // +0x34 jge / +0x38 jl / +0x40 jle
      Drawing = true;                                  // byte [esi+0x18] = 1  (CDrawBase::SetAsDrawing(true) opt-in exists)
      const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();   // edi
      if (FlashTimer >= 0 && Effects[0].GetPItem() == 0) {   // mov ecx,[esi+0x28] = Effects._M_start -> element 0
          pos = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos);   // called BEFORE the C3DVector stores / CRightHandedSet ctor
          CIntelligentPointer<CParticleEmitter> e(GetParticleEmitterManager().Create2DEmitter(FlourishEffect, pos, (EEngine2DLayer)0x44, true,
                                    CRightHandedSet(C3DVector(0,0,1), C3DVector(0,1,0))));   // manager fetched last (after CRightHandedSet ctor)
          Effects[0] = e;                              // operator= 0x0065533C on [esi+0x28]; then ~CBaseIntelligentPointer 0x00A01C10 on temp
      }
      if (Effects[0].GetPItem() != 0) {                // je -> 0x6466f5 (the trailing test)
          CEnginePrimitive2DSprite sprite(0 /*u64 id: push ebx;push ebx*/,
              GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos) + def.AbilityButtonFlourishGraphicOffset,   // C2DVector member operator+ (fld this.X; fadd [eax])
              (EEngine2DLayer)0x43, def.AbilityButtonFlourishGraphic /*+0x790*/, 0 /*frame*/, (blend)2, (interp)0, 0.0f, 0.0f,
              ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0 /*flags*/, C2DVector(0,0) /*uv start [ebp-0x18]*/, C2DVector(0,0) /*uv end [ebp-8]*/);
          info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);   // [ebp+8]->+0x34, vslot +0x5c (23); handle = push [esi+0x1c]
      }
  }
  if (FlourishEffect == 0)                             // 0x6466f5 cmp [esi+0x40],0 (also the jle target 0x6466f8 reuses flags)
      CDrawBase::ClearEffects(Effects);                // lea ecx,[esi+0x28]; call 0x006445BA (static __fastcall)
  ```
  The `0xff` push at 0x00646696 is ScaleAlpha's own argument (ret 4); the three earlier 0xff pushes are blue/green/red; ScaleAlpha's eax result = alpha.
  Note: the GFAnchorToBottomRight-first ordering in the emitter arm means the anchored position is probably a named local (or a prior statement)
  -- in a single right-to-left expression the CRightHandedSet temp would be built first. Sprite arm: the anchored temp lands at [ebp-8] and the
  sum at [ebp-0x10]; uv_end reuses [ebp-8].

## 2. Name / signature
- FableWin.pdb (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv + _fablewin_api.tsv, FableWin 0x0068921B / 0x01842088):
  `?Draw@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`
  -> `void NPlayerGui::CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo& info)` public, non-virtual, non-const.
- Second source: caller CPlayerGui draw loop at 0x00439769: `mov ecx,[esi+0x220]` = CPlayerGui::PDrawMeleeActionButtonEffects (engine/CPlayerGui.h:113 / :388,
  static-asserted +0x220) then `push ebp` (the prepare info) `call 0x646580` -- matches the one-arg non-virtual Draw; arg used as info (+0x34 = Engine).
  RTTI vtable (rebuild/manifest/rtti-vtables.tsv: NPlayerGui::CDrawMeleeActionButtonEffects 0x0125A714, slot0 0x00645829 ClearPrimitivesIfNecessary) -- Draw is not virtual.
- Manifest label CDrawMeleeActionButtonEffects::Draw is RIGHT; the old draft's parameter `CDrawContext* context` is WRONG (it is `const CEnginePrimitivePrepareInfo&`).
- Not in egor_pdb_names.tsv at this address (Ego_r copy is at another address); the FableWin mangled name is the leaf.

## 3. Callees (landed row / landed name / PDB name)
| addr | landed file | landed name | PDB name | draft disagrees? |
|---|---|---|---|---|
| 0x00438570 | 00/43/CPlayerGui_PeekGuiDef_00438570.cpp | PeekGuiDef | `?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ` static __fastcall, returns const CPlayerGuiDef& (CPlayerGui.h:216, opt-in FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF) | YES: draft free fn returning pointer |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | draft returns CThing* (should be CBaseObject*) |
| 0x00643D0C | 00/64/NPlayerGui_GFAnchorToBottomRight_00643d0c.cpp | NPlayerGui::GFAnchorToBottomRight | `?GFAnchorToBottomRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` = `C2DVector __fastcall GFAnchorToBottomRight(const C2DVector&)` (hidden result ecx, pos edx) | YES: draft `void(out*, in*)` |
| 0x00487050 | 00/48/CRightHandedSet_CRightHandedSet_00487050.cpp | CRightHandedSet::CRightHandedSet(const C3DVector& up, const C3DVector& forward) | ??0CRightHandedSet | YES: draft default ctor; real args up=(0,0,1), forward=(0,1,0) |
| 0x00AEAAF0 | 00/ae/NParticleEngineCParticleEmitterManager_Get_00aeaaf0.cpp | (getter) | FableWin: `NParticleEngine::GetParticleEmitterManager()` __fastcall, returns CParticleEmitterManager& (as cited by landed DrawEffects 0x006451DD) | YES: draft `CParticleEmitterManager::Get()` returning pointer |
| 0x00AEB790 | 00/ae/NParticleEngineCParticleEmitterManager_Create2DEmitter_00aeb790.cpp | Create2DEmitter | `?Create2DEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@JABVC2DVector@@W4EEngine2DLayer@@_NABVCRightHandedSet@@@Z` (long handle, const C2DVector&, EEngine2DLayer, bool start, const CRightHandedSet&), ret 14h | YES: draft 7 args, by-value set |
| 0x00655318 | 00/65/CIntelligentPointerNParticleEngineCParticleEmitter_CIntelligentPointer_00655318.cpp | CIntelligentPointer<CParticleEmitter>(CParticleEmitter*) | `??0?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@QAE@PAVCParticleEmitter@NParticleEngine@@@Z` | draft uses CThing |
| 0x0065533C | 00/65/CIntelligentPointerCThingCreatureBase_operator_0065533c.cpp | operator= (one of 24 identical bodies) | CIntelligentPointer<T>::operator= | ok |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ~CBaseIntelligentPointer | `??1CBaseIntelligentPointer@@UAE@XZ` (virtual; called directly on a named temp) | ok |
| 0x00643D5D | 00/64/ConsvalstdallocatorstdpairEHeroMorphType..._CEntryconst_00643d5d.cpp | **Cons_val_CEntry (garbage, propagated)** | Ego_r `?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z` (unsigned char ScaleAlpha(unsigned char) const; NPlayerGui_CDrawBase.h opt-in FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES) | YES: draft "AnchorPoint" w/ 5 args -- NAME_FIXES for the landed 0x00643D5D row |
| 0x00493AC2 | 00/49/CEnginePrimitive2DSprite_CEnginePrimitive2DSprite_00493ac2.cpp | CEnginePrimitive2DSprite ctor | `??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z` (16 params, ret 44h) | YES: draft 11 args |
| vslot +0x5c | rebuild/include/engine/CIEngine.h:97 | CIEngine::SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) slot 23 (opt-in FABLE_RETAIL_CIENGINE) | CEngine 0x00B23BC0 ?SetPersistentPrimitive@CEngine@@ | YES: draft DrawPrimitive(void* Sprite,...) ; landed DrawButton calls it "AddPrimitive" |
| 0x006445BA | 00/64/NPlayerGuiCDrawBase_ClearEffects_006445ba.cpp | NPlayerGui::CDrawBase::ClearEffects | `?ClearEffects@CDrawBase@NPlayerGui@@SIXAAV?$vector@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@...@Z` (static __fastcall, vector&) | YES: draft casts &PEmitter to CEffects* |
(FableWin also has a member `?ClearEffects@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXXZ`; retail here calls the static CDrawBase one directly -- the member is likely an inline wrapper `CDrawBase::ClearEffects(Effects)`; either spelling emits the same call.)

## 4. Members (retail offsets; Ego_r struct_layouts_egor.tsv minus 4 for CDrawBase's vector, minus 8 after Effects)
- this+0x04 CDrawBase::PlayerGui (CPlayerGui*) -- NPlayerGui_CDrawBase.h (protected).
- this+0x18 CDrawBase::Drawing (bool) -- header; FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING gives SetAsDrawing(bool).
- this+0x1c CDrawBase::PrimitiveHandles (vector<CEnginePrimitiveHandle>, STLport 12 bytes) -- FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES makes it the real
  8-byte-element vector (+ declares ScaleAlpha). Used as `PrimitiveHandles[0]` (push [esi+0x1c] = _M_start).
- this+0x28 Effects: vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > (PDB +44). Used as Effects[0] (`mov ecx,[esi+0x28]`), and
  passed whole to ClearEffects (`lea ecx,[esi+0x28]`).
- this+0x34 FlashTimer (long, PDB 60); +0x38 ComboLoadingIconTimer (long, PDB 64); +0x3c FlourishCounter (PDB 68, unused here);
  **+0x40 FlourishEffect (long, PDB 72)** -- draft and landed Update 0x0064BE45 call it "EffectCount" (descriptive, WRONG: PDB name FlourishEffect;
  FableWin has SetFlourishEffect(long)); +0x44 PMeleeAbilityDef (CDefPointer<const CMeleeCombatAbilityDef>, PDB 76; Update calls it "PCurrentAbility").
  -> NAME_FIXES candidates for 0x0064BE45.
- No shared header for CDrawMeleeActionButtonEffects (rebuild/include/engine has none); siblings model it locally deriving from a local CDrawBase.
  Shared header NPlayerGui_CDrawBase.h exists (sizeof 0x28 asserted) -- derive from it.
- CPlayerGui+0x250 ShowSpecialAbilityButtons (bool) -- engine/CPlayerGui.h:119 under FABLE_RETAIL_CPLAYERGUI_MEMBERS (draft wrongly reads it as a GuiDef flag "DisableEffects" via a `Def` member).
- CPlayerGuiDef (engine/CPlayerGuiDef.h:524-528, default layout, no macro needed): +0x788 AbilityButtonFlourishEffectPos_X/_Y (split floats!
  C2DVector in PDB), +0x790 AbilityButtonFlourishGraphic (long), +0x794 AbilityButtonFlourishGraphicOffset_X/_Y. Header splits the C2DVectors into
  _X/_Y floats -> the author needs `*(const C2DVector*)&def.AbilityButtonFlourishEffectPos_X` or a local view; check the header for a C2DVector form.
- CEnginePrimitivePrepareInfo+0x34 Engine (CIEngine*) -- engine/CEnginePrimitivePrepareInfo.h, PDB offset 52.
- Draft's LOCAL models with shared headers: C2DVector (engine/C2DVector.h, member operator+ at line 42), C3DVector, CPlayerGuiDef, CRightHandedSet,
  CParticleEmitterManager, CIntelligentPointer/CBaseIntelligentPointer (engine/CIntelligentPointer.h), CEnginePrimitive2DSprite, CIEngine,
  CEnginePrimitivePrepareInfo, CDrawBase (NPlayerGui_CDrawBase.h), CPlayerGui.

## 5. Callee visibility
- All callees are out-of-line calls; none is inlined. ScaleAlpha (0x00643D5D), GFAnchorToBottomRight (0x00643D0C), ClearEffects (0x006445BA) are
  in the same NPlayerGui TU range (0x643xxx-0x6466xx), defined earlier; they still appear as calls (size TU) -- declarations suffice.
- CIntelligentPointer<CParticleEmitter> ctor 0x00655318 / operator= 0x0065533C are out-of-line COMDATs: opt-ins FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE,
  FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE (engine/CIntelligentPointer.h:39/:47).
- Inline: STLport vector operator[] (`*(begin()+n)` -- stlport/fable_containers.h, as ClearEffects/DrawEffects), C2DVector ctor/operator+ (header),
  C3DVector ctor. CEnginePrimitive2DSprite has no dtor call (trivial).
- Create2DEmitter header opt-in: FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D (engine/CParticleEmitterManager.h:23). PeekGuiDef: FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF.

## 6. Sibling landed rows (copy idioms)
- 00/64/NPlayerGuiCDrawAbilityButtons_DrawEffects_006451dd.cpp -- SAME emitter idiom: named `CIntelligentPointer<CParticleEmitter> emitter(GetParticleEmitterManager().Create2DEmitter(idx, pos+off, layer, true, CRightHandedSet(C3DVector(0,0,1), C3DVector(0,1,0)))); Effects[index] = emitter;` with STLport vector; pragmas y-off + s-on; Flags: none.
- 00/64/NPlayerGuiCDrawAbilityButtons_DrawButton_00645099.cpp -- SAME sprite idiom: `CEnginePrimitive2DSprite sprite(0, C2DVector(position), layer, graphic, 0, BLEND_2, INTERP_0, 0.0f, 0.0f, ScaleAlpha(a), 0xff,0xff,0xff, 0, C2DVector(0,0), C2DVector(0,0)); info.Renderer->AddPrimitive(&handles[index], sprite, 0xc0);` -- reads CPlayerGui ShowSpecialAbilityButtons +0x250; RELOCATION_MATCH.
- 00/64/NPlayerGuiCDrawMeleeActionButtonEffects_Update_0064be45.cpp (same class; y-off + s-on; local model; FABLE_FORCEINLINE CDefPointer).
- 00/64/NPlayerGuiCDrawMeleeActionButtonEffects_ClearPrimitivesIfNecessary_00645829.cpp (same class, s-on).
- 00/64/NPlayerGuiCDrawBase_ClearEffects_006445ba.cpp (the callee; `*(begin()+n)` operator[] lesson), 00/64/NPlayerGuiCDrawQuickAccessIcons_DrawIcon_00645292.cpp (ScaleAlpha user).

## 7. Old draft model errors
1. Effects is a vector, not one intelligent pointer at +0x28: retail `mov ecx,[esi+0x28]` (element 0 via _M_start) for GetPItem/operator=, but the draft's member `PEmitter.GetPItem()` emits `lea ecx,[esi+0x28]`. Biggest residue source.
2. Sprite handle: push [esi+0x1c] is &PrimitiveHandles[0], not a `Sprite` member value; vfunc is CIEngine::SetPersistentPrimitive on info.Engine.
3. 0x00643D5D is ScaleAlpha(unsigned char) (1 arg), not AnchorPoint(5 args): the sprite ctor gets all 16 PDB args (u64 id 0, pos, 0x43, graphic, 0, 2, 0, 0.0f, 0.0f, ScaleAlpha(255), 255,255,255, 0, uv0, uv1).
4. Sprite position is GFAnchorToBottomRight(EffectPos) + GraphicOffset (operator+), draft reused the name EmitterOffset fine but the out-pointer GFAnchor form is wrong (returns by value).
5. Create2DEmitter has 5 args (FlourishEffect, pos, 0x44, true, const CRightHandedSet&) and the manager is the free GetParticleEmitterManager() by reference; CRightHandedSet(up (0,0,1), forward (0,1,0)).
6. Flag at PlayerGui+0x250 is CPlayerGui::ShowSpecialAbilityButtons (draft: `Def->DisableEffects`, with Def mis-typed as CPlayerGuiDef*).
7. Param type is const CEnginePrimitivePrepareInfo&; +0x40 is FlourishEffect; ClearEffects is static CDrawBase::ClearEffects(Effects).

## 0x0052C920

# DOSSIER 0x0052C920 NUISystem::CChangingStateComponent::ProcessChangeState (395 bytes)

## 1. Retail shape (xdis 0052c920 0x19b)
- __thiscall, plain `ret` (no args), void. No frame pointer (no push ebp), no SEH. `sub esp,8` = one 8-byte local
  (esp+8..0xf after push esi/edi) shared by the position temp, the zoom temp and the colour delta.
- Prologue: `push esi ; mov esi,ecx ; cmp [esi+148h],[esi+144h] ; je` -> early out with only esi pushed (0x52caa6).
  `push edi` after the test. edi = GetState(TargetState) (vslot 135 +0x21c, called FIRST with [+148]),
  then GetState(CurrentState) (+0x21c with [+144]) result DISCARDED (virtual call kept).
- Arm flag&1 (Position +0x34 vs target +0x08): fld tX-pX ; fld tY-pY ; `fld st0;fmul st1` (Y*Y) ; `fld st2;fmul st3` (X*X) ;
  faddp ; `fld [0x129BA3C] ; fmul [0x129BA3C] ; fcompp ; fnstsw ; test ah,5 ; jp skip` == C2DVector::IsNonZero()
  (`GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO`). Then the difference is RECOMPUTED from memory
  into the frame temp (two separate `target->Position - Position` expressions, not one named local).
  Linear flag: `test al,al ; je ; push 1 ; jmp ; push 0` then one shared call [edx+80h] (ChangePositionDelta, slot 32).
- Arm flag&4: identical with Zoom +0x5c / target +0x10, call [edx+90h] (ChangeZoomDelta, slot 36).
- Arm flag&2 (Colour +0x84 vs target +0x18): bytes R(+2) G(+1) via `mov al,t ; sub al,[c]` (memory operand),
  B(+0) and A(+3) via regs (`mov dl,t.B ; mov al,c.B ; sub dl,al`; `push ebx ; mov bl,t.A ; mov cl,c.A ; sub bl,cl`);
  stores R,G,A,B order; then `mov eax,[local] ; test eax,eax ; je` (dword != 0 test). The ChangeColourDelta call
  ([edx+9Ch], slot 39) is TAIL-DUPLICATED for linear true/false, each with its own epilogue (not cross-jumped like arms 1/2).
- Float pool 0x0129BA3C = `?DELTA_NOTIONAL_ZERO@@3MB` (const float 1e-4f) -- declared in engine/C2DVector.h under
  FABLE_RETAIL_C2DVECTOR. Draft calls it `g_Epsilon`: WRONG NAME.

## 2. Identity (manifest label correct, namespace missing)
- `?ProcessChangeState@CChangingStateComponent@NUISystem@@MAEXXZ` -> `protected: virtual void __thiscall
  NUISystem::CChangingStateComponent::ProcessChangeState(void)`. Sources: egor_pdb_names.tsv 005a7440 (Ego_r),
  _fablewin_api.tsv 0179ca8e/02283f70 (FableWin); rtti-vtables.tsv NUISystem::CSprite 0122f5d4 slot 58 = 0052c920
  (CComponent slot 58 = 0x52e840 empty `ret`, CMorphingSprite slot 58 = 0x5546f0); the only direct caller
  CMorphingSprite::ProcessChangeState 0x5546f0 does `call 0x52c920` first (base call). Shared header
  engine/NUISystem_CChangingStateComponent.h declares it `protected: virtual void ProcessChangeState();`.
- Leaf line: `// Leaf: ?ProcessChangeState@CChangingStateComponent@NUISystem@@MAEXXZ`.

## 3. Callees (all virtual; bodies irrelevant to inlining)
| slot | off | PDB name (engine/NUISystem_CComponent.h / _CChangingStateComponent.h) | retail body | draft spelling |
|---|---|---|---|---|
| 135 | +0x21c | `virtual CUIState* GetState(unsigned long)` (non-const; slot 134 is the const one) | 0x0052CEB0 (landed NUISystemCChangingStateComponent_GetState_0052ceb0.cpp) | GetState OK |
| 32 | +0x80 | `ChangePositionDelta(const C2DVector&, float, bool)` | 0x0052EA30 | OK |
| 36 | +0x90 | `ChangeZoomDelta(const C2DVector&, float, bool)` | 0x0052EC20 (landed CComponent_ChangeZoomDelta_0052ec20.cpp) | OK |
| 39 | +0x9c | `ChangeColourDelta(const CRGBColour&, float, bool)` | 0x005305A0 (landed NUISystemCComponent_ChangeColourDelta_005305a0.cpp) | OK |
- NAME_FIXES candidates (landed callers/siblings): 0x005546F0 row names slot 135 `GetStateInfo` and declares the base as
  global `CChangingStateComponent::ProcessChangeState` (non-virtual, no NUISystem) -- PDB: NUISystem::...GetState /
  protected virtual. 0x0052C780 InternalChanged row names slot 134 `GetStateInfo`, slot 135 `OnStateChanged` -- PDB:
  both `GetState` (const/non-const); header says InternalChanged returns bool (row returns long).

## 4. Members (shared headers exist for every class the draft models locally)
- this: NUISystem::CChangingStateComponent (engine/NUISystem_CChangingStateComponent.h, size 0x15c, static-asserted):
  CurrentState unsigned long +0x144, TargetState unsigned long +0x148, UpdateTime float +0x150 (all protected; OK from a member).
- base NUISystem::CComponent (engine/NUISystem_CComponent.h, 0x134): Position C2DVector +0x34, Zoom C2DVector +0x5c,
  Colour CRGBColour +0x84 (protected).
- NUISystem::CUIState (engine/NUISystem_CUIState.h, 0x24): StateChangeFlag unsigned long +0, Position +8, Zoom +0x10,
  Colour +0x18, UpdateTime +0x1c, LinearChange bool +0x20.
- CRGBColour (engine/CRGBColour.h): union {B,G,R,A bytes; unsigned long IntValue}. Opt-ins: FABLE_RETAIL_CRGBCOLOUR
  (Initialise(r,g,b,a) + copy ctor via Initialise -- reads all 4 source bytes before storing), FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY,
  FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR. NO operator-/-=/GetAsInt in header. PDB has ??8 / ??9 (operator==/!=), GetAsInt
  (?GetAsInt@CRGBColour@@QBEKXZ), Initialise EEEE, ctors EEE/EEEE/copy/default; no ??G/??Z found in name tables.
- C2DVector (engine/C2DVector.h): opt-ins FABLE_RETAIL_C2DVECTOR (pack(1), DELTA_NOTIONAL_ZERO decl), nested
  FABLE_RETAIL_C2DVECTOR_NORMALISE (gives IsNonZero), FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX (Y*Y first),
  FABLE_RETAIL_C2DVECTOR_ISNONZERO_DIRECT / _ISNONZERO_FORCEINLINE. Retail squares the top-of-stack (Y) first,
  same pattern CullWayPoints needed _YX for -- try with and without _YX.
- Draft models LOCALLY: CComponent (all 136 VSlots), CChangingStateComponent, CUIState, CRGBColour, C2DVector -- all have shared headers.

## 5. Visibility
- All four callees are virtual calls through the vtable -> no devirtualisation; their bodies don't matter.
- Inline helpers that retail expanded: C2DVector::operator-, GetSquaredMagnitude/IsNonZero, CRGBColour ctor/copy/
  channel arithmetic (all header-inline). No STLport helpers touched.

## 6. Siblings (same TU neighbourhood 0x52c730..0x52cf40, NUISystem::CChangingStateComponent)
- 0x0052CF40 ChangeState: uses `#include "engine/NUISystem_CChangingStateComponent.h"` + `NUISystem_CUIState.h`,
  no Flags line, no opt-ins -- the model for using shared headers in this class.
- 0x0052CD20 UpdateStateChange, 0x0052C780 InternalChanged, 0x0052C870 IsPositionIndependent: local models, no Flags/pragmas.
- 0x005305A0 CComponent::ChangeColourDelta: colour arithmetic as `CRGBColour colour = Colour; colour += delta;` with a
  copy ctor that calls Set(R,G,B,A) (= PDB Initialise, i.e. FABLE_RETAIL_CRGBCOLOUR) -- matched retail's
  "read all, then op" channel scheduling.
- IsNonZero users: 0x00A1C010 CullWayPoints (defines FABLE_RETAIL_C2DVECTOR, _NORMALISE, _SQUAREDMAGNITUDE_YX), 0x007C1000.
- No /Oy-, no /EH, no optimize("s") evidence: default lane flags.

## 7. Old draft model errors
- Local classes instead of shared headers (strict verifier rejects); `g_Epsilon` should be DELTA_NOTIONAL_ZERO;
  local IsNonZero spelled GetLengthSquared (PDB: GetSquaredMagnitude / IsNonZero).
- Residue (2 bytes, colour arm only): tried operator- via 4-arg ctor, member-wise, init-list, GetR+EEEE ctor, named
  channel locals. NOT tried: the ChangeColourDelta-sibling idiom `CRGBColour delta = target->Colour; delta -= Colour;`
  (or channel-wise `-=`) with FABLE_RETAIL_CRGBCOLOUR copy ctor (Initialise reads R,G,B,A first: retail loads t.B and
  t.A into dl/bl before subtracting) -- strongest next attempt. Test the result with `delta.GetAsInt() != 0` /
  `delta.IntValue` (dword test) or PDB `operator!=` against CRGBColour(0,0,0,0).
- Arms 1/2 linear-flag merge and arm 3 tail duplication already match per STATUS (explicit if/else of two calls).
- Each vector arm must compute `target->X - X` twice (IsNonZero on one temp, a second temp passed by ref).

## 0x00848B70

# DOSSIER 0x00848B70 -- CCreatureAction_PickUpGenericBox::PerformAction (402 bytes)

## 1. Retail shape (xdis 00848b70 0x1a2)
- thiscall, plain `ret` (no args), void. NO frame pointer (`push ecx; push ebx; push ebp` -- ebp is a GP reg = this, later `add ebp,8` = &PCreature). No SEH. 4-byte frame (`push ecx`) used as: [esp+0xc] key/out slot for 0x47 lookup; [esp+0x10] (after push esi) shared by the 0x46 key, CDefPointer local (zeroed `mov [esp+10h],0`), and the 0x31 key.
- Registers: ebx = &PThingToPickUp (this+0xa8), ebp = this -> &PCreature, esi = carrying component (CTCCarrying*), edi = held thing, then edi REUSED for the CDefPointer object (`mov edi,[esp+10h]`) and kept for the release.
- Three epilogues: main (pop edi/esi/ebp/ebx/ecx), carried-arm (FinishAction then pop esi..), dead-arm (FinishAction, push esi never happened). `push esi` is after the alive test, `push edi` after the 0x46 lookup -> VC7.1 shrink-wrapping; FinishAction calls are tail-duplicated into separate arms after the main path.
- Order: IS_THING_ALIVE(PThingToPickUp) inlined (GetPItem twice: null test, then `test byte [eax+91h],1`); third GetPItem -> PeekTC 0x47 (const LowerBound 0x004365B0; bit test `mov cl,[eax+28h]; test cl,cl; jns`); `cmp [carryable+10h],0 jne -> FinishAction`; then PCreature GetTC 0x46 (`test byte [eax+28h],40h`, non-const LowerBound 0x0040F020); RemoveThingFromPrimarySlot(false) (push 0); if held && !held->IsDead() && PCreature->GetType()==1 (`cmp byte [eax+10h],1`, PCreature GetPItem re-called) && held->HasTC(TCI_WEAPON) (`mov eax,[edi+28h]; test ah,1` = bit 72) -> held->Kill(true); CDefPointer<const CCarryableDef> def (=0); PThingToPickUp.Get()->PBaseDef/def(+0x70)->GetSubDef(&def) (0x005CC10E, result ignored); carrying->PutThingInCarrySlot(*PThingToPickUp.Get(), def->ActiveCarrySlot, true) (note GetPItem for the thing is called BEFORE reading def->ActiveCarrySlot: `call GetPItem; mov edi,[esp+10h]; mov ecx,[edi+28h]; push 1; push ecx; push eax`); PCreature GetTC 0x31 (`test dword [eax+24h],20000h`) -> modes->AddMode(0x22); ~CDefPointer = `dec [edi+4]; jne; call [vtbl+4]` (no null test -- folded after the deref).
- All component lookups are unchecked (miss path reads the out var's frame word) = GetTC/PeekTC with result ignored, exactly the landed sibling style.

## 2. Identity (label is RIGHT)
- rtti-vtables.tsv: CCreatureAction_PickUpGenericBox vtable 0x01274BD4 slot 3 = 0x00848B70; CCreatureActionBase.h virtual order: ~dtor(0), Validate(1, CBaseObject), EditUpdate(2), PerformAction(3) = 0, FinishAction(4).
- _fablewin_api.tsv: `?PerformAction@CCreatureAction_PickUpGenericBox@@UAEXXZ`; egor_pdb_names.tsv 0x00523D20 same mangled name (Ego_r).
- Mangled leaf: `?PerformAction@CCreatureAction_PickUpGenericBox@@UAEXXZ` -- public virtual, void(), non-const. (Draft's leaf comment lacked the @@UAEXXZ.)
- Callers: only the vtable (xref: .rdata 0x01274BE0).
- Slot 4 (`call [edx+10h]`) = `?FinishAction@CCreatureActionBase@@UAEXXZ` (Ego_r 0x0051B5C0); draft calls it "GiveUp" -- WRONG name.

## 3. Callees
| retail | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok (but via CIntelligentPointer<T>::Get()) |
| 0x004365B0 | 00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) const | draft: CInterfaceMap::LowerBound(const long*) -- WRONG, use engine/CVectorMap.h |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | same, non-const QAE | WRONG (local stand-in) |
| 0x006A1E00 | 00/6a/CTCCarrying_RemoveThingFromPrimarySlot_006a1e00.cpp | ?RemoveThingFromPrimarySlot@CTCCarrying@@QAEPAVCThing@@_N@Z | ok |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z | ok (param `immediately`) |
| 0x005CC10E | 00/5c/CDefClassBase_GetSubDefCCarryableDef_005cc10e.cpp | ??$GetSubDef@VCCarryableDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCarryableDef@@@@@Z (const) | draft: GetSubDef(CDefPointer<CAbilityDef>*) non-const -- WRONG type (CCarryableDef) and const-ness |
| 0x006A1F80 | (not landed; called by landed 0x00848500, 0x00849F70) | ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z | draft takes CThing* -- WRONG, it is CThing& |
| 0x0073AE10 | 00/73/CTCCreatureModeManager_AddMode_0073ae10.cpp | ?AddMode@CTCCreatureModeManager@@QAE_NK@Z | ok |
| vslot 4 | CCreatureActionBase::FinishAction | ?FinishAction@CCreatureActionBase@@UAEXXZ | draft "GiveUp" WRONG |

## 4. Members
- this+0xa8 `PThingToPickUp` CIntelligentPointer<CThing> (struct_layouts Ego_r +180 -> retail +0xa8): engine/CCreatureAction_PickUpGenericBox.h under FABLE_RETAIL_CCREATUREACTION_GENERICBOX (private member, needs FABLE_RETAIL_CCREATUREACTIONBASE; declares PerformAction virtual).
- this+0x08 `PCreature` CIntelligentPointer<CThingCreatureBase>: CCreatureActionBase.h (FULL_MEMBERS variant or the default protected `PCreature; _pad_0x10[0x98]` line 270).
- CThing +0x10 `Type` (CClass, engine/CClass.h, FABLE_RETAIL_CCLASS, `GetType()`): creature->GetType()==1 (same spelling in landed 0x008C4780, 0x00846660, 0x008C0C00).
- CThing +0x20 UsingComponents bitset, +0x44 Components CVectorMap, +0x70 the def (PBaseDef CDefPointer<const CThingBaseDef> / GetDef), +0x91 bit0 Dead (IsDead()): engine/CThing.h with FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC/PeekTC(ETCInterfaceType, T**), HasTC).
- Interface ids (engine/ETCInterfaceType.h): 0x31 TCI_ENTITY_MODE_MANAGER, 0x46 TCI_CARRYING, 0x47 TCI_CARRYABLE, 0x48 TCI_WEAPON.
- CTCCarryable +0x10 `PCarryingTC` (CTCCarrying*), engine/CTCCarryable.h. Draft's "CTCOwnable::Owner" is WRONG.
- CCarryableDef +0x28 `ActiveCarrySlot` long (Ego_r +40), engine/CCarryableDef.h FABLE_RETAIL_CCARRYABLEDEF. Draft's "CAbilityDef::CarrySlot" WRONG.
- CDefPointer refcount +4 / vslot 1 release: engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE path as in 0x00848500).
- Locally modelled in draft though shared headers exist: CThing, CInterfaceMap(->CVectorMap.h), CBaseIntelligentPointer(->CIntelligentPointer.h), CDefPointer, CDefClassBase(+GetSubDef.h), CAbilityDef(->CCarryableDef), CTCOwnable(->CTCCarryable), CTCCarrying, CTCCreatureModeManager, CCreatureActionBase, CCreatureAction_PickUpGenericBox.

## 5. Callee visibility
- 0x00848AE0 FrameUpdate (same class, earlier in same TU) and 0x00848D10 DropGenericBox::PerformAction follow; GetPlateFromCupboard 0x00848500 / PickUpGoods 0x00848730 / PutGoodsDown 0x00848850 are in the same TU neighbourhood.
- GetSubDef<CCarryableDef> 0x005CC10E: COMDAT visible -> FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + engine/CDefClassBase_GetSubDef.h (used by 0x00848500 and 0x00848730, same TU). Draft's own stand-in reproduced that effect; use the header.
- LowerBound 0x0040F020 / 0x004365B0: engine/CVectorMap.h provides the same-TU noinline body (per 0x00848500 notes).
- IS_THING_ALIVE: inlined here (two GetPItem calls in place) -> do NOT define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (the ctor row 0x007120D0 does, but that TU calls the COMDAT). Either FABLE_RETAIL_IS_THING_ALIVE (engine/ThingAlive.h, template `thing!=0 / thing->IsDead()`; needs CINTELLIGENTPOINTER_COMPARE_NULL_INT + _DEREFERENCE) or a local inline as FrameUpdate 0x00848AE0 has.
- AllocateMode/AddMode, Kill, PutThingInCarrySlot, RemoveThingFromPrimarySlot: other TUs (out of line).

## 6. Siblings to copy
- 00/84/CCreatureActionGetPlateFromCupboard_PerformAction_00848500.cpp (MATCH): CDefPointer<const CCarryableDef>, GetSubDef, PutThingInCarrySlot(CThing&, long, bool), AddMode; defines FABLE_RETAIL_CCHARSTRING, _DEF_CLASS_BASE, _CCARRYABLEDEF, _CDEFINITIONMANAGER, _CRESOURCELIST, _CDEFCLASSBASE_GETSUBDEF_NOINLINE; includes CDefPointer.h, CCarryableDef.h, CDefClassBase_GetSubDef.h, CVectorMap.h. Its GetInterface writes the end-or-greater choice as ONE ternary (same as CThing.h GETTC_EXPANDED).
- 00/84/CCreatureActionPickUpGenericBox_FrameUpdate_00848ae0.cpp: same class, PeekTC(TCI_CARRYABLE,&carryable) on PBox.Get(), local IS_THING_ALIVE, FABLE_RETAIL_CTHING_GETTC_EXPANDED + FABLE_RETAIL_CINTELLIGENTPOINTER, FinishAction slot 4, local CCreatureActionBase model (retail 0xa8).
- 00/84/CCreatureActionDropGenericBox_PerformAction_00848d10.cpp: GetTC(TCI_CARRYING), GetTC(TCI_ENTITY_MODE_MANAGER) on PCreature.Get().
- 00/84/CCreatureActionPickUpGoods_PerformAction_00848730.cpp: same macro set as 0x00848500.
- 00/71/..._007120d0.cpp and 00/96/CAIStateGroupRecoverStealableItem_Update_009658b0.cpp: full retail header stack (FABLE_RETAIL_CCREATUREACTION_GENERICBOX + CCREATUREACTIONBASE + CTHING*...).
- No `// Flags:` lines / no optimize pragmas in these siblings: default /O2-style flags, no /Oy-.

## 7. Draft model errors (vs disassembly)
- Names: GiveUp->FinishAction; CTCOwnable::Owner->CTCCarryable::PCarryingTC; CAbilityDef::CarrySlot->CCarryableDef::ActiveCarrySlot; GetSubDef non-const on CAbilityDef -> const GetSubDef<CCarryableDef>(CDefPointer<const CCarryableDef>*); m_state10 -> CClass::Type via GetType()==1; HasTC(0x48)->HasTC(TCI_WEAPON); PutThingInCarrySlot takes CThing&.
- Draft's GetInterface uses the if-reassignment form; header wisdom (CThing.h GETTC_EXPANDED comment, 0x00848500 notes) says the ONE-ternary form lands the component straight in a register (`mov ecx,[eax+4]`/`mov esi,[eax+4]`) -- likely source of the esi/edi swap + ebx spill + 8-byte frame (DIFFER 416v402).
- Retail reads the def's ActiveCarrySlot AFTER calling PThingToPickUp.GetPItem() for the first argument (evaluation right-to-left would read the slot first; retail evaluates `*PThingToPickUp.Get()` first, then the def) -- try a named `long slot` only if needed; 0x00848500 used `long slot = carryDef->ActiveCarrySlot;` BEFORE the call but there GetPItem was not in the call. Here retail order = GetPItem, then def load: write the call inline with arguments as is (VC7.1 pushes right-to-left but may evaluate the call first).
- Kill condition order: !IsDead, then PCreature GetType()==1, then HasTC(TCI_WEAPON) -- draft order is right.
- CDefPointer constructed (zeroed) after the Kill block, not at function start -- draft right.

