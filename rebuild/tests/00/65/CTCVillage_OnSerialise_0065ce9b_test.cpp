// Behaviour test for 0x0065CE9B: CTCVillage::OnSerialise(CPersistContext& context, bool fullSave) -- persists the village's population flag, the player's crime set (as a vector), the guard-sighting frame (relative to now), the fine paid, limbo state, enemy flag and hero-criminal flag
// A copy of the source model and logic (compiled separately from src/0065ce9b.cpp) with test stand-ins for the callees.
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
#pragma optimize("y", off)
#pragma optimize("s", on)

long __fastcall GFGetCurrentFrame();                   // 0x0049D870

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
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
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void reserve(unsigned int __n);                    // 0x00660684
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class T> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; pair(const _T1& a, const _T2& b) : first(a), second(b) { } };
template <class _Tp> struct less { };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    iterator begin() const { return iterator(_M_header->_M_left); }
    iterator end() const { return iterator(_M_header); }
    unsigned int size() const { return _M_node_count; }
    pair<iterator, bool> insert(const _Key& __x);      // 0x00514230
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& x) { return back_insert_iterator<C>(x); }
back_insert_iterator<vector<long> > __fastcall copy(_Rb_tree_iterator<long> first, _Rb_tree_iterator<long> last,
                                                    back_insert_iterator<vector<long> > result);   // 0x00661287
}

class CPersistContext {
public:
    bool IsSaving() const;                                                     // 0x004AEF50
    void Transfer(const char* name, bool& value);                              // 0x004107C0
    void Transfer(const char* name, long& value);                              // 0x004110B0
    void Transfer(const char* name, bool& value, const bool& defaultValue);    // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);    // 0x00410BE0
    void TransferVector(const char* name, std::vector<long>* value);           // 0x005815D6
};

class CTCVillage {
public:
    unsigned char  _pad04[0x64 - 0x04];                // vptr at +0x00
    std::set<long> PlayersCrimes;                      // +0x64
    unsigned char  _pad70[0x7a - 0x70];
    bool           HasBeenInitiallyPopulated;          // +0x7a
    unsigned char  _pad7b;
    bool           IsEnemyBecauseOfCrime;              // +0x7c
    unsigned char  _pad7d[3];
    long           FramePlayerLastSeenByGuard;         // +0x80
    unsigned char  _pad84[0xa0 - 0x84];
    long           AmountOfFineAlreadyPaid;            // +0xa0
    unsigned char  _padA4[0xb2 - 0xa4];
    bool           Limbo;                              // +0xb2
    bool           LastLimbo;                          // +0xb3
    unsigned char  _padB4[0xe4 - 0xb4];
    bool           CurrentIsHeroCriminal;              // +0xe4
    virtual void OnSerialise(CPersistContext& context, bool fullSave);   // vtable 0x0125A9AC slot 1 (slot 0 modelled away)
};

void CTCVillage::OnSerialise(CPersistContext& context, bool fullSave)
{
    context.Transfer("HasBeenInitiallyPopulated", HasBeenInitiallyPopulated);
    if (context.IsSaving()) {
        std::vector<long> crimes;
        crimes.reserve(PlayersCrimes.size());
        std::copy(PlayersCrimes.begin(), PlayersCrimes.end(), std::back_inserter(crimes));
        context.TransferVector("PlayersCrimes", &crimes);
        long framesSinceSeen = FramePlayerLastSeenByGuard - GFGetCurrentFrame();
        context.Transfer("FramePlayerLastSeenByGuard", framesSinceSeen, 0);
    } else {
        std::vector<long> crimes;
        context.TransferVector("PlayersCrimes", &crimes);
        std::vector<long>::iterator end = crimes.end();
        for (std::vector<long>::iterator it = crimes.begin(); it != end; ++it)
            PlayersCrimes.insert(*it);
        context.Transfer("FramePlayerLastSeenByGuard", FramePlayerLastSeenByGuard);
        FramePlayerLastSeenByGuard += GFGetCurrentFrame();
    }
    if (fullSave)
        context.Transfer("AmountOfFineAlreadyPaid", AmountOfFineAlreadyPaid);
    context.Transfer("Limbo", Limbo);
    if (!context.IsSaving())
        LastLimbo = Limbo;
    context.Transfer("IsEnemyBecauseOfCrime", IsEnemyBecauseOfCrime);
    context.Transfer("CurrentIsHeroCriminal", CurrentIsHeroCriminal, false);
}

