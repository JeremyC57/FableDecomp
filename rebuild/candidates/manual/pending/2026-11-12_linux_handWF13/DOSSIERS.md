## 0x00492973

# DOSSIER 0x00492973 -- CSubtitleRenderer::SetText (full 4-caption wide overload), 568 bytes

## 1. Retail shape (xdis 00492973 0x248)
- thiscall, `ret 28h` = 10 dword args (font, text, name, b1, b2, b3, flags, time, bool, bool).
- `push ebp; mov ebp,esp; sub esp,1Ch; push esi` ... `leave; ret 28h` => `#pragma optimize("y", off)` + `#pragma optimize("s", on)`
  (same as every landed CSubtitleRenderer row: 0x00492E4B, 0x00492BAB, 0x0048F6BD, 0x0048F939, 0x0048DDF6). No SEH, no /GS cookie
  (Clear 0x00492BAB and Pause 0x0048DD94 carry `// Flags: /GS`, this body has no cookie read -> no /GS flag needed).
- Frame 0x1C = the CSetTextInfo local only ([ebp-1Ch], sizeof 0x1C: 4 strings + flags + time + 2 bools). Everything else is homed
  in DEAD ARGUMENT SLOTS: CCharString temps at [ebp+0Ch]; hero pointer local at [ebp+0Ch] (passed by ref to IS_THING_ALIVE);
  GetTC key 0xCB at [ebp+0Ch]; found list at [ebp+0Ch] with its default allocator temp at [ebp+0Fh]; filter (0x11) at [ebp+14h].
- Early branch: `if (DisplayingText) { CSetTextInfo info(text,b1,b2,b3,flags,time,bool1,bool2); TextInfoQueue.push(info); }`
  then `jmp` to the common epilogue (pop esi only; ebx/edi pushed only on the main path) -> source is if/else or early return.
- ebx = &Text (+0x5C) is cached and reused for the Preload vcall; edi = 0x0122D70E ("" char literal) reused for both CCharString temps.
- Tail loop is UNROTATED: `mov edi,[ebp+0Ch] (sentinel); mov esi,[edi]; jmp test; body: push [esi+8]; mov ecx,ebx; call RemoveMessage;
  mov esi,[esi]; test: cmp esi,edi; jne body`. Manager in ebx (loaded BEFORE the list ctor), sentinel in edi.

## 2. Identity (label OK modulo doubled class)
- Mangled leaf: `?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z`
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0049BBF8) + egor_signatures.tsv; the landed caller 0x00492E4B forwards its 10 args
  here (`call 0x00492973`), and landed Clear 0x00492BAB calls it with (`*PFont, info.Text, L"", info.Button1Text, ...`).
  Manifest label `CSubtitleRenderer::CSubtitleRenderer::SetText` has a doubled class prefix; real name CSubtitleRenderer::SetText.
- Signature: `void SetText(const CFontBank&, const CWideString&, const CWideString&, const CWideString&, const CWideString&,
  const CWideString&, unsigned long, float, bool, bool)` -- public, non-const, returns void.
- Callers (xref.py): 0x00492E27(.. ->0x00492E4B landed), 0x00492EA6/0x00492ED1, 0x00492F36/0x00492F69, 0x00492FA2/0x00492FB4,
  0x00493017/0x00493049, 0x004930A5/0x004930D7 (Update), 0x005EAE56, 0x006DD853, 0x00713392, 0x00892AF2.
- Parameter -> member mapping from bytes (IMPORTANT, not the obvious one):
  [ebp+10h] (arg3) -> NameText +0x60 ; [ebp+14h] (arg4) -> Button2Text +0x68 ; [ebp+18h] (arg5) -> Button1Text +0x64 ;
  [ebp+1Ch] (arg6) -> Button3Text +0x6C. Arg3 ("name") is NOT passed to CSetTextInfo (ctor gets text, arg4, arg5, arg6).
  Clear passes L"" as arg3 and info.Button1Text/2/3 as arg4/5/6. Header opt-in SETTEXT names params button0_text/button1_text/...
  (no PDB param names found). Draft already maps arg4->Button2Text, arg5->Button1Text: keep it.

