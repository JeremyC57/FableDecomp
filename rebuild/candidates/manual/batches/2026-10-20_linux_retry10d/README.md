# Retry 10, fourth landing (2026-10-20)

Segment four of the Fable 5.1 retry pass: 311 rows worked, 149 matched in total; the 11 new rows landed here with
`verify_and_land.py --land` (verify.log). New families: x87 operand order of vector +/+= decided by how operands are
reached (free inline helper vs member operator vs ctor form vs named float locals; 5 rows), store/load pinning through an
inline member Set()/Reset() over a pack(1) sub-record, the PDB unsigned-short bit-field group, GetInterface into a local
then member-assign on success, and the 9-byte vcall thunk `??_9@$BDA@AG` emitted from a pointer-to-member initialiser.
The CDefPointer zero-store/release gap closes to register-only deltas when the zeroing wrapper is a member of the caller's
class. The pass resumes from STATUS.tsv.
