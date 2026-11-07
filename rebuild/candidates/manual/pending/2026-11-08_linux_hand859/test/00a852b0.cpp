// Behaviour test for 0x00A852B0 CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<...> >::Insert: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <list>
#include <vector>
#include <utility>
struct C2DLineF { float X0, Y0, X1, Y1; };
template <class K, class V> struct CVectorMap : std::vector<std::pair<K, V> > {
    bool Dirty;
    CVectorMap() : Dirty(false) { }
    void Insert(const K& key, const V& value) { this->push_back(std::pair<K, V>(key, value)); Dirty = true; }
};
int main() {
    CVectorMap<unsigned long long, std::list<C2DLineF> > m;
    std::list<C2DLineF> lines; C2DLineF a = { 1.0f, 2.0f, 3.0f, 4.0f }; C2DLineF b = { 5.0f, 6.0f, 7.0f, 8.0f };
    lines.push_back(a); lines.push_back(b);
    m.Insert(42ULL, lines);
    lines.clear();
    int ok = m.size() == 1 && m.Dirty && m[0].first == 42ULL && m[0].second.size() == 2 && m[0].second.front().X0 == 1.0f && m[0].second.back().Y1 == 8.0f;
    std::printf(ok ? "00a852b0_TEST PASS\n" : "00a852b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
