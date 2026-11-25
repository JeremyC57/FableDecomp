# Landing nf405 b2: 0x004DBE39 is `??_G?$pair<const unsigned __int64, CGroupMemberData>`

The row is re-rowed in the nf86 / nf88 pick-any form: a helper calls the destructor through a typedef, and `#pragma optimize("s",on)` stays in the source. Its self now agrees with the table line nf398_b1 bound here, so the table needs no change. The row grades RELOCATION_MATCH PASS.

The extra COMDATs are all tier 1. They equal the copies in 0x004DFB8F, 0x00813460 and 0x00813010.

No table line or caller uses the old self, `?vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z`. It is still the strong Leaf of 0x004DA6A9, 0x004E41A1, 0x005C1390 and 0x00656EAD, which is queued in T3 nf405. This landing lowers the /FORCE duplicate count by one (10 -> 9).

Coordinator: 1/1 landed, 0 LITERAL_DIFF, the self line added with column 4 aligned, T3 nf398 0x004DBE39 closed, baselines OK.
