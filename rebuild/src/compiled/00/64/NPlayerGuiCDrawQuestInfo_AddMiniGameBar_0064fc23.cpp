// NPlayerGui::CDrawQuestInfo::AddMiniGameBar(EMiniGameType type, CThing* thing) -- create a mini-game bar element (green colour), append it,
// pick its graphic from the player GUI def by type (types 3..5 also get a red fade colour); type 6 takes the graphic of the
// living thing's inventory item (TC 0x1a) and adds a counter element whose ID goes into the bar's ExtraElements; returns the bar's ID.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064FC23, length 401 bytes, `ret 8`.
// Callees: operator new 0x00BFEA1A (0x40, 0x2c); CQuestInfoElementMiniGameBar(EMiniGameType) 0x00656CAA; CCountedPointer ctor 0x00656F2F,
// converting copy 0x00654C8F, dtor 0x00654C63; CDrawQuestInfo::AddElement 0x0064A3ED; PeekGuiDef 0x00438570 (__fastcall);
// IS_THING_ALIVE<CThing*> 0x0040DCE0; std::bitset::test 0x00410DE0; CVectorMap LowerBound const 0x004365B0; CTCInventoryItem
// GetEngineGraphic 0x005D92D0; CQuestInfoElementCounter(long, long, float) 0x00656BD8; CCountedPointer<Counter> ctor 0x006570FA,
// converting copy 0x00654BC0, dtor 0x00654B94; vector<long>::push_back 0x00485FF3.
// Layout: CQuestInfoElementMiniGameBar (Ego_r.pdb -4): Type +0x1c, Percentage +0x20, Colour +0x24, FadeColour +0x28, Updatable +0x2c,
// Graphic +0x30, ExtraElements +0x34. CPlayerGuiDef graphics +0x468/+0x474..+0x484 (descriptive names).
// Shape: frame-pointer size TU; the bar pointer read once into esi after AddElement (named local); case 6 tests the TC bit
// with a pushed literal and the GetTC lookup. IS_THING_ALIVE has a same-TU read-only stand-in body, so the conversion
// temporary's slot (the dead `type` argument) does not escape and VC7.1 reuses it for the lookup key, as retail.
// Leaf: AddMiniGameBar
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CTCBitset {
    unsigned long _M_w[9];
    bool test(long bit) const;                           // 0x00410DE0
};
struct CThing {
    unsigned char _pad00[0x20];
    CTCBitset     UsingComponents;                       // +0x20
    CTCMap        Components;                            // +0x44
    template<class T> __forceinline T* GetTC(long type) const
    {
        unsigned long key = type;
        const CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return (T*)it->TC;
    }
    template<class T> __forceinline bool GetInterface(T** out, long type) const
    {
        if (UsingComponents.test(type)) {
            unsigned long key = type;
            const CTCMap* map = &Components;
            CTCEntry* it = map->LowerBound(key);
            if (it == map->_M_finish || type < it->Type)
                it = map->_M_finish;
            *out = (T*)it->TC;
            return true;
        }
        return false;
    }
};
struct CThingPhysical : CThing { };
template<class T> bool __fastcall IS_THING_ALIVE_0040dce0(const T& thing);   // 0x0040DCE0 (CThing*)
bool IsThingAliveImpl(const void* thing);                // stand-in callee of the stand-in body below
// Same-TU read-only stand-in for the IS_THING_ALIVE<CThing*> instantiation (out of line in retail): the argument's
// address does not escape, so VC7.1 can reuse its slot.
#pragma auto_inline(off)
template<class T> bool __fastcall IS_THING_ALIVE_0040dce0(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
struct CTCInventoryItem {                                // TC type 0x1a
    const long& GetEngineGraphic() const;                // 0x005D92D0
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x468];
    long MiniGameGraphic468;                             // +0x468
    unsigned char _pad46c[0x474 - 0x46c];
    long MiniGameGraphic474;                             // +0x474
    long MiniGameGraphic478;                             // +0x478
    long MiniGameGraphic47c;                             // +0x47c
    long MiniGameGraphic480;                             // +0x480
    long CounterGraphic484;                              // +0x484
};
const CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CLongVector {                                     // STLport vector<long>
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    void push_back(const long& value);                   // 0x00485FF3
};

namespace NPlayerGui {
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);
    template <class U> CCountedPointer(const CCountedPointer<U>& other);
    ~CCountedPointer();
    T* operator->() const { return m_pObject; }
};
struct CQuestInfoElement {
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    void* PrimitiveHandles[3];                     // +0x10
};
enum EMiniGameType { MINI_GAME_TYPE_NONE = 0 };
struct CQuestInfoElementMiniGameBar : CQuestInfoElement {   // retail 0x40 bytes
    EMiniGameType Type;                            // +0x1c
    float         Percentage;                      // +0x20
    CRGBColour    Colour;                          // +0x24
    CRGBColour    FadeColour;                      // +0x28
    bool          Updatable;                       // +0x2c
    long          Graphic;                         // +0x30
    CLongVector   ExtraElements;                   // +0x34
    CQuestInfoElementMiniGameBar(EMiniGameType type);   // 0x00656CAA
};
struct CQuestInfoElementCounter : CQuestInfoElement {   // retail 0x2c bytes
    long m_count; long m_graphic; long m_displayedCount; long m_maxValue;
    CQuestInfoElementCounter(long graphic, long maxValue, float scale);   // 0x00656BD8
};
struct CDrawQuestInfo {
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddMiniGameBar(EMiniGameType type, CThingPhysical* thing);
};

long CDrawQuestInfo::AddMiniGameBar(EMiniGameType type, CThingPhysical* thing)
{
    CCountedPointer<CQuestInfoElementMiniGameBar> element(new CQuestInfoElementMiniGameBar(type));
    AddElement(element);
    CQuestInfoElementMiniGameBar* bar = element.m_pObject;
    bar->Colour = CRGBColour(0, 255, 0, 255);
    const CPlayerGuiDef* def = PeekGuiDef();
    switch (type) {
    case 1:
        bar->Graphic = def->MiniGameGraphic468;
        break;
    case 2:
        bar->Graphic = def->MiniGameGraphic474;
        break;
    case 3:
        bar->Graphic = def->MiniGameGraphic478;
        bar->FadeColour = CRGBColour(255, 0, 0, 255);
        break;
    case 4:
        bar->Graphic = def->MiniGameGraphic47c;
        bar->FadeColour = CRGBColour(255, 0, 0, 255);
        break;
    case 5:
        bar->Graphic = def->MiniGameGraphic480;
        bar->FadeColour = CRGBColour(255, 0, 0, 255);
        break;
    case 6: {
        if (IS_THING_ALIVE_0040dce0<CThing*>(thing) && thing->UsingComponents.test(0x1a))
            bar->Graphic = thing->GetTC<CTCInventoryItem>(0x1a)->GetEngineGraphic();
        CCountedPointer<CQuestInfoElementCounter> counter(new CQuestInfoElementCounter(def->CounterGraphic484, 0, 1.0f));
        AddElement(counter);
        bar->ExtraElements.push_back(counter->ID);
        break;
    }
    }
    return bar->ID;
}
}