// C3DAnimationBlendState::SetBaseAnimation(const C3DAnimationState&) -- unshare the data, then replace its base animation with a heap copy of the state
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3C260, 112 bytes. Callees: C3DAnimationBlendState::MakeOwnCopyOfData
// 0x00A3C1A0, operator new 0x00BFEA1A, the old state's virtual Delete (slot 1, +4) when its count drops to zero.
// Vtables: CIVCountedPointeeBase 0x012354D0, C3DAnimationState 0x01249F50.
// Model as the landed SetSkeletalMorph (0x00A3C350): Data +4, CSharedData::BaseAnimation +8 is an intrusive
// CIVCountedPointer. `new C3DAnimationState(state)` is the IMPLICIT copy constructor: the base copies the
// reference count word (+4) between its vtable store and the derived one, then AnimationIndex/AnimationTime
// (+8/+0xC) as dwords. The pointer assignment releases the old pointee (`dec [p+4] ; jne ; call [vt+4]`),
// clears the member, then stores the new pointer (no add-ref: the copy carried the count). The release is an
// inline member of the pointee so the pointer stays in ecx (`if (--m_p->count == 0)` re-reads the member).
#include <stddef.h>

struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);       // slot 0
    virtual void Delete();                 // slot 1 (+4)
    unsigned long IVRefCount;              // +4
    void Release() { if (--IVRefCount == 0) Delete(); }
};

struct C3DAnimationStateBase : CIVCountedPointeeBase {
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void Validate();               // slot 5 (+0x14)
};

struct C3DAnimationState : C3DAnimationStateBase {      // 0x10 bytes (engine/C3DAnimationState.h)
    long AnimationIndex;                   // +0x08
    float AnimationTime;                   // +0x0C
};

template <class T>
struct CIVCountedPointer {
    T* m_pObject;
    T* operator->() const { return m_pObject; }
    CIVCountedPointer& operator=(T* object)
    {
        if (m_pObject) {
            m_pObject->Release();
            m_pObject = 0;
        }
        m_pObject = object;
        return *this;
    }
};

struct C3DAnimationStateAlpha;
template <class T>
struct CArray {                            // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;                           // +0
    T* m_pEnd;                             // +4
    T* m_pEndOfStorage;                    // +8
};

struct CSharedData {
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;                                 // +4
    CIVCountedPointer<C3DAnimationStateBase> BaseAnimation;   // +8
    CArray<C3DAnimationStateAlpha> CombinationBlends;         // +0xc
};

struct C3DAnimationBlendState {
    virtual void v00();
    CSharedData* Data;                     // +4 CIVCountedPointer<CSharedData>
    void MakeOwnCopyOfData();              // 0x00A3C1A0
    void SetBaseAnimation(const C3DAnimationState& state);
};

void C3DAnimationBlendState::SetBaseAnimation(const C3DAnimationState& state)
{
    MakeOwnCopyOfData();
    Data->BaseAnimation = new C3DAnimationState(state);
}
