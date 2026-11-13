# 2026-11-11_acctB_c2013 -- acctB hand-back, chunk 2013 (4 reserved rows, all matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00597DF7 NUISystem::CFrontEndManager::GetSavedGameNames | RELOCATION_MATCH 1644v1644, test PASS | size TU; 8 grades |
| 0x00595CC1 NUISystem::CFrontEndManager::ConstructFileDescription | RELOCATION_MATCH 1613v1613, test PASS | size TU, `// Flags: /GS`; 9 grades; CIVCountedPointer destructor forced inline (`_DTOR_DIRECT_CLEAR` opt-in) |
| 0x00626110 CTCQuickAccessMenu::ContructItemLists (std::map overload) | RELOCATION_MATCH 1596v1596, test PASS | `// Flags: /d2inlT59` + `// Inline-budget:` line (see REVIEW) |
| 0x00626750 CTCQuickAccessMenu::ContructItemLists (vector<pair<long,long>> overload) | RELOCATION_MATCH 1771v1771, test PASS | `// Flags: /d2inlT59` + `// Inline-budget:` line (see REVIEW) |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 4 wins, no RELOC_IDENTITY). Warnings: RELOC_CONFLICT on
`??1?$CCountedPointer@VCComponent@NUISystem@@` -> 0x00429C53 here against 0x004291DE in landed 0x00570332 (0x004291DE is
ReleaseData, the dtor is the jmp stub 0x00429C53 -- the 2026-11-10 agentNF finding; NAME_FIXES.tsv), plus label-only
RELOC_NAME and import-thunk RELOC_ALIAS lines. Purity and padded-struct greps are clean.

REVIEW at landing: the two QuickAccess rows need an inline-budget device because retail expands the icon's
`CCountedPointer(T*)` only in the LEFT arm and calls `~CCountedPointer` only in the DOWN arm. Handed back with
`/d2inlT59` (smallest working value). The agent reports a flag-free alternative that also matches: a folded `&& true` in the
loop condition (its probe sources are in `alt/`, NOT re-verified by the coordinator). Pick one device.

To land: the rows need `hdr/` (opt-in blocks; one `#ifdef` -> `#elif` in CIVCountedPointer.h; new engine/CXBConfig.h;
fable_string.h `FABLE_RETAIL_CWIDESTRING_NE` overlaid as a whole file with an unchanged fable_definition_table.h;
`hdr/README.md`). CFrontEndManager.h / CUserProfileManager.h sit on top of c2003's overlay copies: merge c2003 first. The
landed same-TU rows 0x00626E40 and 0x006276F0 were not regraded against the new CAction opt-in (they do not define it).
Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 5 entries.
