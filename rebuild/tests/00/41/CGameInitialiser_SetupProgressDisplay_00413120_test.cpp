#include <cstdio>

// Model of 0x00413120 CGameInitialiser::SetupProgressDisplay: if CInitBaseClass::Init succeeds, clear the two start-up
// fields, wrap a new CProgressDisplay in a temporary counted pointer (ctor 0x00403D20: Info {RefCount 1, DeleteFunc,
// Data}), publish it with SetProgressDisplay (which keeps its own reference) and release the temporary; true.
static int g_displaysAlive = 0;
struct CProgressDisplay {
    int Marker;
    CProgressDisplay() : Marker(0x88) { ++g_displaysAlive; }
    ~CProgressDisplay() { --g_displaysAlive; }
};
struct CCPPointerInfo {
    unsigned long RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteProgressDisplay(void* data) { delete static_cast<CProgressDisplay*>(data); }
struct CCountedProgressDisplayPointer {
    CProgressDisplay* Data;
    CCPPointerInfo* Info;
    explicit CCountedProgressDisplayPointer(CProgressDisplay* object) : Data(object), Info(new CCPPointerInfo)
    {
        Info->RefCount = 1;
        Info->DeleteFunc = DeleteProgressDisplay;
        Info->Data = object;
    }
    ~CCountedProgressDisplayPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
static CProgressDisplay* g_published = 0;
static CCPPointerInfo* g_publishedInfo = 0;
static void SetProgressDisplay(const CCountedProgressDisplayPointer& display)
{
    g_published = display.Data;
    g_publishedInfo = display.Info;
    ++display.Info->RefCount;
}
struct CInitBaseClass {
    virtual ~CInitBaseClass() { }
    bool Valid;
    bool InitResult;
    bool Init() { return InitResult; }
};
struct CGameInitialiser : CInitBaseClass {
    unsigned long m_startupValue;
    unsigned char m_startupFlag;
    bool SetupProgressDisplay()
    {
        if (Init()) {
            m_startupFlag = 0;
            m_startupValue = 0;
            CCountedProgressDisplayPointer display(new CProgressDisplay);
            SetProgressDisplay(display);
            return true;
        }
        return false;
    }
};

static int Fail()
{
    std::printf("00413120_TEST FAIL\n");
    return 1;
}

int main()
{
    CGameInitialiser init;
    init.InitResult = false;
    init.m_startupValue = 5;
    init.m_startupFlag = 1;
    if (init.SetupProgressDisplay() || init.m_startupValue != 5 || init.m_startupFlag != 1 || g_published != 0)
        return Fail();
    init.InitResult = true;
    if (!init.SetupProgressDisplay() || init.m_startupValue != 0 || init.m_startupFlag != 0)
        return Fail();
    if (g_published == 0 || g_published->Marker != 0x88 || g_publishedInfo->RefCount != 1 || g_displaysAlive != 1)
        return Fail();
    CProgressDisplay* first = g_published;
    if (--g_publishedInfo->RefCount == 0) {        // drop the published reference: the display goes away
        g_publishedInfo->DeleteFunc(g_publishedInfo->Data);
        delete g_publishedInfo;
    }
    if (g_displaysAlive != 0 || first == 0)
        return Fail();
    std::printf("00413120_TEST PASS\n");
    return 0;
}