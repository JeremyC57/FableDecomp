// Behaviour test for CCombatSequenceDef::operator= (0x00461D50).
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        for (int i = 0; i < 0x38; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CActionRegisteredTypeVector {
    long* m_first;
    long* m_last;
    long* m_end;
    CActionRegisteredTypeVector& operator=(const CActionRegisteredTypeVector& other)
    {
        m_first = other.m_first; m_last = other.m_last; m_end = other.m_end;
        g_order[g_calls++] = 2;
        return *this;
    }
};

struct CCombatSequenceDef : CParentDefClassBase {
    long                        ValidCombatZones;
    long                        Type;
    long                        InterruptionType;
    long                        IsValidType;
    long                        OnStartType;
    long                        OnStopType;
    CActionRegisteredTypeVector ActionList;
    float                       SecondsToPredictTargetPosition;
    bool                        PreferIfValid;
    CCombatSequenceDef& operator=(const CCombatSequenceDef& other);
};

CCombatSequenceDef& CCombatSequenceDef::operator=(const CCombatSequenceDef& other)
{
    CParentDefClassBase::operator=(other);
    ValidCombatZones = other.ValidCombatZones;
    Type = other.Type;
    InterruptionType = other.InterruptionType;
    IsValidType = other.IsValidType;
    OnStartType = other.OnStartType;
    OnStopType = other.OnStopType;
    ActionList = other.ActionList;
    SecondsToPredictTargetPosition = other.SecondsToPredictTargetPosition;
    PreferIfValid = other.PreferIfValid;
    return *this;
}

int main()
{
    static long actions[3] = { 4, 5, 6 };
    CCombatSequenceDef a, b;
    for (int i = 0; i < 0x38; ++i) { a._body[i] = 0; b._body[i] = (unsigned char)(i + 3); }
    a.ValidCombatZones = a.Type = a.InterruptionType = a.IsValidType = a.OnStartType = a.OnStopType = 0;
    b.ValidCombatZones = 11; b.Type = 12; b.InterruptionType = 13; b.IsValidType = 14; b.OnStartType = 15; b.OnStopType = 16;
    a.ActionList.m_first = a.ActionList.m_last = a.ActionList.m_end = 0;
    b.ActionList.m_first = actions; b.ActionList.m_last = actions + 3; b.ActionList.m_end = actions + 3;
    a.SecondsToPredictTargetPosition = 0.0f; b.SecondsToPredictTargetPosition = 1.5f;
    a.PreferIfValid = false; b.PreferIfValid = true;
    CCombatSequenceDef& r = (a = b);
    if (&r != &a || g_calls != 2 || g_order[0] != 1 || g_order[1] != 2 || a._body[0] != 3
        || a.ValidCombatZones != 11 || a.Type != 12 || a.InterruptionType != 13 || a.IsValidType != 14
        || a.OnStartType != 15 || a.OnStopType != 16 || a.ActionList.m_last != actions + 3
        || a.SecondsToPredictTargetPosition != 1.5f || !a.PreferIfValid) {
        std::printf("00461d50_TEST FAIL\n");
        return 1;
    }
    std::printf("00461d50_TEST PASS\n");
    return 0;
}