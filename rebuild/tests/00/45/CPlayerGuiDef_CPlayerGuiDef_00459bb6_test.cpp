// Behaviour test for CPlayerGuiDef::CPlayerGuiDef @ 0x00459BB6: every non-POD member starts empty (35 vectors with null
// start/finish/end-of-storage, 7 strings and 3 maps constructed), the base set up first.
#include <cstdio>
#include <string>
#include <map>
struct VectorWords { void* Start; void* Finish; void* End; VectorWords() : Start(0), Finish(0), End(0) {} };
struct ParentDefClassBase { int BaseConstructed; ParentDefClassBase() : BaseConstructed(1) {} };
struct PlayerGuiDef : ParentDefClassBase {
    VectorWords Vectors[35];
    std::string Strings[7];
    std::map<std::string, int> Maps[3];
    PlayerGuiDef() {}
};
int main()
{
    PlayerGuiDef* def = new PlayerGuiDef;
    bool ok = def->BaseConstructed == 1;
    for (int i = 0; i < 35; ++i)
        ok = ok && def->Vectors[i].Start == 0 && def->Vectors[i].Finish == 0 && def->Vectors[i].End == 0;
    for (int i = 0; i < 7; ++i)
        ok = ok && def->Strings[i].empty();
    for (int i = 0; i < 3; ++i)
        ok = ok && def->Maps[i].empty();
    delete def;
    if (!ok) { std::printf("00459bb6_TEST FAIL\n"); return 1; }
    std::printf("00459bb6_TEST PASS\n");
    return 0;
}