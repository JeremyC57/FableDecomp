# NF112 lessons (one line each)
- Wire a cpp precheck into the grade script before the first grade: a header #error (CDefString.h without FABLE_RETAIL_CDEFSTRINGTABLE) spins Wine cl at 100% until the timeout.
- A traits temporary of a class with a user default constructor (CPersistTraits<CDefString>, CDefString() stores -1) keeps that store only when the callee taking it is opaque; retail's missing store means the callee's body was visible in the TU (as the float traits' header body removes its 0.0 store).
- Read the newest queue lines for an address before acting on an old claim: q6210's vector<unsigned char> claim was superseded by land1118eg (CArray<unsigned char>, twin 0x005136B0).
- Ten rows with an identical local SGI for_each / mem_fun_ref_t block moved onto stlport/fable_algorithm.h with one regex transformer; all ten graded RELOCATION_MATCH in one payload.
- An inline static getter's out-of-line COMDAT row emits from a free anonymous-namespace function returning its address (no derived class needed for a public static); size-optimised retail (`cmp byte ptr [mem],0`) needs /Os.
- A probe of every REFS row (tools/probe.py, 144 rows) before starting gives the per-entry worklist in one pass; it is resumable after a restart.
