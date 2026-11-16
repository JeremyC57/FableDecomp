# NF46 lessons (one line each)
- A row's `#define FABLE_RETAIL_<CLASS>` choice decides which collisions you meet: the CEngineResourceManager retail branch pulls fable_string.h / CCountedPointer.h / CArray.h, the generated branch pulls nothing (use it, with real member names, when a row keeps a local non-template counted pointer and only reads +0x10 / +0x954).
- FableWin inline accessors that return `unsigned long` (CGraphicInfoFrame::GetWidth / GetHeight) decide the int-to-float shape: read the retail `fild dword` vs `fild + jge + fadd 2^32` before choosing a spelling.
- A bare `// Leaf:` (or none) lets q.py grade the "largest function": always dump the self symbol from the obj (xref.py first line) before and after a port.
- `CPixelFormat format; format.Initialise(...)` and `CPixelFormat format(...)` differ by the `-1` store: check retail for the store before picking the header constructor.
- A transformer assertion that fires mid-script leaves the row unchanged but the next g.sh still spends a grade on it: check the transformer's exit before grading.
- Rows written by heredoc lack a trailing newline difference only; compare graded copies with `sed '$a\'` before calling an object stale.
- `ls` of a file that may not exist must end with `; true` (two slips this round).
- Read the WARN RELOC_NAME lines of a recheck against the originals: a callee respelled from a header opt-in name (PeekAnimatedMesh) can still bind the wrong address; check the landed row at that address first.
