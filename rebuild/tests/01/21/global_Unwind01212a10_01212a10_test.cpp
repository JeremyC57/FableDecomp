// Behaviour test for CSoundEvent::Describe (0x00C21040): the source's logic over a stand-in CString builder (the libpk
// operator<< number formats are modelled with %lu / %ld / %g) and stubbed GetLevel / GetMode / GetRecord.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

class CString { public:
    char m_text[512];
    CString(const char* text) { std::strcpy(m_text, text); }
    CString(const CString& other) { std::strcpy(m_text, other.m_text); }
    CString& operator<<(const char* text) { std::strcat(m_text, text); return *this; }
    CString& operator<<(unsigned long v) { char b[32]; std::sprintf(b, "%lu", v); return *this << b; }
    CString& operator<<(long v) { char b[32]; std::sprintf(b, "%ld", v); return *this << b; }
    CString& operator<<(double v) { char b[32]; std::sprintf(b, "%g", v); return *this << b; }
};
struct CSoundEventManager { };
struct SEventLevel { float m_gain; long m_loops; };
struct SEventRecord { float m_x, m_y, m_z; bool m_headRelative; };
struct CSoundParams {
    float m_pan, m_baseGain, m_userGain;
    SEventRecord m_record;
    SEventRecord GetRecord() const { return m_record; }
};
struct CSoundEvent {
    unsigned long m_index, m_priority, m_idEmitter;
    CSoundParams m_params;
    unsigned short m_state;
    bool m_listening;
    long m_mode;
    SEventLevel m_level;
    void CheckState(long) { }
    void GetLevel(CSoundEventManager*, SEventLevel* level) { *level = m_level; }
    long GetMode() { return m_mode; }
    CString Describe(CSoundEventManager* manager)
    {
        CheckState(-1);
        CString s = CString("EventId(") << m_index << "), Priority=" << m_priority << ", State='";
        switch (m_state) {
        case 2: s << "Waiting for data"; break;
        case 3: s << "Waiting for physical player"; break;
        case 4: s << "Waiting for memory"; break;
        case 5: s << "Playing using virtual player"; break;
        case 6: s << "Playing using physical player"; break;
        }
        SEventLevel level;
        GetLevel(manager, &level);
        s << "', offset(" << level.m_gain * 100.0f << "), loops(" << level.m_loops << "), ";
        if (level.m_gain == 1.0f)
            s << " offset=1.0, ";
        if (m_listening)
            s << "Emitter ID = " << m_idEmitter << ", ";
        else
            s << "Not in behaviour, ";
        if (GetMode()) {
            SEventRecord record = m_params.GetRecord();
            s << ", 3D(";
            if (record.m_headRelative)
                s << "HEADRELATIVE:";
            s << record.m_x << "," << record.m_y << "," << record.m_z << ")";
        } else {
            s << "2D(" << m_params.m_pan << ")";
        }
        s << ", base gain(" << m_params.m_baseGain << "), user gain(" << m_params.m_userGain << ")";
        return s;
    }
};
static void RunTest()
{
    CSoundEventManager manager;
    CSoundEvent e;
    e.m_index = 7; e.m_priority = 3; e.m_idEmitter = 42; e.m_state = 6; e.m_listening = true; e.m_mode = 0;
    e.m_params.m_pan = 0.5f; e.m_params.m_baseGain = 1.0f; e.m_params.m_userGain = 0.25f;
    e.m_level.m_gain = 0.5f; e.m_level.m_loops = 2;
    CString s = e.Describe(&manager);
    CHECK(std::strcmp(s.m_text, "EventId(7), Priority=3, State='Playing using physical player', offset(50), loops(2), "
                                "Emitter ID = 42, 2D(0.5), base gain(1), user gain(0.25)") == 0);
    e.m_state = 1; e.m_listening = false; e.m_mode = 1; e.m_level.m_gain = 1.0f;
    e.m_params.m_record.m_x = 1.0f; e.m_params.m_record.m_y = 2.0f; e.m_params.m_record.m_z = 3.0f; e.m_params.m_record.m_headRelative = true;
    CString t = e.Describe(&manager);
    CHECK(std::strcmp(t.m_text, "EventId(7), Priority=3, State='', offset(100), loops(2),  offset=1.0, Not in behaviour, "
                                ", 3D(HEADRELATIVE:1,2,3), base gain(1), user gain(0.25)") == 0);
    e.m_state = 2;
    CString u = e.Describe(&manager);
    CHECK(std::strstr(u.m_text, "State='Waiting for data'") != 0);
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("00c21040_TEST FAIL\n"); return 1; }
    std::printf("01212a10_TEST PASS\n");
    return 0;
}