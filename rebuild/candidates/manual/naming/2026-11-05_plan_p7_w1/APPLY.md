# Phase 7 hand-back: apply / regrade commands (in order)

Every wave directory is in the apply_w35c.py format. RENAMES.tsv has only its header line: every file is copied in place and
the ps1 is untouched. Apply the waves IN ORDER, because a later wave's text is built on the earlier waves' text:
 - w2 rewrote 0074e560 0077c490 0084db30 008534f0 0085bd70 on top of w1;
 - w3 rewrote 00938580 009415b0 009445f0 00944d10 00970790 on top of w1.
Each wave's NOTES.txt lists its overlap. Run from the repo root with the lane env sourced (`source $S/wine/env.sh`). The index at
`$S/plan/idx` (HEAD 248ea936) is the regrade baseline: do not refresh it before the regrade.

    H=<this hand-back directory>            # the committed copy of $P/handback
    A=rebuild/candidates/manual/naming/2026-11-04_wave35c/scripts/apply_w35c.py

    # 1. wave 1: old-spelling binders respelled + CScriptThing::NullCoord + CThing::PeekPlayer (header: CScriptThing.h)
    python3 $A $H/w1_respells dry           # 0 problems expected
    python3 $A $H/w1_respells               # copies overlay/rebuild/include/engine/CScriptThing.h and the 87 PASS rows
    #    BINDINGS.tsv ADD / RETIRE -> rebuild/oracles/reloc_bindings.tsv (KEEP lines: nothing to do; their binders are deferred)
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/apply_bindings.py $H/w1_respells/BINDINGS.tsv
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/bindings_multi_check.py       # must print 0
    #    QUEUE_LINES.tsv: 4451 / 5150 closed; 249 140 206 277 1659 1893 1979 2223 3902 4988 4223 6222 narrowed
    #    (check line_sha1_12 against the current queue before marking)

    # 2. wave 2: the CThing leftovers (header: CThing.h, FABLE_RETAIL_CTHING_MOREACCESS / _OPERATORNEW)
    python3 $A $H/w2_cthing dry
    python3 $A $H/w2_cthing                 # CThing.h + the 123 PASS rows (rewrites on w1 text for the 5 overlap files)
    python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/apply_bindings.py $H/w2_cthing/BINDINGS.tsv
    #    QUEUE_LINES.tsv: line 220 narrowed (??2CThing@@SIPAXI_N@Z retired; 006ac910 / 00835c80 placeholder stand-ins left)

    # 3. wave 3: the CIntelligentPointer leftovers (no header change: engine/CIntelligentPointer.h as at HEAD)
    python3 $A $H/w3_cintelligentpointer dry
    python3 $A $H/w3_cintelligentpointer    # the 388 PASS rows; no BINDINGS / QUEUE lines (Get / Set are inline: no symbol changes)

    # 4. headers: every changed header is behind a sub-macro
    S=$S python3 tools/decomp_pipeline/linux_lane/lane_tools/coordinator/hdrcheck2.py rebuild/include   # expect default=same

    # 5. regrade from the working tree. Gate + vland --recheck-landed (read-only) on:
    #    - every PASS row of the three waves;
    #    - every includer of CScriptThing.h and CThing.h.
    #    $H/tools/includers.py rebuild/include/engine/CScriptThing.h rebuild/include/engine/CThing.h > inc.lst
    #    (expected from the agent's run: 4754 / 4754 and 1979 / 1979 PASS, the 4 failed w2 rewrites being deferred and their
    #    landed text PASS: w1_respells/logs/gate_includers_CScriptThing.tsv, w2_cthing/logs/gate_includers_CThing*.tsv)
    NJ=2 python3 $H/tools/gatewave.py <W> @<PASS addresses>          # or $S/plan/gate.py directly
    python3 $H/tools/vlandwave.py <W> @<PASS addresses> --tag <unique>  # at most 300 a batch, a unique --tag per concurrent run

    # 6. queue: $H/idxpost/QUEUE_STATUS.tsv (from the HEAD 248ea936 index, before the apply)
    python3 $S/plan/queue_mark.py $H/idxpost/QUEUE_STATUS.tsv       # marks the `satisfied` lines; read the diff first

    # 7. after landing: refresh the index (symindex2.py) and re-run $H/tools/idxpost.py for phase 8
