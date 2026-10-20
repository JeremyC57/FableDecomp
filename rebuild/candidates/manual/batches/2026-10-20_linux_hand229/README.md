# Hand chunk 229 (CStatBar .. CTCBuyableHouse): 24 of 40 byte-exact

## What matched (24)
- CStatBar: 00646b12 UpdateChangeTexts (moves and fades the floating change texts, erasing finished ones). Size TU (`optimize("y",off)` + `("s",on)`); the delta is a named local *assigned* from `end - start`; the test uses the value of `+=` and is written `!(t >= 1 - eps)`, which gives `test ah,1 ; je` (a plain `<` gives `test ah,5 ; jp`); the list iterator has the STLport user copy ctor, so erase's by-value argument is built with `mov eax,esp`.
- CSteeringAgent: 00a1c950 ~CSteeringAgent (unregisters the agent, deletes the behaviour, input and constraint objects, frees the lists). STLport 4-byte lists; delete+null loops, then the implicit list destructors; leaf `??1CSteeringAgent`.
- CStringParser:
  - 009b9aa0 SkipUntilWholeString: one long local for the position and then the offset; the hit pointer starts as `String + pos`.
  - 009ba1d0 ReadNextItemAsQuotedString: inline CalcInQuotes; the inline MoveStringPos reads the members directly.
