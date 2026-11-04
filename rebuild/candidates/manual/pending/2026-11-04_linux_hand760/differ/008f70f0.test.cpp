// Behaviour test for 0x008F70F0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Entry { unsigned long Theme; bool Used; };
struct Map {
    std::vector<Entry> P; std::vector<unsigned char> CellThemes;
    unsigned char Find(unsigned long t) const { for (unsigned long i = 0; i < P.size(); ++i) if (i > 0 && P[i].Theme == t) return (unsigned char)i; return 0; }
    unsigned char FindUnused() const { for (unsigned long i = 0; i < P.size(); ++i) if (i > 0 && !P[i].Used) return (unsigned char)i; return 0; }
    void ZeroAll() { for (size_t i = 0; i < P.size(); ++i) P[i].Used = false; P[0].Theme = 1; P[0].Used = false; for (size_t i = 0; i < CellThemes.size(); ++i) P[CellThemes[i]].Used = true; }
    unsigned char Add(unsigned long t) {
        unsigned char index = Find(t); if (index) return index;
        index = FindUnused(); if (index) { P[index].Theme = t; P[index].Used = true; return index; }
        ZeroAll(); index = FindUnused(); if (index) { P[index].Theme = t; P[index].Used = true; return index; }
        return 0;
    }
};
int main() {
    Map m; Entry e = { 0, false }; m.P.assign(3, e); m.CellThemes.push_back(1);
    bool ok = m.Add(50) == 1 && m.Add(60) == 2 && m.Add(50) == 1;
    ok = ok && m.Add(70) == 2 && m.P[2].Theme == 70;      // retry: entry 2 is unused by the map cells
    m.CellThemes.push_back(2); ok = ok && m.Add(80) == 0;
    std::printf(ok ? "008f70f0_TEST PASS\n" : "008f70f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
