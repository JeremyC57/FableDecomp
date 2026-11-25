# NF428 hand-back (2026-11-25): the CCountedPointer<T> family

**Landed: 71 rows** in `batches/2026-11-24_landing_nf428_acc_r1` (34) and `_acc_r2` (37). All are RELOCATION_MATCH + PASS, the name gate passes on row and test, and leaf bytes are unchanged.
- **What changed:** each row was a `CCountedPointer<T>::Reset` stand-in, a local SCountBlock model with self `?Reset@CCountedPointer_<x>@@QAEXPAX@Z`. It is now the real Reset of `engine/CCountedPointer.h`, under the PDB name the retail callers already bind.
  - 61 rows use explicit instantiation (NF371 form b); 10 use a strong specialization (the 0x00447D90 form).
  - One catalog rename: 0x00413520 is now `CCountedPointer<CConsole>`.
  - 71 self-symbol lines.
- **Two sources per row:** the callers' binding of the PDB Reset name (a FableWin.pdb name), plus retail RTTI. At a retail call site the object passed is freshly constructed just before the call, and its vftable or `??0` names exactly T (`tools/rtti428.py`).
  - The stored deleter's DeleteData<T> binding is not counted: it was derived from these same callers.
  - Hit rate: Reset 71 / 127; family 71 / 1,107 open addresses.
- **Tests:** they include `engine/CCountedPointer.h` and define no local class body.
  - Form-b tests also define `RESET_CAMERA_DECL`, which the row does not use, so that the test links the row's Reset and does not compile the header's inline copy.
  - Mutation control: 0x004441A0 without the count-down DIFFERs and fails its test.

## Held
- **`hold_r1/` (12 rows):**
  - 9 have no RTTI source, only the callers' one.
  - 3 have form b's extra pick-any `ReleaseData<T>` copy differing:
    - 0x00403DB0 and 0x00403E40 have a new body under that name;
    - 0x008AB230's copy is not equal to its landed retail row 0x007E70E0.
- **Not landed, by shape** (`FAMILY_TABLE.tsv`; reasons per address in `T3.txt`):
  - **709 `??1` and 148 ShareData stand-ins:** no retail callers. The 53/44-byte body is both `??1<T>` and `ReleaseData<T>` in retail, so both T and the member are open. FableWin's link order is far from retail's, so its count gate is one source only.
  - **50 Reset rows (the 12 above among them) and 13 ctor(T*) rows:** a second source is missing.
  - **42 not attempted:** OperatorDelete thunks, `AutoTinyVirtualObject` DeleteData, copy ctors and the CCPtr ShareData.
  - **20 skipped:** named in T3_PORT_QUEUE.

## R-NF428-1 (coordinator decision, 2026-11-25): retail layout alone does not name an uncalled copy
The question: may an uncalled `~CCountedPointer` / ReleaseData / ShareData copy take the T of its adjacent named `CCountedPointer<T>` member?

**No.** NF428 measured that predictor leave-one-out on copies that already carry a PDB name:
- 11 right, 5 wrong, 6 conflicting, 43 with no named neighbour;
- that is 69% precision.

It counts as one weak source. A name needs two (EXTRA_RULES, "names right the first time"), and one wrong name in three is exactly the work the queue exists to remove. These rows stay held until a second, independent source exists, such as a caller, RTTI, or a direct R-NF426-1-style layout proof.
