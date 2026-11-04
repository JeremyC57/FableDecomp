// CSubtitleRenderer::GetActionNameFromEmbeddedTag(const CWideString& tag, CWideString& actionName) -- resolve an embedded [GAME_ACTION_...] tag (a GameActionValues entry, or a GAME_ACTION_MOVEMENT_UP/RIGHT/DOWN/LEFT direction) to the name of its assigned input
// Leaf: ?GetActionNameFromEmbeddedTag@CSubtitleRenderer@@CI_NABVCWideString@@AAV2@@Z
// Flags: /Zc:wchar_t
// Retail: 0x0049124E, 1056 bytes (FableWin ?GetActionNameFromEmbeddedTag@CSubtitleRenderer@@CI_NABVCWideString@@AAV2@@Z; bsim
//   0.944 against the PDB function). Same size TU as FormatTextWithGameActionNames 0x004917B5 (its only caller).
// Callees: CPlayerGui::PeekGuiDef 0x00438570, CWideString::ConvertToCharString 0x0099B2C0, the GameActionValues (+0x940)
//   _Rb_tree::_M_find 0x00475861, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, strncmp 0x00BFEAF8,
//   CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, GetAssignedInputForAction 0x00408C90,
//   GetAssignedInputForMovementAction 0x00408D90 (GAME_ACTION 0x3c), GetActionName 0x00446720.
// Shape: CCharString::StartsWith / EndsWith (FableWin ?StartsWith@CCharString@@QBE_NABV1@@Z, ?EndsWith@...) are inline;
//   the suffix temporary is built before the converted tag (argument first) and destroyed after it.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CCharStringData { public:                    // the shared string block
    char* m_pData;                                 // +0x00
    long  m_length;                                // +0x04
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    __forceinline const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    __forceinline long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    __forceinline bool StartsWith(const CCharString& prefix) const
    {
        return strncmp(GetPString(), prefix.GetPString(), prefix.Length()) ? false : true;
    }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(GetPString() + offset, suffix.GetPString()) == 0;
        return false;
    }
};
class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();
    ~CWideString();
    CCharString ConvertToCharString() const;       // 0x0099B2C0
};
namespace std {
template<class T> class allocator {};
template<class Value,class T,class A> class _STLP_alloc_proxy : public A { public: Value _M_data; };
template<class T,class A> class _Vector_base {
protected:
    T* _M_start; T* _M_finish;
    _STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> > class vector : public _Vector_base<T,A> {
public:
    void push_back(const T&);
};
template <class T> struct less {};
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct _Rb_tree_node_base {
    bool                _M_color;                   // +0x00
    _Rb_tree_node_base* _M_parent;                  // +0x04
    _Rb_tree_node_base* _M_left;                    // +0x08
    _Rb_tree_node_base* _M_right;                   // +0x0C
};
template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};
template <class Key, class Value, class Compare> class _Rb_tree {   // the landed 0x00475861 model
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    _Rb_tree_node_base* _M_header;                  // +0x00
    unsigned int        _M_node_count;              // +0x04
    Compare             _M_key_compare;             // +0x08
    _Base_ptr _M_find(const Key& k) const;          // 0x00475861
};
template <class Key, class T> class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, less<Key> > _Rep_type;
    _Rep_type _M_t;
    class const_iterator { public:
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* node) : _M_node(node) { }
        bool operator!=(const const_iterator& other) const { return _M_node != other._M_node; }
        const value_type* operator->() const { return &static_cast<_Rb_tree_node<value_type>*>(_M_node)->_M_value_field; }
    };
    const_iterator find(const Key& key) const { return const_iterator(_M_t._M_find(key)); }
    const_iterator end() const { return const_iterator(_M_t._M_header); }
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass {protected:CBaseClassNonCopyable();~CBaseClassNonCopyable();};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;};
class CRGBColour {public:unsigned char B,G,R,A;};
#define CFadeInFadeOutBase CFadeInFadeOutBaseLayout
#include "engine/CFadeInFadeOutBase.h"
#undef CFadeInFadeOutBase
class CFadeInFadeOutBase {public:bool Active;float FadeInTime,FadeOutTime;bool Closing,Opening;float OpenTimer,CloseTimer;CRGBColour ToColour;};
FABLE_STATIC_ASSERT(sizeof(CFadeInFadeOutBase)==sizeof(CFadeInFadeOutBaseLayout));
class CEnginePrimitiveHandle;
enum EPlayerMode {};
namespace std {
template<class T>struct _Nonconst_traits {};
template<class T>struct _Deque_iterator_base {T* _M_cur;T* _M_first;T* _M_last;T** _M_node;};
template<class T,class Traits>struct _Deque_iterator:public _Deque_iterator_base<T> {};
template<class T,class A>class _Deque_base {protected:_Deque_iterator<T,_Nonconst_traits<T> > _M_start,_M_finish;_STLP_alloc_proxy<T**,T*,allocator<T*> > _M_map;_STLP_alloc_proxy<unsigned int,T,A> _M_map_size;};
template<class T,class A=allocator<T> >class deque:protected _Deque_base<T,A> {};
template<class T,class Sequence=deque<T> >class queue {protected:Sequence c;};
}
class CSubtitleRenderer : public CBaseClassNonCopyable {
public:
    class CSetTextInfo;class CEmbeddedGraphic;class CEmbeddedHighlight;
    C2DBoxF VWindow,VTelevisionWindow;
    long TextCharactersShowing;bool TextGroupHasEntriesRemaining;long TextGroupIndex;
    CWideString TextGroupTag;
    std::queue<CSetTextInfo,std::deque<CSetTextInfo,std::allocator<CSetTextInfo> > > TextInfoQueue;
    CWideString Text,NameText,Button1Text,Button2Text,Button3Text,Button1OverrideText,Button2OverrideText;
    unsigned char Button1Alpha,Button2Alpha;unsigned long DisplayFlags;
    CFontBank* PFont;CGraphicDataBank* Graphics;float MaxDisplayTime,TextTimer;EPlayerMode PlayerModeAdded;
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles,ButtonPrimitiveHandles,EmbeddedPrimitiveHandles;
    std::vector<CEmbeddedGraphic> EmbeddedGraphics;std::vector<CEmbeddedHighlight> EmbeddedHighlights;std::vector<CWideString> EmbeddedHighlightActionNames;
    bool PausedGame,AllCharactersDisplayed,InCutscene,CutsceneSkipped,CutsceneSkippable,CutsceneSkippableWhilePaused,CutsceneActionMode,Button1TextOverridden,Button2TextOverridden,DisplayingText,UpdateWhilePaused,UpdateTimer;
    CFadeInFadeOutBase SkipCutsceneTimer,ButtonPulseTimer;
    static CWideString __fastcall FormatTextWithGameActionNames(const CWideString&, std::vector<CWideString>*);
private:
    static bool __fastcall GetActionNameFromEmbeddedTag(const CWideString&, CWideString&);
};
FABLE_STATIC_ASSERT(sizeof(CSubtitleRenderer)==sizeof(CSubtitleRendererLayout));
enum EGameAction { GAME_ACTION_NONE = 0, GAME_ACTION_MOVEMENT = 0x3c };
class CActionInputControl { public:                // PDB 0x1c bytes (the landed AddTick 0x0064FB47 model)
    long GameAction;                               // +0x00
    long ControllerType;                           // +0x04
    long KeyboardKey;                              // +0x08
    long XboxButton;                               // +0x0c
    long MouseButton;                              // +0x10
    C2DVector ControlDirection;                    // +0x14
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0)
    {
        ControlDirection.X = 0.0f;
        ControlDirection.Y = 0.0f;
    }
};
class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();  // 0x0040D2A0
    bool GetAssignedInputForAction(EGameAction action, bool primary, CActionInputControl* control);   // 0x00408C90
    bool GetAssignedInputForMovementAction(EGameAction action, bool primary, const C2DVector& direction, CActionInputControl& control);   // 0x00408D90
};
bool __fastcall GetActionName(const CActionInputControl& control, CWideString& name, bool shortName);   // 0x00446720
class CPlayerGuiDef { public:
    unsigned char _pad0[0x940];
    std::map<CCharString, long> GameActionValues;  // +0x940 (PDB map<CCharString, EGameAction>; the landed tree model's key type)
};
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};


