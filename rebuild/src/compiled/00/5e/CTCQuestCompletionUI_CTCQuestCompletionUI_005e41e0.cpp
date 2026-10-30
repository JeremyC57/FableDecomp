// CTCQuestCompletionUI::~CTCQuestCompletionUI() -- quest completion screen TC dtor: leave the global TC list, delete the black overlay, re-enable 3D rendering and mark the world as starting 3D rendering, delete the screen's UI components, reset the UI manager (observers, action params, previous-selection list, bastard child), cancel the cutscene preload and reopen the player GUI; then the member and base destructors
// Leaf: ??1CTCQuestCompletionUI@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E41E0, 587 bytes, tail jump to the base destructor.
// Identity: the vtable store 0x0125589C carries RTTI `.?AVCTCQuestCompletionUI@@` (rtti_bases: CTCQuestCompletionUI : CTCBase);
//   Ego_r.pdb has `??1CTCQuestCompletionUI@@UAE@XZ`. The catalog label is right.
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0, CTCBase::GetWorld 0x00686A70 (Ego_r.pdb `IBEAAVCWorld@@XZ`), CWorld::GetFrame
//   0x0049D870 (`SIJXZ`, inside the inline CWorld::SetAsStarting3DRendering, Ego_r.pdb `QAEXXZ`: GetWorld is called first), memmove 0x00BFEAE6 (vector::clear), NUISystem::CManager::GetInstance 0x0041E5F2 and its virtuals
//   ClearObservers (CObservable slot 6), ClearActionParams (slot 29: its body 0x0041D207 clears the ThingActionParams /
//   UIActionParams lists at +0x5C/+0x60), ClearBastardChild (slot 37: 0x0041DED2 releases BastardChild +0x80 and empties
//   BastardChildren +0x88), free 0x00BFEA14, CPlayerGui::IsOpen 0x004374F0 / Open 0x004374B0 (Ego_r.pdb, bound so),
//   std::_Rb_tree<long, pair<const long, unsigned long>, ...>::_M_erase 0x005E8C00 (x3), ~CWideString 0x0099B510 (x4),
//   ~CCharString 0x0099EAE0, std::_List_base<CQuestManager::CBoast>::clear 0x004B8C00, CTCBase::~CTCBase 0x00686830 (tail jump).
//   Virtual calls on the 3D engine (CDisplayEngine::Get3DEngine of the main game component's display engine): CIEngine slot 76
//   Enable3DSceneRendering(bool) and slot 56 CutscenePreload(bool, bool) (vtable 0x012A0D24 in Ego_r declaration order: slot 70
//   is FreeTextureFromEngineResourcePools, as the landed CMovie::FreeTextures 0x00547B90; both PDB signatures fit the pushes).
// Data: g_pMainGameComponent 0x013B86A0 (bound), g_pPlayerGui 0x013B8790 (bound), NUISystem::CList::GPreviousSelected 0x013B8A98:
//   Ego_r.pdb names the dynamic initializers of CList::GPreviousSelected then CHoverable::GAlreadyHovered in that order, and the
//   retail initializers 0x0121A6AC / 0x0121A6D0 build the lists at 0x013B8A98 then 0x013B8AD4, whose atexit destructors run
//   code in the CList (0x0053C9BC) and CHoverable (0x0055C300) TUs; the element type CList* is probable (the CList code at
//   0x00536A6F finds and erases `this` in it).
// Layout: Ego_r.pdb CTCQuestCompletionUI (0xF0) with retail STLport sizes (vector/map 0xC, list 4): Components +0x10, PCardDef
//   +0x2C, Boasts +0x30, QuestSummary +0x38, SuccessSummary +0x3C, QuestObjective +0x40, LastRenderTime +0x48, ScriptName +0x50,
//   FailureReason +0x54, StartQuestExperience/Gold/Renown +0x84/+0x90/+0x9C, PFrame +0xB8, PBlack +0xBC (the landed ctor
//   0x005E4030 and Deactivate 0x005E3DF0 use the same offsets). engine/CTCBase.h flattens the vptr, so the chain is modelled here.
// Shape: the user body in retail order, `delete PBlack; PFrame = 0;`, the component loop (`jbe`, size re-read) and clear, the
//   GPreviousSelected clear inlined; then the implicit member dtors in reverse order (three map dtors, five strings, the list's
//   out-of-line clear + header free, the def pointer release read once into a local, the vector free).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x0
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                         // +0x0 (0xC bytes)
    void clear() { _M_t.clear(); }
};
} // namespace std

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

