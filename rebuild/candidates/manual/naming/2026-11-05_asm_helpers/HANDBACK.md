# Asm cleanup hand-back (2026-11-05; rules: docs/pipeline/ASM_RULES.md)
P = $S/plan/asm. No repo edits, no commits.

| wave | rows | grades (vland, = landed) | helpers |
|---|---|---|---|
| w1 plain C++ | 0 | - | - |
| w2 intrinsics | 0 | - | - |
| w3 existing helpers | 21 | 3 MATCH + 18 RELOCATION_MATCH, behaviour PASS, purity retail_asm | libpk/FtoL.h (+LIBPK_FTOL), engine/GFGetCycleCount.h (+GF_GET_CYCLE_COUNT) |
| w4 new helpers | 76 | 26 MATCH + 50 RELOCATION_MATCH, behaviour PASS, purity retail_asm | 15 new headers (w4/NOTES.md) |
| w5 bakes | 0 | - | - |
DEFERRED.tsv: 2 retail_asm lighting kernels (rsqrtps / rcpps twin needs an owner decision) + the 14 bakes.

Per wave: overlay/, NEW_FILES.txt, PASS.tsv, REGRADE_ROWS.txt, ATTEMPTS.tsv (per site), ATTEMPTS_FAMILIES.tsv (each spelling
compiled, flags, VC7.1 output), NOTES.md. Probe outputs: attempts/probe_*.txt.

Apply, in order (repo root, lane env sourced):
    python3 $S/plan/asm/apply_asm.py $S/plan/asm/w3        # 39 in-place files
    bash    $S/plan/asm/regrade_asm.sh w3
    python3 $S/plan/asm/apply_asm.py $S/plan/asm/w4        # 113 in-place + 31 new files
    bash    $S/plan/asm/regrade_asm.sh w4
    # after both land: the purity change
    patch -p1 < $S/plan/asm/purity_no_inline_asm.patch
No build_candidates.ps1 change (paths, symbols, addresses unchanged), so apply_w35c.py is not needed.

Purity patch: classify() grades any row spelling __asm / naked as naked_asm (the evidence rule stays for helper headers).
After w3 + w4 it changes exactly two grades: 0x00B44820 / 0x00B44A60 retail_asm -> naked_asm (PURITY_STILL_REJECTED.tsv also
lists the 14 bakes and 0x00890750, already non-genuine: its comment says "no naked").
