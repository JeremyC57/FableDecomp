# Original vendor identity

Original zlib 1.2.1 deflate.c deflate_slow, not the automatic manifest label. The function address occurs in configuration_table[4..9].func, matching the six slow-level tuples in original zlib source. The native algorithm includes previous-match laziness, max_lazy_match, Z_FILTERED/TOO_FAR suppression and matched literal/distance tally layouts. The embedded library version is 1.2.1 and neighboring deflate_fast is already matched from this release.

Supporting names and original structures come directly from zlib 1.2.1 deflate.c/deflate.h/zlib.h. Existing bindings checked before declaration: _fill_window=00c01090, _longest_match=00c00e60, _longest_match_fast=00c00fd0, _flush_pending=00c00630, __tr_flush_block=00c057d0, __length_code=012af590, __dist_code=012af390. Same-TU helper definitions are original zlib functions, not synthetic adapters; static private register conventions are selected by VC7.1.

The C compilation mode matches the published deflate.c translation unit and neighboring deflate_fast; external linkage of the exported reconstruction retains the exact C symbol while allowing leaf extraction.