class CCharString { public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld : public CWorldLayout { public:
    static long __fastcall GetFrame();                      // 0x0049D870
    void SetAsStarting3DRendering() { FrameStarted3DRendering = GetFrame(); }   // Ego_r.pdb ?SetAsStarting3DRendering@CWorld@@QAEXXZ (inline here)
};
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
protected:
    CWorld& GetWorld() const;                               // 0x00686A70
};

class CDefPointeeBase { public:                             // counted def object: vptr, reference count +4
    virtual void CUnprovenSlot0() const;
    virtual void CUnprovenSlot1() const;                    // slot 1: called when the last reference goes
    mutable long RefCount;                                  // +0x04
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb: CDefPointeeBase*)
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->CUnprovenSlot1();
            Object = 0;
        }
    }
};
class CQuestCardDef;
class CQuestManager { public:
    class CBoast { public:                                  // Ego_r.pdb layout (0x20)
        long        ID;                                     // +0x00
        long        Cost;                                   // +0x04
        long        Reward;                                 // +0x08
        CCharString ScriptName;                             // +0x0C
        CWideString Text;                                   // +0x10
        CWideString Title;                                  // +0x14
        bool        Evil;                                   // +0x18
        bool        Failed;                                 // +0x19
        bool        Completed;                              // +0x1A
        long        GroupIndex;                             // +0x1C
    };
};
namespace NUISystem {
class CComponent { public: virtual ~CComponent(); };
class CText;
}

