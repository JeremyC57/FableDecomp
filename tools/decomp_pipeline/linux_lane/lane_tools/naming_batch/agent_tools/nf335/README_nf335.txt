NF335 tools (2026-11-24): NF330's agent_tools/nf330 copied (paths retargeted to agentNF335; README.txt / README_nf329.txt are theirs) plus:
flatten_pilot.py  v6: --own-tests (rows whose own test calls the flat name move WITH the test: `owntest` in GEN notes), the moved test
                  <work>/tests/<addr>_test.cpp prechecked in gen, STALE TEST / NO MOVED TEST refusals and the moved test on the new side in
                  grade, a control vland per chunk (HEAD source + moved test must not PASS), export tests/ + orig/tests/; gen prerequisite
                  `class X;` for a header that names a class the row models locally.
testmove.py       the test rewriter (usage in its docstring): flat declaration -> declaration-only call model, calls -> qualified member
                  calls through a cast pointer, data model renamed <Class>Layout (code only), protected -> <Class>_TestCaller friend,
                  literal `this` -> zero-initialised static storage; MANUAL for stubs naming the class and Class:: members.
testgate.py       text + object gates on moved tests (flat name gone, PassPattern, call count, no member body, keys; UND PDB symbol, no
                  flat symbol, no ??_7 / ??_R in the test object).
assemble_nf335.py STATUS / APPLIED / RENAMES / REJECTED / NAME_FIXES / TABLE_CHANGES / rows / rows.txt from the triage, work/skip_reasons.tsv
                  and batches/b*.
verify_nf335.py   rows/ == graded gen source and moved test; landed source and test unchanged since the precheck; locks; orig/tests current.
lockall.sh        retargeted to NF335.
