# NF127 lessons (one line each)
- Probe every REFS row once up front (tools/probe.py, ~436 rows in ~25 min); then classify untouched rows with a script (work/cls.py) instead of re-reading sources.
- A header opt-in block without its own access label inherits whatever label precedes it, which varies by the includer's other opt-ins (PEEK after FINDAGUARD_YELL is public): add a nested `protected:` / `public:` pair under a member-specific macro.
- When an in-place template respelling changes a whole local class (non-template WorkBase -> CAIStateGroup_WorkBase<CAIStateGroupBase>), every other member on it changes spelling too: point them at the landed self rows' spellings (HasSeller QBE, AcquireSeller IAE) or you create a third spelling.
- A header template body can differ from a hand-modelled same-TU body only in where a by-value argument is homed: a cast temporary as the push_back argument (`static_cast<T*>(thing)`) reproduced the old row's late store and register order (0x00910A90).
- A regex that deletes "the declaration line" must stop at the semicolon: one-line class bodies with several members (0x0091E4B0) lose their neighbours otherwise.
- Never assert "old token not in file" after inserting a comment that names the old token.
- Raw-byte REFS hits inside data or misaligned code (0x0045912F) are false binders: disassemble the site before porting anything.
- An empty `ret 10h` body is only safe to name after an exe scan shows a single caller and no absolute reference (no ICF fold).
