# Workflow wf12 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 009ff220 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The match did not change from the original hand-back: the source changes were comments only, and the header change only moved where an include is guarded.
- OPEN at hand-back: final.cpp / payload source_cpp header comment says the header argument works because 'a PeekGraphicHeader() accessor lets c2 reuse the GraphicInfo read across the IsBump call', but no PeekGraphicHeader exists in the row, the hdr/ overlay or rebuild/include/engine (the code passes GraphicInfo->Header directly), so this comment is stale and misleading and must be reworded to describe the actual source (NOTES.md item (3) repeats the same stale claim).
- OPEN at hand-back: hdr/engine/CGraphicInfo.h overlay defines `bool IsBump() const` under FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME while the same header already defines an identical IsBump() under FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT (followed by `private:`), so any TU that defines both macros gets a redefinition error (and IsVolumeTexture would land in the private section); guard it, e.g. `#if defined(FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME) && !defined(FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT)` for IsBump, or merge the two opt-ins.

## 0082b7e0 -- VERIFIED, author attempts 1

- What made it match: Ported onto the shared headers (STLport vector/list; Open/IsOpen/Close on CFadeInFadeOutBase; IsPerformingRegionLoad, IsScreenFadeOut, IsPaused; GetMainPlayer().GetPControlledCreature(), IsDead, AddAutoMarker). Each loop binds `const CMarkerInfo& marker = *it`. The inline IsHeroMarker binds PeekGuiDef() to a const reference local before indexing MiniMapMarkerGraphics[0]. The tail keeps the direct `PeekGuiDef().MiniMapMarkerGraphics[0]` form, which produces retail's dead `add eax,958h`.

## 0084d910 -- VERIFIED, author attempts 2

- What made it match: The source is unchanged apart from comments, and the overlay was rebased on the current repo header. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf12/0084d910/0084d910.cpp and /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf12/0084d910/hdr/engine/CGameCameraManager.h. The row still grades RELOCATION_MATCH, also with strict headers, and addmatch.py rewrote payload.json.

## 006507d5 -- VERIFIED, author attempts 1

- What made it match: Same code as before the repair; only comments were added, citing the retail evidence for the GFToScreen inline body and for the branch-local double return.

## 007af490 -- VERIFIED, author attempts 1

- What made it match: The code is unchanged and only comments were added. GETEFFECT_BODY (a same-TU noinline copy of the real body of landed row 0x00669D30) lets VC7.1 treat the effect local as uncaptured. Its slot [esp+0x24] is then reused for the knockdown spill, as in retail.

## 007f9960 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged apart from the class model moving into a shared overlay header. It still grades RELOCATION_MATCH.

## 0096cb40 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged. The repair only puts the overlay's forward declaration behind the GETSEAT opt-in guard.

## 007bde31 -- VERIFIED, author attempts 1

- What made it match: The repair only changes names and comments; the bytes are the same. The enum value gives the same push 10h. Files: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf12/007bde31/src.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf12/007bde31/test.cpp, /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf12/007bde31/payload.json

## 007d2cc0 -- VERIFIED, author attempts 1

- What made it match: Visible-but-noinline CBasicString<char>::operator==(const char*) with Compare's real body in the TU (the call writes no memory, so one null test covers both literal tests); CoinStartPos through a const reference; and the empty/"NULL" skip written as `continue`, which put name below def in the frame.

## 004085f0 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Same as before the repair: the by-name GetDef<T> body is visible in the TU, now through the shared-header opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY, and each scheme branch has its own CDefPointer local.
- OPEN at hand-back: The source declares the flag at 0x013B85F4 as `extern unsigned char g_bLeftClickIsKing`, but that address is already landed as `extern bool g_bControlSchemeLeftClickIsKing` in rebuild/src/compiled/00/80/CThingFilterIsValidTargetForPlayerCreature_operator_00808e40.cpp:42. That gives one retail address two names and two types, which breaks the one-address-one-name rule. NAME_FIXES.tsv does not cover it, so the row should use the landed spelling (and the landed type bool, then regrade) or carry a NAME_FIXES entry that justifies a respelling.
- OPEN at hand-back: Data name conflict: the landed row rebuild/src/compiled/00/80/CThingFilterIsValidTargetForPlayerCreature_operator_00808e40.cpp already names 0x013B85F4 `extern bool g_bControlSchemeLeftClickIsKing` (descriptive), and its comment even cites this function as the second reader. This row declares the same address as `extern unsigned char g_bLeftClickIsKing`, which breaks the one-address-one-name rule. NOTES.md also wrongly says no landed name exists. The row should use `g_bControlSchemeLeftClickIsKing` (and reconcile bool vs unsigned char), or list a NAME_FIXES entry with evidence.
- OPEN at hand-back: Faithfulness: the startup-manager presence test is written as `!g_StartupDefinitionManagerInstance.IsNull()` on a made-up global. The row's own comment says the retail `cmp [0x013B8760],0` is the inlined FableWin `?IsAllocated@CStartupDefinitionManager@@SI_NXZ` (FableWin 0x018377B4 / 0x01888A60). The original code called `CStartupDefinitionManager::IsAllocated()`, so the source should call that, declared inline in engine/CStartupDefinitionManager.h. Reaching into the descriptive counted-pointer global is not what Lionhead wrote.
- OPEN at hand-back: Unexplained steering construct: the CGameDefinitionManager branches call `CGameDefinitionManager::Get().CDefinitionManager::GetDef(...)` with an explicit base qualification, while the CStartupDefinitionManager branches call GetDef unqualified. The only apparent reason is that the shared header declares a non-retail `CGameDefinitionManager::GetDef(long, CDefPointer<T>&)` that hides the base member (the header's own comment says no such member exists in Ego_r/FableWin). No comment in the row explains it. Remove the non-retail declaration (or put it behind an opt-in) so the call reads as written, or add a comment justifying it.

