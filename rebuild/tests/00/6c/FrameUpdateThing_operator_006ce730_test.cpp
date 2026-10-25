// Behaviour test for 0x006CE730 (source body copied from src/006ce730.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace _STL {
struct __true_type { __true_type() { } };


template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef unsigned int size_type;
    typedef __true_type _IsPODType;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, size_type fill_len,
                            bool atend = false);      // 0x006CD680 for vector<const CSteeringAgent*>
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);
    }
};
// _M_insert_overflow @ 0x006CD680: a same-TU stand-in with the STLport POD reallocation shape (GOTCHAS empty-tag rule:
// the callee that receives the empty __true_type gets a body, never inlined, the tag unnamed).
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, size_type fill_len, bool atend)
{
    size_type old_size = _M_finish - _M_start;
    size_type len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != position; ++p, ++new_finish)
        *new_finish = *p;
    for (size_type i = 0; i < fill_len; ++i, ++new_finish)
        *new_finish = x;
    if (!atend)
        for (T* p = position; p != _M_finish; ++p, ++new_finish)
            *new_finish = *p;
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage = new_start + len;
}
#pragma auto_inline(on)
}

class CTCBase;

struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

// Same-TU body of the const LowerBound (binary search over the sorted entries), noinline: with it VC7.1 homes the
// converted agent pointer over the dead key slot (the `thing` argument slot) instead of opening a `push ecx` frame.
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long len = last - first;
    while (len > 0) {
        long half = len / 2;
        if (it[half].type < *type) {
            it += half + 1;
            len -= half + 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out) const   // the landed 0x007504E0 shape
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CSteeringAgent;

class CNavigatorBase {                                // Ego_r.pdb
public:
    unsigned char   _pad00[0x20];
    CSteeringAgent* SteeringAgent;                    // +0x20
};

class CTCCreatureNavigation {                         // TC 7
public:
    unsigned char   _pad00[0x0C];
    CNavigatorBase* PNavigator;                       // +0x0C
};

class CTCObstacle {                                   // TC 0xFE
public:
    unsigned char   _pad00[0x0C];
    CSteeringAgent* SteeringAgent;                    // +0x0C
};

namespace {
struct FrameUpdateThing {
    _STL::vector<const CSteeringAgent*>& Agents;       // +0x00
    FrameUpdateThing(_STL::vector<const CSteeringAgent*>& agents) : Agents(agents) { }
    bool operator()(const CThing& thing) const;
};

bool FrameUpdateThing::operator()(const CThing& thing) const
{
    CTCCreatureNavigation* navigation;
    if (thing.GetInterface(7, &navigation))
        Agents.push_back(static_cast<const CSteeringAgent*>(navigation->PNavigator->SteeringAgent));
    else {
        CTCObstacle* obstacle;
        if (thing.GetInterface(0xfe, &obstacle))
            Agents.push_back(static_cast<const CSteeringAgent*>(obstacle->SteeringAgent));
    }
    return true;
}
}

// Keeps the anonymous-namespace member alive in the object (retail reaches it from ForEachInRoughArea).
bool (FrameUpdateThing::*g_pfnFrameUpdateThing)(const CThing&) const = &FrameUpdateThing::operator();

// ---- test harness ----
static void SetBit(CThing* t, long type) { t->m_presentBits[type >> 5] |= (1UL << (type & 31)); }
int main()
{
    int ok = 1;
    static unsigned char agentA[4], agentB[4];
    static CNavigatorBase navigator; memset(&navigator, 0, sizeof navigator); navigator.SteeringAgent = (CSteeringAgent*)agentA;
    static CTCCreatureNavigation nav; memset(&nav, 0, sizeof nav); nav.PNavigator = &navigator;
    static CTCObstacle obs; memset(&obs, 0, sizeof obs); obs.SteeringAgent = (CSteeringAgent*)agentB;
    static CThing creature, obstacle, plain, both;
    memset(&creature, 0, sizeof creature); memset(&obstacle, 0, sizeof obstacle); memset(&plain, 0, sizeof plain); memset(&both, 0, sizeof both);
    CInterfaceMap::SEntry ce[2]; ce[0].type = 3; ce[0].component = 0; ce[1].type = 7; ce[1].component = (CTCBase*)&nav;
    creature.m_interfaces.first = ce; creature.m_interfaces.last = ce + 2; SetBit(&creature, 7);
    CInterfaceMap::SEntry oe[1]; oe[0].type = 0xfe; oe[0].component = (CTCBase*)&obs;
    obstacle.m_interfaces.first = oe; obstacle.m_interfaces.last = oe + 1; SetBit(&obstacle, 0xfe);
    CInterfaceMap::SEntry be[2]; be[0].type = 7; be[0].component = (CTCBase*)&nav; be[1].type = 0xfe; be[1].component = (CTCBase*)&obs;
    both.m_interfaces.first = be; both.m_interfaces.last = be + 2; SetBit(&both, 7); SetBit(&both, 0xfe);
    _STL::vector<const CSteeringAgent*> agents; agents._M_start = agents._M_finish = agents._M_end_of_storage = 0;
    FrameUpdateThing f(agents);
    if (!f(creature) || !f(obstacle) || !f(plain) || !f(both)) ok = 0;
    if (agents._M_finish - agents._M_start != 3) ok = 0;
    else if (agents._M_start[0] != (const CSteeringAgent*)agentA || agents._M_start[1] != (const CSteeringAgent*)agentB ||
             agents._M_start[2] != (const CSteeringAgent*)agentA) ok = 0;
    if (g_pfnFrameUpdateThing == 0) ok = 0;
    printf("006ce730_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}