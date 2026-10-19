#include <cstdio>
#include <cstring>

// Source under test, inlined so the test is self-contained (verifier compiles from a scratch dir).
// CFrontendGameComponent::SetMode @ 0x00495FE0.

#include <cstring>

// The two lists are different types with different refresh bodies: the save list's Refresh is 0x00495830, the
// profile list's (a disk enumeration with wide-string path pieces "*", ":", "\\") is 0x004959E0.
class CFrontendSaveList_00495fe0
{
public:
    void Refresh();                 // 0x00495830
};
class CFrontendProfileList_00495fe0
{
public:
    void Refresh();                 // 0x004959E0
};

class CFrontendModePointer_00495fe0
{
    void* m_Data;

public:
    void Assign(const CFrontendModePointer_00495fe0*);
};

extern CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;

class CFrontendGameComponent_00495fe0
{
    unsigned char m_Pad0000[0x1c];
    long m_Mode;
    unsigned char m_Pad0020[0x148];
    long m_ResetValues[32];
    long m_Cursor;
    unsigned char m_Pad01ec[0x1c20];
    CFrontendProfileList_00495fe0 m_ProfileList;
    unsigned char m_Pad1e0d[0x0b];
    long m_ProfileIndex;
    CFrontendSaveList_00495fe0 m_SaveList;
    unsigned char m_Pad1e1d[0x0b];
    long m_SaveIndex;
    unsigned char m_Pad1e2c[0x14];
    CFrontendModePointer_00495fe0 m_ModePointer;
    long m_ModePointerEnabled;

    void PrepareModeTen();

public:
    void SetMode(long);
};

void CFrontendGameComponent_00495fe0::SetMode(long mode)
{
    m_Mode = mode;
    m_ModePointerEnabled = 1;
    std::memset(m_ResetValues, 0, sizeof(m_ResetValues));
    m_Cursor = 0;

    if (m_Mode == 10)
    {
        PrepareModeTen();
        return;
    }

    if (mode == 11)
    {
        m_SaveList.Refresh();
        m_ModePointer.Assign(&g_EmptyFrontendModePointer_00495fe0);
        m_SaveIndex = 0;
        return;
    }

    if (mode == 12)
    {
        m_ProfileList.Refresh();
        m_ProfileIndex = 0;
    }
}


CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;

static unsigned long g_PrepareCalls;
static unsigned long g_RefreshCalls;
static unsigned long g_AssignCalls;
static void* g_PrepareThis;
static void* g_RefreshThis;
static void* g_AssignThis;
static const CFrontendModePointer_00495fe0* g_AssignValue;

void CFrontendGameComponent_00495fe0::PrepareModeTen()
{
    ++g_PrepareCalls;
    g_PrepareThis = this;
}

void CFrontendSaveList_00495fe0::Refresh()
{
    ++g_RefreshCalls;
    g_RefreshThis = this;
}

void CFrontendProfileList_00495fe0::Refresh()
{
    ++g_RefreshCalls;
    g_RefreshThis = this;
}

void CFrontendModePointer_00495fe0::Assign(
    const CFrontendModePointer_00495fe0* value)
{
    ++g_AssignCalls;
    g_AssignThis = this;
    g_AssignValue = value;
}

static void ResetCalls()
{
    g_PrepareCalls = 0;
    g_RefreshCalls = 0;
    g_AssignCalls = 0;
    g_PrepareThis = 0;
    g_RefreshThis = 0;
    g_AssignThis = 0;
    g_AssignValue = 0;
}

static bool CheckMode(long mode)
{
    CFrontendGameComponent_00495fe0 component;
    unsigned char expected[sizeof(component)];
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    std::memset(bytes, 0x5a, sizeof(component));
    std::memset(expected, 0x5a, sizeof(expected));

    *reinterpret_cast<long*>(expected + 0x1c) = mode;
    std::memset(expected + 0x168, 0, 32 * sizeof(long));
    *reinterpret_cast<long*>(expected + 0x1e8) = 0;
    *reinterpret_cast<long*>(expected + 0x1e44) = 1;
    if (mode == 11)
        *reinterpret_cast<long*>(expected + 0x1e28) = 0;
    if (mode == 12)
        *reinterpret_cast<long*>(expected + 0x1e18) = 0;

    ResetCalls();
    component.SetMode(mode);

    if (std::memcmp(bytes, expected, sizeof(component)) != 0)
        return false;

    if (mode == 10)
        return g_PrepareCalls == 1 && g_PrepareThis == &component &&
               g_RefreshCalls == 0 && g_AssignCalls == 0;

    if (mode == 11)
        return g_PrepareCalls == 0 && g_RefreshCalls == 1 &&
               g_RefreshThis == bytes + 0x1e1c && g_AssignCalls == 1 &&
               g_AssignThis == bytes + 0x1e40 &&
               g_AssignValue == &g_EmptyFrontendModePointer_00495fe0;

    if (mode == 12)
        return g_PrepareCalls == 0 && g_RefreshCalls == 1 &&
               g_RefreshThis == bytes + 0x1e0c && g_AssignCalls == 0;

    return g_PrepareCalls == 0 && g_RefreshCalls == 0 && g_AssignCalls == 0;
}

int main()
{
    if (!CheckMode(9) || !CheckMode(10) || !CheckMode(11) || !CheckMode(12) ||
        !CheckMode(13))
    {
        std::printf("CFrontendGameComponent_SetMode_00495fe0_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendGameComponent_SetMode_00495fe0_test PASS\n");
    return 0;
}
