// CVectorMap<K, CCountedPointer<CEntryScriptBase> >::InsertEntry(const K& key, const CCountedPointer<CEntryScriptBase>& entry) -- append a (key, entry) pair to the map's vector (STLport push_back, overflow out of line) and mark the map unsorted
// Leaf: InsertEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBD5D0, length 145 bytes (padded to 160), `ret 8`.
// Callees: vector<pair<K, CCountedPointer<...> > >::_M_insert_overflow(pos, const T&, const __false_type&, size_type, bool)
// @ 0x00CBC850 (catalog "CVectorMap_EntryScriptBase::AllocateGrowth"), the pair temporary's counted-pointer release: the
// count block's destroy function (+4, object +8 in ecx) and operator delete @ 0x00BFE9BC.
// Layout: CVectorMap = { vector Vector +0 (start, finish +4, end_of_storage +8), Compare +0xc, Dirty +0xd }; element = 16 bytes:
// an 8-byte POD key (two words copied plainly) then CCountedPointer {Data, Info}.
// Shape (landed 0x00CBD310 push_back model): the pair is a frame temporary whose words stay in registers for the inline
// placement copy; the __false_type tag temporary lands in the dead `key` argument slot.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
struct CCPPointerInfo {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CEntryScriptBase;
struct CCountedPointer_CEntryScriptBase {
    CEntryScriptBase* Data;
    CCPPointerInfo*   Info;
    CCountedPointer_CEntryScriptBase(const CCountedPointer_CEntryScriptBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer_CEntryScriptBase()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
};
struct CEntryKey {                                 // the 8-byte map key
    unsigned long Word0;
    unsigned long Word1;
};
struct CEntryPair {
    CEntryKey                        first;
    CCountedPointer_CEntryScriptBase second;
    CEntryPair(const CEntryKey& key, const CCountedPointer_CEntryScriptBase& value) : first(key), second(value) {}
};
struct __false_type { __false_type() {} };
struct CEntryPairVector {
    CEntryPair* m_start;
    CEntryPair* m_finish;
    CEntryPair* m_end_of_storage;
    template <class Tag> void _M_insert_overflow(CEntryPair* position, const CEntryPair& x, const Tag&, unsigned long fill_len, bool atend);   // 0x00CBC850
    void push_back(const CEntryPair& x)
    {
        if (m_finish != m_end_of_storage) {
            new (m_finish) CEntryPair(x);
            ++m_finish;
        }
        else
            _M_insert_overflow(m_finish, x, __false_type(), 1, true);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
// Same-TU body of the STLport CEntryPairVector::_M_insert_overflow instantiation (tag unused).
template <class Tag> __declspec(noinline) void CEntryPairVector::_M_insert_overflow(CEntryPair* pos, const CEntryPair& x, const Tag&, unsigned long n, bool atend)
{
    unsigned long oldSize = m_finish - m_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    CEntryPair* ns = len ? (CEntryPair*)malloc(len * sizeof(CEntryPair)) : 0;
    CEntryPair* nf = ns;
    CEntryPair* p;
    for (p = m_start; p != pos; ++p) { new (nf) CEntryPair(*p); ++nf; }
    for (; n != 0; --n) { new (nf) CEntryPair(x); ++nf; }
    if (!atend)
        for (p = pos; p != m_finish; ++p) { new (nf) CEntryPair(*p); ++nf; }
    for (p = m_start; p != m_finish; ++p) p->~CEntryPair();
    if (m_start)
        free(m_start);
    m_start = ns;
    m_finish = nf;
    m_end_of_storage = ns + len;
}
struct CKeyPairCompareLess {
};
struct CEntryScriptVectorMap {
    CEntryPairVector    Vector;                    // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    void InsertEntry(const CEntryKey& key, const CCountedPointer_CEntryScriptBase& entry);
};
void CEntryScriptVectorMap::InsertEntry(const CEntryKey& key, const CCountedPointer_CEntryScriptBase& entry)
{
    Vector.push_back(CEntryPair(key, entry));
    Dirty = true;
}