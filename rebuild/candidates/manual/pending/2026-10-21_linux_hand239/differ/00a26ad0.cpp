// C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2*) -- make sure the shared animation is built, then create and initialise a per-mesh instance (freeing bank memory and retrying when it fails)
// Grade: DIFFER(383v383) -- register allocation only: `sete cl` vs al for the compressed flag, the bank pointer in edx vs ecx for both MemoryPool loads, and the first InitialiseAnimationInstance call loads its arguments in another order (the rotated retry copy matches)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26AD0, length 381 bytes (`ret 8`: hidden result + mesh).
// Callees: C3DAnimationInfo::BeginLoading(bool) @ 0x00A26730, the anim data's resource touch @ 0x009FC440 (manifest
//          "CTCActivationTrigger::OnKill" is propagated) and BuildSharedAnimation @ 0x00A24D70, CBankFile::GetEntryDataType
//          @ 0x009CC210, operator new @ 0x00BFEA1A, C3DAnimation2::C3DAnimation2 @ 0x00A4EE10 (landed),
//          CCountedPointer<C3DAnimation2>::Reset @ 0x00A29C50, CObservable::AddExclusiveObserver @ 0x00A4BE00,
//          C3DAnimation2::InitialiseAnimationInstance @ 0x00A50C20, the resource list's MakeFreeSpace(0) @ 0x009FC210 and
//          CHandleBasedMemoryPool::Defragment @ 0x00A02730 (the two retry remedies), the instance map's operator[]
//          @ 0x00A2D810, operator delete @ 0x00BFE9BC.
// Shape: the two "shared data present" tests re-test the data pointer (inline helpers); the compressed flag is
//          `(type & 0x80) == 0x80` kept in a frame byte and pushed as a dword; the retry loop is a two-case switch
//          on the attempt counter with `default` leaving the loop; the instance is stored in the map with the
//          inline CCountedPointer assignment (the mesh-ID key lives in the dead mesh argument slot) and returned
//          through the copy ctor, whose add-ref VC7.1 merges with the local's release.
// Layout: engine/C3DAnimationInfo.h (+0x0C Data_Object); the bank (+0x04) and entry index (+0x08) are unnamed in
//          the PDB header and read through accessors. CAnimationData: Instances map +0x1C, SharedAnimation +0x28,
//          compressed source +0x30. C3DMesh2::MeshID at retail +0xE8 (PDB +0xF0 minus 8, see 0x00A8B820).
#include "engine/C3DAnimationInfo.h"

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedPointer() { Release(); }
    void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            Release();
            m_pObject = other.m_pObject;
            m_pCount = other.m_pCount;
            if (m_pCount)
                ++m_pCount->m_references;
        }
        return *this;
    }
    void Reset(T* object);                  // 0x00A29C50 (stand-in body below)
    T* operator->() const { return m_pObject; }
};

SCountBlock* NewAnimationCountBlock(void* object);   // count block factory (stand-in helper)
#pragma auto_inline(off)
// Same-TU stand-in for CCountedPointer<C3DAnimation2>::Reset (0x00A29C50, retail's LTCG saw its body): releases the old
// object and wraps the new one; `this` does not escape, so the caller keeps the pointer words in registers.
template <class T> __declspec(noinline) void CCountedPointer<T>::Reset(T* object)
{
    Release();
    m_pObject = object;
    m_pCount = object ? NewAnimationCountBlock(object) : 0;
}
#pragma auto_inline(on)

struct C3DMesh2 {
    unsigned char _00[0xe8];
    unsigned long MeshID;                   // +0xe8
    unsigned long GetMeshID() const { return MeshID; }
};
struct CObserver;
struct C3DAnimation2 {
    C3DAnimation2();                        // 0x00A4EE10
    unsigned char _body[0x7c];
    void AddExclusiveObserver(CObserver* observer);   // 0x00A4BE00 (CObservable base)
    bool InitialiseAnimationInstance(C3DAnimation2* shared, const C3DMesh2* mesh, bool compressed, float* duration);   // 0x00A50C20
};
struct CInstanceMap {                       // map<unsigned long, CCountedPointer<C3DAnimation2> >
    unsigned char _body[0x0c];
    CCountedPointer<C3DAnimation2>& operator[](const unsigned long& key);   // 0x00A2D810
};
struct CAnimationData {
    unsigned char _00[0x1c];
    CInstanceMap Instances;                 // +0x1c
    C3DAnimation2* SharedAnimation;         // +0x28
    unsigned char _2c[0x04];
    void* CompressedSource;                 // +0x30
    void Touch();                           // 0x009FC440
    void BuildSharedAnimation();            // 0x00A24D70
};
struct CResourceList { void MakeFreeSpace(unsigned long bytes); };   // 0x009FC210
struct CHandleBasedMemoryPool { void Defragment(); };                // 0x00A02730
struct CAnimationBank {                     // CBankFile-derived owner
    unsigned long GetEntryDataType(unsigned long entry);             // 0x009CC210
    unsigned char _00[0x26c];
    CResourceList ResourceList;             // +0x26c
    unsigned char _26d[0x360 - 0x26d];
    CHandleBasedMemoryPool* MemoryPool;     // +0x360
};

struct C3DAnimationInfoMethods : C3DAnimationInfo {
    CAnimationBank* Bank() const { return *(CAnimationBank* const*)_pad_0x04; }
    unsigned long EntryIndex() const { return *(const unsigned long*)_pad_0x08; }
    CAnimationData* Data() const { return (CAnimationData*)Data_Object; }
    bool HasSharedAnimation() const { return Data() && Data()->SharedAnimation; }
    bool HasCompressedSource() const { return Data() && Data()->CompressedSource; }
    void BeginLoading(bool immediate);      // 0x00A26730
    CCountedPointer<C3DAnimation2> BuildAnimationInstance(const C3DMesh2* mesh);
};

CCountedPointer<C3DAnimation2> C3DAnimationInfoMethods::BuildAnimationInstance(const C3DMesh2* mesh)
{
    if (!HasSharedAnimation()) {
        if (!HasCompressedSource())
            BeginLoading(true);
        Data()->Touch();
        Data()->BuildSharedAnimation();
    }
    bool compressed = (Bank()->GetEntryDataType(EntryIndex()) & 0x80) == 0x80;
    float duration;
    CCountedPointer<C3DAnimation2> animation;
    animation.Reset(new C3DAnimation2);
    C3DAnimation2* instance = animation.m_pObject;
    instance->AddExclusiveObserver((CObserver*)Bank()->MemoryPool);
    int attempt = 0;
    while (!instance->InitialiseAnimationInstance(Data()->SharedAnimation, mesh, compressed, &duration)) {
        switch (attempt) {
        case 0:
            Bank()->ResourceList.MakeFreeSpace(0);
            break;
        case 1:
            Bank()->MemoryPool->Defragment();
            break;
        default:
            goto done;
        }
        ++attempt;
    }
done:
    Data()->Instances[mesh->GetMeshID()] = animation;
    return animation;
}
