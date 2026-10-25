// Behaviour test for 0080ab40 (_STL::__unguarded_insertion_sort_aux over NPlayerTargeting::CTargetPoint by CThingCompare_BestForTargeting2): the range comes out ordered by the comparator, every element
// moved whole (all members still agree with its key), and the by-value copies are balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
struct CCharString {                            // stands in for the engine string (copy 0x0099EC30, dtor 0x0099EAE0)
    const char* m_pData;
    explicit CCharString(int k) : m_pData(g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return m_pData[0] - 'a'; }
};
struct C3DVector {                              // memberwise user copy constructor, as retail
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
struct CThing { int id; };
static CThing g_things[8];
struct CBaseIntelligentPointer {                // stands in for the engine pointer (ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90)
    CThing* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_live; }
    virtual ~CBaseIntelligentPointer() { --g_live; }
    CThing* GetPItem() const { return PData; }
    void SetPItem(CThing* item) { PData = item; }
};
struct CIntelligentPointer : public CBaseIntelligentPointer {
    explicit CIntelligentPointer(CThing* t) : CBaseIntelligentPointer() { SetPItem(t); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTargetPoint {
    CIntelligentPointer PThing;
    long                Index;
    C3DVector           TargetingPosition;
    long                FrameRejected;
    explicit CTargetPoint(int k) : PThing(&g_things[k]), Index(k), TargetingPosition(10.0f + (float)k, 0.0f, 1.0f), FrameRejected(-k) { }
};
typedef CTargetPoint T;
struct CVectorCompare_BestForTargeting {
    C3DVector AnchorPos;
    C3DVector PreferredLine;
    float     AngleCompare;
    float     SqrDistCompare;
    float     OverrideShortDistanceFactor;
    float     OverrideShortDistanceFalloff;
    bool      CompareInXYPlane;
    CVectorCompare_BestForTargeting() : AnchorPos(10.0f, 0.0f, 0.0f), PreferredLine(1.0f, 0.0f, 0.0f), AngleCompare(0.0f), SqrDistCompare(1.0f),
                                        OverrideShortDistanceFactor(0.0f), OverrideShortDistanceFalloff(0.0f), CompareInXYPlane(true) { }
};
struct C {                                      // CThingCompare_BestForTargeting2: 52 bytes, passed by value
    CThing*                         Hero;
    CVectorCompare_BestForTargeting VectorCompare;
    float                           EnemyPreferenceFactor;
    C() : Hero(0), EnemyPreferenceFactor(1.0f) { }
    float Score(const T& t) const
    {
        float dx = t.TargetingPosition.X - VectorCompare.AnchorPos.X;   // the nearer to the anchor along X, the better
        return dx * dx * VectorCompare.SqrDistCompare * EnemyPreferenceFactor;
    }
    bool operator()(const T& a, const T& b) const { return Score(a) < Score(b); }
};
static int KeyOf(const T& x) { return (int)x.Index; }
static bool Consistent(const T& x) { return x.PThing.GetPItem() == &g_things[x.Index] && x.TargetingPosition.X == 10.0f + (float)x.Index && x.FrameRejected == -x.Index; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x0080A340)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)   // the function under test
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
#define FAIL() { std::printf("0080ab40_TEST FAIL\n"); return 1; }
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };       // keys[0] is the minimum: the guard the unguarded insert relies on
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(T(keys[i]));
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);   // an empty range changes nothing
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) FAIL();
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) FAIL();
    if (g_live != base + 1 * 8) FAIL();
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    if (g_live != base) FAIL();
    std::printf("0080ab40_TEST PASS\n");
    return 0;
}