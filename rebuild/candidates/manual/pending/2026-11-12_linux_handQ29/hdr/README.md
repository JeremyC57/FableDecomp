# chunkQ29 header overlays

Full copies of the current repo headers with additions only, each behind a new `#ifdef FABLE_RETAIL_*` opt-in (`diff` against
the repo shows only `>` lines). None copied from chunkQ25's overlay.

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CTCMultiArrow.h | FABLE_RETAIL_CTCMULTIARROW_RELEASESHOT | `void ReleaseShot();` (Ego_r ?ReleaseShot@CTCMultiArrow@@QAEXXZ, retail 0x007CC710) | 007cc710 |
| engine/CBankFileAsyncData.h | FABLE_RETAIL_CBANKFILEASYNCDATA_ABORT | `bool Abort();` (Ego_r ?Abort@CBankFileAsyncData@@QAE_NXZ, retail 0x009D6350) | 009d6350 |
| engine/CMovableResourceMemoryPool.h | FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL_FREERESOURCE (inside the FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL branch) | `void FreeResource(long);` (?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z, 0x00A62ED0, tail jmp onto CHandleBasedMemoryPool::Free) | 009d6350 |

00eb3ba0 (DIFFER) needs no overlay: CV_MurderTwistFinalScript.h already declares CMurderTwistSheriffGuard::AttackHero.
