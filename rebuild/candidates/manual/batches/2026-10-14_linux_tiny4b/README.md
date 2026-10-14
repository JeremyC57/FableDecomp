# Batch `2026-10-14_linux_tiny4b` — family tiny4 (≤16-byte bodies, `wtiny4`, second pass): 22 bodies (15 landed)

The bodies that failed in `2026-10-14_linux_tiny4`, regenerated after the template fixes (unique
pad names, constructor labels not used as method names, `scale * value` FPU order, the returned
result of a one-slot `__cdecl` call). The seven that still differ are parked in
`docs/journal/FINDINGS_LOG.md`: one `$E` constant initialiser (link-time inlining), three
sibling-call wrappers the DDK compiler tail-jumps, two global-scaled FPU products whose operand
order the swap did not change (`vfunc_18`), and two `xor eax,eax ; mov ax,[ecx+N]` zero-extensions
(the DDK compiler emits `movzx`).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` in the Linux lane (Wine + DDK 13.10.4035 cl.exe) on 2026-10-14.
