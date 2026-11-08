# Landing 2026-11-08o: chunk 865, eleven inline-budget parks landed under the owner's 2026-11-08 rule

All eleven are RELOCATION_MATCH + PASS: a read-only vland grade with a trimmed overlay, then `--land`. Hand-back: `pending/2026-11-08_linux_hand865/`.
Nine use only a per-row compiler flag and leave the source as plain C++. The flag goes into the catalog `CompilerFlags` and the row's
`// Flags:` line. Two need a scoped source device. Line 2 of every row is `// Inline-budget: ...` with the device and the matching band.

| Row (all NScript entity Mains) | Device |
|---|---|
| 0x00CF18D0 COakvaleFan::Main | flag /d2inlS15 (S15..S20) |
| 0x00CF2B70 CBowerstoneFan::Main | flag /d2inlS15 |
| 0x00CF3D20 CHookCoastFan::Main | flag /d2inlS15 |
| 0x00D067A0 CBCGameMaster::Main | flag /d2inlS15 (S15, S20) |
| 0x00D17F20 CBH_GangDeputy1::Main | flag /d2inlS15 |
| 0x00D19BD0 CBH_Hostage1::Main | flag /d2inlS15 + _MSC_VER-guarded __forceinline ScriptedThingBase::operator= |
| 0x00D22F30 CKG_Sheriff::Main | flag /d2inlS15 |
| 0x00DAC760 CNOVI_Guard::Main | flag /d2inlT72 (T63..T72) |
| 0x00E799F0 CEM_Rhodri::Main | inline_depth(0) round the ~CIsThingConscious definition (no flag reaches it) |
| 0x00E8EF80 CGhostGrannySon::Main | flag /d2inlT72 (T55..T72) |
| 0x00EF4B40 CTestPerson::Main | flag /d2inlT36 (T28..T36) |

- **One N per retail TU:** the three arena fans (S15); the BountyHunt TU (0x00D17F20, 0x00D19BD0 and the parked 0x00D18DF0: S15);
  NewOakValeIntro (T72). /d2inlS has a cliff at 15 for every 2.6-4.5 KB Main here (the default is S9).
- **Not landed:**
  - 0x0083F650 LHHttpHeaders::GetHeader matched with /d2inlS11, but the source redeclares its own STL (basic_string, vector, the
    iterator tags) instead of the shared stlport headers, so it is not clean, readable C++. It is back with the agent. Its catalog
    extent is also one byte short: VC7.1 owns the int3 after the trailing noreturn `call abort`, so the row is 443 bytes, not 442.
  - Parked 0x00D18DF0 (one jne displacement, a c2 tail-merge pick), 0x00531EC0, 0x00DAADD0 (no allowed device reaches their
    splits), 0x00E03F70 (x87 order), and 0x004CE1B0 / 0x00DC08C0 / 0x00E2EAC0 / 0x00CDDCB0 (register / frame / dead-store
    residues, not inline decisions).
- **Headers landed:** additive opt-ins; no removed lines; no landed source defines any new macro:
  - CQ_BountyHuntScript: GANGDEPUTY1 / HOSTAGE1 / HOSTAGE2;
  - CV_ExposeMayorScript: CEM_RHODRI;
  - CQ_BanditCampBossBattleScript: GAMEMASTER / HOSTAGE.
  hdrcheck2's default comparisons were vacuous (cpp-err) for these script headers, so the additive diffs were checked by hand.
- **Names:**
  - `ghidra_out/labels_landing_2026-11-08o.tsv`: GetHeader, and the NScript namespace on three parked rows.
  - Queue `land0811m`: TeleportData ctor 0x00D15140 (landed as CMainGameComponentInit) and CCountedPointer<ScriptedThingBase>::operator=
    0x008AB1E0, both CHECK.
  - vland's DELTA_NOTIONAL_ZERO v EPSILON (0x0129BA3C) is a data-symbol spelling: it stays as established (EXTRA_RULES).
  - Catalog sync restricted to this landing's addresses. `bindings_multi_check.py` prints 1: the NullCoord entry that was already there.
