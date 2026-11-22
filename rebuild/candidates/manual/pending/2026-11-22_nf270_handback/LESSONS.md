# NF270 lessons (one line each)
- A port of a row-local model onto a sibling's shared-header define set (same class, landed) matched on the first form; copy the sibling's defines and includes verbatim before trying anything else.
- A copy constructor on the shared headers needs FABLE_RETAIL_CINTELLIGENTPOINTER_COPY for each intelligent-pointer member; without it VC7.1 emits the implicit member-wise copy (DIFFER, 8 bytes short).
- Porting onto a class header can change slots other than the one asked for (here Clone UBE -> the PDB EBE): vtmap every slot HEAD v new, then census the moved slot's address too.
- The q.py SKIPS / KEEP control objects plus a link with an unresolved-symbol list (tools/lnk2.py) show a slot resolving to its owner in one step; extern "C" names print unquoted in LNK2001 lines, so parse both forms.
- pin_asserts.py rewrites existing plain FABLE_STATIC_ASSERT lines below an insertion: that is a header edit to every includer's text, so the whole-tree equiv_check must run on the pinned file.
- Precheck the HEAD tests (not only the sources) before any vland that runs them; clang rejects some VC7.1 SDK headers (<functional>, <cwchar>), which is a clang limit, not a missing include.
- A DECL_ONLY mirror adds an undefined external to the row object, so a test that links the row and stubs its externals by /alternatename LINK_FAILs: add one probe for the new external.
- For a pick-any definer census, only VC7.1 objects count, and objdump -dr shows which COMDAT carries the reference (here the 5-byte ~map jump), which settles whether retail could have an owner.
- A blocking pidfd select on a job's PID (os.pidfd_open + select) waits without sleep; run it as a background call so the wait is not a foreground block.
