// CEngineInternalPrimitiveMeshBase::IsSimulationRunning(long index) -- true when the primitive has simulation info (+0x64) holding a simulation for `index` that reports itself running (virtual slot 7)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBBB0, length 42 bytes (48 with padding).
// Callees: CEnginePrimitiveSimulationInfo::FindSimulation(long) @ 0x00B812C0 (on the info pointer, loaded straight into ecx), the simulation's virtual slot 7 (+0x1c, bool).
// Shape: `this` is only read once (`mov ecx,[ecx+64h]`), so no register save; the three failures share the `xor al,al` tail, the success is `mov al,1`.
struct CEngineSimulationBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IsRunning();                      // slot 7 (+0x1c)
};

struct CEnginePrimitiveSimulationInfo {
    CEngineSimulationBase* FindSimulation(long index);   // 0x00B812C0
};

struct CEngineInternalPrimitiveMeshBase {          // the mesh base keeps its simulation info pointer at +0x64 (see RenderSubPrimitive @ 0x00BA6FA0, landed)
    unsigned char                   _pad[0x64];
    CEnginePrimitiveSimulationInfo* m_pSimulationInfo;   // +0x64
    bool IsSimulationRunning(long index);
};

bool CEngineInternalPrimitiveMeshBase::IsSimulationRunning(long index)
{
    if (m_pSimulationInfo) {
        CEngineSimulationBase* simulation = m_pSimulationInfo->FindSimulation(index);
        if (simulation && simulation->IsRunning())
            return true;
    }
    return false;
}