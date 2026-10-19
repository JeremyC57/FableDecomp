# Chunk240 handoff — whole40 authored

All40 reserved rows have authored C++ candidates and exact-production, expanded-actual-header fixtures passing host C++98/O2 and VC7.1/Od. This is not a claim that all40 match retail. Current inventory has 11 independently root-landed rows, 12 worker-exact rows awaiting root review, five capped DIFFER rows, nine near matches, and three other DIFFER rows. No unauthored row is labeled DIFFER. No replacement chunk is authorized.

The authoritative machine-readable ledger is `ALL40_INVENTORY.tsv`: each row records root acceptance separately from worker readiness, chosen source/fixture hashes and paths, native grade verdict and lengths, flags, fixture logs, historical/local/cumulative counts, and remaining allowances. `RECOVERED_NAMES.tsv` records native identities, fields, PDB/RTTI/caller/string anchors, descriptive aliases, and donor drift for all40. `GRADE_AUDIT.jsonl` reconciles every immutable production snapshot/hash/command/flag entry against `ATTEMPTS.jsonl`. `ARTIFACT_SHA256.tsv` hashes the complete scratch tree.

## Attempt accounting and retention

183 local source/flag/compiler-failure grades + four historical UNCERTAIN reservations =187 cumulative. The historical reservation applies only to006F9430 (prior hand146/differ/006f9430.cpp), whose local5 yields9 total and1 retained. The original history audit is preserved; see `HISTORY_CORRECTION_006f9430.txt`. No allowance resets occurred. 213 mathematical allowances remain across all rows; accepted/exact rows are frozen and their remaining allowances are not instructions to continue grading. Every local row stays at or below10.

The production snapshots in `snapshots/<address>_<grade>.cpp` are immutable. Each was saved and hashed before grading; corresponding complete commands/effective flags and stdout/stderr are in `ATTEMPTS.jsonl` and `logs/<address>_<grade>.log`. Compiler failures and flag variants count. Fixture failures have retained logs and hashes; not every early failed fixture source was separately snapshotted. Current passing fixtures and selected review corrections are retained. This distinction must not be erased in the Claude archive.

`src/` contains only q-confirmed MATCH/RELOCATION_MATCH candidates. `differ/` contains the selected best semantically validated nonmatching source. `probe/` may contain a later worse trial; use the inventory’s selected path/hash, not the newest probe automatically. Do not use obsolete79FD90 g1..7, which had incorrect early-return behavior and an obsolete CTC table.

## Root acceptance and pending review

Root166 bafa63797 accepted A66450/A662A0; root167 0d898805c accepted81E810/6F9430; root182 eb7f1d422 accepted BDFA10/BE6090/BF67F0/BF9BC0/6F99B0; root185 617b90321 accepted9E4470/A42980. Root reports independent compiler, native fixture, host O2 and no-elide checks for these. Keep their frozen hashes.

The other12 exact rows are WORKER_READY_AWAITING_ROOT, not accepted:691AE0,689130,CDF260,DF4610,CF8970,CF9210,CE0150,7A0630,E338E0,E48A30,418DCA,DA4350. Root still needs independent native ABI, relocation/callee/string/float, fixture and compiler review. No worker edits were made to canonical sources, shared headers/tools, git, or root-owned oracles.

## Final three exact candidates and corrected native identities

7A0630 g2: real LightningBeam(CThing*,CThing*) owner, native7C vs donor84 because children list4 vs12. Three native CIntelligentPointer<CThing> members derive from CBaseIntelligentPointer, whose sole virtual slot is its destructor. A01B10/A01B50/A01B90/A01C10 are constructor/GetPItem/SetPItem/destructor; native VT1238C6C slot0=4A9710. Four nullable-pointer cases, sentinel links, all initialized fields, pointer temporary/owner lifetimes, and callback-injected Def release pass. The fixture’s TestDef is explicitly a test-only implementation of a pointee field/method view; it does not claim native CDefPointeeBase construction.

