// CStreamLoadHandler::Process(CPooledResource* request) -- the libpk stream worker's request handler (slot 0 of vtable 0x012B4018):
// under the audio lock, release the request's pool block; for a live, non-empty request copy its 0x24-byte sample header out of
// the pool, find (or create from the header's format, attaching a new stream buffer that finishes loading) the bank sample, then
// find (or create from the header's play parameters) its driver and start it on the owner; finally release and delete the request
// Flags: /EHsc
// Leaf: ?Process@CStreamLoadHandler@@UAEXPAVCPooledResource@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C267E0, 577 bytes, `ret 4`, EH-framed (the scoped lock; state 1 around `new CStreamBuffer`).
// Identity (descriptive: libpk / GameCODA has no PDB or RTTI): vtable 0x012B4018 has this one slot; the landed constructor
//   0x00C26A30 builds the handler at +4 of its owner (vtable 0x012B4018, owner pointer +4) and the worker over it at +0xC; the landed
//   PKCWorkerThread::ProcessNextRequest 0x00C26B20 calls the handler's slot 0 with each popped request. The catalog's
//   "UIManager_UpdateHeroEquipment" is a propagated label; the row was grouped with CHandleBasedMemoryPool only because it calls the
//   pool's Lock / Unlock / GetPointerFromHandle. Twin: 0x00C26DE0 (slot 0 of vtable 0x012B402C, the stream worker's handler).
// Callees: PKCProtectionInstance 0x00C0FFC0 / ~PKCProtectionInstance 0x00C0FFE0 over the lock 0x0143BB14;
//   CHandleBasedMemoryPool_012b4098::Unlock 0x00C283E0 / Lock 0x00C283C0 / GetPointerFromHandle 0x00C28380 (owner +0x360);
//   CPooledResource::IsActive 0x00C265A0 / Release 0x00C265B0 / ~CPooledResource 0x00C26550; CSampleHeader::GetFormat 0x00C356F0 /
//   FillPlayParameters 0x00C35770; the bank's sample and driver caches (virtual slot 17 at +0x38 / +0x50), CreateSample 0x00C0E7E0,
//   CreateDriver 0x00C0E870; operator new 0x00BFEA1A, CStreamBuffer() 0x00C262A0, Attach 0x00C262E0, FinishLoading 0x00C26090,
//   Release 0x00C26390, ~CStreamBuffer 0x00C26400, operator delete 0x00BFE9BC; CSoundPlayParameters() 0x00C1BD10; the owner's
//   StartSound 0x00C12CD0.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;                  // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    void*         m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);    // 0x00C0FFC0
    ~PKCProtectionInstance();                               // 0x00C0FFE0
};

class CHandleBasedMemoryPool_012b4098 { public:
    void* GetPointerFromHandle(long handle);                // 0x00C28380
    void Lock(long handle);                                 // 0x00C283C0
    void Unlock(long handle);                               // 0x00C283E0
};

struct SSampleFormat {                                      // 0x14-byte sample format record
    long           m_dataSize;                              // +0x00
    long           m_04;                                    // +0x04
    long           m_08;                                    // +0x08
    unsigned short m_channels;                              // +0x0c
    unsigned short m_0e;                                    // +0x0e
    unsigned long  m_sampleRate;                            // +0x10
    SSampleFormat() : m_dataSize(0), m_04(0), m_08(0), m_channels(1), m_0e(0), m_sampleRate(44100) { }
};

class CSoundPlayParameters { public:
    unsigned char _body[0x14];
    CSoundPlayParameters();                                 // 0x00C1BD10
};

class CSampleHeader { public:                               // the 0x24-byte header at the front of a pooled sample block
    unsigned char _body[0x24];
    long GetFormat(SSampleFormat* format, unsigned long size);   // 0x00C356F0
    long FillPlayParameters(CSoundPlayParameters* params);       // 0x00C35770
};

class CSoundSample;
class CSoundDriver;

class CSampleCache { public:                                // a polymorphic lookup embedded in the bank
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void* Find(const void* key);                    // slot 17 (+0x44)
    unsigned char _pad04[0x14];
};

