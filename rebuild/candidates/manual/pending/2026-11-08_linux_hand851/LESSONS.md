# Chunk 851 lessons (GOTCHAS one-liners)

- (chunk 851, 0x00BA1B00) A closing run of "stream 0 off, indices off, every used texture stage released" that keeps the render manager in ebp and &PD3DDevice in esi is the inline CRenderManagerCore::FlushRender (FableWin.pdb-only 0x02CABE80) as a same-TU __forceinline body. Written as separate statements, VC7.1 re-reads the manager per statement (md37).
- (chunk 851, 0x00BA1B00) CIndexBuffer::Lock takes the lock flags first: Lock(flags, 0, 0), so retail pushes 0, 0, flags.
- (chunk 851, 0x00A9BBE0) A CIVCountedPointer release that loads the count before `mov ecx,eax` is the pointee's own Release deleting itself at zero (`Object->Release(); Object = 0;`). The cached-copy form with Delete in the caller moves the copy above the load.
- (chunk 851, 0x00A9BBE0) Codex chunk-class rebases: wrap each generated header as `#ifdef FABLE_RETAIL_<CLASS>` retail class / `#else` original / `#endif`. An abstract class (missing overrides) fails only at the `new` site.
- (chunk 851, 0x006001E0) A list header pointer kept in esi across GetAllLeafThings needs the template body in the TU. Map::operator[] keys stored after the argument push need the operator[] body in the TU, as in 0x0047A9F0.
- (chunk 851, 0x0075E4C0) A GetEntrySymbol result re-addressed with `lea` by the following push_back is a named CCharString local, not a temporary.
- (chunk 851, 0x005FE840) The set<long> member-template find split (retail calls 3 sites and expands only loop 2's core test) is the function-wide inline budget. No /d2inlT value gives one of four (T68-72 expand all four, T75+ three), and /d2inlS changes nothing. Park it early.
- (chunk 851) Before hand-back, check that each overlay copy still contains the repo file as a subsequence. Chunk 850 landed CIVCountedPointer / CIVCountedPointeeBase / C3DMeshFileEngineMaterialChunk edits mid-session, so the copies had to be redone.
- (chunk 851, 0x00402510) A Codex rebase whose class needs a polymorphic CBankFile (virtual slot call or a CBankFile-qualified member) cannot be done while engine/CBankFile.h is HOLD. Park it as header debt at once instead of building its 80-header closure.
