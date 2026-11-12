## 0x009F20B0

- "Zero constant in ebx vs immediates" (0x009F20B0): the zero's fate was decided two loops later -- a duplicated tail (`store; break;` in both arms of an if/else, merged by VC7.1 after allocation) gave `size` one more weighted reference block than `a`, `a` took ebx, the constant was split early and its early piece outranked the parameter. Write shared tails out per arm before touching the zero's own references.
- c2prio `--trace` on a deliberately hacked variant (one extra reference to the value retail favours) shows in one probe whether a far-away priority flip explains a residue; then look for the natural spelling that adds that reference.
- A strength-reduced record pointer's base offset follows the field access spelling: `g_Table[index].Field` through an index-taking inline accessor moved loop 2's base from +0x14 to +0; a bool local read through the record reference kept retail's base.

