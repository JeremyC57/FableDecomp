# 2026-11-11_acctB_c2017 -- acctB hand-back, chunk 2017 (3 reserved rows: 2 matched, 1 not attempted)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00951430 CAIStateGroup_JackDragon::IntoGroundSwoop | RELOCATION_MATCH 1765v1765, test PASS | 3 grades. The landed DragonBurninateInto constructor 0x009537B0 is defined in the TU (visible, noinline) |
| 0x00974110 CAIStateGroup_JackOfBladesBase::Summon(long, bool) | RELOCATION_MATCH 1631v1631, test PASS | 4 grades. REVIEW: the 2D distance is written out as `GFFastSqrt(v.X*v.X + v.Y*v.Y)`; the shared C3DVector::GetMagnitude2D two-statement body gives 3 different instructions (the header's CTCTARGETINGAI_ATTACK branch has the one-expression body) -- family check wanted |
| 0x0092DA20 CAIStateGroup_MinionSleep::Update | not attempted | outline in NOTES.md; re-queued by acctB as chunk 2026 |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; label-only RELOC_NAME warnings on
0x008AAC50 / 0x008AB710). Purity and padded-struct greps are clean.

To land: the rows need `hdr/` (2 new headers, 8 new opt-ins, c2009's JackDragon / CTCBoss overlay copies; `hdr/README.md`). A
row must include engine/CAIStateGroupBase.h first so the overlay copy is the one used. Rebuild the oracle from Fable.exe (no
retail bytes are in this folder). NAME_FIXES.tsv: 7 entries (e.g. 0x0125916C is the float literal 8.0f, landed 0x00977820
names it as an invented global `g_MinTeleportMarkerDistance`; 0x00A14040 is RotateXY, bound as RotateAboutZ).
