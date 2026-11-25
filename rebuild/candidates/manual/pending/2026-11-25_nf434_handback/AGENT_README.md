# NF434 hand-back (2026-11-25)

| batch | verdict | rows | what |
|---|---|---:|---|
| batches/g1o | ACCEPT | 7 | base default slots: 0x00CBD4E0 CScriptBase::OnPersist + 0x00CDEBB0/C0/D0/E0 CEntityScriptBase::Init/OnPersist/OnPredicateFail/OnInterrupted respelt from `?Hook@T@@...` (RENAMES.tsv); 0x00DC3EA0 / 0x00DCC000 ??_G rows ported onto engine/CQ_OpeningGraveyardSecretPassageScript.h (add-only CTORS opt-in + two static-assert pins), which removes the only pick-any emitters of those names (compiled census) |
| batches/s48a | ACCEPT, land after g1o | 29 | NF430 debt_header Alloc rows of the 0x48 shape onto 29 NEW class headers (gen/gen434.py) |

- Land **g1o first**: s48a's vftables are 0 WRONG only with g1o's 0x00CBD4E0 self (plain table: 58 WRONG, all slot 4 = that stand-in).
- No HOLD batch. RULINGS_NEEDED R-NF434-1 (g1o 0x00DC3EA0 unreferenced ??0CSecretPassageSkeleton pick-any copy; g1o and so s48a wait on it). NAME_FIXES.tsv: 1 (0x00DC2570 ctor spelling). T3.txt: the 73 unported debt rows grouped by shape, task 2 status, 0x0043B570, two debt lines found here.
- Resolved T3 lines: the nf432 held-rows line's 0x00CBD4E0 part (g1o). Not resolved: nf328 0x00DEF6A0.
- Method, gates, sources: NOTES.md; per row: batches/*/CHECKS.tsv; evidence/ per batch. Tools: tools/ (fwvt434, fwchk434, home434, slot0chk, hdrcheck434, ccl/tl, mut434 + NF432 copies), gen/ (gen434, meta434).
- Header changes: batches/<b>/hdr/engine only; orig/hdr_CQ_OpeningGraveyardSecretPassageScript.h at the root (equal to the repo copy at hand-back).
