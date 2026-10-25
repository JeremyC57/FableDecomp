// Behaviour test for 00c75980: CReverbDiffuser::CReverbDiffuser -- base, three gains, two stages, no configuration
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
    CReverbDiffuser* d = new CReverbDiffuser;
    int ok = std::strcmp(g_seq, "oogogogoogSoogSD") == 0 && d->m_pConfig == 0 && sizeof(CReverbDiffuser) == 0x9c
          && (char*)&d->m_stages[0] - (char*)d == 0x40 && (char*)&d->m_pConfig - (char*)d == 0x90;
    delete d;
    std::printf(ok ? "00c75980_TEST PASS\n" : "00c75980_TEST FAIL\n");
    return ok ? 0 : 1;
}