// ---- test stand-ins ----
struct TestNode {
    std::_Rb_tree_node_base base;
    long value;
};
static TestNode g_nodes[16];
static int g_nodeCount = 0;
static int g_insertCalls = 0;
static int g_reserveCalls = 0;
static int g_copyCalls = 0;
static long g_frame = 1000;

long __fastcall GFGetCurrentFrame() { return g_frame; }

namespace std {
template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::reserve(unsigned int n)
{
    ++g_reserveCalls;
    _Tp* p = (_Tp*)malloc((n ? n : 1) * sizeof(_Tp));
    this->_M_start = p; this->_M_finish = p; this->_M_end_of_storage._M_data = p + n;
}
template <class _Key, class _Compare, class _Alloc>
pair<_Rb_tree_iterator<_Key>, bool> set<_Key, _Compare, _Alloc>::insert(const _Key& x)
{
    ++g_insertCalls;
    // keep the list sorted and unique: walk from the header's leftmost through _M_right links
    _Rb_tree_node_base* prev = _M_header;
    _Rb_tree_node_base* n = _M_header->_M_left;
    while (n != _M_header && ((TestNode*)n)->value < x) { prev = n; n = n->_M_right; }
    if (n != _M_header && ((TestNode*)n)->value == x)
        return pair<_Rb_tree_iterator<_Key>, bool>(_Rb_tree_iterator<_Key>(n), false);
    TestNode* node = &g_nodes[g_nodeCount++];
    node->value = x;
    node->base._M_right = n;
    if (prev == _M_header) _M_header->_M_left = &node->base; else prev->_M_right = &node->base;
    ++_M_node_count;
    return pair<_Rb_tree_iterator<_Key>, bool>(_Rb_tree_iterator<_Key>(&node->base), true);
}
back_insert_iterator<vector<long> > __fastcall copy(_Rb_tree_iterator<long> first, _Rb_tree_iterator<long> last,
                                                    back_insert_iterator<vector<long> > result)
{
    ++g_copyCalls;
    for (_Rb_tree_node_base* n = first._M_node; n != last._M_node; n = n->_M_right) {
        vector<long>& v = *result.container;
        if (v._M_finish == v._M_end_of_storage._M_data) return result;   // test never overflows
        *v._M_finish++ = ((TestNode*)n)->value;
    }
    return result;
}
}

// A tiny persist context: saving records values by name, loading hands them back.
struct TestEntry { const char* name; long value; long items[8]; int count; };
static TestEntry g_store[16];
static int g_storeCount = 0;
static bool g_saving = true;
static int g_defaultChecks = 0;
static TestEntry* Find(const char* name)
{
    for (int i = 0; i < g_storeCount; ++i) if (std::strcmp(g_store[i].name, name) == 0) return &g_store[i];
    return 0;
}
static TestEntry* Slot(const char* name)
{
    TestEntry* e = Find(name);
    if (!e) { e = &g_store[g_storeCount++]; e->name = name; e->value = 0; e->count = 0; }
    return e;
}
bool CPersistContext::IsSaving() const { return g_saving; }
void CPersistContext::Transfer(const char* name, bool& value)
{
    if (g_saving) Slot(name)->value = value ? 1 : 0; else { TestEntry* e = Find(name); if (e) value = e->value != 0; }
}
void CPersistContext::Transfer(const char* name, long& value)
{
    if (g_saving) Slot(name)->value = value; else { TestEntry* e = Find(name); if (e) value = e->value; }
}
void CPersistContext::Transfer(const char* name, bool& value, const bool& defaultValue)
{
    if (!defaultValue) ++g_defaultChecks;
    Transfer(name, value);
}
void CPersistContext::Transfer(const char* name, long& value, const long& defaultValue)
{
    if (defaultValue == 0) ++g_defaultChecks;
    Transfer(name, value);
}
void CPersistContext::TransferVector(const char* name, std::vector<long>* value)
{
    if (g_saving) {
        TestEntry* e = Slot(name);
        e->count = 0;
        for (long* p = value->_M_start; p != value->_M_finish; ++p) e->items[e->count++] = *p;
    } else {
        TestEntry* e = Find(name);
        int n = e ? e->count : 0;
        long* p = (long*)malloc((n ? n : 1) * sizeof(long));
        for (int i = 0; i < n; ++i) p[i] = e->items[i];
        value->_M_start = p; value->_M_finish = p + n; value->_M_end_of_storage._M_data = p + n;
    }
}

