# hdr/ overlay -- chunk 890 header changes (additive opt-ins; default paths unchanged; diff against rebuild/include)

## engine/NUISystem_CTable.h -- `FABLE_RETAIL_CTABLE_CONSTRUCT`
Row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw. Declares the three virtuals CTable introduces after
CChangingStateComponent's last slot (141), protected, in vtable order (rtti-vtables CTable 0x0124A224: slot 142 0x00551EA0,
143 0x005518E0, 144 0x00551BC0; the row calls 143/144 as `call [eax+23Ch]` / `call [eax+240h]`):
- `?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ`
- `?ConstructHorizontalLine@CTable@NUISystem@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z`
- `?ConstructVerticalLine@CTable@NUISystem@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z`
Sources: Ego_r.pdb and FableWin.pdb (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv). Parameter names descriptive.
Declarations only (no layout change; the landed 0x005518E0 / 0x00551BC0 still use their own local models -- at landing they can
move onto this opt-in).

## stlport/fable_map.h -- `FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE`
Row 0x00551EA0. Owner decision D1 (2026-11-03) applied to the non-const `_Rb_tree::find`: STLport 4.6.1 `_tree.h`
`template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }` (under _STLP_MEMBER_TEMPLATES). Same
body as the default; only the symbol changes, to the member-template spelling retail's one-address-one-name rule needs:
0x00553520 = `??$find@W4ETableSprites@NUISystem@@@?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@...`
(called out of line at three sites of the row, expanded at the others). The const twin is untouched.

# Session 2 -- row 0x00498490 CProgressDisplay::DrawRetailDisplay (parked, DIFFER 5101v5101; all additive, default paths unchanged)

## engine/CProgressDisplay.h -- NEW
Ego_r.pdb CProgressDisplay at retail sizes (STLport vectors 0xC, doubles 8-aligned): sizeof 0x88; every offset the landed ctor
0x00499CE0 / dtor 0x00499FB0 / StopProgress 0x00499D90 / RenderProgress 0x00499880 and this row touch is a member. Member functions
in FableWin.pdb spelling (TU 0x018AD6E0..0x018B5330) with retail addresses; static PFont 0x013B8998. Forces
FABLE_RETAIL_CAPROGRESSDISPLAY (needs FABLE_RETAIL_CTEXTURE). fable_boot.h keeps the older stand-in.

## engine/CAProgressDisplay.h -- `FABLE_RETAIL_CAPROGRESSDISPLAY`
The polymorphic base (vptr only, same size): slot 0 dtor, 1 StartProgress, 2 StopProgress, 3 DisplayProgress, 4 InitialiseTextures
0x00497850 (empty), 5 SetToDisplayText 0x00497860 (empty), 6 IsActive (rtti-vtables.tsv); the pack(1) struct is the #else path.

## engine/CNewFrontendGameComponent.h -- NEW
Ego_r.pdb CNewFrontendGameComponent on CGameComponent (0x10): m_pMenuFont +0x10 .. m_pTextBank +0x60, m_pInputManager +0x68, rest
opaque to 0x148. Private static m_pNewFrontendGameComponent 0x013B871C; inline FableWin accessors Get 0x0187B470, IsTextBankInitialised
0x018B4760, GetTextBank 0x0187B480 (all expanded in retail: GetPTextBank 0x00497B30 and this row).

## engine/CMainGameComponent.h -- `FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED` (nested in the TEXTBANK opt-in)
FableWin ?IsTextBankInitialised@CMainGameComponent@@QBE_NXZ (0x018B4730), inline: tests the PTextBank object word.

## engine/CTBaseSingletonNoDefaultAlloc.h -- `FABLE_RETAIL_SINGLETON_ISSETUP`
FableWin ?IsSingletonSetup@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SI_NXZ (0x018B5D80), public static __fastcall, inline
(`mov eax,[013B8790h] ; test eax,eax ; je` at 0x004985F8 / 0x00499167).

## engine/CPlayerGui.h -- `FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK` (+ PeekGraphicBank under FABLE_RETAIL_CPLAYERGUI_CTOR)
?ChangeGraphicsBankScene@CPlayerGui@@QAEXXZ 0x00437570 (Ego_r 0x0044A200, FableWin 0x01A7CC80; body `mov ecx,[ecx+14h] ; jmp
CGraphicDataBank::ChangeScene`); PeekGraphicBank() const { return *Graphics; } (FableWin 0x018B1330, inline).

