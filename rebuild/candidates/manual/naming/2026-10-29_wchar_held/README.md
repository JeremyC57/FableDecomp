# wchar_t wave: verified, NOT landed (owner, 2026-10-29: retail spelling wins)

A full `_W` respelling (361 rows, /Zc:wchar_t, 368/368 rechecked byte-neutral) was built in the scratchpad and held.
Retail evidence (EVIDENCE.md, FINDINGS_LOG 2026-10-29): STLport `uninitialized_fill_n` 0x005123A0 is one instantiation
shared by `vector<unsigned short>` (mesh indices) and the wide basic_string / CWideString, with no identical-code folding
in the image, so retail's wide character type was `unsigned short`. Owner decision: landed code keeps wide text as
`unsigned short` (mangled `G`), as retail compiled it; the PDB `_W` names are the developer builds' spelling and appear
only in labels. DEFERRED.tsv keeps the agent's non-wchar_t leftovers (GetAsCharArray callers, Sprintf / StartFileScan
identities, const members) for a later wave.
