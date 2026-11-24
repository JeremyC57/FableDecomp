# NF271 lessons
- A plan's "callee address" for a stand-in can be wrong even when two hand-backs repeat it: read the table line's target column and disassemble the call sites before choosing the unit (FreeAlignedVirtual was 0x00BD9CA0, not 0x00BD9B90, so the unit gained a self row).
- A global-only move is safe exactly when the stand-in pointee holds no callee spellings: check what the pointee struct declares, not only the global, before drawing the unit (0x00B684F0 yes; the CEngineGlobal four no).
- A shared stand-in spelling with several different pointee models moves only as a whole family; one movable member of the family is count-neutral on its own.
- A planted-token control for an opt-in inside a non-default branch should be planted in that branch: it then flags exactly the branch's includers (28 here), which are the only rows the new macro could reach.
- Keep every read-only Bash call ending in `; true`: cleaning the current directory or a `kill -0` on a finished PID makes the call exit 1.
