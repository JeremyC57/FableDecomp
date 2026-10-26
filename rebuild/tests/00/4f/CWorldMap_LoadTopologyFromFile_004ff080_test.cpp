// Behaviour test for 004ff080: the model below is a copy of src/004ff080.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_mallocs = 0, g_frees = 0, g_resets = 0, g_maps = 0, g_updates = 0, g_edges = 0, g_seedCalls = 0;
static void* t_malloc(unsigned int size) { ++g_mallocs; return std::malloc(size); }
static void t_free(void* p) { ++g_frees; std::free(p); }
static void* t_memmove(void* d, const void* s, unsigned int n) { return std::memmove(d, s, n); }
#define malloc t_malloc
#define free t_free
#define memmove t_memmove

class CCPPointerInfo { public:
    long  Count;
    void (__fastcall* Destroy)(void* data);
    void* Data;
};
class CMap;
static CMap* g_resetWith = 0;
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info && --Info->Count == 0)
        {
            Info->Destroy(Info->Data);
            delete Info;
        }
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    void Reset(T* data) { ++g_resets; Data = data; g_resetWith = (CMap*)data; }
};

struct __true_type { __true_type() { } };
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void _M_insert_overflow(long* position, const long& x, const __true_type& tag, unsigned long n, bool atEnd);
    void push_back(const long& x)
    {
        if (_M_finish != _M_end_of_storage)
        {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
static long* CopyTrivial(const long* first, const long* last, long* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (long*)((char*)memmove(result, first, n) + n);
}
void CLongVector::_M_insert_overflow(long* position, const long& x, const __true_type&, unsigned long n, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    long* newStart = len ? (long*)malloc(len * sizeof(long)) : 0;
    long* newFinish = CopyTrivial(_M_start, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, _M_finish, newFinish);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CAFile { public: int tag; };
class CASuspendableProcess { public:
    virtual ~CASuspendableProcess() {}
    virtual void v01() = 0;
    virtual void Update() = 0;
};
class CGameDefinitionManager { public: int tag; };
static unsigned long g_seed = 77;
class CWorld { public:
    unsigned char                 _pad00[0x10];
    const CGameDefinitionManager* DefinitionManager;
    unsigned long* GetWorldSeed() const { ++g_seedCalls; return &g_seed; }
};
struct CMapArgs { const CGameDefinitionManager* defs; CWorld* world; unsigned long* seed; long map; C2DBoxI area; CAFile* file; CASuspendableProcess* process; };
static CMapArgs g_mapArgs;
class CMap { public:
    CMap(const CGameDefinitionManager& definitions, CWorld& world, unsigned long* seed, long map,
         const C2DBoxI& area, CAFile& file, CASuspendableProcess* process)
    { ++g_maps; g_mapArgs.defs = &definitions; g_mapArgs.world = &world; g_mapArgs.seed = seed; g_mapArgs.map = map; g_mapArgs.area = area; g_mapArgs.file = &file; g_mapArgs.process = process; }
    unsigned char _body[0x1d40];
};
class CMapInfo { public:
    C2DBoxI               Area;
    CCountedPointer<CMap> PMap;
    unsigned char         _rest[0x30];
};
template <class T> class CArray { public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    T* _SweepBegin1() const { return Start; }
    T& operator[](unsigned long i) { return *(_SweepBegin1() + i); }
};
class CWorldMap { public:
    void*            _vptr;
    long             f4;
    CWorld*          World;
    unsigned char    _pad0c[0x14];
    CArray<CMapInfo> Maps;
    unsigned char    _pad2c[0x64];
    CLongVector      LoadedMaps;
    void UpdateAllEdges() { ++g_edges; }
    const CGameDefinitionManager& WorldDefinitionManager() const { return *World->DefinitionManager; }
    void LoadTopologyFromFile(long map, CCountedPointer<CAFile> file, CASuspendableProcess& process);
};

void CWorldMap::LoadTopologyFromFile(long map, CCountedPointer<CAFile> file, CASuspendableProcess& process)
{
    Maps[map].PMap.Reset(new CMap(WorldDefinitionManager(), *World, World->GetWorldSeed(), map, Maps[map].Area,
                                  *file, &process));
    LoadedMaps.push_back(map);
    process.Update();
    UpdateAllEdges();
}

struct TestProcess : CASuspendableProcess { virtual void v01() {} virtual void Update() { ++g_updates; } };
static bool Run(long map, int loadedCount, int capacity, bool expectGrow)
{
    static CGameDefinitionManager defs; static CWorld world; static CMapInfo infos[3]; static CAFile fileObj; static TestProcess process;
    std::memset(&world, 0, sizeof(world)); world.DefinitionManager = &defs;
    for (int i = 0; i < 3; ++i) { infos[i].Area.TLX = i; infos[i].Area.TLY = i + 10; infos[i].Area.BRX = i + 20; infos[i].Area.BRY = i + 30; infos[i].PMap.Data = 0; infos[i].PMap.Info = 0; }
    long* loaded = (long*)t_malloc(sizeof(long) * (capacity ? capacity : 1));
    for (int i = 0; i < loadedCount; ++i) loaded[i] = 100 + i;
    CWorldMap wm; std::memset((void*)&wm, 0, sizeof(wm)); wm.World = &world; wm.Maps.Start = infos; wm.Maps.Finish = infos + 3; wm.Maps.EndOfStorage = infos + 3;
    wm.LoadedMaps._M_start = capacity ? loaded : 0; wm.LoadedMaps._M_finish = capacity ? loaded + loadedCount : 0; wm.LoadedMaps._M_end_of_storage = capacity ? loaded + capacity : 0;
    CCountedPointer<CAFile> file; file.Data = &fileObj; file.Info = 0;
    g_mallocs = g_frees = g_resets = g_maps = g_updates = g_edges = g_seedCalls = 0; g_resetWith = 0; std::memset(&g_mapArgs, 0, sizeof(g_mapArgs));
    wm.LoadTopologyFromFile(map, file, process);
    bool ok = g_maps == 1 && g_resets == 1 && infos[map].PMap.Data == g_resetWith && g_resetWith != 0;
    ok = ok && g_mapArgs.defs == &defs && g_mapArgs.world == &world && g_mapArgs.seed == &g_seed && g_mapArgs.map == map && g_mapArgs.file == &fileObj && g_mapArgs.process == &process;
    ok = ok && g_mapArgs.area.TLX == infos[map].Area.TLX && g_mapArgs.area.BRY == infos[map].Area.BRY && g_seedCalls == 1;
    ok = ok && g_updates == 1 && g_edges == 1;
    long n = (long)(wm.LoadedMaps._M_finish - wm.LoadedMaps._M_start);
    ok = ok && n == loadedCount + 1 && wm.LoadedMaps._M_start[loadedCount] == map;
    for (int i = 0; i < loadedCount; ++i) ok = ok && wm.LoadedMaps._M_start[i] == 100 + i;
    ok = ok && (expectGrow ? (g_mallocs == 1 && g_frees == (capacity ? 1 : 0)) : (g_mallocs == 0 && g_frees == 0));
    delete (CMap*)g_resetWith;
    if (expectGrow) t_free(wm.LoadedMaps._M_start); else t_free(loaded);
    if (expectGrow && !capacity) t_free(loaded);
    return ok;
}
int main()
{
    bool ok = Run(1, 1, 4, false)      // room left: appended in place
           && Run(2, 2, 2, true)       // full: reallocated (old block freed)
           && Run(0, 0, 0, true);      // empty vector: first allocation
    if (!ok) { std::printf("004ff080_TEST FAIL\n"); return 1; }
    std::printf("004ff080_TEST PASS\n");
    return 0;
}