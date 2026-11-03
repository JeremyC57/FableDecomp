# 0121af20 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 3. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/0121af20/NAMING_EVIDENCE.md

# Original identities and complete operand audit

The assigned 2,695 bytes are the dynamic initializer of the private static const array `CPlayer::PlayerModeInfo[MAX_NO_PLAYER_MODES]`. The compiler-generated local Leaf is exactly `_$E1`. Both original PDB procedure records name the initializer in `player_modes.obj`; the release donor has a 2,694-byte initializer, while the debug donor has an 8,141-byte initializer. These are donor builds, not the retail extent. Retail ends at `0121b9a6` with RET, followed by nine alignment CC bytes. The retail CRT pointer at `013743ec` is the only occurrence of the initializer address in the executable. The compiled `.CRT$XCU` contains one relocation to `_$E1`.

`evidence/0121af20_{Ego_r,FableWin}_types.json` independently prove class keys, field access, the private callback declarations, and the private static const table. `CPlayerModeInfo` is a class, 32 bytes, with the public fields `InitFunc` and `UninitFunc`, each a 16-byte general pointer to a nonconst CPlayer member returning void and taking `(EPlayerMode, CProcessedInput*)`. `CProcessedInput` is a class in both original type receipts `0121af20_*_processedinput_types.json`. The existing full CPlayer storage and dependencies were proven in the separate 00501450 receipts and are retained here. The complete original EPlayerMode values are independently recorded in `004fb990_{Ego_r,FableWin}_enums.json`: modes 0 through 47 and MAX_NO_PLAYER_MODES 48. The array's 1,536-byte size independently confirms 48 entries in both PDB global streams.

The exact private const array spelling is `?PlayerModeInfo@CPlayer@@0QBVCPlayerModeInfo@@B`. Both original global streams record that symbol and a typed `const CPlayerModeInfo[48]` object at the same address. The older public, mutable, struct spelling is rejected and recorded in NAME_FIXES.json. The original debug image locates the table at `04abe2c8`, its initializer at `03fe1960`, and its CRT pointer at `040157f0`. Retail table storage starts at `013b8c88`.

`workers/engine/0121af20_table.py` interprets the complete native and original debug initializer bodies. It accepts only the actual straight-line instruction forms and asserts all 384 DWORD table fields are assigned once. It checks the three metadata DWORDs for each of the 96 member pointers and resolves every original debug callback thunk to its exact original PDB procedure name. All 94 nonnull callbacks also have the same exact private procedure name in Ego_r and the correct private signature in both original CPlayer type records. The resulting exhaustive table is `evidence/0121af20_member_pointer_table.json`. The two null member pointers have metadata 0,0,-1, and all nonnull callbacks have metadata 0,0,0. This is a direct cross-build table identity proof, not a guess from native address order; notably the magic, statistics, and experience callbacks are not stored in native function-address order.

`workers/engine/0121af20_review.py` independently audits every one of the 478 selected COFF relocations: 384 destination addresses are the proven private const array plus the precise DWORD addend, and 94 callback addresses match the proven table. Every non-relocated byte is identical across the complete assigned retail extent. Exact native and object raw disassemblies, including the final RET, are saved under `evidence/0121af20_{native,object}.disasm`. There are no virtual calls or imported callees in this initializer. RELOCATIONS.json records every operand, not only unique names.

No member-pointer size directive, packing change, stand-in class, or forced inline is used. Defining the original CPlayerModeInfo before completing CPlayer naturally retains the general member-pointer representation required by both original PDBs and the native table.
