#include <cstdio>
struct CThing { int id; };
static int g_globalAdds = 0;
struct CTCBase {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase() { }
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};
struct CIntelligentPointer {
    void* PData;
    CIntelligentPointer() : PData(0) { }
    virtual ~CIntelligentPointer() { }
};
struct CWideString { const unsigned short* m_pData; CWideString() : m_pData(0) { } };
struct CTCRegionDisplay : CTCBase {
    CWideString CurrentAreaName, CurrentAreaNameExtra;
    long FrameNameChanged, CurrentAlpha, LastAlpha; float DisplayTime; bool TimeOut, InfoDisplay, UpdateWhilePaused;
    CTCRegionDisplay(CThing* thing) : CTCBase(thing), FrameNameChanged(0), CurrentAlpha(0), LastAlpha(0), DisplayTime(0),
        TimeOut(true), InfoDisplay(false), UpdateWhilePaused(true) { AddToGlobalUpdateTCs(); }
};
int main()
{
    CThing t; t.id = 4;
    CTCRegionDisplay r(&t);
    if (r.ParentThing != &t || r.CurrentAreaName.m_pData || r.CurrentAreaNameExtra.m_pData || r.FrameNameChanged || r.CurrentAlpha ||
        r.LastAlpha || r.DisplayTime != 0.0f || !r.TimeOut || r.InfoDisplay || !r.UpdateWhilePaused || !r.IsInGlobalUpdateTCs) { std::printf("006398b0_TEST FAIL\n"); return 1; }
    std::printf("006398b0_TEST PASS\n");
    return 0;
}
