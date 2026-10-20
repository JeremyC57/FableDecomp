// Behaviour test for 00547500: src/00547500.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NUISystem::CSwappingStateComponent::GetDataFromDef()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CSwappingStateComponent::GetDataFromDef() -- copy the def's swapping states (state id + time) into SwappingStates
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
inline void* operator new(unsigned int, void* p) { return p; }
struct CRefCounted {                               // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
template <class T> struct vector;
struct __true_type { __true_type() { } };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x005479A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)
struct CSwappingStateComponentDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted   Ref;                             // +0x04
    unsigned char _pad08[0x1e0 - 0x08];
    vector<unsigned long> SwappingStates;          // +0x1e0
    vector<float>         SwappingTimes;           // +0x1ec
    unsigned char _pad1f8[0x200 - 0x1f8];
    bool          ExtraFlag;                       // +0x200 (sets FlagsSetTwo bit 3)
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
namespace NUISystem {
struct CComponentRoot { virtual ~CComponentRoot(); };
struct CInputListener { virtual void v00(); unsigned char _pad04[0x14 - 0x04]; };
class CComponent : public CComponentRoot, public CInputListener {
public:
    V16(a) V16(b) V16(c) V16(d) V16(e) V16(f) V4(g0) V4(g1) virtual void h0(); virtual void h1(); virtual void h2();
    virtual CDefPointer<CSwappingStateComponentDef> GetDef();   // slot 108 (+0x1b0) 0x00530EC0
    unsigned char _pad18[0x12e - 0x18];
    unsigned char FlagsSetTwo;                     // +0x12e
    unsigned char _pad12f[0x15c - 0x12f];
};
class CSwappingStateComponent : public CComponent {
public:
    vector<pair<unsigned long, float> > SwappingStates;   // +0x15c
    void GetDataFromDef();
};

void CSwappingStateComponent::GetDataFromDef()
{
    CDefPointer<CSwappingStateComponentDef> def = GetDef();
    if (def->ExtraFlag)
        FlagsSetTwo |= 8;
    for (unsigned long i = 0; i < def->SwappingStates.size(); ++i) {
        pair<unsigned long, float> state(def->SwappingStates[i], 0.0f);
        if (i < def->SwappingTimes.size())
            state.second = def->SwappingTimes[i];
        SwappingStates.push_back(state);
    }
}
}

// ---- stubs and driver ----
using namespace NUISystem;
typedef pair<unsigned long, float> State;
static unsigned char g_raw[8 * sizeof(State)]; static int g_grow = 0, g_getdef = 0, g_destroy = 0; static CSwappingStateComponentDef* g_def = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; vector<State>* v = (vector<State>*)storage; State* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
#define D4(n) void CComponent::n##0() {} void CComponent::n##1() {} void CComponent::n##2() {} void CComponent::n##3() {}
#define D16(n) D4(n##a) D4(n##b) D4(n##c) D4(n##d)
namespace NUISystem {
CComponentRoot::~CComponentRoot() {}
void CInputListener::v00() {}
D16(a) D16(b) D16(c) D16(d) D16(e) D16(f) D4(g0) D4(g1)
void CComponent::h0() {} void CComponent::h1() {} void CComponent::h2() {}
CDefPointer<CSwappingStateComponentDef> CComponent::GetDef() { ++g_getdef; ++g_def->Ref.Count; static void* raw[1]; CDefPointer<CSwappingStateComponentDef>* p = (CDefPointer<CSwappingStateComponentDef>*)raw; p->Object = g_def; return *p; }   // raw holder: never destroyed, so only the caller's copy releases
}
void CSwappingStateComponentDef::v00() {}
void CSwappingStateComponentDef::Destroy() { ++g_destroy; }
int main()
{
    unsigned long ids[3] = { 10, 20, 30 }; float times[2] = { 1.5f, 2.5f };
    static CSwappingStateComponentDef def; def.Ref.Count = 1; def.ExtraFlag = false;
    def.SwappingStates._M_start = ids; def.SwappingStates._M_finish = ids + 3; def.SwappingStates._M_end_of_storage = ids + 3;
    def.SwappingTimes._M_start = times; def.SwappingTimes._M_finish = times + 2; def.SwappingTimes._M_end_of_storage = times + 2;
    g_def = &def;
    static CSwappingStateComponent c; c.FlagsSetTwo = 0x41;
    State* store = (State*)g_raw;
    c.SwappingStates._M_start = store; c.SwappingStates._M_finish = store; c.SwappingStates._M_end_of_storage = store + 1;
    c.GetDataFromDef();
    CHECK(g_getdef == 1 && def.Ref.Count == 1 && g_destroy == 0); CHECK(c.FlagsSetTwo == 0x41);
    CHECK(c.SwappingStates.size() == 3 && g_grow == 2);
    CHECK(store[0].first == 10 && store[0].second == 1.5f); CHECK(store[1].first == 20 && store[1].second == 2.5f); CHECK(store[2].first == 30 && store[2].second == 0.0f);
    // flag set, last reference: Destroy called on release; no states
    def.ExtraFlag = true; def.Ref.Count = 0; def.SwappingStates._M_finish = ids;
    c.GetDataFromDef();
    CHECK(c.FlagsSetTwo == 0x49 && g_destroy == 1 && def.Ref.Count == 0 && c.SwappingStates.size() == 3 && g_grow == 2);
    if (fails == 0) printf("00547500_TEST PASS\n"); else printf("00547500_TEST FAIL\n");
    return fails;
}