// Behaviour test for 00c6a3d0: CReverbProcessor::Shutdown -- not ready, memory freed once, blocks released, line detached
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_seq[128] = "";
static void Log(const char* s) { std::strcat(g_seq, s); }
static int g_freed = 0;
class CPKObject { public: static void operator delete(void* p) { ++g_freed; std::free(p); } };
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { } };
class CVtable_012bf0c8 : public CDspObject {
public:
    CVtable_012bf0c8() : m_pSettings(0), m_pTargetSettings(0), m_pPendingSettings(0), m_settingsSize(0), m_blending(false), m_dirty(false) { Log("B"); }
    virtual ~CVtable_012bf0c8() { Log("~B"); }
    void ReleaseBlock() { Log("R"); m_pSettings = m_pTargetSettings = m_pPendingSettings = 0; }
    void* m_pSettings; void* m_pTargetSettings; void* m_pPendingSettings; unsigned long m_settingsSize; bool m_blending; unsigned char _pad15[0x24 - 0x15]; bool m_dirty;
};
class CSoundFormatBase {
public:
    CSoundFormatBase() { m_sampleRate = 48000; m_channels = 2; m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0; }
    virtual ~CSoundFormatBase() { Log("~F"); }
    long m_channels; long m_sampleRate; long m_0c, m_10, m_14, m_18;
};
class CVtable_012b9584 : public CSoundFormatBase { public: CVtable_012b9584() { Log("F"); } ~CVtable_012b9584() { } };
class CDspGain : public CDspObject { public: CDspGain() { Log("G"); } ~CDspGain() { Log("~G"); } float m_gain[4]; };
class CReverbTapBank { public: CReverbTapBank() { Log("T"); } ~CReverbTapBank() { Log("~T"); } unsigned char _body[0xb0]; };
class CReverbDiffuser { public: CReverbDiffuser() { Log("D"); } ~CReverbDiffuser() { Log("~D"); } unsigned char _body[0x9c]; };
class CReverbPreDelay : public CDspObject { public: CReverbPreDelay() { Log("P"); } ~CReverbPreDelay() { Log("~P"); } unsigned char _body[0x14]; };
class CCombFilter : public CDspObject { public: CCombFilter() { Log("C"); } ~CCombFilter() { Log("~C"); } unsigned char _pad04[0x0c]; long m_mode; };
class CReverbDelay : public CDspObject { public: CReverbDelay() { Log("L"); } ~CReverbDelay() { Log("~L"); } unsigned char _body[0x10]; };
class CDelayLine { public: CDelayLine() : m_data(0), m_size(0), m_pos(0) { } virtual ~CDelayLine() { } float* m_data; unsigned int m_size; unsigned int m_pos; };
class CStereoDelayLine { public: CStereoDelayLine() { Log("S"); } virtual ~CStereoDelayLine() { Log("~S"); } void Detach() { Log("X"); } CDelayLine m_lines[2]; };
struct SReverbRing { float* m_data; unsigned long m_length; unsigned long m_position; };
class CReverbProcessor : public CVtable_012bf0c8 {
public:
    CReverbProcessor() : m_pMemory(0), m_memoryBytes(0), m_bWasActive(false), m_bReady(false) { m_settingsSize = 0x234; }
    void Shutdown() { m_bReady = false; if (m_pMemory) { CPKObject::operator delete(m_pMemory); m_pMemory = 0; } ReleaseBlock(); m_stereoDelay.Detach(); }
    virtual ~CReverbProcessor() { m_bReady = false; if (m_pMemory) { CPKObject::operator delete(m_pMemory); m_pMemory = 0; } ReleaseBlock(); m_stereoDelay.Detach(); }
    CVtable_012b9584 m_format; unsigned char _pad44[0x54 - 0x44]; SReverbRing m_combLine; unsigned char _pad60[0x6c - 0x60];
    CDspGain m_inputGain; CDspGain m_outputGain; CReverbTapBank m_taps; CReverbDiffuser m_diffuser; CReverbPreDelay m_preDelay;
    CCombFilter m_combs[4]; CReverbDelay m_delay; CStereoDelayLine m_stereoDelay;
    void* m_pMemory; unsigned long m_memoryBytes; unsigned long m_288; bool m_bWasActive; unsigned long m_290; bool m_bReady;
};
#define OFF(m) ((char*)&r->m - (char*)r)
int main()
{
    CReverbProcessor* r = new CReverbProcessor; r->m_pMemory = std::malloc(64); r->m_bReady = true;
    g_seq[0] = 0;
    r->Shutdown();
    int ok = std::strcmp(g_seq, "RX") == 0 && g_freed == 1 && r->m_pMemory == 0 && !r->m_bReady;
    g_seq[0] = 0; r->Shutdown(); ok = ok && g_freed == 1 && std::strcmp(g_seq, "RX") == 0;
    delete r;
    std::printf(ok ? "00c6a3d0_TEST PASS\n" : "00c6a3d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
