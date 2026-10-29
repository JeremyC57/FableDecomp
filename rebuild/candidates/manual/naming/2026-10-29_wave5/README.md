# Naming pass wave 5 (2026-10-29): 408 rows respelled, 408 of 408 recheck RELOCATION_MATCH PASS

| Family | Rows | Script |
|---|---|---|
| LowerBound, CVectorMap STLport spelling (stand-in models under other class names) | 377 | scripts/fam_lb5.py |
| LowerBound, `int*`-entry macro rows 0x0042C43F..0x0042C8F4 | 17 | scripts/fam_lbint5.py |
| CFrontEndManager stand-ins -> NUISystem::CManager::GetInstance (0053eda0, 0054ecb0) | 2 | scripts/fam_uimgr5.py |
| CBaseClassNonCopyable protected ctor + virtual dtor (00a62120) | 1 | scripts/hand/cb5.py |
| CDataInputStream::ReadWithSrcChunkOverflow private + friend (0046bf1c, 004793f8) | 2 | scripts/hand/rw5.py |
| CVectorMap template rows (005943a2, 008dd6e0, 00896df0) | 3 | scripts/hand/cvt5.py |
| fable_string.h users: GetAsCharArray, Sprintf, FindCharPos, SetLength (0041a430, 0055cfb0, 0055e6b0) | 3 | scripts/hand/fs5.py |
| Own identities: 0x00404280 / 0x0040F0A0 STLport `std::operator==` (G form), 0x00993F00 `CDataOutputStream::WriteNullTerminatedWString(const CWideString&)` | 3 | scripts/hand/own5.py |

Header: `rebuild/include/fable_string.h` (fable_string.h.diff) gains CWideString::GetAsCharArray (0x0099AF10),
FindCharPos (0x0099B070), Sprintf (0x0099BA70), SetLength (0x0099BC50); AllocStringData (0x0099B3C0) moves to protected
(PDB `IAE`). All 1,131 includer rows with catalog bytes were rechecked by the agent with the new header: the same 1,127
pass as with the old header (4 fail identically at baseline: 0040d2a0, 0040d400, 004d9861, 009e1440); the only relocation
change is AllocStringData `AAE` -> `IAE` in 0099b6b0 / 0099b800.

Renames (RENAMES.tsv): 0x00404280 and 0x0040F0A0 source/test files become `std_operator_*`, Module `_global` -> `std`.
Catalog names corrected (rebuild/oracles/auto-re-candidates.tsv): 00404280, 0040f0a0, 0041e5f2
(NUISystem::CManager::GetInstance; the CFrontEndManager getter is 0x00595582), 0099a300
(CBaseClassNonCopyable::~CBaseClassNonCopyable).

Bindings (BINDINGS.tsv): 54 retired, 3 added (AllocStringData IAE, the 0x0040F0A0 two-string operator==,
WriteNullTerminatedWString). Left (DEFERRED.tsv, 122 rows with reasons; DEFERRED_lb.tsv the 56 LowerBound shapes).
recheck.log is the coordinator's full recheck of the 408 rows after applying.
