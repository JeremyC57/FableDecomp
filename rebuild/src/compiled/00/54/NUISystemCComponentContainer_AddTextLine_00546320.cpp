// NUISystem::CComponentContainer::AddTextLine(const CWideString& text, CRGBColour colour, unsigned long spriteIndex, long indent) -- adds a text line: a "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" text (text window indented when indent is set) in the colour; with a sprite index, a "UI_TEXT_SPRITE_CONTAINER" holding that sprite and the text (shifted right past the sprite) is added instead
// Leaf: ?AddTextLine@CComponentContainer@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546320, 648 bytes (0x288, int3 padding to 656), thiscall `ret 10h`, /O2 esp
//   frame (sub esp,28h). Slot 143 of the CComponentContainer vtable 0x01247AF4. Identity: Ego_r.pdb
//   ?AddTextLine@CComponentContainer@NUISystem@@UAEXABVCWideString@@VCRGBColour@@KJ@Z (the unsigned-long sprite overload;
//   the catalog's "UIContainer_SetupTextSprite" is descriptive).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), CFrontEndManager::CreateComponent(const CCharString&, bool)
//   0x0041DB1D, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, operator new 0x00BFEA1A / delete
//   0x00BFE9BC, CCountedPointer<CComponent>(CComponent*) 0x00429C15 (the 2nd and 3rd; the 1st inlines) with the delete
//   function 0x00428B10, map<unsigned long, CUIState>::operator[] 0x0042D5B1; virtual slots: CComponent AddChild 59
//   (+0xec) / GetStates 82 (+0x148), CText SetValue(const CWideString&, bool, bool) 144 (+0x240) / SetTextWindow(C2DBoxF&)
//   147 (+0x24c), CSprite SetGraphicIndex(unsigned long) 145 (+0x244) / GetWidth() 147 (+0x24c). Strings
//   "UI_TEXT_INSIDE_CONTAINER_TEMPLATE" 0x01247D70, "UI_TEXT_SPRITE_CONTAINER_SPRITE" 0x01247D50,
//   "UI_TEXT_SPRITE_CONTAINER" 0x01247D34; floats 4294967296.0 0x0122DCB4 (unsigned convert), 4.0 0x0123088C.
// Layout: CUIState (the landed CSpellContainer::AddChildDynamically 0x00545F00 model) Position +0x08, Colour +0x18;
//   C2DBoxF {TLX, TLY, BRX, BRY} (Ego_r.pdb); CCountedPointer {Data, Info}, Info {RefCount, DeleteFunc, Data}.
// Shape: three counted component pointers (text, sprite, container: the 1st ctor inlines, VC7.1's own choice); the text
//   window box is built before the indent test; the text and sprite components are named casts of the pointers' Data
//   (cached in esi across their virtual calls) while the sprite branch re-reads the text pointer for its two state
//   writes; every GetStates()[0] key is a temporary in the dead `indent` slot. The map's operator[] has a same-TU stand-in
//   (by-value key read): with only a declaration VC7.1 stores the 2nd and 3rd keys before their argument push.
void __cdecl operator delete(void* p);                                               // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);                                        // 0x00BFEA1A

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                                   // 0x0099EBF0
    ~CCharString();                                                                    // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
};
class C2DVector {
public:
    float X;
    float Y;
};
class C2DBoxF {
public:
    float TLX;                                                                         // +0x00
    float TLY;                                                                         // +0x04
    float BRX;                                                                         // +0x08
    float BRY;                                                                         // +0x0c
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CCPPointerInfo { public:
    long RefCount;                                                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);                                         // +0x04
    void* Data;                                                                        // +0x08
    CCPPointerInfo(long refCount, void (__fastcall* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                                              // +0x00
    CCPPointerInfo* Info;                                                              // +0x04
    static void __fastcall DeleteData(void* data);                                     // 0x00428B10
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, &DeleteData, Data);
        else
            Info = 0;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
};

namespace NUISystem {

class CUIState { public:                           // map value (key: the state index)
    unsigned long StateChangeFlag;          // +0x00
    long          StateChangeType;          // +0x04
    C2DVector     Position;                 // +0x08
    C2DVector     Zoom;                     // +0x10
    CRGBColour    Colour;                   // +0x18
    float         UpdateTime;               // +0x1c
    bool          LinearChange;             // +0x20
};
void* LookupState(void* map, unsigned long key);
class CStateMap {                           // STLport map<unsigned long, CUIState> (12 bytes)
public:
    void*         Header;
    unsigned long NodeCount;
    unsigned char Compare;
    __declspec(noinline) CUIState& operator[](const unsigned long& key) { return *(CUIState*)LookupState(this, key); }   // 0x0042D5B1
};

class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void AddChild(CCountedPointer<CComponent>& child);                         // slot 59 (+0xec)
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual CStateMap& GetStates();                                                    // slot 82 (+0x148)
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual void VSlot135();
    virtual void VSlot136();
    virtual void VSlot137();
    virtual void VSlot138();
    virtual void VSlot139();
    virtual void VSlot140();
    virtual void VSlot141();
    virtual void VSlot142();
};
class CText : public CComponent {
public:
    virtual void VSlot143();
    virtual void SetValue(const CWideString& text, bool a, bool b);                    // slot 144 (+0x240)
    virtual void VSlot145();
    virtual void VSlot146();
    virtual void SetTextWindow(C2DBoxF& window);                                       // slot 147 (+0x24c)
};
class CSprite : public CComponent {
public:
    virtual void VSlot143();
    virtual void VSlot144();
    virtual void SetGraphicIndex(unsigned long index);                                 // slot 145 (+0x244)
    virtual void VSlot146();
    virtual unsigned long GetWidth();                                                  // slot 147 (+0x24c)
};
class CComponentContainer : public CComponent {
public:
    virtual void AddTextLine(const CWideString& text, CRGBColour colour, unsigned long spriteIndex, long indent);   // slot 143 (+0x23c)
};
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();                                 // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, bool b);                      // 0x0041DB1D
};

void CComponentContainer::AddTextLine(const CWideString& text, CRGBColour colour, unsigned long spriteIndex, long indent)
{
    CFrontEndManager* manager = CFrontEndManager::GetInstance();
    CCountedPointer<CComponent> textLine(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
    CText* textComponent = (CText*)textLine.Data;
    C2DBoxF window(0.0f, 0.0f, (float)indent, 100.0f);
    if (indent != 0)
        textComponent->SetTextWindow(window);
    textComponent->SetValue(text, true, true);
    textComponent->GetStates()[0].Colour = colour;
    if (spriteIndex != 0) {
        CCountedPointer<CComponent> sprite(manager->CreateComponent("UI_TEXT_SPRITE_CONTAINER_SPRITE", false));
        CSprite* spriteComponent = (CSprite*)sprite.Data;
        spriteComponent->SetGraphicIndex(spriteIndex);
        float x = (float)spriteComponent->GetWidth() + 4.0f;
        textLine->GetStates()[0].Position.X = x;
        textLine->GetStates()[0].Position.Y = 4.0f;
        CCountedPointer<CComponent> container(manager->CreateComponent("UI_TEXT_SPRITE_CONTAINER", false));
        container->AddChild(sprite);
        container->AddChild(textLine);
        AddChild(container);
    }
    else
        AddChild(textLine);
}

}