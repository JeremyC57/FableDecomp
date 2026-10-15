// Behaviour test for 0x004B39B0: ClearQuestPersistScript erases the key's range from the sorted persistence map and clears the thing manager section.
#include <cstdio>
#include <cstring>
struct Pair { const char* key; int value; };
struct Map { Pair data[8]; int n;
  int LowerBound(const char* k) { int i = 0; while (i < n && std::strcmp(data[i].key, k) < 0) ++i; return i; }
  int UpperBound(const char* k) { int i = 0; while (i < n && std::strcmp(data[i].key, k) <= 0) ++i; return i; } };
struct Things { const char* cleared; int calls; void ClearPersistenceDataForSection(const char* s) { cleared = s; ++calls; } };
struct QM { Map map; Things things;
  void ClearQuestPersistScript(const char* name) {
    int it = map.LowerBound(name); int end = map.n;
    if (it != end) { int last = map.UpperBound(name); int i = it; for (int j = last; j < end; ++j, ++i) data_move(i, j); map.n = i; }
    things.ClearPersistenceDataForSection(name); }
  void data_move(int to, int from) { map.data[to] = map.data[from]; } };
int main()
{
    QM q; Pair p[5] = {{"A", 1}, {"B", 2}, {"B", 3}, {"C", 4}, {"D", 5}}; for (int i = 0; i < 5; ++i) q.map.data[i] = p[i]; q.map.n = 5; q.things.calls = 0;
    q.ClearQuestPersistScript("B");
    bool ok = q.map.n == 3 && q.map.data[1].value == 4 && q.map.data[2].value == 5 && q.things.calls == 1 && std::strcmp(q.things.cleared, "B") == 0;
    q.ClearQuestPersistScript("Z"); ok = ok && q.map.n == 3 && q.things.calls == 2;
    if (!ok) { std::printf("004b39b0_TEST FAIL\n"); return 1; }
    std::printf("004b39b0_TEST PASS\n");
    return 0;
}
