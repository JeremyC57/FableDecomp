// behaviour test for 0x004D5ECA CCarriedReadableDef::CCarriedReadableDef() (model: the CDefString members start unset)
#include <cstdio>
#include <cstring>
#include <new>
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct Def { CDefString TitleTextTag; CDefString BodyTextTag; };
int main()
{
    static double raw[32]; std::memset(raw, 0x11, sizeof(raw));
    Def* d = new (raw) Def;
    int ok = 1;
    if (d->TitleTextTag.TablePos != -1) ok = 0;
    if (d->BodyTextTag.TablePos != -1) ok = 0;
    if (!ok) { std::printf("004d5eca_TEST FAIL\n"); return 1; }
    std::printf("004d5eca_TEST PASS\n");
    return 0;
}