// 007b7140 behaviour test: copy_if over a list of marker pointers into a vector through back_insert_iterator with the
// IsCrateStackOfType predicate (crate-stack component present AND the sub-def's StackType equals the wanted type);
// the predicate's def holder reference is released again, the vector grows through its overflow path.
#include <cstdio>
#include <string.h>

class CBaseObject { public: int id; };
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* operator->() const { return (T*)GetPItem(); }
};

static int g_deleted = 0;
class CDefClassBase {
public:
    mutable long RefCount;
    long StackTypeOfSubDef;     // test: the sub-def this base def hands out (0 = none)
    CDefClassBase() : RefCount(1), StackTypeOfSubDef(0) {}
    virtual ~CDefClassBase() {}
    virtual void Delete() const { ++g_deleted; }
};
class CCrateStackDef : public CDefClassBase {
public:
    long StackType;
};
static CCrateStackDef g_subDefs[4];

template <class T> class CDefPointer {
public:
    T* Data;
    CDefPointer() : Data(0) {}
    ~CDefPointer() { if (Data) { if (--Data->RefCount == 0) Data->Delete(); } }
    T* operator->() const { return Data; }
};
static bool GetSubDef(const CDefClassBase* base, CDefPointer<const CCrateStackDef>* out)
{
    for (int i = 0; i < 4; ++i)
        if (g_subDefs[i].StackType == base->StackTypeOfSubDef) {
            out->Data = &g_subDefs[i];
            ++g_subDefs[i].RefCount;
            return true;
        }
    return false;
}

class CThingMarker : public CBaseObject { public:
    bool HasCrateStack;          // the interface bit 0xB5
    CDefClassBase* PBaseDef;
    template <class T> bool PeekTC(const T** out) const { if (HasCrateStack) { *out = 0; return true; } return false; }
};
class CTCCrateStack { public: };

class IsCrateStackOfType {
public:
    long Type;
    explicit IsCrateStackOfType(long type) : Type(type) {}
    bool operator()(CIntelligentPointer<CThingMarker> marker) const
    {
        const CTCCrateStack* stack;
        if (marker->PeekTC(&stack)) {
            CDefPointer<const CCrateStackDef> def;
            const CThingMarker* thing = marker.operator->();
            GetSubDef(thing->PBaseDef, &def);
            return def->StackType == Type;
        }
        return false;
    }
};

// minimal STLport-shaped containers
typedef CIntelligentPointer<CThingMarker> CMarkerPtr;
struct Vec {
    typedef CMarkerPtr value_type;
    CMarkerPtr* start; CMarkerPtr* finish; CMarkerPtr* end;
    int overflows;
    Vec() : start(0), finish(0), end(0), overflows(0) {}
    ~Vec() { delete[] start; }
    void push_back(const CMarkerPtr& x)
    {
        if (finish != end) { *finish = x; ++finish; return; }
        ++overflows;                                     // _M_insert_overflow: grow, copy, append
        int n = int(finish - start), cap = n ? 2 * n : 1;
        CMarkerPtr* p = new CMarkerPtr[cap];
        for (int i = 0; i < n; ++i) p[i] = start[i];
        p[n] = x;
        delete[] start;
        start = p; finish = p + n + 1; end = p + cap;
    }
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& c) : container(&c) {}
    back_insert_iterator& operator=(const typename C::value_type& v) { container->push_back(v); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
    back_insert_iterator operator++(int) { return *this; }
};
struct Node { Node* next; Node* prev; CMarkerPtr data; };
struct ListIt {
    Node* node;
    ListIt(Node* n) : node(n) {}
    const CMarkerPtr& operator*() const { return node->data; }
    ListIt& operator++() { node = node->next; return *this; }
    bool operator!=(const ListIt& o) const { return node != o.node; }
};

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (__pred(*__first))
            *__result++ = *__first;
    return __result;
}

static int Fail(const char* why) { std::printf("007b7140_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    for (int i = 0; i < 4; ++i) { g_subDefs[i].StackType = 10 + i; g_subDefs[i].RefCount = 1; }
    CDefClassBase bases[3];
    bases[0].StackTypeOfSubDef = 11; bases[1].StackTypeOfSubDef = 12; bases[2].StackTypeOfSubDef = 11;
    CThingMarker markers[6];
    const int baseOf[6] = { 0, 1, 2, 0, 1, 2 };
    const bool hasStack[6] = { true, true, true, false, true, true };
    for (int i = 0; i < 6; ++i) { markers[i].id = i; markers[i].HasCrateStack = hasStack[i]; markers[i].PBaseDef = &bases[baseOf[i]]; }
    Node head; head.next = &head; head.prev = &head;
    Node nodes[6];
    for (int i = 0; i < 6; ++i) {           // append
        nodes[i].data.SetPItem(&markers[i]);
        nodes[i].prev = head.prev; nodes[i].next = &head; head.prev->next = &nodes[i]; head.prev = &nodes[i];
    }
    Vec out;
    back_insert_iterator<Vec> it = copy_if(ListIt(head.next), ListIt(&head), back_insert_iterator<Vec>(out), IsCrateStackOfType(11));
    if (it.container != &out) return Fail("returned iterator");
    // markers 0, 2, 5 qualify (base 0/2 -> type 11 with the component); 3 lacks the component; 1, 4 are type 12
    if (out.finish - out.start != 3) return Fail("count");
    if (out.start[0].GetPItem() != &markers[0] || out.start[1].GetPItem() != &markers[2] || out.start[2].GetPItem() != &markers[5])
        return Fail("selection");
    for (int i = 0; i < 4; ++i)
        if (g_subDefs[i].RefCount != 1) return Fail("def holder released");
    if (g_deleted != 0) return Fail("no def deleted");
    // an empty range returns the destination untouched
    Vec none;
    back_insert_iterator<Vec> it2 = copy_if(ListIt(&head), ListIt(&head), back_insert_iterator<Vec>(none), IsCrateStackOfType(11));
    if (it2.container != &none || none.start != 0) return Fail("empty range");
    std::printf("007b7140_TEST PASS\n");
    return 0;
}