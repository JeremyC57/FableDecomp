// CStreamBuffer::FinishLoading(CSoundEventManager* manager, long success) -- end a buffer's load: unlock its pool block and drop its users; unwrap a compressed chunk in place; decode ADPCM data into a newly allocated PCM block that replaces the old one; then move the buffer from the loading queue to the idle and cached queues with a fresh LRU stamp (state 3); -1 on any failure, else 0
// Pool callees: the libpk/audio-library build of the pool is spelled CHandleBasedMemoryPool_012b4098 (its vtable; no RTTI, no PDB) so its Lock 0x00C283C0 / Unlock 0x00C283E0 / GetPointerFromHandle 0x00C28380 never share a symbol with the engine copy (0x00A026F0 / 0x00A02710 / 0x00A026B0, Ego_r names). Owner decision 2026-10-31.
// Leaf: ?FinishLoading@CStreamBuffer@@QAEJPAUCSoundEventManager@@J@Z
struct CSoundFormat {                                     // 20-byte format record
    unsigned long  Size;                                  // +0x00
    unsigned long  m_reserved1;                           // +0x04
    unsigned long  m_reserved2;                           // +0x08
    unsigned short Channels;                              // +0x0C
    unsigned short Encoding;                              // +0x0E
    unsigned long  SampleRate;                            // +0x10
    long CheckPlayable();                                 // 0x00C2D7B0
    unsigned long GetPCMBufferSize();                     // 0x00C2D920
};

struct CSoundSample;
struct CStreamBuffer;
struct CSoundEventManager;

class CHandleBasedMemoryPool_012b4098 { public:
    void* GetPointerFromHandle(long handle);              // 0x00C28380
    void Lock(long handle);                               // 0x00C283C0
    void Unlock(long handle);                             // 0x00C283E0
    void Free(long handle);                               // 0x00C28690
};

struct CSoundEventSet {
    long AllocateHandle(CSoundEventManager* manager, long size, long key);   // 0x00C22100
};

struct CStreamBufferQueue {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CStreamBuffer* buffer);           // slot 8 (+0x20)
    virtual void Add(CStreamBuffer* buffer);              // slot 9 (+0x24)
    unsigned char _pad04[0x2c];
};

struct CSoundEventManager {
    unsigned char          _pad000[0x2c];
    CSoundEventSet         m_eventSet;                    // +0x2C
    unsigned char          _pad02d[0x284 - 0x2d];
    CStreamBufferQueue     m_cachedBuffers;               // +0x284
    CStreamBufferQueue     m_idleBuffers;                 // +0x2B4
    CStreamBufferQueue     m_busyBuffers;                 // +0x2E4
    CStreamBufferQueue     m_loadingBuffers;              // +0x314
    unsigned char          _pad344[0x35c - 0x344];
    long                   m_useStamp;                    // +0x35C
    CHandleBasedMemoryPool_012b4098 m_pool;                        // +0x360
};

long __fastcall ReadChunkExtent_00c347b0(void* first, void* second, long* outStart, long* outEnd);   // 0x00C347B0
long __fastcall DecodeSoundData_00c34630(CSoundFormat* format, void* src, void* dst, unsigned long dstSize);   // 0x00C34630

struct CStreamBuffer {                                    // 0x2C bytes
    CSoundSample*  m_pSample;                             // +0x00
    long           m_handle;                              // +0x04
    long           m_dataSize;                            // +0x08
    long           m_lastUse;                             // +0x0C
    long           m_users;                               // +0x10
    unsigned char  m_state;                               // +0x14
    bool           m_bCompressed;                         // +0x15
    unsigned char  m_16, m_17;                            // +0x16
    CSoundFormat   m_format;                              // +0x18
    long FinishLoading(CSoundEventManager* manager, long success);
};

long CStreamBuffer::FinishLoading(CSoundEventManager* manager, long success)
{
    CHandleBasedMemoryPool_012b4098* pool = &manager->m_pool;
    pool->Unlock(m_handle);
    long users = m_users;
    m_users = 0;
    if (m_pSample == 0 || success == 0)
        return -1;
    if (m_bCompressed) {
        pool->Lock(m_handle);
        void* src = (char*)pool->GetPointerFromHandle(m_handle) + m_dataSize;
        long start;
        long end;
        long result = ReadChunkExtent_00c347b0(src, (void*)m_format.Size, &start, &end);
        pool->Unlock(m_handle);
        if (result < 0)
            return -1;
        m_dataSize += start;
        m_format.Size = end;
        m_bCompressed = false;
    }
    if (m_format.CheckPlayable() < 0) {
        unsigned long pcmSize = m_format.GetPCMBufferSize();
        long handle = manager->m_eventSet.AllocateHandle(manager, pcmSize, users);
        if (handle < 0)
            return -1;
        pool->Lock(m_handle);
        pool->Lock(handle);
        char* src = (char*)pool->GetPointerFromHandle(m_handle) + m_dataSize;
        void* dst = pool->GetPointerFromHandle(handle);
        long result = DecodeSoundData_00c34630(&m_format, src, dst, pcmSize);
        pool->Unlock(handle);
        pool->Unlock(m_handle);
        pool->Free(m_handle);
        m_handle = handle;
        m_dataSize = 0;
        if (result < 0)
            return -1;
    }
    manager->m_loadingBuffers.Remove(this);
    m_lastUse = manager->m_useStamp;
    manager->m_useStamp++;
    manager->m_idleBuffers.Add(this);
    manager->m_cachedBuffers.Add(this);
    m_state = 3;
    return 0;
}