// NUISystem::CFrontEndManager::RefreshAvailableProfiles() -- rebuild the profile menu's list: a "new profile" button and one button per saved profile; returns the profile count
// Retail 0x00596CCE, 824 bytes, plain `ret` (Ego_r.pdb ?RefreshAvailableProfiles@CFrontEndManager@NUISystem@@IAEJXZ).
// Body: profiles = GetProfileNames() (0x005955AB, local vector<CWideString>, dtor 0x00412130); on the profile menu screen
// (m_mapScreens[7], map::operator[] 0x0059B5D7) the old "UI_FRONTEND_LIST_FOR_PROFILES" child (FindChild, slot 3, by-value
// CCharString) gets SActionParamPair{1} through SetActionParameter (slot 62; pair ctor 0x0042BED4, copy 0x0042CD84, list
// dtor 0x0042ABCA); a fresh list of that definition is created (CManager::GetInstance 0x0041E5F2 -> CreateComponent
// 0x0041DB1D), viewed as a CList (CCountedPointer ShareData 0x0059AA86, dtor 0x0059A994) and added to the menu (AddChild,
// slot 59). A "new profile" button (text CWideString::CreateFromCharString("TEXT_GUI_MENU_NEW_PROFILE") 0x0099B6A0, the
// PDB's static __fastcall widening of the tag itself -- not the JustGetTheFrikkingText2 0x0057AD4C text lookup;
// CreateFrontEndButtonComponent 0x00596A68 with "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST" at y 0)
// carries a CAction of type 0x125 (operator new 0x00BFEA1A, CAction 0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / dtor
// 0x00429160) as its action and left-click action (slots 66 / 71) and is added to the list (AddChildDynamically, slot
// 60). Each profile then gets a button at y = 50 + 30 * i whose action is 0x124 when the profile's save passes the
// signature check (VerifySignatureCached 0x005957D9 of CTBaseSingleton<CUserProfileManager>::Get() 0x0040D2A0
// .GetProfileDir(name, false) 0x004082D0) and 0xDB otherwise, with the profile name as a CActionParamString parameter
// (0x0054DAE0; UIActionParams push_back 0x0042B646). Finally the menu's slot 43 re-initialises it.
// Literals: 0x01252428, 0x0125240C, 0x012523E4, "" 0x0122D70E; 30.0f 0x01248018.
// Opt: size TU with a frame pointer (`optimize("s")` + `("y",off)`), as the landed CFrontEndManager rows.
// Shape: three same-TU noinline stand-ins (COMDATs / same-TU callees in retail) give VC7.1 retail's escape knowledge:
// GetProfileNames (so the profile loop is strength-reduced onto an element pointer and a down counter), the
// vector<CWideString> destructor, and map::operator[] (so the screen-key temporary's word is shared with the old-list
// pointer and the loop counter: frame 0x64, not 0x68); `y` is declared before `count` for retail's store order.
// Leaf: ?RefreshAvailableProfiles@CFrontEndManager@NUISystem
#pragma optimize("s", on)
#pragma optimize("y", off)

void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};

class CWideString
{
public:
    void* PStringData;
    ~CWideString();                                       // 0x0099B510
    static CWideString __fastcall CreateFromCharString(const CCharString& text);   // 0x0099B6A0
};
extern "C" void __cdecl free(void* block);

namespace std {
template <class T> class vector
{
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector();                                            // 0x00412130 for CWideString
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
template <class K, class V> class map
{
public:
    void* _body[3];
    V& operator[](const K& key);                          // 0x0059B5D7 for <FRONTEND_SCREEN, CComponent*>
};
}

#pragma auto_inline(off)
// Same-TU body of the vector<CWideString> destructor COMDAT (0x00412130): destroy the names, free the block.
template <> __declspec(noinline) std::vector<CWideString>::~vector()
{
    for (CWideString* name = _M_start; name != _M_finish; ++name)
        name->~CWideString();
    if (_M_start)
        free(_M_start);
}
#pragma auto_inline(on)
extern std::vector<CWideString> g_standInProfileScan;    // stand-in data only (no retail counterpart)

class CCPPointerInfo;
template <class T> class CCountedPointer
{
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);                             // 0x00429C15 (CComponent) / 0x0042AA29 (CAction)
    template <class U> __forceinline CCountedPointer(const CCountedPointer<U>& other) : Data(0), Info(0)
    {
        ShareData((T*)other.Data, other.Info);
    }
    ~CCountedPointer();                                   // 0x004291DE (CComponent) / 0x00429160 (CAction) / 0x0059A994 (CList)
    void ShareData(T* data, CCPPointerInfo* info);        // 0x0059AA86 for CList
    T* operator->() const { return Data; }
};

struct CComponentList { void* m_pNode; ~CComponentList(); };   // 0x0042ABCA
struct SActionParamPair {
    unsigned long  first;
    CComponentList second;
    SActionParamPair();                                   // 0x0042BED4
    SActionParamPair(const SActionParamPair& other);      // 0x0042CD84
};

class CUserProfileManager
{
public:
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
};
template <class T> class CTBaseSingleton
{
public:
    static T& __fastcall Get();                           // 0x0040D2A0 for CUserProfileManager
};

enum FRONTEND_SCREEN { FRONTEND_SCREEN_PROFILE_MENU = 7 };

