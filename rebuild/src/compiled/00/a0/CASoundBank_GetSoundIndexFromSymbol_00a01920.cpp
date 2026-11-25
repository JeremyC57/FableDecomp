// CASoundBank::GetSoundIndexFromSymbol(const CCharString&) -- look the symbol up in the bank's symbol table, 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01920, length 32 bytes (+ int3 padding).
// Callees: CSoundSymbolTable::Lookup(const CCharString&, long*) @ 0x00A38420 (on the table pointer
// at +0x4; the index out-parameter is the `push ecx` frame slot, pushed first = last argument).
// Shape: `index = 0` is stored from the zeroed eax before the null test of the table; the
// out-parameter is re-read for the result (`ret 4`).
class CCharString;

struct CSoundSymbolTable {
};

class CASoundBank { public:
    void* __vftable;                            // +0x00
    CSoundSymbolTable* m_pSymbolTable;          // +0x04
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00A38420 ?FindSymbol@CCRCSymbolMap@@QBE_NABVCCharString@@AAJ@Z (was ?Lookup@CSoundSymbolTable@@QAEXABVCCharString@@PAJ@Z)
class CCRCSymbolMap { public: bool FindSymbol(CCharString const&, long&) const; };
long CASoundBank::GetSoundIndexFromSymbol(const CCharString& symbol) const
{
    long index = 0;
    if (m_pSymbolTable != 0)
        reinterpret_cast<const CCRCSymbolMap*>(m_pSymbolTable)->FindSymbol(symbol, index);
    return index;
}