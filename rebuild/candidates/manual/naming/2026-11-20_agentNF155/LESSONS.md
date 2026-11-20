# NF155 lessons (one line each)
- Probe HEAD by compiling every REFS row before editing: here it found 7 more wrong binders than WORKLIST.tsv listed, and showed 3 entries were done-already.
- Check `$S/w22/nflocks/<addr>/owner` for EVERY row you edit, including WORKLIST binders that are missing from rows.txt (0x00957F20 was NF153's).
- A free `Dest_val(void*)` stand-in of 15 bytes (`add ecx,8 ; GetPItem ; jmp X`) is a CCreatureActionBase protected forwarder: read Ego_r order next to 0x00692940 GetWorld.
- A local model's class key is what MSVC mangles even when an earlier forward declaration says `class`: re-key the definition in place.
- Moving a protected CThing member into a row's existing local CThing model needs `protected:` / `public:` around it and a forward declaration of its return type.
- Rows with no `// Leaf:` line that are compiled with a big shared header: add the Leaf line (a C++-mangled free fastcall is `?Name@@YI...`, not `@Name@4`).
