# Phase 6 hand-back: apply / regrade commands (in order)

Every wave directory is in the apply_w35c.py format (RENAMES.tsv header-only: every file is copied in place; the ps1 is untouched).
From the repo root, lane env sourced (`source $S/wine/env.sh`), index at `$S/plan/idx` (HEAD b7d0361, do not refresh before the regrade):

    H=<this hand-back directory>            # the committed copy of $P/handback
    A=rebuild/candidates/manual/naming/2026-11-04_wave35c/scripts/apply_w35c.py

    # 1. wave 1: the 0x0041E5F2 GetInstance respell (27 rows, no header)
    python3 $A $H/w1_getinstance dry        # 0 problems expected
    python3 $A $H/w1_getinstance
    #    LABELS.tsv -> ghidra_out/labels_*.tsv (0041d198 NUISystem::CManager::SetMeshBank); BINDINGS.tsv RETIRE lines -> reloc_bindings.tsv
    #    QUEUE_LINES.tsv: lines 62 / 568 / 579 / 701 closed, 649 closed_wrong (queue_mark.py marks only `closed`; 649 by hand)

    # 2. wave 2: the CThing header wave (header + rows)
    python3 $A $H/w2_cthing dry
    python3 $A $H/w2_cthing                  # copies overlay/rebuild/include/engine/CThing.h and every PASS row in place
    #    BINDINGS.tsv ADD / RETIRE -> reloc_bindings.tsv; CALLERS.tsv: the old-spelling binders left (OK(pdb) targets) for the respell
    S=$S python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/hdrcheck2.py rebuild/include   # expect default=same

    # 3. regrade (both waves) from the working tree: gate + vland --recheck-landed on every PASS row and every includer
    NJ=2 python3 $H/tools/gatewave.py <W> @<PASS addresses>        # or $S/plan/gate.py directly, GATE_BASELINE=landed
    python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py <payload> <oracle> --recheck-landed   # payload: lane_tools/recheck.py

    # 4. the index post files (from the index, before the apply): $H/idxpost/MULTINAME.tsv, QUEUE_STATUS.tsv
    python3 $S/plan/queue_mark.py $H/idxpost/QUEUE_STATUS.tsv     # marks the `satisfied` lines; read the diff first

    # 5. after the landing: refresh the index (symindex2.py) and re-run $H/tools/idxpost.py for phase 7
