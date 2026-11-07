# Chunk 855 notes (8 rows: 3 matched, 5 parked)

Matched (all RELOCATION_MATCH, tests PASS under g++ and VC7.1): 0x00CA2BB0 floor1_encode, 0x00490BE0
CSubtitleRenderer::GetGraphicFromEmbeddedTag, 0x0052F900 NUISystem::CComponent::UpdateColour. Parked (differ/ with tests):
0x007602F0, 0x00A68DE0, 0x00642AA0, 0x00D0F640, 0x00507070. Every row cut checked: each ends at its own `ret` (+ jump table for
0x00642AA0) with filler before the next function; no wrong cuts. All eight names are proven (two sources each, LABELS.tsv); the
only descriptive names are the two 0x00507070 globals, CGameDefinitionManager::PGlobalDef / PeekGlobalDef (chunk 854's) and the
EVolumePool enumerator.

## 0x00CA2BB0 floor1_encode -- RELOCATION_MATCH (1 grade)
Codex pass-12 source unchanged on the repo's engine/Vorbis.h (C TU, `// Flags: /TC /Gr`); Xiph floor1.c identity, file-local
ilog/ilog2/render helpers kept static noinline. Label CEnvironment_SerializeTheme is wrong.
vland: WARN RELOC_CONFLICT @ilog@4 -> 0x00CA1790 here, 0x00C9FE70 in landed 0x00C9FEA0: libvorbis defines a file-local
`static int ilog` in several .c files (floor1.c here); two static functions, two addresses, same source name -- expected, not a fix.

## 0x00490BE0 CSubtitleRenderer::GetGraphicFromEmbeddedTag -- RELOCATION_MATCH (14 attempts)
Identity: Ego_r + FableWin private const ?GetGraphicFromEmbeddedTag@CSubtitleRenderer@@ABEJABVCWideString@@@Z, sole caller
ProcessEmbeddedGraphicTags 0x0049241E. Shape: each tag `if (tag == L"X") return ...;` as its own if (an `||` pair inverts the null
branch), the script-sprite map lookup found-first. Callees: CCharString::GetAsUpperCase 0x0099EE20 (landed as "ToUpper": NAME_FIXES),
CPlayerGui::PeekGuiDef 0x00438570 (static __fastcall), _M_find<CCharString> 0x0042B467, wcscmp import 0x00BFE94C.
Headers: fable_string.h GETASUPPERCASE, CSubtitleRenderer.h GRAPHICTAG, CPlayerGui.h PEEKGUIDEF, new generated CPlayerGuiDef.h
(+ SCRIPTSPRITES opt-in). Label ProcessEmbeddedGraphicTags_ResolveButtonName is wrong.

## 0x0052F900 NUISystem::CComponent::UpdateColour -- RELOCATION_MATCH (153 attempts)
Identity: NUISystem::CSprite vtable 0x0122F5D4 slot 40 + Ego_r/FableWin ?UpdateColour@CComponent@NUISystem@@UAEXM@Z between UpdateZoom
0x0052F5C0 and UpdatePosition 0x0052FFD0. Key spellings: CRGBFloatColour::Clamp must be __forceinline (opt-in); acceleration and
the result assigned (temp + copy); velocity scaled per channel by `1.0f / ColourTime`; the render colour a third named local
`render = colour * parent` with parent declared first. Label GameAnimation_UpdatePosition is wrong.

## 0x007602F0 NShape::IsShapeConvex -- DIFFER(1135v1126), 179 lines, 157 attempts
Name proven (Ego_r 0x00771990 / FableWin 0x025D50C0, followed by GetFirstPointThatBreaksConvexity in both PDBs as in retail, where
0x00760760 is landed). No structural residue: the call sequence (three _CIacos 0x00BFEBA2) and constants pair; what is left is
the register colouring of the outline iterators. Chunk 854's a8 base was 278 lines; the best tail is
`acos(c)*K + (acos(closing)*K + total)` and `return t * t < 0.0025f`. 128 statement toggles all canonicalise to the same code.

## 0x00A68DE0 CVolumeTexture::LoadFromDataStream -- DIFFER(923v919), 134 lines, 113 attempts
Name proven (Ego_r 0x00BEA360 / FableWin 0x0332D160 + the proven CVolumeTexture members it calls; caller 0x00A69530). New headers
for the volume texture family (hdr/README.md). The body is retail's as written: the uncompressed level reads into the locked
memory and copies rows (RowPitch) / slices (SlicePitch) into a packed buffer; both 32-bit overflow guards (__allmul) return with
the box still locked. Residue: `this` spill slot and the width/Z register roles. Label note: retail calls vector<1-byte>(n)
0x005136B0, which is byte-identical to 0x00514E20; both are landed as ??0?$vector@EV?$allocator@E@std@@@std@@QAE@I@Z, so one of
them is another 1-byte element type (char / signed char / bool) -- one retail address per name is violated, no proof yet which.

## 0x00642AA0 CMinimapData::PopulateMinimapThemeList -- DIFFER(1308v1340), 447 lines, 142 attempts
Name proven (Ego_r 0x00686400 / FableWin 0x0218B6C0; caller 0x006430BC). The call sequence agrees call for call; the 8-case
switch maps EMinimapThemeType 1..7 to minimap themes 1,6,4,5,2,7,3. Residue is frame layout only (retail 0x120 bytes with
separate argument / result / mesh / intersection slots, ours 0x110) plus the first loop's register roles. The GetDef<T> template
is the sibling 0x0066CF00's explicit re-read body (the template operator= caches the pointer). Sibling CRegionMinimap::
PopulateRegionMinimap 0x0066CF00 (chunk 802 differ) shows the same frame residue.

## 0x00D0F640 NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaMassacre -- DIFFER(1893v2101), 330 lines, 97 attempts
Name proven (Ego_r 0x00D13E10 / FableWin 0x0368B050; referenced from Main at 0x00D04165). Retail's per-site inline split: the
~vector<CScriptThing> COMDAT 0x008AC970 is called at the returns nested inside `if (MsgGetThingsKilled)`, at the massacre entry
and in the massacre tail, expanded at the loop end and the four loop-level returns; ~CScriptThing 0x004AA840 is called for the
Gate3Guard temporary but expanded for the leader temporaries. The 72-variant sweep of dtor declaration forms and loop shapes gives only all-or-nothing outcomes;
the static-frequency levers (docs/pipeline/INLINE_DEPTH_REVISIT.md, last section) are the next step. The kills/things frame slots are swapped against retail and no declaration order or scope swaps them. Fixed on the way:
the cast map's ctor expands (malloc'd header) and its dtor is the COMDAT (new FABLE_RETAIL_STLP_MAP_OUTOFLINE_DTOR).

