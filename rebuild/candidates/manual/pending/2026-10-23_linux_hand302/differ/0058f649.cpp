// CTCInventory::OpenPCInventory() -- open the PC inventory: fade the three sound groups, stop the inventory sound component, prepare the screen, create the mouse pointer and top-level list, set up the skills and photojournal entries (arrow action, or a disabled-entry action when unavailable), register with the front end, reset the screens and open the base inventory
// Grade: DIFFER(883v883) -- full body modelled; residue: (1) the front-end manager takes [ebp-8] and the name temporaries [ebp-4] (retail swaps them), (2) the PTop_Level_List address (edi) is formed before the second name ctor, (3) in the photojournal arrow block (no null test) the action.first = 1 store is scheduled above the vtable load (retail loads the vtable first; the null-tested skills arrow block matches) (6 grades)
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class CThing;
class CTCBase;
struct CPresenceBits { bool test(long kind); };         // bitset<>::test 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                              // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) { return present.test(type); }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CTCInventorySoundPlayer {                        // interface type 0x9D
    void Stop();                                        // 0x007C8FD0
};
struct SComponentListNode;
struct CComponentList {
    SComponentListNode* m_pNode;
    ~CComponentList();                                  // 0x0042ABCA
};
struct SActionParamPair {                               // pair<unsigned long, list<...> > (8 bytes)
    unsigned long  first;
    CComponentList second;
    SActionParamPair();                                 // 0x0042BED4
    SActionParamPair(const SActionParamPair& other);    // 0x0042CD84
};
namespace NUISystem {
struct CAction {                                        // retail 0x10 bytes
    long ActionType;                                    // +0x00
    unsigned char _pad04[0x0c];
    CAction();                                          // 0x0042BE50
};
}
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x0042AA29
    ~CCountedPointer();                                 // 0x00429160
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
using ::CCountedPointer;
struct CComponent {
    virtual void c000(); virtual void c001(); virtual void c002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0xC)
    virtual void c004(); virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014(); virtual void c015();
    virtual void c016(); virtual void c017(); virtual void c018(); virtual void c019(); virtual void c020(); virtual void c021();
    virtual void c022(); virtual void c023(); virtual void c024(); virtual void c025(); virtual void c026(); virtual void c027();
    virtual void c028(); virtual void c029(); virtual void c030(); virtual void c031(); virtual void c032(); virtual void c033();
    virtual void c034(); virtual void c035(); virtual void c036(); virtual void c037(); virtual void c038(); virtual void c039();
    virtual void c040(); virtual void c041(); virtual void c042();
    virtual void Reset();                                      // slot 43 (+0xAC)
    virtual void c044(); virtual void c045(); virtual void c046(); virtual void c047();
    virtual void SetState(long state);                         // slot 48 (+0xC0)
    virtual void c049(); virtual void c050(); virtual void c051(); virtual void c052(); virtual void c053(); virtual void c054();
    virtual void c055(); virtual void c056(); virtual void c057(); virtual void c058(); virtual void c059(); virtual void c060();
    virtual void c061();
    virtual void DoAction(SActionParamPair action);            // slot 62 (+0xF8)
    virtual void c063(); virtual void c064(); virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068();
    virtual void AddAction(const CCountedPointer<CAction>& action); // slot 69 (+0x114)
    virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073(); virtual void c074(); virtual void c075();
    virtual void c076(); virtual void c077(); virtual void c078(); virtual void c079(); virtual void c080(); virtual void c081();
    virtual void c082(); virtual void c083(); virtual void c084(); virtual void c085(); virtual void c086(); virtual void c087();
    virtual void c088(); virtual void c089(); virtual void c090(); virtual void c091();
    virtual void Highlight();                                  // slot 92 (+0x170)
};
struct CManager {
    virtual void m000(); virtual void m001(); virtual void m002(); virtual void m003(); virtual void m004(); virtual void m005();
    virtual void m006(); virtual void m007(); virtual void m008(); virtual void m009(); virtual void m010(); virtual void m011();
    virtual void m012(); virtual void m013(); virtual void m014(); virtual void m015(); virtual void m016(); virtual void m017();
    virtual void m018(); virtual void m019(); virtual void m020();
    virtual void OnInventoryScreenOpened(void* screen);        // slot 21 (+0x54)
    virtual void m022(); virtual void m023(); virtual void m024(); virtual void m025();
    virtual void OnThingInventoryOpened(CThing* thing);        // slot 26 (+0x68)
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}
struct CHudOwner {
    virtual void h000(); virtual void h001(); virtual void h002(); virtual void h003(); virtual void h004(); virtual void h005();
    virtual void h006(); virtual void h007(); virtual void h008(); virtual void h009(); virtual void h010(); virtual void h011();
    virtual void h012(); virtual void h013(); virtual void h014(); virtual void h015(); virtual void h016(); virtual void h017();
    virtual void h018(); virtual void h019(); virtual void h020(); virtual void h021(); virtual void h022(); virtual void h023();
    virtual void h024(); virtual void h025(); virtual void h026(); virtual void h027(); virtual void h028(); virtual void h029();
    virtual void h030(); virtual void h031(); virtual void h032(); virtual void h033(); virtual void h034(); virtual void h035();
    virtual void h036(); virtual void h037(); virtual void h038(); virtual void h039(); virtual void h040(); virtual void h041();
    virtual void h042(); virtual void h043(); virtual void h044(); virtual void h045(); virtual void h046(); virtual void h047();
    virtual void h048(); virtual void h049(); virtual void h050(); virtual void h051(); virtual void h052(); virtual void h053();
    virtual void h054(); virtual void h055(); virtual void h056(); virtual void h057(); virtual void h058(); virtual void h059();
    virtual void h060(); virtual void h061(); virtual void h062(); virtual void h063(); virtual void h064(); virtual void h065();
    virtual void h066(); virtual void h067(); virtual void h068(); virtual void h069(); virtual void h070(); virtual void h071();
    virtual void h072(); virtual void h073(); virtual void h074(); virtual void h075();
    virtual void SetEnabled(bool enabled);                     // slot 76 (+0x130)
};
struct CWorldPart {
    unsigned char _pad_0x00[0x2c];
    CHudOwner* Hud;                                     // +0x2c
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x28];
    CWorldPart* World;                                  // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CSoundSystem {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005();
    virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011();
    virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017();
    virtual void s018(); virtual void s019(); virtual void s020(); virtual void s021(); virtual void s022(); virtual void s023();
    virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029();
    virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035();
    virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041();
    virtual void s042(); virtual void s043(); virtual void s044(); virtual void s045();
    virtual void RefreshCategory(long category, float a, float b); // slot 46 (+0xB8)
};
extern CSoundSystem* g_pSoundSystem;                    // 0x013B8394
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(NUISystem::CComponent* const& value);   // 0x00486024
};
// Layout: retail CTCInventory Components +0x0C (CTCInventoryBase), +0x140 pending item, PMouse_Pointer +0x214,
//   PTop_Level_List +0x218, PCInventoryState +0x21C, fade time +0x220.
struct CTCInventory {
    void*            __vftable;                         // +0x000
    CThing*          ParentThing;                       // +0x004
    unsigned char    _pad08[0x0c - 0x08];
    CComponentVector Components;                        // +0x00c
    unsigned char    _pad18[0x140 - 0x18];
    long             PendingItem;                       // +0x140
    unsigned char    _pad144[0x214 - 0x144];
    NUISystem::CComponent* PMouse_Pointer;              // +0x214
    NUISystem::CComponent* PTop_Level_List;             // +0x218
    long             PCInventoryState;                  // +0x21c
    float            AccumFadeTime;                     // +0x220
    void PrepareScreen();                               // 0x005BC6ED (CTCInventoryBase)
    void OpenInventory();                               // 0x005BD4EA (CTCInventoryBase)
    bool IsPhotojournalDisabled();                      // 0x00581B33
    void SelectPendingItem(long item);                  // 0x0058E99F
    void OpenPCInventory();
};

