# 2026-10-27_linux_hand672 — chunk 672 (NA017, never-attempted pool)

Landed 2026-11-01 via `land_chunk.sh 672`: 10/10 RELOCATION_MATCH, bindings multi-check 0. Ten destructors; seven quest/entity-script rows
named RTTI-less (??_G -> ctor -> Alloc -> registration string; entity scripts via the parent quest's Main), three engine rows via RTTI.
7 of 10 catalog labels were wrong or lacked the nesting class. NAME_FIXES.tsv (26 rows) queued as c672; three manifest ctor labels corrected.
Decisions (coordinator under owner delegation 2026-11-01): script TUs define the PDB ctor only to emit the implicit dtor (chunks 664/665/667 practice);
g_pEngine_01436e98 and the CQ_SunnyvaleMasterData template argument stay the established landed spellings until a PDB data-symbol dump.
Header debt (NOTES.md): engine/CLevelScriptBase.h still declares `virtual ~CLevelScriptBase();` (migration wave 10); nine flattened pack(1) engine headers restated locally.
Agent note: one stray recursive `find /` was started and stopped by the agent itself; no other process touched.
