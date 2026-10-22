// Grade: DIFFER(538v569) -- first drafts only: the STLport map push_back/_M_insert_overflow, counted-pointer copy and the swap/destroy tail are not yet reproduced (inline vs out-of-line choices); 4 forms tried
// CEnginePrimitiveMorphInfo::InitialiseCompositeSurfaces(const vector<CCompositeSurfaceDesc>& descs) -- rebuild the key -> composite surface map: reuse the existing surface for each key (or create one through the resource bank), refresh its parameters from the description, collect the pairs into a new map, sort it and swap it in (the old pairs are released)
// Leaf: InitialiseCompositeSurfaces
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BECBA0, length 567 bytes (`ret 4`).
// Callees: CVectorMap::reserve 0x00BEEB80, CBase ctor/dtor 0x0099A2D0 / 0x0099A2E0, CVectorMap::LowerBound 0x00BED2A0,
//   CResourceBank::InitialiseCompositeSurface 0x00B2C460 (global 0x01436EAC), vector::operator= 0x006CADE0,
//   _M_insert_overflow 0x00BEEF90, sort 0x00BEF5C0 (__fastcall first/last + comparator), _Destroy_range 0x00BA9FD0, free 0x00BFEA14.
#include <stdlib.h>
inline void* operator new(unsigned int, void* where) { return where; }
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CBase {
    CBase();                                       // 0x0099A2D0
    ~CBase();                                      // 0x0099A2E0
};
struct CCPInfo {
    long  References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CSurfaceList { void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CSurfaceList& operator=(const CSurfaceList& other);   // 0x006CADE0
};
struct CCompositeSurface {
    unsigned char _pad[0x20];
    long          Key;                             // +0x20
    long          Value;                           // +0x24
    CSurfaceList  Surfaces;                        // +0x28
};
struct CCompositeSurfaceInfo : CBase {             // vtable 0x012A8154
    CCompositeSurface* Object;
    CCPInfo*           Info;
    CCompositeSurfaceInfo() : Object(0), Info(0) { }
    CCompositeSurfaceInfo(const CCompositeSurfaceInfo& other) : Object(other.Object), Info(other.Info) { if (Info) ++Info->References; }
    CCompositeSurfaceInfo& operator=(const CCompositeSurfaceInfo& other)
    {
        Release();
        Object = other.Object;
        Info = other.Info;
        if (Info) ++Info->References;
        return *this;
    }
    void Release()
    {
        if (Info) {
            if (--Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    virtual ~CCompositeSurfaceInfo() { Release(); Object = 0; Info = 0; }
};
struct CCompositeSurfaceDesc {                     // 0x14 bytes
    long         Key;
    long         Value;
    CSurfaceList Surfaces;
};
struct CDescVector { CCompositeSurfaceDesc* _M_start; CCompositeSurfaceDesc* _M_finish; CCompositeSurfaceDesc* _M_end_of_storage; };
struct CSurfacePair {                              // pair<long, CCompositeSurfaceInfo>, 0x10 bytes
    long                  first;
    CCompositeSurfaceInfo second;
    CSurfacePair(const long& key, const CCompositeSurfaceInfo& info) : first(key), second(info) { }
    CSurfacePair(const CSurfacePair& other) : first(other.first), second(other.second) { }
};
struct __false_type { };
struct CKeyPairCompareLess { };
void __fastcall SortSurfacePairs(CSurfacePair* first, CSurfacePair* last, CKeyPairCompareLess comp);   // 0x00BEF5C0
void __fastcall DestroySurfacePairs(CSurfacePair* first, CSurfacePair* last, const void* alloc);       // 0x00BA9FD0
struct CCompositeSurfaceMap {                      // CVectorMap<long, CCompositeSurfaceInfo>
    CSurfacePair*       _M_start;
    CSurfacePair*       _M_finish;
    CSurfacePair*       _M_end_of_storage;
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                NeedsSort;                 // +0x0d
    CCompositeSurfaceMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), NeedsSort(false) { }
    ~CCompositeSurfaceMap()
    {
        DestroySurfacePairs(_M_start, _M_finish, this);
        if (_M_start) free(_M_start);
    }
    void reserve(unsigned long n);                 // 0x00BEEB80
    CSurfacePair* LowerBound(const long& key);     // 0x00BED2A0
    void _M_insert_overflow(CSurfacePair* pos, const CSurfacePair& x, const __false_type&, unsigned long n, bool atEnd);   // 0x00BEEF90
    CSurfacePair* find(const long& key)
    {
        CSurfacePair* it = LowerBound(key);
        if (it == _M_finish || key < it->first)
            it = _M_finish;
        return it;
    }
    void push_back(const CSurfacePair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CSurfacePair(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void Insert(const CSurfacePair& x) { push_back(x); NeedsSort = true; }
    void Sort()
    {
        if (NeedsSort) {
            SortSurfacePairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
    void swap(CCompositeSurfaceMap& other)
    {
        CSurfacePair* s = _M_start; _M_start = other._M_start; other._M_start = s;
        CSurfacePair* f = _M_finish; _M_finish = other._M_finish; other._M_finish = f;
        CSurfacePair* e = _M_end_of_storage; _M_end_of_storage = other._M_end_of_storage; other._M_end_of_storage = e;
        bool n = NeedsSort; NeedsSort = other.NeedsSort; other.NeedsSort = n;
    }
};
struct CResourceBank {
    void InitialiseCompositeSurface(CCompositeSurfaceInfo* info);   // 0x00B2C460
};
extern CResourceBank* g_pResourceBank_01436eac;    // 0x01436EAC

struct CEnginePrimitiveMorphInfo {
    unsigned char        _pad00[0x10];
    CCompositeSurfaceMap CompositeSurfaces;        // +0x10
    void InitialiseCompositeSurfaces(const CDescVector& descs);
};

void CEnginePrimitiveMorphInfo::InitialiseCompositeSurfaces(const CDescVector& descs)
{
    unsigned long count = descs._M_finish - descs._M_start;
    CCompositeSurfaceMap surfaces;
    surfaces.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        const CCompositeSurfaceDesc& desc = descs._M_start[i];
        CCompositeSurfaceInfo info;
        CSurfacePair* it = CompositeSurfaces.find(desc.Key);
        if (it != CompositeSurfaces._M_finish) {
            info.Object = it->second.Object;
            info.Info = it->second.Info;
            if (info.Info) ++info.Info->References;
        }
        else
            g_pResourceBank_01436eac->InitialiseCompositeSurface(&info);
        CCompositeSurface* surface = info.Object;
        surface->Key = desc.Key;
        surface->Value = desc.Value;
        surface->Surfaces = desc.Surfaces;
        surfaces.Insert(CSurfacePair(desc.Key, info));
    }
    surfaces.Sort();
    CompositeSurfaces.swap(surfaces);
}
