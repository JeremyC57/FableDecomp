// CTCInGameMenu::GlobalUpdate(bool) -- per-frame menu work: queues a pending load-game event, counts down a deferred save, and drives the "video options changed" dialog's 15-second revert timeout
// Grade: DIFFER(860v860) -- whole body matches except the ChangePosition push ecx/fsubp scheduling swap shared with 0x0047E651 / 0x0047F30F (retail pushes the float slot after the X fsubp)
// Leaf: ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z
// Flags: /GS /Zc:wchar_t
// Tried: C2DVector ctor/copy forms, temp orders, RTM+QFE (6 grades, 2026-10-23); chunk 537 (RTM): real PDB names throughout (NUISystem::CUIScreenUnitConverter::GetBackBufferWidth/Height, CManager::GetUIScale const, CComponent::ChangePosition(const C2DVector&, float, bool)), same-TU noinline GetBackBuffer*/GetUIScale bodies, a named temporary C2DVector, a dialog local, named x/y locals, `/ 2.0f`, negation forms and compound `-=`: all inert. | next: lane gap: scheduling -- VC7.1 RTM places the ChangePosition float-slot `push ecx` before the X `fsubp` for every source form tried; shared with 0x0047E651 / 0x0047E9AE / 0x0047F30F.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047E9AE, 860 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Callees: CTCBase::GetPlayerManager 0x00686D40 (ICF-folded getter), CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0, IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0, CGameEvent::AddData 0x009F1760,
//   CMainGameComponent::AddGameEvent 0x0041649C, CTCInGameMenu::SaveGame 0x0047E196, GFGetTime 0x009E1BC0, bitset<274>::test
//   0x00410DE0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCInventory::RevertVideoChanges 0x00582754,
//   CUserProfileManager::Get 0x0040D2A0 / Apply 0x0040BC80 / Save 0x0040A740, NGameText::CDataBank::GetTextBySymbol 0x009C95E0,
//   CWideString copy 0x0099B720 / dtor 0x0099B510 / += (const wchar_t*) 0x0099B940 / += (const CWideString&) 0x0099B8D0,
//   operator+(const wchar_t*, const CWideString&) 0x0099BFF0, GFIntToWideString 0x00405F00, __ftol2, plus the dialog callees
//   of VideoOptionsChanged (CCharString, GetInstance, GetUIScale, GetBackBufferWidth/Height). Virtual slots as in 0x0047E651.
#include "engine/CCountedPointer.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CWideString;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
double __fastcall GFGetTime();                        // 0x009E1BC0
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);
namespace std {
struct __true_type { };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class list {       // STLport 4.6 (4 bytes)
public:
    void* _M_node;
    void push_back(const _Tp& __x);                   // 0x0042B646 (CActionParam*)
};
template <class _Tp, class _Alloc> class vector {     // STLport 4.6 (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);                   // 0x00486024 (CComponent*)
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);   // 0x00485DBC
};
}

namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static float __fastcall GetBackBufferWidth();     // 0x0041CC14
    static float __fastcall GetBackBufferHeight();    // 0x0041CC2B
};
enum EActionType { };
enum EActionParamType { };
class CActionParam {                                  // Ego_r.pdb 8 bytes
public:
    CActionParam(EActionParamType type) : ActionParamType(type) { }
    virtual ~CActionParam();
    EActionParamType ActionParamType;                 // +0x04
};
class CActionParamSLONG : public CActionParam {       // vtable 0x012366F4
public:
    CActionParamSLONG(long value) : CActionParam((EActionParamType)3), Value(value) { }
    virtual ~CActionParamSLONG();                     // only vtable slot: the deleting destructor 0x0047CEBD
    long Value;                                       // +0x08
};
class CAction {                                       // retail 0x10 bytes (STLport lists)
public:
    EActionType ActionType;                           // +0x00
    std::list<CActionParam*, std::allocator<CActionParam*> > UIActionParams;   // +0x04
    std::list<long, std::allocator<long> >          ThingActionParams;         // +0x08
    bool        Valid;                                // +0x0C
    CAction();                                        // 0x0042BE50
    void AddParam(CActionParam* param) { UIActionParams.push_back(param); }
};
enum EEvent { };
class CObserver {                                     // at +4 of every component
public:
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(EEvent event);          // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void ObserveAllEvents();                  // slot 6 (+0x18)
};
class CObservable {                                   // CManager vtable 0x01230134
public:
    virtual void b00(); virtual void b01(); virtual void b02(); virtual void b03(); virtual void b04();
    virtual void RemoveObserver(CObserver* observer); // slot 5 (+0x14) 0x0055C9C0
    virtual void b06(); virtual void b07(); virtual void b08();
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);   // slot 9 (+0x24) 0x0055CA90
};
#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
class CUIInterface {
public:
    virtual void u00(); virtual void u01(); virtual void u02();
};
class CComponent : public CUIInterface, public CObserver {   // RTTI: CUIInterface@0, CObserver@4, CActionParam@24; vtable 0x0124608C
public:
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2)
    virtual void v30();
    virtual void ChangePosition(const C2DVector& position, float time, bool b);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void Initialise();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void ChangeState(unsigned long state);    // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7)
    virtual void AddAction(CCountedPointer<CAction>& action);   // slot 80 (+0x140)
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual unsigned long GetCurrentStateIndex();     // slot 87 (+0x15C)
};
class CChangingStateComponent : public CComponent {
public:
    virtual void v88(); virtual void v89();
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41();
};
class CYesNo : public CChangingStateComponent {       // vtable 0x0122F884
public:
    virtual void SetYesCallBack(void (__fastcall* callBack)());   // slot 142 (+0x238)
    virtual void SetNoCallBack(void (__fastcall* callBack)());    // slot 143 (+0x23C)
};
class CText : public CChangingStateComponent {        // vtable 0x01249CCC
public:
    virtual void w42();
    virtual void SetValue(const CCharString& value, bool a, bool b);                   // slot 145 (+0x244)
    virtual void SetValue(const CWideString& value, bool a, bool b);                   // slot 144 (+0x240)
    virtual void SetValue(unsigned long value, const CWideString& format, bool a);     // slot 143 (+0x23C)
};
class CManager : public CObservable {
public:
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
    C2DVector GetUIScale() const;                     // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

#pragma auto_inline(off)
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::_M_insert_overflow(NUISystem::CComponent** __position, NUISystem::CComponent* const& __x, const std::__true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = this->_M_finish - this->_M_start;
    const unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    NUISystem::CComponent** __new_start = __len != 0 ? (NUISystem::CComponent**)malloc(__len * sizeof(NUISystem::CComponent*)) : 0;
    NUISystem::CComponent** __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_start, this->_M_start, (char*)__position - (char*)this->_M_start) + ((char*)__position - (char*)this->_M_start));
    for (unsigned int __n = __fill_len; __n > 0; --__n, ++__new_finish)
        *__new_finish = __x;
    if (!__atend)
        __new_finish = (NUISystem::CComponent**)((char*)memmove(__new_finish, __position, (char*)this->_M_finish - (char*)__position) + ((char*)this->_M_finish - (char*)__position));
    if (this->_M_start != 0)
        free(this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage = __new_start + __len;
}
template <> __declspec(noinline) void std::vector<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >::push_back(NUISystem::CComponent* const& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        *this->_M_finish = __x;
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, std::__true_type(), 1UL, true);
}
#pragma auto_inline(on)

#define CGameEvent CGameEventLayout
#include "engine/CGameEvent.h"
#undef CGameEvent
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);            // 0x0099B720
    ~CWideString();                                   // 0x0099B510
    const CWideString& operator+=(const wchar_t* text);        // 0x0099B940
    const CWideString& operator+=(const CWideString& other);   // 0x0099B8D0
};
CWideString __fastcall operator+(const wchar_t* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall GFIntToWideString(long value); // 0x00405F00
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CGameEvent : public CGameEventLayout {
public:
    CGameEvent(long type) { Type = type; Player = -1; EndPos = 0; Valid = true; Replacement = false; }
    void AddData(long size, const void* data);        // 0x009F1760
    void AddSLONG(long value) { AddData(sizeof(value), &value); }
};
class CMainGameComponent {
public:
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;  // +0x14
    void AddGameEvent(const CGameEvent& event);       // 0x0041649C
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank.Data; }
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0

enum ETCInterfaceType { };
class CTCBase;
namespace std {
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;              // 0x00410DE0 (bitset<274>)
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type, std::allocator<value_type> > Data;
    TCompare Compare;
    bool     Dirty;
    value_type* LowerBound(const K& key);             // 0x0040F020
};
class CTCInventory;
class CThing {
public:
    unsigned char    _pad00[0x20];
    std::bitset<274> UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char    _pad54[0x90 - 0x54];
    signed char      PlayerNumber;                    // +0x90 (PDB +0xA0)
    unsigned char    Dead : 1;                        // +0x91 bit 0 (PDB +0xA1)
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans : 1;
    unsigned char    Activated : 1;
    unsigned char    GloballyLoaded : 1;
    unsigned char    InLimbo : 1;
    unsigned char    KillLocked : 1;
    unsigned char    CreatedBySerialisation : 1;
    __forceinline CTCBase& GetTC(ETCInterfaceType type)
    {
        ETCInterfaceType key = type;
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        if (it == map->Data._M_finish || type < it->first)
            it = map->Data._M_finish;
        return *it->second;
    }
    template <class T> bool GetTC(T** component);
};
class CPlayerManager;
class CTCBase {                                       // Ego_r.pdb 12 bytes
public:
    virtual void t00();
    CThing* ParentThing;                              // +0x04
    bool    ToBeRemoved;                              // +0x08
    bool    IsInGlobalUpdateTCs;                      // +0x09
protected:
    CPlayerManager& GetPlayerManager();               // 0x00686D40
};
class CTCInventory : public CTCBase {                 // interface 0x11
public:
    void RevertVideoChanges();                        // 0x00582754
};
template <> __forceinline bool CThing::GetTC<CTCInventory>(CTCInventory** component)
{
    if (UsingComponents.test(0x11)) {
        ETCInterfaceType key = (ETCInterfaceType)0x11;
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        *component = (CTCInventory*)((it == map->Data._M_finish || (ETCInterfaceType)0x11 < it->first) ? map->Data._M_finish : it)->second;
        return true;
    }
    return false;
}
class CThingPlayerCreature : public CThing { };
// 0x0043DBD0: the header template, visible in the TU and out of line as in retail (VC7.1 then knows the hero is only read).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->Dead;
}
#pragma auto_inline(on)
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                         // 0x00449970
};
class CUserProfileManager {
public:
    unsigned char _pad00[0x34];
    bool          DisplayDialogOnOptionsChanged;      // +0x34
    void Apply();                                     // 0x0040BC80
    void Save();                                      // 0x0040A740
    static CUserProfileManager& __fastcall Get();     // 0x0040D2A0
};
enum { GAME_EVENT_LOAD_GAME = 0x3C };

