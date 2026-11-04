// Behaviour test for 0x0067E290 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
#include <algorithm>
struct Info { float Health; int Graphic; };
static bool Less(const Info& a, const Info& b) { return a.Health < b.Health; }
static void Transfer(std::vector<Info>& d, int mode, int graphicType) {
    if (mode == 0) { std::sort(d.begin(), d.end(), Less); for (size_t i = 0; i < d.size(); ++i) d[i].Graphic = graphicType; }
}
int main() {
    std::vector<Info> d; Info a = { 0.5f, 0 }, b = { 0.1f, 0 }; d.push_back(a); d.push_back(b);
    Transfer(d, 1, 7); bool ok = d[0].Health == 0.5f && d[0].Graphic == 0;
    Transfer(d, 0, 7); ok = ok && d[0].Health == 0.1f && d[1].Graphic == 7;
    std::printf(ok ? "0067e290_TEST PASS\n" : "0067e290_TEST FAIL\n");
    return ok ? 0 : 1;
}
