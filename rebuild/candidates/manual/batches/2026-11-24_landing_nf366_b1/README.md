# nf366 b1: seven @DeleteIfSet@4 stand-ins respelled onto their PDB CCountedPointer<T>::DeleteData names

Agent NF366 (Account A naming queue; hand-back `pending/2026-11-24_nf366_handback/`), from the NAME_FIXES nf360 research lines. Each row is
now in the 0x00428B10 form (FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY + engine/CCountedPointer.h, a strong `template <>` DeleteData
specialization with the landed body, the PDB `// Leaf:`):

| Row | T |
|---|---|
| 0x00447660 | const CInputTypeBase |
| 0x00A28370 | CMeshDataBankEntry |
| 0x00A281E0 | C3DMesh2 |
| 0x00AA4310 | QuadricOptimiserInternals::COptimisedMesh |
| 0x00B3E080 | CLoadingMemoryFailureHandler |
| 0x00CD42E0 | CScriptDefinitionManager |
| 0x00F38920 | CRenderTarget |

T is proved by two sources per row:
- the FableWin.pdb name set (DeleteData and Reset for that T);
- retail: the address's only absolute reference is the `mov dword ptr [eax+4], <addr>` in its 103 B Reset, whose caller passes an object of
  that type (retail RTTI; for 0x00CD42E0 the vtable slot map, since that vtable has no RTTI).

Details are in evidence/identity.tsv. Rows whose T has a class header include it (the shared-headers rule); the forward-declared variants
measured byte-equal.
Skipped (T not proven by two sources; nf360 research lines annotated): 0x0044AC20, 0x00A0BB40, 0x009A6C00, 0x00991E10.

Rulings (coordinator, on precedent, 2026-11-24):
- **N1:** the Reset binders keep their table stand-ins (`?Destroy_<addr>@@YIXPAX@Z`), unresolved at HEAD and after (NF360 pattern). Their
  PDB Reset spellings are queued (NAME_FIXES nf366).
- **N2:** the new LNK2005s of 0x00A281E0 (v 0x00A24520's copy) and 0x00F38920 (v 0x00F38990's copy) have identical bodies (11 B, no
  relocations): the nf184 / R3 pattern.
- **N3:** the include form stands.

Agent gates:
- vland MATCH PASS HEAD and new;
- objsec OTHER_SAME, symdelta 0;
- both checkers GATE PASS with controls, 0 reach flips;
- drop test: only `@DeleteIfSet@4` drops, which has 0 table lines and no binder;
- links over 41 HEAD-compiled controls: no different-body duplicate; the 6 HEAD LNK4006 on `@DeleteIfSet@4` go to 0.

Coordinator (HEAD and repo builds, `$S/v366H.sh` / `$S/v366N.sh`):
- 7/7 MATCH PASS before and after; the only WARN change is the HEAD "leaf matches no symbol" warnings going away;
- pk_all / pickany_vs_retail: 0 new NEQ; reachflip 0 (7 pairs);
- drop test: only `@DeleteIfSet@4` (bind 0).

self_symbols refreshed, and the seven nf360 research lines closed (NAME_FIXES_CLOSED.tsv). The file / Module renames are proposals only
(RENAMES.tsv in the hand-back).
