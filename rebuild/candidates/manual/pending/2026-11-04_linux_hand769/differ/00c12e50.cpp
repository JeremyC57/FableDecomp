// CSoundEventManager::PlayStreamed(CStreamSoundBank* bank, long position, unsigned long size, const Input* settings) -- slot 14 of the manager vtable 0x012B2748: under the library lock, -1 without a bank stream; a sample already resident for the position (the bank's sample map, then its driver map) plays through the landed 0x00C12CD0 lookup with the loaded voice parameters and yields the event's id (-1 when none); otherwise allocate a pool block for the streamed range (block-aligned offset + size + tail, padding), lock it, build a pooled stream request (id from the manager's counter, offset, tail, handle, bank, size, key, padding, voice parameters), activate it and queue the block read on the bank stream; a refused read unlocks, releases and deletes the request (-1); else the request id
// Grade: DIFFER(741v741) -- one store scheduled early: retail stores the request's +0x14 block offset (edx, reloaded from its spill) after the +0x20 bank store; ours emits it before the +4 handle store. Every other byte matches (dl=2); 10 graded attempts incl. an exhaustive compile-only search of the eight field-store orders and 1532 random orders with the id store
// Leaf: ?PlayStreamed@CSoundEventManager@@UAEJPAVCStreamSoundBank@@JKPBUInput@@@Z
// Flags: /EHsc
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;                  // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    void*         m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);    // 0x00C0FFC0
    ~PKCProtectionInstance();                               // 0x00C0FFE0
};

struct Input;                                               // CPKVoiceParams::Load's settings record
struct CPKVoiceParams {                                     // 0x50 bytes
    unsigned char _body[0x50];
    CPKVoiceParams();                                       // 0x00C10A30
    void Load(const Input* settings);                       // 0x00C27C10
};

class CHandleBasedMemoryPool_012b4098 { public:
    void* GetPointerFromHandle(long handle);                // 0x00C28380
    void Lock(long handle);                                 // 0x00C283C0
    void Unlock(long handle);                               // 0x00C283E0
};

class CPooledResource;
class CCombatSequenceBase { public:                         // the bank's block stream (landed spelling of 0x00C1E970's argument)
    virtual void v00();
    virtual bool Read(unsigned long firstBlock, unsigned long blockCount, unsigned long size, void* buffer,
                      CPooledResource* request, long owner);   // slot 1
    virtual unsigned long GetBlockSize();                   // slot 2
};
unsigned long __fastcall CCombatSequenceBase_IsBoastSequence(CCombatSequenceBase* stream, unsigned long position);   // 0x00C1E970
long __fastcall CombatSequence_AlignDuration(CCombatSequenceBase* stream, long position, long size);                 // 0x00C1E990

class CSampleCache { public:                                // a polymorphic lookup embedded in the bank
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void* Find(const void* key);                    // slot 17 (+0x44)
    unsigned char _pad04[0x10];
};

class CStreamSoundBank { public:
    void*                m_00;
    CCombatSequenceBase* m_pStream;                         // +0x04
    unsigned char        _pad08[0x38 - 8];
    CSampleCache         m_samples;                         // +0x38
    long                 m_padding;                         // +0x4c
    CSampleCache         m_drivers;                         // +0x50
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
    CPKVoiceParams    m_params;                             // +0x24
    short             m_state;                              // +0x74
    short             m_flag;                               // +0x76
    CPooledResource();                                      // 0x00C266E0
    ~CPooledResource();                                     // 0x00C26550
    unsigned long GetFirstBlock();                          // 0x00C26500
    unsigned long GetBlockCount();                          // 0x00C26520
    void Activate(CPooledResourceManager* manager);         // 0x00C26560
    void Release(CPooledResourceManager* manager);          // 0x00C265B0
};

struct CSoundEventManager;
struct CSoundEventSet {
    long AllocateHandle(CSoundEventManager* manager, long size, long padding);   // 0x00C22100
};
struct CSoundEventEntry_00c13140 {
    unsigned char _pad[0xc];
    long m_handle;                                          // +0xc
};

void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct CSoundEventManager {
    unsigned char                   _pad04[0x2c - 4];
    CSoundEventSet                  m_events;               // +0x2c
    unsigned char                   _pad2d[0x360 - 0x2d];
    CHandleBasedMemoryPool_012b4098 m_pool;                 // +0x360
    unsigned char                   _pad361[0x7b8 - 0x361];
    long                            m_7b8;                  // +0x7b8
    unsigned char                   _pad7bc[0x88c - 0x7bc];
    long                            m_nextId;               // +0x88c
    CSoundEventEntry_00c13140* FindEntry_00c12cd0(long sound, long flags, const CPKVoiceParams* params, long mode);   // 0x00C12CD0
    virtual long PlayStreamed(CStreamSoundBank* bank, long position, unsigned long size, const Input* settings);
};

long CSoundEventManager::PlayStreamed(CStreamSoundBank* bank, long position, unsigned long size, const Input* settings)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    if (bank->m_pStream == 0)
        return -1;
    void* sample = bank->m_samples.Find(&position);
    if (sample) {
        void* driver = bank->m_drivers.Find(&sample);
        if (driver) {
            CPKVoiceParams params;
            params.Load(settings);
            CSoundEventEntry_00c13140* entry = FindEntry_00c12cd0((long)driver, 0, &params, 0);
            if (entry)
                return entry->m_handle;
            return -1;
        }
    }
    long padding = bank->m_padding;
    unsigned long offset = CCombatSequenceBase_IsBoastSequence(bank->m_pStream, position);
    long tail = CombatSequence_AlignDuration(bank->m_pStream, position, size);
    long handle = m_events.AllocateHandle(this, tail + offset + size, padding);
    if (handle < 0)
        return -1;
    m_pool.Lock(handle);
    CPooledResource* request = new CPooledResource;
    long id = m_nextId++;
    request->m_08 = id;
    request->m_offset = offset;
    request->m_18 = tail;
    request->m_handle = handle;
    request->m_flag = 0;
    request->m_pBank = bank;
    request->m_size = size;
    request->m_key = position;
    request->m_0c = padding;
    request->m_params.Load(settings);
    request->Activate((CPooledResourceManager*)this);
    if (!bank->m_pStream->Read(request->GetFirstBlock(), request->GetBlockCount(), offset + size,
                               m_pool.GetPointerFromHandle(handle), request, m_7b8)) {
        m_pool.Unlock(handle);
        request->Release((CPooledResourceManager*)this);
        delete request;
        return -1;
    }
    return id;
}