namespace NUISystem {
class CActionParam;
class CActionParamString
{
public:
    unsigned char _body[0x0c];
    CActionParamString(const CWideString& text);          // 0x0054DAE0
};
class CActionParamList
{
public:
    void* m_pNode;
    void push_back(CActionParam* const& value);           // 0x0042B646
};
class CAction
{
public:
    long             ActionType;                          // +0x00
    CActionParamList UIActionParams;                      // +0x04
    void*            ThingActionParams;                   // +0x08
    bool             Valid;                               // +0x0C
    CAction();                                            // 0x0042BE50
};

class CComponent
{
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);      // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                            // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(CCountedPointer<CComponent>& child);              // slot 59 (+0xEC)
    virtual void AddChildDynamically(CCountedPointer<CComponent>& child);   // slot 60 (+0xF0, CList's override)
    virtual void v61();
    virtual void SetActionParameter(SActionParamPair parameter);            // slot 62 (+0xF8)
    virtual void v63(); virtual void v64(); virtual void v65();
    virtual void AddAction(CCountedPointer<CAction>& action);               // slot 66 (+0x108)
    virtual void AddActionOnSelected(CCountedPointer<CAction>& action);     // slot 67
    virtual void AddActionOnBack(CCountedPointer<CAction>& action);         // slot 68
    virtual void AddPreAction(CCountedPointer<CAction>& action);            // slot 69
    virtual void AddActionOnInactive(CCountedPointer<CAction>& action);     // slot 70
    virtual void AddActionOnLeftClicked(CCountedPointer<CAction>& action);  // slot 71 (+0x11C)
};
class CList : public CComponent {};

class CManager
{
public:
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();            // 0x0041E5F2
};


}   // namespace NUISystem (break for the std specialisation)
NUISystem::CComponent*& __fastcall ScreenSlotStandIn(void* map, long key);   // stand-in helper (no retail counterpart)

#pragma auto_inline(off)
// Same-TU stand-in for map<FRONTEND_SCREEN, CComponent*>::operator[] (0x0059B5D7, a COMDAT of this TU in retail): it
// reads the key by value and keeps no pointer to it, so the key temporary's frame word is free again after the call
// (retail shares it with the old-list pointer and the loop counter).
template <> __declspec(noinline) NUISystem::CComponent*& std::map<FRONTEND_SCREEN, NUISystem::CComponent*>::operator[](const FRONTEND_SCREEN& key)
{
    return ScreenSlotStandIn(this, key);
}
#pragma auto_inline(on)
namespace NUISystem {

class CFrontEndManager
{
public:
    unsigned char                          _pad00[0x54];
    std::map<FRONTEND_SCREEN, CComponent*> m_mapScreens;  // +0x54
    void GetProfileNames(std::vector<CWideString>& names);    // 0x005955AB
    bool VerifySignatureCached(const CWideString& filename);   // 0x005957D9
    CComponent* CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef,
                                              const CCharString& buttonDef, bool flag);   // 0x00596A68
    long RefreshAvailableProfiles();
};

#pragma auto_inline(off)
// Same-TU stand-in for GetProfileNames (0x005955AB, same TU in retail; the real body scans the save directory): it
// rewrites the caller's vector words without keeping a pointer to it, so VC7.1 knows `profiles` does not escape and
// strength-reduces the profile loop onto a walking element pointer and a down counter, as retail does.
__declspec(noinline) void CFrontEndManager::GetProfileNames(std::vector<CWideString>& names)
{
    names._M_start = g_standInProfileScan._M_start;
    names._M_finish = g_standInProfileScan._M_finish;
    names._M_end_of_storage = g_standInProfileScan._M_end_of_storage;
}
#pragma auto_inline(on)

long CFrontEndManager::RefreshAvailableProfiles()
{
    std::vector<CWideString> profiles;
    GetProfileNames(profiles);
    CManager* manager = CManager::GetInstance();
    CComponent* menu = m_mapScreens[FRONTEND_SCREEN_PROFILE_MENU];
    CComponent* oldList = menu->FindChild("UI_FRONTEND_LIST_FOR_PROFILES");
    if (oldList)
    {
        SActionParamPair parameter;
        parameter.first = 1;
        oldList->SetActionParameter(parameter);
    }
    CCountedPointer<CComponent> newList(manager->CreateComponent("UI_FRONTEND_LIST_FOR_PROFILES", false));
    CCountedPointer<CList> list(newList);
    menu->AddChild(newList);

    CWideString newProfileText = CWideString::CreateFromCharString("TEXT_GUI_MENU_NEW_PROFILE");
    CCountedPointer<CComponent> newProfileButton(
        CreateFrontEndButtonComponent(newProfileText, 0.0f, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", true));
    CCountedPointer<CAction> newProfileAction(new CAction);
    newProfileAction->ActionType = 0x125;
    newProfileButton->AddAction(newProfileAction);
    newProfileButton->AddActionOnLeftClicked(newProfileAction);
    list->AddChildDynamically(newProfileButton);

    float y = 50.0f;
    long count = profiles.size();
    for (long i = 0; i != count; ++i)
    {
        CCountedPointer<CComponent> button(
            CreateFrontEndButtonComponent(profiles[i], y, "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST", "", false));
        CCountedPointer<CAction> action(new CAction);
        bool valid = VerifySignatureCached(CTBaseSingleton<CUserProfileManager>::Get().GetProfileDir(profiles[i], false));
        action->ActionType = valid ? 0x124 : 0xdb;
        CActionParam* parameter = (CActionParam*)new CActionParamString(profiles[i]);
        action->UIActionParams.push_back(parameter);
        button->AddAction(action);
        button->AddActionOnLeftClicked(action);
        list->AddChildDynamically(button);
        y += 30.0f;
    }
    menu->Initialise();
    return count;
}
}
