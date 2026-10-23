// CTCInventory::OpenPCMapMenu() -- switches the PC inventory to the map screen (6): finds or builds the map menu component and hooks its scroll and back/close clickables to the input observer
// Grade: DIFFER(549v549) -- one reload-order swap after the inline strcmp's repe cmpsb: retail reloads esi (this) before edi (index), ours edi first (RTM and QFE alike)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00589A30, 549 bytes; size TU without a frame pointer (`optimize("s", on)`,
// as the landed ConstructStyleCardsScreenPC 0x0058A729 and CTCInventory ctor 0x00589C55).
// Callees: DeleteMenuComponents 0x00587A41, CFrontEndManager::GetInstance 0x0041E5F2 (__fastcall; the first call's
// result is unused), the component's GetName (slot 84, by value; CBasicString operator==(const char*) 0x004115A0,
// ~CCharString 0x0099EAE0), std::bitset::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020,
// CTCInventoryMap::BuildMapMenu 0x0060EEB0, vector<CComponent*>::push_back 0x00486024, CCharString(const char*, long)
// 0x0099EBF0 (FindChild's by-value name), the observer slot 3 (ObserveEvent 0x19) and the manager's slot 2 (AddObserver).
// Layout: retail CTCInventory (12-byte vectors): ParentThing +0x04, Components +0x0C, ActivePCScreen +0x21C; CThing
// interface bitset +0x20, map +0x44 (as the landed OpenPCScoreboard 0x00587B93).
#pragma optimize("s", on)
#include <string.h>

#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;          // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
namespace NUISystem {
struct CObserver {                                    // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);        // slot 3 (+0x0C)
};
struct CComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2) V10(v3)
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetState(long state);                // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual CCharString GetName() const;              // slot 84 (+0x150)
};
struct CComponent : CComponentBase, CObserver {
};
struct CFrontEndManager {
    virtual void m00(); virtual void m01();
    virtual void AddObserver(CObserver* observer);    // slot 2 (+0x08)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}

struct CComponentVector {                             // retail std::vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** m_first;
    NUISystem::CComponent** m_last;
    NUISystem::CComponent** m_end;
    unsigned long size() const { return m_last - m_first; }
    NUISystem::CComponent*& operator[](unsigned long i) { return m_first[i]; }
    void push_back(NUISystem::CComponent* const& value);   // 0x00486024
};
#pragma auto_inline(off)
__declspec(noinline) void CComponentVector::push_back(NUISystem::CComponent* const& value)
{
    if (m_last != m_end)
        *m_last++ = value;
}
#pragma auto_inline(on)

struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                              // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                    // +0x00
    SEntry* last;                                     // +0x04
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                          // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                       // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)(it == map->last || wanted < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCInventoryMap {                              // interface type 0x1B
    NUISystem::CComponent* BuildMapMenu(long a, long b);   // 0x0060EEB0
};

struct CTCInventory {
    void*            __vftable;                       // +0x00
    CThing*          ParentThing;                     // +0x04
    unsigned char    _pad_0x08[0x04];
    CComponentVector Components;                      // +0x0C
    unsigned char    _pad_0x18[0x204];
    long             ActivePCScreen;                  // +0x21C
    void DeleteMenuComponents();                      // 0x00587A41
    void OpenPCMapMenu();
};

void CTCInventory::OpenPCMapMenu()
{
    DeleteMenuComponents();
    ActivePCScreen = 6;
    NUISystem::CComponent* mapMenu = 0;
    NUISystem::CFrontEndManager* frontEnd = NUISystem::CFrontEndManager::GetInstance();
    for (unsigned long i = 0; i < Components.size() && !mapMenu; ++i) {
        if (Components[i]->GetName() == "PC_MAP_LIST")
            mapMenu = Components[i];
    }
    if (!mapMenu) {
        CTCInventoryMap* map = 0;
        ParentThing->GetInterface(&map, 0x1B);
        mapMenu = map->BuildMapMenu(0, 0);
        if (!mapMenu)
            return;
        mapMenu->Initialise();
        Components.push_back(mapMenu);
    }
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_LIST_UP", -1)))
        c->ObserveEvent(0x19);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_DESCRIPTION_UP", -1)))
        c->ObserveEvent(0x19);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_LIST_DOWN", -1)))
        c->ObserveEvent(0x19);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("SCROLL_DESCRIPTION_DOWN", -1)))
        c->ObserveEvent(0x19);
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("PC_BACK_CLICKABLE", -1))) {
        NUISystem::CFrontEndManager::GetInstance()->AddObserver(c);
        c->ObserveEvent(0x19);
    }
    if (NUISystem::CComponent* c = mapMenu->FindChild(CCharString("PC_CLOSE_CLICKABLE", -1))) {
        NUISystem::CFrontEndManager::GetInstance()->AddObserver(c);
        c->ObserveEvent(0x19);
    }
    mapMenu->SetState(5);
}
