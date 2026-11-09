# Parent model fixes found by their EH unwind funclets (chunk 896, 2026-11-09)

Six landed parents emit a `$L` funclet block that differs from retail. A parent's own grade does not cover that block,
so these defects only showed when the funclet was graded. Each fix changes the class model (layout / base vs member /
where a throwing call sits), never the body: every parent keeps RELOCATION_MATCH + behaviour PASS under
`vland --recheck-landed` (payload from the catalog row, source/test replaced: `logs/parent_regrade/`), and its funclet then
grades RELOCATION_MATCH + PASS + offcheck2 OK at the expected offset of exactly one block of the right length whose
`__ehhandler$` names the parent (`logs/parent_fix_<n>/fun/`). The landed sibling funclets of 0x00C2D3B0 (0x01213AF1)
and 0x00C287E0 (0x012134A0, 0x012134B9), which include these parent files, still grade RELOCATION_MATCH + PASS with
the fix (`logs/parent_fix_<n>/sib/`). Funclet grading used `FABLE_HDR_OVERLAY=$A/ov/include` so the stubs' unchanged
`#include "../src/compiled/<parent>"` resolved to the fixed file (`$A/ov/src/compiled/...` symlinks).

Apply: copy `<addr>.cpp` over the parent's `rebuild/src/compiled/<Source>` (and `<addr>_test.cpp`, where present, over its
TestSource; the tests are standalone mirrors, updated only to the new offsets). `parent_fixes.diff` has all changes as a
unified diff against the repo. THEN land the six funclets in `payload_land_after_parent_fixes.json` /
`oracle_after_parent_fixes.tsv` (their stubs include the parent by its unchanged path). Do not land them without the fix.

| Parent | Source | Funclet | Retail funclet | Defect -> fix |
|---|---|---|---|---|
| 0x00C2D3B0 | 00/c2/unattributed_CArtResourceSet00c2d3b0_00c2d3b0.cpp | 0x01213B0A | `mov ecx,[ebp-10h] ; add ecx,320h ; jmp 0x00C162A0` | 0x00C162A0 is `add ecx,4 ; jmp 0x00C167C0`: a holder at +0x320 whose implicit dtor destroys the member at +0x324. Model: `CMemberHolder_00c162a0 m_holder` at +0x320 (pad 0xc -> 0x8). Was `add ecx,324h ; jmp ~CMember`. |
| 0x00C287E0 | 00/c2/unattributed_CStackOwner00c287e0dtor_00c287e0.cpp | 0x012134AB | `add ecx,43Ch ; jmp 0x00C29CA0` | the vector starts at +0x43C (empty allocator word, begin +0x440; 0x00C29CA0 reads `[esi+4]`). Model: leading `unsigned char m_allocator`, pad 0x408 -> 0x404. Was `add ecx,440h`. |
| 0x00C28AE0 | 00/c2/CHandleBasedMemoryPool_CHandleBasedMemoryPool_00c28ae0.cpp | 0x012134E3 | one action only (FuncInfo 0x0136CED0 maxState 1): `+34h ; jmp 0x00C21D80` | the body's `Handles.reserve(0)` stand-in ran with vector+name alive, giving a 3-action 49-byte block (the grader's any-offset fallback still reported RELOCATION_MATCH; offcheck2 NOHIT). Moved into the vector's ctor (Dinkumware `vector() { _Buy(0); }`), so only state 0 exists. |
| 0x00CAA790 | 00/ca/unattributed_CVolume00caa790_00caa790.cpp | 0x01216DE0 | `mov ecx,[ebp-10h] ; jmp 0x00CA7780` | strmbase `CUnknown` (0x00CA7780 `??1CUnknown@@UAE@XZ`) is a BASE at +0, not a CMyBaseObject member at +4 (`add ecx,4`). Model copied from the landed CBaseAllocator dtor 0x00CA9D00 / CUnknown 0x00CA7780 rows; class `__declspec(novtable)`, dtor now virtual (`UAE`, as the vendor proof ??1CBaseMediaFilter@@UAE@XZ). |
| 0x00CAAAC0 | 00/ca/CJoystickDX_CJoystickDX_00caaac0.cpp | 0x01216E60 | `mov ecx,[ebp-10h] ; jmp 0x00CA7780` | same as above (vendor proof ??1CBaseFilter@@UAE@XZ); pad 0xc covers the IBaseFilter/IAMovieSetup vptrs + m_State. |
| 0x00CB1890 | 00/cb/unattributed_CEngineLandscapePatch00cb1890_00cb1890.cpp | 0x012173E0 | `mov ecx,[ebp-10h] ; add ecx,0Ch ; jmp 0x00CB1210` | the member is strmbase `CBaseList::CNodeCache` at +0xC {m_iCacheSize, m_iUsed, m_pHead +8} (landed 0x00CB1210 layout), not a one-word list at +0x14. Was `mov ecx,[ebp-18h] ; add ecx,14h`. |

Leaf/grade notes: 0x00CAA790 / 0x00CAAAC0 now emit `??1CVolume_00caa790@@UAE@XZ` / `??1CJoystickDX@@UAE@XZ` (virtual); the
Leaf-less recheck picks them by the `??1<Class>@` prefix. VENDOR_PROOF_MISMATCH warnings are environmental (no dxsdk tree
under FABLE_THIRDPARTY in this lane).

Naming follow-ups (not changed here; for the coordinator, "one retail address, one name"):
- 0x00C162A0 is landed as `std::_Cons_val<...>` (a COMDAT-folded label); its bytes are a destructor (`add ecx,4 ; jmp ~member`).
  The fix names the holder `CMemberHolder_00c162a0` by address, so the TU emits `??1CMemberHolder_00c162a0@@QAE@XZ` for it.
- 0x00C29CA0 is landed as `CBuffer_00c29ca0::~CBuffer_00c29ca0`; the 0x00C287E0 TU's vector class is still `CVector_00c287e0`
  (its out-of-line dtor is that address); and 0x00C28AE0's `CHandleVector` (same type) is still at +0x440 with `m_43c` before it
  (bytes unaffected; the true layout is the +0x43C one above).
- 0x00CB1210 is landed as `??1CNodeCache@CBaseList@@QAE@XZ`; the 0x00CB1890 TU (itself strmbase `CBaseList::~CBaseList` per its
  vendor proof) still spells the class `CPatchNodeList_00cb1890`.
