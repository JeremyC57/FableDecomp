// CStreamBuffer::Attach(CPanel* panel, CSoundSample* sample, long handle, long users) -- take a reference on the sample, copy its format (minus the header), record the pool handle and user count, queue the buffer as loading and lock its pool block
// Leaf: Attach
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C262E0, 162 bytes (`ret 10h`). Descriptive names (no PDB covers the libpk
//   audio library): the manifest's "Thiscall_ParticleTrailSetup" is propagated. Callers: 0x00C218DB (right after
//   `operator new(0x2c)` + the buffer constructor 0x00C262A0) and 0x00C26940.
// Callees: CSoundSample::AddRef @ 0x00C1EA30, CSoundSample::GetDataSize @ 0x00C1E9C0, the sample's virtual slots 5
//   (header size, twice) and 4 (looping flag), the panel's loading queue (+0x314) Add (slot 9),
//   CHandleBasedMemoryPool::Lock @ 0x00C283C0 on the panel's pool (+0x360).
// Shape: the format is a POD struct assignment (five dword moves through ebx, the destination pointer kept in edi
//   for the later `m_format.m_dataSize -=`); the data size is assigned and then incremented in a second statement.

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
    void Attach(CPanel* panel, CSoundSample* sample, long handle, long users);
};

void CStreamBuffer::Attach(CPanel* panel, CSoundSample* sample, long handle, long users)
{
    m_state = 1;
    m_pSample = sample;
    sample->AddRef();
    m_format = m_pSample->m_format;
    m_dataSize = m_pSample->GetDataSize();
    m_dataSize += m_pSample->GetHeaderSize();
    m_format.m_dataSize -= m_pSample->GetHeaderSize();
    m_handle = handle;
    m_lastUse = 0;
    m_users = users;
    m_bLooping = m_pSample->IsLooping();
    panel->m_loadingBuffers.Add(this);
    panel->m_pool.Lock(m_handle);
}