// Behaviour test for 0x00551340: NUISystem::CTable::GetDataFromDef -- size, separation vectors and one created component per def sprite
#include <cstdio>
#include <map>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CComponent { long Index; int Initialised; CComponent(long i) : Index(i), Initialised(0) { } void Initialise() { ++Initialised; } };
static std::vector<CComponent*> g_created;
struct CManager { CComponent* CreateComponent(long index) { CComponent* c = new CComponent(index); g_created.push_back(c); return c; } };
static CManager g_manager;
struct CUIDef { float Height, Width; std::map<int, long> Sprites; std::vector<unsigned long> HorizontalSeparations, VerticalSeparations; };
struct CTable {
    float SizeX, SizeY; std::map<int, CComponent*> Sprites; std::vector<unsigned long>* PHorizontalSeparations; std::vector<unsigned long>* PVerticalSeparations;
    const CUIDef* Def;
    void GetDataFromDef() {
        const CUIDef* def = Def;
        SizeX = def->Width; SizeY = def->Height;
        PHorizontalSeparations = new std::vector<unsigned long>(def->HorizontalSeparations);
        PVerticalSeparations = new std::vector<unsigned long>(def->VerticalSeparations);
        for (std::map<int, long>::const_iterator it = def->Sprites.begin(); it != def->Sprites.end(); ++it) {
            Sprites[(*it).first] = g_manager.CreateComponent((*it).second);
            Sprites[(*it).first]->Initialise();
        }
    }
};
int main()
{
    CUIDef def; def.Height = 10.0f; def.Width = 20.0f; def.Sprites[0] = 100; def.Sprites[3] = 103; def.Sprites[12] = 112;
    def.HorizontalSeparations.push_back(5); def.VerticalSeparations.push_back(7); def.VerticalSeparations.push_back(9);
    CTable t; t.Def = &def; t.GetDataFromDef();
    CHECK(t.SizeX == 20.0f && t.SizeY == 10.0f);
    CHECK(t.PHorizontalSeparations->size() == 1 && (*t.PHorizontalSeparations)[0] == 5);
    CHECK(t.PVerticalSeparations->size() == 2 && (*t.PVerticalSeparations)[1] == 9);
    CHECK(g_created.size() == 3 && t.Sprites.size() == 3);
    CHECK(t.Sprites[3]->Index == 103 && t.Sprites[3]->Initialised == 1 && t.Sprites[12]->Index == 112);
    if (g_fail) { std::printf("00551340_TEST FAIL\n"); return 1; }
    std::printf("00551340_TEST PASS\n");
    return 0;
}
