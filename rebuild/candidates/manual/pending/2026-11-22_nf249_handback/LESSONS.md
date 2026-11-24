# NF249 lessons (one line each)
- An access-only fix on a header opt-in line can keep the file's line count (`protected: void F(...); public:` on one line): no assert moves, so the equiv proof is the definer grep plus a clean equiv run.
- A self rename onto the real ctor spelling can collide with a landed "TU emitter" row that defines that ctor as scaffolding (0x00B2AB10's empty `CEngine::CEngine() { }`): link every row that binds or defines the new self symbol, not only the callers.
- CArray<T> as a typed member needs the complete T in the TU (C2036 in fable_containers.h), even for a ctor that only zero-fills it.
- A scalar deleting dtor copy that differs only by an inlined base dtor (30 v 36: call ??1CIEngine v vptr store + call ~CInitBaseClass) means the base dtor was inline in retail; the owner ??_G body is the evidence.
- vtmap.py cannot map a multiple-inheritance subobject vtable by RTTI (`retail NONE`); join rvt.py (N=<slots>) with the object's slot list by index instead (work/e3/slots.tsv).
- The double constant 0.0 loaded by `fld qword ptr [addr]` is `__real@0000000000000000`; a typed `= 0.0` member init binds it exactly where the HEAD row needed a named stand-in global.
- equiv_check address lists go stale between landings: regenerate the includer list from today's tree (tools/incl.py) rather than reuse an earlier agent's.
