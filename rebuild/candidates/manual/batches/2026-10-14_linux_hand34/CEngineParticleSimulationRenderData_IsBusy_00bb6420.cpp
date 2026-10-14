// CEngineParticleSimulationRenderData::IsBusy() -- true when either of the two 0x34-byte simulation slots at +4 holds a buffer (+4 of the slot) that is still in use
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB6420, length 44 bytes.
// Callees: the slot's IsInUse @ 0x00AC20C0 (on the slot; the manifest label "CThingFilter_IsValid::operator()" is propagated). Shape: the index runs in edi
// (`inc ; cmp edi,2 ; jl`), the slot address in esi (`add esi,34h`); a true result leaves the loop with `mov al,1`, the fall-through returns `xor al,al`.
struct SSimulationSlot_00bb6420 {                 // 0x34 bytes
    long          m_a;
    void*         m_pBuffer;                       // +0x04
    unsigned char _pad[0x2c];
    bool IsInUse();                                // 0x00AC20C0
};

struct CEngineParticleSimulationRenderData {
    unsigned char            _pad[4];
    SSimulationSlot_00bb6420 m_slots[2];           // +0x04
    bool IsBusy();
};

bool CEngineParticleSimulationRenderData::IsBusy()
{
    for (long i = 0; i < 2; ++i) {
        if (m_slots[i].m_pBuffer && m_slots[i].IsInUse())
            return true;
    }
    return false;
}
