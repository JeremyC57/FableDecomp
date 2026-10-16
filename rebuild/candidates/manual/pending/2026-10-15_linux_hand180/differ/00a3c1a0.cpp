// Grade: DIFFER(182v189) -- retail re-reads Data and re-tests it for null before the shared-count compare (mov eax,[edi+4] ; cmp eax,ebx ; je) and pushes the copy source through edx; ours folds the known-non-null test away. tried: Data && count > 1, null-safe inline GetRefCount(), a reinterpret re-read; next: a volatile-free accessor the optimiser cannot value-number (e.g. an out-of-class inline IsShared over CIVCountedPointer)
// C3DAnimationBlendState::MakeOwnCopyOfData() -- give the blend state unshared data: a fresh CSharedData when it has none, a private copy when the current one is shared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3C1A0, length 192 bytes.
// Callees: operator new @ 0x00BFEA1A (0x20-byte CSharedData), C3DAnimationBlendState::CSharedData copy ctor @ 0x00A3D330
// (landed), the pointee's virtual Delete (slot 1, +4) when a count drops to zero (the last one as a tail jump).
// Vtable: C3DAnimationBlendState::CSharedData 0x0129D1F8 (rtti-vtables.tsv).
// Model: as the landed SetBaseAnimation 0x00A3C260: Data +4 is an intrusive CIVCountedPointer, the count at +4 of
// the pointee. Shape: the empty case attaches the new data (release-old, clear, store: no add-ref, the new data
// starts at 1); the shared case builds a CIVCountedPointer local over the copy, assigns it (skip when equal,
// release old, clear, store, add-ref) and releases the local at the end.
// Leaf: MakeOwnCopyOfData
struct C3DAnimationStateBase;
template <class T> struct CArray { T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage; };
struct C3DAnimationStateAlpha;

struct CIVCountedPointeeBase {                      // vtable 0x012354D0 (its store is dead in the inline ctor)
    CIVCountedPointeeBase() : IVRefCount(1) {}
    virtual void Destroy(int flags);
    virtual void Delete();                          // slot 1 (+4)
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) Delete(); }
    unsigned long IVRefCount;                       // +4
};

struct CSharedData : CIVCountedPointeeBase {
    CSharedData() : BaseAnimation(0), CombinationBlends0(0), CombinationBlends1(0), CombinationBlends2(0),
                    SkeletalMorph(0), m_1c(0) {}
    CSharedData(const CSharedData& other);          // 0x00A3D330
    virtual void Destroy(int flags);
    virtual void Delete();
    C3DAnimationStateBase* BaseAnimation;           // +8
    C3DAnimationStateAlpha* CombinationBlends0;     // +0xc CArray<C3DAnimationStateAlpha>
    C3DAnimationStateAlpha* CombinationBlends1;     // +0x10
    C3DAnimationStateAlpha* CombinationBlends2;     // +0x14
    void* SkeletalMorph;                            // +0x18
    long m_1c;                                      // +0x1c
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                              // +4 CIVCountedPointer<CSharedData>
    unsigned long GetDataRefCount() const { return Data ? Data->IVRefCount : 0; }   // null-safe GetRefCount()
    void MakeOwnCopyOfData();
};

void C3DAnimationBlendState::MakeOwnCopyOfData()
{
    if (!Data) {
        CSharedData* data = new CSharedData;        // Data = new CSharedData (attach: no add-ref)
        if (Data) {
            Data->Release();
            Data = 0;
        }
        Data = data;
    }
    else if (GetDataRefCount() > 1) {
        CSharedData* copy = new CSharedData(*Data); // CIVCountedPointer<CSharedData> copy(new CSharedData(*Data))
        if (Data != copy) {                         // Data = copy
            if (Data) {
                Data->Release();
                Data = 0;
            }
            Data = copy;
            if (copy)
                copy->AddRef();
        }
        if (copy)                                   // ~copy
            copy->Release();
    }
}
