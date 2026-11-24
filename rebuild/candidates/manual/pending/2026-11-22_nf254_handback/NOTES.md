# NF254 notes (2026-11-22, round 20): CODEXP27 L4 / L5 family, L111, L114, L120 done; L28 / L57 open; L37 / L129 census + plan

Handed back early (context). 11 rows changed, all RELOCATION_MATCH + test PASS in one `vland --recheck-landed` run (recheck_final.log; 0
RELOC_CONFLICT). Three headers get one add-only opt-in each (hdr/, orig/, hdr/README.md; equiv_check over 57,125 rows: 0 differ). No new split: every address
touched ends on the same or fewer spellings (census per line below). Link of all 11 new objects + 7 HEAD TU-mate objects (0x00A90AE0,
0x0074FE50, 0x00D17A80, 0x0074F680, 0x004D9C5D, 0x00A42B80, 0x00A42CE0): 0 LNK4006 / LNK2005 (control with a duplicated object: 2 LNK4006,
so the check is live). No other landed row defines BuildBuffers / BuildOuterSkyMesh / CLipSyncDataBank::Update / MsgOnBoastsMade /
the CDeviceIndependent members (source grep), and no row includes any of the 11 changed sources (grep of src/include/tests). Repo root listed before / during / after: identical. No git, no
foreground sleep, every grade under timeout, every new source prechecked with clang before its grade.

## L4 / L5 + 0x0074FCE0 (done, one 7-row unit)
| row | before | after | grade / forms |
|---|---|---|---|
| 0x0074FCE0 overflow | FableVector_0074fce0 local model, `pair<int,CCharString>`, local CCharString / vector / helpers | stlport/fable_containers.h explicit instantiation `?_M_insert_overflow@?$vector@U?$pair@JVCCharString@@@std@@...@std@@IAEX...I_N@Z` (0x00CBC130 recipe + OUTOFLINE_UNINIT_COPY / _FILL_N + the new DESTROY_AUX size-body opt-in), /Gr from CompilerFlags | RELOCATION_MATCH, form 3 |
| 0x0074F4F0 | `UninitCopy_SPairLongCSecond_0099ec30_0074f4f0` stand-in | `??$__uninitialized_copy@PAU?$pair@JVCCharString@@@std@@PAU12@@std@@YI...` explicit instantiation (0x00AD7B50 recipe) | RELOCATION_MATCH, 1 |
| 0x0074F530 | `UninitFillN_SPairLongCSecond_...` stand-in | `??$__uninitialized_fill_n@PAU?$pair@JVCCharString@@@std@@IU12@@std@@YI...` | RELOCATION_MATCH, 1 |
| 0x008A9AE0 MsgOnBoastsMade | CBoastVector / CBoastPair / CCharString stand-ins, self `QBE_NAAUCBoastVector@@` | shared headers (OVERFLOW_DECL_ONLY), self `?MsgOnBoastsMade@CGameScriptInterface@@UBE_NAAV?$vector@...@Z` (both PDBs; vtable 0x01260F0C slot 34; `virtual` added in place on the HEAD local CGameScriptInterface model, nf248 ruling 1 / nf250 ruling 4) | RELOCATION_MATCH, 1 |
| 0x008ED1F0 InsertItem | CContainerItemVector / SContainerItem / CCharString stand-ins | shared headers (OVERFLOW_DECL_ONLY); Leaf line added (symbol unchanged) | RELOCATION_MATCH, 1 |
| 0x008ED440 | local namespace-std model with a 53-byte STAND-IN `_M_insert_overflow` body under the STLport name (NEQ v 0x0074FCE0: HEAD debt) | shared headers, OVERFLOW_OUTOFLINE + OUTOFLINE_UNINIT_COPY / _FILL_N: the copy is now EQ (249 = 249) | RELOCATION_MATCH, form 3 (form 1 also defined the /Gr-only DESTROY_AUX opt-in, which in this non-/Gr TU names a different, unowned YA symbol: dropped, same grade and copies; form 2 DECL_ONLY: DIFFER 143v143) |
| 0x0074FE50 push_back | already the STLport spelling | unchanged (HEAD object for census / link) | not-a-binder |
- Two sources for the pair<int> -> pair<long> self signature: Ego_r.pdb + FableWin.pdb GetRewardItems / CTCContainer::ContainerItems
  (vector<pair<long,CCharString> >, J) and the four binders' element type (all `J`); the STLport protected spelling is the one
  0x0074FE50 already binds (descriptive-by-library: both PDBs are Dinkumware builds). Helper template arguments are what the header
  emits for that vector (I = O = pair*, N = size_type = unsigned int).
