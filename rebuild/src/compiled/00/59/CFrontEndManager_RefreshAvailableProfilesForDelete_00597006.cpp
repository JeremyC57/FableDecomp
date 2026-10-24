// NUISystem::CFrontEndManager::RefreshAvailableProfilesForDelete() -- rebuild the delete-profile screen's list: one button per saved profile, each carrying a "delete this profile" action; returns the profile count
// Leaf: ?RefreshAvailableProfilesForDelete@CFrontEndManager@NUISystem@@
// Retail: Fable.exe @ 0x00597006, 546 bytes, `ret`; frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`).
// Body: GetProfileNames(names) (0x005955AB, landed); the delete screen (m_mapScreens[21], map operator[] 0x0059B5D7)
// drops its old "UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE" child (FindChild, slot 3, CCharString by value; the child gets a
// type-1 CAction by value through slot 62: CAction 0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA); a new list of that
// definition comes from CManager::CreateComponent (0x0041DB1D on CFrontEndManager::GetInstance 0x0041E5F2) into a counted
// pointer (0x00429C15), converted to CCountedPointer<CList> (ShareData 0x0059AA86) and added to the screen (AddChild, slot 59).
// Per profile name: CreateFrontEndButtonComponent(name, y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false) (0x00596A68,
// landed), a new CAction (0x10 bytes, ctor 0x0042BE50, counted 0x0042AA29) of type 0xD7 holding a new CActionParamString(name)
// (0x0054DAE0) pushed on its UIActionParams (0x0042B646), given to the button (slots 66 and 71) and the button added to
// the list (slot 60); y advances by 30. Finally the screen's slot 43, the counted pointers' dtors (0x0059A994, 0x00429160,
// 0x004291DE) and ~vector<CWideString> (0x00412130).
// Layout: engine/CFrontEndManager.h m_mapScreens +0x54 (local model of the retail class).
// Shape: retail walks the names with a pointer plus a down counter, keeps the list's Data in ebx and shares the screen
// key / old-child / counter frame word; VC7.1 does that only when every callee taking a local's address is a same-TU
// body that never stores it: CListPointer ShareData/Release, the screen map operator[] (key read by value),
// GetProfileNames and ~vector<CWideString> (stand-in bodies below). With them declared only: 540v546 (frame 0x44,
// index loop reloading the names vector), 546v546 with the names stand-ins alone (key in its own word).
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace NUISystem {
class CComponent;
class CList;
class CActionParam;
class CActionParamString;
struct CActionParamList {                                   // STLport list<CActionParam*> (one node pointer)
    void* _M_node;
    void push_back(CActionParam* const& x);                 // 0x0042B646
};
struct CActionParamListOwned {                              // the 8-byte action's list (destroyed out of line)
    void* _M_node;
    ~CActionParamListOwned();                               // 0x0042ABCA
};
namespace N_0042bed4 { struct CAction {                     // retail 8 bytes: ActionType +0, UIActionParams +4
    long                  ActionType;
    CActionParamListOwned UIActionParams;
    CAction();                                              // 0x0042BED4
    CAction(const CAction& other);                          // 0x0042CD84
}; }
class CAction {                                             // the 16-byte counted action (ctor 0x0042BE50)
public:
    long             ActionType;                            // +0x00
    CActionParamList UIActionParams;                        // +0x04
    void*            ThingActionParams;                     // +0x08
    bool             Valid;                                 // +0x0c
    CAction();                                              // 0x0042BE50
};
class CActionParamString {                                  // 12 bytes: vptr, ActionParamType, Value
public:
    void*       __vftable;
    long        ActionParamType;
    CWideString Value;
    CActionParamString(const CWideString& value);           // 0x0054DAE0
};
}
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);
    ~CCountedPointer();
    template <class U> __forceinline CCountedPointer(const CCountedPointer<U>& other) : Data(0), Info(0)
    {
        ShareData((T*)other.Data, other.Info);
    }
    void ShareData(T* data, CCPPointerInfo* info);
    void Release();
    T* operator->() const { return Data; }
};
namespace NUISystem { class CList; }
// CCountedPointer<NUISystem::CList> (the instance the linker kept at 0x0059AA86 ShareData / 0x0059A994 Release): its
// members are defined in this TU (out of line), so VC7.1 knows the list pointer never escapes and keeps Data in ebx.
struct CListPointer {
    NUISystem::CList* Data;
    CCPPointerInfo*   Info;
    __forceinline CListPointer(const CCountedPointer<NUISystem::CComponent>& other) : Data(0), Info(0)
    {
        ShareData((NUISystem::CList*)other.Data, other.Info);
    }
    __forceinline ~CListPointer() { Release(); }
    void ShareData(NUISystem::CList* data, CCPPointerInfo* info);
    void Release();
    NUISystem::CList* operator->() const { return Data; }
};
#pragma auto_inline(off)
void CListPointer::Release()
{
    if (Info) {
        if (--Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data = 0;
    Info = 0;
}
void CListPointer::ShareData(NUISystem::CList* data, CCPPointerInfo* info)
{
    if (Info != info) {
        Release();
        Data = data;
        Info = info;
        if (info)
            ++info->RefCount;
    }
}
#pragma auto_inline(on)
namespace NUISystem {
class CComponent {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);            // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                                   // slot 43 (+0xac)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59 (+0xec)
    virtual void AddChildAfterFocus(CCountedPointer<CComponent>& child);   // slot 60 (+0xf0) (name descriptive)
    virtual void v61();
    virtual void SetAction(N_0042bed4::CAction action);          // slot 62 (+0xf8) (name descriptive)
    virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetSelectAction(CCountedPointer<CAction>& action);   // slot 66 (+0x108) (name descriptive)
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual void SetDeleteAction(CCountedPointer<CAction>& action);   // slot 71 (+0x11c) (name descriptive)
};
class CList : public CComponent {
};
class CManager {
public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
enum FRONTEND_SCREEN { FRONTEND_SCREEN_DELETE_PROFILE = 21 };
struct CScreenMap {                                         // map<FRONTEND_SCREEN, CComponent*> (12 bytes)
    void* _M_header; unsigned long _M_node_count; long _M_key_compare;
    CComponent*& operator[](const FRONTEND_SCREEN& key);    // 0x0059B5D7
};
struct CWideStringVector {                                  // vector<CWideString> (12 bytes)
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CWideStringVector();                                   // 0x00412130
    unsigned long size() const { return _M_finish - _M_start; }
    CWideString& operator[](unsigned long n) { return _M_start[n]; }
};
class CFrontEndManager : public CManager {
public:
    static CFrontEndManager* __fastcall GetInstance();      // 0x0041E5F2
    void GetProfileNames(CWideStringVector& names);         // 0x005955AB
    CComponent* CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef,
                                              const CCharString& buttonDef, bool flag);   // 0x00596A68
    unsigned long RefreshAvailableProfilesForDelete();
    unsigned char _pad00[0x54];
    CScreenMap    m_mapScreens;                             // +0x54
};
// Same-TU stand-in for GetProfileNames 0x005955AB (body is not the retail one): it only writes the vector's words and
// never stores its address, so the names vector stays unaliased across the loop's calls.
CWideString* ProfileNames_005955AB(CFrontEndManager* manager, unsigned long* count);
void DestroyWideStrings_00412130(CWideString* first, CWideString* last);
CComponent** ScreenSlot_0059B5D7(CScreenMap* map, long key);
#pragma auto_inline(off)
__declspec(noinline) CComponent*& CScreenMap::operator[](const FRONTEND_SCREEN& key)
{
    return *ScreenSlot_0059B5D7(this, key);
}
__declspec(noinline) CWideStringVector::~CWideStringVector()
{
    DestroyWideStrings_00412130(_M_start, _M_finish);
}
__declspec(noinline) void CFrontEndManager::GetProfileNames(CWideStringVector& names)
{
    unsigned long count;
    CWideString* first = ProfileNames_005955AB(this, &count);
    names._M_start = first;
    names._M_finish = first + count;
    names._M_end_of_storage = first + count;
}
#pragma auto_inline(on)
unsigned long CFrontEndManager::RefreshAvailableProfilesForDelete()
{
    CWideStringVector names;
    GetProfileNames(names);
    CFrontEndManager* manager = GetInstance();
    CComponent* screen = m_mapScreens[FRONTEND_SCREEN_DELETE_PROFILE];
    CComponent* oldList = screen->FindChild("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE");
    if (oldList) {
        N_0042bed4::CAction close;
        close.ActionType = 1;
        oldList->SetAction(close);
    }
    CCountedPointer<CComponent> listComponent(manager->CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE", false));
    CListPointer list(listComponent);
    screen->AddChild(listComponent);
    float y = 0.0f;
    const unsigned long count = names.size();
    for (unsigned long i = 0; i < count; ++i) {
        CCountedPointer<CComponent> button(CreateFrontEndButtonComponent(names[i], y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> action(new CAction());
        CActionParam* param = (CActionParam*)new CActionParamString(names[i]);
        action->ActionType = 0xd7;
        action->UIActionParams.push_back(param);
        button->SetSelectAction(action);
        button->SetDeleteAction(action);
        list->AddChildAfterFocus(button);
        y += 30.0f;
    }
    screen->Initialise();
    return count;
}
}
