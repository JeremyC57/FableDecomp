# Chunk 716 (NA059) -- 10 of 10 byte-exact (1 MATCH + 9 RELOCATION_MATCH), one opt-in branch (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS): NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::~CTeamSpawn 0x00DCF700 (compiler-generated; the TU's
??_G and ctor also match the landed 0x00DCF6E0 / 0x00DCD160; chunk 713's FABLE_RETAIL_CLEVELSCRIPTBASE), CActiveEntityScriptBase::
OnPredicateFail 0x00CE1090 (MATCH; slot 5 of all 121 CActiveEntityScript<T> vtables), the DirectShow strmbase COM members CEnumPins::AddRef
0x00CA7AE0, CEnumMediaTypes::AddRef 0x00CA7C10 and CMediaSample::AddRef 0x00CA9060 (near-twins told apart by m_cRef's offset and the QI's IID)
and CMediaPosition::GetIDsOfNames 0x00CAD2E0, and four dynamic initializers (`_$E1`): NPlayerGui VALUE_DISPLAY_DURATION 0x0121DF1C (an
unaligned start after the previous initializer's `ret`, as in Ego_r), LHNetworkLib::LSocket::GlobalList 0x0121F850, gIME 0x01219E90 and
GVersion 0x01219F40. Every name from both PDB name sets.

Headers (hdr/README.md): opt-in FABLE_RETAIL_CIME in fable_ime.h (PDB members, IMEUI_APPEARANCE, inline `CIME() { ResetToDefaultSettings(); }`
/ `~CIME() { Shutdown(); }`, the static members); default text unchanged under `cpp`. The four fable_ime.h includers rechecked 4/4
RELOCATION_MATCH + PASS (includer_recheck_grades.tsv).

Coordinator decisions: the data symbols VALUE_DISPLAY_DURATION and gIME land with external spellings (`const` / `static` would give the same
bytes but internal linkage; no PDB data dump settles it yet). The CCharString at 0x013B85E0 (landed 0x01219F20, "Unknown") is probably
GVersionString and is left unrenamed. Expected vland warnings: `_$E2` RELOC_CONFLICT / RELOC_NAME (per-TU `_$E` reuse plus the landed
placeholder thunk labels at 0x0122A200 / 0x012288B0, which these TUs now emit correctly).

Second spellings at landing (queued PRIORITY, NAME_FIXES_QUEUE.tsv c716): GVersion 0x013B85E4 bound as `?g_Name@@3QBDB` by 0x00416268,
VALUE_DISPLAY_DURATION 0x013BA860 as a g_fFramesPerSecond stand-in by 0x006472B2, the atexit-thunk stand-ins 0x0122A200 / 0x012288B0, the
global CTeamSpawn family (0x00DCF6E0, 0x00DCD160, 0x00DCD1D0, 0x00DCD1C0, 0x00DCD350), and 0x00405350's two landed sources. The agent's 26
NAME_FIXES rows are queued as c716; labels in ghidra_out/labels_chunk716_2026-10-27.tsv.
