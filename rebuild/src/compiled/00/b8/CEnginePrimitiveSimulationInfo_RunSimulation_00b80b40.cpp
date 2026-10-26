// CEnginePrimitiveSimulationInfo::RunSimulation() -- call virtual slot 6 on every non-null simulation of the 8-byte-entry vector at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80B40, length 40 bytes.
// Callees: virtual slot 6 (+0x18) of each entry's simulation. Shape: pointer iteration from +8 to +0xc with the end re-read after each call (`mov eax,[edi+0Ch]`),
// `test ecx,ecx ; je` around the call; the loop head is aligned with `lea esp,[esp]`.
struct CSimulation_00b80b40 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void Run();                            // slot 6 (+0x18)
};

struct SSimulationEntry { CSimulation_00b80b40* m_pSimulation; long m_tag; };   // 8 bytes

class CEnginePrimitiveSimulationInfo { public:
    unsigned char     _pad[8];
    SSimulationEntry* m_pFirst;                    // +0x08
    SSimulationEntry* m_pLast;                     // +0x0c
    void RunSimulation();
};

void CEnginePrimitiveSimulationInfo::RunSimulation()
{
    for (SSimulationEntry* entry = m_pFirst; entry != m_pLast; ++entry) {
        if (entry->m_pSimulation)
            entry->m_pSimulation->Run();
    }
}