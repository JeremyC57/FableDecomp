# NF202 lessons (one line each)
- Read a row's lock file before the first edit, even when it is the entry address itself: an entry address is not always in rows.txt (0x009F03B0 was NF199's).
- `FABLE_FASTCALL` comes from rebuild_abi.h, not fable_compiler.h: a header opt-in that uses it (FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE) fails C2146 without it.
- A pick-any check must compare relocation targets, not just bytes; when the owner row defines the body under a stand-in name, compare by body (tools/cmp2.py takes a symbol per object).
- Before porting a resize(n) / dtor-bearing temporary onto a shared header, check whether retail runs the temporary's destructor at all (0x00A98600 has none; the header emits ReleaseData).
