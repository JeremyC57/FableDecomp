// Behaviour test for 0x0052AB20 CThingPlayerCreature::CThingPlayerCreature: CThingCreatureBase(2), then the acceleration map is an empty red-black tree (allocated header: colour red, no root, leftmost = rightmost = header, count 0) and both def pointers are null
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static int g_type = -1;
struct Header { bool color; Header* parent; Header* left; Header* right; };
static Header g_header; static int g_allocs = 0;
struct Map { Header* header; unsigned count;
    Map() { ++g_allocs; header = &g_header; count = 0; header->color = false; header->parent = 0; header->left = header; header->right = header; } };
class CThingCreatureBase { public: CThingCreatureBase(int t) { g_type = t; } };
class CThingPlayerCreature : public CThingCreatureBase { public:
    float MaxSprintingSpeed; Map RelativeMovementAccelerationComponents; void* PHeroCombatDef; void* PPlayerMovementDef;
    CThingPlayerCreature() : CThingCreatureBase(2), PHeroCombatDef(0), PPlayerMovementDef(0) { }
};
int main()
{
    g_header.color = true; g_header.parent = &g_header;
    CThingPlayerCreature c;
    CHECK(g_type == 2 && g_allocs == 1);
    CHECK(c.RelativeMovementAccelerationComponents.count == 0);
    CHECK(g_header.color == false && g_header.parent == 0 && g_header.left == &g_header && g_header.right == &g_header);
    CHECK(c.PHeroCombatDef == 0 && c.PPlayerMovementDef == 0);
    if (fails == 0) printf("0052ab20_TEST PASS\n"); else printf("0052ab20_TEST FAIL\n");
    return fails;
}