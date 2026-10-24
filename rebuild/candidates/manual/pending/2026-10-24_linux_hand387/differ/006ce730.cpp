// FrameUpdateThing::operator()(const CThing& thing) const -- (anonymous namespace) collect a nearby thing's steering agent: its navigator's agent for a creature, the obstacle's own agent otherwise; always continue
// Leaf: ??RFrameUpdateThing@
// Grade: DIFFER(196v193) -- slot homing only: retail has no frame, the `const CSteeringAgent*` value, the lookup key and
//        the empty tag all share the dead `thing` argument slot; ours opens a `push ecx` frame word for the agent value
//        (key and tag in the argument slot) and loads the component chain through ecx/edx instead of eax/ecx/edx.
//        Tried: direct member argument (199: no conversion temporary, tag zero store), named local per arm with a
//        user-constructed tag (196), plus a same-TU LowerBound body (196), explicit static_cast temporaries (196).
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
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
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
    if (thing.HasTC(7))
        Agents.push_back(static_cast<const CSteeringAgent*>(thing.GetTC<CTCCreatureNavigation>(7)->PNavigator->SteeringAgent));
    else if (thing.HasTC(0xfe))
        Agents.push_back(static_cast<const CSteeringAgent*>(thing.GetTC<CTCObstacle>(0xfe)->SteeringAgent));
    return true;
}
}

// Keeps the anonymous-namespace member alive in the object (retail reaches it from ForEachInRoughArea).
bool (FrameUpdateThing::*g_pfnFrameUpdateThing)(const CThing&) const = &FrameUpdateThing::operator();
