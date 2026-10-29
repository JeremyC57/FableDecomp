// CEngineSimulationBase::CEngineSimulationBase() -- CBase, this class's vtable, a reference count of 1 at +4, and the simulation id at +8 taken from the physics settings' byte counter (+0x1c, post-incremented)
// Leaf: ??0CEngineSimulationBase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80900, length 44 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x012A38FC = CEngineSimulationBase (rtti-vtables.tsv). Globals: the physics settings @ 0x01436E68. Shape: the byte
// post-increment through al/dl (`mov al ; mov dl,al ; inc dl ; mov [counter],dl ; mov [esi+8],al`); `this` is returned.
struct SPhysicsSettings_01436e68 {
    unsigned char _pad[0x1c];
    unsigned char m_simulationCounter;             // +0x1c
    unsigned char NextSimulationId()               // inline member: the settings land in ecx as `this`, the old byte returns in al, the increment goes through dl
    {
        unsigned char id = m_simulationCounter;
        m_simulationCounter = id + 1;
        return id;
    }
};
extern SPhysicsSettings_01436e68* g_pPhysicsSettings_01436e68;   // 0x01436E68

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CEngineSimulationBase : public CBaseClassNonCopyable { public:             // vtable 0x012A38FC
    long          m_references;                    // +0x04
    unsigned char m_id;                            // +0x08
    CEngineSimulationBase();
};

CEngineSimulationBase::CEngineSimulationBase()
    : CBaseClassNonCopyable()
    , m_references(1)
    , m_id(g_pPhysicsSettings_01436e68->NextSimulationId())
{
}