- Census (table + objects): 0x0074FCE0 4 spellings (FableVector self, STLport, CBoastVector, CContainerItemVector) -> 1;
  0x0074F4F0 / 0x0074F530 2 -> 1 each; 0x008A9AE0 (no landed referencer) self only. 0x0099EC30: two users of `??0CSecond_0099ec30`
  move to `??0CCharString@@QAE@ABV0@@Z` (count-neutral there: 11 other rows keep the stand-in).
- Pick-any copies (pk.py, owners.tsv): helpers EQ 0x0074F4F0 / 0x0074F530 in every object that defines them; the overflow copy in
  0x008ED440 EQ 0x0074FCE0; push_back EQ 0x0074FE50; pair(a,b) EQ 0x00695250; `__destroy_aux<pair<long,CCharString>*>` EQ 0x004DD782
  with the new opt-in (form 1 had it 36 B NEQ; form 2, a row-tail optimize("s"), fixed it but made every template COMDAT /Os incl. the leaf:
  DIFFER 172v249, so the fix went into a header explicit specialization instead). All common COMDATs of the 0x0074FCE0 and 0x008ED440
  objects are identical (cmpobj).
- **D1 (decision): `??_G?$pair@JVCCharString@@@std@@QAEPAXI@Z`.** 0x0074FCE0 and 0x008ED440 newly define it at 33 B (the /O2 body) v
  owner 0x004DA68A (31 B, an optimize("s") TU). It is compiler-generated (from `p->~T()`), so no header specialization can give it the
  owner's mode; only an end-of-TU optimize("s") does, which breaks the leaf (form 2). The same 33-byte body under the same name is already
  defined by landed 0x00D17A80 at HEAD (cmpobj SAME), so no new body variant enters the build; neither row's LEAF references it and the
  table binds it at HEAD (witness 0x004DD782): the 2026-11-21 "not leaf-bound and bound at HEAD: debt, queued" case. nf250 ruling 2 held
  L86 for a NEQ `??_G` (there the copy met a leaf-bound use).
  Caveat (affects the opt-in): with FABLE_RETAIL_STLP_DESTROY_AUX_PAIR_LONG_CCHARSTRING_SIZE_BODY, 0x0074FCE0's `__destroy_aux` copy is
  byte-EQ 0x004DD782 and calls the in-object `??_G`; under the nf212 refinement (condition 3) that relocation counts as the owner's target
  only if the in-object `??_G` equals its own owner, which it does not. So the `__destroy_aux` copy is "bytes equal, relocs equal only once
  D1 is resolved". Options for the coordinator:
  (a) land as handed back (opt-in + D1 queued as debt);
  (b) land form 1 instead (no header change: the 36-byte /O2 `__destroy_aux` copy, NEQ v 0x004DD782, which references no `??_G`): a NEW
      NEQ copy of an owned symbol, so by the pick-any rule it is the weaker choice; work/drafts/0074fce0_f1.cpp;
  (c) hold the whole 7-row unit (0x0074F4F0 / 0x0074F530 cannot move without 0x0074FCE0) until a TU-emitter / size-mode device for
      `??_G` exists. My default: (a).
