# Hand chunk 2010 (CSubtitleRenderer .. NThingManager::CEntityRuntimePersistence): 4 of 4 byte-exact

All four rows grade RELOCATION_MATCH under VC7.1 RTM (gall.sh with `FABLE_HDR_OVERLAY=hdr`), pass the read-only vland
(4 WINS, behaviour PASS) and their tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Every row needs the hdr/
overlay (hdr/README.md: additions-only opt-in blocks; merge into rebuild/include before landing). All four starts / ends were checked:
0x00492381 follows the `ret` of 0x00491AD2 at 0x00492380 (unpadded /Os TU) and ends `ret` at 0x00492972; 0x004959E0 ends `ret` at
0x00495FD1; 0x005248B0 follows int3 padding and ends `ret 4` at 0x00524EB4; 0x00716DE0 follows SetUpTextAndSpeech's int3 padding and ends
`ret` at 0x007173C9. No length was wrong (no SPLITS.tsv).

## What matched (4)
- **NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript (0x005248B0, 8 grades)** -- reload the game-persistent entity
  scripts per START_THING_LEVEL block (map UID -> map number through CWorldMap::GetMapNumberFromMapUID when not a map index) and the
  START_NEW_DEAD_THING records. Shape: the same-TU AddPersistedDeadEntity / SetGamePersistentScript bodies defined above the row (VC7.1
  expands both, and resize(n) -> resize(n, CEntityScriptInfo()) with them); the CStringParser ctor inline (chunk 2001 opt-in). The lever was
  the **visible CPersistContext::Transfer<T> body** (new opt-in, the body of the landed 0x0044BB00 / 0x004106F0, whose instances in this TU
  grade RELOCATION_MATCH against both): VC7.1 then knows the out-of-line Transfer only reads its default, hoists the defaults' zero
  stores into the dead-thing loop's preheader and keeps 0 in edi (declaration-only: `this` in edi and the stores in the loop).
- **CFrontendGameComponent::GetProfileNames (0x004959E0, 2 grades, /GS)** -- for each save sub-directory open every *.bin profile, read the
  text (FablePro header: version, end offset, text; else the raw file), transfer "ProfileName", collect, sort. Shape: the "." test as a
  named bool statement (its temporaries die before the IsPathFile test; one `!a && !b` keeps conditional-destruction flags), the profile
  file opened with an implicit unsigned short* -> CWideString conversion (temporary built before the 1 / 2 pushes).
- **NTCScriptedControl::CActionTalkBase::Update (0x00716DE0, 10 grades)** -- conversation update: look-at, speech timing, end of line,
  animation / attitude / gesture / camera tags. Shape: `key * GameTurnDuration <= UpdateCounter` for the three tag tests (product first,
  `test ah,1 ; jne`); a named `CThingCreatureBase& creature` and a named `long fps` ahead of the GetAnimDuration call (retail reads
  ConstantFPS before the call and keeps it in the frame for the `fimul`); CSoundPair(0) arguments; stack actions destroyed by
  ~CCreatureActionBase (implicit derived destructors, header opt-in).
- **CSubtitleRenderer::ProcessEmbeddedGraphicTags (0x00492381, 7 grades, /Os)** -- [tag] -> embedded button graphics. Shape (measured with
  the c2 frame-packer trace tools/pk.sh, hand886/agent2003 phook): the CEmbeddedGraphic record in its own block closing before the
  GFFloatToLongFloor asm expansion (a local in scope at an inline-asm block gets +2 IL weight; the record must weigh less than the box
  and the parser to share the parser's slot); the newline-count loop as a `done`-flag loop (no rotation, retail tests the zero in memory
  every pass); the area box a temporary argument with implicit double -> float Y arguments; the centring offset a named float.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Grades used (pass 1 only; no row needed a second pass)
| Row | grades | detail |
|---|---|---|
| 0x00492381 | 7 | 1541 (first draft), 1528 (done-flag loop), 1528 (temporary box: frame 0x9C), 1528 (record block: slots), 1522 (named offset: one store-order line), RELOCATION_MATCH (implicit double->float), RELOCATION_MATCH (shared CPlayerGui / CDisplayManager headers instead of row-local declarations); plus 11 fragment probes and 10 c2 frame-packer traces (not diffed against retail) |
| 0x004959E0 | 2 | 1570, RELOCATION_MATCH |
| 0x005248B0 | 8 | 1539 (first draft) and 6 full-row probes (GetDefaultVal / named / in-loop defaults, the visible Transfer stand-in diagnostic), RELOCATION_MATCH (real Transfer body); plus 2 validation compiles of the Transfer instances against 0x0044BB00 / 0x004106F0 |
| 0x00716DE0 | 10 | 1488 (draft), 1511 (CSoundPair(0), `!(a < b)`), 1514 (fps local), 1511 / 1511 (compare spellings), 1514 / 1514 (`x <= UpdateCounter` on all three), RELOCATION_MATCH (named creature reference + fps local, the 8th), 1517 / 1517 (index / float fps variants run in the same batch) |

## Names (proven from two sources unless marked; LABELS.tsv)
- Rows: all four PDB spellings (Ego_r.pdb + FableWin.pdb) with the argument / return shape and callers (LABELS.tsv).
- Callees bound with corrected spellings (NAME_FIXES.tsv, 12 entries): 0x00751A70 CTCLook::IsOverriddenByParent (landed as a free
  EditWorldUpdate stand-in), 0x00715A70 SetUpCamera / 0x00716B30 SetUpTextAndSpeech (private AAE, bound IAE / QAE), 0x007534B0
  IsPosInViewRange (QBE, bound QAE), 0x00842400 PlayConversationAnimation ctor (reference + enum), 0x0048DA89 / 0x0048DAA2 CFontBank
  string helpers, 0x0049417D vector<CEmbeddedGraphic>::push_back, 0x00999850 CAFile::ScanNextFile, 0x00437260 / 0x00A0C910 CWStringParser
  ctor (const) / SkipWhitespace (long), 0x00440D70 vector<CEnginePrimitiveHandle>::erase (stand-ins bound by eight callers).
- **Descriptive**: `g_bLeftAlignText` (0x013B861B): no data-symbol dump on this machine; the bool is the "LeftAlignText" config value
  (string 0x0122DB98, transferred at 0x0040276F, handed to CXMVPlayer::SetLeftAlignText at 0x00402826); spelled like its config neighbours
  (g_bUseCompiledDefs 0x013B8617 ...). Search: Ego_r / FableWin name sets (functions only), strings, FSE manifest -- no data name.
  `VIRTUAL_SCREEN_HEIGHT` (0x013961F0) and `GPDisplayManager` (0x013B8390) use the established spellings.

## Header debt
- CFrontendGameComponent has no shared header: 0x004959E0 declares the class with only its static member (no layout used).
- CActionTalkBase / CSubtitledMessage have no shared header: 0x00716DE0 reuses the landed sibling 0x00716B30's row-local models at the
  retail offsets (same TU), adding Update / the private sibling declarations.
- CPlayerGuiDef is the generated flattened layout (TextBoxTextAreaTLPos_X ...): 0x00492381 reads those members.
- The repo's CWStringParser.h declares a virtual destructor and lacks the ctor / readers; CCreatureAction_PlayAnimation(.WithLookTurning)
  declare destructors retail does not have; CCreatureAction_PlayConversationAnimation.h is the generated pad layout: all handled by opt-ins.

## Label corrections
See NAME_FIXES.tsv. The catalog / manifest labels of the four rows were right (CFrontendGameComponent::GetProfileNames is static).
