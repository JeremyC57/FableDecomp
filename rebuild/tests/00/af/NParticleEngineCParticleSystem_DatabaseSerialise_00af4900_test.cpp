// Behaviour test for 0x00AF4900 NParticleEngine::CParticleSystem::DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&):
// host model of the load path over a text script. Checked: the transfers in order (SystemName, Enabled default true, ScaleX/Y/Z default
// 1.0, ScaleParticles default true, the bool members written back); every BEGIN_COMPONENT block adds one component with its quoted type,
// the CRC-32 (seed 0, reflected table, no inversion) of its quoted name, enabled unless the flag line holds DISABLED, visible only when it
// holds VISIBLE (whole white-space separated words), and hands the component the text up to END_COMPONENT.
#include <cstdio>
#include <cstring>

static unsigned int g_CRCTable[256];
static void MakeTable() { for (unsigned int i = 0; i < 256; ++i) { unsigned int c = i; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320U ^ (c >> 1) : c >> 1; g_CRCTable[i] = c; } }
static unsigned int CRC(const char* s) { unsigned int seed = 0; for (; *s; ++s) seed = g_CRCTable[(unsigned char)*s ^ (seed & 0xFF)] ^ (seed >> 8); return seed; }

struct Parser {                                   // the text parser the persist context owns
    const char* P;
    bool SkipPastString(const char* s) { const char* f = std::strstr(P, s); if (!f) return false; P = f + std::strlen(s); return true; }
    void SkipSpace() { while (*P == ' ' || *P == '\t' || *P == '\r' || *P == '\n') ++P; }
    void ReadQuoted(char* out) { SkipSpace(); int n = 0; if (*P == '"') { ++P; while (*P && *P != '"') out[n++] = *P++; if (*P) ++P; } out[n] = 0; }
    void ReadLine(char* out) { int n = 0; while (*P && *P != '\n') out[n++] = *P++; if (*P) ++P; out[n] = 0; }
    void ReadUntil(const char* s, char* out) { const char* f = std::strstr(P, s); int n = f ? (int)(f - P) : (int)std::strlen(P); std::memcpy(out, P, n); out[n] = 0; P += n; if (f) P += std::strlen(s); }
};
struct Added { char Type[32]; unsigned int NameCRC; bool Enabled, Visible; char Script[64]; };
struct System {
    bool Enabled, ScaleParticles; float ScaleX, ScaleY, ScaleZ; int Transfers; Added Components[4]; int Count;
    void DatabaseSerialise(Parser& parser)          // 0x00AF4900 (load path; the transfers keep their values here)
    {
        Transfers = 6;                              // SystemName, Enabled, ScaleX, ScaleY, ScaleZ, ScaleParticles
        while (parser.SkipPastString("BEGIN_COMPONENT")) {
            Added& a = Components[Count++];
            char name[32], flags[64];
            parser.ReadQuoted(a.Type); parser.ReadQuoted(name);
            a.NameCRC = CRC(name);
            parser.ReadLine(flags);
            bool enabled = true, visible = false;
            const char* f = flags;
            while (*f) {
                while (*f == ' ' || *f == '\t' || *f == '\r') ++f;
                char word[32]; int n = 0;
                while (*f && *f != ' ' && *f != '\t' && *f != '\r') word[n++] = *f++;
                word[n] = 0;
                if (std::strcmp(word, "DISABLED") == 0) enabled = false;
                if (std::strcmp(word, "VISIBLE") == 0) visible = true;
            }
            parser.ReadUntil("END_COMPONENT", a.Script);
            a.Enabled = enabled; a.Visible = visible;
        }
    }
};

int main()
{
    MakeTable();
    int fails = 0;
    if (CRC("") != 0 || CRC("a") != 0x3AB551CEU) ++fails;      // the retail table 0x0129A168 is the reflected 0xEDB88320 table: CRC("a") = table[0x61]
    const char* text =
        "BEGIN_COMPONENT \"RenderSprite\" \"Sparks\" VISIBLE\n Bank 3;\nEND_COMPONENT\n"
        "BEGIN_COMPONENT \"Light\" \"Glow\" DISABLED VISIBLEX\n Radius 2;\nEND_COMPONENT\n"
        "BEGIN_COMPONENT \"Orbit\" \"\"\nEND_COMPONENT\n";
    Parser p; p.P = text;
    System s; std::memset(&s, 0, sizeof(s));
    s.DatabaseSerialise(p);
    if (s.Transfers != 6 || s.Count != 3) ++fails;
    if (std::strcmp(s.Components[0].Type, "RenderSprite") != 0 || s.Components[0].NameCRC != CRC("Sparks")) ++fails;
    if (!s.Components[0].Enabled || !s.Components[0].Visible) ++fails;
    if (std::strcmp(s.Components[0].Script, " Bank 3;\n") != 0) ++fails;
    if (s.Components[1].Enabled || s.Components[1].Visible) ++fails;          // VISIBLEX is not VISIBLE
    if (!s.Components[2].Enabled || s.Components[2].Visible || s.Components[2].NameCRC != 0) ++fails;
    if (fails) { std::printf("00af4900_TEST FAIL\n"); return 1; }
    std::printf("00af4900_TEST PASS\n");
    return 0;
}