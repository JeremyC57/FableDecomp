# NF140 header overlay

## engine/CMemoryAllocator.h
Base: the repo working-tree copy (orig/engine/CMemoryAllocator.h, byte-equal to rebuild/include at the start of this round). Two add-only,
member-specific opt-ins; with neither macro defined the preprocessed header is token-identical to the base (checked with cpp -P).
- `FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE` (row 0x009D52D0): puts FABLE_FORCEINLINE on the existing inline `Allocate` (an
  inline control: the plain inline is called out of line in that EH function, DIFFER 254v310; the landed row had a FABLE_FORCEINLINE
  helper with the same body). Pulls fable_compiler.h.
- `FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE` (12 rows of q4197 / q4198): the inline public static `Deallocate(void*)` (FableWin.pdb
  ?Deallocate@CMemoryAllocator@@SIXPAX@Z) with the body of the landed 0x009F2B20, plus the members it reaches, spelled from Ego_r.pdb /
  FableWin.pdb: GetNoAllocations (QAEKXZ, 0x009F1F10), ~CMemoryAllocator (IAE, 0x009F2310), operator delete (KIXPAX, 0x009F1F90),
  InternalDeallocate (IAEXPAX, 0x009F2920), Destroy (KIXXZ, inline here), and the static flag DestroyWhenEmpty (0x013CAA94; no PDB data
  name, the landed 0x009F2B20 spelling). Deallocate and Destroy carry FABLE_FORCEINLINE as in 0x009F2B20 (0x00BD9B90: plain inline Destroy
  DIFFER 68v97, then plain inline Deallocate DIFFER 16v97, both forced: RELOCATION_MATCH).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT; its size check is a typedef under FABLE_RETAIL_CMEMORYALLOCATOR_CTOR,
  untouched).
- Landed includers of CMemoryAllocator.h: 0x00A65720, 0x009AF7A0, 0x009A3640, 0x00416005, 0x009F20B0, 0x009F2450, 0x00B482A0. None defines
  either macro, so the opt-ins change none of them.
- NF137 / NF139 overlays: neither has CMemoryAllocator.h.

No other header is overlaid. q4056 needed no header change (0x009C11A0 uses the existing FABLE_RETAIL_C2DEXTENTSI_INITIALISE).
