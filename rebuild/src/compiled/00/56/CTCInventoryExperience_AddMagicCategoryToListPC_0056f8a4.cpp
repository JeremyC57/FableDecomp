// CTCInventoryExperience::AddMagicCategoryToListPC(CComponent* screen, CList* list, CWideString title, CCharString definition, CList* subList, CWideString description, CCharString icon) -- adds a magic-category branch entry (title, sub-list, category description and subtitle) to the PC experience-spending list and registers its parts with the PC screen
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056F8A4, 563 bytes, `ret 1Ch`; frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`, as the landed AddSoundAction 0x0056F615 of the same TU).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall), CManager::CreateMenuEntryComponent 0x00428494
// (manifest signature: const CWideString&, float, float, const CCharString&, float, CCharString, float, CCharString),
// CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCountedPointer<CComponent>(CComponent*) 0x00429C15
// and its dtor 0x004291DE, AddSoundAction 0x0056F615, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, CCharString::ToWideString 0x0099E480, ~CWideString 0x0099B510; component slots 59 (+0xEC, AddChild),
// 144 (+0x240, SetText(const CWideString&, bool, bool)), list slot 152 (+0x260, AddItem), viewport slot 142 (+0x238), the screen's slot 124 (+0x1F0, registers a component for mouse input).
// Layout: retail CTCInventoryExperience pTextViewport +0x1DC (landed ctor 0x0057832D).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                   // 0x0099B510
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
    CWideString ToWideString() const;                 // 0x0099E480
};

namespace NUISystem {
struct CComponent;
}
struct SCountBlock;
struct CComponentPointer {                            // CCountedPointer<NUISystem::CComponent>
    NUISystem::CComponent* m_pObject;
    SCountBlock*           m_pCount;
    CComponentPointer(NUISystem::CComponent* object); // 0x00429C15
    ~CComponentPointer();                             // 0x004291DE
    NUISystem::CComponent* operator->() const { return m_pObject; }
};

#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
namespace NUISystem {
struct CGUIElement;
struct CComponent {
    V10(v0) V10(v1) V10(v2) V10(v3) V10(v4)
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(const CComponentPointer& child);    // slot 59 (+0xEC)
    V10(v6) V10(v7) V10(v8) V10(v9) V10(w0) V10(w1)
    virtual void w20(); virtual void w21(); virtual void w22(); virtual void w23();
    virtual void RegisterInputComponent(CComponent* component);   // slot 124 (+0x1F0)
    virtual void w25(); virtual void w26(); virtual void w27(); virtual void w28(); virtual void w29();
    V10(w3)
    virtual void w40(); virtual void w41();
    virtual void AddViewportChild(CComponent* child); // slot 142 (+0x238)
    virtual void w43();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
    virtual void w45(); virtual void w46(); virtual void w47(); virtual void w48(); virtual void w49();
    virtual void w50(); virtual void w51();
    virtual void AddItem(const CComponentPointer& item);      // slot 152 (+0x260)
};
struct CList : CComponent {
};
struct CViewport : CComponent {
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString d, float e, CCharString f);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}

struct CTCInventoryExperience {
    void*                 __vftable;                  // +0x00
    unsigned char         _pad_0x04[0x1D8];
    NUISystem::CViewport* pTextViewport;              // +0x1DC
    void AddSoundAction(NUISystem::CComponent* element, CCharString soundName);   // 0x0056F615
    void AddMagicCategoryToListPC(NUISystem::CComponent* screen, NUISystem::CList* list, CWideString title, CCharString definition,
                                  NUISystem::CList* subList, CWideString description, CCharString icon);
};

void CTCInventoryExperience::AddMagicCategoryToListPC(NUISystem::CComponent* screen, NUISystem::CList* list, CWideString title,
                                                      CCharString definition, NUISystem::CList* subList, CWideString description,
                                                      CCharString icon)
{
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    CComponentPointer entry(frontEnd->CreateMenuEntryComponent(title, 0.0f, 0.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                               0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CComponentPointer subPointer(subList);
    AddSoundAction(entry.operator->(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_ALL");
    CComponentPointer definitionPointer(NUISystem::CFrontEndManager::GetInstance()->CreateComponent(definition, false));
    entry->AddChild(subPointer);
    entry->AddChild(definitionPointer);
    screen->RegisterInputComponent(definitionPointer.m_pObject);
    CComponentPointer descriptionPointer(frontEnd->CreateComponent("PC_EXPSPEND_CAT_DESC", false));
    descriptionPointer->SetText(icon.ToWideString(), true, true);
    entry->AddChild(descriptionPointer);
    if (pTextViewport)
        pTextViewport->AddViewportChild(descriptionPointer.m_pObject);
    screen->RegisterInputComponent(descriptionPointer.m_pObject);
    CComponentPointer subtitlePointer(frontEnd->CreateComponent("PC_SUBTITLE", false));
    subtitlePointer->SetText(description, true, true);
    entry->AddChild(subtitlePointer);
    screen->RegisterInputComponent(subtitlePointer.m_pObject);
    list->AddItem(entry);
}