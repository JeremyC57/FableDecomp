# Remaining rows

| Address | Object/retail bytes | Strict instruction hunks | Unequal masked bytes at identical offsets | Residue |
| --- | ---: | ---: | ---: | --- |
| 0066a9e0 | 626/626 | 1 | 1 | The correctly bound GetString call leaves one string-home displacement at 0x0066ABFE. |
| 00ae3c30 | 2310/2310 | 1 | 11 | At 00AE40AC the texture-coordinate buffer starts in EDI rather than EAX. Retail copies EAX to EDI after the count test. The later read and the remainder of the function realign. |
| 00b0d360 | 2240/2240 | 2 | 2 | At 0x00B0DAA0 and 0x00B0DAB0 the following float default occupies +0x14 instead of the retired context argument word +0x28. |
| 006e89b0 | 344/344 | 2 | 3 | In the preferred-distance branch, retail multiplies prong.X by the distance at 0x006E8A7A before restoring esi at 0x006E8A7C. The selected compiler build restores esi first. |
| 00c52c40 | 75/75 | 2 | 3 | The CPUID model selection uses the other available CPU operand from 0x00C52C40. |
| 00f3abf0 | 605/605 | 2 | 4 | The EDI-to-EBX copy straddles the block-width guard at 00F3AC0C in the opposite order. The selected function otherwise has the full retail extent. |
| 00680050 | 439/439 | 2 | 4 | After HasDummyObject, retail reloads pappearance_def.Object into edi at 0x00680196 and the conditional temporary-cleanup bitmask into eax at 0x0068019A. The selected compiler build reloads those independent values in the opposite order. |
| 005ac10e | 3532/3532 | 2 | 4 | Retail reloads the category children before the frame in the subtitle join. The rebuilt instructions are reversed. |
| 00b6dc40 | 771/771 | 2 | 7 | Retail evaluates the bank-entry size before loading the high-priority setting and then reloads LoadingSeaEntry. The draft loads priority first and moves one UnloadSea restore. |
| 008eec70 | 1129/1129 | 2 | 122 | The return-buffer address at 0x008EEF2E moves across the x87 calculation. |
| 00671600 | 1449/1450 | 2 | different length | At 00671B06 the byte temporary and flag storage enter AL and CL in the opposite order while updating IsIndestructible. The corresponding shift and mask reverse as well. |
| 0089ce70 | 614/616 | 2 | different length | The inlined sensory check emits a compare and pop sequence that differs at 0x0089D096. |
| 00918a40 | 114/116 | 2 | different length | Retail leaves the TC pointer in eax across the nested block exit at 0x00918A85. The shared port forwards it directly to ecx and restores esi after the call. |
| 00879070 | 1249/1251 | 2 | different length | The later two-vector cross products now agree. At 0x008792C1 the retail body keeps the quadratic factor live through a second x87 pop. |
| 0099f7a0 | 126/130 | 2 | different length | The shared copy operator leaves a four-byte deficit in the inline reference-count increment at 0x0099F7BF. |
| 00837220 | 716/716 | 3 | 9 | The third normalization setup and cross-product Z operand order differ at 0x008373EC and 0x00837419. |
| 00a3c3b0 | 777/777 | 3 | 12 | Grouping the slerp weight reduces the residue to three induction and reload hunks at 0x00A3C57A. |
| 00a1ba10 | 642/642 | 3 | 13 | The height constructor restores Z operand order but X and Y still enter the x87 stack differently. |
| 00919240 | 508/508 | 3 | 23 | The filter initialization at entry schedules in a different order at 0x0091924A through 0x0091925D. |
| 00b50370 | 406/406 | 3 | 74 | Body constant loads and stores schedule differently from 0x00B50464. |
| 00add690 | 646/648 | 3 | different length | The unrolled radius scan remainder at 0x00ADD8C0 restores a distinct counter before looping. The compiler coalesces that counter with count. |
| 00f3e120 | 1411/1411 | 4 | 8 | Two reload pairs remain in reverse order at 0x00F3E291 and 0x00F3E3C5. |
| 00875fb0 | 1315/1315 | 5 | 37 | The loop initializer is scheduled after two float stores where retail places it between them at 0x008760C0. |
| 00803540 | 1108/1108 | 5 | 38 | Two instruction scheduling sites remain around 0x0080380E and 0x00803890. |
| 009397e0 | 569/571 | 5 | different length | The shared random rotation restores the native seed update before the external call. Around 009398C2 the seed pointer enters EDX where retail retains it in EDI. |
| 009552c0 | 861/861 | 6 | 148 | definition cleanup and follow-up combat action tail merging differ. |
| 0070e2a0 | 618/618 | 7 | 8 | The placement of the second blend-state temporary differs across seven frame-reference sites. |
| 00837570 | 697/697 | 7 | 19 | The entry cleanup-state store and last ScriptName temporary use different scheduling and stack placement. |
| 0074a4e0 | 232/234 | 8 | different length | The entity reference reload at 0074A512, loop scheduling, and the removal path differ. The direct array/position form is the closest retained port. |
| 00adefc0 | 472/471 | 9 | different length | The existing shared SSE kernel matches. The x87 fallback still differs in the ordering of the four products and the first two addends. |
| 0059fc17 | 725/716 | 9 | different length | The tattoo card pointer, size home and loop coloring still differ. |
| 00903f30 | 254/254 | 10 | 11 | The allocator exchanges proute and ai_move_type from 0x00903F75. |
| 0067e290 | 273/273 | 10 | 19 | The enum transfer arm exchanges scalar CPU operands from 0x0067E2EB. |
| 00698170 | 693/693 | 10 | 40 | The first CTools calculation schedules an origin reload across the x87 operation at 0x00698299. |
| 007bb1f0 | 958/960 | 10 | different length | The original typed field and helper declarations reach the expected virtual and callees, with ten scheduling and CPU operand hunks beginning at the opinion-source setup. |
| 006e3890 | 546/529 | 11 | different length | Inline intrusive pointer release removes the excess destructor relocation. Group lookup is still scheduled after the emote arm and the pending-return paths retain different instruction sequences. |
| 00a571a0 | 652/654 | 13 | different length | From 00A57210 the optional output tests and temporary x87 values differ. The public const leaf and optional output pointee types agree with both donors. |
| 007c81f0 | 768/768 | 14 | 96 | The iterator tag and hit_direction stack positions differ from 0x007C8210. |
| 00873e10 | 2384/2386 | 15 | different length | The shared Z-Y-X dot product improves the grade to 2384 of 2386 bytes. The first difference is in input sensitivity stack placement, followed by current DollyLength and rotation scheduling. |
| 006af7d0 | 612/612 | 16 | 70 | The original scoped axis locals on the shared physics headers improve the shape, but the response-basis computations and x87 operands differ. |
| 004a1cb0 | 1331/1330 | 18 | different length | The remaining differences are the cross-product loads, inverse translation term order and bounding-box copy scheduling. |
| 00766dd0 | 1161/1067 | 19 | different length | The shared owner and STLport containers retain the native predicate calls and component names. Vector allocation, period scheduling, and iterator code generation still differ. |
| 009f18f0 | 166/166 | 20 | 111 | The copy-state allocator gives this and read_pos different priorities. |
| 007e0f20 | 531/537 | 21 | different length | The presence field and fx_index receive distinct frame allocation near 0x007E0F2A. |
| 008f18c0 | 795/770 | 21 | different length | The region using the card-definition out-argument allocates different frame slots. The table-space basis and projected card bounds schedule differently. |
| 006a64d0 | 865/865 | 23 | 25 | The full-expression render text lifetime restores the 865-byte extent. The frame size, renderer call dispatch temporary, and choice of registers in component lookup and string concatenation still differ. |
| 0091b570 | 515/498 | 26 | different length | Shared list, filter, perception and animation types preserve the call boundaries. The compiler still allocates the actor, list and rectangle temporaries differently. |
| 00699670 | 759/761 | 28 | different length | The emitted vector growth and water-height path agree in meaning. Retail saves the observed position in a different register, shifts the local frame homes, and schedules the ground comparison differently. |
| 007c0ad0 | 936/938 | 28 | different length | The creature-shot constructor and class-name comparisons have different expansion and temporary placement from retail. |
| 00931390 | 3863/3863 | 29 | 1279 | The door vector storage, talk receiver registers and branch alignment still differ. |
| 00935580 | 605/617 | 30 | different length | The shared port preserves original fields and call names but changes the minion state and vector frame homes. The retired local-model draft had a smaller x87 residue. |
| 00589402 | 1564/1582 | 34 | different length | Donor card scopes improve the early register allocation and control flow. Card-release scheduling and the title setter register remain different. |
| 00a90260 | 498/496 | 35 | different length | The index-buffer bookkeeping retains a CPU operand permutation from 0x00A90271 and differs by two bytes. |
| 007f6200 | 1458/1458 | 36 | 437 | The old enemy and receiver occupy opposite frame words near 007F621C. The later component-selection paths retain additional differences. |
| 00b30610 | 678/710 | 37 | different length | The shared matrix operator produces fewer repeated corner sums and shrinks the leaf by 32 bytes. |
| 0099c1e0 | 613/609 | 38 | different length | The current prefix lifetime leaves a larger frame and different ownership-counter and destructor scheduling. The public underlying append specialization and reference-count ownership are present. |
| 0072fb70 | 1319/1278 | 40 | different length | The shared village and spawn APIs retain the native second marker pass, including frustum and line-of-sight gating. Definition cleanup, vector allocation and marker traversal scheduling still differ. |
| 00a3a410 | 942/920 | 41 | different length | The typed bank and stream hierarchy emit their original members. Counted allocation, creation and destruction order, and common exit scheduling still differ. The native inequality call and equal-timestamp skip are preserved. |
| 006a03a0 | 855/825 | 42 | different length | The shared action and graphic classes use the original dummy lookup and bounding query. The animation scan, registry calls, and x87 evaluation order still differ. |
| 00ac9a60 | 743/745 | 48 | different length | The shared STLport and quadric types reproduce the extent closely, but the normal dot pair and the adjacent-face erase/sweep allocation differ. |
| 00a36a80 | 747/747 | 49 | 606 | Equal length. Stack allocation and x87 scheduling after constructing the animated dummy differ. |
| 00ade080 | 2696/2735 | 58 | different length | Portable SSE intrinsics and the scalar four-diagonal path produce different operand scheduling and instruction selection starting in the SIMD arm. |
| 00a18fc0 | 1127/1125 | 61 | different length | Frame and scratch-CPU operand assignment differ at entry. The normalisation and final altitude tests also schedule differently. |
| 0093fcd0 | 1053/1053 | 72 | 585 | The frame size, task-pointer register assignment and look timer spills differ. The emitted leaf has the retail length. |
| 007542b0 | 2220/2266 | 75 | different length | The shared owner, render mesh and attachment classes retain the native receiver offsets and frame extent. Retained handles, index-slot evaluation and the two eye transform schedules remain different. |
| 0068dd00 | 1617/1471 | 84 | different length | The shared input, event and component interfaces preserve the native SpellMode and UseInfoDisplay gates. The compiler still differs in component lookups, event lifetimes and the protected input-body exits. |
| 008e9b70 | 1736/1705 | 86 | different length | The shared model keeps the animation decisions and targets. The prologue, construction order and frame stores still differ from retail. |
| 00d0ee70 | 1991/1989 | 94 | different length | The watcher still expands or calls different string and vector destructor sites and retains a different allocator-argument construction path. |
| 007a76f0 | 1219/1239 | 95 | different length | The shared owner uses the original battle definition, effect map, noise and particle APIs. The direction construction and three intelligent-pointer effect blocks retain different stack and instruction schedules. |
| 00677970 | 2304/2272 | 112 | different length | Initializing the numeric trait locals improves instruction correspondence. Context allocation and numeric token string lifetimes still differ. |
| 0094ab00 | 1949/2067 | 127 | different length | The shared version still emits calls where retail expands list insertion and the component-map search. Pointer temporary slots and the early branch layout also differ. |
| 00afae50 | 2413/2405 | 156 | different length | The accepted inline split removes the extra IsActive, GetRecordedPos and TransformPosIfLocal calls. The receiver allocation and frame differ from the prologue onward. |