## L114 (done): 0x00A978C0 / 0x00A978A0
- The same-TU 0x00A90AE0 HEAD object already defines vtable 0x0129DF88, `??_GCDeviceIndependent@CAnimatedBlock@N3DPrimitive@@UAEPAXI@Z`
  (30 B, EQ 0x00A978A0) and `??1...@@UAE@XZ` (43 B, EQ 0x00A978C0) from engine/C3DPrimitive2.h FABLE_RETAIL_CANIMATEDBLOCK_DATA. Both rows
  copy its #define / #include block and emit the pair through an anonymous-namespace harness (`new CDeviceIndependent`, marked
  scaffolding, the nf250 / nf252 pattern), each with its own Leaf: RELOCATION_MATCH first form; each row's copy of the other is EQ.
- Names: FableWin.pdb ??1 / ??_E (debug); retail slot 0 is the scalar ??_G body; VC7.1's vtable COMDAT names the weak external ??_E,
  aliased to ??_G (objdump: storage class 105, tag = ??_G), slot for slot as 0x00A90AE0's (vtcheck NOLINE for both slots, no table line).
- Census: 0x00A978C0 self stand-in + the ??_G's `CDeviceIndependent_dtor` stand-in = 2 -> 1; 0x00A978A0 self stand-in + the vtable's
  ??_E(->??_G) = 2 -> 1 (??_E counted as the alias of ??_G).
- Unowned copies: `allocator<unsigned long>::deallocate` / `allocator<C3DIndexedBoneWeight>::deallocate` differ from 0x00A90AE0's
  (that row ends with a row-tail `#pragma optimize("s", on)`, so its pick-any template copies are /Os); no retail owner is known for
  either (no table line), so no pick-any rule applies; not mirrored.

## L111 (done): 0x00A427A0
- Self `?Update@CLipSyncDataBank_Methods@@QAEXXZ` -> `?Update@CLipSyncDataBank@@UAEXXZ` (Ego_r.pdb + FableWin.pdb; vtable 0x0129D3D4
  slot 7), body `if (!SkipUpdate) CBankFile::Update();` (tail jump), on FABLE_RETAIL_CLIPSYNCDATABANK_CLASS with the 0x00A42980 prelude +
  the new FABLE_RETAIL_CLIPSYNCDATABANK_UPDATE. 0x009CEC60: 2 -> 1. Vtable census: 0x00A42B80's HEAD object emits a 1-slot stand-in
  vtable (`?v00@CLipSyncDataBank@@UAEXXZ`), 0x00A42CE0 none reaching slot 7, so no slot binder exists.

## L120 (done): 0x00B620A0 (+ 0x00B61B30)
- 0x00B620A0 off the CSkyRendererMethods stand-in onto the header's private BuildBuffers; its four calls on the PDB private builders
  (new opt-in FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES for three of them). Census: 0x00B620A0 2 -> 1, 0x00B61DD0 2 -> 1,
  0x00B61B30 2 -> 1 (its self QAE -> AAE in place), 0x00B61CD0 2 -> 2, 0x00B61FF0 2 -> 2 (count-neutral: CSkyRendererMethods stand-in
  replaced by the PDB spelling; their self rows still spell the CArray& public form / the CSkyRendererMethods stand-in: NAME_FIXES).
- Follow-ups: 0x00B61CD0 self-signature port (CTVertexColTex1** / unsigned short**, both PDBs + the caller) onto engine/CTVertexColTex1.h;
  0x00B61FF0 onto the retail sky header branch with the new opt-in.

