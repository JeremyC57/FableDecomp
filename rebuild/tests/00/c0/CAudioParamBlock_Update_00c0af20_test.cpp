// behaviour test for 0x00C0AF20 CAudioParamBlock::Update: advance every running gain ramp by one tick, refresh the owner when any ran
#include <cstdio>

static int g_updates = 0, g_refreshes = 0, g_spriteUpdates = 0;
static float g_lastDt = 0.0f;

struct CRampedParam {
    float m_value;
    float m_target;
    float m_targetRate;
    float m_rate;
    float m_10;
    float m_time;
    float m_duration;
    float m_coef[5];
    bool  m_bAngle;
    void Update(float dt) { ++g_updates; g_lastDt = dt; m_time = m_duration; }   // stand-in for 0x00C0B7B0
};

struct CDisplay { void Refresh() { ++g_refreshes; } };
struct CSprite { void Update() { ++g_spriteUpdates; } };

struct CAudioOwner {
    CDisplay* m_pDisplay;
    CSprite* m_pSprite;
    float m_tickRate;
};

struct CAudioParamBlock {
    void* m_vtable;
    CRampedParam m_gains[16];
    float m_pitchFactors[8];
    CAudioOwner* m_pOwner;
    void Update();
};

void CAudioParamBlock::Update()
{
    bool changed = false;
    for (int i = 0; i < 16; ++i) {
        if (m_gains[i].m_time != m_gains[i].m_duration) {
            m_gains[i].Update(1000.0f / m_pOwner->m_tickRate);
            changed = true;
        }
    }
    if (changed) {
        m_pOwner->m_pDisplay->Refresh();
        if (m_pOwner->m_pSprite)
            m_pOwner->m_pSprite->Update();
    }
}

int main()
{
    CDisplay display; CSprite sprite;
    CAudioOwner owner; owner.m_pDisplay = &display; owner.m_pSprite = &sprite; owner.m_tickRate = 50.0f;
    CAudioParamBlock block; block.m_pOwner = &owner;
    for (int i = 0; i < 16; ++i) { block.m_gains[i].m_time = 1.0f; block.m_gains[i].m_duration = 1.0f; }
    block.Update();                                               // nothing running: no refresh
    if (g_updates != 0 || g_refreshes != 0 || g_spriteUpdates != 0) { std::printf("00c0af20_TEST FAIL\n"); return 1; }
    block.m_gains[3].m_duration = 2.0f;
    block.m_gains[15].m_time = 0.5f;
    block.Update();
    if (g_updates != 2 || g_refreshes != 1 || g_spriteUpdates != 1) { std::printf("00c0af20_TEST FAIL\n"); return 1; }
    if (g_lastDt < 19.999f || g_lastDt > 20.001f) { std::printf("00c0af20_TEST FAIL\n"); return 1; }
    owner.m_pSprite = 0;
    block.m_gains[7].m_duration = 3.0f;
    block.Update();
    if (g_updates != 3 || g_refreshes != 2 || g_spriteUpdates != 1) { std::printf("00c0af20_TEST FAIL\n"); return 1; }
    std::printf("00c0af20_TEST PASS\n");
    return 0;
}