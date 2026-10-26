// NEntityEvents::CEventTalkedToAnyone::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor (PThingTalkedTo destroyed, then the inline NEntityEvents::CEventBase destructor's vtable store) and the flag-tested operator delete
// Leaf: ??_GCEventTalkedToAnyone@NEntityEvents@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B150, 39 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity (proven): NEntityEvents::CEventTalkedToAnyone::`scalar deleting destructor'; evidence: it is slot 0 of vtable 0x01274270 (RTTI .?AVCEventTalkedToAnyone@NEntityEvents@@,
//   bases .?AVCEventBase@NEntityEvents@@(+0)), it destroys PThingTalkedTo at +0x8 (Ego_r.pdb CIntelligentPointer<CThing> PThingTalkedTo +0x8) and stores the
//   NEntityEvents::CEventBase vtable 0x0124F2C4 (RTTI .?AVCEventBase@NEntityEvents@@).
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H085: six deleting destructors of classes whose only non-trivial
//   member is one object at +8 / +0x94; retail has no identical-code folding, so each address is its own class's ??_G).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 on PThingTalkedTo (the implicit ~CIntelligentPointer<CThing> inline), operator delete 0x00BFE9BC.
// Shape: NEntityEvents::CEventTalkedToAnyone has no user-declared destructor (no own vtable store); NEntityEvents::CEventBase's `virtual ~CEventBase() { }` is inline
//   and stores its vtable; the vtable (and so this ??_G) is emitted by the construction helper below. Local classes, Review at landing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CBaseIntelligentPointer {                            // 8 bytes: vptr + PData
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
};
class CThing;
namespace NEntityEvents {
class CEventBase {                                         // vtable 0x0124F2C4
public:
    virtual ~CEventBase() { }
    long EventType;                                         // +0x04
};
class CEventTalkedToAnyone : public CEventBase {
public:
    CIntelligentPointer<CThing> PThingTalkedTo; // +0x08
};
}

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_0084b150()
{
    return new NEntityEvents::CEventTalkedToAnyone;
}