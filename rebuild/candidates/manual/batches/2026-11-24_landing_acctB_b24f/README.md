# Landing acctB_b24f (Account A, 2026-11-24): 0x00DEE2E0 NScript::CQ_SecretPassageScript::CMotherTorture::Main, staged by Account B

Owner ruling: B verifies and stages; A lands. B's row and notes: `pending/2026-11-24_acctB_b24/` (rows/00dee2e0.cpp, hdr/00dee2e0,
notes/00dee2e0/). Header: `engine/NScript_CQ_SecretPassageScript.h` gains B's add-only opt-in FABLE_RETAIL_CQ_SECRETPASSAGESCRIPT_CMOTHERTORTURE
(0 removed lines). hdrcheck2 printed `cpp-err`, so A rechecked the header's 5 includers directly: same grades with and without the
overlay, and their objects differ only in `.debug$S` (logs/includer_recheck_*.log).

- Grade: RELOCATION_MATCH PASS on A's tree with the overlay (`logs/verify.log`, strict headers) and again from the repo (`logs/recheck.log`).
- keypayload changed only the test's `struct CIsThingAlive` to `class` (as the PDB records it).
- A coordinator comment-only edit added the `// Pick-any:` note (condition 4 below).

## Pick-any: landed as debt under the identical-HEAD-copy exception (EXTRA_RULES 2026-11-24; first use 0x00ED8680, acctB_b24d)
The leaf calls `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` (25 bytes), NEQ to retail 0x006E7B80. All four conditions were checked on A's
objects:
1. **Same copy already landed.** `logs/movbase_vs_00d62480.txt`: OBJECT_EQ with landed 0x00D62480's copy (bytes, relocation symbols,
   COMDAT selection 2, characteristics).
2. **No fix on record.** The coordinator probed this row (`logs/probe_D.grade.log`, `probe_O.grade.log`): with the decl-only
   ~CCountedPointer<MovieBase> removed (NF343's D text), and with FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR, the
   function DIFFERs 3028 v 3102 in both.
3. **Nothing else.** Under both checkers with `--leaf` (`logs/pickany.log`, `logs/pk_all.log`), ~MovieBase is the only leaf-reached NEQ copy.
   ??1CIsThingAlive 0x00E241B0, ??_GCScriptThing 0x004AB170 and ??_GCBaseClass 0x0099A470 are not leaf-reached (existing debt).
4. **Booked.** The row carries a `// Pick-any:` note, and T3 nf340 0x006E7B80 lists 0x00DEE2E0 as an emitter.
