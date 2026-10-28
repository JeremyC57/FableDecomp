// Behaviour test for 00624330 (std::__unguarded_insertion_sort_aux over CTestQuest by CTestQuestNameCompare): the range comes out ordered by the comparator, every element
// moved whole (all members still agree with its key), and the by-value copies are balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
class CCharString { public:                            // stands in for the engine string (copy 0x0099EC30, dtor 0x0099EAE0)
    const char* m_pData;
    explicit CCharString(int k) : m_pData(g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return m_pData[0] - 'a'; }
};
class C3DVector { public:                              // memberwise user copy constructor, as retail
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
class CTestQuest { public:
    CCharString QuestName;
    CCharString HSPName;
    long        ColourIndex;
    CCharString DisplayName;
    CCharString IniFile;
    CCharString OutiFile;
    CCharString QuestCardDefName;
    explicit CTestQuest(int k) : QuestName(k), HSPName(k), ColourIndex(k * 10), DisplayName(k), IniFile(k), OutiFile(k), QuestCardDefName(k) { }
};
typedef CTestQuest T;
struct C {                                      // CTestQuestNameCompare
    bool operator()(const T& a, const T& b) const { return a.QuestName.Key() < b.QuestName.Key(); }
};
static int KeyOf(const T& x) { return x.QuestName.Key(); }
static bool Consistent(const T& x) { return x.HSPName.Key() == x.QuestName.Key() && x.ColourIndex == 10 * x.QuestName.Key() && x.DisplayName.Key() == x.QuestName.Key() && x.IniFile.Key() == x.QuestName.Key() && x.OutiFile.Key() == x.QuestName.Key() && x.QuestCardDefName.Key() == x.QuestName.Key(); }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x00623E90)
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
#define FAIL() { std::printf("00624330_TEST FAIL\n"); return 1; }
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
    if (g_live != base + 6 * 8) FAIL();
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    if (g_live != base) FAIL();
    std::printf("00624330_TEST PASS\n");
    return 0;
}