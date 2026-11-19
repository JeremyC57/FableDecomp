# ESCALATE 0x00AE3C30 CParticleProgram::ReadFromFile -- draft DIFFER(2310v2310) 1 hunk; probe/ae_d.cpp 2278v2310

Constraint: retail's texcoord read (0x00AE40AC) is the `_M_start` form of GetAsCArray (begin into eax, size test, `mov edi,eax`,
`ja`, `mov edi,esi`) -- ae_d (FABLE_RETAIL_CARRAY_GETASCARRAY_START, `is.Read(IndexedTextureCoords.GetAsCArray(), size()*8)`, no
local, as Ego_d says) reproduces that block exactly, but then the &IndexedTextureCoords CSE temp is split (resize | Read),
`this` gets an ebx piece (#46) before const 0, and the rest of the function loses ebx (2278 bytes). The draft's named `data`
local (not in Ego_d) keeps the CSE temp spanning resize..Read (temp 153, prio 56, esi), so `this` gets no ebx piece and const 0
keeps ebx as retail -- but its local order is wrong (`mov edi,[esi] ; mov ebx,edi` instead of retail's eax copy after the test).
Measured: GetByteSize, PeekIndexedTextureCoords (const GetAsCArray) spellings: 2278. Needed: a no-local spelling that keeps the
address temp across resize and Read (callee visibility of resize/Read? an inline accessor?) or evidence for the data local.

Addendum: keeping the named local but assigning it test-first (`data = size() > 0 ? begin() : (C2DVector*)&...`, or
`data = GetAsCArray()` with _START) also gives 2278: only the draft's begin-first-then-if form keeps retail's allocation.
