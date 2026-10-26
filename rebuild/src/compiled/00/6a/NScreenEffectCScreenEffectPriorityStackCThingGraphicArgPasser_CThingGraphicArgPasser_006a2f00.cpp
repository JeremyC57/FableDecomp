// NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser::CThingGraphicArgPasser(CThing* thing, bool bypassSet) -- remember the thing in an intelligent pointer and the bypass flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2F00, length 39 bytes.
// Leaf: ??0CThingGraphicArgPasser
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90. Vtable
// 0x01238C6C = CIntelligentPointer<CThing> (landed sources).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer" is propagated. The
// object is a CIntelligentPointer<CThing> at +0 followed by a bool at +8, built from a pointer and a bool passed
// by value (`ret 8`). Ego_r.pdb has exactly two 9-byte { CIntelligentPointer<CThing> thing; bool ... } structs,
// and FableWin.pdb names the constructor of one of them with this signature:
// ??0CThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@QAE@PAVCThing@@_N@Z (thing, bypassSet).
// Shape: base constructor, the pointer's own vtable store scheduled between the argument push and SetPItem
// (the `CIntelligentPointer(T*)` constructor), then the flag byte.
class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
};

namespace NScreenEffect {
class CScreenEffectPriorityStack { public:
    struct CThingGraphicArgPasser {
        CIntelligentPointer<CThing> thing;      // +0x00
        bool                        bypassSet;  // +0x08
        CThingGraphicArgPasser(CThing* thing_, bool bypassSet_);
    };
};

CScreenEffectPriorityStack::CThingGraphicArgPasser::CThingGraphicArgPasser(CThing* thing_, bool bypassSet_)
    : thing(thing_), bypassSet(bypassSet_)
{
}
}
#pragma pack(pop)