static int Fail(int line) { std::printf("0065ce9b_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

static void InitSet(std::set<long>& s, std::_Rb_tree_node_base* header)
{
    header->_M_left = header; header->_M_right = header; header->_M_parent = 0;
    s._M_header = header; s._M_node_count = 0;
}

int main()
{
    // ---- save ----
    std::_Rb_tree_node_base header1;
    static CTCVillage saver;
    InitSet(saver.PlayersCrimes, &header1);
    saver.PlayersCrimes.insert(7); saver.PlayersCrimes.insert(3); saver.PlayersCrimes.insert(11);
    CHECK(saver.PlayersCrimes._M_node_count == 3);
    saver.HasBeenInitiallyPopulated = true;
    saver.IsEnemyBecauseOfCrime = false;
    saver.FramePlayerLastSeenByGuard = 1250;
    saver.AmountOfFineAlreadyPaid = 400;
    saver.Limbo = true; saver.LastLimbo = false;
    saver.CurrentIsHeroCriminal = true;
    g_frame = 1000; g_saving = true; g_insertCalls = 0; g_defaultChecks = 0;
    CPersistContext ctx;
    saver.OnSerialise(ctx, false);
    CHECK(g_reserveCalls == 1 && g_copyCalls == 1 && g_insertCalls == 0);
    TestEntry* e = Find("PlayersCrimes");
    CHECK(e && e->count == 3 && e->items[0] == 3 && e->items[1] == 7 && e->items[2] == 11);
    CHECK(Find("HasBeenInitiallyPopulated") && Find("HasBeenInitiallyPopulated")->value == 1);
    CHECK(Find("FramePlayerLastSeenByGuard") && Find("FramePlayerLastSeenByGuard")->value == 250);   // relative to now
    CHECK(saver.FramePlayerLastSeenByGuard == 1250);                                                  // member untouched
    CHECK(Find("AmountOfFineAlreadyPaid") == 0);                                                      // not a full save
    CHECK(Find("Limbo") && Find("Limbo")->value == 1);
    CHECK(saver.LastLimbo == false);                                                                  // only on load
    CHECK(Find("IsEnemyBecauseOfCrime") && Find("IsEnemyBecauseOfCrime")->value == 0);
    CHECK(Find("CurrentIsHeroCriminal") && Find("CurrentIsHeroCriminal")->value == 1);
    CHECK(g_defaultChecks == 2);                                                                      // both defaults zero
    saver.OnSerialise(ctx, true);
    CHECK(Find("AmountOfFineAlreadyPaid") && Find("AmountOfFineAlreadyPaid")->value == 400);

    // ---- load ----
    std::_Rb_tree_node_base header2;
    static CTCVillage loader;
    InitSet(loader.PlayersCrimes, &header2);
    loader.PlayersCrimes.insert(5);
    loader.HasBeenInitiallyPopulated = false;
    loader.IsEnemyBecauseOfCrime = true;
    loader.FramePlayerLastSeenByGuard = -1;
    loader.AmountOfFineAlreadyPaid = 0;
    loader.Limbo = false; loader.LastLimbo = false;
    loader.CurrentIsHeroCriminal = false;
    g_frame = 5000; g_saving = false; g_insertCalls = 0; g_reserveCalls = 0; g_copyCalls = 0;
    loader.OnSerialise(ctx, true);
    CHECK(g_reserveCalls == 0 && g_copyCalls == 0 && g_insertCalls == 3);
    CHECK(loader.PlayersCrimes._M_node_count == 4);
    std::_Rb_tree_node_base* n = header2._M_left;
    long expect[4] = { 3, 5, 7, 11 };
    for (int i = 0; i < 4; ++i) { CHECK(n != &header2 && ((TestNode*)n)->value == expect[i]); n = n->_M_right; }
    CHECK(n == &header2);
    CHECK(loader.HasBeenInitiallyPopulated == true);
    CHECK(loader.FramePlayerLastSeenByGuard == 5250);                                                 // made absolute again
    CHECK(loader.AmountOfFineAlreadyPaid == 400);
    CHECK(loader.Limbo == true && loader.LastLimbo == true);
    CHECK(loader.IsEnemyBecauseOfCrime == false);
    CHECK(loader.CurrentIsHeroCriminal == true);
    loader.AmountOfFineAlreadyPaid = 9;
    loader.OnSerialise(ctx, false);                                                                   // fine skipped
    CHECK(loader.AmountOfFineAlreadyPaid == 9);
    std::printf("0065ce9b_TEST PASS\n");
    return 0;
}