#include <cstdio>
// test thing: the retail state bits, the type mask, a position and an attribute word the test filters read
class CThing { public:
    enum { ThingTypeMask = 0 };
    unsigned long ThingTypeFlags;
    unsigned char StateBits;
    float X;
    unsigned long Attr;
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    bool MemberTest() const { return (Attr & 1) != 0; }
    bool HasTCType(long type) const { return ((Attr >> type) & 1) != 0; }
};
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
class CThingAICreature : public CThing { public: enum { ThingListType = 1 }; };
class CThingBuilding : public CThing { public: enum { ThingListType = 3 }; };
class CThingVillage : public CThing { public: enum { ThingListType = 4 }; };
class CThingGameObject : public CThing { public: enum { ThingTypeMask = 0x7 }; };
class CThingObject : public CThingGameObject { public: enum { ThingTypeMask = 0x807 }; };
class C3DVector { public: float X, Y, Z; };
class CThingFilter_In2DRange { public: C3DVector Pos; float Range; bool operator()(const CThing& t) const { return (t.Attr & 2) != 0; } };
class CThingFilter_In3DRange { public: C3DVector Pos; float Range; bool operator()(const CThing& t) const { return (t.Attr & 0x400) != 0; } };
class CThingFilter_ObjectHasProperty { public: long Property; bool operator()(const CThing& t) const { return (t.Attr & 0x40) != 0; } };
class CThingFilter_HasFreeActionPoint { public: bool operator()(const CThing& t) const { return (t.Attr & 0x80) != 0; } };
class CThingFilter_HasTC { public: long Type; bool operator()(const CThing& t) const { return t.HasTCType(Type); } };
class CThingFilter_AICreatureHasProperty { public: unsigned long Property; bool operator()(const CThing& t) const { return (t.Attr & 0x20) != 0; } };
class CThingFilter_HasScriptedControl { public: bool CheckForActive; bool operator()(const CThing& t) const { return (t.Attr & 0x10) != 0; } };
struct CThingFilter_OnFire { bool operator()(const CThing& t) const { return (t.Attr & 0x8) != 0; } };
struct CThingFilter_IsValidAttackTargetForCreature { unsigned char _sub[0x20]; bool operator()(const CThing& t) const { return (t.Attr & 0x4) != 0; } };
template <class T> class CFilter_Not { public: const T& F; template <class U> bool operator()(const U& thing) const { return !F(thing); } };
template <class A, class B> class CFilter_Bind2 { public:
    const A& First; const B& Second;
    template <class T> bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};
