// CKeyedBuffer_00bebe90::Assign(const CRetailTree_erase004aba70& keys) -- copy the key tree in and
// empty the byte buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBE90, length 56 bytes (+ int3 padding).
// Callees: the retail red-black tree's operator= @ 0x006B9E00 (self-assignment check, then erase
// through 0x004ABA70 -- the tree family the landed CRetailTree_erase004aba70 destructor @ 0x004AD4A0
// names; the "LTextTreeWalkThrough_CopyAssign" label is propagated) on the member at +0x20,
// then memmove @ 0x00BFEAE6 from the inlined SGI `vector<char>::erase(begin(), end())`:
// `__copy_trivial(last, finish, first)` is `(last == first) ? result : memmove(result, first,
// n) + n` with `last` and `finish` the same load, which is why retail compares a register with
// itself (`cmp eax,eax`), takes the memmove path never, and otherwise stores `finish = first`.
// Layout: +0x20 the tree {head, size}, +0x2c char* first, +0x30 char* finish (0x34 bytes).
// The manifest label "CCreatureAbilityDef::Copy" is propagated and rejected (the PDB
// CCreatureAbilityDef has no tree member); the argument is the tree itself, not a def.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct CRetailTree_erase004aba70 {             // std::map/set: head + size
    void* head;
    unsigned long size;
    CRetailTree_erase004aba70& operator=(const CRetailTree_erase004aba70& other);   // 0x006B9E00
};

struct CRetailVector_char {                    // SGI vector<char>: start / finish (end-of-storage not touched here)
    char* first;
    char* finish;
    static __forceinline char* CopyTrivial(const char* copyFirst, const char* copyLast, char* result)
    {
        return (copyLast == copyFirst) ? result
             : static_cast<char*>(memmove(result, copyFirst, copyLast - copyFirst)) + (copyLast - copyFirst);
    }
    __forceinline char* erase(char* eraseFirst, char* eraseLast)
    {
        char* i = CopyTrivial(eraseLast, finish, eraseFirst);
        finish = i;
        return eraseFirst;
    }
    __forceinline void clear() { erase(first, finish); }
};

struct CKeyedBuffer_00bebe90 {
    unsigned char             _pad[0x20];      // +0x00..+0x20
    CRetailTree_erase004aba70 m_keys;          // +0x20
    unsigned char             _pad28[4];       // +0x28
    CRetailVector_char        m_buffer;        // +0x2c
    void Assign(const CRetailTree_erase004aba70& keys);
};

void CKeyedBuffer_00bebe90::Assign(const CRetailTree_erase004aba70& keys)
{
    m_keys = keys;
    m_buffer.clear();
}
