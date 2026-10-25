// FrameUpdateThing::operator()(const CThing& thing) const -- (anonymous namespace) collect a nearby thing's steering agent: its navigator's agent for a creature, the obstacle's own agent otherwise; always continue
// Leaf: ??RFrameUpdateThing@
// Grade: RELOCATION_MATCH -- each lookup is the landed out-parameter `__forceinline bool GetInterface(type, T** out)` (0x007504E0
//        shape: presence bit test inside, `*out = it->component; return true;`) written `if (GetInterface(7, &nav)) ... else
//        { if (GetInterface(0xfe, &obs)) ... }`: the component chain then starts in eax as retail (`mov eax,[eax+4] ; mov
//        ecx,[eax+0Ch] ; mov edx,[ecx+20h]`). Kept from agent901: the same-TU noinline _M_insert_overflow stand-in (tag
//        unnamed) and LowerBound body. HasTC + void out-pointer GetInterface, F88 tails and the GetTC ternary stay at 193v193.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CE730, length 193 bytes (+ int3 padding), thiscall `ret 4`. Single
// caller: CThingSearchTools::ForEachInRoughArea<CThing, CThingFilter_IsNot, (anonymous namespace)::FrameUpdateThing>
// 0x006CE800 (call at 0x006CE95A, `bl = 1` when it returns true). Ego_r.pdb names both
// (`??RFrameUpdateThing@?A0xd4ba0483@@QBE_NABVCThing@@@Z`; its ctor takes `vector<const CSteeringAgent*>&`).
// The manifest label "CTCInterface_InsertPropertyToVector" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (x2), vector<const CSteeringAgent*>::
// _M_insert_overflow(pos, x, const __true_type&, 1, true) @ 0x006CD680.
// Layout: FrameUpdateThing {vector<const CSteeringAgent*>* Agents} (Ego_r.pdb, 4 bytes; a reference member);
// retail CThing interface bitset +0x20 (TC 7 = CTCCreatureNavigation: byte +0x20 bit 7, `test cl,cl ; jns`; TC 0xFE =
// CTCObstacle: dword +0x3C bit 30), interface map +0x44; CTCCreatureNavigation::PNavigator +0x0C,
// CNavigatorBase::SteeringAgent +0x20, CTCObstacle::SteeringAgent +0x0C (Ego_r.pdb).
// Shape: two `if / else if` arms each with its own GetTC lookup (key homed in the dead `thing` argument slot) and its
// own inline STLport push_back; VC7.1 cross-jumps the two push_back tails (one in-place store block, one overflow
// call). The converted `const CSteeringAgent*` temporary and the empty `__true_type` tag share that same slot.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
namespace std {
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
    std::vector<const CSteeringAgent*>& Agents;       // +0x00
    FrameUpdateThing(std::vector<const CSteeringAgent*>& agents) : Agents(agents) { }
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