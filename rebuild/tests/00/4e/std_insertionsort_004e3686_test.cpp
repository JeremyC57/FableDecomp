// Behaviour test for 004e3686 (std::__insertion_sort over SStringEntry16 = pair<CCharString, 12-byte value>): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
class CCharString { public:
    void* m_pData;
    explicit CCharString(int k) : m_pData((void*)g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }   // stands in for 0x0099EC30
    ~CCharString() { --g_live; }                                                   // stands in for 0x0099EAE0
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return ((const char*)m_pData)[0] - 'a'; }
};
struct SValue12 { long m_words[3]; };
struct SStringEntry16 {
    CCharString first;
    SValue12    second;
    SStringEntry16(int k) : first(k) { second.m_words[0] = k; second.m_words[1] = 2 * k; second.m_words[2] = 3 * k; }
    SStringEntry16(const SStringEntry16& other) : first(other.first), second(other.second) { }   // stands in for 0x004D788D
};
struct SStringEntry16Less {
    bool operator()(const SStringEntry16& a, const SStringEntry16& b) const { return a.first.Key() < b.first.Key(); }
};
typedef SStringEntry16 T;
typedef SStringEntry16Less C;
static int KeyOf(const T& x) { return x.first.Key(); }
static bool Consistent(const T& x) { return x.second.m_words[0] == x.first.Key() && x.second.m_words[2] == 3 * x.first.Key(); }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee)
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
void __linear_insert(T* first, T* last, T val, C comp)
{
    if (comp(val, *first)) {
        for (T* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    } else {
        __unguarded_linear_insert(last, val, comp);
    }
}
void __insertion_sort(T* first, T* last, C comp)
{
    if (first == last)
        return;
    for (T* i = first + 1; i != last; ++i)
        __linear_insert(first, i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 6, 3, 4, 1, 7, 2, 5, 0 };
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(keys[i]);
    C comp = C();
    __insertion_sort(items, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("004e3686_TEST FAIL\n"); return 1; }
    __insertion_sort(items, items + 8, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("004e3686_TEST FAIL\n"); return 1; }
    if (!(g_live == 8)) { std::printf("004e3686_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    std::printf("004e3686_TEST PASS\n");
    return 0;
}