class CIEngine { public:                                    // vtable 0x012A0D24
    virtual ~CIEngine();
    virtual void CUnprovenSlot1_012a0d24();
    virtual void CUnprovenSlot2_012a0d24();
    virtual void CUnprovenSlot3_012a0d24();
    virtual void CUnprovenSlot4_012a0d24();
    virtual void CUnprovenSlot5_012a0d24();
    virtual void CUnprovenSlot6_012a0d24();
    virtual void CUnprovenSlot7_012a0d24();
    virtual void CUnprovenSlot8_012a0d24();
    virtual void CUnprovenSlot9_012a0d24();
    virtual void CUnprovenSlot10_012a0d24();
    virtual void CUnprovenSlot11_012a0d24();
    virtual void CUnprovenSlot12_012a0d24();
    virtual void CUnprovenSlot13_012a0d24();
    virtual void CUnprovenSlot14_012a0d24();
    virtual void CUnprovenSlot15_012a0d24();
    virtual void CUnprovenSlot16_012a0d24();
    virtual void CUnprovenSlot17_012a0d24();
    virtual void CUnprovenSlot18_012a0d24();
    virtual void CUnprovenSlot19_012a0d24();
    virtual void CUnprovenSlot20_012a0d24();
    virtual void CUnprovenSlot21_012a0d24();
    virtual void CUnprovenSlot22_012a0d24();
    virtual void CUnprovenSlot23_012a0d24();
    virtual void CUnprovenSlot24_012a0d24();
    virtual void CUnprovenSlot25_012a0d24();
    virtual void CUnprovenSlot26_012a0d24();
    virtual void CUnprovenSlot27_012a0d24();
    virtual void CUnprovenSlot28_012a0d24();
    virtual void CUnprovenSlot29_012a0d24();
    virtual void CUnprovenSlot30_012a0d24();
    virtual void CUnprovenSlot31_012a0d24();
    virtual void CUnprovenSlot32_012a0d24();
    virtual void CUnprovenSlot33_012a0d24();
    virtual void CUnprovenSlot34_012a0d24();
    virtual void CUnprovenSlot35_012a0d24();
    virtual void CUnprovenSlot36_012a0d24();
    virtual void CUnprovenSlot37_012a0d24();
    virtual void CUnprovenSlot38_012a0d24();
    virtual void CUnprovenSlot39_012a0d24();
    virtual void CUnprovenSlot40_012a0d24();
    virtual void CUnprovenSlot41_012a0d24();
    virtual void CUnprovenSlot42_012a0d24();
    virtual void CUnprovenSlot43_012a0d24();
    virtual void CUnprovenSlot44_012a0d24();
    virtual void CUnprovenSlot45_012a0d24();
    virtual void CUnprovenSlot46_012a0d24();
    virtual void CUnprovenSlot47_012a0d24();
    virtual void CUnprovenSlot48_012a0d24();
    virtual void CUnprovenSlot49_012a0d24();
    virtual void CUnprovenSlot50_012a0d24();
    virtual void CUnprovenSlot51_012a0d24();
    virtual void CUnprovenSlot52_012a0d24();
    virtual void CUnprovenSlot53_012a0d24();
    virtual void CUnprovenSlot54_012a0d24();
    virtual void CUnprovenSlot55_012a0d24();
    virtual void CutscenePreload(bool preload, bool force);   // slot 56 (+0xE0)
    virtual void CUnprovenSlot57_012a0d24();
    virtual void CUnprovenSlot58_012a0d24();
    virtual void CUnprovenSlot59_012a0d24();
    virtual void CUnprovenSlot60_012a0d24();
    virtual void CUnprovenSlot61_012a0d24();
    virtual void CUnprovenSlot62_012a0d24();
    virtual void CUnprovenSlot63_012a0d24();
    virtual void CUnprovenSlot64_012a0d24();
    virtual void CUnprovenSlot65_012a0d24();
    virtual void CUnprovenSlot66_012a0d24();
    virtual void CUnprovenSlot67_012a0d24();
    virtual void CUnprovenSlot68_012a0d24();
    virtual void CUnprovenSlot69_012a0d24();
    virtual void CUnprovenSlot70_012a0d24();
    virtual void CUnprovenSlot71_012a0d24();
    virtual void CUnprovenSlot72_012a0d24();
    virtual void CUnprovenSlot73_012a0d24();
    virtual void CUnprovenSlot74_012a0d24();
    virtual void CUnprovenSlot75_012a0d24();
    virtual void Enable3DSceneRendering(bool enable);          // slot 76 (+0x130)
};
#define CDisplayEngine CDisplayEngineLayout
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
class CDisplayEngine : public CDisplayEngineLayout { public:
    CIEngine& Get3DEngine() { return *Engine3D; }
};
template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CMainGameComponent { public:                          // Ego_r.pdb offsets up to +0x28 hold in retail
    unsigned char              _pad00[0x28];
    scoped_ptr<CDisplayEngine> PDisplayEngine;              // +0x28
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CPlayerGui : public CPlayerGuiLayout { public:
    void Open();                                            // 0x004374B0
    bool IsOpen() const;                                    // 0x004374F0
};
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790

namespace NUISystem {
enum EEvent { };                                            // values not needed here
class CObserver;
class CObservable { public:                                 // virtuals in Ego_r declaration order
    virtual void SetEvent(EEvent event);                    // slot 0
    virtual void SetOwnedEvent(CObserver* observer, EEvent event);   // slot 1
    virtual void AddObserver(CObserver* observer);          // slot 2
    virtual void AddExclusiveObserver(CObserver* observer); // slot 3
    virtual void RemoveExclusiveObserver(CObserver* observer);       // slot 4
    virtual void RemoveObserver(CObserver* observer);       // slot 5
    virtual void ClearObservers();                          // slot 6 (+0x18)
    virtual CObserver* GetExclusiveObserver();              // slot 7
    virtual bool IsExclusivelyObserved();                   // slot 8
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);    // slot 9
    virtual void RemoveConcurrentExclusiveObserver(CObserver* observer); // slot 10
};
class CManager : public CObservable { public:               // vtable 0x01230134
    virtual void CUnprovenSlot11_01230134();
    virtual void CUnprovenSlot12_01230134();
    virtual void CUnprovenSlot13_01230134();
    virtual void CUnprovenSlot14_01230134();
    virtual void CUnprovenSlot15_01230134();
    virtual void CUnprovenSlot16_01230134();
    virtual void CUnprovenSlot17_01230134();
    virtual void CUnprovenSlot18_01230134();
    virtual void CUnprovenSlot19_01230134();
    virtual void CUnprovenSlot20_01230134();
    virtual void CUnprovenSlot21_01230134();
    virtual void CUnprovenSlot22_01230134();
    virtual void CUnprovenSlot23_01230134();
    virtual void CUnprovenSlot24_01230134();
    virtual void CUnprovenSlot25_01230134();
    virtual void CUnprovenSlot26_01230134();
    virtual void CUnprovenSlot27_01230134();
    virtual void CUnprovenSlot28_01230134();
    virtual void ClearActionParams();                       // slot 29 (+0x74)
    virtual void CUnprovenSlot30_01230134();
    virtual void CUnprovenSlot31_01230134();
    virtual void CUnprovenSlot32_01230134();
    virtual void CUnprovenSlot33_01230134();
    virtual void CUnprovenSlot34_01230134();
    virtual void CUnprovenSlot35_01230134();
    virtual void CUnprovenSlot36_01230134();
    virtual void ClearBastardChild();                       // slot 37 (+0x94)
    static CManager* __fastcall GetInstance();              // 0x0041E5F2
};
class CList { public:
    static std::list<CList*> GPreviousSelected;             // 0x013B8A98
};
}

