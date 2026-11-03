# 00efdc40 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 18. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00efdc40/NAMING_EVIDENCE.md

# 00efdc40 identity and evidence

The function is private virtual `NScript::CMarkTestScriptsScript::CShopKeeper::Main`, leaf `?Main@CShopKeeper@CMarkTestScriptsScript@NScript@@EAEXXZ`. Both original PDB name sets contain it. Original Ego_r procedure records place it after the MarkTestScripts active-entity machinery and before the owner Alloc, matching native order. Native `CShopKeeper::Alloc` at 00efd9c0 allocates 0x1c, stores vtable 012f6e44, and stores ParentClass +14 and PMasterData +18. That table's slot 1 is exactly 00efdc40. The original complete TPI records are 0x107c7 (owner) and 0x107d7 (nested CShopKeeper). The owner has no own data members; the nested class retains both protected fields, the exact CEntityScriptBase hierarchy and its private Main override.

The retail extent is 1847 bytes: the final ret is at 00efe376 and the next byte is CC. No recut is used. The selected RTM object emits the exact leaf once and has 1810 bytes, a DIFFER result.

Original procedure records recover seh_me, speech_movie, PauseMyEntitiesPlease, ShopList, int i, and FableWin's TRetVoid _Fail_Val. Raw original records are in `evidence/00efdc40_types.json`, `00efdc40_base_types.json`, `00efdc40_reference_types.json`, `00efdc40_locals.json`, and `00efdc40_enums.json`. All were read from the supplied original PDBs, not inferred from the header text.

Original TPI corrects CEntityScriptBase Game to a protected reference and Me to protected, CLevelScriptBase Game and MasterScriptData to references, CIsThingConscious Thing to private and CBaseObject IntelligentPointer to private. These types have the same native offsets but now preserve the original declaration types/access. Header changes are private to workers/scripts_a/hdr and every final compile snapshots them.

Native 00ef7ab0 is the CIsThingConscious destructor, proven by vtable 012c2fe8, its private Thing member at +4 and original PDB name. The supplied placeholder CScriptThingFunctor name is rejected. The vector destructor is the real two-template-argument STLport vector<CScriptThing,allocator<CScriptThing>> at 008ac970. MovieBase PImp's out-of-line destructor is CCountedPointer<CScriptGameResourceObjectMovieBase> at 006e7ab0. Both calls are declared with exact real types; no stand-in release helper is used.

All 86 selected-main object relocations are individually recorded in RELOCATIONS.json. Literal data is checked against native operands including terminators; the two health comparisons use native float zero at 0122dedc. Calls and vtables are checked by original symbols and independent native identities. Because the instructions diverge, the audit does not assign native targets from masked object offsets. Fifteen virtual method identities are checked against the actual native Game and CScriptThing vtables and original PDB symbols. NAME_FIXES.json contains wrong legacy spellings found during that review.

The exact selected Main body is copied unchanged into a standalone C++03 fixture. GNU64 and native VC7.1 /GR- pass all condition combinations, empty/nonempty stock, acquisition/dialogue waits, each observed termination checkpoint, short-circuit healing exclusion, signed prices and balanced RAII cleanup. These tests establish behavior coverage, not machine-code parity.

## Current main verified selection

# Naming evidence

Exact Main Leaf exists in both original PDB name sets. Retained class/resource/predicate/template declarations are unchanged from root15; the current root explicit resource destructor-declaration gate replaces the old coupled gate without changing the original declared destructor. Every88 object relocation is mapped to its native positional target in RELOCATIONS.json. Original PDB identities are checked for all function/global operands; vtable identities, string terminated bytes and IEEE float payloads are independently checked. Native own vtable012f6e44 slot1 points to00efdc40. Full1847-byte extent ends at RET00efe376, followed by9CC bytes.15 virtual slot identities are checked against the real game and thing vtables.

TRetVoid is the existing original adapter. No additional helper body, stronger annotation, authored compiler-generated body, or invented type is introduced. Donor loop evidence establishes source shape without claiming original spelling beyond the observed control structure.

Previous parked conclusion is superseded by the exact original generated loop form. Root independently verifies the final selected source, all target operands and both test compilers.
