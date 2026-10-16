# Lessons from chunk 175
- VC7.1 removes a null test on a pointer it has already dereferenced (`if (c && f(c))` after reading `c->x` loses `test`); retail's thing-search loops keep it, reproduced by re-reading `(T*)*it` for the test instead of using the dereferenced local.
- A template type-mask test `(flags & T::Mask) == T::Mask` folds away for Mask 0; copying the mask into a local first keeps retail's unfolded `xor al,al ; jne` (and `xor dl,dl ; jne`).
- CThingSearchTools GetPFirst/GetPBest(Leaf)Thing: one template model (scratch w22/agent175/gen) matches 31 instantiations; filter callees and masks are relocations/immediates, so the labels' template arguments are often wrong (see NOTES).
- Clearing a CIntelligentPointer member in a dtor: `CIntelligentPointer cleared(0); CThing* item = cleared.GetPItem(); member.SetPItem(item);` puts the member `lea` after GetPItem as retail (the operator= form hoists it).
- A CCharString argument retail builds before any other push, and destroys right after the call, is a named local in its own `{}` block whose result is stored to a bool declared outside.
- A 14-byte PDB record (CEngineGraphic) needs `#pragma pack(2)` to copy as 3 dwords + 1 word.
- A def released with a null test on one arm only (IsExpressionAvailable) is natural: the success arm dereferenced the def; a holder copied from the out slot gives retail's single-register def.
- Float pool 0x0122DEDC is 0.0f.
