#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CEngineInternalPrimitiveMorphInfo::InitialiseCompositeSurfaces(const vector<CCompositeSurfaceDesc>& descs) -- rebuild the key -> composite surface map: reuse the existing surface for each key (or create one through the resource bank), refresh its parameters from the description, collect the pairs into a new map, sort it and swap it in (the old pairs are released)
// Leaf: InitialiseCompositeSurfaces
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BECBA0, length 567 bytes (`ret 4`).
// Callees: CVectorMap::reserve 0x00BEEB80, CBase ctor/dtor 0x0099A2D0 / 0x0099A2E0, CVectorMap::LowerBound 0x00BED2A0,
//   CResourceBank::InitialiseCompositeSurface 0x00B2C460 (global 0x01436EAC), vector::operator= 0x006CADE0,
//   _M_insert_overflow 0x00BEEF90, sort 0x00BEF5C0 (__fastcall first/last + comparator), _Destroy_range 0x00BA9FD0, free 0x00BFEA14.
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* where) { return where; }
#else
inline void* operator new(__SIZE_TYPE__, void* where) { return where; }
#endif
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
namespace N_0099a2d0 { struct CBase {
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();                              // 0x0099A2E0 (vtable 0x01231710)
}; } using N_0099a2d0::CBase;
struct CCPInfo {
    long  References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CSurfaceList { void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CSurfaceList& operator=(const CSurfaceList& other);   // 0x006CADE0
};
struct CCompositeSurfaceDesc {                     // 0x14 bytes
    long         Key;
    long         Value;
    CSurfaceList Surfaces;
};
struct CCompositeSurface {
    unsigned char         _pad[0x20];
    CCompositeSurfaceDesc Desc;                    // +0x20 (Key, Value, Surfaces)
};
struct CCountedPointer {                           // CCountedPointer<CCompositeSurface>: object + count block
    CCompositeSurface* Object;
    CCPInfo*           Info;
    CCountedPointer() : Object(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Info(other.Info) { if (Info) ++Info->References; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPInfo* info = other.Info;             // the source's fields are read first
        CCompositeSurface* object = other.Object;
        if (Info != info) {
            Release();
            Object = object;
            Info = info;
            if (info) ++info->References;
        }
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
    ~CCountedPointer() { Release(); Object = 0; Info = 0; }
};
struct CCompositeSurfaceInfo : CBase {             // vtable 0x012A8154 (implicit virtual dtor)
    CCountedPointer Ptr;
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
template <class T> inline void SwapValues(T& a, T& b) { T tmp = a; a = b; b = tmp; }
// Same-TU body standing in for the STLport sort instantiation 0x00BEF5C0 (comparator unused: keys compared directly).
template <class Compare> __declspec(noinline) void __fastcall SortSurfacePairs(CSurfacePair* first, CSurfacePair* last, Compare)
{
    for (CSurfacePair* i = first; i != last; ++i)
        for (CSurfacePair* j = i + 1; j != last; ++j)
            if (j->first < i->first) {
                long k = i->first; i->first = j->first; j->first = k;
                CCompositeSurfaceInfo t(i->second); i->second.Ptr = j->second.Ptr; j->second.Ptr = t.Ptr;
            }
}
// Same-TU body of the STLport _Destroy_range instantiation 0x00BA9FD0 (tag unused).
template <class Tag> __declspec(noinline) void __fastcall DestroySurfacePairs(CSurfacePair* first, CSurfacePair* last, const Tag&)
{
    for (; first != last; ++first)
        first->~CSurfacePair();
}
struct CCompositeSurfaceMap {                      // CVectorMap<long, CCompositeSurfaceInfo>
    CSurfacePair*       _M_start;
    CSurfacePair*       _M_finish;
    CSurfacePair*       _M_end_of_storage;
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                NeedsSort;                 // +0x0d
    CCompositeSurfaceMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), NeedsSort(false) { }
    ~CCompositeSurfaceMap()
    {
        DestroySurfacePairs(_M_start, _M_finish, __false_type());
        if (_M_start) free(_M_start);
    }
    void reserve(unsigned long n);                 // 0x00BEEB80
    CSurfacePair* LowerBound(const long& key);     // 0x00BED2A0
    template <class Tag> void _M_insert_overflow(CSurfacePair* pos, const CSurfacePair& x, const Tag&, unsigned long n, bool atend);   // 0x00BEEF90
    CSurfacePair* find(const long& key)
    {
        CSurfacePair* it = LowerBound(key);
        CSurfacePair* e = _M_finish;
        if (it == e || key < it->first)
            return e;
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
    __forceinline void Add(const long& key, const CCompositeSurfaceInfo& info) { push_back(CSurfacePair(key, info)); NeedsSort = true; }
    void Sort()
    {
        if (NeedsSort) {
            SortSurfacePairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
    void swap(CCompositeSurfaceMap& other)
    {
        SwapValues(_M_start, other._M_start);
        SwapValues(_M_finish, other._M_finish);
        SwapValues(_M_end_of_storage, other._M_end_of_storage);
        SwapValues(NeedsSort, other.NeedsSort);
    }
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
// Same-TU body of the STLport CCompositeSurfaceMap::_M_insert_overflow instantiation (tag unused).
template <class Tag> __declspec(noinline) void CCompositeSurfaceMap::_M_insert_overflow(CSurfacePair* pos, const CSurfacePair& x, const Tag&, unsigned long n, bool atend)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    CSurfacePair* ns = len ? (CSurfacePair*)malloc(len * sizeof(CSurfacePair)) : 0;
    CSurfacePair* nf = ns;
    CSurfacePair* p;
    for (p = _M_start; p != pos; ++p) { new (nf) CSurfacePair(*p); ++nf; }
    for (; n != 0; --n) { new (nf) CSurfacePair(x); ++nf; }
    if (!atend)
        for (p = pos; p != _M_finish; ++p) { new (nf) CSurfacePair(*p); ++nf; }
    for (p = _M_start; p != _M_finish; ++p) p->~CSurfacePair();
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
// Same-TU body of CVectorMap::reserve 0x00BEEB80 (STLport vector::reserve).
__declspec(noinline) void CCompositeSurfaceMap::reserve(unsigned long n)
{
    if ((unsigned long)(_M_end_of_storage - _M_start) < n) {
        unsigned long oldSize = _M_finish - _M_start;
        CSurfacePair* ns = (CSurfacePair*)malloc(n * sizeof(CSurfacePair));
        CSurfacePair* nf = ns;
        for (CSurfacePair* p = _M_start; p != _M_finish; ++p) { new (nf) CSurfacePair(*p); ++nf; }
        if (_M_start) {
            DestroySurfacePairs(_M_start, _M_finish, __false_type());
            free(_M_start);
        }
        _M_start = ns;
        _M_finish = ns + oldSize;
        _M_end_of_storage = ns + n;
    }
}
// Same-TU body of CVectorMap::LowerBound 0x00BED2A0 (binary search on the sorted keys).
__declspec(noinline) CSurfacePair* CCompositeSurfaceMap::LowerBound(const long& key)
{
    CSurfacePair* first = _M_start;
    unsigned long len = _M_finish - _M_start;
    while (len > 0) {
        unsigned long half = len >> 1;
        CSurfacePair* middle = first + half;
        if (middle->first < key) { first = middle + 1; len = len - half - 1; }
        else len = half;
    }
    return first;
}
struct CResourceBank {
    void InitialiseCompositeSurface(CCompositeSurfaceInfo* info);   // 0x00B2C460
};
extern CResourceBank* g_pResourceBank_01436eac;    // 0x01436EAC

struct CEngineInternalPrimitiveMorphInfo {
    unsigned char        _pad00[0x10];
    CCompositeSurfaceMap CompositeSurfaces;        // +0x10
    void InitialiseCompositeSurfaces(const CDescVector& descs);
};

void CEngineInternalPrimitiveMorphInfo::InitialiseCompositeSurfaces(const CDescVector& descs)
{
    unsigned long count = descs._M_finish - descs._M_start;
    CCompositeSurfaceMap surfaces;
    surfaces.reserve(count);
    for (unsigned long i = 0; i < count; ++i) {
        const CCompositeSurfaceDesc& desc = descs._M_start[i];
        CCompositeSurfaceInfo info;
        CSurfacePair* it = CompositeSurfaces.find(desc.Key);
        if (it != CompositeSurfaces._M_finish)
            info = it->second;
        else
            g_pResourceBank_01436eac->InitialiseCompositeSurface(&info);
        info.Ptr.Object->Desc = desc;
        surfaces.Add(desc.Key, info);
    }
    surfaces.Sort();
    CompositeSurfaces.swap(surfaces);
}

// ---- behaviour-test stubs ----
static int g_bases, g_deleted, g_destroyed, g_created, g_listCopies;
CBase::CBase() { ++g_bases; }
CBase::~CBase() { }
void __cdecl operator delete(void* p) { ++g_deleted; free(p); }
CSurfaceList& CSurfaceList::operator=(const CSurfaceList& other)
{
    _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage;
    ++g_listCopies;
    return *this;
}
static void __fastcall DestroySurface(void* object) { ++g_destroyed; free(object); }
void CResourceBank::InitialiseCompositeSurface(CCompositeSurfaceInfo* info)
{
    CCompositeSurface* surface = (CCompositeSurface*)malloc(sizeof(CCompositeSurface));
    CCPInfo* block = (CCPInfo*)malloc(sizeof(CCPInfo));
    block->References = 1; block->Destroy = DestroySurface; block->Object = surface;
    info->Ptr.Object = surface; info->Ptr.Info = block;
    ++g_created;
}
static CResourceBank g_bank;
CResourceBank* g_pResourceBank_01436eac = &g_bank;
static CCompositeSurfaceDesc g_descs[4];
static void SetDescs(CDescVector& v, const long* keys, int n)
{
    for (int i = 0; i < n; ++i) {
        g_descs[i].Key = keys[i]; g_descs[i].Value = keys[i] * 10;
        g_descs[i].Surfaces._M_start = &g_descs[i]; g_descs[i].Surfaces._M_finish = 0; g_descs[i].Surfaces._M_end_of_storage = 0;
    }
    v._M_start = g_descs; v._M_finish = g_descs + n; v._M_end_of_storage = g_descs + n;
}
int main()
{
    int ok = 1;
    CEngineInternalPrimitiveMorphInfo* m = (CEngineInternalPrimitiveMorphInfo*)malloc(sizeof(CEngineInternalPrimitiveMorphInfo));
    new (&m->CompositeSurfaces) CCompositeSurfaceMap();
    CDescVector v;
    // 1: one new key -> created through the bank
    long k1[1] = { 5 };
    SetDescs(v, k1, 1);
    m->InitialiseCompositeSurfaces(v);
    CCompositeSurfaceMap& map = m->CompositeSurfaces;
    if (map._M_finish - map._M_start != 1 || g_created != 1 || map._M_start[0].first != 5 || map._M_start[0].second.Ptr.Info->References != 1 ||
        map._M_start[0].second.Ptr.Object->Desc.Value != 50 || map._M_start[0].second.Ptr.Object->Desc.Surfaces._M_start != &g_descs[0] || map.NeedsSort) { ok = 0; printf("case1\n"); }
    CCompositeSurface* s5 = map._M_start[0].second.Ptr.Object;
    // 2: 7 new, 5 reused, 3 new -> sorted 3,5,7, surface 5 shared and refreshed
    long k2[3] = { 7, 5, 3 };
    SetDescs(v, k2, 3);
    g_descs[1].Value = 99;
    m->InitialiseCompositeSurfaces(v);
    if (map._M_finish - map._M_start != 3 || g_created != 3 || map._M_start[0].first != 3 || map._M_start[1].first != 5 || map._M_start[2].first != 7 ||
        map._M_start[1].second.Ptr.Object != s5 || s5->Desc.Value != 99 || s5->Desc.Key != 5 || map._M_start[1].second.Ptr.Info->References != 1 ||
        map._M_start[0].second.Ptr.Info->References != 1 || map._M_start[2].second.Ptr.Object->Desc.Value != 70 || g_destroyed != 0 || map.NeedsSort ||
        g_listCopies != 4) { ok = 0; printf("case2 %d %d\n", g_created, g_destroyed); }
    // 3: empty description list -> every old surface released
    SetDescs(v, k2, 0);
    m->InitialiseCompositeSurfaces(v);
    if (map._M_start != map._M_finish || g_destroyed != 3 || g_deleted != 3) { ok = 0; printf("case3 %d %d\n", g_destroyed, g_deleted); }
    if (ok) printf("00becba0_TEST PASS\n");
    return ok ? 0 : 1;
}