# Hand chunk 837 (CQ_BowerstoneTownLifeIntroScript .. CV_StatueMasterScript): 2 of 6 byte-exact

## What matched (2)
- 0x00D23DC0 NScript::CQ_BowerstoneTownLifeIntroScript::Main (RELOCATION_MATCH, 2 grades): two entity bindings, posts, the
  BowerstoneIntroRegionExit close / reopen around the guard speech, arena and posh-guard waits (each guard removed when alive, then a
  fade-in), and the final unload wait before DeactivateQuestLater("Q_BowerstoneTownLifeIntro", 0). Shape: a function-scope
  `CScriptThing Guard;` re-assigned twice through `__forceinline CScriptThing::operator=` over the declared-only
  CCountedPointer<CScriptThing>::operator= 0x00704580 (needs `class CScriptThing;` before the spec); plain-inline IsAlive gives retail's
  2-expanded / 2-called split by itself. New header engine/CQ_BowerstoneTownLifeIntroScript.h.
- 0x00ED3B30 NScript::CV_StatueMasterScript::Main (RELOCATION_MATCH, first grade): three bindings, posts, then the four centre-to-marker
  XY angles into AnglesToFaceList. Shape: five named CScriptThing locals (a named local devirtualises GetPos; a temporary does not), the
  landed SickChild GFArcTan / GET_ROUNDED_ANGLE C3DVector::GetAngleXY spelling with `#pragma intrinsic(atan2,fmod)` (fmod helper is the
  MSVCR71 _CIfmod thunk 0x00BFEB28). New header engine/CV_StatueMasterScript.h. The FSE Lua port of StatueMaster agrees on markers.

## Parked for Fable 5.1 (4)
- 0x00DDAE40 NScript::CQ_PrisonRaceScript::Main DIFFER(1180v1180): `fstp dword ptr [esp+0x18]` right after `call [edx+0x28]` (retail
  after the next `mov ecx,[esp+0x20]; cmp ecx,ebp`) | tried: 17 grades, GetAngleXY / GetPos inline spellings (if / ternary / reversed /
  IsNull / named float / __forceinline either or both), named angle / pos / hero locals, block-scope StartPos | next: x87 scheduling
  residue; try compiling the TU-mate WatchForHeroWinOrLose body above Main.
- 0x00D3A4A0 NScript::CQ_FireHeartScript::CFH_Prisoner::Main DIFFER(1140v1140): seh_Prophet[MyCageNumber] address `mov ecx,[esi+0x20];
  shl ecx,4; lea edi,[edx+ecx]` (ours `mov edi; shl edi; add edi,edx`) and eax/ecx swap for ParentClass at StartScriptingEntity |
  tried: 11 grades (reference / pointer / begin()+n / &[0]+n / cage and parent locals, block v function scope) | next: lane gap:
  register colouring.
- 0x00EF4B40 NScript::CPersonalScript_EmptyScript::CTestPerson::Main DIFFER(1252v1268): seh_me's STB ctor is a call to 0x007E72A0 where
  retail expands it (CBaseObject ctor + vtable 0x0127094C + zero +8/+C); seh_hero's is a call in both | tried: 9 grades (implicit /
  row-local plain-inline / out-of-line STB ctor, implicit movie pair, widescreen ctor, folded if, /d2inl diagnostics) | next: lane gap:
  per-site implicit-ctor inline split (chunk 823 kind); the out-of-line form gets frame and registers exact.
- 0x00EFA4F0 NScript::Cbandit_level_cutsceneScript::Main DIFFER(1365v1362): the implicit movie dtor is expanded at the post-SPLINE_03
  terminate exit (retail calls 0x006E7B80 there; the first three exits expand, the rest call, in both) | tried: 15 grades (outer loop as
  while / do-while / `for (;; NSF) { if (T) return; ... }`, PImp spec on/off, implicit / forced movie and widescreen ctors, /d2inlS and
  /d2inlT diagnostics: no flag reproduces it either) | next: lane gap: per-site MovieBase dtor inline split; the for-NSF outer loop
  form moved it from 7 expanded sites to 4.

## Skipped
- none.

## Names
- Proven (two sources each): the six Main symbols (vtable slots + Ego_r); CQ_PrisonRaceScript::WatchForHeroWinOrLose 0x00DDB2F0
  (thread literal + Ego_r/FableWin); CQ_FireHeartScript::UpdateHistory 0x00D3A920 / CheckCompletion 0x00D3AD60 (PDB + literal /
  landed labels); CFH_Prisoner::GetEndCutscene 0x00D3AB00 / GetEndSpeech 0x00D3ABE0 / MakeRoundStartComment 0x00D3A9B0 (PDB + call
  shape); NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748 (label + landed binding).
- Weaker: CFH_Prisoner::MakeComment 0x00D3ACE0 (PDB member set by elimination + behaviour).
- Descriptive only: the test-person locals (seh_hero, hero_stand_pos, test_pos) and the bandit `movie` local (not in any PDB).

## Label corrections
- 0x00D3A4A0 `Quest_Tournament_Manager` (labels_forge_autoname_t5) is a placeholder: NScript::CQ_FireHeartScript::CFH_Prisoner::Main.
- 0x00D3ACE0 catalog `CFishCompOrganiser::SpeakConversationLine` -> CFH_Prisoner::MakeComment.
- 0x00D3AB00 / 0x00D3ABE0 / 0x00D3A9B0 under-qualified catalog names -> NScript::CQ_FireHeartScript::CFH_Prisoner::*.
- reloc_bindings: 0x0099F100 bound as three Append adapters (should be ??YCCharString@@QAEABV0@PBD@Z); 0x01375748 bound as
  g_PlayAnimationDefaultFlag / g_bScriptAnimationFlag (should be NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations).
  All in NAME_FIXES.tsv / LABELS.tsv.

## Header debt
- engine/CQ_PrisonRaceScript.h: `void MakeRacerComment();` should be `bool MakeRacerComment(const CScriptThing&, const CCharString&);`.

## Diagnostic flags
- No /d2 flag is kept anywhere. /d2inlS10-20 and /d2inlT40-90 were used only as meters on 0x00EFA4F0 and 0x00EF4B40; neither row
  matches under any of them, so neither is a caller-size-budget park.
