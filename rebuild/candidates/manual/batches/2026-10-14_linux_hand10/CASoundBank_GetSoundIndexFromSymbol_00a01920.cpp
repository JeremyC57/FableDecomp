// CASoundBank::GetSoundIndexFromSymbol(const CCharString&) -- look the symbol up in the bank's symbol table, 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01920, length 32 bytes (+ int3 padding).
// Callees: CSoundSymbolTable::Lookup(const CCharString&, long*) @ 0x00A38420 (on the table pointer
// at +0x4; the index out-parameter is the `push ecx` frame slot, pushed first = last argument).
// Shape: `index = 0` is stored from the zeroed eax before the null test of the table; the
// out-parameter is re-read for the result (`ret 4`).
struct CCharString;

struct CSoundSymbolTable {
    void Lookup(const CCharString& symbol, long* index);    // 0x00A38420
};

struct CASoundBank {
    void* __vftable;                            // +0x00
    CSoundSymbolTable* m_pSymbolTable;          // +0x04
    long GetSoundIndexFromSymbol(const CCharString& symbol);
};

long CASoundBank::GetSoundIndexFromSymbol(const CCharString& symbol)
{
    long index = 0;
    if (m_pSymbolTable != 0)
        m_pSymbolTable->Lookup(symbol, &index);
    return index;
}
