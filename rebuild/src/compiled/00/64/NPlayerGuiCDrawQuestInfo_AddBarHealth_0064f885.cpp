#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::AddBarHealth(const CThingPhysical* thing, const CRGBColour& colour, const CCharString& symbol, float scale) -- return the ID of the health bar already tracking this thing, else create one with the symbol's sprite, append it through AddElement and return its new ID
// Leaf: AddBarHealth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064F885, length 184 bytes (+ padding), `ret 10h`.
// Callees: the element's virtual slot 3 (+0xc, the element type; 1 = bar), IS_THING_ALIVE<CIntelligentPointer<const
// CThingPhysical>>(const&) @ 0x00655D04 (`__fastcall`), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B, operator new @ 0x00BFEA1A (0x58 bytes),
// CQuestInfoElementBar::CQuestInfoElementBar(const CThingPhysical*, const CRGBColour&, long, float) @ 0x0064A450,
// CCountedPointer<CQuestInfoElementBar>::CCountedPointer(T*) @ 0x0065707E, the converting copy
// CCountedPointer<CQuestInfoElement>(const CCountedPointer<CQuestInfoElementBar>&) @ 0x00654B36,
// CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>) @ 0x0064A3ED, ~CCountedPointer<CQuestInfoElementBar>
// @ 0x00654B0A (the landed AddBar 0x0064F7FC model).
// Layout: CDrawQuestInfo elements are an STLport list at +0x58 (header node pointer; node next +0, data +8 = the counted
// element pointer). CQuestInfoElementBar (retail 0x58 bytes, packed): ThingValid +0x38, PThing (8-byte
// CIntelligentPointer) +0x39; CThing UID (64-bit) at +0x18.
// Shape: a frame-pointer size TU; the list walk re-reads the header for the end test, the match is a nested `if` chain
// with a 64-bit UID equality, returning the element ID straight from the loop; the creation is AddBar's.
class CCharString { public: char* m_pData; };
class CRGBColour { public: unsigned char R, G, B, A; };
class CThingPhysical { public:
    unsigned char _pad[0x18];
    unsigned __int64 UID;                          // +0x18
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x00655D04

namespace NPlayerGui {
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);
    template <class U> CCountedPointer(const CCountedPointer<U>& other);
    ~CCountedPointer();
    T* operator->() const { return m_pObject; }
};
struct CQuestInfoElement {                         // retail 0x1c bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long GetType() const;                  // slot 3 (+0xc)
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    void* PrimitiveHandles[3];                     // +0x10
};
#pragma pack(push, 1)
struct CQuestInfoElementBar : CQuestInfoElement {  // retail 0x58 bytes
    float      Value;                              // +0x1c
    float      MaxValue;                           // +0x20
    float      MinValue;                           // +0x24
    CRGBColour Colour;                             // +0x28
    CRGBColour FadeColour;                         // +0x2c
    long       Graphic1;                           // +0x30
    long       Graphic2;                           // +0x34
    bool       ThingValid;                         // +0x38
    CIntelligentPointer<const CThingPhysical> PThing;   // +0x39
    unsigned char _rest[0x58 - 0x41];
    CQuestInfoElementBar(const CThingPhysical* thing, const CRGBColour& colour, long graphic, float scale);   // 0x0064A450
};
#pragma pack(pop)
struct SElementNode {
    SElementNode* Next;                            // +0x00
    SElementNode* Prev;                            // +0x04
    CCountedPointer<CQuestInfoElement> Data;       // +0x08
};
class CDrawQuestInfo { public:
    unsigned char _pad[0x58];
    SElementNode* Elements;                        // +0x58 STLport list header
    long GetScriptSpriteBySymbol(const CCharString& symbol);           // 0x0064A37B
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddBarHealth(const CThingPhysical* thing, const CRGBColour& colour, const CCharString& symbol, float scale);
};

long CDrawQuestInfo::AddBarHealth(const CThingPhysical* thing, const CRGBColour& colour, const CCharString& symbol, float scale)
{
    for (SElementNode* node = Elements->Next; node != Elements; node = node->Next) {
        CQuestInfoElement* element = node->Data.m_pObject;
        if (element->GetType() == 1) {
            CQuestInfoElementBar* bar = static_cast<CQuestInfoElementBar*>(element);
            if (IS_THING_ALIVE(bar->PThing) && bar->PThing.GetPItem()->UID == thing->UID)
                return element->ID;
        }
    }
    long graphic = GetScriptSpriteBySymbol(symbol);
    CCountedPointer<CQuestInfoElementBar> element(new CQuestInfoElementBar(thing, colour, graphic, scale));
    AddElement(element);
    return element->ID;
}
}