template <class A, class B, class C, class D> class CFilter_Bind4 { public:
    const A& First; const B& Second; const C& Third; const D& Fourth;
    template <class T> bool operator()(const T& thing) const { return First(thing) && Second(thing) && Third(thing) && Fourth(thing); }
};
namespace std {
template <class P> class unary_negate {
protected:
    P _M_pred;
public:
    template <class T> bool operator()(const T& x) const { return !_M_pred(x); }
};
template <class R, class T> class const_mem_fun_ref_t {
    R (T::*_M_f)() const;
public:
    explicit const_mem_fun_ref_t(R (T::*f)() const) : _M_f(f) {}
    R operator()(const T& r) const { return (r.*_M_f)(); }
};
template <class R, class T, class A> class const_mem_fun1_ref_t {
    R (T::*_M_f)(A) const;
public:
    explicit const_mem_fun1_ref_t(R (T::*f)(A) const) : _M_f(f) {}
    R operator()(const T& r, A a) const { return (r.*_M_f)(a); }
};
template <class Op> class binder2nd;
template <class R, class T, class A> class binder2nd<const_mem_fun1_ref_t<R, T, A> > {
protected:
    const_mem_fun1_ref_t<R, T, A> op;
    A value;
public:
    binder2nd(const const_mem_fun1_ref_t<R, T, A>& o, A v) : op(o), value(v) {}
    R operator()(const T& x) const { return op(x, value); }
};
template <class T> struct logical_or { bool operator()(const T& x, const T& y) const { return x || y; } };
template <class Op1, class Op2, class Op3> class binary_compose {
protected:
    Op1 _M_fn1; Op2 _M_fn2; Op3 _M_fn3;
public:
    binary_compose(const Op1& a, const Op2& b, const Op3& c) : _M_fn1(a), _M_fn2(b), _M_fn3(c) {}
    bool operator()(const CThing& x) const { return _M_fn1(_M_fn2(x), _M_fn3(x)); }
};
}
static float Dist(const CThing* t, float x) { float d = t->X - x; return d < 0 ? -d : d; }
class CThingCompare_Nearest { public:
    C3DVector Pos;
    template <class T> T* operator()(T* best, T* candidate) const
    {
        if (!best || Dist(candidate, Pos.X) < Dist(best, Pos.X)) return candidate;
        return best;
    }
};
class CThingCreatureBase;
class CThingCompare_BestCreatureToAttack { public:           // test: the highest X wins
    CThingCreatureBase* Targeter; CThing* PCurrentBestTargetForAttack;
    float PreferPlayerWeight, PreferNonCombatantsWeight, PreferLastAttackerWeight, PreferCurrentBestEnemyWeight;
    template <class T> T* operator()(T* best, T* candidate) const
    {
        if (!best || candidate->X > best->X) return candidate;
        return best;
    }
};
static const int N = 48;
static CThing g_things[N];
static CThingList::SNode g_nodes[N + 1];
static CThingList g_list;
static int g_peekType = -1;
struct CThingSearchToolsMethods {
    CThingList* PeekAllThings() { g_peekType = 100; return &g_list; }
    CThingList* PeekTypeList(long type) { g_peekType = (int)type; return &g_list; }
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out);
};
template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        if ((*it)->IsIncludedInThingScans() && IsThingOfType<TThing>(*it))
        {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(*candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}
template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}
static void Build()
{
    static const unsigned char states[6] = { 0x0c, 0x0c, 0x0d, 0x04, 0x2c, 0x0c };
    static const unsigned long types[4] = { 0x807, 0x7, 0x3, 0x807 };
    int i = 0;
    CThingList::SNode* head = &g_nodes[N];
    head->m_pNext = head; head->m_pPrev = head;
    for (i = 0; i < N; ++i) {
        CThing& t = g_things[i];
        t.StateBits = states[i % 6];
        t.ThingTypeFlags = types[i % 4];
        t.X = (float)((i * 37) % 23);
        t.Attr = (unsigned long)((i * 2654435761u) >> 7);
        CThingList::SNode* n = &g_nodes[i];
        n->m_value = &t;
        n->m_pPrev = head->m_pPrev; n->m_pNext = head;
        head->m_pPrev->m_pNext = n; head->m_pPrev = n;
    }
    g_list.m_pHead = head;
}

int main()
{
    Build();
    int i = 0;
    std::binder2nd<std::const_mem_fun1_ref_t<bool, CThing, long> > filter(std::const_mem_fun1_ref_t<bool, CThing, long>(&CThing::HasTCType), 9);
    CThingCompare_Nearest compare = { { 11.0f, 0.0f, 0.0f } };
    CThingVillage* found = 0;
    bool ok = CThingSearchToolsMethods().GetPBestLeafThing(filter, compare, &found);
    const CThing* want = 0;
    for (i = 0; i < N; ++i) {
        const CThing& t = g_things[i];
        if (g_things[i].IsIncludedInThingScans() && (t.HasTCType(9)) && (!want || Dist(&g_things[i], 11.0f) < Dist(want, 11.0f)))
            want = &g_things[i];
    }
    int bad = 0;
    if (!want || !ok || (const void*)found != (const void*)want) bad = 1;
    if (g_peekType != 4) bad = 1;
    for (i = 0; i < N; ++i)
        g_things[i].StateBits = 0x0d;
    found = (CThingVillage*)&g_things[0];
    if (CThingSearchToolsMethods().GetPBestLeafThing(filter, compare, &found) || found != 0) bad = 1;
    if (bad) { std::printf("0048cb50_TEST FAIL\n"); return 1; }
    std::printf("0048cb50_TEST PASS\n");
    return 0;
}