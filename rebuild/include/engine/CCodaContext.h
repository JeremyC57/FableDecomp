// Retail opt-in (Codex pass20): SDK CodeView and native00c3d470; PROCESS branch only, default preserved.
#ifndef FABLE_CCODACONTEXT_H
#define FABLE_CCODACONTEXT_H
#ifdef FABLE_RETAIL_CCODACONTEXT_SETERROR
#include "engine/CODA_PlatformTypes.h"
#endif
#include "engine/CCodaObjectBase.h"
// Layout: SDK 2.10 CodeView member names; native offsets are 12 bytes below the SDK debug donor.
// The SDK release also retains eight more base bytes than this native fork; see VENDOR_COMPILER_EVIDENCE.txt.
// Native 00c3d030 confirms all accessed offsets, and list stride 0x14.
typedef struct _CRESOURCEDESC {                         // CODA.h (SDK 2.10 ProgrammingReference: CODA_Create's parameter)
    int n2DSources;
    int n3DSources;
    int nListeners;
    int nEnvironments;
} CRESOURCEDESC;
class CSalPlatform;
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
class CCodaSource;
#endif
class CCoda2DSource;
class CCoda3DSource;
class CCodaListener;
class CCodaEnvironment;
class CCodaEAX;
class CCodaObject;
class CCodaObjectList : public CCodaObjectBase { public: // SDK 2.10 CodeView, 20 bytes
#ifdef FABLE_RETAIL_CCODACONTEXT_PROCESS
    CCodaObject* GetObjects()const{return m_pObjects;}
#endif
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
    CCodaObject* GetFree() const;
    void RemoveFree();
    void Add(CCodaObject*);
#endif
    int GetNumFree() const { return m_nFree; }
private:
    CCodaObject* m_pObjects;                             // +0x04
    CCodaObject* m_pFree;                                // +0x08
    int m_nFree;                                         // +0x0c
    unsigned int m_nObjects;                             // +0x10
};
#ifndef FABLE_RETAIL_CCODACONTEXT_SETERROR
class CCodaObject : public CCodaObjectBase { };
#endif // Original context base has no object-list links in native
//          // release layout: vptr only (debug adds m_DebugTest/links)
#ifdef FABLE_RETAIL_CCODACONTEXT_SETERROR
class CCodaContext : public CCodaObjectBase { public:
#else
class CCodaContext : public CCodaObject { public:
#endif
#ifdef FABLE_RETAIL_CCODACONTEXT_SETERROR
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
    void SetError(int error);
    CCoda3DSource* Create3DSource(unsigned int ffCaps);
    void ReleaseSource(CCodaSource*);
#else
    void SetError(int);
#endif
#ifdef FABLE_RETAIL_CCODACONTEXT_PROCESS
    void Process();
#endif
#endif
    CRESOURCEDESC* GetResourceCounts(CRESOURCEDESC* pResourceDesc) const;   // descriptive name: absent from SDK 2.10
private:
    CSalPlatform* m_pSal;                                // +0x04
    CCoda2DSource* m_2DSourceMem;                                 // +0x08
    CCoda3DSource* m_3DSourceMem;                                 // +0x0c
    CCodaListener* m_ListenerMem;                                 // +0x10
    CCodaEnvironment* m_EnvironmentMem;                              // +0x14
    CCodaEAX* m_EAX;                                         // +0x18
    CCodaObjectList m_2DSources;                         // +0x1c (m_nFree +0x28)
    CCodaObjectList m_3DSources;                         // +0x30
    CCodaObjectList m_Listeners;                         // +0x44
    CCodaObjectList m_Environments;                      // +0x58
    int m_nMax2DSources;                                 // +0x6c
    int m_nMax3DSources;                                 // +0x70
    int m_nMaxListeners;                                 // +0x74
    int m_nMaxEnvironments;                              // +0x78
#ifdef FABLE_RETAIL_CCODACONTEXT_SETERROR
    void (FABLE_STDCALL *m_pfnErrorCallback)(int );
    void (FABLE_STDCALL *m_pfnPlayCallback)(void * ,unsigned int ,unsigned int );
    unsigned int m_PlayCallbackData;
    unsigned int m_uiHFReference;
    float m_fZAxis;
    float m_fDistanceUnits;
    float m_fDopplerFactor;
    float m_fDopplerSpeed;
    float m_fMacroFxFactor;
    float m_fRolloffFactor;
    unsigned int m_ffUpdate;
#else
    unsigned char UnmodelledTail[0x2c];
#endif
// // SDK release size 0xa8; no tail member used here.
};
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE3D
// Retail opt-in (Codex pass20): SDK SetError reads the process-wide latch, then calls this context's callback.
extern int CODA_Error;
extern CCodaContext CODA;
inline void CCodaContext::SetError(int error) {
 if(CODA_Error==0) { CODA_Error=error; if(m_pfnErrorCallback)m_pfnErrorCallback(error); }
}
#endif
#endif
