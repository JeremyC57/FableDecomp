// NEntityEvents::CEventOpenedChest::CEventOpenedChest(CThing&) -- the event starts at world frame 0 and its PChest intelligent pointer is pointed at the argument
// Leaf: ??0CEventOpenedChest@NEntityEvents@@QAE@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084B080, 52 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: NEntityEvents::CEventOpenedChest::CEventOpenedChest(CThing&) -- proven; evidence: it stores vtable 0x01274258 whose RTTI is
//   .?AVCEventOpenedChest@NEntityEvents@@ : CEventBase@NEntityEvents; FableWin.pdb name set holds ??0CEventOpenedChest@NEntityEvents@@QAE@AAVCThing@@@Z;
//   Ego_r.pdb NEntityEvents::CEventOpenedChest is 16 bytes with CIntelligentPointer<CThing> PChest at +8. Family G038 (8 event
//   constructors that differ in the class, its vtable and the member).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
//   0x00A01B90. Vtables: CEventOpenedChest 0x01274258, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: CEventBase's inline constructor stores WorldFrame = 0; its own vtable store is dead (overwritten by the class
//   vtable), so WorldFrame lands before the class vptr. The member's inline (T*) constructor runs the out-of-line base
//   constructor, stores its own vtable, then SetPItem.
// Model: local -- the generated engine headers flatten the vptr into a word, which cannot carry these vtables.
class CBaseObject {
public:
    virtual ~CBaseObject();
};
class CThing : public CBaseObject { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    virtual ~CIntelligentPointer() {}
    virtual T* GetPItem() const;
};
namespace NEntityEvents {
class CEventBase {
public:
    CEventBase() : WorldFrame(0) {}
    virtual ~CEventBase() {}
    long WorldFrame;                                         // +0x04
};
class CEventOpenedChest : public CEventBase {
public:
    CEventOpenedChest(CThing& thing);
    CIntelligentPointer<CThing> PChest;            // +0x08
};

CEventOpenedChest::CEventOpenedChest(CThing& thing)
    : PChest(&thing)
{
}
}