## engine/CPlayerGuiDef.h -- `FABLE_RETAIL_CPLAYERGUIDEF_PROGRESS`
RegionChangeGuildSealGraphics +0xA24 and LoadProgressBackdropGraphics +0xA40 typed as std::vector<long> (12 bytes each, the
opaque arrays are the #else path).

## engine/C2DBoxF.h -- `FABLE_RETAIL_C2DBOXF_EXTENTS`
FableWin ??0C2DBoxF@@QAE@ABVC2DVector@@ABVC2DExtentsF@@@Z (0x018B13C0), inline: TLX/TLY from the corner, BRX/BRY = corner + extents.

## engine/CRenderManager2D.h -- `FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN`
Draw2DScaledGraphic 0x009DD210, Draw2DBox 0x009DE3F0, Draw2DGraphic 0x009DCA80 (Ego_r / FableWin spellings; declarations only).

## engine/NGameText_CDataBank.h -- `FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING`
?ConstructString@NGameText@@YI_NAAVCWideString@@ABV2@@Z 0x009C7980 (Ego_r 0x00B156F0); declaration only.

## fable_user_profile.h -- `FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVENAME`
?GetAutoSaveDisplayName@CUserProfileManager@@QBE?AVCWideString@@XZ 0x00406C20 (Ego_r 0x00405860); declaration only.

## fable_string.h -- `FABLE_RETAIL_CCHARSTRING_ENDSWITH`
FableWin ?EndsWith@CCharString@@QBE_NABV1@@Z (0x0187F050); no copy in Ego_r.pdb (release) or bound in retail: FABLE_FORCEINLINE (the
StartsWith precedent), because the plain inline body costs 405 in c2's inline model (strcmp intrinsic) and stays a call where retail
expands both sites (work/inl/r3_v1.json). Body `long offset = GetLength() - suffix.GetLength(); if (offset >= 0) return strcmp(...)
== 0; return false;` -- the only spelling of three that gives retail's block layout (`js` to the false store last, `sete` in each
strcmp exit; TRACE B5). Next to the landed StartsWith opt-in (chunk 784) in the same class. Session 3: the row no longer includes
fable_string.h itself; it reaches the opt-in through the normal include chain (hdr/fable_user_profile.h and the engine headers
include "fable_string.h", which resolves to this overlay copy; after the merge into rebuild/include there is one copy), so no
include-order device remains in the row.

# Session 3 -- row 0x0056AD40 CTCBoastUI::Activate (all additive; default paths unchanged)

## engine/CTCBoastUI.h -- `FABLE_RETAIL_CTCBOASTUI` (the generated pack(1) struct is the #else path)
`class CTCBoastUI : public CTCBase` (RTTI; forces FABLE_RETAIL_CTCBASE), vtable 0x0124DCFC overrides (slots 0/4/11/21/22/26 per
rtti-vtables), Ego_r.pdb members at retail sizes: BoastGroups +0x0C map<NUISystem::CMenuEntry*, CQuestManager::CBoast>, Active +0x18,
Components +0x1C vector<NUISystem::CComponent*>, Quitting / AllowedToOpen / ActivationRequested / ActivateLater +0x28..+0x2B; sizeof
0x2C (static assert). Member functions in the Ego_r.pdb spelling (TU 0x005E1290..0x005E3700). Needs FABLE_RETAIL_CQUESTMANAGER.

## engine/CQuestManager.h -- `FABLE_RETAIL_CQUESTMANAGER_BOASTUI` (nested)
CActiveQuest head (ID +0, PScriptInfo +4, PScript +8; Ego_r.pdb) with FableWin's inline HasScript / GetScriptInfo; the layout branch
types RegisteredQuestNames +0x2C and ActiveQuests +0x38 (list<CCountedPointer<CActiveQuest> >) in place of the default branch's opaque
bytes (same offsets); FableWin inline GetActiveQuests; out-of-line GetBoastsForScript 0x004B19F0 and GetActiveQuestCardFromScriptName
0x004B0C80 (Ego_r + FableWin spellings). #error if combined with the REGIONLOCK / LOADGAMESTATE layouts (both model those bytes).

## engine/CScriptInfo.h -- `FABLE_RETAIL_CSCRIPTINFO_GETNAME`
FableWin ?GetName@CScriptInfo@@QBEABVCCharString@@XZ (0x0239E200), inline (Name +8 read in place by Activate).

