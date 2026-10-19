# Retry section200F: 5 of 5 byte-exact

All five candidates are RELOCATION_MATCH at base /O2 /Oy /W3. Exact-source,
expanded-header fixtures pass g++ C++98/O2 and VC7.1 Od and O2. See
NATIVE_AUDIT.md for corrected identities, signatures, native layout and lifetime
limits, and logs/ for compiler/runtime evidence.

Historical four UNCERTAIN reservations remain for every row. New grades:
CE4B70=1, CE5620=3, CE56E0=1, CE5E10=1, CE6A40=1. CE5620's first grade was a
real compiler failure in native offset assertions; its second was DIFFER188v179;
its third matched after branching within the poll loop instead of retesting
flags/messages after a compound loop condition. All grade snapshots preserve
source/hash, full q command and effective flags before compilation, and full
stdout/stderr, including failures. No source/flag grades were spent after a match.
A root-approved comment-only correction replaced the wrong Game VT address with
01260F0C; logs/metadata-correction.json keeps before/after hashes and token-equality
was asserted. Separate verification logs record the unchanged-token compiles.

No parked or skipped rows. No canonical files, shared tools or headers changed.
