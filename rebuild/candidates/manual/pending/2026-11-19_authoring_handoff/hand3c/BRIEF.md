
## THIS SESSION (2026-11-19, hand2c) -- read this first; it OVERRIDES everything below where they differ
Owner: "add a third agent on the not plain c++ rows / functions and have it work them until every single one matches under our
rules of matching retail bytes, matching behavior, and being portable / clean."
- Your 98 rows were parked as "LTCG / CRT / asm-impossible". That label is a hypothesis, not a fact: MANY of these residues have since
  been solved. Re-check each against what exists TODAY before believing it.
- NO PARKING. Every row ends MATCH / RELOCATION_MATCH (or retail_asm / toolchain_library grade where docs/pipeline/ASM_RULES.md allows)
  + behaviour test PASS in the strict read-only vland check. There is no other end state.
- 100-try rule: count your grades per row in ATTEMPTS.log (one line per grade). A row that passes 100 grades of yours without matching
  goes to the main chat: SendMessage to "main", message starting `ESCALATE <addr>:` (residue retail v ours at exact addresses, best
  source path, LEVELS of ideas tried, best guess at the original shape) + /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/tmp/ESCALATE_<addr>.md. Then continue other rows.

### The rules for these rows (read docs/pipeline/ASM_RULES.md IN FULL first; it is the canonical rule and overrides anything here)
- Plain VC7.1 C++ first, always. Only where the ORIGINAL retail code used asm (retail bytes no VC7.1 C++ spelling emits, cited by
  address + instruction) may a row go past plain C++, in this order: an intrinsic; else an isolated macro-wrapped `__asm` in a SHARED
  HEADER with a `FABLE_PORTABLE` twin and a dual-path test; else (only if necessary) a FABLE_FORCEINLINE helper. Never for a compiler
  residue. No `__asm` / naked / _emit in a row source or test. `volatile` only where retail used it.
- REUSE the shared retail-asm helpers that already exist before writing any new one: rebuild/include/engine/GFFloatToLongCeil.h,
  GFFloatToLongFloor.h, GFFloatToLongNear.h, GFFastFloatToLong.h, GFFPUSetRoundingNearest.h, GFFPUSetDoublePrecision.h, GFPackFloat.h,
  NVectorMathSSE.h, CMatrix4x4AlignedSSE.h, CMatrix4x4TransformSSE.h, CParticleProgramSSE.h, CParticleSimulationSSE.h,
  CEngineLightingSSE.h, CEngineClothRendererSSE.h, GFGetInterpolatedColourMMX.h, C3DCoordIStrayMaxps.h, GFInitVectorMathCPUID.h ...
  (`grep -rln FABLE_PORTABLE rebuild/include`). Read how LANDED rows use them (`grep -rl GFFloatToLongCeil rebuild/src`) and the
  2026-10-27 retail-asm lane notes: rebuild/candidates/manual/pending/2026-10-27_linux_retail_asm_rasm2/NOTES.md (44 rows of these same
  families matched: ceil/floor block, bare fistp, libpk 2^x, SIMD kernels). A new helper is a new shared header with its portable twin,
  documented in hdr/README.md with the retail address + instruction that proves retail used asm there.
- "LTCG" residues (a callee taking arguments in registers no C++ declaration gives): first prove it is not an ordinary convention
  (__fastcall / __thiscall / a same-TU static VC7.1 optimises), a same-TU inline body, or a different callee identity. Read GOTCHAS
  and MATCHING_PLAYBOOK for these. If it truly needs /GL-style code generation, measure whether the TU compiles with /GL /LTCG in our
  lane and say exactly what you found; do not invent a convention.
- CRT residues: bytes the linker copied from the VC7.1 library are grade toolchain_library (docs/pipeline/LIBRARY_SIGNATURES.md,
  libsig/lib_verify.py) -- check before treating a CRT routine as code to author.
- Portable / clean (owner): the FABLE_PORTABLE build must compile with g++ and the test must pass on both paths where a helper is used.
  No steering declarations, no invented helpers, shared headers only, names right the first time.
- Ego_d.pdb + types are on disk (/tmp/claude-0/egod_pdb/); ref/<addr>/egod_locals.txt has each row's named locals. Ego_r / FableWin
  object tables: ghidra_out/egor_obj_functions.tsv, fablewin_obj_functions.tsv (VS2010 builds: names / TU membership carry over, code
  does not).
- Wine needs /run/user/0 (the env line creates it). Two other agents (hand2a, hand2b) share the Wine lane: one compile at a time of
  yours, never kill their processes, never wineserver -k.
- Tools: $S/tools/q.py, xdis.py, xref.py; oracle $S/batch/hand2c_oracle.tsv (lengths: retail size from the best DIFFER grade, else the
  extent scan -- correct a length with evidence and say so).
- Your context may end at any time: keep NOTES.md / STATUS.tsv / ATTEMPTS.log current after every row. Final message only when every
  row is matched or only escalated rows remain.

# hand2c -- egod lane author brief (rows reserved in rebuild/candidates/manual/pending/2026-11-18_hand1/RESERVED.tsv; the 2026-11-17 Ego_d lane's docs are in pending/2026-11-17_egod_parked/; this file OVERRIDES older briefs where they differ)

**Who you are:** an expert in reverse engineering and in decompiling retail x86 game code into clean, portable C++. You read the
retail disassembly and recover the C++ a Lionhead programmer wrote; you know how VC7.1 RTM (cl 13.10.3077) turns each spelling
into machine code; portability is a main goal (no inline asm except as docs/pipeline/ASM_RULES.md allows).

**Project:** FableDecomp, a byte-exact VC7.1 reconstruction of retail PC Fable.exe (Fable: The Lost Chapters). A row counts only when
its source compiles to the retail bytes (MATCH / RELOCATION_MATCH) AND a behaviour test passes. You author; you do not land.

## Hand authoring first (owner, 2026-11-18 -- the most important rule of this round)
Hand authoring is often better than automated tools. For every row and every stuck residue: read the retail disassembly around the
differing site and the LANDED siblings of the same TU (egod_tu.txt marks them), decide what statement the Lionhead programmer
actually wrote, and write that block by hand -- Ego_d's locals (egod_locals.txt: every record of the name; pick the one whose
parameters fit the row) in their scopes and declaration order, the source's natural expression shapes. Variant generators, flag
sweeps and spelling permutations only answer ONE specific question you have already formed; they are never how the source is
found. Last round's matches came from careful reading (a plain X, Y, Z dot product; a vector built member by member).

## Your rows: the 98 rows UNMATCHED_ROWS.tsv files as `not plain C++` (PARKED_INDEX residue class `ltcg/crt/asm-impossible`)
Order is free: take same-family rows together (one solved family closes many rows), closest first within a family.
1. 0x0041AFA0 CSprite::Draw (1455 bytes; extent scan 1455; best DIFFER(1041v1455); residue: DIFFER(1041v1455) -- lane gap: the four corner snaps are bare `fistp ; fild` (inline-asm ROUND); first draft of the visibility / cached-batch / rectangle / layer / quad flow; the inlined sprite-bank t)
   ref/0041afa0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
