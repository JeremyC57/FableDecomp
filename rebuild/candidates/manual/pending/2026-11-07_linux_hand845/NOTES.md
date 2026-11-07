# Chunk 845 -- authoring notes

Result: 1 RELOCATION_MATCH (0x00E57E60), 5 parked in differ/ (each with its test). No Codex prior art was usable (coordinator
note): every row was authored from scratch. The S15 inline-budget meter was graded once per row as a diagnostic only
(probe/*_bS15.cpp); no /d2 flag and no pragma is in any src/ or differ/ source.

Names: all six row names are in the Ego_r.pdb / FableWin.pdb name sets in the exact spelling of the `// Leaf:` lines (objdump -t
of our objects agrees). The brief said the three TLC-only classes have no PDB symbols: that is wrong for CBS_Teacher (both
scripts) and CTB_Bard -- `?Main@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ`,
`?Main@CBS_Teacher@CV_HeroDollsScript@NScript@@EAEXXZ`, `?Main@CTB_Bard@CV_TalentlessBardScript@NScript@@EAEXXZ` are in the sets.
Second source for each: the entity / script vtable slot (slot 1 of the entity vtables 0x012E5F40, 0x012ECEB4, 0x012F3498,
0x012C8B9C; slot 2 of the script vtables 0x012E65E8, 0x012D1E10) stored by the landed Alloc / ctor.
Descriptive local names (no retail evidence): `asked`, `hero_has_dolls`, `answer`, `seh_kid`, `seh_me` (the HeroDolls and Bard
locals; Book's seh_me is the PDB member), `speech`, `return_speech`, `no_cash_speech`, `bully`, `beggar`, `whisper`, `count`.

## 0x00E57E60 NScript::CV_BeggarAndChildScript::Main -- RELOCATION_MATCH (src/00e57e60.cpp)
Two `new CEntityScriptBinding<CV_BeggarAndChildScript>` bindings ("LookoutPointBeggar" -> CLookoutPointBeggar::Alloc 0x00E58C60,
"BeggarBully" -> CBeggarBully::Alloc 0x00E5B2D0), both PostAdds, the four-flag wait, then one block: bully / beggar things,
three scripted handles started with the BC_START_SCRIPTING loop, the std::map cast HERO/BULLY/BEGGAR, the movie with the
MOVIEBASE OUTOFLINE opt-ins, FixMovieSequenceCamera(true) written explicitly, the RunCutsceneMacro chain per outcome, the
expression tutorial (IsXbox: Display / wait; the first arm tests `ExpressionTutorialShown` without `!`, as retail),
SetPreferredQuickAccessItem, GiveHeroMorality(pGlobalScriptDef->BC_EvilMoralityLoss / BC_GoodMoralityGain),
FixMovieSequenceCamera(false), DeactivateQuestLater(GetActiveQuestName(), 0). New header engine/CV_BeggarAndChildScript.h.
vland: RELOCATION_MATCH + PASS; one RELOC_NAME warning (placeholder label at 0x00CDBFB0, in LABELS.tsv).

## 0x00D67890 NScript::CQ_GuildTrainingWoodsWillScript::Main -- parked DIFFER(3472v3426), md 120
Shape in differ/00d67890.cpp (new header engine/CQ_GuildTrainingWoodsWillScript.h). Residue: retail inlines the plain
`seh_bandits.resize(bandits.size())`; ours calls the resize(n) COMDAT. A bisect (probe x1-x7, mini.cpp) shows it is inline
frequency: the same call inlines in a mini TU and after one preceding start-scripting wait, not after two. The explicit
`resize(count, STB())` expands but then colours registers differently from entry. Spawned functions: WatchForTermination
0x00D68600, DoMission 0x00D68AE0 (label missing, LABELS.tsv).

## 0x00D04AE0 NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main -- parked DIFFER(3847v3623), md 183
Header: opt-in FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSIN1 in engine/CQ_BanditCampBossBattleScript.h. Probe b (two idle
loops, the if-arm with its own `for (;;)`) is RELOCATION_MATCH under the diagnostic S15 meter: the documented caller-size budget
(the implicit MovieBase dtor is called at three scope ends / speech pads where retail expands it). differ/ holds c (best plain
grade, A1_SAY as a do-while statement macro).

## 0x00E54E90 NScript::CV_BookCollectingScript::CBS_Teacher::Main -- parked DIFFER(3534v3534), md 35
Same size. A row-local `inline CScriptThing::~CScriptThing() { }` fixed the per-site temporary dtors and slot order (md 104 -> 35).
Residue: retail's entry `mov [esp+34h],0` (a conditional-temporary flag home sharing the SetQuestAsCompleted
GetActiveQuestName temporary slot; its owning expression is threaded away completely and was not found) and the
expanded-movie-dtor return pad of the first TalkedTo block placed before the called-dtor pads (ours last). New header
engine/CV_BookCollectingScript.h. Callees: AskForBook 0x00E55CE0 (NAME_FIXES.tsv), LookForBook 0x00E57530 (LABELS.tsv).

## 0x00E97070 NScript::CV_HeroDollsScript::CBS_Teacher::Main -- parked DIFFER(3619v3619), md 60
Same size. A row-local `__forceinline CScriptThing::operator=` expands both KidIntro assignments (the implicit one was called at
the second). Residue: the same entry flag-word store as Book (`mov [esp+20h],ebx`, later the GetActiveQuestName temporaries'
slot) and the 4-byte shift of the locals above it. New header engine/CV_HeroDollsScript.h (RemoveHeroDolls 0x00E97EA0,
HeroHasAllDolls 0x00E97FD0, HeroHasAnyDolls 0x00E98200).

## 0x00ED8680 NScript::CV_TalentlessBardScript::CTB_Bard::Main -- parked DIFFER(3793v3652), md 196
New header engine/CV_TalentlessBardScript.h. The spawned-function region name: retail builds `""` after the new-expression and
destroys it first, so it is AddSpawnedFunction's default argument: new opt-in FABLE_RETAIL_CSCRIPTBASE_ADDSPAWNEDFUNCTION_DEFAULT_REGION
in engine/CScriptBase.h (md 324 -> 196; Ego_d.pdb has no SpawnFunction-style wrapper on CScriptBase, so a default argument is the
plain spelling; landed local-model rows use a row-local __forceinline SpawnFunction for the same order). The names must be
`CCharString("ParentClass.") + CCharString("WatchForAttack")` (a const char* left operand picks another operator+).
Residue: the documented nested-copy blocker 1 (retail calls 0x004ABE90 for the CIsThingAlive member copy and the CSpawnedFunc1
Par1 copy in the inlined ctor, expands the by-value Me copy and calls the param dtor 0x004AA840). Callees:
WatchForAttack 0x00ED9790, WatchForResetWanderCentre 0x00ED94D0, CTB_Bard::GetRandomSpeech 0x00ED99D0 (LABELS.tsv).

## Bindings
Checked against rebuild/oracles/reloc_bindings.tsv: no symbol bound here is bound at another address. Note for the coordinator:
0x0099E960 already carries three symbols (??9CCharString@@QBE_NPBD@Z, ?Compare@CCharString@@QBE_NPBD@Z, ?check@Str@@QAEDH@Z);
HeroDolls uses the `!=` spelling. 0x00CDBFB0 likewise carries the map dtor and two CCutsceneCastMap names (folded body).

## Header debt
- engine/CV_BookCollectingScript.h: `vector<CConversation>` uses a forward-declared CConversation (engine/CConversation.h exists
  and could be included); the two vector<bool> members are opaque 0x14-byte blocks (no shared STLport vector<bool> model yet).

## Skipped
None.

## Label corrections
See LABELS.tsv (5 missing / wrong labels plus the matched row and one placeholder) and NAME_FIXES.tsv (AskForBook).
