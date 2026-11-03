# 00e26e30 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 33. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00e26e30/NAMING_EVIDENCE.md

# Identity and native evidence

Exact leaf: `?Main@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@EAEXXZ`, private virtual. Original Ego_r TPI10e29/10e39 and both original PDB receipts are in evidence/00e26e30_types.json. Original module locals are in00e26e30_locals.json: seh_me, WavingFrequency, speech_movie, PauseMyEntitiesPlease, seh_hero, mymap.

Independent native registration at00cd7d18 uses string012c54e8 QS_GuardianSisterInfo2_SisterInBanditCamp;00cd7d3d stores Alloc00e277e0;00cd7d4d uses alias012f77d0 S_QSGSI2. Owner allocation requests0x4c and installs012e1f08. Owner binding names MazeAtTavern (012e1e18) and stores child allocator00e26af0. That allocator requests0x28, stores Game+4, Me+8, protected ParentClass reference+14 and PMasterData pointer+18, and installs012e1f74. Slot1 at012e1f78 is00e26e30. Child Init00e26bd0 writes private TalkCounter+1c, WavedOver+20 and HitCount+24. Parent GuardianSpokeToHero+48 is corroborated by its persistence key. Full owner layout uses native0x48 base instead of donor0x5c; no member is dropped.

Native2477-byte extent ends RET00e277dc, followed by CC. Actual selected RTM object has one exact leaf. All selected leaf relocations and28 virtual identities are in RELOCATIONS/SELECTION; selected attempt24 contains actual unmasked objdump. DIFFER relocation review proves individual target identities and literals, never positional equivalence.

Helpers use original PDB names/PUBLIC32/TPI plus native bodies/table entries. Original pDSTGame is CGameScriptInterfaceBase pointer at0143e8f8; CTimer has private index. Timer slots87-90 register/deregister/set/read. FadeOutAndKillEntity takes `(const CScriptThing&,float,bool,bool)`; native00cbe716 receives the first bool in DL because fastcall skips float arguments, and reads float from stack. Older bool-before-float binding is rejected. Native01375748 is NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations. Types and helper records are in00e26e30_helpers.json.

Literal audit corrected cast key MAZE (012ce55c), alongside HERO (01255174). Cutscene is CS_GUARDIAN_SISTER_2; quest card OBJECT_QUEST_CARD_BANDIT_CAMP / Q_BanditCamp. Native hit-response slot599 sets reciprocal allies. Earlier prototypes used enemy-unset and inherited ME; neither is selected and repairs are excluded from the strategy count.

Historical eligible attempt24 DIFFER(2462v2477), superseded by verified33 below, exact-body GNU64/native VC7.1 tests PASS/PASS. Natural compiler-generated resource construction is restored. Original STB default/copy constructor TPI attributes0x103 marks compiler generation in both PDBs (evidence/00e26e30_STB_TPI_EVIDENCE.json). Expert donor inspection of027a7500 found base02467d30 followed by default counted-pointer constructor027ac860, with no null assignment. Native007e72a0 only exposes base construction, vtable and zero fields; it cannot distinguish a redundant assignment. Therefore prior explicit PImp=0 probes19/22/23 are rejected diagnostics, despite their closer stack layout. Selected natural construction still differs at late inline expansion, timer cleanup sharing and register scheduling. Behavior PASS does not claim a byte match.

## Expert32 and independent main33 match

# Final original identity and header/body compliance

Root rows/00e26e30/NAMING_EVIDENCE.md and original PDB/registration/factory/helper receipts remain identity authorities. Exact private virtual Leaf exists in both original name sets, own native vtable012e1f74 slot1 points to00e26e30, and exact2477-byte extent ends RET00e277dc then3CC. Current header keeps original ParentClass and Game references and full owner/master layouts. Every126 actual COFF operand is independently resolved at the native position; all strings and float payloads match exactly.28 real game/Thing vtable slots and exact original symbols are read from the native tables.

The genuine late STB constructor call007e72a0 now appears naturally, with both original PDB names/TPI and actual original donor body default construction. All resource special members carry compiler-generated0x100 bits and remain implicit/external declarations. No PImp null assignment or generated special-member source body is supplied. Cast key MAZE/HERO and reciprocal ally slot599 corrections from eligible24 are retained.

The only stronger authored bodies are the original CTimer assignment and pause constructor. Original timer assignment0360a190 independently reads TimerIndex+0, pDSTGame and virtual SetTimer89, returning this; existing evidence/00e26e30_helpers.json gives original class/member types. Original pause constructor0359fd38 and native00cbe09a prove protected CGameScriptInterfaceBase* Game initialized from the reference argument, then PauseAllNonScriptedEntities(true). The current shared opt-in is reused. Ordinary forms failed in preserved histories; newest Owner conditional helper preference applies as for accepted timer/pause rows. This is a documented category application, and original keyword remains unproven. No native expansion is offered as keyword proof.

Original debug Main0391a6f0 proves generated frame-loop increment, TRetVoid and scoped success return. Its success condition nesting differs from retail, so private26 is explicitly diagnostic-only. Native00e275d9 loads the shared completion flag on both talked-to paths; selected final source keeps that native behavior and returns normally inside the loop after success actions. The exact fixture distinguishes the initially complete/no-talk case and verifies correct effects/cleanup.

Main independently recompiled selected expert32 as33 against shared opt-in headers and reran both C++03 fixtures (1580 scenarios each). All126 actual operand positions,28 original virtual identities, exact Leaf and full2477-byte extent passed; the correctly rebound bytes equal the entire native extent. reviews/00e26e30/RESULT.json is the accepted receipt.
