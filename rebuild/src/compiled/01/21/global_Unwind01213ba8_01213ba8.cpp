// EH funclet @ 0x01213BA8 of CSegmentFile::CSegmentFile (parent @ 0x00C2DA20): VC7.1 emits the unwind funclets and the __ehhandler thunk of an
// EH-framed function as one block after it; the linker placed Fable's blocks at 0x0121xxxx. This row has no code of its own:
// it is produced by the parent's translation unit, included below (Codex pass 23 proved the parent, its EH data and this block).
// Leaf: ??0CSegmentFile@@QAE@PAXK@Z
// Flags: /EHsc /GS
#include "../src/compiled/00/c2/CSegmentFile_CSegmentFile_00c2da20.cpp"