// CTCInventory::GetStyleCardsListUIComponent(long hairCount, long facialHairCount, long tattooCount, CComponent* parent) -- sort the hero's non-augmentation items into haircut, facial hair and tattoo style cards (hair cards by their style's body-area flags, tattoo cards as tattoos), fill the three style lists of the parent menu, put each list's description into the text viewport and title each list's button and sub-title "<text> x<count>"
// Grade: DIFFER(1660v1678) -- block layout only: retail emits the hair-card arm of each category block after the shared def releases (case 4) / after the function epilogue (default) and jumps back; ours keeps it inline. Releases, slots, switch and callees as retail
// Leaf: ?GetStyleCardsListUIComponent@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x005850FD, 1678 bytes, `ret 10h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (result unused), CTCInventoryBase::GetAllItems 0x005BF482, the item-kind tests
//   0x005D9970 / 0x005D9830, GetInventoryCategory 0x005D96F0, GetHairCardDef 0x005D9A10 / GetTattooCardDef 0x005D9AA0 (__fastcall,
//   hidden result), CTCBase::GetDefinitionManager 0x006869D0, GetPDefFromGlobalIndex 0x009AD9E0, GetSubDef 0x0057FF92, vector
//   erase 0x00593139 / push_back 0x005947B6, ConstructStyleEntry 0x00582ACE, CWideString() 0x0099AED0 / operator= 0x0099B7D0 /
//   += 0x0099B8D0 / ~ 0x0099B510, GetXString 0x005CC772, IntToWideString 0x00405F00, free 0x00BFEA14. Slot names are descriptive.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void free(void*);
typedef unsigned short wchar_t;
struct CWideString {
    void* m_pStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall GetXString();                    // 0x005CC772
CWideString __fastcall IntToWideString(long value);     // 0x00405F00
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {                 // a counted def reference returned by value
    T* m_p;
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
    T* operator->() const { return m_p; }
};
template <class T> struct CDefOutPointer {              // a counted def reference filled through an out pointer
    T* m_p;
    CDefOutPointer() : m_p(0) { }
    __forceinline ~CDefOutPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
struct CStyleDef : CDefBase {
    unsigned char _pad08[0x40 - 0x08];
    long CoversBodyAreaFlags;                           // +0x40
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefOutPointer<CStyleDef>* out) const;   // 0x0057FF92
};
void* FindSubDef(const CDefClassBase* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for the GetSubDef template instance (retail's COMDAT sits in this TU at 0x0057FF92): CDefPointer::operator= on
// the out pointer, so VC7.1 knows the out pointer's address does not escape the call.
bool CDefClassBase::GetSubDef(CDefOutPointer<CStyleDef>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CTattooCardDef : CDefBase { };
struct CHairCardDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    long StyleDefIndex;                                 // +0x28
};
CDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);   // 0x005D9AA0
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);       // 0x005D9A10
bool __fastcall IsAugmentation(long itemType);          // 0x005D9970
bool __fastcall IsHiddenItem(long itemType);            // 0x005D9830
long __fastcall GetInventoryCategory(long itemType);    // 0x005D96F0
struct CDefinitionManager {
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
};

struct SStyleEntry {                                    // pair<long, long>
    long first;
    long second;
};
struct CStyleVector {                                   // STLport vector<pair<long, long> >
    SStyleEntry* _M_start;
    SStyleEntry* _M_finish;
    SStyleEntry* _M_end_of_storage;
    CStyleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CStyleVector() { if (_M_start) free(_M_start); }
    SStyleEntry* begin() { return _M_start; }
    SStyleEntry* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    SStyleEntry& operator[](unsigned long i) { return _M_start[i]; }
    SStyleEntry* erase(SStyleEntry* pos);               // 0x00593139
    void push_back(const SStyleEntry& x);               // 0x005947B6
};

namespace NUISystem {
struct CComponent;
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* GetChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141();
    virtual void AddViewportChild(CComponent* child);   // slot 142 (+0x238)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
struct CComponent : CComponentBase {};
struct CTextComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual const CWideString& GetText();   // slot 142 (+0x238)
};
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
using namespace NUISystem;

struct CTCInventory {
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0 (CTCBase)
    void GetAllItems(CStyleVector* items);              // 0x005BF482 (CTCInventoryBase)
    void ConstructStyleEntry(CComponent& list, const CStyleVector& styles, const CCharString& headerDefinition, CComponentBase* viewport);   // 0x00582ACE
    void GetStyleCardsListUIComponent(long hairCount, long facialHairCount, long tattooCount, CComponent* parent);
};

void CTCInventory::GetStyleCardsListUIComponent(long hairCount, long facialHairCount, long tattooCount, CComponent* parent)
{
    CFrontEndManager::GetInstance();
    CStyleVector items;
    CStyleVector haircuts;
    CStyleVector facialHair;
    CStyleVector tattoos;
    GetAllItems(&items);
    for (SStyleEntry* it = items.begin(); it != items.end(); ) {
        if (IsAugmentation(it->first))
            it = items.erase(it);
        else
            ++it;
    }
    for (unsigned long i = 0; i < items.size(); ++i) {
        if (IsHiddenItem(items[i].first))
            continue;
        switch (GetInventoryCategory(items[i].first)) {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4: {
            CDefPointer<CHairCardDef> hair = GetHairCardDef(items[i].first);
            CDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(items[i].first);
            if (!hair.m_p) {
                if (tattoo.m_p)
                    tattoos.push_back(items[i]);
            } else {
                CDefPointer<CDefClassBase> def = GetDefinitionManager()->GetPDefFromGlobalIndex(hair->StyleDefIndex);
                CDefOutPointer<CStyleDef> style;
                if (def.m_p) {
                    def->GetSubDef(&style);
                    if (style.m_p) {
                        long flags = style->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facialHair.push_back(items[i]);
                        else if (flags & 0x10)
                            haircuts.push_back(items[i]);
                    }
                }
            }
            break;
        }
        default: {
            CDefPointer<CHairCardDef> hair = GetHairCardDef(items[i].first);
            CDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(items[i].first);
            if (!hair.m_p) {
                if (tattoo.m_p)
                    tattoos.push_back(items[i]);
            } else {
                CDefPointer<CDefClassBase> def = GetDefinitionManager()->GetPDefFromGlobalIndex(hair->StyleDefIndex);
                CDefOutPointer<CStyleDef> style;
                if (def.m_p) {
                    def->GetSubDef(&style);
                    if (style.m_p) {
                        long flags = style->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            facialHair.push_back(items[i]);
                        else if (flags & 0x10)
                            haircuts.push_back(items[i]);
                    }
                }
            }
            break;
        }
        }
    }
    CWideString text;
    CComponent* textArea = parent->GetChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");

    CComponent* entry = parent->GetChild("UI_MENU_ENTRY_HAIRCUTS");
    ConstructStyleEntry(*entry, haircuts, "UI_HAIRCUTS_TITLE", parent);
    textArea->AddViewportChild(entry->GetChild("UI_HAIRCUTS_DESC"));
    CComponent* button = entry->GetChild("UI_HAIRCUTS_BUTTON")->GetChild("UI_TEXT_HAIRCUTS");
    text = ((CTextComponent*)button)->GetText();
    text += GetXString();
    text += IntToWideString(hairCount);
    button->SetText(text, true, true);
    CComponent* subTitle = entry->GetChild("UI_HAIRCUTS_SUB_TITLE");
    if (subTitle)
        subTitle->SetText(text, true, true);

    entry = parent->GetChild("UI_MENU_ENTRY_FACIAL_HAIR");
    ConstructStyleEntry(*entry, facialHair, "UI_FACIAL_HAIR_TITLE", parent);
    textArea->AddViewportChild(entry->GetChild("UI_FACIAL_HAIR_DESC"));
    button = entry->GetChild("UI_FACIAL_HAIR_BUTTON")->GetChild("UI_TEXT_FACIAL_HAIR");
    text = ((CTextComponent*)button)->GetText();
    text += GetXString();
    text += IntToWideString(facialHairCount);
    button->SetText(text, true, true);
    subTitle = entry->GetChild("UI_FACIAL_HAIR_SUB_TITLE");
    if (subTitle)
        subTitle->SetText(text, true, true);

    entry = parent->GetChild("UI_MENU_ENTRY_TATTOOS");
    ConstructStyleEntry(*entry, tattoos, "UI_TATTOOS_TITLE", parent);
    textArea->AddViewportChild(entry->GetChild("UI_TATTOOS_DESC"));
    button = entry->GetChild("UI_TATTOOS_BUTTON")->GetChild("UI_TEXT_TATTOOS");
    text = ((CTextComponent*)button)->GetText();
    text += GetXString();
    text += IntToWideString(tattooCount);
    button->SetText(text, true, true);
    subTitle = entry->GetChild("UI_TATTOOS_SUB_TITLE");
    if (subTitle)
        subTitle->SetText(text, true, true);
}
