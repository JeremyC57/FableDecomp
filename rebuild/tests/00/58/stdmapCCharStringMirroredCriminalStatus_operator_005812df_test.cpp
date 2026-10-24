// Behaviour test for 0x005812DF std::map<CCharString, MirroredCriminalStatus>::operator[] (model copy over a sorted array)
#include <cstdio>
#include <cstring>
struct Status { long Crimes[10]; long Seen; long Exited; bool Teleporter; float X, Y, Z, Angle;
    Status() { for (int i = 0; i < 10; ++i) Crimes[i] = 0; Seen = -1; Exited = -1; Teleporter = false; X = Y = Z = Angle = 0.0f; } };
struct Entry { char key[16]; Status value; };
struct FakeMap {
    Entry e[8]; int n;
    int lower_bound(const char* k) const { int i = 0; while (i < n && std::strcmp(e[i].key, k) < 0) ++i; return i; }
    int insert(int pos, const char* k, const Status& v) { for (int j = n; j > pos; --j) e[j] = e[j - 1]; std::strcpy(e[pos].key, k); e[pos].value = v; ++n; return pos; }
    Status& operator[](const char* k)
    {
        int i = lower_bound(k);
        if (i == n || std::strcmp(k, e[i].key) < 0) { Status def; i = insert(i, k, def); }
        return e[i].value;
    }
};
int main()
{
    FakeMap m; m.n = 0;
    m["Oakvale"].Crimes[2] = 5;
    m["Bowerstone"].Seen = 100;
    Status& s = m["Oakvale"];
    if (m.n != 2 || s.Crimes[2] != 5 || std::strcmp(m.e[0].key, "Bowerstone") != 0 || m.e[0].value.Seen != 100 || m.e[1].value.Exited != -1) { std::printf("005812df_TEST FAIL\n"); return 1; }
    std::printf("005812df_TEST PASS\n");
    return 0;
}