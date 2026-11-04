// Behaviour test for vector<CNavigationLayerUpdate>::resize(n) (0x0051D870, parked DIFFER row): a model of the row -- growing appends empty layer updates, shrinking destroys the tail.
#include <stdio.h>
#include <vector>
struct Layer { std::vector<int> Lines; std::vector<int> Blockages; };
static int Check() { std::vector<Layer> v(2); v[0].Lines.push_back(1); v.resize(5); int ok = v.size() == 5 && v[4].Lines.empty() && v[0].Lines.size() == 1;
    v.resize(1); return ok && v.size() == 1 && v[0].Lines[0] == 1; }
int main()
{
    int ok = Check();
    printf(ok ? "0051d870_TEST PASS\n" : "0051d870_TEST FAIL\n");
    return ok ? 0 : 1;
}
