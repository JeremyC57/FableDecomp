# Chunk 707 header overlays (hdr/engine/)

All four edits are **opt-in branches**. Each one adds a new `FABLE_RETAIL_*` macro (gen_class_headers.py `RETAIL_MARK` keeps them).
The default text is unchanged in every header. The diff against `rebuild/include/engine/` removes one line per header and nothing
else: the "do not edit by hand" banner, now "then hand-corrected to retail". No landed source defines any of the new macros
(`grep -rE` over rebuild/src and rebuild/include: 0 hits), so every existing includer still preprocesses to the same text.
That covers CGraphicFrame.h 13 includers, CVirtualKeyboard.h 5, CAIStateGroup_ApprenticeHero.h 6 and CAIStateGroupBase.h 214, of which
98 define FABLE_RETAIL_CAISTATEGROUPBASE. All ten chunk-707 rows grade RELOCATION_MATCH with FABLE_HDR_OVERLAY=hdr.

| header | macro (new) | used by | change |
|---|---|---|---|
| engine/CGraphicFrame.h | FABLE_RETAIL_CGRAPHICFRAME | 0x009FCB20, 0x009FCB60 | Adds the retail class `CGraphicFrame { CGraphicFrameData* FrameData; }` (sizeof 4, not polymorphic), with `InitialiseAsSurface(const CCountedPointer<CTexture>&, long, long, bool, bool)` 0x009FCB20 and `InitialiseAsSurfaceSection(const CCountedPointer<CTexture>&, float, float, long, long, bool, bool)` 0x009FCB60. Includes engine/CCountedPointer.h. The flat pack(1) layout stays in `#else`. |
| engine/CVirtualKeyboard.h | FABLE_RETAIL_CVIRTUALKEYBOARD | 0x00851720 | Adds the retail class with `CWideString ProfileName` +0 (fable_string.h), `bool Canceled` +4, `bool Done` +5, `NUISystem::CComponent* PEntryScreen` +8 and `NUISystem::CEditBox* PEntryBox` +0xc (sizeof 0x10, no vtable). Member functions: ctor(CComponent*), ~, Initialize, Cancel, GetProfileName, Confirm (Ego_r, donor order = retail 0x00851700..0x00851920) and Update / IsDone / WasCanceled / Render (FableWin). |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS (inside the existing FABLE_RETAIL_CAISTATEGROUPBASE branch) | 0x0092B3A0 (through ApprenticeHero.h) | Adds the virtual slots 1-13 of vtable 0x01288C60 with FableWin/Ego_r spellings and access: Initialise, OnActivate, InitCreaturePosition, AllowPriorityOverlap, IsStillValid const, GetDebugText const, IsTimingCritical const, GetStateGroupType const; protected Cleanup, OnReplace, IsRunnable, `Update() = 0`, InitStateGroup(const CWorld&, CThingAICreature&). Also protected `IsStopping() const` 0x008FD010 and public `Terminate()` 0x008FCF40. Each base body was checked in retail; the slot addresses are in the comment. Without the second macro the existing retail branch is byte-for-byte what it was. |
| engine/CAIStateGroup_ApprenticeHero.h | FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO | 0x0092B3A0 | Defines both CAIStateGroupBase macros and includes that header. Declares `class CAIStateGroup_ApprenticeHero : public CAIStateGroupBase` (RTTI) with the ctor 0x008FE862, protected overrides of slots 3/7/11/12/13 and the new slot 14 `PracticeSkills()`. Members: NextFramePractice +0x14, PDummy, Action, PracticeOpponent, CCharString Interaction +0x24, NumFights, Submissive, Critical +0x2d. Static asserts check size 0x30. The flat layout stays in `#else`. |

Evidence for each header is in its comment block, and in NOTES.md under the rows that use it.
