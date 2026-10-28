// CSubtitleRenderer::Clear(bool keepText) -- drop every primitive handle; unless keeping the text, reset the captions and overrides, end the player mode it added, release the hero's item display, post the "text finished" messages its flags ask for, unpause the game, and show the next queued text
// Leaf: ?Clear@CSubtitleRenderer@@QAEX_N@Z
// Flags: /GS /Zc:wchar_t
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManagerLayout
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CGameEvent CGameEventLayout
#include "engine/CGameEvent.h"
#undef CGameEvent
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector { public:   // STLport 4.6 (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
};
template <class _Ret, class _Tp> class mem_fun_ref_t { public:
    typedef _Ret (_Tp::*__fun_type)(void);
    explicit mem_fun_ref_t(__fun_type __pf) : _M_f(__pf) { }
    _Ret operator()(_Tp& __r) const { return (__r.*_M_f)(); }
private:
    __fun_type _M_f;
};
template <class _Ret, class _Tp> inline mem_fun_ref_t<_Ret, _Tp> mem_fun_ref(_Ret (_Tp::*__f)()) { return mem_fun_ref_t<_Ret, _Tp>(__f); }
template <class _InputIter, class _Function> _Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x0043F360
template <class _Tp> struct _Deque_iterator_base {
    _Tp*  _M_cur;
    _Tp*  _M_first;
    _Tp*  _M_last;
    _Tp** _M_node;
};
template <class _Tp> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    _Tp& operator*() const { return *this->_M_cur; }
};
template <class _Tp> inline bool __cdecl operator==(const _Deque_iterator<_Tp>& __x, const _Deque_iterator<_Tp>& __y) { return __x._M_cur == __y._M_cur; }
template <class _Tp, class _Alloc> class _Deque_base { public:
    _Deque_iterator<_Tp> _M_start;
    _Deque_iterator<_Tp> _M_finish;
    _STLP_alloc_proxy<_Tp**, _Tp*, _Alloc> _M_map;
    _STLP_alloc_proxy<unsigned int, _Tp*, _Alloc> _M_map_size;
};
template <class _Tp, class _Alloc> class deque : protected _Deque_base<_Tp, _Alloc> { public:
    bool empty() const { return this->_M_finish == this->_M_start; }
    _Tp& front() { return *this->_M_start; }
    void pop_front();                                   // 0x00493CF5
};
template <class _Tp, class _Sequence> class queue { public:
    bool empty() const { return c.empty(); }
    _Tp& front() { return c.front(); }
    void pop() { c.pop_front(); }
protected:
    _Sequence c;
};
template <size_t _Nb> class bitset { public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(size_t __pos) const;                      // 0x00410DE0 (bitset<274>)
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}

class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const wchar_t* text);  // 0x0099B800
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CEnginePrimitiveHandle { public:
    unsigned long Handle;
    void Clear();                                       // 0x0041BE70
};
class CFontBank;

