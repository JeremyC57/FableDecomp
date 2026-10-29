// Behaviour test for 0x00853190 (CScriptModuleOpenGate constructor): the self-terminating base is set up for
// `duration` frames and PGate points at the gate.
#include <cstdio>

static long g_frame = 500;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
long CWorld::GetFrame() { return g_frame; }

class CThing { public: int dummy; };

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    const void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void SetPItem(const void* item) { m_pItem = item; }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem(thing); }
};

class C3DVector { public: float X, Y, Z; };

struct CScriptModuleBase {
    virtual void v00() { }
    virtual ~CScriptModuleBase() { }
    unsigned char m_state[0x48];
    CScriptModuleBase() { }
};

struct CScriptModuleSelfTerminateBase : CScriptModuleBase {
    long WFDuration;
    long EndFrame;
    CScriptModuleSelfTerminateBase(long duration)
        : WFDuration(duration)
    {
        EndFrame = CWorld::GetFrame() + WFDuration;
    }
};

struct CScriptModuleOpenGate : CScriptModuleSelfTerminateBase {
    CIntelligentPointer_CThingConst PGate;
    C3DVector                       Pos;
    C3DVector                       DeltaPos;
    CScriptModuleOpenGate(const CThing* gate, long duration);
};

CScriptModuleOpenGate::CScriptModuleOpenGate(const CThing* gate, long duration)
    : CScriptModuleSelfTerminateBase(duration), PGate(gate)
{
}

int main()
{
    CThing gate;
    CScriptModuleOpenGate m(&gate, 45);
    if (m.PGate.m_pItem != &gate || m.WFDuration != 45 || m.EndFrame != 545) {
        std::printf("00853190_TEST FAIL\n");
        return 1;
    }
    std::printf("00853190_TEST PASS\n");
    return 0;
}