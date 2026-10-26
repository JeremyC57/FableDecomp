// CEngineResourceManager::FrameEnd() -- process loading until now plus the frame budget at +0x2aa0, release the three pools at +0x10..+0x18 (each when present), then the graphic data bank at +0x954 reloads with 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B870, length 73 bytes.
// Callees: GFGetTime @ 0x009E1BC0 (a double in st0), ProcessLoading @ 0x00B3B7F0 (on `this`: the flag 1 and the double), the pool release @ 0x00A23AD0 (the manifest
// label is a propagated _Dest_val symbol), the bank's Reload @ 0x009FD100 (3). Shape: the double is added straight from memory (`fadd qword ptr [edi+2AA0h]`)
// and stored into the reserved 8-byte argument slot; the three pointers are walked with a counted loop (`mov ebx,3 ; ... ; dec ebx ; jne`) through esi.
struct CResourcePool_00a23ad0 {
    void Release();                                // 0x00A23AD0
};

class CGraphicDataBank { public:
    void Reload(long flags);                       // 0x009FD100
};

double __cdecl GFGetTime();                        // 0x009E1BC0

class CEngineResourceManager { public:
    unsigned char           _pad0[0x10];
    CResourcePool_00a23ad0* m_pPools[3];           // +0x10 .. +0x1c
    unsigned char           _pad1c[0x938];
    CGraphicDataBank*       m_pDataBank;           // +0x954
    unsigned char           _pad958[0x2148];
    double                  m_loadBudget;          // +0x2aa0
    void ProcessLoading(long flag, double until);  // 0x00B3B7F0
    void FrameEnd();
};

void CEngineResourceManager::FrameEnd()
{
    ProcessLoading(1, GFGetTime() + m_loadBudget);
    for (long i = 0; i < 3; ++i) {
        if (m_pPools[i])
            m_pPools[i]->Release();
    }
    m_pDataBank->Reload(3);
}