bool __fastcall CSubtitleRenderer::GetActionNameFromEmbeddedTag(const CWideString& tag, CWideString& actionName)
{
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    std::map<CCharString, long>::const_iterator it = def.GameActionValues.find(tag.ConvertToCharString());
    if (it != def.GameActionValues.end()) {
        EGameAction action = (EGameAction)it->second;
        CActionInputControl control;
        if (CUserProfileManager::Get().GetAssignedInputForAction(action, true, &control)
            || CUserProfileManager::Get().GetAssignedInputForAction(action, false, &control)) {
            if (GetActionName(control, actionName, false))
                return true;
        }
    }
    else if (tag.ConvertToCharString().StartsWith("GAME_ACTION_MOVEMENT_")) {
        C2DVector direction(0.0f, 0.0f);
        if (tag.ConvertToCharString().EndsWith("UP"))
            direction = C2DVector(0.0f, 1.0f);
        else if (tag.ConvertToCharString().EndsWith("RIGHT"))
            direction = C2DVector(1.0f, 0.0f);
        else if (tag.ConvertToCharString().EndsWith("DOWN"))
            direction = C2DVector(0.0f, -1.0f);
        else if (tag.ConvertToCharString().EndsWith("LEFT"))
            direction = C2DVector(-1.0f, 0.0f);
        if (direction.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO) {
            CActionInputControl control;
            if (CUserProfileManager::Get().GetAssignedInputForMovementAction(GAME_ACTION_MOVEMENT, true, direction, control)
                || CUserProfileManager::Get().GetAssignedInputForMovementAction(GAME_ACTION_MOVEMENT, false, direction, control)) {
                if (GetActionName(control, actionName, false))
                    return true;
            }
        }
    }
    return false;
}