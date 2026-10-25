// Behaviour test for 0x0062CE60: CWorld::EALoadGameFromInGameMenu(const CGameEvent* event) -- game-event action: if the event player's hero is alive, copy the save-game name list from the hero's interface 0xC6, read the chosen index from the event data and load that save
// A copy of the source model and logic (compiled separately from src/0062ce60.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);             // 0x0099B720
    ~CWideString();                                    // 0x0099B510
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    _Tp& operator[](unsigned int __n) { return *(this->_M_start + __n); }
};
}

class CMemoryBuffer {
public:
    unsigned char Data[0x20];                          // +0x00
    long          Length;                              // +0x20
    long          ReadPos;                             // +0x24
    long          WritePos;                            // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);       // 0x009F18F0
    __forceinline long ReadLong() { long value = 0; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                                // +0x00
    signed char   Player;                              // +0x04
    unsigned char Data[32];                            // +0x05
    unsigned char EndPos;                              // +0x25
    bool          Valid;                               // +0x26
    bool          Replacement;                         // +0x27
    long GetData(CMemoryBuffer& buffer) const;         // 0x009F17A0
};

class CTCBase;
struct CInterfaceMap {                                 // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

class CTCHeroSaveGames {                               // interface 0xC6 (descriptive name)
public:
    void GetSaveGameNames(std::vector<CWideString>* names);   // 0x0047E186
};

class CThingCreatureBase {
public:
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];                  // +0x20 bitset<274>
    CInterfaceMap Components;                          // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                            // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CPlayer {
public:
    CThingCreatureBase* GetHeroCreature();             // 0x00487DC0
};
class CWorld {
public:
    CPlayer* GetPlayer(long number);                   // 0x0049D850
    bool LoadGameState(const CWideString& name, bool flag);   // 0x004A3200
    void EALoadGameFromInGameMenu(const CGameEvent* event);
};

void CWorld::EALoadGameFromInGameMenu(const CGameEvent* event)
{
    CThingCreatureBase* hero = GetPlayer(event->Player)->GetHeroCreature();
    if (!hero || hero->Dead)
        return;
    CTCHeroSaveGames* saveGames;
    hero->GetInterface(&saveGames, 0xC6);
    std::vector<CWideString> names;
    saveGames->GetSaveGameNames(&names);
    CMemoryBuffer buffer;
    event->GetData(buffer);
    long index = buffer.ReadLong();
    CWideString name(names[index]);
    LoadGameState(name, false);
}

// ---- test stand-ins ----
static int g_copies = 0, g_dtors = 0, g_loads = 0, g_lookups = 0;
static const char* g_loadedName = 0;
static bool g_loadFlag = true;
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copies; }
CWideString::~CWideString() { ++g_dtors; }

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lookups;
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}

static const char* g_names[3] = { "Save A", "Save B", "Save C" };
static int g_getNamesCalls = 0;
void CTCHeroSaveGames::GetSaveGameNames(std::vector<CWideString>* names)
{
    ++g_getNamesCalls;
    CWideString* p = (CWideString*)malloc(3 * sizeof(CWideString));
    for (int i = 0; i < 3; ++i) p[i].PStringData = (void*)g_names[i];
    names->_M_start = p; names->_M_finish = p + 3; names->_M_end_of_storage._M_data = p + 3;
}

static int g_eventIndex = 0;
long CGameEvent::GetData(CMemoryBuffer& buffer) const
{
    std::memcpy(buffer.Data, &g_eventIndex, 4);
    buffer.Length = 4; buffer.ReadPos = 0;
    return 4;
}
void CMemoryBuffer::Read(unsigned long length, void* data)
{
    std::memcpy(data, Data + ReadPos, length);
    ReadPos += (long)length;
}

static CThingCreatureBase* g_hero = 0;
static long g_playerAsked = -99;
CThingCreatureBase* CPlayer::GetHeroCreature() { return g_hero; }
static CPlayer g_player;
CPlayer* CWorld::GetPlayer(long number) { g_playerAsked = number; return &g_player; }
bool CWorld::LoadGameState(const CWideString& name, bool flag)
{
    ++g_loads; g_loadedName = (const char*)name.PStringData; g_loadFlag = flag;
    return true;
}

static int Fail(int line) { std::printf("0062ce60_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    static CThingCreatureBase hero;
    std::memset(hero.UsingComponents, 0, sizeof hero.UsingComponents);
    hero.Dead = 0;
    const long type = 0xC6;
    hero.UsingComponents[type >> 5] |= (1UL << (type & 31));
    static CTCHeroSaveGames games;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x11; entries[0].component = 0;
    entries[1].type = 0xC6; entries[1].component = (CTCBase*)&games;
    entries[2].type = 0xD0; entries[2].component = 0;
    hero.Components.first = entries; hero.Components.last = entries + 3;

    CWorld world;
    CGameEvent event;
    event.Type = 7; event.Player = 2;

    // no hero: nothing happens
    g_hero = 0;
    world.EALoadGameFromInGameMenu(&event);
    CHECK(g_playerAsked == 2 && g_loads == 0 && g_getNamesCalls == 0);

    // dead hero: nothing happens
    g_hero = &hero; hero.Dead = 1;
    world.EALoadGameFromInGameMenu(&event);
    CHECK(g_loads == 0 && g_getNamesCalls == 0);

    // living hero: load the chosen save by name
    hero.Dead = 0; g_eventIndex = 1;
    world.EALoadGameFromInGameMenu(&event);
    CHECK(g_lookups == 1 && g_getNamesCalls == 1);
    CHECK(g_loads == 1 && g_loadedName == g_names[1] && g_loadFlag == false);
    CHECK(g_copies == 1);
    CHECK(g_dtors == 4);                 // the copy plus the three listed names
    g_eventIndex = 2;
    world.EALoadGameFromInGameMenu(&event);
    CHECK(g_loads == 2 && g_loadedName == g_names[2] && g_dtors == 8);
    std::printf("0062ce60_TEST PASS\n");
    return 0;
}