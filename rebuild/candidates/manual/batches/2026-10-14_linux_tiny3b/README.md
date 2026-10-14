# Batch `2026-10-14_linux_tiny3b` — family tiny3 (≤16-byte bodies, `wtiny3`, second pass): 11 bodies (7 landed)

The bodies that differed in `2026-10-14_linux_tiny3`, regenerated after the template fixes: the
`== 1` byte predicates (`vfunc_19` of four CThing classes, an `unsigned char` result) and the
flag-setting `BeginWith` bodies of the tavern games (assignment order). The four `$E` dynamic
initialisers that store one constant into a global (`mov dword ptr [g], imm ; ret`) do not
reproduce: VC7.1 folds every inline constructor, const-reference binding or static-const read into
static initialisation (no `??__E` is emitted at all), so these bodies are link-time inlining of an
out-of-line constructor -- parked in `docs/journal/FINDINGS_LOG.md`.

All landed bodies graded `MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
