// Behaviour test for 00c75a00: CReverbDiffuser scalar deleting destructor -- destroy (stages, gains, base), then free
#include <cstdio>
#include <cstring>
static char g_seq[64] = "";
static void Log(const char* s) { std::strcat(g_seq, s); }
class CDspObject { public: CDspObject() { Log("o"); } virtual ~CDspObject() { Log("~o"); } };
class CDspGain : public CDspObject { public: CDspGain() { Log("g"); } ~CDspGain() { Log("~g"); } float m_gain[4]; };
class CReverbFilterTap : public CDspObject {
public:
    CReverbFilterTap() { Log("T"); }
    CDspGain m_gain; float* m_data; unsigned int m_size; unsigned int m_pos; unsigned int m_index;
};
class CReverbDiffuserStage : public CDspObject {
public:
    CReverbDiffuserStage() { Log("S"); }
    float* m_data; unsigned int m_size; unsigned int m_pos; CDspGain m_gain; unsigned int m_index;
};
struct CReverbConfig;
class CReverbDiffuser : public CDspObject {
public:
    CReverbDiffuser() : m_pConfig(0) { Log("D"); }
    CDspGain m_gains[3]; CReverbDiffuserStage m_stages[2]; CReverbConfig* m_pConfig; unsigned int m_firstIndex; unsigned int m_endIndex;
};

int main()
{
    CReverbDiffuser* d = new CReverbDiffuser; g_seq[0] = 0;
    delete d;
    int ok = std::strcmp(g_seq, "~g~o~o~g~o~o~g~o~g~o~g~o~o") == 0;
    std::printf(ok ? "00c75a00_TEST PASS\n" : "00c75a00_TEST FAIL\n");
    return ok ? 0 : 1;
}
