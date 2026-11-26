# 0x006C9350 CPersistTraits<std::pair<long, COpinionDeed*>>::TransferIn

Landed 2026-11-26 on FullDecomp-B under VC7.1 RTM cl 13.10.3077. Grade RELOCATION_MATCH, behaviour test `006c9350_TEST PASS`. Retail extent 312 bytes.

PDB spelling: `?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z` (Ego_r donor 0x00700120; FableWin has the same symbol). The manifest still says `CPersistTraits<CAtmosBankEntry>::TransferIn`. That respell is queued in `NAME_FIXES.tsv` for Account A. The unmatched length estimate was 320.

`Equals` and `IsEqualSlow` are descriptive local helpers. Neither PDB has those spellings (`CCharString::Equals` there takes a `CCharString&`). They exist so the comma test calls `CBasicString<char>::operator==` at 0x004115A0 while the parenthesis tests inline `Compare` at 0x00411570. The verifier warns that `CStringParser` and `CPersistTraits` have shared headers; this spelling keeps the local helpers that produce that split.

The 2026-10-30 name-review draft and the 2026-11-10 draft are unchanged.
