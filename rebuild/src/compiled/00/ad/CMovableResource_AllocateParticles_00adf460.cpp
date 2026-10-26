// CMovableResource::AllocateParticles(unsigned long, CMovableResourceMemoryPool*) -- allocate a movable block of count 16-byte particles from the pool and cache its pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADF460, 101 bytes (`ret 8`).
// Callees: CMovableResourceMemoryPool::AllocateResource(long*, CMovableResource*, ulong, ulong) @ 0x00A62E40,
//   CHandleBasedMemoryPool::GetPointerFromHandle(long) @ 0x00A026B0.
// Layout (retail): memory handle +0x138, pool +0x13c, particle array +0xc0 (the owning particle class derives from
//   CMovableResource; its own header is not generated, so a local descriptive model is used).
// Shape: the 64-bit size guard `mul edx ; test edx,edx ; ja ; cmp eax,-1 ; jbe` is an `unsigned __int64` product
//   compared against 0xFFFFFFFF; the pool argument stays in ecx for the allocate call (its store sinks above);
//   the handle member is read inline as the argument (a named local moves it into ebx).
class C4DVector { public:
    float X, Y, Z, W;
};

class CMovableResource { public:
    virtual void v00();
};

class CHandleBasedMemoryPool { public:
    void* GetPointerFromHandle(long handle);         // 0x00A026B0
};

class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { public:
    bool AllocateResource(long* handle, CMovableResource* owner, unsigned long size, unsigned long alignment);   // 0x00A62E40
};

struct CParticleArrayOwner : CMovableResource {
    unsigned char               _pad04[0xbc];
    C4DVector*                  m_pParticles;        // +0xc0
    unsigned char               _padC4[0x74];
    long                        m_memoryHandle;      // +0x138
    CMovableResourceMemoryPool* m_pMemoryPool;       // +0x13c
    bool AllocateParticles(unsigned long count, CMovableResourceMemoryPool* pool);
};

bool CParticleArrayOwner::AllocateParticles(unsigned long count, CMovableResourceMemoryPool* pool)
{
    m_pMemoryPool = pool;
    if ((unsigned __int64)count * sizeof(C4DVector) > 0xFFFFFFFF)
        return false;
    if (!pool->AllocateResource(&m_memoryHandle, this, count * sizeof(C4DVector), 0))
        return false;
    m_pParticles = (C4DVector*)m_pMemoryPool->GetPointerFromHandle(m_memoryHandle);
    return true;
}