// CActiveEntity::~CActiveEntity() -- remove every particle morph named in the morph-name list from the particle
// Grade: RELOCATION_MATCH -- landing retry: the name list cleared through an inline clear() { erase(begin(), end()); } (bounds read through the vector this)
// morph manager singleton (created on first use), clear the list, then destroy both member vectors.
// Leaf: ??1CActiveEntity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5B90, length 209 bytes (+ int3 padding).
// Label correction: the catalog's "CActiveEntity::ResetParticleMorphs" is the inlined body; 0x00CB5B90 itself is a
// destructor -- it frees both vectors' storage without resetting them, and its callers are the scalar deleting
// destructor 0x004A83A0 (`test [esp+8],1 ; operator delete`) and the null-tested delete helper 0x004A9B70 (no vtable
// store: the class is not polymorphic). The class name is the catalog's; layout local.
// Callees: operator new @ 0x00BFEA1A (0x2c bytes), the manager ctor @ 0x009F0320, CCountedPointer::Reset @ 0x00413590 on
// the static manager pointer @ 0x013CAA68, CCharString copy ctor @ 0x0099EC30 (the by-value argument), the manager's
// RemoveMorph(CCharString) @ 0x009F03B0, the name vector's erase(first, last) @ 0x0043336A, ~CCharString @ 0x0099EAE0,
// operator delete @ 0x00BFEA14.
// Shape: the singleton accessor is inline (`if (!p) p.Reset(new T); return p;`) and read before the by-value copy;
// erase is out of line; the two vector destructors use by-value _Destroy(first, last) range helpers (end cached).
void* __cdecl operator_new(unsigned int size);     // 0x00BFEA1A
void __cdecl operator_delete(void* p);             // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

struct CParticleMorphManager {
    unsigned char _body[0x2c];
    CParticleMorphManager();                        // 0x009F0320
    void RemoveMorph(CCharString name);             // 0x009F03B0
};

struct CParticleMorphManagerPointer {               // static CCountedPointer<CParticleMorphManager>
    CParticleMorphManager* m_pObject;               // +0
    void* m_pCount;                                 // +4
    void Reset(CParticleMorphManager* object);      // 0x00413590
};
extern CParticleMorphManagerPointer g_pParticleMorphManager;   // 0x013CAA68

__forceinline CParticleMorphManager* GetParticleMorphManager()
{
    if (!g_pParticleMorphManager.m_pObject)
        g_pParticleMorphManager.Reset(new(operator_new(0x2c)) CParticleMorphManager());
    return g_pParticleMorphManager.m_pObject;
}

__forceinline void DestroyRange(CCharString* first, CCharString* last)
{
    for (; first != last; ++first)
        first->~CCharString();
}

struct CMorphNameVector {                           // STLport vector<CCharString>: first, last, end
    CCharString* m_first;
    CCharString* m_last;
    CCharString* m_end;
    CCharString* erase(CCharString* first, CCharString* last);   // 0x0043336A
    CCharString* begin() const { return m_first; }
    CCharString* end() const { return m_last; }
    void clear() { erase(begin(), end()); }
    __forceinline ~CMorphNameVector()
    {
        DestroyRange(m_first, m_last);
        if (m_first)
            operator_delete(m_first);
    }
};

struct CMorphEntry {                                // 0x18 bytes: two strings
    CCharString Name;                               // +0x00
    unsigned char _pad04[8];
    CCharString Group;                              // +0x0c
    unsigned char _pad10[8];
};

__forceinline void DestroyRange(CMorphEntry* first, CMorphEntry* last)
{
    for (; first != last; ++first)
        first->~CMorphEntry();
}

struct CMorphEntryVector {                          // STLport vector<CMorphEntry>
    CMorphEntry* m_first;
    CMorphEntry* m_last;
    CMorphEntry* m_end;
    __forceinline ~CMorphEntryVector()
    {
        DestroyRange(m_first, m_last);
        if (m_first)
            operator_delete(m_first);
    }
};

struct CActiveEntity {
    long             m_unknown0;                    // +0x00
    CMorphEntryVector m_morphs;                     // +0x04
    unsigned char    _pad10[4];
    CMorphNameVector m_morphNames;                  // +0x14
    __forceinline void ResetParticleMorphs()
    {
        for (CCharString* it = m_morphNames.m_first; it != m_morphNames.m_last; ++it)
            GetParticleMorphManager()->RemoveMorph(*it);
        m_morphNames.clear();
    }
    ~CActiveEntity();
};

CActiveEntity::~CActiveEntity()
{
    ResetParticleMorphs();
}