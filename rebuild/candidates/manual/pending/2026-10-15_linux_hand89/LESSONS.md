# Chunk 89 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A constructor whose zero stores ALL follow the vtable store, starting with a def pointer at +0xc, needs that def pointer as a
  member OBJECT with an inline null constructor (`struct CDefPointer { T* Object; CDefPointer() : Object(0) { } }`); a plain
  `T*` with an initialiser schedules its store before the vtable store (0x0077D470, 57v57 -> RELOCATION_MATCH).
- `GetWorld().GetFrame()` where GetFrame is a static: VC7.1 still calls the out-of-line GetWorld (0x00686A80) for the object
  expression and then the static with nothing in ecx (`mov ecx,esi ; call GetWorld ; call GetFrame`), 0x00706640/0x00706EC0.
- The VC6-era `vector::erase(begin(), end())` (used by `clear()`) keeps its copy loop even though its count is provably zero:
  `mov ecx,eax ; sub ecx,eax ; sar ecx,3 ; test ; jle` then the 8-byte record copy (0x00706640).
- A clamp to [0, N] that reads N from .rdata (`mov eax,offset ; ... mov eax,[eax]`) is a const-reference Clamp template with a
  namespace-scope `const long` bound (0x007B29B0); the lower bound's temporary shows as a dead `mov [esp+4],0` store.
- A vector element read whose first pointer is re-loaded AFTER a reference-returning Min select needs operator[] written through
  an iterator object (`*(Begin() + i)`); `_Myfirst[i]` loads the pointer early (0x006D65E0 / 0x006D6620).
- `if (GetPDef().IsValid())` with a by-value counted-pointer getter = add-ref, `setne bl`, release, then `test bl,bl` -- the
  whole temporary lives and dies before the branch (0x006D7570).
- A by-value counted def copy that is dereferenced (`combat->Member`) loses the null test of its release (`dec [esi+4]` directly);
  one only used through `lea` (`&combat->Weighting`) keeps it (0x006D6EA0 vs 0x006D6870).
- An `optimize("s")` forwarder `Getter()->Setter(a, b)` re-pushes both stack arguments (`push [esp+8]` twice) before the getter
  call; the speed form loads them into eax/ecx first (0x00572672 vs 0x006C9FA0).
- A `bool` parameter forwarded twice is loaded once as a whole dword into a callee-saved register (`mov edi,[esp+0Ch] ; push edi`),
  so the PDB's `(bool, double)` signature matches as well as a pointer would (0x00B2AD90).
- Lane gaps seen (park early): branch on the flags of a memory `sub` across a reload (`jns`, 0x006D6590 / 0x006D6F60); a float
  kept live on the x87 stack across a same-module call (0x006D7120); out-pointer register choice ecx vs eax after an inline
  GetInterface with a zero-initialised out pointer (0x007B2D50).

Review at landing:
- Rows that needed more than 4 grades: none (0x006D6590 / 0x006D6F60 got a fourth/fifth grade only to re-create the first
  agent's overwritten best form; 0x00B2AD90 / 0x006CA040 / 0x0077D470 were re-graded after switching to PDB signatures/names).
- Local base models next to an existing engine header: CTCHeroExperience (engine/CTCHeroExperience.h: PDB debug-STL layout,
  TrainableStatLevels +0x28 vs retail +0x24), CTCHeroAttachableAppearanceModifiers (header AppearanceModifiers +0x38 vs retail
  +0x30), CTCHeroCentre (header WaitingForTutorialToFinish +0x38 vs retail +0x30), CEngineSurfaceCompositionManager (header
  banks at +0x94/+0x41c/+0x7a4 vs retail bank members at +0x4fc/+0x808, modelled as `CEngineSurfaceCompositionManager_Retail`),
  CTCBase everywhere (engine/CTCBase.h is a flattened POD). Callee-only classes with headers are declared as `<Class>_Calls`
  (CWorld, CTCCombat, CTCVillage, CTCInventoryAbilities, CTCHeroExperience in 0x00572672).
- The 0x00B2AD90 bank member names are neutral (m_bankAt4FC / m_bankAt808): which two of the PDB's three CGraphicDataBank members
  they are is not established.
- Catalog rows holding several functions: none. 0x00707930 and 0x00707AD0 are two distinct functions with byte-identical bodies
  (UploadChangesToGraphics and the virtual FrameUpdate).
