#ifndef FABLE_CCODACONTEXT_H
#define FABLE_CCODACONTEXT_H
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
class CCoda2DSource;
class CCoda3DSource;
class CCodaListener;
class CCodaEnvironment;
class CCodaEAX;
class CCodaObject;
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
    unsigned char UnmodelledTail[0x2c]; // SDK release size 0xa8; no tail member used here.
};
#endif