DF4610 g3: KrakenMinion::Main installs CIsThingConscious, not CIsThingAlive. Native predicateVT12C2FE8 targetsCDDAB0/CDDAE0/CDDB10; operator calls Thing slot75 IsAlive and negates slot61 IsUnconscious. PDBCC2D20 names the predicate. This masked-vtable identity was corrected after g2. The genuine resource has30 slots, BaseObject hierarchy, default constructor and ordinary destructor7E74D0. CD23B9 is IsValid (PDB035A07CA), CD2770 is Release (PDB035C5B25). Game slots7/8/70/350/451 and literalHUD_ORB_QUEST_CORE are proven. Fixture covers acquisition wait and termination checkpoints; ordinary fresh-resource IsValid is truthfully false. Source line3 still contains a stale copied D2A820/D36CB0 owner comment; current owner constructorPDB038ABA10 and DF4610 function identity are authoritative.

418DCA g3 /O1: native294-byte CMainGameComponent(CGame&,const Init&) with real GameComponent->NonCopyable->BaseClass plus secondary DeviceResetCallback+4. Primary table has10 slots; secondary table has2 and no destructor. Access qualifiers for Base SetQuit and Main Update/GetInputs/Render/PostInit were corrected to protected in g3. Genuine external member constructors use actual available NetworkClient/EventPackageSet/DiskFile headers and honest field projections. Native FrameRateSmoother48 vs donor56 shifts later fields by8. Native owner161E8. Fixture checks construction order, secondary callback adjustment, pre-callback fields, post-callback debug-counter store, intentionally untouched fields, and lifetime counts. Init-copy dependency is tested with empty strings only, not an invented implementation of general string-copy behavior.

`probe/final_three_native_tables.txt` and `probe/final_three_emitted_tables.txt` preserve native/COFF table evidence. `probe/007a0630_rtti.txt` separately proves pointer hierarchy. Production helper bodies remain external; dependency fixtures are stubs of the genuine signatures, not replacement simplified target algorithms.

## Nonmatches and remaining work

Five capped rows:81F910 (257v263),8F6FC0 (303v297),BE5E70 (302v302),547500 (241v241),79FD90 (253v253).79FD90 g10 has corrected native semantics and an honest field/method view with full-host-width PMF for proven slot33 CThing*()const. Native null/dead/no-component Home cases return TRUE. The second missing-component branch reads saved beam-this +40; this odd fallback is preserved and tested through the native view. Remaining code difference is stack12 vs native8 and corresponding saved-this/key offsets. Old variants are invalid semantic models.

Near matches retain budgets rather than consuming variants without a supported hypothesis: D9EB30 differs only in mov-ECX/push ordering; /Os and/G7 trials did not improve.4164E0 /O1 differs in final time/subtract/increment/store scheduling; /G7 changes length but does not match.53B7F0 differs in Find temporary stack allocation/store timing. A65480 differs in header alignment/register selection and reused size load. A65720 differs in register allocation and retry-loop tail control flow. A5CEE0 native SSE sequence behaves like manually grouped vector operations; compiler intrinsics schedule operations differently even at identical length. BE37E0 differs in x/y register allocation and side-zero compare; /O1 worsens length. No assembly, bytes injection, artificial volatile/alignment, or fabricated slots were used to force matches.

41DB84 has a known material ABI block: native GetTextBySymbol41CFB2 uses LTCG EDI hidden result storage plus stack string reference. Current ordinary fastcall return-by-value model is a semantic fallback, not native ABI ready. Its passing fixtures do not validate that native helper call ABI. Other larger differences are536A30 (228v242),5BEC92 (325v313),A9D480 (221v243); see selected logs and names table. Partial budgets are retained with exact best sources and meaningful fixtures for Claude or an explicitly authorized later retry.

