# Chunk 827 header overlay

Both edits are opt-in `#ifdef FABLE_RETAIL_*` branches; the default path is textually unchanged (checked with `diff` against
rebuild/include on 2026-11-07), so no landed includer can change and none was rechecked.

## engine/CTCCreatureOpinionOfHero.h
- `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY` (chunk 824, carried unchanged from `pending/2026-11-06_linux_hand824/hdr/`): the
  `class CMemoryDataInputStream;` forward declaration and `void COpinionTransientOffset::TransferBinaryIn(CMemoryDataInputStream&)`.
- NEW `FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY` (chunk 827): a user copy constructor
  `COpinionTransientOffset(const COpinionTransientOffset&)`, member-wise in memory order (init list).
  Evidence: Ego_r.pdb names `??0COpinionTransientOffset@@QAE@ABV0@@Z` (Ego_r 0x0045DA29; an implicit trivial copy is never emitted as
  a function, so the class had a user copy ctor); retail copies the offset into the list node member-wise in memory order
  (0x00684FD9..0x00684FFB) and builds the two hoisted GetDefaultVal temporaries with one `xor eax,eax` and six memory-order stores
  (0x00684A46, 0x00684F37) -- exactly what VC7.1 emits for `{ COpinionTransientOffset value; return value; }` (no NRVO) through this
  copy ctor. Used by src/00684610.cpp (RELOCATION_MATCH).

## engine/CDefPointer.h
- NEW `FABLE_RETAIL_CDEFPOINTER_ASSIGN_REREAD` (chunk 827): the template `operator=<U>(const CDefPointer<U>&)` written in the
  copy-assignment's reread form (`if (Object != other.InternalGetObject()) { Release(); Object = other.InternalGetObject(); if (Object)
  Object->AddRef(); }`) instead of `Assign(other.InternalGetObject())`; the `#else` keeps the existing line.
  Evidence: in 0x0066CF00's inline `CDefinitionManager::GetDef<CEngineThemeDef>` retail reads the source pointer again after the
  Release (0x0066D03C `mov eax,[esp+40h]`, 0x0066D049) where the Assign form caches it in edi; the PDB name sets carry the template `operator=<U>` itself
  (`??$?4$$CBVCDefClassBase@@@?$CDefPointer@VCDefClassBase@@@@QAEAAV0@ABV?$CDefPointer@$$CBVCDefClassBase@@@@@Z` and the
  CExpressionDef instantiation). Used only by the parked differ/0066cf00.cpp (669 -> 659 lines); keep it out of the repo if the coordinator
  prefers not to ship an opt-in with no landed user.
