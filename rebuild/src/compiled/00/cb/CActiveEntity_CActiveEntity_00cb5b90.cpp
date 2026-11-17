// CActiveEntity::~CActiveEntity() -- remove every particle morph named in the morph-name list from the particle
// Grade: RELOCATION_MATCH -- landing retry: the name list cleared through an inline clear() { erase(begin(), end()); } (bounds read through the vector this)
// morph manager singleton (created on first use), clear the list, then destroy both member vectors.
// Leaf: ??1CActiveEntity
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE (header opt-in) -- retail expands the m_morphNames ~vector<CCharString>
//   (the destroy loop and free, end cached); the stand-in carried the same body as a __forceinline destructor.
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
#include "fable_compiler.h"
// NF48 (round 7): m_morphNames is STLport 4.6.2 std::vector<CCharString> from the shared stlport/fable_containers.h (a local
// CMorphNameVector stand-in before): clear() is the header's inline erase(begin(), end()) over the out-of-line
// vector<CCharString>::erase(iterator, iterator) 0x0043336A (FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE); its destructor is the
// header body, expanded (FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE, the stand-in's __forceinline).
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE
#include "stlport/fable_containers.h"
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

class CCharString { public:
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

FABLE_FORCEINLINE CParticleMorphManager* GetParticleMorphManager()
{
    if (!g_pParticleMorphManager.m_pObject)
        g_pParticleMorphManager.Reset(new(operator new(0x2c)) CParticleMorphManager());
    return g_pParticleMorphManager.m_pObject;
}


struct CMorphEntry {                                // 0x18 bytes: two strings
    CCharString Name;                               // +0x00
    unsigned char _pad04[8];
    CCharString Group;                              // +0x0c
    unsigned char _pad10[8];
};

FABLE_FORCEINLINE void DestroyRange(CMorphEntry* first, CMorphEntry* last)
{
    for (; first != last; ++first)
        first->~CMorphEntry();
}

struct CMorphEntryVector {                          // STLport vector<CMorphEntry>
    CMorphEntry* m_first;
    CMorphEntry* m_last;
    CMorphEntry* m_end;
    FABLE_FORCEINLINE ~CMorphEntryVector()
    {
        DestroyRange(m_first, m_last);
        if (m_first)
            free(m_first);
    }
};

struct CActiveEntity {
    long             m_unknown0;                    // +0x00
    CMorphEntryVector m_morphs;                     // +0x04
    unsigned char    _pad10[4];
    std::vector<CCharString> m_morphNames;                  // +0x14
    FABLE_FORCEINLINE void ResetParticleMorphs()
    {
        for (CCharString* it = m_morphNames.begin(); it != m_morphNames.end(); ++it)
            GetParticleMorphManager()->RemoveMorph(*it);
        m_morphNames.clear();
    }
    ~CActiveEntity();
};

CActiveEntity::~CActiveEntity()
{
    ResetParticleMorphs();
}