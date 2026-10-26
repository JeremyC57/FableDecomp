// Behaviour test for CSystemManagerInit::CSystemManagerInit (0x00403B10): every default the retail constructor sets.
#include <cstdio>
#include <cstring>
#include <stddef.h>

static int g_wideCtors = 0, g_charCtors = 0, g_displayCtors = 0;

class CWideString { public:
    char m_text[32];
    CWideString() { m_text[0] = 0; ++g_wideCtors; }
    const CWideString& operator=(const wchar_t* text)
    {
        int i = 0;
        for (; text[i] && i < 31; ++i) m_text[i] = (char)text[i];
        m_text[i] = 0;
        return *this;
    }
};
class CCharString { public: void* m_pStringData; CCharString() : m_pStringData(0) { ++g_charCtors; } };
class CInputManagerInit { public:
    bool UseMouse, UseJoystick, UseKeyboard;
    CInputManagerInit() : UseMouse(false), UseJoystick(false), UseKeyboard(false) {}
};
class CDisplayManagerInit { public: long Width; CDisplayManagerInit() : Width(0) { ++g_displayCtors; } };
struct SStartupSizes {
    long m_secondarySize, m_primarySize; float m_scaleA, m_scaleB, m_scaleC; long m_reservedA, m_reservedB, m_bufferSize;
    SStartupSizes()
    {
        m_primarySize = 0x40; m_secondarySize = 0x80; m_scaleA = 64.0f; m_scaleB = 2.0f; m_scaleC = 1.0f;
        m_reservedB = 0; m_reservedA = 0; m_bufferSize = 0x40000;
    }
};
class CSystemManagerInit { public:
    void* InstanceHandle; char* CommandLine; long WindowsShow; CWideString AppName;
    bool ExclusiveMode, PreferPrimaryDisplayDevice; unsigned long Flags;
    bool PreventHardwareAcceleration, AttachToExternalWindow; void* ApplicationWindowHandle;
    bool PreventExclusiveModeChanges, UseAccurateTimer; void (*AppReinitFunc)(void*); void* AppReinitContext;
    CCharString FontBankHandle, StreamingFontBankHandle; bool WaitWhileInactive, SkipConfigDetection;
    SStartupSizes StartupSizes; CInputManagerInit InputManagerInit; CDisplayManagerInit DisplayManagerInit;
    bool UseTripleBuffer; bool (*IsErrorFailureFunc)(long); char Identifier[17]; long ScratchBufferSize;
    CWideString LogfileName, TempPath; long m_extraA, m_extraB;
    CSystemManagerInit()
    {
        InstanceHandle = 0; CommandLine = 0; ExclusiveMode = true; Flags = 0; IsErrorFailureFunc = 0;
        PreferPrimaryDisplayDevice = false;
        memset(Identifier, 0, 17); strncpy(Identifier, "BBB GUID", 8); Identifier[8] = 0;
        LogfileName = L"bbb.log";
        ScratchBufferSize = 0x20000; AttachToExternalWindow = false; PreventExclusiveModeChanges = false;
        ApplicationWindowHandle = 0; UseAccurateTimer = false; AppReinitFunc = 0; AppReinitContext = 0;
        TempPath = L"C:\\temp";
        m_extraA = 0; m_extraB = 0; WaitWhileInactive = true;
    }
};

int main()
{


    CSystemManagerInit* s = new CSystemManagerInit;
    int ok = 1;
    if (s->InstanceHandle != 0 || s->CommandLine != 0 || !s->ExclusiveMode || s->PreferPrimaryDisplayDevice || s->Flags != 0) ok = 0;
    if (s->AttachToExternalWindow || s->ApplicationWindowHandle != 0 || s->PreventExclusiveModeChanges || s->UseAccurateTimer) ok = 0;
    if (s->AppReinitFunc != 0 || s->AppReinitContext != 0 || !s->WaitWhileInactive || s->IsErrorFailureFunc != 0) ok = 0;
    if (s->StartupSizes.m_primarySize != 0x40 || s->StartupSizes.m_secondarySize != 0x80 || s->StartupSizes.m_scaleA != 64.0f) ok = 0;
    if (s->StartupSizes.m_scaleB != 2.0f || s->StartupSizes.m_scaleC != 1.0f || s->StartupSizes.m_reservedA != 0) ok = 0;
    if (s->StartupSizes.m_reservedB != 0 || s->StartupSizes.m_bufferSize != 0x40000) ok = 0;
    if (s->InputManagerInit.UseMouse || s->InputManagerInit.UseJoystick || s->InputManagerInit.UseKeyboard) ok = 0;
    if (strcmp(s->Identifier, "BBB GUID") != 0 || s->Identifier[16] != 0 || s->ScratchBufferSize != 0x20000) ok = 0;
    if (strcmp(s->LogfileName.m_text, "bbb.log") != 0 || strcmp(s->TempPath.m_text, "C:\\temp") != 0) ok = 0;
    if (s->m_extraA != 0 || s->m_extraB != 0) ok = 0;
    if (g_wideCtors != 3 || g_charCtors != 2 || g_displayCtors != 1) ok = 0;
    if (!ok) { std::printf("00403b10_TEST FAIL\n"); return 1; }
    std::printf("00403b10_TEST PASS\n");
    return 0;
}