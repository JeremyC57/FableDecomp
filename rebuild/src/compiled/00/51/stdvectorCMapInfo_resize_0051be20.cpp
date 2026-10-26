// std::vector<CMapInfo>::resize(unsigned int newSize) -- STLport one-argument resize: resize(newSize, CMapInfo()) with the default map info built and destroyed in the frame
// Leaf: resize
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051BE20, 147 bytes (`ret 4`).
// Identity: the manifest label "CThingCreatureBase::GetAnimsWithNameStartingWith" is propagated. The callee 0x0051B770
//   (manifest "vector<CUIMiniMapMarker>::resize", also propagated) is resize(n, const T&) striding 0x48 bytes
//   (`imul 38E38E39h ; sar 4`, `lea [n+n*8]*8`), and the frame element is built by CMapInfo's own constructor
//   sequence (landed 0x004FDE50: PMap cleared, three CCharStrings, Clear() @ 0x004FDDE0), so T = CMapInfo (Ego_r.pdb 72 bytes).
// Callees: CCharString::CCharString() @ 0x0099E4B0 x3, CMapInfo::Clear @ 0x004FDDE0,
//   vector<CMapInfo>::resize(unsigned int, const CMapInfo&) @ 0x0051B770, CCharString::~CCharString @ 0x0099EAE0 x3,
//   the PMap count block's destroy function (+4, object +8) and operator delete @ 0x00BFE9BC.
// Layout (PDB = retail): Area (C2DBoxI) +0x00, PMap (CCountedPointer<CMap>) +0x10, LevelName +0x18, LevelScriptName +0x1c,
//   MapUID +0x20, Used/Placed/Activated/Loading/LoadedOnPlayerProximity +0x24..+0x28, SerialiseScript +0x2c,
//   LevStamp +0x30, TngStamp +0x38, IsSea +0x40.
// Shape: the landed 0x0056A230 resize(n) form; CMapInfo's ctor and dtor inline (members destroyed in reverse, the
//   counted pointer's release re-reading its block).
void __cdecl operator delete(void* block);              // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};
struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4 per-type deleter, object in ecx
    void* m_pObject;                            // +8
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* m_pCount;
    CCountedPointer() : Data(0), m_pCount(0) { }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
    }
};
class C2DBoxI { public: long Left, Top, Right, Bottom; };
class CMap;
class CMapInfo { public:                               // 0x48 bytes
    C2DBoxI Area;                               // +0x00
    CCountedPointer<CMap> PMap;                 // +0x10
    CCharString LevelName;                      // +0x18
    CCharString LevelScriptName;                // +0x1c
    unsigned long MapUID;                       // +0x20
    bool Used;                                  // +0x24
    bool Placed;                                // +0x25
    bool Activated;                             // +0x26
    bool Loading;                               // +0x27
    bool LoadedOnPlayerProximity;               // +0x28
    CCharString SerialiseScript;                // +0x2c
    unsigned __int64 LevStamp;                  // +0x30
    unsigned __int64 TngStamp;                  // +0x38
    bool IsSea;                                 // +0x40
    CMapInfo() { Clear(); }                     // out of line at 0x004FDE50
    void Clear();                               // 0x004FDDE0
};

struct CMapInfoVector {                         // std::vector<CMapInfo>: first/last/end
    CMapInfo* first;
    CMapInfo* last;
    CMapInfo* end_of_storage;
    void resize(unsigned int newSize, const CMapInfo& value);   // 0x0051B770
    void resize(unsigned int newSize);
};

void CMapInfoVector::resize(unsigned int newSize)
{
    resize(newSize, CMapInfo());
}