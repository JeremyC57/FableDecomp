# Hand chunk 570 (NVectorMath .. CTriangleStripifier2): 4 of 5 byte-exact

(3 compiler-emitted RELOCATION_MATCH in src/, 1 RELOCATION_MATCH graded `retail_asm` in retail_asm/, 1 parked.)

## What matched (4)

- **0x00A5BEF0 NVectorMath::GFInterpolateVectors** (per-vector weights; retail_asm, `// Flags: /Zi`) -- out[i] =
  lerp(from[i], to[i], weights[i]) over four independent strides; SSE path when `g_SSEAvailable` (0x013D2880), x87
  fallback otherwise. Shape: the SSE loop is the original hand `__asm` block (an xmmintrin spelling gives a different
  frame and schedule); a `__declspec(align(16)) C4DVector one(1,1,1,1)` local is what produces the ebx/ebp realigned
  frame, and **/Zi** adds the return-address copy (`mov ebp,[ebx+4] ; mov [esp+4],ebp`) the retail frame has (chunk 511
  called that frame a lane gap; it is not). The x87 fallback needs local copies of the four stream pointers and a
  `__forceinline` C4DVector::Interpolate (`(to - *this) * t + *this`); VC7.1 unrolls it by four as retail does.
  `purity.classify` = retail_asm; the `// Retail-asm:` line cites 0x00A5BF30..0x00A5BF74.
  - Name: Ego_r.pdb 0x00BD6470 `?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z`; the three
    overloads 0x00A5B870 / 0x00A5BBA0 / 0x00A5BEF0 map in donor order onto BD5D90 / BD60F0 / BD6470 and the argument
    lists (scalar weight, two streams + scalar weight, two streams + weight stream) match the stack slots each reads.
- **0x00E295E0 NScript::CQS_MeetSisterScript::CMeetSisterSister::Main** -- take Theresa (Me) and then the hero under
  scripted control (priority 4, frame-waits), cast them as "Sister"/"Hero" in a std::map, start a movie sequence, pause
  the non-scripted entities around RunCutsceneMacro("CS_ARENA_LEAVE_THERESA_02"), then remove Me, complete and
  deactivate the active quest and fade in. Shape: the landed CQHeroSoulsArenaScript::PlayCutscene 0x00D7FC70 model (real
  _Rb_tree map, out-of-line CCountedPointer<MovieBase> dtor 0x006E7AB0); `__forceinline` operator= on both
  CScriptGameResourceObjectScriptedThingBase and CCountedPointer (retail reads other.Info then other.Data before the
  self-compare) for `cast["Hero"] = hero`. CGSI slots 7, 8, 70, 108, 281, 288, 370, 374, 379, 655.
  - Name: slot 1 of the entity vtable the landed CMeetSisterSister::Alloc 0x00E29040 stores; Ego_r.pdb 0x00E759D0
    `?Main@CMeetSisterSister@CQS_MeetSisterScript@NScript@@EAEXXZ`.
- **0x00EC3D40 NScript::CV_RockTrollFirstEncounterScript::Main** -- bind RTFE_Sparrow / RTFE_RockTroll /
  M_RTFERockTrollTrigger, wait for Witchwood1, spawn WatchForRegionExit, activate "V_RockTrollFirstEncounter_Activate"
  once, wait for MissionOver, drop the health-bar quest info, wait for a region-loaded message, re-enable the Witchwood1
  creature generators and deactivate both quests. Shape: landed SwordInTheStone Main model (CEntityScriptBinding,
  CSpawnedFunc, SpawnFunction); CScriptBase must be polymorphic (`virtual ~CScriptBase(); virtual void Init(); virtual
  void Main() = 0; unsigned char _body[0x3c];`) or `this` shifts by 4; the CGSI class must keep its padding virtuals
  (generated, tools/cgsi.py). Slots 7, 12, 22, 64, 276, 281, 321, 338, 558, 655.
  - Name: Ego_r.pdb 0x00F341C0 `?Main@CV_RockTrollFirstEncounterScript@NScript@@UAEXXZ`; the three binding factories it
    stores are CM_RTFERockTrollTrigger::Alloc 0x00EC41F0, CRTFE_RockTroll::Alloc 0x00EC4490, CRTFE_Sparrow::Alloc
    0x00EC45C0, the spawned thread WatchForRegionExit 0x00EC4130 (all by string + vtable).
