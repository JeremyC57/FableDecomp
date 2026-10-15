#include <cstdio>
#include <cstring>
static int g_lookups = 0; static long g_lastId = -1;
class CWideString { public: unsigned short* m_pData; CWideString() : m_pData(0) {} };
class CCharString { public: const char* m_text; CCharString(const char* text, long = -1) : m_text(text) {} };
struct CVectorMapTags {
    void* m_pFirst; void* m_pLast; void* m_pEnd; unsigned char Compare; bool Dirty;
    CVectorMapTags() : m_pFirst(0), m_pLast(0), m_pEnd(0), Dirty(false) {}
};
struct CASoundBank { int id; };
static CASoundBank g_bank;
struct CSoundBankRegistry { CASoundBank* GetBank(long id) { ++g_lookups; g_lastId = id; return &g_bank; } };
static CSoundBankRegistry g_registry; static CSoundBankRegistry* g_pSoundBankRegistry = &g_registry;
class CSubtitledMessage {
public:
    CASoundBank* PSpeechBank; long SpeechIndex; bool Speech3D; CWideString Text; CCharString Narrator;
    CVectorMapTags AttitudeTags, AnimationTags, CameraTags;
    CSubtitledMessage() : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Narrator("UNKNOWN") { PSpeechBank = g_pSoundBankRegistry->GetBank(1); }
};
int main()
{
    CSubtitledMessage m;
    if (m.PSpeechBank != &g_bank || g_lookups != 1 || g_lastId != 1 || m.SpeechIndex != 0 || !m.Speech3D || m.Text.m_pData != 0
        || std::strcmp(m.Narrator.m_text, "UNKNOWN") != 0 || m.AttitudeTags.m_pFirst != 0 || m.AnimationTags.m_pEnd != 0
        || m.CameraTags.m_pLast != 0 || m.CameraTags.Dirty) { std::printf("004cdb70_TEST FAIL\n"); return 1; }
    std::printf("004cdb70_TEST PASS\n");
    return 0;
}
