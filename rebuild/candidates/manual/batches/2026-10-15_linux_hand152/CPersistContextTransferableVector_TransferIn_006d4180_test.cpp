#include <cstdio>
#include <cstring>
struct Str { char b[16]; Str(const char* t) { std::strcpy(b, t); } };
template <class A, class B> struct Pair { A first; B second; Pair(const A& a, const B& b) : first(a), second(b) {} };
static const Pair<Str, long>* g_def = 0; static Pair<Str, long>* g_target = 0;
struct Traits {
    Pair<Str, long> DefaultVal;
    Traits() : DefaultVal("", 0) {}
    void TransferIn(void*, void*, Pair<Str, long>* v) { g_def = &DefaultVal; g_target = v; v->second = (long)std::strlen(DefaultVal.first.b) + 40 + DefaultVal.second; }
};
struct Vec { Pair<Str, long>* first; };
struct TV { Vec* v; void TransferIn(void* c, void* p, long index) { Traits t; t.TransferIn(c, p, &*(v->first + index)); } };
int main()
{
    Pair<Str, long> arr[3] = { Pair<Str, long>("x", 0), Pair<Str, long>("y", 1), Pair<Str, long>("z", 2) };
    Vec v; v.first = arr; TV t; t.v = &v;
    t.TransferIn(0, 0, 2);
    if (g_target != &arr[2] || arr[2].second != 40) { std::printf("006d4180_TEST FAIL\n"); return 1; }
    std::printf("006d4180_TEST PASS\n");
    return 0;
}
