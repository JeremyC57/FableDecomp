// Behaviour test for 0x00743FE0 implicit ~CModeIceTrollGroundSmashIcicleAssault: ViablePoints, Icicles (each icicle's PTrap then
// PEffect), PDef, Filter (Group string), PTarget, then ~CModeStandBase.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[64];
static void Log(const char* s) { std::strcat(g_log, s); }
class CIntelligentPointer { public: CIntelligentPointer(char c = 'p') : C(c) { } virtual ~CIntelligentPointer() { char b[2] = { C, 0 }; Log(b); } char C; };
class CAnimFilterBase { public: virtual ~CAnimFilterBase() { } };
class CAnimFilterOfGroup : public CAnimFilterBase { public: virtual ~CAnimFilterOfGroup() { Log("G"); } unsigned long GroupCRC; };
class CModeStandBase { public: virtual ~CModeStandBase() { Log("S"); } };
class C3DVector { public: float X, Y, Z; };
struct CViablePoints { std::vector<C3DVector> V; ~CViablePoints() { Log("V"); } };
class CIcicleData { public: CIcicleData() : PEffect('e'), PTrap('t') { } CIntelligentPointer PEffect; CIntelligentPointer PTrap; long EffectTimer, TrapTimer; C3DVector Position; };
class CModeIceTrollGroundSmashIcicleAssault : public CModeStandBase {
public:
    CModeIceTrollGroundSmashIcicleAssault() : PTarget('T') { }
    CIntelligentPointer PTarget;
    CAnimFilterOfGroup Filter;
    long Timer;
    const void* PDef;
    std::vector<CIcicleData> Icicles;
    CViablePoints ViablePoints;
};
int main()
{
    CModeIceTrollGroundSmashIcicleAssault* m = new CModeIceTrollGroundSmashIcicleAssault;
    m->Icicles.resize(1);
    C3DVector v = { 1.0f, 2.0f, 3.0f };
    m->ViablePoints.V.push_back(v);
    g_log[0] = 0;
    delete (CModeStandBase*)m;
    bool ok = std::strcmp(g_log, "VteGTS") == 0;
    std::printf(ok ? "00743fe0_TEST PASS\n" : "00743fe0_TEST FAIL\n");
    return ok ? 0 : 1;
}