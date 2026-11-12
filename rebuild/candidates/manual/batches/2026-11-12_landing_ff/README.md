# 2026-11-12 landing ff -- 0x009F20B0 CMemoryAllocator::CMemoryAllocator (the one Fable author pass)

Source: `pending/2026-11-12_linux_handWF14F/`. Three Opus author passes in workflow wf14 (about 150 grades) left this row at
DIFFER(596v598), register allocation only. It is the first row of the Opus-only workflows that Opus could not match, so under the
owner's rule it got ONE author pass with `args.authorModel = 'fable'` (scout, both verifiers and the repair stage stayed Opus). The
Fable author matched it on its first attempt, in about 27 minutes, starting from the Opus notes and priority traces.

**What made it match:** in the second loop the lookup-entry store + `break` is written out in BOTH arms (fixed-size range match /
variable size) instead of once after a `continue` chain. VC7.1 merges the two tails after register allocation; before the merge
`size` has one more weighted reference block than `a`, so `a` takes ebx, the zero constant is split early, and its first piece
outranks `heap`: zero keeps ebx through the first loop, as in retail. The row is ported onto the shared headers (protected ctor,
PDB storage, CMemoryAllocatorBase::operator new, real FixedSize / VariableSize classes).
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1; both Opus verifiers passed it with no findings. Purity clean
(`#pragma intrinsic(memset)` only). Headers (additions under new opt-ins, unused elsewhere, defaults unchanged per hdrcheck2):
CMemoryAllocator.h, CMemoryAllocatorBase.h, CMemoryAllocatorFixedSize.h, CMemoryAllocatorVariableSize.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011ff): 0x00A66730 CMemoryAllocatorBase::operator new, 0x009F1D20 InitializeMemorySystem (anonymous namespace).