- CSubtitleRenderer:
  - 0048dc8b WordWrapText: frame size TU; the width comes from named left/right float locals, with left loaded first.
  - 0048f6bd GetTextFromTextTag (a text entry's text, or the next member of a group entry; otherwise the tag itself). The text call is written in both arms, so their identical pushes hoist above the type test and the calls cross-jump. The group member is read through a named `const long* items` pointer, which reuses esi. The counted-pointer dtor is `__forceinline`, releases and nulls.
- CSubtitledMessage: 004d1360 is really STLport `_M_insert_overflow` for `pair<float,CCameraTag>`. Same-TU stand-in callee bodies home the tags.
- CSummonerToKill: 00d7dd80 Arena Main; the scripted handle is block-local.
- CSurface: 009f35d0 CanDoProperSignedConversion. Nested success path (`if (ok) {...; return true;} return false;`); the first calls go through inline helpers.
- CSymbolMap: 009e4f80 Error. `// Flags: /GS`; a varargs member (cdecl `this`); an explicit `CCharString("")` chain.
- CSystemManager: 009a5340 InitApplicationName. `// Flags: /GS`; memset intrinsic with the terminator declared before the memset (ebx scheduling); the copy loop becomes `rep movsw`.
- CSystemManagerInit: 00403b10 ctor. The retail-only 0x20 block is a record whose ctor body runs in store order.
- CTCAIScratchpad: 007ad880 OnCreate: a memberwise copy from GetSubDef<CAIScratchpadDef>.
- CTCActionUseBase: 007eacb0 IsThisTheft: nested success path; `__forceinline` HasTC; const GetInterface plus the ternary GetTC.
- CTCActionUseBuyHouse:
  - 0072ed10 OnActionUse(CThing&) (slot 28): out-parameter GetInterface plus a reference to the owner pointer; leaf `?OnActionUse@CTCActionUseBuyHouse`.
  - 0072ee30 GlobalUpdate: nested ifs; a named manager local; a filter temporary.
  - 0072e810 OnActionUseRent: implicit CCharString key temporaries (re-addressed with `lea`); an inline GetTextBank accessor; uses engine/CTCBuyableHouse.h.
- CTCActionUseWallMount: 007efe70 OnActionUse (slot 28): the action temporary is passed by address straight from its ctor.
- CTCAnimatingObject: 007f8e20 FrameUpdate (slot 9): STLport pop_front; a ternary-built CAnimFrameTime.
- CTCAssassinRush: 007a9b70 Reset: PDB layout minus 4 after the vector; the intelligent-pointer `op=` comes from a `(0)` temporary.
- CTCBattleCharge:
  - 007a6f00 Reset: sibling of 007a9b70 (retail = PDB - 4 after PeopleHit).
  - 007a7bf0 FinishMove: action temporaries by address; by-value intelligent pointers are built in the argument area.
- CTCBed: 007468c0 UpdateAnimation: the landed CTCBed sibling model; a named progress float; `1.0f - g_epsilon`.
- CTCBulletTime: 0079c750 PlayCreatureOnHitEffect: three named locals (the spell effects homed in the argument slot, the blurs in edi, the parent in ebp).

## Parked for Fable 5.1 (15)
- 007cdfe0 CTCBurntEffect::CTCBurntEffect DIFFER(284v284): `lea ecx,[esp+20h]` vs retail `lea ecx,[esp+34h]` for the __copy hidden result (dead index slot vs dead parent slot) | tried: GetInterface forms, named/temporary results, block scopes | next: lane gap: hidden-result slot choice
- 007cf110 CTCCreatureTintEffect::CTCCreatureTintEffect DIFFER(278v278): the same __copy hidden-result slot as 007cdfe0 | tried: as 007cdfe0 (more than 4 grades across the twins) | next: lane gap: hidden-result slot choice
- 00490927 CSubtitleRenderer::IsAllowedToSetText DIFFER(251v251): exit layout; retail puts `mov al,1 ; jmp` before the shared `xor al,al` | tried: flat early returns, speed TU, bool flags, do/while(0)+break | next: the nested success form (fixed 009f35d0 and 007eacb0)
- 0048f40a CSubtitleRenderer::DrawTextEmbeddedHighlights DIFFER(258v258): `lea edx,[ebp-18h]` / `lea ecx,[ebp-10h]` scheduled after, not between, the Y `fld`/`fadd` | tried: member sums, member and free operator+ | next: lane gap: scheduling
- 009f36e0 CSurface::CopyFromUnsignedDataToSignedSurface DIFFER(246v253): frame 0x58 vs 0x4c; the lock result's 16 bytes are not reused for the inner loop's locals | tried: strides at the top of the row loop, the lock result in its own block | next: lane gap: frame-slot sharing
- 006c0160 CTCBuyableHouse::GetCurrentCollectableRentValue DIFFER(271v271): the 0x21 lookup key gets its own frame word (0x10 vs 0x0c frame) | tried: out-parameter GetInterface, implicit name temporary, `__forceinline` contents accessor | next: same-TU stand-in GetDefGlobalIndexFromName body (the 004d1360 trick)
- 007aa370 CTCAssassinRush::CleanUpEffects DIFFER(218v268): retail keeps STLport erase's counted copy loop (`finish - finish`); ours folds it | tried: plain pointer erase (254v268), `__forceinline` copy/_Destroy, iterator forms | next: the real STLport `__copy` with random-access tag dispatch in the TU
- 00729f60 CTCActionUseScriptedHook::OnInitialActivate DIFFER(290v290): retail loads the thing into ecx before the `push 0 ; push 0` of each AddTC | tried: inline GetParentThing, named parent local, inline forwarding helper | next: lane gap: Setup*-family AddTC this-load
- 007e5570 CTCBetting::GetPlayersMoney DIFFER(292v262): frame packing; the def pointer is not homed in the hero lookup's key word (0x10 vs 0x0c frame) | tried: lookup inline, static/forceinline GetHeroMoney helper, named result | next: stand-in GetSubDef body (fixed the 007e5680 frame)
- 007e5680 CTCBetting::DrawPreparePrimitivesForRendering DIFFER(320v320): register rotation only (eax/ecx/edx) once the stand-in GetSubDef fixed the frame | tried: def via operator-> at each use, named def local | next: inline GetBaseDef accessor
- 006c06d0 CTCBuyableHouse::GetRentMoneyPos DIFFER(312v312): normalised Y as `fld st(1) ; fmul [Forward.Y]` vs retail `fld [Forward.Y] ; fmul st(2)` | tried: early return, nested form, `r * Y`, named result | next: lane gap: x87 single-term commutation
- 00871ec0 CStringAndCageBase::OnReset DIFFER(270v270): `sub esp,48h` vs retail `3Ch`; the named focal-point result does not share the dead raise temporary's words | tried: direct assignment, named focal, named thing, thing + named focal | next: the focal point in an inner block; lane gap: dead-temporary slot sharing
- 006bf9e0 CTCBuyableHouse::OnCreate DIFFER(301v301): `mov ax,[eax+8Ch] ; test ax,ax ; movzx edx,ax` vs retail `cmp word ptr [eax+8Ch],0 ; movzx edx,word ptr [eax+8Ch]` | tried: direct test, early return, a `const unsigned short&` accessor, an /Os probe | next: an index accessor returning by value for the push
- 006bfb90 CTCBuyableHouse::NeedsMainSignPost DIFFER(324v311): the village def copies re-test the copy (`setne bl`) and home the flag in a frame byte where retail uses `xor bl,bl` and an `inc`/`dec` copy through registers; plus `(x >> 11) & 0xFFFFFF01` | tried: flat early returns (328), nested success form | next: a counted pointer with CDefPointer-style copy/dtor and no null re-test; lane gap: `& 0xFFFFFF01` narrowing
- 00ab7b00 CStaticFontBank::GetStringExtents DIFFER(287v287): the FP stack holds the line width (`fstp st(0) ; fld 0`) where retail holds the widest height; ebx/ebp are shrink-wrapped (0x10 vs 0x14 frame) | tried: compound adds, `height = lineHeight + height`, `width = Offset + width + Advance`, declaration order | next: the return object built up front, or a for(;;) loop with a bottom test; lane gap: x87 variable allocation

## Skipped
- 007f7870 CTCActivationReceptorBase::FrameUpdate: the row holds FrameUpdate (with its jump table) plus a second function at 0x007F7940 (STLport remove_copy-like); two functions in one catalog row.

## Label corrections
- 0x007CF110 is the CTCCreatureTintEffect ctor (catalog: CTCBurntEffect).
- 0x004D1360 is STLport `vector<pair<float,CCameraTag>>::_M_insert_overflow` (catalog: CSubtitledMessage::ReserveAndInsertCameraTags).
- 0x00843610 is the CCreatureAction_UseWallMount ctor (vtable 0x0127498C); the landed 0x007EDDF0 calls it DeclarePlayerACriminal.
- 0x004016F0 is CDisplayManagerInit (manifest: CRegionDef).
- 0x0048DB13 is a CWorld mode predicate (`mode != 0 && mode != 9`) (manifest: CPersistContext::IsBinary).
- 0x006A2220 is a CTCCarrying member (GetCarriedThings) (manifest: RemapGroups).
- 0x007EAB90 is CTCActionUseBase::OnActionUse (manifest: $E2).
- 0x00686870 is CTCBase::OnInitialActivate.
- 0x007E5820 is GetSubDef<CBettingDef>; 0x007AD9C0 is GetSubDef<CAIScratchpadDef>.
- 0x007E50C0 is GetBetText (manifest: IsOfType).
- 0x00487150 is CPlayer's mode-0x12 exit (manifest: OnPredicateFail).
- 0x005CC091 is the Unsheathe action dtor; 0x0062E900 is ~CCombatActionBase (manifest: ~CTCLightningOrb).
- 0x007DF3D0 is a wall-mount effects member.
- The catalog's vfunc_28 rows (0x0072ED10, 0x007EFE70) are OnActionUse. vfunc_9 (0x007F8E20) is FrameUpdate. vfunc_4 (0x006BF9E0) is CTCBuyableHouse::OnCreate.
- Interface 0xD5 is CTCBuyableHouse (OwnedByPlayer +0x11); 0x23 is CTCVillageMember.
- 0x009C9070 is CDataBank::GetPTextEntry (manifest and the landed rows say GetPGroupEntry, which is 0x009C8890); 0x009C7A40, called on its result, is then the text entry's GetText (landed rows say CGroupEntry::GetText).
- 0x007EE540 is CTCVillageMember::GetPVillage (manifest: IS_THING_ALIVE<CIntelligentPointer<CThingPhysical>>).
