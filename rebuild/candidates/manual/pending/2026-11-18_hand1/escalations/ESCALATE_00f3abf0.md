# ESCALATE 0x00F3ABF0 CRangeCompressor::CalcCompressionScript -- probe/f3_a.cpp DIFFER(605v605) register swap only

Constraint: with Ego_d's locals (no `left`; loop `count += CalcCompressionScript(..., block_size - n > 4 ? 4 : block_size - n)`)
every instruction matches retail except ebx<->ebp: retail this=ebp, strength-reduced `block_size - n` temp=ebx; ours the
reverse. c2prio: `this` prio 58 key 7, SR temp prio 56 key 24 (higher prio colours first and takes ebx). Retail needs
this <= 56 (tie -> temp by key) or temp >= 58: a -2 for `this` = one more depth-1 block (fill loop) or two depth-0 blocks in
which `this` is live without a reference. The draft's named `left` gives the right registers but a misplaced `mov ebx,edi`
(2 hunks) and Ego_d has no such local.
Measured, all 19 hunks or worse: ternary spellings, if/else (this 60) and switch fill loops, Ego_d bprel declaration order,
std::min (634), else-if, count in ==3 path, loop braces, CARRAY_INDEX_ASSERT dead arm.

Addendum (PRIOARGS=--blocks on the `left` draft, which has retail's registers): `left` reaches 60 v this 59 because its
init `left = block_size` sits in the pre-guard block (K=4, +8); the strength-reduced temp's init sits in the K=2 preheader
(+4). So retail's temp needs about +2 more there, or `this` -2.
