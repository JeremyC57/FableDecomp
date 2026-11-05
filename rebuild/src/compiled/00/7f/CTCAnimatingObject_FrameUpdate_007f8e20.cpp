// CTCAnimatingObject::FrameUpdate() -- notice when the current animation has finished, start the next queued animation when idle, then advance the thing's animation by one frame (1 / fps seconds, divided by the bullet-time scale when it applies)
// Leaf: FrameUpdate
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8E20, 267 bytes (+5 int3 padding), `ret`; CTCAnimatingObject vtable
//   0x012417E4 slot 9 (the TC FrameUpdate slot; the catalog's "vfunc_9").
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCAnimationComplex, type
//   0x5A), CTCAnimationComplex::GetAnimation 0x0070B460, CTCAnimatingObject::PlayAnimation(const CAnimInit&, long)
//   0x007F8DC0, free 0x00BFEA14, CTCBase::PeekWorld() const 0x00686A80 (twice), CThing::IsAffectedByBulletTime
//   0x004C9920, CBulletTime::GetTimeScale 0x004C5FA0, CAnimateThing::UpdateFrameLogic(const CAnimFrameTime&) 0x0070E8B0.
//   Globals CMainGameComponent::GetConstantFPS() 0x01375550, 1.0f 0x0122DED8.
// Layout = Ego_r.pdb CTCAnimatingObject: Animating +0x0C, CurrentAnimationPriority +0x10, QueuedAnims +0x14 (STLport
//   list<CAnimInit>, one word: the landed ClearQueuedAnims 0x007F8BE0); the world's bullet-time controller +0x68.
// Shape: the landed CTCBed/CTCCupboard animation accessor (an unset local when the component is absent); the queue's
//   front is played and popped through the inline STLport erase(begin()); the frame time record is built by a
//   (seconds) constructor from `scaled ? seconds / scale : seconds`.
extern "C" void __cdecl free(void* memory);     // 0x00BFEA14
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAnimInit { public:                              // PDB 44 bytes
    long          UpdateMode;
    unsigned char Type;
    bool          Looping;
    long          Anim;
    float         Speed;
    float         Alpha;
    float         LastAlpha;
    long          Frame[2];
    unsigned long Flags;
    unsigned long TypeFlags;
    long          Priority;
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {         // STLport list: one word, the sentinel node
    SListNode<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
    void erase(SListNode<T>* position)
    {
        SListNode<T>* next = position->_M_next;
        SListNode<T>* prev = position->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position);
    }
    void pop_front() { erase(_M_node->_M_next); }
};
struct CAnimFrameTime {                         // 12-byte UpdateFrameLogic argument
    float Seconds;
    long  Reserved1;
    long  Reserved2;
    CAnimFrameTime(float seconds) : Seconds(seconds), Reserved1(0), Reserved2(0) { }
};
class CAnimateThing { public:
    unsigned char _pad00[0x04];
    void*         m_pCurrent;                   // +0x04
    void*         m_pEnd;                       // +0x08
    unsigned char _pad0c[0x51 - 0x0c];
    unsigned char m_stateFlags;                 // +0x51 (bit 1: finished)
    void UpdateFrameLogic(const CAnimFrameTime& time);   // 0x0070E8B0
    bool IsFinished() const { return m_pCurrent == m_pEnd || (m_stateFlags & 2); }
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
struct CBulletTime {
    unsigned char _pad0[4];
    void* m_pController;                        // +0x04
    unsigned char _pad1[4];
    bool m_active;                              // +0x0c
    float GetTimeScale();                       // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};
class CWorld { public:
    unsigned char _pad[0x68];
    CBulletTime*  m_pBulletTime;                // +0x68
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCAnimatingObject : public CTCBase {
public:
    CThing*                ParentThing;         // +0x04
    bool                   ToBeRemoved;         // +0x08
    bool                   IsInGlobalUpdateTCs; // +0x09
    unsigned char          _pad0a[2];
    bool                   Animating;           // +0x0c
    long                   CurrentAnimationPriority;   // +0x10
    CRetailList<CAnimInit> QueuedAnims;         // +0x14
private:
    void PlayAnimation(const CAnimInit& init, long priority);   // 0x007F8DC0 (Ego_r: private)
public:
    __forceinline CTCAnimationComplex* GetAnimationComplex() const
    {
        CTCAnimationComplex* complex;
        if (ParentThing->HasTC(TCI_ANIMATION))
            complex = ((CTCAnimationComplex*)&ParentThing->GetTC(TCI_ANIMATION));
        return complex;
    }
    void FrameUpdate();
};

void CTCAnimatingObject::FrameUpdate()
{
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    if (Animating) {
        if (animate.IsFinished())
            Animating = false;
    }
    if (!Animating) {
        if (!QueuedAnims.empty()) {
            PlayAnimation(QueuedAnims.front(), -1);
            QueuedAnims.pop_front();
        }
    }
    float seconds = 1.0f / CMainGameComponent::GetConstantFPS();
    CAnimFrameTime time((PeekWorld().m_pBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime())
                            ? seconds / PeekWorld().m_pBulletTime->GetTimeScale() : seconds);
    animate.UpdateFrameLogic(time);
}