class CStreamSoundBank { public:
    unsigned char _pad00[0x38];
    CSampleCache  m_samples;                                // +0x38
    CSampleCache  m_drivers;                                // +0x50
    CSoundSample* CreateSample(SSampleFormat* format, long key, long headerSize, long flags);   // 0x00C0E7E0
    CSoundDriver* CreateDriver(CSoundSample* sample, const CSoundPlayParameters* params);      // 0x00C0E870
};

class CPooledResourceManager;

class CPooledResource { public:
    void*             __vftable;                            // +0x00
    long              m_handle;                             // +0x04 pool handle
    long              m_08;                                 // +0x08
    long              m_0c;                                 // +0x0c
    unsigned long     m_size;                               // +0x10
    long              m_offset;                             // +0x14
    long              m_18;                                 // +0x18
    long              m_key;                                // +0x1c
    CStreamSoundBank* m_pBank;                              // +0x20
    long              m_24;                                 // +0x24
    unsigned char     _pad28[0x74 - 0x28];
    short             m_state;                              // +0x74
    short             m_flag;                               // +0x76 (set by PostRequest 0x00C26A60)
    long IsActive();                                        // 0x00C265A0
    void Release(CPooledResourceManager* manager);          // 0x00C265B0
    ~CPooledResource();                                     // 0x00C26550
};

class CStreamBuffer { public:                               // 0x2C bytes
    unsigned char _body[0x2c];
    CStreamBuffer();                                        // 0x00C262A0
    ~CStreamBuffer();                                       // 0x00C26400
    void Attach(CPooledResourceManager* manager, CSoundSample* sample, long handle, long users);   // 0x00C262E0
    long FinishLoading(CPooledResourceManager* manager, long flags);                               // 0x00C26090
    void Release(CPooledResourceManager* manager);                                                 // 0x00C26390
};

class CPooledResourceManager { public:
    unsigned char _pad00[0x360];
    CHandleBasedMemoryPool_012b4098 m_pool;                 // +0x360
    void StartSound(CSoundDriver* driver, long flags, long* a, long* b);   // 0x00C12CD0
};

void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A

class CStreamLoadHandler { public:                          // vtable 0x012B4018
    virtual void Process(CPooledResource* request);         // slot 0
    CPooledResourceManager* m_pOwner;                       // +0x04
};

void CStreamLoadHandler::Process(CPooledResource* request)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_pOwner->m_pool.Unlock(request->m_handle);
    if (request->m_flag != 0 && request->IsActive() && request->m_size > 0x24) {
        m_pOwner->m_pool.Lock(request->m_handle);
        char* block = (char*)m_pOwner->m_pool.GetPointerFromHandle(request->m_handle);
        CSampleHeader header = *(const CSampleHeader*)(block + request->m_offset);
        m_pOwner->m_pool.Unlock(request->m_handle);
        CSoundSample* sample = (CSoundSample*)request->m_pBank->m_samples.Find(&request->m_key);
        if (sample == 0) {
            SSampleFormat format;
            if (header.GetFormat(&format, request->m_size) >= 0) {
                sample = request->m_pBank->CreateSample(&format, request->m_key, 0x24, 1);
                CStreamBuffer* buffer = new CStreamBuffer;
                buffer->Attach(m_pOwner, sample, request->m_handle, request->m_0c);
                request->m_handle = -1;
                if (buffer->FinishLoading(m_pOwner, 1) < 0) {
                    buffer->Release(m_pOwner);
                    delete buffer;
                    sample = 0;
                }
            }
        }
        if (sample) {
            CSoundDriver* driver = (CSoundDriver*)request->m_pBank->m_drivers.Find(&sample);
            if (driver == 0) {
                CSoundPlayParameters params;
                if (header.FillPlayParameters(&params) >= 0)
                    driver = request->m_pBank->CreateDriver(sample, &params);
            }
            if (driver)
                m_pOwner->StartSound(driver, 0, &request->m_24, &request->m_08);
        }
    }
    request->Release(m_pOwner);
    delete request;
}
