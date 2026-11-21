# 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData -- AUTH1 log (overlay hdr/: repo hdr.diff applied to current headers)
1. a0.cpp = repo best (p25C v6) /Gr: DIFFER(3481v3491), residue 43.
2. a1 = a0 + CARRAY_INDEX_ASSERT + ASSERT_DEAD_ARM (p25P): 3491v3491, residue 26 ((2) position compressed-size schedule, (3) scaling
   sum order, pad at 0xA4FBE9).
3-4. e1/e2 = a1 + FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD (new overlay opt-in decoded from Ego_d AllocateType<char/uchar/
   CQuaternion> lines 21..34: if(count){u64 total_size; if(total_size>0xffffffff){ASSERT; return 0;} return Allocate(count*sizeof(T));}
   return 0;) with assert cond `false` / `total_size <= 0xffffffffUL` (first try included fable_assert.h before DEAD_ARM: 3481, void).
   -> both 3491v3491 residue 16.
5-7. Residue (2): retail evaluates uncompressed (imul /6) before the compressed arithmetic and keeps the paletted finish in ebx.
   x1: position compressed operands swapped (paletted + unique; Ego_d 1206 26 B either way); x2: `uncompressed > compressed`;
   x3: `!(compressed >= uncompressed)`.
   -> x1 3491 residue 16, x2 3491 residue 17, x3 3491 residue 16 (no move; B: in-block order / operand order never move it).
8. Size check of the AllocateType text (odl.py: VC7.1 /Od listing of mini/at1.cpp; VC10 deltas reasoned): `if (count)` 6 B here (short
   je) = Ego_d 10 with the near je over the 261-B assert; total_size 11 / 16 B = Ego_d 11 / 16 (char / 16-byte T); the u64 test VC10
   `cmp hi,0 ; ja short ; cmp lo,-1 ; jbe near` = 16 = Ego_d 16 for every T (VC7.1 /Od folds it to 8 / 12); `return 0;` xor + jmp = 4 =
   Ego_d 28; Allocate return 14 / 17 = Ego_d 31 (re-multiplies count); final `return 0;` 2 = Ego_d 33. Consistent line for line.
   Other helpers' Ego_d tables read (egl.py): GetBoneName 16/27/6 (one by-value return), GetMatrix 7/15/6, GetMatrixCount 7/11/4,
   GetPosition / GetScaling 31, GetRotation 44, SetScaling 34, GetMaximumAbsoluteComponent 9/244/6 (one line), SetupIdentityFlag
   7/54/15/2/15/4: all one-statement bodies as in the overlay; not yet compiled at /Od for byte-level calibration.
