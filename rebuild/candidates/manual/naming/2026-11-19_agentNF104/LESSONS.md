# NF104 lessons

- Read the evidence column of a queue line, not just correct_symbol: in five entries the real claim was there (q6539 even contradicts
  its correct_symbol column).
- reloc_bindings.tsv keeps one witness row per symbol; probing every REFS row once (work/probe.tsv) made each entry's state a lookup.
- Before writing a strong definition of an inline-able member, grep the shared header for an inline body under an opt-in; if one
  exists use the inline_depth(0) scaffolding form (0x004371A0).
- STLport out-of-line members: the fable_containers.h opt-ins (LIST_INSERT_OUTOFLINE + LIST_PUSHBACK_NOINLINE, VECTOR_RESIZE_OUTOFLINE,
  VECTOR_RESIZE_ONE_DECL) reproduce retail call-vs-inline choices exactly; a local "noinline stand-in" in a binder is usually one of them.
- A test that links the row breaks when the row's symbols change; check every test for "calls a function it does not define" before
  vland. A test linking an explicit STLport instantiation must also supply the out-of-line helpers (_M_fill_insert) the row keeps
  declared only.
- After any overlay edit run pin_asserts.py, then regrade every row that uses the overlay.
- Wine/lane tools can create Windows-path directories relative to the cwd; run them from the repo root and check the workspace after.
