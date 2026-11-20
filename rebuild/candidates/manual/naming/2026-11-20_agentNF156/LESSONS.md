# NF156 lessons (one line each)
- Probe every REFS row once before anything else (tools/probe.py, ~4 min for 266 rows): 8 of 18 entries turned out done-already, and the WORKLIST GFGetIME line (0x00851860) was stale.
- Rows with no Leaf line grade the wrong function in a probe (DIFFER with a tiny object); objdump their probe objects for the target name instead (q6312's July rows still emit a global CFrameData spelling).
- A stand-in list member is often another class's member in disguise: the "CAction { ActionType; list<CActionParam*> }" of 0x0059DB82 is CDeletion (catalog + FableWin ctor names, slot 62 SetDeletion), so the list is list<CComponent*>.
- An out-of-line STLport list dtor needs FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE to bind ~list; without it the shared header binds ~_List_base (0x00531E90 form 1).
- When a list stand-in is dropped but the local CDeletion stays, VC7.1 keeps STLport's `_M_data(0)` store before the head malloc (the local escapes to a declaration-only copy ctor); porting CDeletion too (FABLE_RETAIL_CDELETION_INLINE) matched at once (0x00555390).
- A second-base slot-2 call on a list<CComponent*> element is CObserver::IsObservingEvent (+4 sub-object); the stand-ins called it IsInState(n).
- Before an in-place template respell, list EVERY callee whose mangling the class change touches (binds.py without a filter): 0x005959AB's deque respell fixed back() but would have minted new spellings at 0x0059AD35 / 0x0059AC77.
- A Bash call that launches vland must source env.sh itself; without FABLE_ROOT verify_and_land looks for the manifest under a Windows path and dies.