// STLport 4.6 _List_base<T*>::clear, inlined in this TU for the trivially destructible pointer list
template <> inline void std::_List_base<NUISystem::CList*, std::allocator<NUISystem::CList*> >::clear()
{
    _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _Node* __tmp = __cur;
        __cur = (_Node*)__cur->_M_next;
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

class CTCQuestCompletionUI : public CTCBase { public:
    enum CompletionType { };                                // values not needed here
    bool                                    Active;                       // +0x0C
    bool                                    ReallyActive;                 // +0x0D
    std::vector<NUISystem::CComponent*>     Components;                   // +0x10
    bool                                    Quitting;                     // +0x1C
    bool                                    WaitingForRegionLoad;         // +0x1D
    bool                                    WaitingForPlayerControl;      // +0x1E
    bool                                    WaitingForBoastProcessing;    // +0x1F
    CompletionType                          Completion;                   // +0x20
    unsigned long                           Frame;                        // +0x24
    long                                    FrameStartedBoastProcessing;  // +0x28
    CDefPointer<const CQuestCardDef>        PCardDef;                     // +0x2C
    std::list<CQuestManager::CBoast>        Boasts;                       // +0x30
    unsigned long                           RenderedObjectDef;            // +0x34
    CWideString                             QuestSummary;                 // +0x38
    CWideString                             SuccessSummary;               // +0x3C
    CWideString                             QuestObjective;               // +0x40
    double                                  LastRenderTime;               // +0x48
    CCharString                             ScriptName;                   // +0x50
    CWideString                             FailureReason;                // +0x54
    long                                    GoldReward;                   // +0x58
    long                                    RenownReward;                 // +0x5C
    long                                    TargetGoldGained;             // +0x60
    long                                    InitialGold;                  // +0x64
    long                                    TargetExperienceGained;       // +0x68
    long                                    InitialExperience;            // +0x6C
    long                                    TargetRenownGained;           // +0x70
    long                                    InitialRenown;                // +0x74
    float                                   ExperienceGainedDisplayed;    // +0x78
    float                                   RenownGainedDisplayed;        // +0x7C
    float                                   GoldGainedDisplayed;          // +0x80
    std::map<long, unsigned long>           StartQuestExperience;         // +0x84
    std::map<long, unsigned long>           StartQuestGold;               // +0x90
    std::map<long, unsigned long>           StartQuestRenown;             // +0x9C
    NUISystem::CText*                       PRenownText;                  // +0xA8
    NUISystem::CText*                       PGoldText;                    // +0xAC
    NUISystem::CText*                       PExperienceText;              // +0xB0
    float                                   AccumFadeTime;                // +0xB4
    NUISystem::CComponent*                  PFrame;                       // +0xB8
    NUISystem::CComponent*                  PBlack;                       // +0xBC
    bool                                    QuittingFadeOut;              // +0xC0
    long                                    QuestCardDef;                 // +0xC4
    unsigned long                           ExperienceCountingSound;      // +0xC8
    unsigned long                           GoldCountingSound;            // +0xCC
    unsigned long                           RenownCountingSound;          // +0xD0
    bool                                    CanTeleportToGuildMember;     // +0xD4
    virtual ~CTCQuestCompletionUI();
};

CTCQuestCompletionUI::~CTCQuestCompletionUI()
{
    RemoveFromGlobalTCs();
    delete PBlack;
    PFrame = 0;
    g_pMainGameComponent->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    GetWorld().SetAsStarting3DRendering();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    g_pMainGameComponent->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
    if (!g_pPlayerGui->IsOpen())
        g_pPlayerGui->Open();
}