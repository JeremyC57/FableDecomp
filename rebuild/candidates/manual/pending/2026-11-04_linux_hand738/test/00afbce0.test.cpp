// Behaviour test for NParticleEngine::CPSCRenderSprite::DatabaseSerialise (0x00AFBCE0): a standalone model of the load path --
// graphic names resolve through the symbol map (unknown name -> bank 0); a missing "SpriteFlags" (-1) rebuilds Flags from the legacy
// booleans (FaceMe3D 0x01, CrossedSprites 0x40, RotateAroundCentre 0x20, !SelfIlluminating 0x80, plus SpriteAlignment); packed floats
// are unpacked (GFUnpackFloat: v / (2^bits - 1) * (max - min) + min, rounded to 2 places), transferred and packed again (GFPackFloat:
// round to 2 places, normalise, * (2^bits - 1), round to nearest), so a save / load round trip keeps the packed value.
#include <cstdio>
#include <cmath>
// round half to even, the FPU default the retail fistp uses (portable: no C99 nearbyint in VC7.1)
static double RoundEven(double v)
{
    double f = std::floor(v), d = v - f;
    if (d > 0.5 || (d == 0.5 && std::fmod(f, 2.0) != 0.0)) f += 1.0;
    return f;
}
#include <map>
#include <string>
static float RoundNear(float v, long places) { float s = 1; for (long i = places; i; --i) s *= 10; return (float)RoundEven(v * s) / s; }
static unsigned long Pack(float v, int bits, float lo, float hi) { float n = (RoundNear(v, 2) - lo) / (hi - lo); return (unsigned long)RoundEven(n * ((float)(1 << bits) - 1)); }
static float Unpack(unsigned long p, int bits, float lo, float hi) { float n = p / ((float)(1 << bits) - 1); return RoundNear(n * (hi - lo) + lo, 2); }
typedef std::map<std::string, long> Symbols;
struct Context { std::map<std::string, long> longs; std::map<std::string, bool> bools; std::map<std::string, std::string> names; std::map<std::string, float> floats; };
static bool B(const std::map<std::string, bool>& m, const char* k) { std::map<std::string, bool>::const_iterator i = m.find(k); return i != m.end() && i->second; }
static long L(const std::map<std::string, long>& m, const char* k) { std::map<std::string, long>::const_iterator i = m.find(k); return i != m.end() ? i->second : 0; }
static float F(const std::map<std::string, float>& m, const char* k, float d) { std::map<std::string, float>::const_iterator i = m.find(k); return i != m.end() ? i->second : d; }
struct Sprite {
    long SpriteBankIndex, TrailBankIndex; unsigned long Flags, StartRenderSize, FlickerBias;
    void Load(const Context& c, const Symbols& sym)
    {
        std::map<std::string, std::string>::const_iterator n = c.names.find("SpriteName");
        Symbols::const_iterator s = n == c.names.end() ? sym.end() : sym.find(n->second);
        SpriteBankIndex = s != sym.end() ? s->second : 0;
        n = c.names.find("TrailName"); s = n == c.names.end() ? sym.end() : sym.find(n->second);
        TrailBankIndex = s != sym.end() ? s->second : 0;
        std::map<std::string, long>::const_iterator f = c.longs.find("SpriteFlags");
        long flags = f == c.longs.end() ? -1 : f->second;
        if (flags == -1) {
            Flags = 0;
            Flags |= B(c.bools, "FaceMe3D") ? 1 : 0;
            Flags |= B(c.bools, "CrossedSprites") ? 0x40 : 0;
            Flags |= B(c.bools, "RotateAroundCentre") ? 0x20 : 0;
            Flags |= B(c.bools, "SelfIlluminating") ? 0 : 0x80;
            Flags |= L(c.longs, "SpriteAlignment");
        } else
            Flags = flags;
        float size = F(c.floats, "StartRenderSize", 1.0f);
        StartRenderSize = Pack(size, 11, 0.0f, 20.0f);
        float bias = F(c.floats, "FlickerBias", 0.0f);
        FlickerBias = Pack(bias, 8, -1.0f, 1.0f);
    }
};
int main()
{
    Symbols sym; sym["SPARK"] = 7; sym["TRAIL"] = 12;
    Context c; c.names["SpriteName"] = "SPARK"; c.names["TrailName"] = "NOPE";
    c.bools["FaceMe3D"] = true; c.bools["CrossedSprites"] = true; c.bools["SelfIlluminating"] = false; c.longs["SpriteAlignment"] = 4;
    Sprite sp; sp.Load(c, sym);
    bool ok = sp.SpriteBankIndex == 7 && sp.TrailBankIndex == 0 && sp.Flags == (0x01 | 0x40 | 0x80 | 4)
        && sp.StartRenderSize == Pack(1.0f, 11, 0.0f, 20.0f) && sp.FlickerBias == 128;
    Context d; d.longs["SpriteFlags"] = 0x21; d.floats["StartRenderSize"] = Unpack(1234, 11, 0.0f, 20.0f);
    Sprite sq; sq.Load(d, sym);
    ok = ok && sq.Flags == 0x21 && std::fabs(Unpack(sq.StartRenderSize, 11, 0.0f, 20.0f) - Unpack(1234, 11, 0.0f, 20.0f)) < 0.006f
        && sq.StartRenderSize >= 1233 && sq.StartRenderSize <= 1235;
    printf(ok ? "00afbce0_TEST PASS\n" : "00afbce0_TEST FAIL\n");
    return ok ? 0 : 1;
}
