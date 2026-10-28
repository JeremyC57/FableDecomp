// Behaviour test for 0x0046F6DC CLensFlareElementDef copy constructor: the CParentDefClassBase copy runs, then radius, colour (channel by channel), position and texture are copied
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static const void* g_baseSrc = 0;
class CParentDefClassBase { public: int x; CParentDefClassBase() : x(0) { } CParentDefClassBase(const CParentDefClassBase& o) : x(o.x) { g_baseSrc = &o; } };
class CRGBColour { public: unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() { } CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); } };
class CLensFlareElementDef : public CParentDefClassBase { public:
    float Radius; CRGBColour Colour; float Position; int Texture;
    CLensFlareElementDef() { }
    CLensFlareElementDef(const CLensFlareElementDef& o) : CParentDefClassBase(o), Radius(o.Radius), Colour(o.Colour), Position(o.Position), Texture(o.Texture) { }
};
int main()
{
    CLensFlareElementDef a; a.x = 5; a.Radius = 2.5f; a.Colour.Set(10, 20, 30, 40); a.Position = 0.75f; a.Texture = 1234;
    CLensFlareElementDef b(a);
    CHECK(g_baseSrc == &a && b.x == 5);
    CHECK(b.Radius == 2.5f && b.Position == 0.75f && b.Texture == 1234);
    CHECK(b.Colour.R == 10 && b.Colour.G == 20 && b.Colour.B == 30 && b.Colour.A == 40);
    if (fails == 0) printf("0046f6dc_TEST PASS\n"); else printf("0046f6dc_TEST FAIL\n");
    return fails;
}