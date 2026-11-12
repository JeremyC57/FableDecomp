
- (agentNF24, 2026-11-12) Wine cl 13.10 hangs (does not exit) on a hit `#error`; run a gcc `cpp` pre-pass with the row's defines before every grade, and kill a hung cl only by PID.
- (agentNF24, 2026-11-12) The 0x007FEF40 "lane gap" was a model problem: the shared STLport body in fable_containers.h reproduces _M_insert_overflow and the row matched.
- (agentNF24, 2026-11-12) A `#pragma auto_inline(off)` around fable_containers.h stops the implicit inline push_back from expanding; leave it out.
- (agentNF24, 2026-11-12) After including a header that only declares `~CScriptThing`, an `inline CScriptThing::~CScriptThing() {}` in the TU gives the retail inline dtor without a local model.
- (agentNF24, 2026-11-12) Explicit template instantiation ignores access, so a private nested key/value type (CTCUnholyPower::PerCreatureInfo) can be named in the instantiation.
- (agentNF24, 2026-11-12) 0x00797E30 (map in CTCUnholyPower) needs the full define set of landed 0x00794E60 plus FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE and CThingCreatureBase.h included first.
- (agentNF24, 2026-11-12) FAM07-style RENAMES rows need a bracket-aware split at the last top-level `::` to put the class in module.
- (agentNF24, 2026-11-12) Check every vtable-slot binder by reading reloc_bindings for the exact slot: the 0x012C2FF4 CThingFunctorBase binder was 0x00EF7AB0, not 0x00E241B0.
- (landing dg, 2026-11-12) A naming landing that renames a parent row's file breaks the EH funclet rows that `#include` it by path (8919d6f50 broke three); run lane_tools/coordinator/funclet_includes_check.py after every landing that moves files.
