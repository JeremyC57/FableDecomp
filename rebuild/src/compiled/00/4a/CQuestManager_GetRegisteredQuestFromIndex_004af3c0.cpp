// CQuestManager::GetRegisteredQuestFromIndex(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF3C0, oracle length 38 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jl E ; mov edx,[ecx+30h] ; sub edx,[ecx+2Ch] ; sar edx,2 ; cmp eax,edx ; jae E ; mov ecx,[ecx+2Ch] ; lea eax,[ecx+eax*4] ; ret 4 ; E: mov eax,offset g_EmptyQuestName ; ret 4
// RegisteredQuestNames is a std::vector<CCharString> (4-byte elements). In the shipped binary a
// std::vector is {first, last, end} (12 bytes, `first` at +0), 4 bytes smaller than in Ego_r.pdb;
// the two containers before it move the vector from the header's +0x34 to +0x2C, hence the
// retail-offset view. Out of range indices (negative, or >= size as an unsigned compare) return
// the shared empty name at 0x013BD804 instead of null.
//
// Lane feedback: a raw `first[index]` keeps `first` in a register and speculates the `lea` before the
// branch (34 bytes); the retail-vector iterator object reproduces the reload. RELOCATION_MATCH.
// Retail std::vector (12 bytes: first/last/end, no allocator slot) with an iterator OBJECT.
// VC7.1 does not fold `*(begin() + n)` through the iterator temporary the way it folds
// `first[n]`: the element pointer is materialised and the `first` load is not reused, which
// is exactly the retail instruction order. A plain `first[n]` differs by 2-4 bytes.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

class CCharString { public: void* PStringData; };

extern CCharString g_EmptyQuestName;       // 0x013BD804 -- returned for an out-of-range index

// Retail CQuestManager view: RegisteredQuestNames (vector<CCharString>) at +0x2C (PDB +0x34).
struct CQuestManager_Retail {
    unsigned char              _pad_0x00[0x2C];
    CRetailVector<CCharString> RegisteredQuestNames;   // +0x2C
    CCharString* GetRegisteredQuestFromIndex(long index);
};

CCharString* CQuestManager_Retail::GetRegisteredQuestFromIndex(long index)
{
    // Negative or past-the-end index => the shared empty name; `size()` is `(last-first)>>2`
    // straight from memory and the element pointer is re-derived through the iterator, which
    // is why retail reloads `[ecx+2Ch]` after the bounds check.
    if (index >= 0 && (unsigned long)index < this->RegisteredQuestNames.size())
        return &this->RegisteredQuestNames[index];
    return &g_EmptyQuestName;
}