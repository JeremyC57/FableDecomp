# NF70 header overlays (round 7, slice land1117fu, q10724..q10818)

Ten overlay headers, 13 opt-in macros. Each overlay adds `#ifdef` / `#if defined(...)` blocks only, named after the member
(`FABLE_RETAIL_<CLASS>_<MEMBER>`). Without its macros each file is token-identical to the repo header: `work/tokchk.py`
strips comments and the NF70 blocks and compares the token streams (all ten TOKEN_IDENTICAL). `diff repo overlay | grep '^<'`
is empty for nine of them. The tenth, CHeroSuitDef.h, shows two `<` lines because its one-line class was split over several
lines to make room for the opt-in. The tokens are the same.

If the repo headers move before landing, re-apply the blocks below as they are. Each block is self-contained.

| Overlay | Opt-in macro | Rows using it | What it adds | Evidence |
|---|---|---|---|---|
| `engine/CHeroSuitDef.h` | `FABLE_RETAIL_CHEROSUITDEF_COPY` | 0x004E54AD | `virtual void Copy(const CDefClassBase*);` in both layout branches | Ego_r `?Copy@CHeroSuitDef@@UAEXPBVCDefClassBase@@@Z`; vtable slot checked against the exe |
| `engine/CSpecialEffectsDef.h` | `FABLE_RETAIL_CSPECIALEFFECTSDEF_COPY` | 0x0045C77F | `virtual void Copy(const CDefClassBase*);` (the slot-19 override) | Ego_r / FableWin `?Copy@CSpecialEffectsDef@@UAEXPBVCDefClassBase@@@Z` |
| `engine/C3DClothPrimitive.h` | `FABLE_RETAIL_C3DCLOTHPRIMITIVE_REMAPBONEINDICES` | 0x00AC3AF0 | `void RemapBoneIndices(std::map<unsigned long,unsigned long>&);` declaration (retail `mov ecx,[ecx+4] ; jmp` to the particle program's) | Ego_r 0x00C4B610 / FableWin 0x03496150 (q10799 spelling) |
| `engine/CParticleProgram.h` | `FABLE_RETAIL_CPARTICLEPROGRAM_REMAPBONEINDICES_PUBLIC` | 0x00AC3AF0 | public `RemapBoneIndices(std::map<...>&)` declaration, placed after GetInitialSimulation | Ego_r |
| `engine/CPersistContext.h` | `FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERHELPER_WEAPONTRAILGRAPHICSET` | 0x004E8FCD | `void TransferHelper(class CWeaponTrailGraphicSet* value);` | **DESCRIPTIVE** name (no PDB name for the helper); see NOTES.md |
| `engine/CMap.h` | `FABLE_RETAIL_CMAP_INITPASSABILITYFROMTHEMES` | 0x008201E0 | `InitPassabilityFromThemesIfNecessary(const C2DBoxI&)` and `C3DVector GetGroundFaceNormalAt(const C3DVector&) const` declarations, placed before the THEMESIZEZ block | Ego_r |
| `engine/CMap.h` | `FABLE_RETAIL_CMAP_GETSTRONGESTENGINETHEMEAT_BODY` (+ `_DEFINE`) | 0x0081F910 (BODY+DEFINE), 0x0081FA20, 0x0081FA90, 0x008201E0 (BODY) | the one body of GetStrongestEngineThemeAt 0x0081F910 after the size assert, guarded by `FABLE_RETAIL_CMAP_THEMESIZEZ && _BODY`. BODY alone gives an `inline __declspec(noinline)` COMDAT, and BODY plus DEFINE gives the ordinary definition (the self row). This is the q10735 one-definition fix. | Ego_r; the rows' former per-row copies were removed |
| `engine/CWorldMap.h` | `FABLE_RETAIL_CWORLDMAP_CONVERTCOORDSTOLOCALBLOCK` | 0x0063A7E0, 0x0063AE70 | `C2DCoordI ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI&) const` (0x004FC3C0) | Ego_r |
| `engine/CWorldMap.h` | `FABLE_RETAIL_CWORLDMAP_STRONGESTTHEMEATBLOCK` | 0x0063AE70 | `GetStrongestEngineThemeAtBlock` (0x0063AE70) | Ego_r |
| `engine/CTCHeroOnlineScoreboard.h` | `FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLEARSCOREBOARD` | 0x00562E70 | `private: void ClearScoreboard(); public:`, after the CLOSE block | Ego_r |
| `engine/CTCCreatureModeManager.h` | `FABLE_RETAIL_CTCCREATUREMODEMANAGER_ALLOCATEMODE_BODY` | 0x0073AE10 | the AllocateMode body as `inline __declspec(noinline)` (the caller needs the mod/ref knowledge; DIFFER 109v92 without it) | FableWin / Ego_r |
| `libpk/CItem.h` | `FABLE_RETAIL_CITEMDISPATCHER_FINDBYKEY` | 0x00C10140 | slots 16 and 17 of CItemDispatcher: `virtual void TransferAllFrom(PKCTRList<CItem>*);` and `virtual CItem* FindByKey(const unsigned long&);`, the same two the sibling list CNamedIndex declares in libpk/CBankProcessor.h. FindItem is `call [eax+44h]`. | retail bytes 0x00C10140 (MATCH); libpk has no PDB |

## Not overlaid

`fable_string.h` was not overlaid (q10763). The session's permission check refused the attempt, so nothing was written.
The exact opt-in that the one-definition fix for CBasicString<char>::operator< 0x00429950 needs is in NOTES.md and
`work/q10763_change.txt`.

## Repo opt-ins the rows use, unchanged

The rows also use existing repo opt-ins, unchanged:

- CPersistContext: `FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NAMED_BODY`
- CThingCreatureBase: `FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE`
- CIntelligentPointer: `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN`
- CThingPhysical: `FABLE_RETAIL_CTHINGPHYSICAL_CONSTRUCTFROMPARAMS`
- CThingGameObject: `FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT`
- STLport find: `FABLE_RETAIL_STLP_FIND`
- CStaticMapBankStateBlock: `FABLE_RETAIL_CSTATICMAPBANKSTATEBLOCK_CLASS`
- CItemDispatcher: `FABLE_RETAIL_CITEMDISPATCHER_FINDITEM`
- The tree and vector opt-ins in `stlport/fable_map.h`, including `FABLE_RETAIL_STLP_TREE_ASSIGN_BODY` and `_MCOPY_BODY`
- The CPooledResource and CSegmentFile pass-23 macros
