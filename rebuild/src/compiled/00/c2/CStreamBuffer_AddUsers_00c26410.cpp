// CStreamBuffer::AddUsers(CPanel* panel, long users) -- move the buffer to the panel's busy queue: off the busy queue (state 2) or off the idle queue with its pool block re-locked (state 3), add the users, queue it busy
// Leaf: AddUsers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C26410, 101 bytes (`ret 8`). Descriptive names (no PDB covers the libpk audio
//   library): the manifest's "CTCInventoryItem_SetEquipped" is propagated. Callers: the panel item state handlers
//   0x00C209A9, 0x00C20AB5, 0x00C20D90, 0x00C20DDA (`item->m_pBuffer->AddUsers(panel, item->m_users)`).
// Callees: the panel's busy (+0x2E4) / idle (+0x2B4) queues Remove (slot 8) / Add (slot 9),
//   CHandleBasedMemoryPool::Lock @ 0x00C283C0 on the panel's pool (+0x360).

struct SSampleFormat {                          // 0x14-byte sample format record (the buffer ctor 0x00C262A0 sets
    long           m_dataSize;                  // +0x00         1 channel / 44100 Hz)
    long           m_04;                        // +0x04
    long           m_08;                        // +0x08
    unsigned short m_channels;                  // +0x0c
    unsigned short m_0e;                        // +0x0e
    long           m_sampleRate;                // +0x10
};

struct CSoundSample {                           // the sample a stream buffer holds (refcount word +0x1E)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool IsLooping();                   // slot 4 (+0x10)
    virtual long GetHeaderSize();               // slot 5 (+0x14)
    void*          m_pData;                     // +0x04
    SSampleFormat  m_format;                    // +0x08
    unsigned short m_1c;                        // +0x1c
    unsigned short m_refCount;                  // +0x1e
    void AddRef();                              // 0x00C1EA30 (`inc word [ecx+1Eh]`)
    void Release();                             // 0x00C1EA40 (`dec word [ecx+1Eh]`)
    long GetDataSize();                         // 0x00C1E9C0
};

struct CStreamBuffer;

struct CStreamBufferQueue {                     // 0x30-byte polymorphic buffer queue of the panel
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CStreamBuffer* buffer); // slot 8 (+0x20)
    virtual void Add(CStreamBuffer* buffer);    // slot 9 (+0x24)
    virtual void v10();
    virtual CStreamBuffer* GetFirst();          // slot 11 (+0x2C)
    unsigned char _pad04[0x2c];
};

struct CHandleBasedMemoryPool {                 // this library's pool variant (Handles table +0x440)
    void Lock(long handle);                     // 0x00C283C0
    void Unlock(long handle);                   // 0x00C283E0
    void Free(long handle);                     // 0x00C28690
};

struct CPanel {
    unsigned char          _pad000[0x284];
    CStreamBufferQueue     m_cachedBuffers;     // +0x284 every cached (state 3) buffer
    CStreamBufferQueue     m_idleBuffers;       // +0x2B4 state 3: loaded, no users, unlocked
    CStreamBufferQueue     m_busyBuffers;       // +0x2E4 state 2: loaded, in use, locked
    CStreamBufferQueue     m_loadingBuffers;    // +0x314 state 1: being filled
    unsigned char          _pad344[0x35c - 0x344];
    long                   m_useStamp;          // +0x35C LRU stamp given to buffers going idle
    CHandleBasedMemoryPool m_pool;              // +0x360
};

struct CStreamBuffer {                          // 0x2C-byte stream buffer (operator new(0x2c) + ctor 0x00C262A0)
    CSoundSample*  m_pSample;                   // +0x00
    long           m_handle;                    // +0x04 pool handle, -1 when none
    long           m_dataSize;                  // +0x08
    long           m_lastUse;                   // +0x0c LRU stamp
    long           m_users;                     // +0x10
    unsigned char  m_state;                     // +0x14 0 free, 1 loading, 2 busy, 3 idle
    bool           m_bLooping;                  // +0x15
    unsigned char  m_16, m_17;                  // +0x16
    SSampleFormat  m_format;                    // +0x18
    void AddUsers(CPanel* panel, long users);
};

void CStreamBuffer::AddUsers(CPanel* panel, long users)
{
    if (m_state == 2)
        panel->m_busyBuffers.Remove(this);
    else if (m_state == 3) {
        panel->m_idleBuffers.Remove(this);
        panel->m_pool.Lock(m_handle);
    }
    m_users += users;
    panel->m_busyBuffers.Add(this);
    m_state = 2;
}