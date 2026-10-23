// Behaviour test for CTCParticleAttacher::AttachParticles (0x007F8890): per attachment, an empty object name attaches one
// particle to the owner itself (type | 2); otherwise one particle per dummy object (type | 2) or helper point (type | 4),
// indexed; type is 9 when the owner is flagged oriented, else 1; dead or failed particles are skipped.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Info { long particle; std::string name; bool dummy; };
struct Attach { long particle; long type; std::string name; long index; };

static int g_created;
static int g_failEvery;
static long CountFor(const std::string& name, bool dummy) { if (name == "A") return dummy ? 2 : 3; return 0; }

static void AttachParticles(const std::vector<Info>& infos, bool oriented, std::vector<Attach>& out)
{
    for (size_t k = 0; k < infos.size(); ++k) {
        const Info& info = infos[k];
        long count = CountFor(info.name, info.dummy);
        long type = 1;
        if (oriented) type = 9;
        if (info.name == "") {
            ++g_created;
            if (g_failEvery && g_created % g_failEvery == 0) continue;
            Attach a = { info.particle, type | 2, "", 0 };
            out.push_back(a);
        } else if (count > 0) {
            for (long i = 0; i != count; ++i) {
                ++g_created;
                if (g_failEvery && g_created % g_failEvery == 0) continue;
                Attach a = { info.particle, type | (info.dummy ? 2 : 4), info.name, i };
                out.push_back(a);
            }
        }
    }
}

int main()
{
    std::vector<Info> infos;
    Info a = { 5, "", false }; infos.push_back(a);
    Info b = { 6, "A", true }; infos.push_back(b);
    Info c = { 7, "A", false }; infos.push_back(c);
    Info d = { 8, "B", false }; infos.push_back(d);
    std::vector<Attach> out;
    g_created = 0; g_failEvery = 0;
    AttachParticles(infos, false, out);
    bool ok = out.size() == 6 && out[0].type == 3 && out[0].particle == 5 && out[1].type == 3 && out[1].index == 0 &&
              out[2].index == 1 && out[3].type == 5 && out[5].index == 2 && out[5].particle == 7;
    out.clear(); g_created = 0; g_failEvery = 2;
    AttachParticles(infos, true, out);
    ok = ok && out.size() == 3 && out[0].type == 11 && out[0].particle == 5 && out[1].index == 1 && out[2].type == 13;
    std::printf(ok ? "007f8890_TEST PASS\n" : "007f8890_TEST FAIL\n");
    return ok ? 0 : 1;
}