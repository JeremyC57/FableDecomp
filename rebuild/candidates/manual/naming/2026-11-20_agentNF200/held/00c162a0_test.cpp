// Independent behaviour test for 0x00C162A0 SPKVoiceSettings::~SPKVoiceSettings: the only work is destroying the member at +4.
#include <cstdio>

struct CMember_00c2d3b0 {
    unsigned char _body[8];
    ~CMember_00c2d3b0();
};

struct SPKVoiceSettings {
    unsigned long    m_id;
    CMember_00c2d3b0 m_reference;
    long             m_enabled;
    long             m_priority;
    long             m_delay;
    float            m_gain;
    ~SPKVoiceSettings();
};

static void* g_self;
static int g_calls;

CMember_00c2d3b0::~CMember_00c2d3b0()
{
    g_self = this;
    ++g_calls;
}

SPKVoiceSettings::~SPKVoiceSettings()
{
}

int main()
{
    void* expect_self;
    {
        SPKVoiceSettings s;
        s.m_id = 0x11223344;
        expect_self = (void*)((char*)&s + 4);
    }
    bool ok = (g_self == expect_self) && (g_calls == 1);
    if (ok) {
        printf("CONS_VAL_FWD_OK\n");
        return 0;
    }
    printf("CONS_VAL_FWD_FAIL self=%p exp=%p calls=%d\n", g_self, expect_self, g_calls);
    return 1;
}
