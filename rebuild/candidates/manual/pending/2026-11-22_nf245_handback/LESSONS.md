# NF245 lessons (one line each)
- A row-local container stand-in can carry extra members (0x005DCA60's 8-byte list): moving it onto the 4-byte shared list shifts the row's own local layout, so fix the local model to the retail offsets, not the bytes.
- Removing a local std block can turn a declared-only member into a visible inline one (0x009EC890's vector(size_type)): check the defined-symbol diff for new pick-any copies and reach for the existing declaration-only opt-in.
- equiv_check sees only HEAD sources: for a new partial mode also prove that no row defining the macro includes a repo header that re-includes the overlaid header.
- Byte-identical bodies (0x00404690 / 0x004046F0) leave layout as the only source that tells them apart: say so instead of counting pk.py EQ as a second source.
- Anonymous-namespace data symbols change their `?A0x<hash>` with the compiled file name: a relocation diff on them is not a binding change.
