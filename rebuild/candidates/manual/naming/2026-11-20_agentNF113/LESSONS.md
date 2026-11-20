# NF113 lessons (one line each)
- A queue line whose correct_symbol "differs" from the landed leaf is often only a bad Leaf line (bare name, trailing note, missing line): compile the self row and read its defined symbol before porting anything.
- Read the retail call body before trusting a stand-in's verb: 0x006556D3 "Destroy_range" pushes the value and calls operator= per element, i.e. STLport fill (and fill in this repo is a /Gr fastcall instance).
- STLport list::clear is the inline `_Base::clear()`: the out-of-line COMDAT is _List_base<T>::clear; a list<T>::clear claim comes from Dinkumware FableWin.
- A pair-diff of the HEAD and new objects (tools/pdiff.sh) proves an in-place respell changes only the intended binding, without reading the whole object.
- In-place respelling of a pre-existing local model fixes access/const/nesting cheaply (nest a struct, add `private:` + `friend`), and can pull a second wrong callee (CalculateArmorValues) onto the PDB spelling in the same edit.
- A derived-class stand-in that declares base members (CTCTavernGameCardPairs::SetStateAfterOK) cannot be respelled in place: it needs the real base header, so plan it as a port.
- Stage a self rename in work/ when one REFS row cannot follow; the gate rule is per REFS row, not per spelling count.
