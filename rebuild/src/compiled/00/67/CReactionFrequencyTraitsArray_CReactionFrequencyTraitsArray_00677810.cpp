// CReactionFrequencyTraitsArray::~CReactionFrequencyTraitsArray() -- delete the low- and high-frequency traits object of each of the 79 reaction types
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00677810, length 40 bytes (+ int3 padding). A byte-identical copy of the
// same inline destructor sits at 0x0067A520 (emitted by a second translation unit).
// Callees: operator delete @ 0x00BFE9BC (x2 per iteration).
// Layout (PDB, Ego_r.pdb): `CReactionFrequencyTraits* LowHigh[2][79]` -- 0x278 bytes, the high row at +0x13C
// (79 * 4); no engine header exists for this class.
// Shape: one counted loop over the reaction types (`cmp esi,4Fh ; jb` -- an unsigned index), deleting row 0 then
// row 1 of the same column; each `delete` is a bare `operator delete` call (no null test, no destructor call), so
// CReactionFrequencyTraits has a trivial, non-virtual destructor in this build. The class itself is not polymorphic
// (no vtable store) and is never deleted here, so the TU emits only `??1`.
// Leaf: ??1CReactionFrequencyTraitsArray
class CReactionFrequencyTraits { public:
    void* __vftable;                                    // +0x00 (polymorphic in the engine; its destructor is trivial)
};

class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];           // +0x000 low row, +0x13C high row
    ~CReactionFrequencyTraitsArray();
};

CReactionFrequencyTraitsArray::~CReactionFrequencyTraitsArray()
{
    for (unsigned long i = 0; i < 79; ++i) {
        delete LowHigh[0][i];
        delete LowHigh[1][i];
    }
}