# Batch `2026-10-14_linux_eh6` — hand-authored small EH-framed bodies: 7 bodies (7 landed)

Sixth hand pass (Wine lane, `/EHsc`): two `new CDerived` factories through the class operator new
(inline derived constructor over an out-of-line base constructor), the constructor and destructor
of `CCardPositionsDef` whose base and member types are `__declspec(dllimport)` (constructors and
destructors called through import-address-table slots), the two `CAIStateGroupBehaviour`
constructors that pair with the eh5 destructors (chunk base, own vtable, a chunk member built
through the same base constructor), and `CEngineProfileLog::Open` (a scoped critical-section lock
whose inline constructor/destructor call `EnterCriticalSection`/`LeaveCriticalSection` through the
import table). Parked: the two member factories `new CObject(this, argument)` @ 0x00C37990 /
0x00C36180 -- retail returns through one epilogue with the block pointer, VC7.1 here emits a
separate null return (see `docs/journal/FINDINGS_LOG.md`).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets10`.