BE5E70 capped fixture derives from older Patch/Water declarations; compare override ownership against accepted BE6090g2 before reusing it. No new grade may be silently taken after its cap. Native fixture PMFs are four bytes, host fixtures retain full host width. Host native offset assertions are disabled only for host-width portability; VC assertions remain active.

## Fixture limits and known comments

All40 current fixtures execute expanded selected production sources. Native fixture builds are VC7.1/Od; host builds are C++98/O2. Worker did not run no-elide variants; only the root-landed rows carry root-reported no-elide PASS. E48A30/E338E0/D9EB30 use the actual CCharString header, which lacks an explicit copy constructor; do not infer extra no-elide portability from their ordinary passes.547500’s expanded fixture has a whitespace-only `0x12e - 4` fix for GCC preprocessing; selected production hash is unchanged.

Stale nonsemantic comments remain in6F99B0 (old address/length), D9EB30 (copied TollBandit introduction), and DF4610 line3 (copied prior owner). Inventory/oracle/PDB evidence overrides those comments. Do not spend grades merely to update comments unless source-content grading rules explicitly require it.

Root validated eight oracle boundaries without consuming grades:691AE0=278,689130=271,9E4470=270,BDFA10=38,BE37E0=287,6F9430=146,81E810=247,418DCA=294. Proof/original rows are root-owned `root_native_review/chunk240_boundaries`; this lane retains corrected chunk.tsv and BOUNDARY_EVIDENCE.txt.418DCA ends RET8 before the distinct418EF0 setter.

## Per-row index

Full hashes and commands are in the TSV/JSONL ledgers. This compact table is navigation only.

