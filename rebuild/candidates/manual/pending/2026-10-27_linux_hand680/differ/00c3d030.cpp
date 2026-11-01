// CCodaContext::GetResourceCounts(CRESOURCEDESC* pResourceDesc) -- fill the four CODA resource counts: with a SAL platform, the free count of each object list whose maximum is positive (else the maximum); without one, the four maxima
// Grade: DIFFER(98v94) -- only the out-pointer load placement differs: retail loads pResourceDesc into eax once before the m_pSal branch (`mov edx,[ecx+6Ch]` also ahead of the `test`), ours loads it at the top of each arm (+4 bytes). 5 normalised diff lines (ChatGPT baseline: 92v94, ~50 lines). next: the Fable 1.5 fork's real declaration (name/ABI) may change the parameter use; try a by-value CRESOURCEDESC return once the CODA 1.x header is found.
// Leaf: ?GetResourceCounts@CCodaContext@@QBEPAU_CRESOURCEDESC@@PAU2@@Z
// Retail: Fable.exe @ 0x00C3D030, 94 bytes (`ret 4`, both arms tail-duplicated). Reached only from the API thunk 0x00C333F0
//   (`mov ecx,0143BB58h ; jmp 00C3D030`, no catalog row). `this` = ?CODA@@3VCCodaContext@@A 0x0143BB58 (GameCODA SDK 2.10
//   ApiSource.obj binding, ChatGPT pass 10 SDK recovery; landed rows spell that global g_audioSystem / g_listenerStateFactory).
// Names: DESCRIPTIVE method name (GetResourceCounts) -- the query is absent from SDK 2.10's CCodaContext/CODA.h and from both
//   PDB name sets; returns the pointer because retail leaves pResourceDesc in eax at both `ret`s. Members are the SDK 2.10
//   CodeView names; the release layout drops the debug-only CCodaObject words (offsets are the debug ones minus 0xC: m_pSal
//   +4, CCodaObjectList m_2DSources +0x1C (m_nFree +0x28, stride 0x14), m_nMax2DSources +0x6C). CRESOURCEDESC is CODA.h's
//   struct (ProgrammingReference 2.10: n2DSources, n3DSources, nListeners, nEnvironments).
// Shape: `count = max; if (count > 0) count = list.GetNumFree(); desc->x = count;` per field, the last written as an
//   if/else of two stores (retail's duplicated `mov [eax+0Ch]` tails); count loaded before the m_pSal test.
typedef struct _CRESOURCEDESC {                         // CODA.h (SDK 2.10 ProgrammingReference: CODA_Create's parameter)
    int n2DSources;
    int n3DSources;
    int nListeners;
    int nEnvironments;
} CRESOURCEDESC;
class CSalPlatform;
class CCodaObject;
class CCodaObjectBase { public:                          // SDK 2.10 CodeView: polymorphic root (vptr +0)
    virtual ~CCodaObjectBase();
};
class CCodaObjectList : public CCodaObjectBase { public: // SDK 2.10 CodeView, 20 bytes
    int GetNumFree() const { return m_nFree; }
private:
    CCodaObject* m_pObjects;                             // +0x04
    CCodaObject* m_pFree;                                // +0x08
    int m_nFree;                                         // +0x0c
    unsigned int m_nObjects;                             // +0x10
};
class CCodaObject : public CCodaObjectBase { };          // release layout: vptr only (debug adds m_DebugTest/links)
class CCodaContext : public CCodaObject { public:
    CRESOURCEDESC* GetResourceCounts(CRESOURCEDESC* pResourceDesc) const;   // descriptive name: absent from SDK 2.10
private:
    CSalPlatform* m_pSal;                                // +0x04
    void* m_2DSourceMem;                                 // +0x08
    void* m_3DSourceMem;                                 // +0x0c
    void* m_ListenerMem;                                 // +0x10
    void* m_EnvironmentMem;                              // +0x14
    void* m_EAX;                                         // +0x18
    CCodaObjectList m_2DSources;                         // +0x1c (m_nFree +0x28)
    CCodaObjectList m_3DSources;                         // +0x30
    CCodaObjectList m_Listeners;                         // +0x44
    CCodaObjectList m_Environments;                      // +0x58
    int m_nMax2DSources;                                 // +0x6c
    int m_nMax3DSources;                                 // +0x70
    int m_nMaxListeners;                                 // +0x74
    int m_nMaxEnvironments;                              // +0x78
};
CRESOURCEDESC* CCodaContext::GetResourceCounts(CRESOURCEDESC* pResourceDesc) const
{
    int count = m_nMax2DSources;
    if (m_pSal) {
        if (count > 0) count = m_2DSources.GetNumFree();
        pResourceDesc->n2DSources = count;
        count = m_nMax3DSources;
        if (count > 0) count = m_3DSources.GetNumFree();
        pResourceDesc->n3DSources = count;
        count = m_nMaxListeners;
        if (count > 0) count = m_Listeners.GetNumFree();
        pResourceDesc->nListeners = count;
        count = m_nMaxEnvironments;
        if (count > 0) pResourceDesc->nEnvironments = m_Environments.GetNumFree();
        else pResourceDesc->nEnvironments = count;
    } else {
        pResourceDesc->n2DSources = count;
        pResourceDesc->n3DSources = m_nMax3DSources;
        pResourceDesc->nListeners = m_nMaxListeners;
        pResourceDesc->nEnvironments = m_nMaxEnvironments;
    }
    return pResourceDesc;
}