class CTCInGameMenu : public CTCBase {                // retail layout (12-byte containers; Ego_r.pdb offsets +0x10..+0x24)
public:
    bool                   InGameMenuOpen;            // +0x0C
    unsigned char          _pad_0x0d[0x57];
    long                   LoadGameIndex;             // +0x64 (PDB +0x78)
    unsigned char          _pad_0x68[0x48];
    NUISystem::CYesNo*     PVideoOptionsNotAcceptedDialog;   // +0xB0 (PDB +0xD0)
    NUISystem::CYesNo*     PVideoOptionsChangedDialog;       // +0xB4 (PDB +0xD4)
    unsigned char          _pad_0xb8[0x08];
    double                 VideoChangesTimer;         // +0xC0 (PDB +0xE0)
    long                   SaveGameIndex;             // +0xC8 (PDB +0xE8)
    long                   FramesToWaitToSave;        // +0xCC (PDB +0xEC)
    void SaveGame(long index);                        // 0x0047E196
    virtual void GlobalUpdate(bool);
};

void CTCInGameMenu::GlobalUpdate(bool)
{
    if (!InGameMenuOpen)
        return;
    if (LoadGameIndex != -1) {
        CThingPlayerCreature* const& hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            CGameEvent event(GAME_EVENT_LOAD_GAME);
            event.AddSLONG(LoadGameIndex);
            event.Player = hero->PlayerNumber;
            g_pMainGameComponent->AddGameEvent(event);
        }
        LoadGameIndex = -1;
    }
    if (SaveGameIndex != -1) {
        if (FramesToWaitToSave > 0) {
            --FramesToWaitToSave;
        } else {
            SaveGame(SaveGameIndex);
            SaveGameIndex = -1;
            FramesToWaitToSave = -1;
        }
    }
    if (PVideoOptionsChangedDialog && PVideoOptionsChangedDialog->GetCurrentStateIndex() == 1 && VideoChangesTimer != -1.0) {
        if (GFGetTime() - VideoChangesTimer > 15.0) {
            CThing* parent = ParentThing;
            CTCInventory* inventory;
            if (parent->GetTC(&inventory) && inventory)
                inventory->RevertVideoChanges();
            CUserProfileManager::Get().DisplayDialogOnOptionsChanged = false;
            CUserProfileManager::Get().Apply();
            CUserProfileManager::Get().Save();
            CUserProfileManager::Get().DisplayDialogOnOptionsChanged = true;
            NUISystem::CText* text = (NUISystem::CText*)PVideoOptionsChangedDialog->FindChild(CCharString("PC_GAME_OPTIONS_CHANGED_TEXT", -1));
            if (text)
                text->SetValue("TEXT_GUI_OPTIONS_RESET_TIMEOUT", true, true);
            C2DVector pos(NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 204.0f,
                          NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
            PVideoOptionsChangedDialog->ChangePosition(pos, 0.0f, false);
            VideoChangesTimer = -1.0;
        } else {
            NUISystem::CText* text = (NUISystem::CText*)PVideoOptionsChangedDialog->FindChild(CCharString("PC_GAME_OPTIONS_CHANGED_TEXT", -1));
            if (text) {
                CWideString message = g_pMainGameComponent->PTextBank->GetTextBySymbol("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY");
                message += L"\n";
                message += g_pMainGameComponent->GetTextBank().GetTextBySymbol("TEXT_GUI_OPTIONS_REVERTING_IN");
                message += L" " + GFIntToWideString((long)(15.0 - (GFGetTime() - VideoChangesTimer)));
                text->SetValue(message, true, true);
            }
        }
    }
}