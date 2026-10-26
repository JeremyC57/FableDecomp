// _global::_Destroy_range_CMapInfo(CMapInfo* first, CMapInfo* last, SAllocator alloc) -- run the inlined ~CMapInfo over [first, last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00515250, length 101 bytes (padded to 112).
// Leaf: _Destroy_range_CMapInfo
// Label: the catalog "_Destroy_range<std::allocator<CMapInfo>>" is right about the element: the stride is 0x48 and the
// destroyed members sit exactly where Ego_r.pdb puts CMapInfo's (72 bytes): SerialiseScript +0x2c, LevelScriptName +0x1c,
// LevelName +0x18 (three ~CCharString @ 0x0099EAE0) and PMap +0x10 (CCountedPointer<CMap>: object, count block).
// Convention: LTCG register form -- first in ecx, last in edx, the by-value (empty) allocator on the stack (`ret 4`).
// Callees: CCharString::~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC (count block).
// Shape: the implicit CMapInfo destructor is inlined: members in reverse order, the counted pointer releases through
// its block (decrement, at zero call the block's destroy function with the object in ecx, delete the block) and then
// nulls both words. VC strength-reduces the element to `esi = &first->PMap.Block` (stepped with edi).
class CCharString { public:
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};
class CMap;
struct CCountedPointerCMap {
    CMap* m_pObject;                        // +0
    SCountBlock* m_pCount;                  // +4
    ~CCountedPointerCMap()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
class C2DBoxI { public: long x0, y0, x1, y1; };
class CMapInfo { public:                           // Ego_r.pdb, 72 bytes
    C2DBoxI Area;                           // +0x00
    CCountedPointerCMap PMap;               // +0x10
    CCharString LevelName;                  // +0x18
    CCharString LevelScriptName;            // +0x1c
    unsigned long MapUID;                   // +0x20
    bool Used;                              // +0x24
    bool Placed;                            // +0x25
    bool Activated;                         // +0x26
    bool Loading;                           // +0x27
    bool LoadedOnPlayerProximity;           // +0x28
    CCharString SerialiseScript;            // +0x2c
    unsigned __int64 LevStamp;              // +0x30
    unsigned __int64 TngStamp;              // +0x38
    bool IsSea;                             // +0x40
};
struct SAllocator { };

void __fastcall _Destroy_range_CMapInfo(CMapInfo* first, CMapInfo* last, SAllocator alloc)
{
    for (; first != last; ++first)
        first->~CMapInfo();
}