void CTCInventory::OpenPCInventory()
{
    if (g_pSoundSystem) {
        g_pSoundSystem->RefreshCategory(0x10, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x40, 0.0f, 500.0f);
        g_pSoundSystem->RefreshCategory(0x80, 0.0f, 500.0f);
    }
    CThing* parent = ParentThing;
    if (parent->HasTC(0x9d))
        parent->FindTC<CTCInventorySoundPlayer>(0x9d)->Stop();
    PCInventoryState = 0;
    PrepareScreen();
    g_pMainGameComponent->World->Hud->SetEnabled(false);
    AccumFadeTime = 0.0f;
    NUISystem::CComponent** list;
    {
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    PMouse_Pointer = frontEnd->CreateComponent("UI_MOUSE_POINTER", false);
    list = &PTop_Level_List;
    *list = frontEnd->CreateComponent("PC_TOP_LEVEL_LIST", false);
    NUISystem::CComponent* entry = (*list)->FindChild("PC_MENU_ENTRY_SKILLS");
    if (entry) {
        if (!ParentThing->HasTC(0x6f)) {
            entry->Highlight();
            NUISystem::CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW");
            if (arrow) {
                SActionParamPair action;
                action.first = 1;
                arrow->DoAction(action);
            }
        } else {
            CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
            action->ActionType = 0xea;
            entry->AddAction(action);
        }
    }
    entry = (*list)->FindChild("PC_MENU_ENTRY_PHOTOJOURNAL");
    if (entry) {
        if (!IsPhotojournalDisabled()) {
            entry->Highlight();
            NUISystem::CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW");
            SActionParamPair action;
            action.first = 1;
            arrow->DoAction(action);
        } else {
            CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
            action->ActionType = 0xfa0;
            entry->AddAction(action);
        }
    }
    frontEnd->OnInventoryScreenOpened(this);
    frontEnd->OnThingInventoryOpened(ParentThing);
    }
    Components.push_back(PMouse_Pointer);
    Components.push_back(*list);
    for (unsigned long i = 0; i < Components.size(); ++i)
        Components[i]->Reset();
    (*list)->SetState(5);
    OpenInventory();
    g_pMainGameComponent->World->Hud->SetEnabled(false);
    long& pending = PendingItem;
    if (pending != -1) {
        SelectPendingItem(pending);
        pending = -1;
    }
}
