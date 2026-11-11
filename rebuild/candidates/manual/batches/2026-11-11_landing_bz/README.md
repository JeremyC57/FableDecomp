# 2026-11-11 landing bz -- family FAM01: 64 `_global::CCountedPointer_*` stand-ins renamed to CCountedPointer<T> members

Source: `naming/2026-11-11_agentFAM01/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES / hdr README). First one-family naming pass
(user, 2026-11-11: "separate the renames by family ... launch naming agents 1 family each time"). vland --recheck-landed: 64 WINS
(14 MATCH, 50 RELOCATION_MATCH); 8 tests PASS in vland, the other 56 print their catalog PassPattern -- checked by running each exe, 56/56.
Bindings re-derived; bindings_multi_check unchanged (NullCoord baseline).
- Renamed (explicit instantiations over engine/CCountedPointer.h): ReleaseData x18 (44-byte /O1 bodies x14, 53-byte x4), ~CCountedPointer x7,
  converting copy ctors x12 (CQuestInfoElement / CVillageTaskBase from their element types), copy ctors x2, operator= x12, SetStaticCast x3,
  Reset x4, (T*) ctors x6.
- Finding: in /O2 TUs every T emits ReleaseData and ~CCountedPointer as identical 53-byte bodies; the caller decides the name (ShareData /
  Reset / a size-optimised caller -> ReleaseData; a scalar deleting dtor in a speed TU -> the destructor).
- Left: 692 unreferenced 53-byte bodies (no call / jmp / absolute reference anywhere in retail: T and member unprovable), 39 others (NOTES).
- Headers: engine/CCombatActionBase.h FABLE_RETAIL_CCOMBATACTIONBASE_STRIKEEVENT, engine/NPlayerGui_CDrawQuestInfo.h
  FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS (hdrcheck2: default paths unchanged).
- 65 name fixes queued as land1011bz (38 landed binders of the old spellings; eight landed "~CCountedPointer" rows that are ReleaseData;
  nine ShareData rows with a propagated <NUISystem::CAction> label; nine 5-byte dtor thunks).
- vland STANDIN_LEAF warnings on the converting ctors are the catalog-name heuristic reading `CCountedPointer<A>::CCountedPointer<B>`.
