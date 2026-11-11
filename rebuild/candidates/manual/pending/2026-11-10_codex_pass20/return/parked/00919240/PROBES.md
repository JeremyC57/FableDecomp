# Source strategies and immutable history

1. s01_full_owner_original_collector/s01b_original_mapwho_forward: full original24-byte owner,CTCInterestingToVillagers16,CInterestingToVillagersDef44 and actual vector<CThingGameObject*>. Real original rough-area collector body replaced old capture-only sink. Missing CTCMapwho forward corrected before successful compile.510bytes,164instructions; original GetCreature body absent due include order.
2. s02_visible_original_getcreature: include genuine state-group base before CAIBrain so existing actual PBrain->GetCreature body becomes visible.508bytes,162/164instructions equal (98.7805%). Every old weighting-loop value/size/pointer/sar difference fixed. Only entry schedule remains. Selected stable simplest source.
3. s03 named original filter before vector: same residue. Original constructor/type retained.
4. s04 original filter constructor body assignment rather than member initializer under opt-in: same residue. Scratch header removed.
5. s05 named creature reference before filter/vector: same residue.
6. s06 proven original C2DBoxI radius member-initializer branch: same residue.
7. s07 filter const reference bound to temporary (lifetime extended): same residue.
8. s08 named position reference before vector initialization:508bytes,158/164equal,worse entry; reverted.
9. s09 raw-donor full collector locals: const map iterator,type_flags at function entry,named typed pthing/null guard before filter/push_back. Same leaf residue; scratch helper overlay removed. No incomplete stand-in.
10. s10 genuine filter operator body moved out of class with original signature/body visible: same residue.
11. s11 scoped auto_inline(off/on) around genuine filter definition: same residue; compiler-specific branch guarded. Scratch header removed. No device retained.
12. s12 explicit original STLport allocator constructor argument: same residue.
13. s13 original named area and filter in a sibling local block: same residue.

Budget verification p10T65,p11T61,p12T60 leave selected508/508 and same entry. p13T20 outlines unrelated helpers,504bytes/162instructions and worse. Flags are not counted as source strategies. No forced-inline workaround added. Final selection is unchanged s02,not the last experimental source.

Actual RTM13.10.3077 /O2 /Oy /W3, no flags. Every one of15 native external targets audited,all17 internal jumps exact. Native prefix loads PBrain,then pushes esi/zeros ebp/stores filter179,then reads Creature,pushes edi and zeros vector. Selected compiler loads Creature immediately after PBrain and delays filter store until vector zeros.23 differing bytes at offsets10..36,all caused by that equivalent scheduling; no false match claim. All remaining instructions/operand destinations agree. No literal,text or scalar/array CRT ambiguities; _free is original vector allocator cleanup,__ftol2 is original float-to-long helper at both coordinates.

Both actualVC71 /Od /W3 /GR- and native64-bit g++03 tests PASS163840cases. Exact selected body copied.5counts0..4,256 exact binary weight profiles,8 random-boundary modes(-1,0,1,half,total,total+1,NaN,+infinity),four exact fractional/negative positions and4 seed patterns. Tests check precise truncation/20-wide search box/filter,all first-loop visits and second-loop early selection order,zero weights/empty collection/fallthrough,range+seed passed to float RNG,empty-case32bitLCG/remainder/FPS/frame schedule,unchanged nonempty FrameToLook and collected vector. Oracle cumulative thresholds independent; real external float RNG is stubbed with controlled boundaries,not reimplemented. Empty seed arithmetic explicitly masks32bits. Logs rows/006d18a0/test_logs/00919240/main_run_001.

# Final revisit after all assigned rows were attempted

Four additional families were checked without changing the selected source, headers or tests, bringing the conservative family count to17. r14/r15/r19 choose other original empty-vector constructor forms (zero-count, copy-initialized empty vector, zero-count/fill); zero-count forms508bytes/fourhunks are worse, copy-initialization508/threehunks unchanged. r16/r17/r18/r20/r21/r22/r23/r27 parenthesize the original creature/position/coordinate/filter arguments, getter returns, or filter initializer: all508bytes/three entryhunks unchanged. r24 applies the final permitted budget-only FABLE_FORCEINLINE device to the genuine filter constructor after existing plain/flags/scoped trials; r25/r26 apply it to each proven original GetCreature getter: same508bytes/threehunks. Scratch overlays and immutable histories are retained; none of these devices or source forms is selected. No additional behavior test was needed because selected production/test bytes remain unchanged. The honest entry scheduling residue remains.