- **0x00EF87D0 NScript::CPersonalScript_EmptyScript::CUndeadFellow::Main** -- SetPredicate(CIsThingAlive(Me) &&
  CIsThingAlive(Marker4) && CIsThingAlive(Marker5)), one frame, spawn the parent's ChangeAlpha thread on Me, take Me
  under scripted control (priority 4) and walk it Marker4 -> Marker5 every frame until the thread terminates. Shape: the
  landed family models (L139 0x00E0DAA0, CCorneredGuard 0x00DA7830, CLookAtMe1 0x00EF5CB0): out-of-line const
  CScriptThing copy 0x004ABE90 plus an inline non-const copy (Me into the by-value functor argument); `__forceinline`
  CCountedPointer copy/dtor; CPredicate_And ctors out of line, predicate dtors user-declared virtual; returns after the
  scripting wait under inline_depth(0). New: the functor parameter's dtor is out of line (0x004AA840) while the markers'
  dtors at the first return are inline -- VC7.1 instantiates class-template member bodies at the END of the TU (pragma
  state there applies; a pragma around the template is ignored), so the CSpawnedFunc1 ctor is an explicit specialization
  defined under `#pragma inline_depth(0)` (same body as the primary).
  - Name: slot 1 of vtable 0x012F64FC, which the landed CUndeadFellow::Alloc 0x00EF47C0 stores; Ego_r.pdb 0x00F74AB0
    `?Main@CUndeadFellow@CPersonalScript_EmptyScript@NScript@@EAEXXZ`; the spawned member pointer is the landed
    `?ChangeAlpha@CPersonalScript_EmptyScript@NScript@@QAEXVCScriptThing@@@Z` 0x00EF8BC0; callees all PDB-spelled
    (MoveToThing UAE per Ego_r 0x007E2B90, ~CIsThingAlive UAE per 0x00CC4A80, CBaseObject ctor IAE per 0x00B01FD0).

## Parked for Fable 5.1 (1)
- 0x00F3E6B0 CTriangleStripifier2::BuildVertexAndPolyMaps DIFFER(928v920): section 1 (vertexPolys) exact; section 2 keeps v*12 in ebx (retail esi, spilled [esp+0x10]) with vertexPolys not in ebp, and narrows the neighbour index with an extra `movzx eax,ax` + `mov bp,word` (retail holds it 32-bit in edx, spilled [esp+0x18]) | tried: ushort / long / unsigned long / int / unsigned int for vert, n and k; Contains helper; goto form; found flag vs inner break; the full STLport 4.6 vector model with out-of-line vector<long>::_M_insert_overflow specialization and same-TU erase/_M_fill_insert stand-ins | next: a CArray<unsigned short>::const reference / iterator-based neighbour scan (`std::find` over nb.begin()..nb.end()) or reading the triangle corner through a `const unsigned short*` row pointer, which may move the 32-bit widening to the load.
  - Name: Ego_r.pdb 0x00BD96F0 `?BuildVertexAndPolyMaps@CTriangleStripifier2@@AAEXABV?$CArray@G@@AAV?$CArray@V?$CArray@J@@@@AAV?$CArray@V?$CArray@G@@@@@Z` (private); the three CArray arguments and `ret 0xc` match, the body builds exactly those two maps.

## Skipped
None.

## Label corrections
- 0x00E295E0 "CScriptGameResourceObjectScriptedThingBase::HandleSisterArenaDialogue" is
  NScript::CQS_MeetSisterScript::CMeetSisterSister::Main (this is the entity script, not a resource object).
- 0x00F3E6B0 "CTriangleStripifier2::ProcessTriangles" is CTriangleStripifier2::BuildVertexAndPolyMaps.
- 0x00EC3D40 and 0x00EF87D0 were gap-scan sub_ rows (names above).
- The manifest labels the CV_RockTrollFirstEncounterScript binding factories (0x00EC41F0 etc.) "CTriangleList": wrong
  (propagated across byte-identical Alloc bodies).
- Landed 0x00E292C0 CMeetSisterMessenger::Main is declared as a global struct; Ego_r 0x00E75570 nests it in
  NScript::CQS_MeetSisterScript (NAME_FIXES.tsv).

## Header debt
- `rebuild/include/engine/CScriptGameResourceObjectScriptedThingBase.h` is a flat data view (no vtable, no
  CBaseObject base), unusable for the polymorphic locals of rows 00e295e0 / 00ef87d0; both model the class locally
  (`CBaseObject -> CScriptGameResourceObjectBase -> ...ScriptedThingBase`, vtable 0x0127094C, PImp +0x08).
- IsScripting 0x00CD23B9 const-ness is unproven (no PDB entry; bindings hold both QAE and QBE); QAE used (majority).
- The CScriptGameResourceObjectScriptedThingBase dtor 0x007E74D0 has no PDB entry (compiler-generated); spelled
  `??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ`.
- vland warns SHARED_HEADER_AVAILABLE for CSpawnedFuncBase (00ec3d40, 00ef87d0) and CGameScriptInterface (00ec3d40):
  `rebuild/include/engine/CSpawnedFuncBase.h` / `CGameScriptInterface.h` are generated data views without the vtables
  these rows call through (SuspendableProcess slots, the CGSI slot table), so the rows keep local polymorphic models.
- 0x00A5BEF0: vland reports RELOC_UNCHECKED (multi-block TU, the C4DVector inline helpers); checked by hand with
  tools/relocs.py -- its only data reloc is `?g_SSEAvailable@@3_NA` -> 0x013D2880 (the spelling already bound by the
  landed NVectorMath siblings; no PDB data entry found).

## Dry vland (tmp/payload_all.json = payload.json + the retail_asm row)
WINS 4 (00e295e0, 00ec3d40, 00ef87d0, 00a5bef0 all RELOCATION_MATCH + behaviour PASS), 0 RELOC_IDENTITY; lv/ deleted.
