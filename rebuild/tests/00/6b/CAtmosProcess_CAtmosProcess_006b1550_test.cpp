// Behaviour test for CAtmosProcess::~CAtmosProcess (0x006B1550): with a sound manager the atmos sounds and then the atmos map
// are cleared before the rain-player pointer is destroyed; without one only the member destructor runs.
#include <cstdio>
static int order[8]; static int n = 0;
class CASoundManager {
public:
    virtual ~CASoundManager() { }
    virtual void ClearAtmosSounds() { order[n++] = 1; }
    virtual void ClearAtmosMap() { order[n++] = 2; }
};
CASoundManager* PSoundManager = 0;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() { order[n++] = 3; }
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThingMarker;
class CAtmosProcess {
public:
    ~CAtmosProcess();
    long DayNightState;
    bool RainCurrentlyPlaying, AtmosSoundsMuted;
    CIntelligentPointer<CThingMarker> PRainPlayer;
    long InsideBuilding;
};
CAtmosProcess::~CAtmosProcess()
{
    if (PSoundManager) {
        PSoundManager->ClearAtmosSounds();
        PSoundManager->ClearAtmosMap();
    }
}
int main()
{
    { CAtmosProcess a; }
    if (n != 1 || order[0] != 3) { std::printf("006b1550_TEST FAIL\n"); return 1; }
    n = 0;
    CASoundManager sm; PSoundManager = &sm;
    CAtmosProcess* p = new CAtmosProcess;
    delete p;
    if (n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) { std::printf("006b1550_TEST FAIL\n"); return 1; }
    std::printf("006b1550_TEST PASS\n");
    return 0;
}