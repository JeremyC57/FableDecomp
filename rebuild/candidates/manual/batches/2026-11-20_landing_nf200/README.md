# Landing nf200 (Account A, 2026-11-20): fireball spell, quest-script speech, CBaseList and libpk buffer names (15 rows)

Agent NF200 (round 10) handed back early (context) after q6932 with 50 entries: 7 done, 6 blocked, 3 deferred and 34 open. Its files
are in `naming/2026-11-20_agentNF200/`, with the held libpk unit (`held/`). The open entries, the partial families q4492 / q4522 / q6930
and the three deferred entries go back to the queue unclaimed; their rows were NF199's, which has since landed. q4411 / q6405 (the
TU-emitter `??_G` family), q6931 and q6932 stay blocked. 40 entries are returned.

- 15 rows land: 1 MATCH (0x00E83680) + 14 RELOCATION_MATCH, every test PASS. The two funclets whose parents changed grade
  RELOCATION_MATCH from disk with their funclet offsets (`logs/funclet_recheck.log`, repo recheck.py + vland): 0x012173E0 (parent
  0x00CB1890) and 0x012134AB (parent 0x00C287E0). The landing's includer pass graded 0x012134AB DIFFER 136 v 14 because it graded at the
  parent's offset (the nf188 lesson).
- Self renames (RENAMES.tsv section 1, 9 rows):
  - 0x007C9180 `CTCFireballSpell::FinishSpell` (both PDBs), on the new engine/CTCFireballSpell.h opt-ins.
  - 0x008EC1A0 the STLport `vector::push_back` instance.
  - Quest scripts onto their nested NScript classes: 0x00DA2610 / 0x00DA26B0 / 0x00DA27B0 (CBriarRose DoSpeech / DoOnHitSpeech /
    DoOnSpeakToSpeech), 0x00E83680 / 0x00E83620 (CTyler CleanupAfterFight / OnPredicateFail), 0x00E2E440 (CASTrickster GetRandomSpeech).
  - 0x00CB1890 becomes `??1CBaseList@@QAE@XZ`, DirectShow BaseClasses (strmbase) `CBaseList::~CBaseList`. Advisor decision (owner
    delegated): it lands. The self-rename rule concerns the OLD SELF LEAF, and `??1CEngineLandscapePatch_00cb1890@@QAE@XZ` is now gone
    from every row, test and table line. The address narrows 5 -> 4 spellings onto one 0x00CAF580 already binds; the other stand-in
    binders (0x00CA7A20, 0x00CB0F10, and the extern-C forwarders 0x00CA7A10 / 0x00CAEEA0 / 0x00CB0CA0) join the queued strmbase light
    pass. Funclet 0x012173E0's include is repointed to the moved file.
- Binders respelled (section 2): 0x007C93B0, 0x007C7A00, 0x0047DB73 (IS_THING_ALIVE<CThing*>; its `static_cast<CThing*>` is to the
  value's own type, so no stand-in cast), 0x006412B0, 0x00C72E00, and 0x00C287E0, which moves in place onto the owner 0x00C29CA0's
  existing Leaf. q6931 stays blocked: that Leaf is the libpk house placeholder `??1CBuffer_00c29ca0@@QAE@XZ`, and the real name needs a
  family port with 0x00C22520.
- Held at landing (advisor decision): the libpk unit 0x00C162A0 / 0x00C2D3B0 / funclet 0x01213B0A. NF200 renamed it onto
  `SPKVoiceSettings`, rewriting the self row and its test, which is more than the light vendor renaming libpk gets. The 0x1C record type
  also carries three made-up names, none in a PDB (SPKVoiceSettings, SThemeRequest, CVolumeRecord_00c161a0), and the evidence leans away
  from the one being extended. q6932 is blocked with that decision item.
- Headers: add-only `#ifdef` opt-ins, `orig` equals HEAD for all three, and no landed includer defines them (`naming/.../hdr/README.md`).
  - engine/CTCFireballSpell.h: MEMBERS (the Ego_r data members +0x0C .. +0x7C under the macro, sizeof 0x80 asserted, the
    CTCTurncoatSpell.h pattern) and FINISHSPELL.
  - engine/CCreatureAction_FinishFireballSpell.h: FINISHFIREBALLSPELL_CLASS (two ctors 0x008BCE10 / 0x008BCE50, sizeof 0x114); 2 asserts
    pinned.
  - engine/NGraphicAppearance_CTCGraphicAppearanceReplaceableMesh.h: REPLACEMESHBYINDEX (0x007A69F0); 1 assert pinned.
- Pick-any (`logs/pickany_check.txt`): every copy with a table owner equals it, bytes and relocations: IS_THING_ALIVE (0x0040DCE0,
  0x004CC340), CBasicString `==` 0x004A93C0, CIntelligentPointer `=` / copy ctor (0x004A9760, 0x004A9730), SetPItemRemoved 0x0072D360, and
  bitset<274>::test 0x00410DE0. `?Next@CNode@CBaseList@@` (new in 0x00CB1890, no owner) equals 0x00CB1210's copy. `?HasTC@CThing@@`
  0x004118C0 has no landed row.
- Queued: 0x007C93B0's self Leaf should be the virtual `?FrameUpdate@CTCFireballSpell@@UAEXXZ` (NAME_FIXES; both PDBs). Also the two
  stand-ins left in 0x0047DB73, the 8-spelling 0x00C167C0 (likely `PKCAutoDeleteArray<unsigned short>` destructor), and the 0x00C28AE0
  layout note.
- Link (`logs/link_landing.log`): 2 LNK2005 against 3 at HEAD (`logs/link_control_head.log`). The `?FrameUpdate@CTCFireballSpell@@QAEXXZ`
  clash is gone, and no new one appears.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Referencing rows 204 good, 1 bad, and the bad one
  is the funclet offset artifact above.
- tablecheck (`logs/tablecheck.txt`): twelve addresses shrink (among them 0x004AC0F0 6 -> 5, 0x00CB1340 4 -> 3, 0x0040DCE0 5 -> 4,
  0x004CC340 4 -> 3, 0x0062E900 2 -> 1); none grows. No out-of-image oracle line.
