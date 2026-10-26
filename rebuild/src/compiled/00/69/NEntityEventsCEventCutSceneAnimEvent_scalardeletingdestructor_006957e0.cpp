// NEntityEvents::CEventCutSceneAnimEvent::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor (EventName destroyed, then the inline NEntityEvents::CEventBase destructor's vtable store) and the flag-tested operator delete
// Leaf: ??_GCEventCutSceneAnimEvent@NEntityEvents@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006957E0, 39 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity (proven): NEntityEvents::CEventCutSceneAnimEvent::`scalar deleting destructor'; evidence: it is slot 0 of vtable 0x0125C1CC (RTTI .?AVCEventCutSceneAnimEvent@NEntityEvents@@,
//   bases .?AVCEventBase@NEntityEvents@@(+0)), it destroys EventName at +0x8 (Ego_r.pdb CCharString EventName +0x8) and stores the
//   NEntityEvents::CEventBase vtable 0x0124F2C4 (RTTI .?AVCEventBase@NEntityEvents@@).
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H085: six deleting destructors of classes whose only non-trivial
//   member is one object at +8 / +0x94; retail has no identical-code folding, so each address is its own class's ??_G).
// Callees: CCharString::~CCharString 0x0099EAE0 on EventName, operator delete 0x00BFE9BC.
// Shape: NEntityEvents::CEventCutSceneAnimEvent has no user-declared destructor (no own vtable store); NEntityEvents::CEventBase's `virtual ~CEventBase() { }` is inline
//   and stores its vtable; the vtable (and so this ??_G) is emitted by the construction helper below. Local classes, Review at landing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString {                                        // 4 bytes: shared string block
public:
    void* PStringData;
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
};
namespace NEntityEvents {
class CEventBase {                                         // vtable 0x0124F2C4
public:
    virtual ~CEventBase() { }
    long EventType;                                         // +0x04
};
class CEventCutSceneAnimEvent : public CEventBase {
public:
    CCharString EventName;                    // +0x08
};
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_006957e0()
{
    return new NEntityEvents::CEventCutSceneAnimEvent;
}