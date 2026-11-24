# NF338 lessons
- A header opt-in must not move any line above a plain FABLE_STATIC_ASSERT: its typedef is named after __LINE__, equiv_check flags it
  (DIFF in every includer that sees that assert), and rebuild_abi.h notes stack-slot order can depend on those names. Put the add-only
  block after the last plain assert, or use FABLE_STATIC_ASSERT_AT. The same holds for a row with its own plain asserts: keep the row
  edit line-neutral (join comment lines).
- A "census FREE stand-in" T3 reason is often not an obstacle: when the binder already models the PDB class locally (or a shared header
  has an opt-in block for it), the PDB static is one declaration away (entries 62, 223 / 224).
- ref_scan's identifier census cannot handle a decorated C name (@Name@8) or a common class identifier (CCharString): it reports
  TOO_MANY; a grep for the exact stand-in identifier is the census then.
- land_scripted holds an accepted row named by any open COORDINATION_CLAIMS line, including a stale `claimed` line of an old landed
  hand-back (0x006116E0, line 197): check the claims file before handing back a unit.
- --release-t3 closes only lines whose column 2 is an accepted row; for binder respells the T3 line's column 2 is the entry target, so
  the hand-back must list the lines to close.