enum ETCInterfaceType { TCI_HERO_RECEIVE_ITEMS = 0xCB };
class CTCBase;
class CTCHeroReceiveItems { public:                     // interface 0xCB (GetInterfaceType, vtable 0x0126EBFC slot 21)
    unsigned char _pad00[0x27];
    bool          ForceClear;                           // +0x27 (Ego_r +0x33)
};
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class TCompare> class CVectorMap { public:   // retail 0x10 bytes
    typedef std::pair<K, V> value_type;
    std::vector<value_type, std::allocator<value_type> > Data;
    TCompare Compare;
    bool     Dirty;
    value_type* LowerBound(const K& key);               // 0x0040F020
};
class CThing { public:
    unsigned char   _pad00[0x20];
    std::bitset<274> UsingComponents;                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    __forceinline CTCBase& GetTC(ETCInterfaceType type)   // ?GetTC@CThing@@QAEAAVCTCBase@@W4ETCInterfaceType@@@Z (inline)
    {
        ETCInterfaceType key = type;
        CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
        std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(key);
        if (it == map->Data.end() || type < it->first)
            it = map->Data.end();
        return *it->second;
    }
};
class CThingPlayerCreature : public CThing { public: };
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThingPlayerCreature*)
enum EPlayerMode { };
class CPlayer : public CPlayerLayout { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                  // 0x00634190
};
class CPlayerManager : public CPlayerManagerLayout { public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
template <class T> class scoped_ptr { public:
    T* ptr;
    T* operator->() const { return ptr; }
};
class CMessageEvent;
enum EMessageEventType { MESSAGE_EVENT_TEXT_FINISHED_2 = 0x11, MESSAGE_EVENT_TEXT_FINISHED_8 = 0x12 };
class CMessageEventManager { public:
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifetime, const C3DVector& pos, CThing* from, CThing* to,
                              float radius, float data, long extra);   // 0x00687540
};
class CWorld : public CWorldLayout { public:
};
class CGameEvent : public CGameEventLayout { public:
    CGameEvent(long type) { Type = type; Player = -1; EndPos = 0; Valid = true; Replacement = false; }
    void AddData(long size, const void* data);          // 0x009F1760
    void AddUBYTE(unsigned char value) { AddData(1, &value); }
};
class CMainGameComponent { public:
    unsigned char              _pad00[0x1C];
    scoped_ptr<CPlayerManager> PPlayerManager;          // +0x1C
    unsigned char              _pad20[0x04];
    CWorld*                    PWorld;                  // +0x24
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
enum { GAME_EVENT_PAUSE = 9 };

class CSubtitleRenderer : public CSubtitleRendererLayout { public:
    class CSetTextInfo { public:
        CWideString   Text;
        CWideString   Button1Text;
        CWideString   Button2Text;
        CWideString   Button3Text;
        unsigned long DisplayFlags;
        float         DisplayTime;
        bool          UpdateWhilePaused;
        bool          ChangePlayerMode;
        ~CSetTextInfo();                                // 0x0043E3D0
    };
    typedef std::queue<CSetTextInfo, std::deque<CSetTextInfo, std::allocator<CSetTextInfo> > > CTextInfoQueue;
    typedef std::vector<CEnginePrimitiveHandle, std::allocator<CEnginePrimitiveHandle> > CHandles;
    void SetButton1OverrideText(const CCharString& text);   // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
    void SetText(const CFontBank& font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, const CWideString& button3, unsigned long flags, float time,
                 bool updateWhilePaused, bool changePlayerMode);   // 0x00492973
    void Clear(bool keepText);
};

void CSubtitleRenderer::Clear(bool keepText)
{
    std::for_each(reinterpret_cast<CHandles&>(PrimitiveHandles).begin(), reinterpret_cast<CHandles&>(PrimitiveHandles).end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    std::for_each(reinterpret_cast<CHandles&>(ButtonPrimitiveHandles).begin(), reinterpret_cast<CHandles&>(ButtonPrimitiveHandles).end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    std::for_each(reinterpret_cast<CHandles&>(EmbeddedPrimitiveHandles).begin(), reinterpret_cast<CHandles&>(EmbeddedPrimitiveHandles).end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    if (keepText)
        return;
    CPlayer& player = g_pMainGameComponent->PPlayerManager->GetMainPlayer();
    CThingPlayerCreature* hero = player.GetPControlledCreature();
    DisplayingText = false;
    UpdateTimer = true;
    Button1Alpha = 0xff;
    Button2Alpha = 0xff;
    reinterpret_cast<CWideString&>(Button1Text_PStringData) = L"";
    reinterpret_cast<CWideString&>(Button2Text_PStringData) = L"";
    reinterpret_cast<CWideString&>(Button3Text_PStringData) = L"";
    SetButton1OverrideText("");
    SetButton2OverrideText("");
    EPlayerMode mode = reinterpret_cast<EPlayerMode&>(PlayerModeAdded);
    if (mode != 0) {
        if (IS_THING_ALIVE(hero))
            player.RemoveMode(mode);
        reinterpret_cast<EPlayerMode&>(PlayerModeAdded) = (EPlayerMode)0;
    }
    if ((DisplayFlags & 0x40) && IS_THING_ALIVE(hero) && hero->UsingComponents.test(TCI_HERO_RECEIVE_ITEMS))
        ((CTCHeroReceiveItems&)hero->GetTC(TCI_HERO_RECEIVE_ITEMS)).ForceClear = true;
    if (DisplayFlags & 8)
        g_pMainGameComponent->PWorld->PMessageEventManager_Data->AddMessage(MESSAGE_EVENT_TEXT_FINISHED_8, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    if (DisplayFlags & 2)
        g_pMainGameComponent->PWorld->PMessageEventManager_Data->AddMessage(MESSAGE_EVENT_TEXT_FINISHED_2, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    DisplayFlags = 0;
    if (PausedGame) {
        CGameEvent event(GAME_EVENT_PAUSE);
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
        PausedGame = false;
    }
    if (!reinterpret_cast<CTextInfoQueue&>(TextInfoQueue).empty()) {
        CSetTextInfo& info = reinterpret_cast<CTextInfoQueue&>(TextInfoQueue).front();
        SetText(*PFont, info.Text, L"", info.Button1Text, info.Button2Text, info.Button3Text,
                info.DisplayFlags, info.DisplayTime, info.UpdateWhilePaused, info.ChangePlayerMode);
        reinterpret_cast<CTextInfoQueue&>(TextInfoQueue).pop();
    }
}