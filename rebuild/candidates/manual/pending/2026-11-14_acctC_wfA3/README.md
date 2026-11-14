# acctC workflow hand-back wfA3 (unworked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00E2F170 | CV_AmbushScamScript::SpringAmbush | 2,781 | RELOCATION_MATCH | PASS |
| 0x004A0D90 | CWorld::LoadQuestsFromFile | 2,733 | RELOCATION_MATCH | PASS |
| 0x00DADF80 | NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main | 2,787 | RELOCATION_MATCH (`/d2inlT65`) | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`).
REVIEW ITEMS:
- 0x00DADF80 carries `// Flags: /d2inlT65` with an `// Inline-budget:` line (owner decision 2026-11-08, preference 2): one
  implicit destructor at the hit block's terminate check is expanded at the default threshold and called in retail. It
  matches for T51..T65; the author chose 65 (inside the landed same-TU sibling CNOVI_Guard::Main's range T63..T72) where the
  rule says "smallest N" (51): the lander picks. `long count` is FITTED.
- 0x004A0D90: WARN RELOC_CONFLICT — `vector<char>::vector(unsigned)` binds to 0x00410F30 here and to 0x00411910 in landed
  0x009EC890 (two byte-identical COMDAT copies): decide whether both may carry the name.
`open/`: `00acdb80_LOCAL_CLASS.cpp` COptimisedPrimitive ctor — bytes match (2,674) and the test passes, but the source
defines its own `QuadricOptimiserInternals::COptimisedPrimitive` although engine/COptimisedPrimitive.h has that branch
(`FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE`): NOT staged as matched until respelt on the shared header.
00981410 (real class CAIStateGroupBase; needs a shared-header port).
