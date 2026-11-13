# DOSSIER 0x004736C4 -- CPlayerGuiDef::Transfer(CPersistContext&)   (6498 bytes, 0x004736C4..0x00475025)

Prior material: $W/prior/ is EMPTY and /opt/fable_lane/w22/agent2091 (the earlier chunk) holds no work (empty src/probe/hdr).
This is a fresh scout. Helper files written here: calltable.tsv (every call: site, member offset, callee, header member),
typed_members.txt (the full retail-typed member list +0x3C..+0xAB4, contiguous, checked), transfer_body.txt (the 463-statement body).

## 1. Retail shape
- `push ebp ; mov ebp,esp ; push esi ; mov esi,ecx ; push edi ; mov edi,[ebp+8]` ... `pop edi ; pop esi ; pop ebp ; ret 4`.
  No `sub esp`, no SEH, no /GS cookie, no `leave`. thiscall virtual, `ret 4`.
- Frame pointer: the only frame use is the dead `context` parameter slot [ebp+8] reused 3x as the zero default for
  TransferMap (`and dword ptr [ebp+8],0 ; lea eax,[ebp+8] ; push eax`). context itself lives in edi the whole time.
  Same idiom as landed CSoundThemeDef::Transfer 0x0046329E (rebuild/src/compiled/00/46/CSoundThemeDef_Transfer_0046329e.cpp:
  `#pragma optimize("y", off)` + `#pragma optimize("s", on)`, "the zero default long lives in the dead context slot").
  => use `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
- Size idioms: every member address through eax (`lea eax,[esi+N] ; push eax ; mov ecx,edi ; call`), last member formed
  in place: `add esi,0AB0h ; push esi ; mov ecx,edi ; call Transfer<long>` (0x00475012). Same as landed siblings
  CAppearanceDef::Transfer 0x00473504, CAbilityTrackDef::Transfer 0x00478AB7, CEnvironmentThemeDaySetDef::Transfer 0x00476DC3.
- Entry unaligned, directly after Transfer<CAnimationSet> 0x0047367A's `leave ; ret 4` (bytes 0x004736C1..C3, no int3 pad).
  The next function 0x00475026 (TransferMap<long,CCharString>) and 0x0047550F (TransferMap<EGameAction,CCharString>),
  0x00475D99 (TransferVector<C2DVector>), 0x0047652B / 0x00476024 (TransferVectorOfSubComponents) follow it: template
  COMDATs first instantiated by this TU (the def TU). They are still CALLED, so declaration-only is right.
- 463 calls, no branches, no other instructions (verified: every non lea/push/mov ecx,edi/call line is the prologue,
  the three map blocks, and the epilogue).

## 2. Name (proven, 3 sources)
- `?Transfer@CPlayerGuiDef@@UAEXAAVCPersistContext@@@Z` -- `virtual void CPlayerGuiDef::Transfer(CPersistContext& context)`.
  (a) vtable 0x012352DC slot 18 -> 0x004736C4 (rebuild/manifest/rtti-vtables.tsv; slot 18 = Transfer for every def, e.g.
  CAppearanceDef 0x012354DC[18] = landed Transfer 0x00473504); (b) Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x004837E7);
  (c) FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x01856F74 / 0x019203F3). Only xref: the vtable (.rdata 0x01235324).
  Manifest label is RIGHT. Return void, param non-const ref.
- Class: RTTI .?AVCPlayerGuiDef@@ : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase
  (ghidra_out/rtti_bases.tsv). Retail sizeof 0xAB4 (landed GetSizeofClass 0x00459E24 returns 2740 = 0xAB4).
  Retail member offsets = Ego_r.pdb - 4 (PDB first member InventoryNoMovementSound +0x40 -> retail +0x3C).

## 3. Callees (all landed; all ecx=context, one pushed address, `ret 4`, except the maps `ret 0Ch`)
| retail | count | what it is (PDB spelling) | landed file / landed name | flag |
|---|---|---|---|---|
| 0x00431102 | 217 | CPersistContext::Transfer<long>(long&)  ??$Transfer@J@... | 00/43/CPersistContext_Transferlong_00431102.cpp | ok |
| 0x004568BC | 127 | CPersistContext::Transfer<C2DVector>(C2DVector&) | 00/45/CEngineLightingManager_UpdateShadowScene_004568bc.cpp | WRONG landed name (UpdateShadowScene); body = mode2 read 8 bytes / mode3 write: C2DVector |
| 0x00431061 | 46 | Transfer<float>(float&) ??$Transfer@M@ | 00/43/CPersistContext_Transfer_00431061.cpp | ok |
| 0x004595A3 | 23 | Transfer<CRGBColour>(CRGBColour&) | 00/45/CPersistContext_TransferCRGBColour_004595a3.cpp | ok |
| 0x00466A47 | 16 | TransferVector<long>(vector<long>&) | 00/46/CPersistContext_TransferBinary00466a47_00466a47.cpp "TransferBinary_00466a47" | WRONG: PDB template is ??$TransferVector@J@CPersistContext@@ (header CPersistContext.h:104/253) |
| 0x00431143 | 7 | Transfer<CCharString>(CCharString&) | 00/43/CPersistContext_TransferCCharString_00431143.cpp | ok |
| 0x00468EB4 | 5 | TransferVector<CRGBColour> | 00/46/..._TransferBinary00468eb4_00468eb4.cpp | WRONG name (TransferBinary_) |
| 0x00464CBF | 5 | TransferVector<float> | 00/46/..._TransferBinary00464cbf_00464cbf.cpp | WRONG name |
| 0x00475D99 | 3 | TransferVector<C2DVector> | 00/47/..._TransferBinary00475d99_00475d99.cpp | WRONG name |
| 0x004331F9 | 2 | TransferVector<CCharString> | 00/43/CPersistContext_TransferVectorBinaryCCharString_004331f9.cpp "TransferVectorBinary" | WRONG name (PDB: TransferVector) |
| 0x00431020 | 2 | Transfer<unsigned long> ??$Transfer@K@ | 00/43/CPersistContext_Transferunsignedlong_00431020.cpp | ok |
| 0x0047652B | 2 | TransferVectorOfSubComponents<CAbilityButtonGuiDef> | 00/47/..._0047652b.cpp | ok (its comment already cites this caller) |
| 0x00476024 | 2 | TransferVectorOfSubComponents<CQuickAccessItemButtonGuiDef> | 00/47/..._00476024.cpp | ok |
| 0x00475026 | 2 | TransferMap<long,CCharString>(const char*, map<CCharString,long>&, const long&) ??$TransferMap@JVCCharString@@@CPersistContext@@QAEXPBDAAV?$map@...@ABJ@Z | 00/47/CPersistContext_TransferMap_00475026.cpp leaf "TransferMap" | leaf is bare |
| 0x0047550F | 1 | TransferMap<EGameAction,CCharString>(const char*, map<CCharString,EGameAction>&, const EGameAction&) ??$TransferMap@W4EGameAction@@VCCharString@@@... | 00/47/CPersistContext_TransferMap_0047550f.cpp | WRONG: its header says <CCharString,long>; called only for "GameActionValues" +0x940 (map<CCharString,EGameAction> in struct_layouts_egor; stdRbtree_erase_00475c5e.cpp proves the tree) |
| 0x00456903 | 1 | Transfer<C3DVector>(C3DVector&) | 00/45/CEngineLightingManager_UpdateShadowScene_00456903.cpp | WRONG landed name; Vec12 = C3DVector (DiffuseVector +0x54) |
NAME_FIXES candidates: 0x004568BC, 0x00456903, 0x00466A47, 0x00468EB4, 0x00464CBF, 0x00475D99, 0x004331F9, 0x0047550F (types above).
Strings pushed: 0x012362CC "ScriptSprites", 0x012362B8 "GameActionValues", 0x012362A8 "MiniMapGraphics" (read from Fable.exe).

## 4. Members
- Shared header rebuild/include/engine/CPlayerGuiDef.h (generated; sizeof 0xAB4, offsets already RETAIL, static-asserted) names every
  member, but models C2DVector/C3DVector as `X_X/X_Y` float pairs, CRGBColour as `unsigned char[4]`, CCharString as
  `CCharStringData* X_PStringData`, vectors/maps as `unsigned char[0xc]`, base as `void* __vftable` + pads; NO virtual Transfer.
- Existing opt-ins (partial typings only): FABLE_RETAIL_CPLAYERGUIDEF_CRIME (+0x434/+0x43C C2DVector), _MINIGAME_OFFSET, _FLOURISH,
  _QUICKACCESSEFFECTOFFSET (+0x6F0), _PROGRESS, _QUESTINFO, _TARGETING, _SCRIPTSPRITES (+0x934 std::map<CCharString,long>),
  _ABILITYBUTTONS (+0x7FC vector<CAbilityButtonGuiDef> -- defines its OWN local CAbilityButtonGuiDef: clashes with
  engine/CAbilityButtonGuiDef.h; do not combine).
- Needed: a new overlay opt-in (e.g. FABLE_RETAIL_CPLAYERGUIDEF_TRANSFER, put in $W/hdr as a full copy of the current header plus
  the #ifdef block) modelled like engine/CQuickAccessItemButtonGuiDef.h: `class CPlayerGuiDef : public CParentDefClassBase {
  virtual void Transfer(CPersistContext&); virtual void Copy(const CDefClassBase*); virtual unsigned long GetSizeofClass(); <typed members> };`
  with FABLE_RETAIL_DEF_CLASS_BASE. CParentDefClassBase is 0x39 (pack 1), first member long at +0x3C. $W/typed_members.txt is the
  complete typed list (types: long, float, unsigned long, CRGBColour, C2DVector, C3DVector, CCharString, std::vector<long|float|
  C2DVector|CRGBColour|CCharString|CQuickAccessItemButtonGuiDef|CAbilityButtonGuiDef>, std::map<CCharString,long|EGameAction>), contiguous to 0xAB4.
  Headers: engine/C2DVector.h, C3DVector.h, CRGBColour.h, EGameAction.h, CAbilityButtonGuiDef.h, CQuickAccessItemButtonGuiDef.h
  (both need FABLE_RETAIL_DEF_CLASS_BASE), engine/CParentDefClassBase.h, fable_string.h, stlport/fable_containers.h, stlport/fable_map.h.
- engine/CPersistContext.h: `template<class T> void Transfer(T&)` (unconditional, line 91), `TransferVector<T>(vector&)` (104),
  `TransferVectorOfSubComponents<T>(vector&)` (106), and the 3-arg `template<class V,class K> TransferMap(const char*, map<K,V,...>&, const V&)`
  only under FABLE_RETAIL_CPERSISTCONTEXT + FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (line 93-100).
  Template-deduction trap: V is deduced from both the map and the default -> pass `long(0)`/`0L` (not `0`) for the long maps and
  `EGameAction(0)` (or an enumerator equal to 0) for GameActionValues, or give explicit <V,K>.
- Out-of-order members in retail (not declaration order): +0x994 MiniMapImpassableWaterColour BEFORE +0x98C MiniMapWaterColour and
  +0x990 MiniMapMaplessColour; then MaxMinimapThemeColours +0x99C, MinimapThemeColours +0x9AC, MaxMinimapZoneColours +0x998,
  MinimapZoneColours +0x9A0. Everything else is in declaration order; every member +0x3C..+0xAB0 is transferred exactly once.

## 5. Callee visibility
- Keep EVERY callee declaration-only (no FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY, no _TRANSFERVECTOROFSUBCOMPONENTS body):
  retail calls all 463 out of line. Template members defined outside the class are not inline-eligible under /Os(/Ob1) anyway, but
  declaration-only removes the risk. The STLport containers are only members (no ctor/dtor/operator[] expansions in this body).
- The only codegen-sensitive spots: the three map temporaries in the dead [ebp+8] slot (needs y-off frame; each temp re-zeroed),
  and the final `add esi,0AB0h` (comes for free).

## 6. Siblings to copy
- CSoundThemeDef::Transfer 0x0046329E (00/46/CSoundThemeDef_Transfer_0046329e.cpp): y-off + s-on, `context.TransferMap("AtmosGroupMap", AtmosGroupMap, 0)`, no Flags.
- CAppearanceDef::Transfer 0x00473504 (00/47/CAppearanceDef_Transfer_00473504.cpp, retail neighbour just before): s-on only (no locals, no frame).
- CCombatAbilityBlockDefBase::Transfer 0x00465C54: shared-header form (FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CPERSISTCONTEXT, class
  opt-in FABLE_RETAIL_CCOMBATABILITYBLOCKDEFBASE_TRANSFER in its engine header).
- CEnvironmentThemeDef::Transfer 0x004732E0, CGlobalDef::Transfer 0x00472506, CHeroStatsDef::Transfer 0x00479CBA: s-on, one transfer per member.
- TransferVectorOfSubComponents rows 0x0047652B / 0x00476024: macro set FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _DEF_CLASS_BASE,
  _CPERSISTCONTEXT, ... and include order fable_string.h, stlport/fable_containers.h.