## engine/CTCQuickAccessMenu.h -- `FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE`
FableWin inline StatGetInterfaceType (TCI_QUICK_ACCESS_MENU 246) and ?Deactivate@CTCQuickAccessMenu@@QAEXXZ 0x00625920 (Ego_r
0x0066A140, FableWin 0x02119DD0; the landed binding ?RequestClose... is a NAME_FIX). Includes engine/ETCInterfaceType.h under the opt-in.

## engine/CTCQuestCard.h -- `FABLE_RETAIL_CTCQUESTCARD_QUESTNAME` (nested in FABLE_RETAIL_CTCQUESTCARD)
FableWin inline StatGetInterfaceType (TCI_QUEST_CARD 108) and ?GetQuestName@CTCQuestCard@@QBE?AVCWideString@@XZ 0x00702310.

## engine/CTCCombat.h -- `FABLE_RETAIL_CTCCOMBAT_BLOCKING` (nested in FABLE_RETAIL_CTCCOMBAT)
?IsBlocking@CTCCombat@@QBE_NXZ 0x007331E0 and ?UnsetBlocking@CTCCombat@@QAEXXZ 0x007338A0 (Ego_r + FableWin); declarations only.

## engine/CTCFireballSpell.h -- NEW (`FABLE_RETAIL_CTCFIREBALLSPELL`; without it the header only forward-declares the class)
RTTI CTCFireballSpell : CTCBase; FableWin inline StatGetInterfaceType (TCI_FIREBALL_SPELL 157) and ?CancelMove@CTCFireballSpell@@QAEXXZ
0x007C8FD0 (Ego_r 0x007C7D90, FableWin 0x02721140). Data members not modelled (pointer use only).

## engine/CPlayerGui.h -- `FABLE_RETAIL_CPLAYERGUI_CLOSE`
?Close@CPlayerGui@@QAEX_N@Z 0x00438400 and ?ForceClearGui@CPlayerGui@@QAEXXZ 0x004382C0 (Ego_r 0x0044B0F0, FableWin 0x01A7CAC0).
0x004382C0 is landed as PrepareForLoad, which is 0x00438460: NAME_FIXES.tsv.

## engine/CSubtitleRenderer.h -- `FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE`
FableWin ?IsInCutscene@CSubtitleRenderer@@QBE_NXZ (0x0187F200), inline (byte +0xF6 of the CPlayerGui singleton).

## engine/NUISystem_CMenuEntry.h -- `FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS`
The first two virtuals CMenuEntry adds after CClickable's last slot 149: AddThingActionParam(long) slot 150 0x0053DCD0 and
AddUIActionParam(CActionParam*) slot 151 0x0053DD50 (Ego_r + FableWin spellings; Activate's `call [edx+25Ch]`).

## engine/NUISystem_CClickable.h -- `FABLE_RETAIL_CACTIONPARAMSTRING_CTOR`
??0CActionParamString@NUISystem@@QAE@AAVCWideString@@@Z declared out of line (retail 0x0054DAE0, landed); the in-class body is #else.

## fable_definition_table.h -- overlay mirror only (unchanged copy)
rebuild/include/fable_definition_table.h includes "fable_string.h" from its own directory, so in the overlay build it pulled the repo
copy next to hdr/fable_string.h (C2011 redefinitions via CTCCombat -> CCombatAbilityData -> CDefString). The unchanged copy makes the
include resolve to the overlay; drop it at the merge (there is one fable_string.h in rebuild/include).

# Session 4 -- row 0x00498490 (additive, default path unchanged)

## fable_string.h -- `FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN`
CCharString::GetAsCharArray() const in its early-return spelling (`if (PStringData==0) return &NullChar; return
PStringData->Data.PeekData();`), the same inline accessor and behaviour as the default ternary. Where the row's EndsWith expands it
(0x00499411 / 0x004994E4: `mov ecx,offset NullChar ; je ; mov ecx,[esi]`) retail forms the tail pointer as `lea esi,[ecx+eax]`
(text base, length difference index); the default ternary gives `[eax+ecx]`, and the chunk-848 SELECT spelling fixes only the
first site. Exclusive with FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT (SELECT wins if both are defined). Verified: with the
opt-in undefined the row's object is byte-identical to the one built against the unmodified header.

## Session 5 (2026-11-09)

No header changes. Row 0x00498490 matched with the opt-ins of sessions 3-4 (FABLE_RETAIL_CCHARSTRING_ENDSWITH, FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN);
row 0x0056AD40 dropped seven of its eight inline_depth regions through a loop shape; its opt-ins are unchanged.
