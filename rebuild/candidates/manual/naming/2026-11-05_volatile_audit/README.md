# volatile audit (2026-11-05): did retail actually use it?

## Method
For each of the 16 landed rows with `volatile` in code, the keyword was removed and the row regraded with vland
`--recheck-landed`. The payload was rebuilt after the edit, because `mkpay.py` embeds the source text. The edits were then reverted.
Results are in `VOLATILE_TEST.tsv` and `vland_without_volatile.log`.

## Result
- **Retail depends on `volatile` in 8 rows.** Without it the bytes change, because VC7.1 then drops reads that retail keeps:
  - 0x00CACD20 `CUnknown::NonDelegatingRelease`, which re-reads `m_cRef` after `InterlockedDecrement`. The DirectShow BaseClasses
    source declares `volatile LONG m_cRef` (combase.h).
  - 7 CFileInstaller rows, whose cross-thread flags are re-read on every access: 0x009C3590, 0x009C31E0, 0x009C42F0, 0x009C3250,
    0x009C3660, 0x009C5690, 0x009C4F20.
- **No byte effect in 7 rows:**
  - 6 strmbase / Interlocked rows that only declare the same `m_cRef` field or the SDK `LONG volatile*` prototypes;
  - 0x0059A238 `CFrontEndManager::Action`, a `void* volatile local_10` trick. The trick is removed: still RELOCATION_MATCH PASS,
    purity `genuine`.
- **0x00CAC300** is an EH funclet row. This harness cannot regrade it, because `mkpay.py` drops `funclet_offset`.

`volatile` is standard, portable C++, not asm. The question for each row is only whether retail had it.
