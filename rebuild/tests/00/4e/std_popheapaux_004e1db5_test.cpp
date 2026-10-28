// Behaviour test for 0x004E1DB5 std::__pop_heap_aux over pair<CCharString, CTCBase::CRegisteredTCInfo>: copies the last element, then hands (first, last-1, last-1, that copy, comp, 0) to __pop_heap
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct Pair { int key; int value; };
static Pair* g_first; static Pair* g_last; static Pair* g_result; static Pair g_val; static int g_calls = 0;
static void pop_heap(Pair* first, Pair* last, Pair* result, Pair val, int*) { ++g_calls; g_first = first; g_last = last; g_result = result; g_val = val; }
static void pop_heap_aux(Pair* first, Pair* last) { pop_heap(first, last - 1, last - 1, Pair(*(last - 1)), (int*)0); }
int main()
{
    Pair v[3] = { { 1, 10 }, { 2, 20 }, { 3, 30 } };
    pop_heap_aux(v, v + 3);
    CHECK(g_calls == 1 && g_first == v && g_last == v + 2 && g_result == v + 2);
    CHECK(g_val.key == 3 && g_val.value == 30);
    if (fails == 0) printf("004e1db5_TEST PASS\n"); else printf("004e1db5_TEST FAIL\n");
    return fails;
}