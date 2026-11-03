# 00dc1770 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 9. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00dc1770/NAMING_EVIDENCE.md

# Identity and original declarations

The exact leaf is `?Main@COakValeBanditRaidBandit@CQ_OakValeBanditRaidScript@NScript@@EAEXXZ`, present in both original PDB name sets and the selected actual COFF table. Native registration dbf607 constructs literal12d9ec8 `OakValeBanditRaidBandit`; dbf633 binds factory dbfdd0. The factory allocates1c, builds CEntityScriptBase, stores table12d9f64 at dbfe04, and sets ParentClass14/PMasterData18. Slot1 is dc1770. The final native instruction is the cleanup jump atdc2160, followed by14 alignment bytes fromdc2162 before the next functiondc2170. Full receipts: evidence/00dc1770_binding.disasm, 00dc1770_factory.disasm, 00dc1770_native.disasm.

Original raw TPI receipts are preserved in evidence/00dc1770_types.json: owner Ego_r1064e/FableWin10808; nested Bandit1066d/10827 and RestartMarker1065e/10818. Main is private virtual; ParentClass is protected lvalue reference and PMasterData protected pointer. The private shared owner header retains all13 original fields and both children. Retail factorydc24e0 allocates78; root base48, 12-byte HerosFollowers at4c, booleans48/49/4a and58/59, SoundPlayed5c, StopWhistling60, BanditsKilled61, IsBanditInRange62, BanditInRange64, BanditsStopChasing70, NoPersistence74. Original GetMasterData is non-const; GetParentScript is const.

CanThingSeeAnythingInVector is public thiscall long(CScriptThing&,std::vector<CScriptThing>&), **both non-const references**, proved by both original function type records10647/10801. Its native dc2170 loop uses parent Game40 and native slot632, scans a12-byte vector and returns the first seen index or-1; dc1866 anddc1f1f are this row's calls. The older supplied neighbor's const first-reference spelling is wrong and recorded in NAME_FIXES.json. Original naming sets alone are never used as retail address maps.

The original locals receipt evidence/00dc1770_locals.json proves one shared long `spotted`, seh_hero/seh_me/seh_merc, mymap, speech_movie, PauseMyEntitiesPlease, ICU, `_Fail_Val`, and GoBackToWandering. Sharing spotted across the two branches restores the original temporary allocation. Native literals independently prove Q_OakValeBanditRaid, HERO/ME/MERC and the two gate-close cutscene names; the selected cast assignment/dynamic calls are audited individually.

The authored CScriptThing equality body is independently proven by native008aae20 and original donor02a838b0. The donor calls debug ValidatePImp then the original counted pointer operator-> and operator*, then IsEqualTo slot78. Both original TPI records make operator== a public const normal method with raw attributes3; it is not compiler-generated. Private header expansion retains this original release body. Ordinary variants02/03/04/06 fail; the newest Owner conditional header-helper preference is applied without claiming an original stronger keyword. Details and exact thunk targets are in evidence/00dc1770_EQUALITY_BODY.json, with full original raw fields in00dc1770_scriptthing_types.json. The pause helper stays ordinary because its natural expansion matches. Generated resource constructors/destructors are only declared where native calls them; no authored replacement body is supplied.

Selected07 is RELOCATION_MATCH2546. All116 actual COFF operands and18 virtual identities were checked; replacing only those correctly bound operands makes all2546 bytes identical. Exact selected body passes1688 standalone C++03 cases on GNU64 and native VC7.1 /GR-. The local audit is development/00dc1770_review.py and reviews/00dc1770/LOCAL_REVIEW.json; header hashes and immutable snapshot paths are in SELECTION.json and HEADER_HASHES.json.

Main09 independently rebuilt the exact source after original declaration-only dependencies: RELOCATION_MATCH2546,116 operand identities and18 virtual slots checked,1688 cases/compiler PASS. Attempt08 is missing-header repair only. No generated body authored.
