# Section 6 parked-row retry (wave 2) -- notes

Range 0x00B16AA0-0x0122AE7E, 353 WORKLIST rows. Stopped at the lane-time limit; STATUS.tsv has one line per row
(88 rows `NOT_REACHED`, each with its baseline / header pre-screen). Final dry vland: 38 authored, 38 PASS.

## Outcome
| status | rows |
|---|---|
| MATCH / RELOCATION_MATCH / MATCHED (src/ + test/) | 38 |
| IMPROVED (differ/) | 19 |
| HELD (attempts + residue named) | 208 |
| NOT_REACHED | 88 |

## Per-lever win counts (matches)
| lever | wins | rows |
|---|---|---|
| shared engine header replaces a local model (CCountedPointer.h; NScript headers) | 5 | 00e64d10, 00b2c4e0, 00d5dc50, 00d61bb0, 00d7d8d0 (00dc3ea0/00dcc000/00f07b80 moved onto CCountedPointer.h after matching) |
| over-span fix (BOUNDS_FIX.tsv) + loop shape `Blocks[i].NextEntry = FreeList; FreeList = &Blocks[i]` | 3 | 00b48be0, 00b48cb0, 00b48d80 |
| TU flags: drop a parked /EHsc the same-TU landed rows do not carry | 2 | 00dc3ea0, 00dcc000 |
| early return out of a guarded (PKCProtectionInstance) scope keeps the /EHsc frame | 2 | 00c14b50, 00c14bc0 |
| PDB member type (list<long>: the unsigned id binds const long& via a converted temporary) + direct [id] indexing + STLport __prev insert | 1 | 00bbce10 |
| CDefPointer dtor shape (object copy + `const long&` count view) | 2 | 0122ae7e, 01228b70 |
| const& view of the record for self-copies (volatile-candidate replacement) | 1 | 00c34630 |
| for(;;){if(cond)break;} instead of while(!cond) | 1 | 00f07b80 |
| statement order (local += before member +=) | 1 | 00c62790 |
| pointer-param forceinline `--*owners` helper + FreeData helper | 1 | 0122a550 |
| same-TU static (libz statics with a real caller) | 2 | 00c00dd0, 00c03ee0 |
| RELOC_IDENTITY fixes (operand order; MSVCR71 operator delete[]) | 3 | 00c5af50, 00c34b80, 00c34e60 |
| VC6 Dinkumware insert(end(), x) inline at the call site | 2 | 00c26cd0, 00c27130 |
| named locals / reference locals / inline accessors (x87, integer and register order) | 10 | 00c1ca20, 00c25f40, 00c12300, 00c72a40, 00c72800, 00c35370, 00c36b90, 00c16e10, 00b677d0, 00bcc1c0 |
| indexed walk + push-order argument order | 1 | 00c5dc60 |
| ungraded all-__forceinline chain graded | 1 | 00b6cd80 |

retail_asm FtoL (`fld; fistp` / `fld; mov edx,out; fistp dword ptr [edx]`) replaced volatile stand-ins in 00bd71b0 and
00ba1140 (IMPROVED, not matched). No libvorbis / libpk / CRT row in the reached set needed a retail_asm or
toolchain_library grade; retail_asm/ is empty. seed_loop 0x00C92E20 bonus test: not reached.

## Descriptive / unproven names in the matched set
- `CSettingsProxy_012b29f8` (00c14b50/00c14bc0): class named by its vtable; `g_settingsLock` (0x0143BB14) descriptive.
- 00f3fb60: element type of the resize wrapper unproven (name_review/00f3fb60.txt; HELD).
- 00c62790: `C16BitSamplePtr` dtor label placeholder (vland RELOC_NAME vs `_global::Nop_ret0_00c62a20`).

## Files
src/ (38), test/ (38), differ/ (21), hdr/ (15 header-moved sources whose residue stayed), probe/ (all graded
variants), BOUNDS_FIX.tsv (4 rows), oracle_extra.tsv (54 no-grade rows; names fixed for 6), NAME_FIXES.tsv (8 rows),
name_review/, payload.json + oracle_final.tsv (38 rows).
