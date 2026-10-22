// C3DClothPrimitive::LoadBinary(CMemoryDataInputStream&, CMovableResourceMemoryPool*) -- load the primitive and material indices, then the particle program from a pooled copy of its bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC3BD0, length 560 bytes (`ret 8`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CHandleBasedMemoryPool::Allocate @ 0x00A032F0,
//          CHandleBasedMemoryPool::GetPointerFromHandle @ 0x00A026B0, CHandleBasedMemoryPool::Free @ 0x00A02B30,
//          CDataInputStream::ReadData (vtable slot 4), CMemoryDataInputStream ctor @ 0x00994930 / dtor @ 0x00993B10,
//          operator new @ 0x00BFEA1A, CParticleProgram ctor @ 0x00AE3550 / ReadFromFile @ 0x00AE3C30,
//          CCountedPointer<CParticleProgram>::Reset @ 0x00AC3FF0, operator delete @ 0x00BFE9BC.
// Shape: the mirror of the landed SaveBinary 0x00AC3B00: PrimitiveIndex and MaterialIndex through inlined 4-byte reads,
// then the program byte count; a scoped pool handle {pool 0, handle -1} is allocated from the pool argument (false and
// the handle released on failure), the bytes are read into the locked block and wrapped in a memory stream; the program
// is reset to a new CParticleProgram and read from that stream with the global particle pool; on failure the counted
// pointer is released inline (count cleared before the object) and false returned. The stream and the handle die at
// every exit (handle dtor: free the handle when a pool is set).
// Layout: C3DClothPrimitive ParticleProgram CCountedPointer +4, PrimitiveIndex +0xc, MaterialIndex +0x10 (landed ctor
// 0x00AC3E00); retail CMemoryDataInputStream 0x1c; CScopedMemoryHandle {pool, handle}.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct SCountBlock { long references; void (__fastcall* destroy)(void*); void* object; };
template<class T> struct CCountedPointer {
    T* Object; SCountBlock* Count;
    void Release()
    {
        if (Count) { --Count->references; if (!Count->references) { Count->destroy(Count->object); delete Count; } }
        Count = 0;
        Object = 0;
    }
    void Reset(T* object);                         // 0x00AC3FF0
    T* operator->() const { return Object; }
};

struct CDataInputStream {
    virtual ~CDataInputStream();
    virtual void v1(); virtual void v2(); virtual void v3();
    virtual void ReadData(void* buffer, unsigned long size);   // slot 4 (+0x10)
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
};
struct CMemoryDataInputStream : CDataInputStream {
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    ~CMemoryDataInputStream();                     // 0x00993B10
    void* StreamSource;                            // +0x18 (retail 0x1c bytes)
};

struct CHandleBasedMemoryPool;
struct CScopedMemoryHandle {
    CHandleBasedMemoryPool* Pool;
    long Handle;
    CScopedMemoryHandle() : Pool(0), Handle(-1) {}
    ~CScopedMemoryHandle();
    void* GetPointer() const;
};
struct CHandleBasedMemoryPool {
    bool Allocate(CScopedMemoryHandle* handle, unsigned long size, unsigned long flags);   // 0x00A032F0
    void* GetPointerFromHandle(long handle);       // 0x00A026B0
    void Free(long handle);                        // 0x00A02B30
};
inline CScopedMemoryHandle::~CScopedMemoryHandle() { if (Pool) Pool->Free(Handle); }
inline void* CScopedMemoryHandle::GetPointer() const { return Pool->GetPointerFromHandle(Handle); }
struct CMovableResourceMemoryPool : CHandleBasedMemoryPool {};
extern CMovableResourceMemoryPool* g_ParticleMemoryPool;   // 0x013D2D70

struct CParticleProgram {
    CParticleProgram();                                            // 0x00AE3550
    bool ReadFromFile(CDataInputStream* in, CMovableResourceMemoryPool* pool);   // 0x00AE3C30
    unsigned char _body[0x140];
};

struct CBase { virtual ~CBase(); };
class C3DClothPrimitive : public CBase {
public:
    CCountedPointer<CParticleProgram> ParticleProgram;   // +0x04
    unsigned long PrimitiveIndex;                        // +0x0c
    unsigned long MaterialIndex;                         // +0x10
    bool LoadBinary(CMemoryDataInputStream& in, CMovableResourceMemoryPool* pool);
};

bool C3DClothPrimitive::LoadBinary(CMemoryDataInputStream& in, CMovableResourceMemoryPool* pool)
{
    PrimitiveIndex = in.ReadULong();
    MaterialIndex = in.ReadULong();
    unsigned long size = in.ReadULong();
    CScopedMemoryHandle handle;
    if (!pool->Allocate(&handle, size, 0))
        return false;
    void* data = handle.GetPointer();
    in.ReadData(data, size);
    CMemoryDataInputStream stream(data, size);
    ParticleProgram.Reset(new CParticleProgram);
    if (!ParticleProgram->ReadFromFile(&stream, g_ParticleMemoryPool)) {
        ParticleProgram.Release();
        return false;
    }
    return true;
}
