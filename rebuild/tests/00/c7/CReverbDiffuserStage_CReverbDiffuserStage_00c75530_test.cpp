// Behaviour test for 00c75530: CReverbDiffuserStage::CReverbDiffuserStage -- base, then the gain member at +0x10
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
    CReverbDiffuserStage* s = new CReverbDiffuserStage;
    int ok = std::strcmp(g_seq, "oogS") == 0 && sizeof(CReverbDiffuserStage) == 0x28 && (char*)&s->m_gain - (char*)s == 0x10;
    delete s;
    std::printf(ok ? "00c75530_TEST PASS\n" : "00c75530_TEST FAIL\n");
    return ok ? 0 : 1;
}
