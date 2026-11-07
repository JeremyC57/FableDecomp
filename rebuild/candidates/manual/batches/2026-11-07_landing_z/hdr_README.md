# Chunk 848 header overlay

Every edit is an additive opt-in branch: a new `#ifdef FABLE_RETAIL_...` block, or an `#ifdef/#else` (`#ifndef`) wrapper whose default
branch keeps the original line byte for byte. No default-path text changed. The copies were taken fresh from rebuild/include after
chunk 846 landed (its opt-ins are in these copies) and re-applied with `bin/reapply.py` + `bin/reapply_extra.py` just before
hand-back; `diff` against rebuild/include shows only added lines (0 removed) in the nine edited headers.

Includers rechecked read-only with this final overlay: 33 landed rows that include the edited headers (all of
CPersistContextReadSection.h and NThingManager_CEntityRuntimePersistence.h, samples of the others), all MATCH / RELOCATION_MATCH +
PASS (SIBLINGS.tsv, logs/recheck_siblings.log).

## Edited headers

| header | macro | change | evidence | row |
|---|---|---|---|---|
| fable_string.h | FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT | `GetAsCharArray` as a preset pointer plus `if` (the default `?:` line kept under `#else`) | inline CleanOutLine inside 0x009BC120 indexes `[ecx+eax]` (0x009BC610..0x009BC632); the ternary gives `[eax+ecx]`; the landed CleanOutLine 0x009BB0D0 uses the same preset form | 009bc120 |
| fable_string.h | FABLE_RETAIL_CCHARSTRING_REPLACE | `__forceinline void Replace(const CCharString&, const CCharString&)` | FableWin.pdb `?Replace@CCharString@@QAEXABV1@0@Z` (0x018D9CA0, public); LoadGameState expands it four times (0x0052573A..0x005257FD: strstr 0x00BFEB3A, the match cut with `mov byte ptr [esi],0`, inline strlen, `+=` 0x0099F0A0, operator= 0x0099EFB0). The second strstr starts at `found` (retail advances it before the piece copy) | 005253f0 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE | the tree's const `find` as a member template (declaration only) | owner decision D1 (STLport 4.6.1 _tree.h 457-467 under _STLP_MEMBER_TEMPLATES); retail calls the set<long> instance 0x00B2EB20 out of line with a hidden const_iterator result | b2d9f0 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_SET_FIND | `set::find(const K&) const { return _M_t.find(key); }` | STLport 4.6 _set.h; inline in retail, the tree find stays the call above | b2d9f0 |
| engine/CEngineResourceManager.h | FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK | inline non-const `GetGraphicBank(EGraphicBankNo)` | Ego_r.pdb `?GetGraphicBank@CEngineResourceManager@@QAEPAVCGraphicDataBank@@W4EGraphicBankNo@@@Z`; UpdateDiskCache reads GraphicBanks[0] at +0x954 (0x00B2DA07, 0x00B2DBA2) | b2d9f0 |
| engine/CGraphicFrame.h | FABLE_RETAIL_CGRAPHICFRAME_CTOR | inline `CGraphicFrame() : FrameData(0) { }` | Ego_r.pdb / FableWin.pdb `??0CGraphicFrame@@QAE@XZ`; the expanded CCompositeSurfaceCache ctor zeroes GraphicFrame +0x4C in member order before the ManagedTexture CBaseClass call 0x00B2DB0B | b2d9f0 |
| engine/CThingManager.h | FABLE_RETAIL_CTHINGMANAGER_LOADGAMESTATE | declarations `LoadGameState(CPersistContext&)`, `LoadHeroStateScript(CStringParser&)` 0x00520570, `SetNameForThing(unsigned __int64, const CWideString&)` 0x00521A90; NPCNameMap +0xCC typed as `std::map<unsigned __int64, std::pair<CCharString, CWideString> >` (the opaque `unsigned long[3]` kept under `#else`; STLport map is 12 bytes, same layout); `class CStringParser;` forward | Ego_r.pdb / FableWin.pdb mangled names (all public, out of line); the Ego_r member type of NPCNameMap; LoadGameState calls its operator[] out of line (0x0052B8B0) | 005253f0 |
| engine/NThingManager_CEntityRuntimePersistence.h | FABLE_RETAIL_CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT | declaration `LoadFromGameStateScript(CStringParser&)` 0x005248B0 + forward | Ego_r.pdb / FableWin.pdb `?LoadFromGameStateScript@CEntityRuntimePersistence@NThingManager@@QAEXAAVCStringParser@@@Z` | 005253f0 |
| engine/CPersistContextReadSection.h | FABLE_RETAIL_CPERSISTCONTEXTREADSECTION_IMPLICIT_DTOR | `#ifndef` round the declared dtor (0x00485CF1), so the TU's dtor is implicit | per-TU inline opt-in like FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY: one retail class; the thing-manager TU expands the destructor in place (0x00525C2C..0x00525CB9: ~Text, the parser's comment list, two strings, ~CBaseClass 0x0099A2E0, ~CDataStream 0x00993B10) where other TUs call the COMDAT | 005253f0 |
| engine/CStringParser.h | FABLE_RETAIL_CSTRINGPARSER_READLINE | declaration `CCharString ReadLineAsString()` 0x009BA390 | Ego_r.pdb `?ReadLineAsString@CStringParser@@QAE?AVCCharString@@XZ` (hidden result pointer) | 005253f0 |
| engine/CDataInputStream.h | FABLE_RETAIL_CDATAINPUTSTREAM_READPRESIZED | declaration `CCharString ReadPresizedString()` 0x00996390 | Ego_r.pdb `?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ` | 005253f0 |
| engine/CDataInputStream.h | FABLE_RETAIL_CDATAINPUTSTREAM_READUHUGE | `__forceinline unsigned __int64 ReadUHUGE() { return ReadType<unsigned __int64>(); }` | FableWin.pdb `?ReadUHUGE@CDataInputStream@@QAE_KXZ` (wraps `??$ReadType@_K@...`), expanded in retail like ReadULONG (an 8-byte inline read, 0x005255F0..0x0052566C) | 005253f0 (parked) |

## Unchanged copies (19)

`fable_string.h` is `#pragma once`. A repo root header that includes it pulls the repo copy past the overlay copy and gives C2011
CCharString redefinitions, so every root header that reaches fable_string.h is copied here byte for byte: fable_bank_file.h,
fable_boot.h, fable_definition_table.h, fable_filesystem.h, fable_game.h, fable_game_text_bank.h, fable_gfuninitialise.h,
fable_language.h, fable_profile.h, fable_registry.h, fable_render2d_vertex_queue.h, fable_resource_directories.h,
fable_retail_banks.h, fable_system.h, fable_user_profile.h, fable_video_system.h, fable_visual_boot.h, fable_window_title.h,
fable_write_permissions.h (the same set chunk 846 copies). They need no landing action beyond not being stale.

## Not edited

engine/CBankFile.h (HOLD) and rebuild_abi.h. engine/CIsThingAlive.h was tried in a temporary copy only (an implicit-dtor opt-in for
row 00dd4ad0, probe h5); it did not help and is not in this overlay.
