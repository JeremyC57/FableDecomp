// CEngineResourceManager::Render(SRenderRequest* request) -- for the full-reload request (+4 == 0x40000000): flush the global graphics device's owner at +0x6c, then the graphic data bank at +0x954 reloads with 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B7C0, length 44 bytes.
// Callees: the global accessor @ 0x009A4EC0 (returns the device record @ 0x013CA618), the owner's flush @ 0x009F8200 (on the object at +0x6c of it), the bank's
// reload @ 0x009FD100 (on +0x954, argument 1). Shape: the request word is compared with the immediate before `this` is used; `ret 4`.
struct CDeviceOwner_009f8200 {
    void Flush();                                  // 0x009F8200
};

class CSystemManager { public:
    unsigned char         _pad[0x6c];
    CDeviceOwner_009f8200* m_pOwner;               // +0x6c
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0

class CGraphicDataBank { public:
    void Reload(long flags);                       // 0x009FD100
};

struct SRenderRequest_00b3b7c0 {
    long m_a;
    long m_kind;                                   // +0x04
};

class CEngineResourceManager { public:
    unsigned char     _pad[0x954];
    CGraphicDataBank* m_pDataBank;                 // +0x954
    void Render(SRenderRequest_00b3b7c0* request);
};

void CEngineResourceManager::Render(SRenderRequest_00b3b7c0* request)
{
    if (request->m_kind == 0x40000000) {
        GFGetSystemManager()->m_pOwner->Flush();
        m_pDataBank->Reload(1);
    }
}