2. 0x00439BB0 CPlayerGui::UpdateMoney (592 bytes; extent scan 592; best DIFFER(588v592); residue: DIFFER(583v592) -- the bar's current value is rounded by an inline fistp helper (float copied to a frame word, fld, fistp; asm_bake family, lane gap); the rest is frame-slot assignment CPlayerGui::Upd)
   ref/00439bb0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
3. 0x0052EE00 CComponent::GetTargetRenderPosition (192 bytes; extent scan 192; best DIFFER(185v192); residue: DIFFER(185v192) -- structure/registers: retail keeps the target position in the frame (eax/ecx copy) and tests !parent and IsPositionAbsolute before re-testing it, ours holds the position in ebx/ebp a)
   ref/0052ee00/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
4. 0x0057C4F1 CTCHeroStats::ApplyBonusItemEffects (1192 bytes; extent scan 1192; best DIFFER(1183v1192); residue: DIFFER(1169v1192) -- first draft; this/def registers swapped (esi/edi), fabs compares without the fld st(0) copy; lane gap: the speed-duration round-to-int is retail's inline fistp helper (0.5 / 1.0 s)
   ref/0057c4f1/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
5. 0x00591B7F CTCInventory::UpdateContextSensitiveItems (735 bytes; extent scan 735; best DIFFER(729v735); residue: DIFFER(729v735) -- logic modelled (frame-8 gate, exhausted-candidate erase, item scan for context-sensitive subdefs and opinion-expression counts, global candidates, sort, fade-in per button slot, cle)
   ref/00591b7f/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
6. 0x00621A20 CTCInventoryQuests::InitialiseQuestCardList (600 bytes; extent scan 600; best DIFFER(620v600); residue: DIFFER(620v600) -- the inlined CVectorMap lower_bound loop keeps an extra eax copy (known lane gap: inlined lower_bound eax copy))
   ref/00621a20/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
7. 0x0063F4B0 CThingSearchTools::ForEachInRoughArea<CThingPhysical,CFilter_Bind2<CThingFilter_HasNavigationInformation,CThi (769 bytes; extent scan 769; best DIFFER(769v769); residue: DIFFER(769v769) -- with the 007FCEF0 indexing recipe applied (no change in length), the only residue is one swapped spill-reload pair at the map-loop exit (edi before esi; retail esi first); same lane)
   ref/0063f4b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
8. 0x00646762 NPlayerGui::CStatBar::DrawStatBarOnLeft (740 bytes; extent scan 740; best DIFFER(731v740); residue: DIFFER(711v740) -- lane gap: retail rounds the bar end with a hand-written inline-assembly fistp block (0.5/1.0 frame constants, +1 correction), which also forces the ebp frame; this brief C++ attempt)
   ref/00646762/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
9. 0x0064D404 CDrawQuestInfo::DrawMiniGameInfo (399 bytes; extent scan 399; best DIFFER(178v399); residue: DIFFER(129v399) -- parked at once: fistp/fild rounding (lane gap); only the element walk is reconstructed, the sprite tail is behind a stand-in helper)
   ref/0064d404/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
10. 0x0064DD9A NPlayerGui::CDrawMiniMap::UpdateRegionExitNames (580 bytes; extent scan 580; best DIFFER(521v580); residue: DIFFER(521v580) -- empty CAllocatorTag/filter temporaries homed at [ebp-1]/[ebp-2] where retail overlays them on [ebp+0Bh] (top byte of the LIVE region parameter slot; GOTCHAS lane gap), frame 0x44 v )
   ref/0064dd9a/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
11. 0x00664BA0 CThingCreatureBase::GetCurrentMeshRadius (184 bytes; extent scan 184; best DIFFER(174v184); residue: DIFFER(207v184) -- the failure-path null-tested release becomes the long `mov ecx,eax ; add eax,4 ; dec ; test ; mov [eax],..` form (retail `dec [ecx+4] ; jne`, the retry-8 lane gap), the key and out )
   ref/00664ba0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
12. 0x0066CD50 CRegionMinimap::BuildRegionMinimapTGA (423 bytes; extent scan 423; best DIFFER(430v423); residue: DIFFER(430v423) -- the light-to-index rounding is a bare dword fistp after a float store/reload (fstp/fld/fistp [esp+34h]) next to three __ftol2 conversions: an inline-asm rounding helper in the origi)
   ref/0066cd50/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
13. 0x00699E60 CGameCameraManager::SetScriptedCameraMode (431 bytes; extent scan 431; best DIFFER(450v431); residue: DIFFER(459v431) -- lane gap: retail builds the new-mode counted pointer in the dead by-value argument slots (no frame, Info cached in edi); ours opens an 8-byte frame and re-reads the address-taken lo)
   ref/00699e60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
14. 0x006B2960 CGameNavigatorManagerExternalInterface::?GetNavigationInformation@CGameNavigatorManagerExternalInterface@@UBEXABVC2DBoxF@@AAVCNavi (250 bytes; extent scan 250; best DIFFER(237v250); residue: DIFFER(231v250) -- the four box edges are rounded with bare dword `fistp` (inline-asm FastFloor/RoundToInt helpers in the original, the asm_bake family) and the body calls interface slot 13 once with )
   ref/006b2960/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
15. 0x006B75A0 CGameMap::Resize (434 bytes; extent scan 434; best DIFFER(414v434); residue: DIFFER(414v434) -- first draft: the min/max-by-reference bounds, loop counters homed in the dead offset argument slots and the destination index arithmetic (rows stored bottom-up, 7-byte cells) are no)
   ref/006b75a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
16. 0x006C06D0 CTCBuyableHouse::GetRentMoneyPos (312 bytes; extent scan 312; best DIFFER(312v312); residue: DIFFER(312v312) -- one x87 operand order: retail forms the normalised Y as `fld [Forward.Y] ; fmul st(2)`, ours as `fld st(1) ; fmul [Forward.Y]` (X and Z match); the single-term commutation lane gap )
   ref/006c06d0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
17. 0x006CC800 CTCCreatureNavigation::FrameUpdate (399 bytes; extent scan 399; best DIFFER(401v399); residue: DIFFER(358v399) -- retail rounds the scan half-size with an inline-asm fistp helper (0.5 / 1.0 in ebp-frame locals, bare fistp, ebp frame); pure C++ cannot emit it (asm_bake family) CTCCreatureNavigat)
   ref/006cc800/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
18. 0x006E5A00 CScriptConversation::FrameUpdate (1772 bytes; extent scan 1772; best DIFFER(191v1772); residue: DIFFER(120v1772) -- skeleton draft, parked on sight (lane gap): the ebp frame and the fld/fadd 0.5/fistp/ fild/fsub 1.0 round-down sequence (twice) come from an inline-asm rounding helper, which pure )
   ref/006e5a00/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
19. 0x00766660 CTCObjectAugmentations::AttemptToStartParticle (696 bytes; extent scan 696; best DIFFER(700v696); residue: DIFFER(700v696) -- 160 difflines: this/effect in edi/esi (retail esi/edi) and the end particle kept in ebx (retail spills it to the dead particleType slot and keeps the start emitter in ebx); `or byte)
   ref/00766660/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
20. 0x007727E0 CTCShop::GetCurrentBasicPrice (433 bytes; extent scan 433; best DIFFER(497v433); residue: DIFFER(497v433) -- minimal attempt; retail rounds the stock level with an inline-assembly fistp sequence (fadd 0.5 ; fistp ; fild ; fsub 1.0 ; compare ; dec) that compiled C++ never produces, so this )
   ref/007727e0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
21. 0x0077A430 CTCDExperienceOrb::FrameUpdate (1115 bytes; extent scan 1115; best DIFFER(1140v1115); residue: DIFFER(1067v1115) -- lane gap: the opening round-to-int is an inlined __asm helper (fistp/fild/fsub in ebp slots) that C++ cannot reproduce; the rest is a semantic first draft)
   ref/0077a430/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
22. 0x0077D5B0 CTCHeroCentre::FrameUpdate (1491 bytes; extent scan 1491; best DIFFER(5v1491); residue: DIFFER(-v1504) -- not drafted: lane gap /GS per-return epilogue (the early game-event return at 0x0077D683 checks the cookie before the register pops, as 007EC710; not reproduced under RTM or QFE-4035)
   ref/0077d5b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
23. 0x00780D80 CTCFlammable::FrameUpdate (539 bytes; extent scan 539; best DIFFER(539v539); residue: DIFFER(539v539) -- same size; the only difference is the slot of the sound-name CCharString conversion temporary: retail homes it in the sound def pointer's frame word (dead once the def is held in eb)
   ref/00780d80/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
24. 0x0078A690 CTCHeroRangedAttacks::FrameUpdate (1097 bytes; extent scan 1097; best DIFFER(1094v1097); residue: DIFFER(1094v1097) -- frame 0x1c v 0x14: retail shares one key word for every lookup and keeps the root-vector start in ebp across the tail (free(ebp)); byte-aligned bitset narrowing (mov cl,[+0x22] v )
   ref/0078a690/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
25. 0x0078D250 CTCBerserk::CTCBerserk::vfunc_9 (1729 bytes; extent scan 1729; best DIFFER(1592v1729); residue: DIFFER(1592v1729) -- lane gap: the tint level is a bare `fistp` (inline-asm ROUND); first full draft of the release/knock-back/ramp/countdown flow, with the knock-back sphere filter, the inverse-sqrt )
   ref/0078d250/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
26. 0x0079D360 CTCBulletTime::CTCBulletTime (1115 bytes; extent scan 1115; best DIFFER(1047v1115); residue: DIFFER(1037v1115) -- lane gap: the remaining-ticks rounding is a bare `fistp` (an inline-asm ROUND in the original, which is also why retail has an ebp frame); first draft otherwise follows the member)
   ref/0079d360/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
27. 0x007DF770 CTCWallMountEffects::CTCWallMountEffects::vfunc_9 (437 bytes; extent scan 437; best DIFFER(436v437); residue: DIFFER(436v437) -- lane gap (register-cached frame local): an argument is pushed before `mov ecx,<callee-saved>` for a pointer local that retail spills at its definition and reloads after the region; )
   ref/007df770/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
28. 0x007E2DF0 CTCWobble::OnInitialActivate (257 bytes; extent scan 257; best DIFFER(260v257); residue: DIFFER(260v257) -- lane gap: inline-asm rounding. Retail builds an ebp frame and runs the x87 fistp idiom (fld f ; fadd 0.5 ; fistp i ; fild i ; fsub 1.0 ; fstp t ; if (f == t) --i) with its 0.5 / 1.0)
   ref/007e2df0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
29. 0x007F8C20 CTCAnimatingObject::AddQueuedAnimation (221 bytes; extent scan 221; best -; residue: DIFFER(ours v retail) -- the CDefPointer release re-reads the escaped out slot after the dec (0x006631C0 lane gap); the list insert reads pos->_M_prev twice (needs a prev local); retail pushes ebx/esi)
   ref/007f8c20/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
30. 0x0082A020 CMiniMapDisplay::GetDrawPosition (343 bytes; extent scan 343; best DIFFER(332v343); residue: DIFFER -- lane gap: retail rounds both coordinates with a bare dword fistp through dead argument slots (the inline-asm rounding helper of GOTCHAS chunk 145); this draft's arithmetic is approximate and)
   ref/0082a020/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
31. 0x0083DB60 CThingPhysicalSwitch::GetCurrentGraphic (125 bytes; extent scan 125; best DIFFER(162v125); residue: DIFFER(162v125) -- the CDefPointer dtor re-reads the escaped local after `dec [eax+4]` (retail keeps the def in esi: `test esi ; dec [esi+4] ; jne`) and the by-value helper adds a copy; known lane gap)
   ref/0083db60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
32. 0x0087CFE0 CCreatureAction_InterruptableMidAttackAutoTurn::SetupTurningAnimation (1447 bytes; extent scan 1447; best DIFFER(3v1447); residue: DIFFER(3v1447) -- parked unreached (skeleton only): three local STLport vectors with inline push_back/grow (0x5E1420), sort/unique/erase (memmove 0xBFEAE6) and CRT free; def-pointer refcounts; not rea)
   ref/0087cfe0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
33. 0x008A8260 CGameScriptInterface::?GetHeroSummonedCreaturesList@CGameScriptInterface@@UBE_NAAV?$vector@VCScriptThing@@V?$all (466 bytes; extent scan 466; best DIFFER(473v466); residue: DIFFER(481v466) -- the found flag: retail homes it in a frame byte (stored there in the loop, reloaded into bl), ours keeps it in bl; plus edi/ebp pushes sunk into the guarded blocks (lane gap). Two t)
   ref/008a8260/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
34. 0x008D5AA0 CCreatureAction_FireMissileWeapon::?PerformAction@CCreatureAction_FireMissileWeapon@@UAEXXZ (1126 bytes; extent scan 1126; best DIFFER(1v1126); residue: DIFFER(1v1126) -- parked unreached (skeleton only): large multi-feature row: frame-counter gate, carried-thing check (0x46), SIMPLE_NOISE sound, multi-arrow shot-definition component (CTCShotDefinitio)
   ref/008d5aa0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
35. 0x00914650 CAIStateGroup_BirdStartled::?IsRunnable@CAIStateGroup_BirdStartled@@UAE_NXZ (340 bytes; extent scan 340; best DIFFER(334v340); residue: DIFFER(308v340) -- lane gap: inline-asm rounding helper. Retail is an ebp frame (`push ebp ; mov ebp,esp`) around an inline float->int round (fld 2.5 ; fadd 0.5 ; fistp ; fild ; fsub 1.0 ; compare ; d)
   ref/00914650/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
36. 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable (461 bytes; extent scan 461; best DIFFER(450v461); residue: DIFFER(483v461) -- lane gap: inline-asm fistp -- retail rounds the extended sight radius with a bare dword `fistp` + `fild`/`fsub 1.0` and an integer compare of the float bits (an inline-asm ceil help)
   ref/00986210/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
37. 0x00988E80 CShaderRenderManager::UpdateLocalisedSphereMapping (917 bytes; extent scan 917; best DIFFER(889v917); residue: DIFFER(839v917) -- lane gap: retail's SSE branch is a hand-written inline-assembly block (operands routed through frame slots, fixed xmm0..xmm7 schedule); the intrinsic spelling compiles to a differen)
   ref/00988e80/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
38. 0x00989230 CShaderRenderManager::UpdateObjectSpaceTextureTransform (636 bytes; extent scan 636; best DIFFER(636v636); residue: DIFFER(535v636) -- lane gap: retail's SSE branch is a hand-written inline-assembly block (operands routed through frame slots, fixed xmm0..xmm7 schedule); the intrinsic spelling compiles to a differen)
   ref/00989230/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
39. 0x009ACEE0 CDefinitionManager::ParseEnumForSymbols (1017 bytes; extent scan 1017; best DIFFER(1030v1017); residue: DIFFER(ours v retail) -- 1030v1017: retail caches separator.PStringData in edi across the out-of-line operator== call so the "=" test needs no reload/null retest (lane gap: register caching across cal)
   ref/009acee0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
40. 0x009BF7E0 CDisplayManager::CreateDevice (846 bytes; extent scan 846; best DIFFER(824v846); residue: DIFFER(824v846) -- call sequence and field stores follow retail, but retail keeps a 0x18 frame and builds its CPixelFormat temporaries in the dead params argument slot (known lane gap: locals in dead )
   ref/009bf7e0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
41. 0x009C38C0 CActiveFile::OnWriteFinished (409 bytes; extent scan 409; best DIFFER(414v409); residue: DIFFER(414v409) -- MakeBackupName 0x009C33A0 is an LTCG esi-result callee (lane gap); also GetFileTime/SetFileTime argument scheduling CActiveFile::OnWriteFinished() -- close the temp file, trim it to)
   ref/009c38c0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
42. 0x009C3C20 CFileInstaller::ProcessActiveFileForDecompression (909 bytes; extent scan 909; best DIFFER(877v909); residue: DIFFER(877v909) -- first-pass reconstruction: retail keeps the 'finished' flag and the read size in the dead argument slot (known lane gap), rotates the decompress loop with the finished test at the t)
   ref/009c3c20/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
43. 0x00A38C20 CCRCSymbolMap::LoadBinary (551 bytes; extent scan 551; best DIFFER(564v551); residue: DIFFER(564v551) -- calls and block structure match; retail caches the constants 0 (esi) and 4 (ebp) in registers across the whole body (Read process argument, stream-source store, inline-Read size/com)
   ref/00a38c20/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
44. 0x00A475C0 CNavigatorBase::FindNearestNavigableNode (1857 bytes; extent scan 1857; best -; residue: DIFFER(-v1857) -- triage only, no source drafted: `and esp,-8` aligned frame (8-byte local in the search loop) plus 222 x87 ops; lane gap per GOTCHAS chunk 232 when the 8-byte struct address is taken )
   ref/00a475c0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
45. 0x00A48BA0 CLipSyncSampleData::WriteToFile (875 bytes; extent scan 875; best DIFFER(873v875); residue: DIFFER(873v875) -- known lane gap: the weight byte uses an inline-asm fistp (fld/fmul 255/fstp/fld/fistp); also the lane's <map> is not retail's STLport map (header malloc 0x18 at 0x00BFEA0E, inline _)
   ref/00a48ba0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
46. 0x00A48F10 CLipSyncSampleData::GetFrameInterpolate (811 bytes; extent scan 811; best DIFFER(815v811); residue: DIFFER(845v811) -- known lane gap: retail inlines an inline-asm fistp floor helper (fld/fsub 0.5/fistp/fild/fadd 1.0 with frame constants, ebp frame); the rest of the blend is modelled CLipSyncSampleD)
   ref/00a48f10/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
47. 0x00A4C1F0 C3DAnimationSequenceData::GetKeyframeInterpolate (1006 bytes; extent scan 1006; best DIFFER(922v1006); residue: DIFFER(922v1006) -- lane gap: inline-asm fistp rounding helper (fsub 0.5 ; fstp ; fld ; fistp) inside the table acos/sin of the inlined slerp; first draft, (int) cast calls _ftol)
   ref/00a4c1f0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
48. 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative (420 bytes; extent scan 420; best DIFFER(89v420); residue: DIFFER(89v420) -- retail's slerp-weight block survives only because its float->int conversions are inline-asm fistp (fstp/fld through a float slot, then fistp); every result is dead, so compiler-emitt)
   ref/00a4d770/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
49. 0x00A52760 C3DAnimationSequenceData::GetKeyframeInterpolate (1004 bytes; extent scan 1004; best DIFFER(922v1004); residue: DIFFER(922v1004) -- lane gap: inline-asm fistp rounding helper in the inlined slerp; same draft as 00a4c1f0)
   ref/00a52760/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
50. 0x00A53860 C3DAnimation2::CalcLocalBoneTransformation (1005 bytes; extent scan 1005; best DIFFER(922v1005); residue: DIFFER(922v1005) -- lane gap: inline-asm fistp rounding helper in the inlined slerp; first draft)
   ref/00a53860/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
51. 0x00A567A0 CCardinalSpline::GetSplinePosition (766 bytes; extent scan 766; best DIFFER(790v766); residue: DIFFER(790v766) -- retail rounds the segment position with an inline-asm fistp helper (ebp frame, fistp/fild at [ebp-8], the x == i+1 bump), which C++ cannot emit (lane gap: inline-asm fistp rounding))
   ref/00a567a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
52. 0x00A5B650 CCachedFile::Copy (184 bytes; extent scan 184; best DIFFER(179v184); residue: DIFFER(179v184) -- erase/copy-trivial now matches in shape but ebx is pushed in the prologue (retail scopes push ebx to the memmove arm) and the 0x009E9900 range append uses an LTCG custom convention )
   ref/00a5b650/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
53. 0x00A62AE0 CPackedUIntArray::CalculateBias (130 bytes; extent scan 130; best DIFFER(132v130); residue: DIFFER(132v130) -- lane gap (dead-argument-slot homing): retail has no frame and keeps the loop counter in the dead `bits` slot and the mask in the dead `count` slot with `data` loaded into ebx at ent)
   ref/00a62ae0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
54. 0x00A770B0 CNavQuadTree::GetNodeAt (533 bytes; extent scan 533; best DIFFER(408v533); residue: DIFFER(408v533) -- lane gap (inline-asm fistp rounding helper): retail rounds each cell coordinate with an inlined `fistp (f - 0.5)` plus a float-bit `== i + 1` bump, which compiler-emitted C++ can on)
   ref/00a770b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
55. 0x00A781A0 CNavQuadTree::UpdateLines (1244 bytes; extent scan 1244; best -; residue: DIFFER(lane gap: inline-asm fistp) -- 4 bare dword `fistp` sites (float store/reload, then fistp/fild); VC7.1 C++ emits __ftol2 or (/QIfist) a qword fistp, so the body is asm_bake family per GOTCHAS; )
   ref/00a781a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
56. 0x00A793D0 CMap::ConnectHorizantalMapEdge (678 bytes; extent scan 678; best DIFFER(593v678); residue: DIFFER(593v678) -- push ebp/mov ebp,esp/and esp,-8 aligned frame (retail: plain sub esp,0x44) forced by the 8-byte C2DVector GetNodeAt argument temporaries whose address is taken inside the nested lay)
   ref/00a793d0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
57. 0x00A79680 CNavQuadTree::ConnectVerticalMapEdge (678 bytes; extent scan 678; best DIFFER(593v678); residue: DIFFER(593v678) -- vertical twin of 0x00A793D0 (byte-identical shape, y walk); same aligned and esp,-8 frame from the C2DVector GetNodeAt temporaries inside the nested loops (GOTCHAS chunk 232 lane ga)
   ref/00a79680/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
58. 0x00A9D480 CMatrix4x4::AlignedPreScale (243 bytes; extent scan 243; best DIFFER(243v243); residue: DIFFER(ours v retail) -- asm_bake family: the SSE path is inline assembly (this copied to a frame word and reloaded into edx, movaps/shufps/mulps on an and-esp-16 frame); only the x87 fallback is sour)
   ref/00a9d480/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
59. 0x00A9D750 C3DAnimationManager::ApplyBoneMorphScaling (551 bytes; extent scan 551; best DIFFER(551v551); residue: DIFFER(492v551) -- lane gap (original inline-assembly SSE block): retail stores the row pointer to a frame local and re-reads it into edx, loads the scale from the frame into a fixed xmm3 and rows int)
   ref/00a9d750/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
60. 0x00AABF60 CSoundEmitterLH::CSoundEmitterLH::vfunc_3 (620 bytes; extent scan 620; best DIFFER(625v620); residue: DIFFER(625v620) -- retail is link-time-code-generated: the second mask loop calls FindFirstSetBit 0x00A3F860 once, hoisted out of the loop with a register (ecx/edx) convention and its 1<<bit hoisted t)
   ref/00aabf60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
61. 0x00AAD340 CWaveFile::ConvertSamplingRate (584 bytes; extent scan 584; best DIFFER(606v584); residue: DIFFER(613v584) -- lane gap: retail inlines an inline-asm fistp floor helper (fld; fsub 0.5; fistp; fild+1 compare) three times in an ebp frame; plain C++ calls _ftol instead CWaveFile::ConvertSamplin)
   ref/00aad340/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
62. 0x00AC7EB0 QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction (215 bytes; extent scan 215; best DIFFER(215v215); residue: DIFFER(215v215) -- a `s = Z*o.Z; s += Y*o.Y; s += X*o.X` Dot called as v1.Normal.Dot(v2.Normal) fixes the z/y term order; left: the x term loads the first vertex (retail the second) and n*n*w is scale)
   ref/00ac7eb0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
63. 0x00ADF960 CParticleSimulation::SatisfyDistanceConstraints (766 bytes; extent scan 766; best DIFFER(764v766); residue: DIFFER(466v766) -- lane gap: inline __asm SSE (the g_bParticleUseSSE branch is a hand-written movaps/shufps/sqrtps/maxss block with push esi/pop esi on a 16-byte aligned ebx/ebp frame); the source cov)
   ref/00adf960/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
64. 0x00AE1320 CParticleProgram::ClingToMesh (681 bytes; extent scan 681; best DIFFER(681v681); residue: DIFFER(376v681) -- lane gap: inline __asm SSE (the 0x013D2880 branch multiplies each bone matrix by the inverse rest matrix in a hand-written movaps/shufps/mulps/addps block reading pointers the C cod)
   ref/00ae1320/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
65. 0x00AE29D0 CParticleProgram::SetupSimulation (726 bytes; extent scan 726; best DIFFER(726v726); residue: DIFFER(422v726) -- lane gap: inline __asm SSE (the 0x013D2880 branch of the bone-matrix product is a hand-written movaps/shufps/mulps/addps block reading pointers the C code stored to frame slots, on )
   ref/00ae29d0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
66. 0x00B012E0 NParticleEngine::CPSCRenderMesh::CPSCRenderMesh (1296 bytes; extent scan 1296; best -; residue: DIFFER(lane gap: inline-asm fistp) -- 22 bare dword `fistp` sites (float store/reload, then fistp/fild); VC7.1 C++ emits __ftol2 or (/QIfist) a qword fistp, so the body is asm_bake family per GOTCHAS;)
   ref/00b012e0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
67. 0x00B05C60 NParticleEngine::CPSCSingleSprite::GetComponentCost (429 bytes; extent scan 429; best DIFFER(277v429); residue: DIFFER(277v429) -- the two size terms round through the inline-asm `fld ; fistp` helper (store/reload, bare dword fistp next to __ftol2 calls): the documented asm_bake lane gap (GOTCHAS chunks 150/164)
   ref/00b05c60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
68. 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere (977 bytes; extent scan 977; best -; residue: DIFFER(-v977) -- not authored: retail rounds with 1 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks )
   ref/00b075d0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
69. 0x00B079B0 CPSCSingleSprite::DatabaseSerialise (2032 bytes; extent scan 2032; best DIFFER(2034v2032); residue: DIFFER(-v2032) -- triage only, no source drafted: contains the same `xor cl,[mem]` bitfield write-back that keeps the parked twin 0x00B1A870 at DIFFER (same length, one instruction); lane gap until th)
   ref/00b079b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
70. 0x00B103F0 NParticleEngine::CPSCEmitterGeneric::CPSCEmitterGeneric (1463 bytes; extent scan 1463; best -; residue: DIFFER(lane gap: inline-asm fistp) -- 28 bare dword `fistp` sites (float store/reload, then fistp/fild); VC7.1 C++ emits __ftol2 or (/QIfist) a qword fistp, so the body is asm_bake family per GOTCHAS;)
   ref/00b103f0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
71. 0x00B16410 COrbitState::Update (1671 bytes; extent scan 1671; best DIFFER(1674v1671); residue: DIFFER(lane gap: inline-asm fistp) -- 8 bare dword `fistp` sites (float store/reload, then fistp/fild); VC7.1 C++ emits __ftol2 or (/QIfist) a qword fistp, so the body is asm_bake family per GOTCHAS; )
   ref/00b16410/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
72. 0x00B1D550 CPSCDecalRenderer::DecalCollides (576 bytes; extent scan 576; best -; residue: DIFFER(-v576) -- not authored: retail rounds with 2 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks )
   ref/00b1d550/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
73. 0x00B1D790 CPSCDecalRenderer::PoolDecal (876 bytes; extent scan 876; best -; residue: DIFFER(-v876) -- not authored: retail rounds with 2 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks )
   ref/00b1d790/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
74. 0x00B1DC70 CPSCDecalRenderer::AddToBoundingSphere (860 bytes; extent scan 860; best -; residue: DIFFER(-v860) -- not authored: retail rounds with 4 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks )
   ref/00b1dc70/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
75. 0x00B1DFD0 NParticleEngine::CPSCDecalRenderer::CPSCDecalRenderer (931 bytes; extent scan 931; best -; residue: DIFFER(-v931) -- not authored: retail rounds with 14 bare dword `fistp` (float store/reload, no _ftol), the original hand-written assembly float-to-int helper -- known lane gap (GOTCHAS wave 25 chunks)
   ref/00b1dfd0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
76. 0x00B328C0 CEnginePrimitiveRenderer::?LoadNewScene@CEnginePrimitiveRenderer@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z (517 bytes; extent scan 517; best DIFFER(529v517); residue: DIFFER(501v517) -- retail rounds with an inline-asm ceil helper (0.5f/1.0f stored in ebp locals, fsub [local], fistp into the dead params slot, fild/fadd/compare/inc) which forces the ebp frame; C++ u)
   ref/00b328c0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
77. 0x00B41A80 CEngineMap::PeekWaterHeight (424 bytes; extent scan 424; best DIFFER(329v424); residue: DIFFER(329v424) -- lane gap: retail quantises the game-map height with an inline-asm rounding helper (`fstp [arg] ; fld [arg] ; fistp dword ; fild`, GOTCHAS wave 25 fistp family); this body (one struc)
   ref/00b41a80/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
78. 0x00B51AD0 CEngineWeatherRenderer::BuildBoxPositionMatrix (969 bytes; extent scan 969; best DIFFER(961v969); residue: DIFFER(951v969) -- retail rounds each axis with an inline-asm fistp helper (ebp frame, 0.5/1.0 constants stored to frame slots, fild/fadd/fstp, integer compare): asm_bake family lane gap, parked at on)
   ref/00b51ad0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
79. 0x00B53F60 CEngineWeatherRenderer::CEngineWeatherRenderer::FrameStart (411 bytes; extent scan 411; best DIFFER(418v411); residue: DIFFER(418v411) -- retail keeps `this` in ecx across the call to UpdateBoxPositions (0x00B52990, same TU, inline-asm body that never touches ecx: VC7.1 uses the known callee register set). Lane gap: t)
   ref/00b53f60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
80. 0x00B905A0 CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest (280 bytes; extent scan 280; best DIFFER(272v280); residue: DIFFER(272v280) -- lane gap: retail pushes ebx/esi/edi only after the early-out tests (the failure tail `*handle = -1 ; ret 0Ch` has no pops); the lane pushes ebx in the prologue, so every stack offse)
   ref/00b905a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
81. 0x00B93B30 CEngineSceneMap::AddPrimitive (395 bytes; extent scan 395; best DIFFER(282v395); residue: DIFFER(282v395) -- lane gap: asm_bake family: retail floors both cell coordinates with the inline-asm fistp/fild helper (ebp frame, 0.5/1.0 frame constants); compiled code cannot produce it CEngineSce)
   ref/00b93b30/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
82. 0x00B947E0 CEngineSceneMap::Update (246 bytes; extent scan 246; best DIFFER(251v246); residue: DIFFER(236v246) -- the float->long rounding is a bare `fstp ; fld ; fistp` (inline-asm FtoL helper in the original, asm_bake family); a C cast gives `call __ftol2` and the frame/register knock-on (esi)
   ref/00b947e0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
83. 0x00B94BF0 CEngineScene::GetMapAt (311 bytes; extent scan 311; best DIFFER(295v311); residue: DIFFER(295v311) -- asm_bake family: both cell coordinates are rounded with a bare `fld ; fsub 0.5 ; fistp` + `fild ; fadd 1.0` floor correction (an inline-asm float-to-long helper in the original, as )
   ref/00b94bf0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
84. 0x00B99300 CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW (871 bytes; extent scan 871; best DIFFER(871v871); residue: DIFFER(855v871) -- register/schedule only: retail loads the bool and count args at entry (dl/ebp), keeps the two SetVSConstants tails apart (ours cross-jumps them, -16 bytes) and orders the position/n)
   ref/00b99300/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
85. 0x00BA1140 CEnginePrimitiveManagerStaticMeshesBase::CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive (2344 bytes; extent scan 2344; best DIFFER(2372v2344); residue: DIFFER(2372v2344) -- inline-asm FtoL x2 (`fstp ; fld ; fistp dword`) unreproducible (__ftol2 stand-in), LOD loop rotated by the lane, frame 0x294 v 0x298 + slot/callee-saved register assignment drift)
   ref/00ba1140/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
86. 0x00BADE40 CEnginePrimitiveManagerSprites::RenderSprite (793 bytes; extent scan 793; best DIFFER(743v793); residue: DIFFER(726v793) -- lane gap: retail converts the faded alpha with an inline-asm FloatToInt (fstp [esp+0x10]; fld [esp+0x10]; fistp [esp+0x14]; mov ebp,[esp+0x14]) that byte-pure C++ cannot emit (same )
   ref/00bade40/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
87. 0x00BD71B0 CEnginePrimitiveManagerAnimatedMeshesBase::CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive (2116 bytes; extent scan 2116; best DIFFER(2121v2116); residue: DIFFER(2121v2116) -- inline-asm FtoL (`fstp ; fld ; fistp dword [edx]`) unreproducible (__ftol2 stand-in), radius `fld [eax+34h]` hoisted first in retail, frame 0x290 v 0x294 + slot/callee-saved regis)
   ref/00bd71b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
88. 0x00BE2120 CEngineInternalPrimitive2DViewport::CEngineInternalPrimitive2DViewport::ClearChildPrimitive (97 bytes; extent scan 106; best DIFFER(97v97); residue: DIFFER(107v106) -- retail forms the slot address first (`push esi ; lea esi,[edx+eax*8] ; mov eax,[esi+4]`) and runs both null tests on eax; the lane folds the first load (`mov ecx,[edx+eax*8+4]`) bef)
   ref/00be2120/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
89. 0x00BEE070 _global::__pop_heap<pair<long,CCompositeSurfaceInfo> > (206 bytes; extent scan 206; best DIFFER(206v206); residue: DIFFER(ours v retail) -- 206v206: a pure callee-saved register swap (retail esi = result, edi = first; ours the reverse) plus the eax/ecx/edx rotation that follows from it; same instructions otherwise)
   ref/00bee070/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
90. 0x00C1C330 _global::AIMatrixPointHelper_00c1c330 (192 bytes; extent scan 192; best -; residue: DIFFER(ours v retail) -- lane gap: LTCG custom register convention (matrix A in eax, point in ecx, matrix B in edx, result pointer in esi, plain `ret`); a standard-convention body cannot reproduce the)
   ref/00c1c330/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
91. 0x00C40590 _global::AudioLib_NormalizeOrientationFrame_00c40590 (192 bytes; extent scan 192; best DIFFER(192v192); residue: DIFFER(192v192) (retry 9, differ/) -- the dot product now matches as the chain `s = Z*v.Z; s += Y*v.Y; s += X*v.X` inside the inline Dot; left: four of the six inline cross-product multiplies load the)
   ref/00c40590/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
92. 0x00C41FB0 CCombatant::CCombatant::ComputeDerivedCombatStats (1570 bytes; extent scan 1570; best DIFFER(1531v1570); residue: DIFFER(1469v1570) -- 81 blocks: first pass; retail rounds the four levels with a bare dword fistp through dead argument slots (inline-asm rounding helper, asm_bake family per GOTCHAS), ours calls __ft)
   ref/00c41fb0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
93. 0x00C6AC90 CSoundMixer::MixBlockRamped (38 bytes; extent scan 38; best DIFFER(39v38); residue: DIFFER(39v38) -- lane gap (LTCG custom calling convention): retail loads the gain into ecx (`mov ecx,[esp+10h]`) for a callee that also takes four caller-cleaned stack arguments (`add esp,10h`): 0x00C)
   ref/00c6ac90/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
94. 0x00D6C830 CQ_HangingTreeEvilScript::CheckBanditFollowStatus (607 bytes; extent scan 607; best DIFFER(607v607); residue: DIFFER(607v607) -- tail-merge lane gap: retail shares one terminating-return block between the inner and outer waits (branch targets/scheduling only, same instructions))
   ref/00d6c830/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
95. 0x00D8A240 CQ_HerosOldHouseScript::WatchForBooty (610 bytes; extent scan 610; best DIFFER(610v610); residue: DIFFER(610v610) -- tail-merge lane gap: retail shares the terminating-return block between the Oak Bay waits (branch targets/scheduling only; advisor b2 top-tested form))
   ref/00d8a240/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
96. 0x00F36010 CPerlinNoise::GetGaussian2DNoise (442 bytes; extent scan 442; best DIFFER(412v442); residue: DIFFER(412v442) -- retail floors both coordinates with the inline-asm `fld ; fsub 0.5 ; fistp` helper (frame-pointer function, 0.5/1.0 stored to the frame and the argument slot before each fistp): the)
   ref/00f36010/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
97. 0x00F361D0 NFractal::CPerlinNoise::Get3DNoise (873 bytes; extent scan 873; best DIFFER(744v873); residue: DIFFER(740v873) -- lane gap: inline __asm floor (retail stores each coordinate and the 0.5f/1.0f constants to ebp-frame slots and floors with fld/fsub/fistp/fild/fadd/cmp/inc inside an __asm block, fo)
   ref/00f361d0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
98. 0x00F36540 NFractal::CPerlinNoise::GetGaussian2DNoise (321 bytes; extent scan 321; best DIFFER(319v321); residue: DIFFER(319v321) -- lane gap: retail floors each coordinate with the inline-asm FloatToInt helper (`fld ; fsub 0.5 ; fistp` into the frame, frame-pointer function, the 0.5/1.0 constants stored as local)
   ref/00f36540/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt


## What is new for these rows: Ego_d.pdb (read rebuild/candidates/manual/pending/2026-11-17_egod_parked/README.md first)
Every row here is a PARKED row: earlier agents got close and stopped. Ego_d.pdb (a VC10 /Od debug build of the same TLC source
tree) adds source facts none of them had:
- `ref/<addr>/egod_locals.txt`: the function's own parameter and local NAMES, their TYPES and their SCOPE BLOCKS (`{ ... }` =
  an `if` / loop / plain block where locals were declared). Use them as source evidence: declare the same named locals in the same
  scopes with the same types (`float` vs `double`, reference vs value, `CCharString` copy vs `const CCharString&`), name every local
  as the source did, keep the source's statement structure. Named locals are the known lever for register / frame / x87 residues.
  Frame offsets in it are VC10 debug, NOT retail; unnamed temporaries are not recorded.
- `ref/<addr>/egod_tu.txt`: every function of the same object file (TU) in source order, with retail addresses where the catalog
  knows the name. Same-TU callees retail could see (templates, inline members, file statics) may need their real bodies defined in
  the row (this closed 0x006D0830 after eleven failed passes: a same-TU template COMDAT that was only declared).
- Tools: `python3 -I rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/egod_locals.py /tmp/claude-0/egod_pdb/Ego_d.pdb '<Class::Name>'`
  (any function, e.g. callees and siblings) and `python3 -I .../lane/egod_type.py /tmp/claude-0/egod_pdb/types.txt <hexindex>...`
  (resolve a type index). Full type records: `grep -n -A12 '^ *0x<INDEX> |' /tmp/claude-0/egod_pdb/types.txt` (150 MB: grep only).
  Class layouts / members / static members / nested typedefs of the TLC source: grep that same types.txt for "LF_CLASS ... `<Name>`".
- STL in Ego_d is VC10 Dinkumware (`_Tree`, `_List_val`, `_Vector_val`): never copy STL internals from it; retail is STLport, spell
  STL from rebuild/include/stlport/. Game classes, members, enums, statics and function names carry over.
- IDENTIFY rows (made-up current names): find the function in `egod_tu.txt` of its likely object by its order next to the named
  neighbours and by its parameters / locals vs the retail body, then CONFIRM the name from a SECOND source (Ego_r / FableWin name
  tables `ghidra_out/egor_pdb_names.tsv` / `fablewin_pdb_names.tsv`, retail strings, FSE, callers, vtable slots). Put the proven
  name (exact decorated spelling) in NAME_FIXES.tsv and LABELS.tsv; if no second source exists say so in NOTES.md and keep the
  name descriptive. Then match the row like any other.

## Environment (every Bash call starts with these two lines; shell state does not persist)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; mkdir -p /run/user/0; export W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c FABLE_HDR_OVERLAY=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr; cd /home/user/FableDecomp
- Write ONLY inside `/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c` (src/, test/, differ/, hdr/, tmp/, probe/). The repo is READ-ONLY for you. NO git commands at all.
- Never kill a process you did not start (another agent shares this Wine lane): kill a hung cl.exe of yours by explicit PID only;
  never pkill / killall / wineserver -k. Wrap long commands in `timeout`. One compile at a time.
- Preflight a new or re-ported source before grading: `g++ -E -D_MSC_VER=1310 -I /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr -I rebuild/include <src> > /dev/null`
  (cl under Wine spins until the timeout on a missing include or an #error instead of printing it).
- Tools ($S/tools): `python3 $S/tools/q.py <addr8> <src.cpp> [leaf] [--flags ...]` grades one source against retail (relocations
  masked; side-by-side disassembly of the differing hunks; the oracle is $S/batch/hand2c_oracle.tsv); `xdis.py <hexva> <hexlen>`
  disassemble retail; `xref.py <hexva>` callers / references; `rstr.py 0x<va>` string; `iat.py 0x<slot>` import; `fun.py 0x<va>` EH info.
  Log every grade as one line in /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/ATTEMPTS.log (addr, idea, grade). There is no fg.py / vt.py / addmatch.py / fwsize.py here.
- Behaviour test + relocation check of a matched row (read-only; the lander's real check): add the row to /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/payload.json
  (`{"result": {"authored": [{"address": "<addr8>", "module": "<Module>", "name": "<Name>", "source_cpp": <src text>, "test_cpp": <test text>,
  "pass_pattern": "<addr8>_TEST PASS"}]}}`, plus `"compiler_flags": [...]` when the row has a Flags line), copy its line from
  $S/batch/hand2c_oracle.tsv into /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/oracle.tsv (true length in the length column), then
  `LANDVERIFY_WORK=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/lv LANDVERIFY_STRICT_HEADERS=1 timeout 3000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/payload.json /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/oracle.tsv > /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/tmp/vland.log 2>&1`
  (NEVER with --land / --recheck-landed). A row is matched only with `RELOCATION_MATCH PASS` (or MATCH PASS) there; fix every RELOC_IDENTITY; delete /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/lv after.
- Tool paths under /opt/fable_lane/... named in older notes do NOT exist on this machine. c2 measuring tools that are in the repo:
  rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools/ (copy what you need into your directory).
- PDB-derived evidence in the repo: ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, egod_pdb_names.tsv (name SETS keyed by
  donor addresses, never by retail address), pdb_class_keys.tsv, struct_layouts_egor.tsv, naming_stragglers/FableWin.exe.signatures.tsv,
  rebuild/oracles/reloc_bindings.tsv (retail address -> bound symbol), rebuild/include/engine/<Class>.h. A third, weaker source
  (names of locals / original file; PowerPC build): `llvm-pdbutil dump ... $S/anniv_dbg/WellingtonGame-Xbox360-Test.4k.pdb` (large:
  always pipe through grep; rule in docs/re_reference/FABLE_ANNIVERSARY.md).

## Read first (large files in parts; grep rather than read whole where you can)
0. rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/LESSONS_EGOD.md (what closed this lane's rows so far; short)
1. /home/user/FableDecomp/CLAUDE.md
2. docs/pipeline/ASM_RULES.md
3. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md (later sections override earlier ones)
4. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/BRIEF.md -- the sections "Rapid fire" and "Stay on task" are your working
   style: every tool call ends in a grade, short loop, one row at a time until it matches, no side quests.
5. rebuild/candidates/manual/pending/2026-11-13_acctB_FINAL/lane/CHUNK_BRIEF_lessons.md -- the lessons of ~60 earlier chunks on rows
   exactly like yours (what closed each residue class). Read ALL of it once; it is the most valuable file here.
6. docs/pipeline/MATCHING_PLAYBOOK.md (measured c2 rules; before calling anything a "compiler tie")
7. rebuild/candidates/manual/pending/2026-11-13_acctC/README.md (the rules that keep the hand-back landable) and
   rebuild/candidates/manual/pending/2026-11-13_acctC/lane/WF_RULES.md (owner decisions and tips; its LATER sections override earlier ones)
8. grep docs/pipeline/GOTCHAS.md for your classes and callee addresses; read the nearest LANDED sibling rows of your class
   (`grep -rl "<Class>" rebuild/src/compiled | head`) and copy their idioms.

## Rules (each is re-verified; a row that breaks one is rejected)
- MATCH or RELOCATION_MATCH under VC7.1 RTM for the exact file in src/<addr>.cpp, graded against TODAY's rebuild/include plus your
  hdr/ overlay, AND test/<addr>.cpp printing exactly `<addr>_TEST PASS` in the vland check. The test must exercise the function's real
  logic (a mutant with one decision removed should fail).
- Plain portable C++: no `__asm`, `naked`, `_emit`, the word `register`, bare `__forceinline` (write FABLE_FORCEINLINE), and
  `volatile` only per ASM_RULES. No dead padding, no steering declarations a Lionhead programmer would not have written; every
  statement whose result is unused cites the retail bytes that require it. Inline-budget devices only per EXTRA_RULES 2026-11-08.
- Shared headers only: model every class with `engine/<Class>.h`; never a local padded struct or a second header for a class that
  has one. A header change is an ADDITION under a NEW `#ifdef FABLE_RETAIL_<CLASS>_<WHAT>` opt-in made on a copy of the CURRENT repo
  header placed in your hdr/ (same relative path). Never reuse an existing opt-in name for different content, never edit the body
  of an existing opt-in, never change a default path, never move a FABLE_STATIC_ASSERT line (insert additions so existing asserts
  keep their line numbers where possible, and say so in hdr/README.md when one moved). hdr/README.md: one line per opt-in (header,
  name, what, evidence). After ANY overlay header change regrade every row you already matched.
- Names right the first time: the row, every callee, global, class and member in the exact PDB spelling (class key, namespace,
  const, access, calling convention), two sources each. One retail address, one name: grep rebuild/oracles/reloc_bindings.tsv for
  every callee / global address before declaring it. A made-up name only when the original is found nowhere; then descriptive,
  consistent, listed as descriptive in NOTES.md. Wide text is `unsigned short`, never wchar_t.
- Line 1 of a source: `// <Module>::<Name>(<params>) -- <what it does>`; a `// Leaf:` line with the exact object symbol (check with
  `objdump -t`); exactly one `// Flags:` line if flags are needed; `// Retail:` address and length.
- No retail bytes leave the lane: nothing you write may embed retail byte dumps (disassembly excerpts in notes are fine).

## Working style
Rapid fire (BRIEF.md): port the draft onto today's headers, grade, read the hunks, one hypothesis, edit, grade. Grades are not
rationed; ten grades with the same residue mean change the LEVEL of the idea (callee body visibility, helper texts, the
real structure re-derived from the disassembly, identities, TU flags), not an eleventh spelling. Do not rebuild compiler-hook
tooling. The moment a row matches: write its test, run the read-only vland check, next row.

## Hand-over (all inside /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c)
src/<addr>.cpp + test/<addr>.cpp (matched); differ/<addr>.cpp (best source of an unmatched row, line 2 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`);
hdr/ + hdr/README.md; payload.json + oracle.tsv (matched rows only); STATUS.tsv (`address grade bytes test grades_used notes`);
NOTES.md (per matched row: what made it match, headers / flags; per open row: the exact residue with retail addresses and every
idea tried); LESSONS.md (one line per new lesson); NAME_FIXES.tsv (`address correct_symbol evidence wrong_spellings_seen`);
LABELS.tsv (`address name evidence`). Keep NOTES.md and STATUS.tsv current after every matched row (your context may end at any
time and the files are the only channel). Final message: rows matched / in progress / untouched, grades used, header changes,
names proven vs descriptive, and anything the coordinator must decide.

## Owner decisions that apply (details in acctC lane/WF_RULES.md)
- Dead release asserts (shared fable_assert.h `FABLE_RETAIL_ASSERT` only) are accepted at any count IF the row source carries an
  `// Inferred:` note: count, the residue they close (addresses), evidence. Asserts inside an inline helper that retail plausibly
  asserted in are accepted too (owner 2026-11-16), still only with byte match + test + note.
- Hand-authoring often beats automatic tools: read the retail disassembly and the matched siblings and write the block the way a
  programmer would; sweeps (k locals, assert counts, flag ranges) are for measuring one question, not for finding the source.
- Unused locals (device c, k <= 7) need a `// Fidelity:` line; inline controls (/d2inlT, scoped inline_depth / auto_inline) only for a
  pure inline-decision residue with an `// Inline-budget:` line and the smallest N. No `__assume`, no home-made macros, no invented
  helper functions, no local class models where a shared header exists.

## Owner's requirements for this lane (2026-11-18; they override every older budget or parking rule)
"I want every function it works on to reach a match. No exceptions." Every row in your list must end MATCH or RELOCATION_MATCH, with a
passing behaviour test, in plain portable C++ that behaves exactly like retail. There is NO grade or time budget and NO parking: a row
is finished only when the read-only vland check (strict headers) says RELOCATION_MATCH PASS or MATCH PASS.
- The three goals hold together: retail's exact bytes, retail's exact behaviour, and portability (plain C++; no __asm / naked /
  _emit / register; volatile only per ASM_RULES; no tricks a Lionhead programmer would not have written; header changes only as new
  opt-ins with the default path unchanged). A match that breaks behaviour or portability is not a match.
- Hand authoring first, always: read the retail disassembly around each differing site and the landed same-TU siblings, decide
  what the programmer wrote, write it by hand. Sweeps only answer a question you have already formed.
- Work order: closest row first. When a row stops moving, change the LEVEL of the idea (source structure -- duplicated arms,
  hoisted stores, scopes, inline helper bodies, same-TU callee bodies, the real class / header model, identities), not the spelling.
- Fable advisor: when you have tried several levels on a row and believe you need another view, send the row to the main chat for
  the Fable advisor: use SendMessage with to "main" and a message that starts `ESCALATE <addr>:` and gives the residue (retail v
  ours at the exact addresses), your best source path, what you tried (levels, not spellings) and your best guess at the original
  shape. Also write the same into tmp/ESCALATE_<addr>.md. Then keep working on your OTHER rows; the coordinator runs the review and
  sends its candidates back to you as a message. Grade them, then continue the row until it matches. If you have no SendMessage
  tool, write the file, keep working on the other rows, and when only escalated rows are left end your turn with the list of
  ESCALATE files: you will be resumed with the review.
- Keep NOTES.md / STATUS.tsv / ATTEMPTS.log current after every grade batch (your context may end at any time).
- Your final message comes only when every row is matched (or when you stop to wait for escalations); list per row the grade,
  what closed it, header opt-ins, flags, and names proven.
