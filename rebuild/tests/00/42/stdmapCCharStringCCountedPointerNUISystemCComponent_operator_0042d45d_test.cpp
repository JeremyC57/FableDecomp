#include <cstdio>
#include <cstring>
// Behaviour test for map<CCharString, CCountedPointer<CComponent>>::operator[] (retail 0x0042D45D): an existing key
// returns its value; a missing key is inserted at the lower-bound hint with a null counted pointer and returned.
// Modelled on a sorted array (lower_bound + insert at the hint) with the same decision logic.
struct CCountedPointer { void* Data; void* Info; CCountedPointer() : Data(0), Info(0) {} };
struct Entry { char key[16]; CCountedPointer value; };
struct Map {
    Entry e[8]; int n;
    int lower_bound(const char* k) const { int i = 0; while (i < n && std::strcmp(e[i].key, k) < 0) ++i; return i; }
    int end() const { return n; }
    int insert(int pos, const char* k, const CCountedPointer& v)
    {
        for (int j = n; j > pos; --j) e[j] = e[j - 1];
        std::strcpy(e[pos].key, k); e[pos].value = v; ++n; return pos;
    }
    CCountedPointer& operator[](const char* k)
    {
        int i = lower_bound(k);
        if (i == end() || std::strcmp(k, e[i].key) < 0)
            i = insert(i, k, CCountedPointer());
        return e[i].value;
    }
};
int main()
{
    Map m; m.n = 0;
    int a = 1, b = 2;
    m["MENU_B"].Data = &b;
    m["MENU_A"].Data = &a;
    CCountedPointer& again = m["MENU_B"];
    CCountedPointer& fresh = m["MENU_C"];
    int ok = m.n == 3 && std::strcmp(m.e[0].key, "MENU_A") == 0 && m.e[0].value.Data == &a
          && again.Data == &b && fresh.Data == 0 && fresh.Info == 0 && std::strcmp(m.e[2].key, "MENU_C") == 0;
    if (!ok) { std::printf("0042d45d_TEST FAIL\n"); return 1; }
    std::printf("0042d45d_TEST PASS\n");
    return 0;
}