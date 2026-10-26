// CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&) -- build the map object from its topology file, record the map as loaded, step the suspendable process and rebuild the map edges
// before the pushes and reads file.Data after the first push; ours loads process/file.Data first (eax/edx) and Maps.Start late
// Leaf: LoadTopologyFromFile
// Retail: Fable.exe @ 0x004FF080, 198 bytes, `ret 10h`. PDB ?LoadTopologyFromFile@CWorldMap@@UAEXJV?$CCountedPointer@VCAFile@@@@AAVCASuspendableProcess@@@Z
// (CWorldMap virtual slot 6 of vtable 0x01244AEC).
// Callees: operator new 0x00BFEA1A, CWorld::GetWorldSeed() const 0x0049D810 (manifest "CVertexBufferWin32::DoSizeof";
// returns &[World+8]+0x1613C, GetLocalSeed is 0x0049D800), CMap::CMap(const CGameDefinitionManager&, CWorld&,
// unsigned long*, long, const C2DBoxI&, CAFile&, CASuspendableProcess*) 0x008224E0, CCountedPointer<CMap>::Reset(CMap*)
// 0x0050FAA0, STLport vector<long>::_M_insert_overflow(long*, const long&, const __true_type&, unsigned, bool)
// 0x00440990, CASuspendableProcess virtual slot 2 (+0x08, pure in the base; name unknown), CWorldMap::UpdateAllEdges 0x0063DFD0, operator delete 0x00BFE9BC.
// WorldDefinitionManager() is a descriptive inline accessor for *World->DefinitionManager (the real out-of-line
// CWorldMap::PeekDefinitionManager 0x004FB310 goes through the component instead).
// Retail offsets: CWorldMap World +0x08, Maps +0x20 (CMapInfo 0x48 bytes: Area +0x00, PMap +0x10), LoadedMaps
// (STLport vector<long>) +0x90; CWorld DefinitionManager +0x10.
// Shape: the STLport _M_insert_overflow body sits above the caller (auto_inline off) so `map` stays in edi although its
// address is passed; the inline push_back builds its `__true_type` const-ref tag (user ctor) at the `map` argument slot, like the
// element reference; the by-value file pointer's inline release keeps its count block in esi (`dec [esi] ; jne`).
// Shape: the vector's operator[] is STLport's `*(begin() + n)` through an inline begin() accessor (`_SweepBegin1()`): the element
//   address is formed from the accessor's returned pointer plus the scaled index as retail; `first[n]` folds the index into the
//   load and recolours the registers (the GOTCHAS wave-25 rule, confirmed on the RTM compiler).
class CCPPointerInfo { public:
    long  Count;                                             // +0
    void (__fastcall* Destroy)(void* data);                  // +4
    void* Data;                                              // +8
};
template<class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
    void Reset(T* data);                                     // 0x0050FAA0 (CCountedPointer<CMap>)
};

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct __true_type { __true_type() { } };
struct CLongVector {                                         // STLport vector<long> (12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void _M_insert_overflow(long* position, const long& x, const __true_type& tag, unsigned long n, bool atEnd);   // 0x00440990
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

// The STLport body of 0x00440990 (a template instantiated in this TU), defined above the caller under
// auto_inline(off): VC7.1 then keeps `map` in a register although its address is passed.
#pragma auto_inline(off)
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
#pragma auto_inline(on)

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CAFile { public: };
class CASuspendableProcess { public:                                // vtable 0x0129D428 (slots 1-3 are _purecall)
    virtual ~CASuspendableProcess();                         // slot 0 (0x00A44A20)
    virtual void v01() = 0;
    virtual void Update() = 0;                               // slot 2 (+0x08; name unknown, descriptive)
};
class CGameDefinitionManager;
class CWorld { public:
    unsigned char                 _pad00[0x10];
    const CGameDefinitionManager* DefinitionManager;         // +0x10
    unsigned long* GetWorldSeed() const;                     // 0x0049D810
};
class CMap { public:
    CMap(const CGameDefinitionManager& definitions, CWorld& world, unsigned long* seed, long map,
         const C2DBoxI& area, CAFile& file, CASuspendableProcess* process);   // 0x008224E0
    unsigned char _body[0x1d40];
};
class CMapInfo { public:                                            // 0x48 bytes
    C2DBoxI               Area;                              // +0x00
    CCountedPointer<CMap> PMap;                              // +0x10
    unsigned char         _rest[0x30];                       // +0x18 .. +0x48
};
template <class T> class CArray { public:
    T* Start;                                                // +0x00
    T* Finish;                                               // +0x04
    T* EndOfStorage;                                         // +0x08
    T* _SweepBegin1() const { return Start; }
    T& operator[](unsigned long i) { return *(_SweepBegin1() + i); }
};

class CWorldMap { public:
    void*            _vptr;                                  // +0x00
    long             f4;                                     // +0x04
    CWorld*          World;                                  // +0x08
    unsigned char    _pad0c[0x14];
    CArray<CMapInfo> Maps;                                   // +0x20
    unsigned char    _pad2c[0x64];
    CLongVector      LoadedMaps;                             // +0x90 (PDB +0xAC)
    void UpdateAllEdges();                                   // 0x0063DFD0
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