## 3. Callees (landed file / landed name / PDB name; FLAG = draft spelling wrong)
- 0x0048DDF6 CSetTextInfo ctor: 00/48/CSubtitleRendererCSetTextInfo_CSetTextInfo_0048ddf6.cpp; PDB
  `??0CSetTextInfo@CSubtitleRenderer@@QAE@ABVCWideString@@000KM_N1@Z`. FLAG: draft models CSetTextInfo as a top-level struct; it is
  the NESTED class CSubtitleRenderer::CSetTextInfo (Clear 0x00492BAB models it nested).
- 0x004941C0: 00/49/stddequeCSubtitleRendererCSetTextInfo_pushback_004941c0.cpp = std::deque<CSetTextInfo>::push_back (out of line);
  source spelling is the inline `queue::push(x)` -> `c.push_back(x)`. FLAG: draft's `CSetTextInfoQueue::push` stand-in; use
  std::queue<CSetTextInfo, std::deque<...> > as Clear does (TextInfoQueue opaque [0x28] in header -> reinterpret_cast like Clear).
  fable_deque.h offers FABLE_STLPORT_DEQUE_PUSH_BACK (+ _AUX_NOINLINE).
- 0x0043E3D0 ~CSetTextInfo: 00/43/CSubtitleRendererCSetTextInfo_CSetTextInfo_0043e3d0.cpp, `??1CSetTextInfo@CSubtitleRenderer@@QAE@XZ`.
- 0x0099B800 CWideString::operator=(const wchar_t*) (00/99/CWideString_operatorwchartconst_0099b800.cpp); Clear declares it
  `const CWideString& operator=(const wchar_t*)` -- FLAG draft returns `CWideString&` and takes `const unsigned short*` with a cast.
  Literal L"" = 0x0122D70C.
- 0x0048F6BD `?GetTextFromTextTag@CSubtitleRenderer@@AAE_NABVCWideString@@AAV2@@Z` (private, returns bool) -- landed 00/48/..._0048f6bd.cpp.
- 0x00492381 `?ProcessEmbeddedGraphicTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x00491AD2 `?ProcessEmbeddedHighlightTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x0099EBF0 CCharString(const char*, long = -1) / 0x0099EAE0 ~CCharString (landed 00/99). Literal "" = 0x0122D70E. Args are temps:
  `SetButton1OverrideText("")`.
- 0x0048F7C0 / 0x0048F881 `?SetButton{1,2}OverrideText@CSubtitleRenderer@@QAEXABVCCharString@@@Z` (landed; header opt-in
  FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE declares Button2 only).
- 0x00449970 `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` returns CPlayer& -- FLAG draft returns CPlayer*.
- 0x00487DC0 `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` (const) -- FLAG draft `GetHeroThing`/CThingCreatureBase.
- 0x0043DBD0 `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z` (__fastcall, ptr by const ref) --
  FLAG draft instantiates IS_THING_ALIVE<CThing*>; must be <CThingPlayerCreature*>.
- 0x00410DE0 std::bitset<274>::test (CThing::UsingComponents +0x20) -- FLAG draft `CBitset Present`.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) via the inline
  CThing::GetTC(ETCInterfaceType) -- FLAG draft `CInterfaceMap`/`GetTC<T>(long)`; copy Clear's CThing::GetTC model verbatim.
- 0x0048DD94 `?Pause@CSubtitleRenderer@@QAEX_N@Z` (header opt-in FABLE_RETAIL_CSUBTITLERENDERER_PAUSE).
- 0x0048F939 `?InitPlayerMode@CSubtitleRenderer@@AAEXXZ` (private) -- FLAG draft `ProcessPlayerModes`.
- 0x00493BC2 std::list<const CMessageEvent*>::list(const allocator&) out of line (landed as a placeholder
  global_ListNodeCreateCircularSentinel_00493bc2.cpp, `ret 4`, arg = &allocator temp [ebp+0Fh]).
