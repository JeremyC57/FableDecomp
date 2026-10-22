// NParticleEngine::CParticleEmitterInfo::CEmitterData::UnloadResource() -- refuse while an asynchronous bank read is still pending; otherwise drop the resource's counted pointer (held through the pointer at +0x2c) and report true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1440, length 56 bytes.
// Callees: CBankFileAsyncData::IsFinished() @ 0x009D54E0 (on the async record at +0x1c), the counted object's virtual slot 1 (Destroy) when its count at +4
// reaches zero. Shape: the pending test returns the IsFinished result's zero byte straight (`pop esi ; ret`); the release is the landed counted-pointer
// Unload shape (test, `dec [ecx+4] ; jne`, `call [eax+4]`, null inside the branch, null again) applied through the holder pointer at +0x2c, then `mov al,1`.
// No PDB layout for CEmitterData; member names are descriptive.
struct CBankFileAsyncData {
    bool IsFinished() const;                       // 0x009D54E0
};

struct CResourceObject {                           // counted object: vptr, then the reference count
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CIVCountedPointer_Resource {
    CResourceObject* m_p;
    CIVCountedPointer_Resource() : m_p(0) { }
    ~CIVCountedPointer_Resource() { Release(); }
    void Release()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    CIVCountedPointer_Resource& operator=(const CIVCountedPointer_Resource& other)
    {
        Release();
        m_p = other.m_p;
        if (m_p)
            ++m_p->m_refCount;
        return *this;
    }
};

struct CEmitterData {
    unsigned char               _pad[0x1c];
    CBankFileAsyncData*         m_pAsyncData;      // +0x1c
    unsigned char               _pad20[0x0c];
    CIVCountedPointer_Resource* m_pResourceHolder; // +0x2c
    bool UnloadResource();
};

bool CEmitterData::UnloadResource()
{
    if (m_pAsyncData != 0) {
        if (!m_pAsyncData->IsFinished())
            return false;
    }
    *m_pResourceHolder = CIVCountedPointer_Resource();
    return true;
}