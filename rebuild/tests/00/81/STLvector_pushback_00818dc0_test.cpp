// Behaviour test for 00818dc0 (_STL::vector<pair<NHitLocations::CHitLocation, long> >::push_back): with room the element is
// copy-constructed at end() (both def pointers bump their RefCount) and end() advances; when full, _M_insert_overflow gets
// (end(), x, fill length 1, at-end true).
#include <cstdio>
#include <new>
struct CThing { int id; };
struct CDefClassBase {
    virtual ~CDefClassBase() { }
    long RefCount;
    CDefClassBase() : RefCount(0) { }
};
struct CHitLocationDef : public CDefClassBase { };
struct CArmourDef : public CDefClassBase { };
template <class T> struct CDefPointer {
    T* Object;
    explicit CDefPointer(T* o) : Object(o) { if (Object) ++Object->RefCount; }
    CDefPointer(const CDefPointer<T>& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};
enum EActionRegisteredType { ACTION_REGISTERED_TYPE_NULL, ACTION_REGISTERED_TYPE_7 = 7 };
struct CHitLocation {
    CThing*                      POwnerThing;
    CDefPointer<CHitLocationDef> PDef;
    CDefPointer<CArmourDef>      PArmourDef;
    EActionRegisteredType        HitResponseAction;
    EActionRegisteredType        BlockResponseAction;
    bool                         Enabled;
    CHitLocation(CThing* t, CHitLocationDef* d, CArmourDef* a) : POwnerThing(t), PDef(d), PArmourDef(a),
        HitResponseAction(ACTION_REGISTERED_TYPE_7), BlockResponseAction(ACTION_REGISTERED_TYPE_NULL), Enabled(true) { }
};
struct __false_type { __false_type() { } };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<CHitLocation, long> T;
static int g_overflow = 0;
static const T* g_overflowX = 0;
static T* g_overflowPos = 0;
static unsigned int g_overflowLen = 0;
static bool g_overflowAtEnd = false;
struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)   // stands in for 0x00818CA0
    {
        ++g_overflow;
        g_overflowPos = __position;
        g_overflowX = &__x;
        g_overflowLen = __fill_len;
        g_overflowAtEnd = __atend;
    }
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) T(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1u, true);
    }
};
union UStorage { double d; void* p; char bytes[sizeof(T) * 2]; };
#define FAIL() { std::printf("00818dc0_TEST FAIL\n"); return 1; }
int main()
{
    static CThing owner;
    static CHitLocationDef def;
    static CArmourDef armour;
    UStorage storage;
    T* buf = reinterpret_cast<T*>(storage.bytes);
    vector v;
    v._M_start = v._M_finish = buf;
    v._M_end_of_storage = buf + 2;
    T x(CHitLocation(&owner, &def, &armour), 99L);
    const long defBase = def.RefCount, armourBase = armour.RefCount;
    v.push_back(x);
    v.push_back(x);
    if (v._M_finish != buf + 2 || g_overflow != 0) FAIL();
    if (def.RefCount != defBase + 2 || armour.RefCount != armourBase + 2) FAIL();
    for (int i = 0; i < 2; ++i)
        if (buf[i].first.POwnerThing != &owner || buf[i].first.PDef.Object != &def || buf[i].first.PArmourDef.Object != &armour
            || buf[i].first.HitResponseAction != ACTION_REGISTERED_TYPE_7 || buf[i].first.BlockResponseAction != ACTION_REGISTERED_TYPE_NULL
            || !buf[i].first.Enabled || buf[i].second != 99L) FAIL();
    v.push_back(x);
    if (g_overflow != 1 || g_overflowPos != buf + 2 || g_overflowX != &x || g_overflowLen != 1u || !g_overflowAtEnd) FAIL();
    if (v._M_finish != buf + 2 || def.RefCount != defBase + 2) FAIL();
    std::printf("00818dc0_TEST PASS\n");
    return 0;
}