- 0x00493EAB `??$FindAllMessages@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBE?BJABVCMessageEventFilter_IsOfType@@AAV?$list@PBVCMessageEvent@@...@Z`
  -- CONST member, returns `const long`, output is std::list<const CMessageEvent*>&. FLAG draft non-const, returns long, custom list.
- 0x00493B5B `?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z` (opt-in FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE, decl only).
- 0x00493BDD list<...>::~list out of line (std_list_T_00493bdd_dtor_00493bdd.cpp; placeholder name). fable_containers.h has
  FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE.
- vcall [PFont vtbl+0x10] = slot 4 `CFontBank::Preload(const CWideString&, bool) const` (engine/CFontBank.h line 41, 0x009A0B20) --
  FLAG draft `LayoutText`. Called as `PFont->Preload(Text, true)` (push 1; push &Text).
- Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent via inline static Get() (naming wave 18; draft's g_pMainGameComponent
  is the old stand-in) ; +0x1C PPlayerManager (scoped_ptr<CPlayerManager>), +0x24 PWorld; CWorld +0x60 PMessageEventManager_Data
  (engine/CWorld.h, inline GetMessageEventManager() under FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER).
- Message type 0x11 = MESSAGE_EVENT_TEXT_FINISHED_2 (Clear's enum) -- filter CMessageEventFilter_IsOfType(type).

## 4. Members touched (engine/CSubtitleRenderer.h, retail offsets, struct_layouts_egor.tsv names)
+0x24 TextCharactersShowing long (`and [esi+24h],0`) ; +0x28 TextGroupHasEntriesRemaining bool ; +0x2C TextGroupIndex long (`or -1`);
+0x30 TextGroupTag_PStringData (CWideString) ; +0x34 TextInfoQueue opaque [0x28] (queue<CSetTextInfo,deque>) ; +0x5C Text_PStringData;
+0x60 NameText ; +0x64 Button1Text ; +0x68 Button2Text ; +0x6C Button3Text (all `X_PStringData`, reinterpret_cast<CWideString&>
as Clear does) ; +0x78/+0x79 Button1Alpha/Button2Alpha =0xFF ; +0x7C DisplayFlags ; +0x80 PFont (CFontBank*) ; +0x88 MaxDisplayTime ;
+0x8C TextTimer (fldz) ; +0xDD AllCharactersDisplayed ; +0xE5 DisplayingText ; +0xE6 UpdateWhilePaused ; +0xE7 UpdateTimer.
Header opt-ins available: FABLE_RETAIL_CSUBTITLERENDERER (IsDisplayingText inline), _PAUSE, _BUTTONOVERRIDE, _CLEAR, _SETTEXT,
_SETTEXT_CHAR (declares this 10-arg overload), _INTERACTIVE, _INCUTSCENE, _GRAPHICTAG, _CTOR, _DTOR. Clear uses the
`#define CSubtitleRenderer CSubtitleRendererLayout` + derived-class trick to add the nested CSetTextInfo + member decls.
Other headers: engine/CWorld.h, engine/CPlayer.h, engine/CPlayerManager.h, engine/CThing.h (UsingComponents +0x20, Components +0x44),
engine/CMessageEventManager.h, engine/CFontBank.h, engine/CTCHeroReceiveItems.h (no members past PickedUpItems modelled).
- Hero component byte: `mov byte ptr [GetTC(0xCB)+0x26],1` = CTCHeroReceiveItems::DisplayingItems (Ego_r +0x32; retail is Ego_r-0xC,
  same shift as Clear's ForceClear Ego_r +0x33 -> retail +0x27). FLAG draft `CHeroControls::Frozen`. Interface 0xCB =
  TCI_HERO_RECEIVE_ITEMS (Clear).
- Classes the draft models LOCALLY that have shared headers: CSubtitleRenderer, CThing, CPlayer, CPlayerManager, CWorld,
  CMainGameComponent, CMessageEventManager, CFontBank, CVectorMap (draft's CInterfaceMap), CTCHeroReceiveItems (draft CHeroControls).

## 5. Callee visibility
- Same TU (CSubtitleRenderer.cpp, defined EARLIER): Pause 0x0048DD94, CSetTextInfo ctor 0x0048DDF6, GetTextFromTextTag 0x0048F6BD,
  SetButton1/2OverrideText 0x0048F7C0/0x0048F881, InitPlayerMode 0x0048F939, ProcessEmbeddedHighlightTags 0x00491AD2,
  ProcessEmbeddedGraphicTags 0x00492381. All called out of line in retail.
- COMDATs emitted after the TU (0x00493xxx/0x00494xxx, unaligned odd addresses = template/inline instantiations of this TU):
  list ctor 0x00493BC2, list dtor 0x00493BDD, RemoveMessage 0x00493B5B (out of line here! inline in 0x0089AF30's TU),
  FindAllMessages 0x00493EAB, deque::push_back 0x004941C0. Sibling 0x0089AF30 note: "the sentinel stays in edi across
  FindAllMessages only when that template's body (and push_back's) is compiled earlier in the TU" -- they gave those bodies
  under `#pragma auto_inline(off)` before the row. Consider the same for FindAllMessages / list ctor / dtor / RemoveMessage here.
- Opt-ins: FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (fable_containers.h);
  FABLE_STLPORT_DEQUE_PUSH_BACK (fable_deque.h); FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE; FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER.

## 6. Sibling landed rows (copy idioms)
- 00/49/CSubtitleRenderer_Clear_00492BAB.cpp -- next function in retail; THE template: Layout-rename include trick, nested CSetTextInfo,
  std::queue/deque stand-ins, CThing::GetTC FABLE_FORCEINLINE over CVectorMap::LowerBound, CPlayer::GetPControlledCreature,
  IS_THING_ALIVE(hero), CMainGameComponent::Get(), `// Flags: /GS` (this row needs none), pragmas y-off + s-on.
- 00/49/CSubtitleRenderer_SetText_00492E4B.cpp, _00492F69.cpp -- forwarding overloads; y-off + s-on.
- 00/48/CSubtitleRenderer_Pause_0048DD94.cpp -- includes engine/CSubtitleRenderer.h directly.
- 00/89/CGameScriptInterface_RemoveAllCutSceneSkippedMessages_0089AF30.cpp -- the FindAllMessages(IsOfType)+remove idiom, with
  list<const CMessageEvent*> model and auto_inline(off) callee bodies.

## 7. Draft model errors vs disassembly
- Body control flow is right (DIFFER 572v568, tail only). Residue: retail homes the found list in the dead `text` slot [ebp+0Ch]
  (allocator temp at [ebp+0Fh]) and the filter in [ebp+14h]; the draft gets list +8/filter +0xC; and retail's removal loop is
  unrotated (jmp-to-test) with sentinel in edi, manager in ebx.
- Draft list/ctor: retail calls an out-of-line list ctor taking `const allocator&` (default-arg temp) -- draft's CAllocatorTag stand-in
  should be the real STLport `std::list<const CMessageEvent*>` with `explicit list(const _Alloc& a = _Alloc())`.
- Loop: retail iterates begin()..end() via iterator (`it = head->_M_next` loaded via `mov esi,[edi]`), calling RemoveMessage(*it);
  write `for (std::list<const CMessageEvent*>::iterator it = found.begin(); it != found.end(); ++it)` with a real STLport iterator
  (or a while loop) -- draft's raw-node for loop rotates.
- FindAllMessages is const and returns `const long`; manager pointer is read as `Get()->PWorld->PMessageEventManager_Data` (ebx)
  before the list is constructed -- keep a named manager local declared before the list.
- Names wrong in draft: GetHeroThing->GetPControlledCreature, ProcessPlayerModes->InitPlayerMode, LayoutText->Preload,
  CHeroControls::Frozen->CTCHeroReceiveItems::DisplayingItems, IS_THING_ALIVE<CThing*> -> <CThingPlayerCreature*>,
  CBitset Present -> std::bitset<274> UsingComponents, CInterfaceMap -> CVectorMap Components, g_pMainGameComponent ->
  CMainGameComponent::Get(), top-level CSetTextInfo -> CSubtitleRenderer::CSetTextInfo, local CSubtitleRenderer -> shared header.

