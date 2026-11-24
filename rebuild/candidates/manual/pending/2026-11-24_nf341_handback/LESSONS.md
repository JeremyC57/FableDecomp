# NF341 lessons
- A GetDef<T> / GetSubDef<T> pick-any copy that differs from retail by a cached source pointer (`mov esi,eax`) plus a second null test before
  the temporary's release is the CDefPointer SetDynamicCast form, not the GetDef text: retail's release SetDynamicCast re-reads the source
  (FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD). The pointee's Release variant (default / SELFDELETING_POINTEE / RELEASE_DELETES /
  ORIGINAL_SCOPES_PASS22) makes no difference to that copy.
- Probe a header text with a small TU (the row's own defines + includes, then one caller of the template instance) before touching the
  header: one compile per variant, the copy compared with pickany.py. Keep the HEAD-text probe as the negative control.
- B's leaf-copy count needs string-literal relocations handled: a GetSubDef copy relocates its own `??_C@` type-name literal; compare the
  literal text at the retail target instead of looking the literal up in reloc_bindings.tsv (tools/leafcopies.py).
- hdrcheck2 defines one macro at a time, so a block nested under two macros (GETDEF_GLOBAL_BODY needs FABLE_RETAIL_CDEFINITIONMANAGER)
  compares as `=same` vacuously; the equiv_check over real row configurations is the proof there.
- reloc_bindings.tsv had no line for 0x006EA430 at all: the binder came from an exe scan (tools/xscan.py), not from the table.
- A header specialization meant for one TU family must skip the TUs that already get retail's form another way: the first text dropped an
  unreferenced helper COMDAT from a SETDYNAMICCAST_REREAD TU whose copy was already EQ. Guard the addition with `#ifndef <that opt-in>`.
- objsec across a header change must key COMDAT sections by their symbol, not by section index: one dropped COMDAT shifts every later index.
- pickany_vs_retail.py marks only NEQ copies LEAF-REFERENCED, so a HEAD-v-new comparison of "leaf-called" must take the flag from either
  side (tools/pkcmp.py); run it once with the sides swapped as a planted control.
- vland.py needs tools/decomp_pipeline on PYTHONPATH (run from the repo root, or set PYTHONPATH); FABLE_HDR_OVERLAY reaches it through
  verify_and_land.
- pass.py writes its own pass_<tag>.log; a launch shell that does not source wine/env.sh fails with cl.exe "not found" (D:\ path).