| Address | Status | Best verdict | Local+prior | Remaining | Source | Fixture |
|---|---|---|---|---|---|---|
| 00691ae0 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00691ae0.cpp | test/00691ae0.cpp |
| 00688a80 | NEAR_MATCH | DIFFER(289v285) | 3+0 | 7 | differ/00688a80.cpp | test/00688a80.cpp |
| 00689130 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 4+0 | 6 | src/00689130.cpp | test/00689130.cpp |
| 005bec92 | OTHER_DIFFER | DIFFER(325v313) | 5+0 | 5 | differ/005bec92.cpp | test/005bec92.cpp |
| 009e4470 | ROOT_LANDED | RELOCATION_MATCH | 5+0 | 5 | src/009e4470.cpp | test/009e4470.cpp |
| 00cdf260 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 4+0 | 6 | src/00cdf260.cpp | test/00cdf260.cpp |
| 00df4610 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00df4610.cpp | test/00df4610.cpp |
| 00cf8970 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 1+0 | 9 | src/00cf8970.cpp | test/00cf8970.cpp |
| 00cf9210 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00cf9210.cpp | test/00cf9210.cpp |
| 00ce0150 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00ce0150.cpp | test/00ce0150.cpp |
| 00bdfa10 | ROOT_LANDED | RELOCATION_MATCH | 5+0 | 5 | src/00bdfa10.cpp | test/00bdfa10.cpp |
| 00be5e70 | CAPPED_DIFFER | DIFFER(302v302) | 10+0 | 0 | differ/00be5e70.cpp | test/00be5e70.cpp |
| 00be6090 | ROOT_LANDED | RELOCATION_MATCH | 2+0 | 8 | src/00be6090.cpp | test/00be6090.cpp |
| 00bf67f0 | ROOT_LANDED | MATCH | 4+0 | 6 | src/00bf67f0.cpp | test/00bf67f0.cpp |
| 00be37e0 | NEAR_MATCH | DIFFER(285v287) | 6+0 | 4 | differ/00be37e0.cpp | test/00be37e0.cpp |
| 0079fd90 | CAPPED_DIFFER | DIFFER(253v253) | 10+0 | 0 | differ/0079fd90.cpp | test/0079fd90.cpp |
| 007a0630 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 2+0 | 8 | src/007a0630.cpp | test/007a0630.cpp |
| 00a42980 | ROOT_LANDED | RELOCATION_MATCH | 4+0 | 6 | src/00a42980.cpp | test/00a42980.cpp |
| 00536a30 | OTHER_DIFFER | DIFFER(228v242) | 4+0 | 6 | differ/00536a30.cpp | test/00536a30.cpp |
| 0053b7f0 | NEAR_MATCH | DIFFER(302v302) | 4+0 | 6 | differ/0053b7f0.cpp | test/0053b7f0.cpp |
| 00547500 | CAPPED_DIFFER | DIFFER(241v241) | 10+0 | 0 | differ/00547500.cpp | test/00547500.cpp |
| 00bf9bc0 | ROOT_LANDED | MATCH | 4+0 | 6 | src/00bf9bc0.cpp | test/00bf9bc0.cpp |
| 00d9eb30 | NEAR_MATCH | DIFFER(288v288) | 7+0 | 3 | differ/00d9eb30.cpp | test/00d9eb30.cpp |
| 00e338e0 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 4+0 | 6 | src/00e338e0.cpp | test/00e338e0.cpp |
| 00e48a30 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00e48a30.cpp | test/00e48a30.cpp |
| 004164e0 | NEAR_MATCH | DIFFER(264v264) | 5+0 | 5 | differ/004164e0.cpp | test/004164e0.cpp |
| 00418dca | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 3+0 | 7 | src/00418dca.cpp | test/00418dca.cpp |
| 0041db84 | NEAR_MATCH | DIFFER(278v281) | 1+0 | 9 | differ/0041db84.cpp | test/0041db84.cpp |
| 006f9430 | ROOT_LANDED | RELOCATION_MATCH | 5+4 | 1 | src/006f9430.cpp | test/006f9430.cpp |
| 006f99b0 | ROOT_LANDED | RELOCATION_MATCH | 5+0 | 5 | src/006f99b0.cpp | test/006f99b0.cpp |
| 0081e810 | ROOT_LANDED | RELOCATION_MATCH | 3+0 | 7 | src/0081e810.cpp | test/0081e810.cpp |
| 0081f910 | CAPPED_DIFFER | DIFFER(257v263) | 10+0 | 0 | differ/0081f910.cpp | test/0081f910.cpp |
| 008f6fc0 | CAPPED_DIFFER | DIFFER(303v297) | 10+0 | 0 | differ/008f6fc0.cpp | test/008f6fc0.cpp |
| 00da4350 | WORKER_READY_AWAITING_ROOT | RELOCATION_MATCH | 2+0 | 8 | src/00da4350.cpp | test/00da4350.cpp |
| 00a9d480 | OTHER_DIFFER | DIFFER(221v243) | 4+0 | 6 | differ/00a9d480.cpp | test/00a9d480.cpp |
| 00a5cee0 | NEAR_MATCH | DIFFER(285v285) | 4+0 | 6 | differ/00a5cee0.cpp | test/00a5cee0.cpp |
| 00a662a0 | ROOT_LANDED | RELOCATION_MATCH | 4+0 | 6 | src/00a662a0.cpp | test/00a662a0.cpp |
| 00a66450 | ROOT_LANDED | RELOCATION_MATCH | 3+0 | 7 | src/00a66450.cpp | test/00a66450.cpp |
| 00a65720 | NEAR_MATCH | DIFFER(267v269) | 5+0 | 5 | differ/00a65720.cpp | test/00a65720.cpp |
| 00a65480 | NEAR_MATCH | DIFFER(265v265) | 5+0 | 5 | differ/00a65480.cpp | test/00a65480.cpp |

Root188 review correction:691AE0 g4 restores protected const IsEventButtonHeld and original EInputEventType spelling; default grade remains exact278 and host/VC fixtures pass. Prior freeze/ remains historical; current corrected immutable source/fixture are freeze_root188/00691ae0_{source,fixture}.cpp. ALL40_INVENTORY.tsv supersedes the compact row table for this correction.