## 0x00507070 CWorldMap::BuildStaticMapGenerationVisibilityInfo -- DIFFER(1628v1432), 403 lines, 112 attempts
Name proven (Ego_r 0x004DAF10 / FableWin 0x01C91A40; CWorldMap block between the ctor 0x005066E0 and DeleteAllRegions 0x00507610;
CStaticMapGenerationVisibiilityInfo layout: Maps +0 / SeaBodies +0xC / EstimatedMaxCameraHeightAboveLandscape +0x18). No retail caller
(dead tools path). Fixed: __destroy_aux is __fastcall in this TU (new FABLE_RETAIL_STLP_FASTCALL_DESTROY_AUX), vector<long>::
_M_insert_overflow 0x00440990 is called from both push loops (declared-only specialization), __uninitialized_copy<CSeaBodyDesc*>
0x00518C00 out of line (FABLE_STLP_OUTOFLINE_UNINIT_COPY). Residue: the vector<long> copy constructor 0x00514160 is called for the
two CSeaBodyDesc copies inside the inlined SeaBodies.push_back but expanded for the visMaps copy (static-frequency split).
Descriptive: g_bBuildRetailStaticMaps 0x013B864A, g_bEnableMapOptimisations 0x013B864B.
Header debt: CRegionDef (HasSea retail +0xA4), CGlobalDef (+0x58 StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape) and
CStaticMapGenerationVisibiilityInfo (CMapDesc { set<long> VisibleMaps }, CSeaBodyDesc { CCharString Identifier; CArray<long> Maps })
are local models in the differ source; no usable engine/ header exists (CGlobalDef.h is quarantined).

## Labels to correct
See LABELS.tsv (10 rows; the two CCharString case converters are name fixes in NAME_FIXES.tsv).

## Diagnostics that did not count as attempts
calls.sh (call/ret sequence diff with built relocation symbols), relpair.py, rd.py, dis.sh; the 96 a68 variants of the W=2
generator branch that failed to compile are not counted.
