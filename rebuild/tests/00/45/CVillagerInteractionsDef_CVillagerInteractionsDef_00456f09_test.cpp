// behaviour test for 0x00456F09 CVillagerInteractionsDef::CVillagerInteractionsDef() (model: the CDefString members start unset)
#include <cstdio>
#include <cstring>
#include <new>
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct Def { CDefString Animation1; CDefString Animation2; float Distance; };
int main()
{
    static double raw[32]; std::memset(raw, 0x11, sizeof(raw));
    Def* d = new (raw) Def;
    int ok = 1;
    if (d->Animation1.TablePos != -1) ok = 0;
    if (d->Animation2.TablePos != -1) ok = 0;
    if (!ok) { std::printf("00456f09_TEST FAIL\n"); return 1; }
    std::printf("00456f09_TEST PASS\n");
    return 0;
}