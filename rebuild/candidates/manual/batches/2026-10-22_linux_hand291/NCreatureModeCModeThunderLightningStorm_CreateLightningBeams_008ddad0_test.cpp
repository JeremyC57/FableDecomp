// Behaviour test for CModeThunderLightningStorm::CreateLightningBeams (0x008DDAD0): CRC-keyed effect lookup, two spawn loops
// that keep only live beams, and the sub-def release.
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>

static unsigned int g_table[256];
static void BuildTable()
{
    for (unsigned int n = 0; n < 256; ++n) {
        unsigned int c = n;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        g_table[n] = c;
    }
}
static unsigned int CalcCRC(unsigned int seed, const char* data, unsigned int length)
{
    if (length == 0)
        return seed;
    for (unsigned int index = 0; index < length; ++index)
        seed = g_table[((const unsigned char*)data)[index] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}
struct Def {
    int refs;
    bool destroyed;
    std::map<unsigned int, int> effects;
    bool GetEffect(const char* name, int& type) const
    {
        unsigned int len = (unsigned int)std::strlen(name);
        std::map<unsigned int, int>::const_iterator it = effects.find(CalcCRC(0, name, len));
        if (it == effects.end())
            return false;
        type = it->second;
        return true;
    }
};
struct Mode {
    std::vector<int> mainBeams, endBeams;
    bool mainCreated;
    int numberOfBeams;
};
static int g_spawned = 0;
static int Spawn(int type) { ++g_spawned; return (g_spawned % 3 == 0) ? -type : type; }   // every third spawn is dead
static void CreateLightningBeams(Mode& m, Def* def)
{
    int type = 0;
    if (def->GetEffect("LIGHTNING_BEAM_EFFECT", type)) {
        m.mainCreated = true;
        for (int i = 0; i < m.numberOfBeams; ++i) {
            int beam = Spawn(type);
            if (beam > 0)
                m.mainBeams.push_back(beam);
        }
    }
    if (def->GetEffect("LIGHTNING_BEAM_END_EFFECT", type)) {
        for (int i = 0; i < m.numberOfBeams; ++i) {
            int beam = Spawn(type);
            if (beam > 0)
                m.endBeams.push_back(beam);
        }
    }
    if (--def->refs == 0)
        def->destroyed = true;
}
int main()
{
    BuildTable();
    int ok = CalcCRC(0xFFFFFFFFu, "123456789", 9) == 0x340BC6D9u;   // reflected CRC-32 without the final xor
    ok = ok && CalcCRC(77, "", 0) == 77;
    Def def;
    def.refs = 2;
    def.destroyed = false;
    def.effects[CalcCRC(0, "LIGHTNING_BEAM_END_EFFECT", 25)] = 9;
    Mode m;
    m.mainCreated = false;
    m.numberOfBeams = 4;
    CreateLightningBeams(m, &def);
    ok = ok && !m.mainCreated && m.mainBeams.empty() && m.endBeams.size() == 3 && m.endBeams[0] == 9 && def.refs == 1
         && !def.destroyed;
    def.effects[CalcCRC(0, "LIGHTNING_BEAM_EFFECT", 21)] = 5;
    CreateLightningBeams(m, &def);
    ok = ok && m.mainCreated && m.mainBeams.size() == 3 && m.mainBeams[0] == 5 && m.endBeams.size() == 5 && def.destroyed;
    std::printf(ok ? "008ddad0_TEST PASS\n" : "008ddad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
