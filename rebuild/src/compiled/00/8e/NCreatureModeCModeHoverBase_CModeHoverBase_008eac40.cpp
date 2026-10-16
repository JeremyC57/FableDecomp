// NCreatureMode::CModeHoverBase::CModeHoverBase(CThing* thing) -- CModeBase(thing), own vtable, empty HoverStates vector, CurrentName set to "", an empty AnimQueue list (self-linked sentinel), a null PParticleEffect
// Leaf: ??0CModeHoverBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EAC40, length 99 bytes (`ret 4`). Catalog label "CTCKrakenTentacle::CTCKrakenTentacle"
// is a propagated label: the vtable store 0x01287D14 is RTTI .?AVCModeHoverBase@NCreatureMode@@ (base CTCCreatureModeManager::CModeBase);
// callers are the derived hover-mode ctors (CModeHoverNormal @ 0x00744040, vtable 0x012687FC; also 0x00743DB0, 0x00744130).
// Callees: CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CCharString::CCharString() @ 0x0099E4B0, malloc @ 0x00BFEA0E (0x10-byte list
// node), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CCharString::operator=(const char*) @ 0x0099EFE0 (literal "" @ 0x0122D70E).
// Layout (retail, below the PDB's 0x4C): CModeBase 0x1C; HoverStates +0x1C (12-byte vector, PDB 16), CurrentName +0x28 (PDB +0x2C),
// PCurrentState +0x2C and CurrentMovementType +0x30 (left uninitialised), AnimQueue +0x34 (4-byte STLport list: sentinel pointer only),
// PParticleEffect +0x38 (CIntelligentPointer<CThing>, vtable 0x01238C6C).
// Shape: init-list members with inline ctors (the vector as the STLport _Vector_base + _STLP_alloc_proxy model, which puts
// `mov ecx,edi` between its second and third zero store; zeros share ebx; list zeroes then mallocs its sentinel as in landed 0x0070A520),
// then `CurrentName = ""` in the body.
#include <stdlib.h>
struct CThing;
struct CCharString {
    void* m_pData;
    CCharString();                                  // 0x0099E4B0
    CCharString& operator=(const char* text);       // 0x0099EFE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};
struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];                      // ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);                       // 0x0073EAC0
};
namespace NCreatureMode {
struct CHoverState;
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             m_pStart;
    T*             m_pFinish;
    CAllocProxy<T> m_endOfStorage;
    CVectorBase(const CAllocator<T>& alloc) : m_pStart(0), m_pFinish(0), m_endOfStorage(alloc, 0) { }
};
struct CHoverStateVector : CVectorBase<CHoverState> {   // retail vector<CModeHoverBase::CHoverState>: 12 bytes
    CHoverStateVector() : CVectorBase<CHoverState>(CAllocator<CHoverState>()) { }
};
struct SHoverAnimNode {                             // STLport _List_node<CCountedPointer<CHoverAnim>>: 0x10 bytes
    SHoverAnimNode* Next;
    SHoverAnimNode* Prev;
    void* Object;
    void* Count;
};
struct CHoverAnimList {                             // STLport list: the sentinel pointer only
    SHoverAnimNode* Node;
    CHoverAnimList() : Node(0)
    {
        SHoverAnimNode* sentinel = static_cast<SHoverAnimNode*>(malloc(sizeof(SHoverAnimNode)));
        sentinel->Next = sentinel;
        sentinel->Prev = sentinel;
        Node = sentinel;
    }
};
struct CModeHoverBase : CModeBase {                 // vtable 0x01287D14
    CHoverStateVector        HoverStates;           // +0x1c
    CCharString              CurrentName;           // +0x28
    void*                    PCurrentState;         // +0x2c
    long                     CurrentMovementType;   // +0x30
    CHoverAnimList           AnimQueue;             // +0x34
    CIntelligentPointerThing PParticleEffect;       // +0x38
    virtual void v00();
    CModeHoverBase(CThing* thing);
};

CModeHoverBase::CModeHoverBase(CThing* thing)
    : CModeBase(thing)
{
    CurrentName = "";
}
}