## Open: L28, L57 (not started, context)
- L28 plan: 0x0063E480 onto the 0x0049C490 self row's define block (CTHINGSEARCHTOOLS + ORIGINAL_ROUGHAREA + ROUGHAREA_EGOD +
  OVERFLOW_OUTOFLINE ...) plus engine/CThingFilter_HasPhysicsMesh.h (a header now exists; 0x0049BE90 still models it locally, debt),
  engine/CWorldMap.h FABLE_RETAIL_CWORLDMAP_HEIGHTZ (declares GetHeightZBelowPos QBE), C2DBoxI(long x4), std::vector<const CThing*>; the
  same-TU stub becomes the visible template instantiation, whose pick-any copy must be EQ 0x0049C490 (pk.py) and the leaf must keep the
  vector start in ebp (HEAD note). Check first whether the filter argument is HasPhysicsMesh (table: the binder's stand-in says All).
- L57 plan: engine/CGameScriptInterface.h already declares `virtual bool IsPlayerCreatureFlourishEnabled() const;` (slot 152, both PDBs
  ?IsPlayerCreatureFlourishEnabled@CGameScriptInterface@@UBE_NXZ). Body: main player -> GetPControlledCreature 0x00487DC0 -> alive
  (+0x91 bit 0) and HasTC(0x30) (+0x24 bit 16) -> the NON-const GetTC path (its LowerBound is 0x0040F020, not PeekTC's 0x004365B0)
  -> CTCCombat::IsMeleeFlourishValid 0x00733EE0 (engine/CTCCombat.h) as a thiscall. Find the CThing.h GetTC variant whose expansion
  calls 0x0040F020 (grep landed rows binding `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...QAE`).

## L37 (census + plan only): 0x00535470 vector<unsigned long> copy ctor
- Correct spelling: `??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z` (STLport, table witness 0x009B7650). Self row leaf prefix
  `??0?$vector@` over a local model. Table spellings (6): that one; `??0?$vector@K@std@@QAE@ABV01@@Z` (one-parameter local std model,
  witness 0x00ADCE10); `??0CULongVector@@QAE@ABU0@@Z` (0x006250B0); `??0?$CRetailVector@K@@QAE@ABU0@@Z` (0x009B0AC0);
  `??0CGroupArray@@QAE@ABU0@@Z` (0x00AD9B20; 0x00ADA240 also models CGroupArray); `?ctor@CCharString@@QAEXPAV1@@Z` (0x00ADC310, a
  _Cons_val row; 0x009D3CA0 is its twin shape).
- The 26 referencers by source shape (work/l37_census.txt): 14 local namespace-std models (009B7650 .. 009D4810, 00AD94F0, 00AD9720,
  00ADCE10 / 00ADCE60 / 00ADCFD0, 00531560); 4 on shared headers (00A90AE0 fable_containers.h, 009B9290 / 00AD9770 / 00AD9980 CArray.h);
  stand-in classes 006250B0 (CULongVector), 009B0AC0 (CRetailVector), 00AD9B20 / 00ADA240 (CGroupArray); 009D3CA0 / 00ADC310 (_Cons_val,
  ctor@CCharString); 00531090 / 00551340 (no vector token: compile to see). Five are /Gr (00531090, 009B7650, 009B8770, 009B8C00, 009B9290).
- Compiled census (HEAD objects, all 26 + the self; work/l37_bind_final.tsv; locks NF254): the self row already DEFINES the right
  `??0?$vector@KV?$allocator@K@std@@@std@@QAE@ABV01@@Z`; 18 binders bind it (00531090, 00531560, 00551340, 009B7650, 009B8220,
  009B8770, 009B8C00, 009B8DD0, 009B9170, 009B9290, 009D40B0, 00A90AE0, 00AD94F0, 00AD9720, 00AD9770, 00AD9980, 00ADCE60, 00ADCFD0);
  8 bind stand-ins: 006250B0 `??0CULongVector@@QAE@ABU0@@Z`, 009B0AC0 `??0?$CRetailVector@K@@QAE@ABU0@@Z`, 009D4810 and 00ADCE10
  `??0?$vector@K@std@@QAE@ABV01@@Z`, 00AD9B20 and 00ADA240 `??0CGroupArray@@QAE@ABU0@@Z`, 00ADC310 `?ctor@CCharString@@QAEXPAV1@@Z`,
  and 009D3CA0 `?copy_00535470@CArrayUL@@QAEXPBU1@@Z` (a 7th spelling with no table line). 7 spellings today -> 1 is the target.
  (The grade column did not parse for 009D3CA0 / 009D40B0 / 00ADC310: regrade before porting.)
- Plan: one family hand-back of the 8 stand-in binders (the self row and the 18 already-right binders do not change): port each onto
  stlport/fable_containers.h (+ engine/CArray.h where the row is a CArray user: CGroupArray / CArrayUL are CArray<unsigned long> stand-ins,
  CULongVector / CRetailVector vector<unsigned long>), the one-parameter `vector<K>` models (009D4810, 00ADCE10) by adding the allocator
  argument (in place: the local std::vector model gains its default second parameter) or a port; the two _Cons_val rows (009D3CA0,
  00ADC310) onto the map value_type copy. Each object's pick-any `??0?$vector@K...` copy must be EQ 0x00535470 (pk.py).

## L129 (census + plan only): 0x01436EA8 data global
- 91 landed referencers (absolute), 18 table spellings (census0.txt entry 19). `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`
  is in Ego_r.pdb, FableWin.pdb and Ego_d.pdb (raw grep of the PDB files) and 7 landed rows spell it (witness 0x00BEAE50); its sibling
  0x01436E98 is the proven NEngineInternal::GPResourceManager (NF46). The address proof (PDB data-symbol order / the S_GDATA32 record
  next to GPResourceManager, plus what the 0x00BE*/0x00BF* referencers read through it) is owed before any respell.
- Plan: (1) prove the address (two sources); (2) declare it in engine/CEngineLandscapeRenderer.h behind an add-only opt-in;
  (3) respell the 84 stand-in rows in payloads of 20-30 by pointee family (CEngineGlobal / CEngineGlobals / EngineGlobal /
  CStaticMapEngineState / LocalDetailEngine / SEngineLandscape / NativeLandscapeRenderer / Native::Renderer / GB / AsyncRoot / DAT_ /
  CEngine), each family's stand-in pointee replaced by CEngineLandscapeRenderer members (a port per family, not a rename).

## Flags lines / funclets / tests / descriptive names / slips
- Flags: no new CompilerFlags. 0x0074FCE0 keeps its catalog /Gr (its `// Flags: /Gr` line dropped: the line does not reach the build).
- Funclets: none of the changed sources is included by another row (grep of `../src/compiled/` includes for the 11 file names: none).
- Tests: all 11 keep their HEAD tests (self-contained host models; PASS in recheck_final.log); rewriting them on the shared headers is owed.
- Descriptive names: the harness `EmitDeviceIndependentVirtuals` (anonymous namespace, scaffolding). STL template names are descriptive-by-library.
- Catalog renames needed (RENAMES section 1): 0x0074FCE0 module int -> long, 0x0074F4F0 / 0x0074F530 (Module std, the STLport template
  names), 0x00A978C0 (class N3DPrimitive::CAnimatedBlock::CDeviceIndependent, ~CDeviceIndependent). The others are leaf-only.
- Locks: rows.txt (23 claimed; 11 changed). Slip-adjacent: the HEAD control compiles of landed sources (sc.sh pH side, headobj.sh) had
  no clang precheck; every new source was prechecked. 0x008A9AE0 slot binder: 0x006E7740's row-local CGameScriptInterface model declares
  2 virtuals (~CBaseClassNonCopyable, ~CGameScriptInterface), so no landed vtable COMDAT reaches slot 34 (the table line names the vtable
  only as the ctor's vptr store). L57 offsets (+0x91 bit 0, +0x24 bit 16) and the L129 address are read from HEAD sources / one PDB name:
  unproven until the plans run. pk.py owner map: work/owners.tsv (copied as owners.tsv).
