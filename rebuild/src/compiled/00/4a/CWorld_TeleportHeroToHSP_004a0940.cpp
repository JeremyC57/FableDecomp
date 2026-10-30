// CWorld::TeleportHeroToHSP(const CCharString& name) -- teleport the hero to the holy site whose ScriptName is name (false when no such site exists)
// Leaf: ?TeleportHeroToHSP@CWorld@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0940, 416 bytes, `ret 4` (Ego_r.pdb ?TeleportHeroToHSP@CWorld@@QAE_NABVCCharString@@@Z).
//   Caller: the test-quest activation 0x0061ABFC (CTCBase::GetWorld().TeleportHeroToHSP(quest name)).
// Callees: CWorld::PeekThingManager (virtual slot 4, +0x10; 0x0049E1C0), the CWorld-TU copy of the holy-site copy
//   template 0x004ADCB0 (/Gr, hidden result in ecx; the same body as CPlayer's 0x0048D5C0), CCharString copy 0x0099EC30 /
//   dtor 0x0099EAE0, STLport __find_if 0x0048CC60 with bind2nd(ptr_fun(0x0048BC70), name), CThing::GetScriptThing 0x004C7E50,
//   CPlayerManager::GetMainPlayer 0x00449970 (g_pMainGameComponent 0x013B86A0 +0x1C), CPlayer::GetPlayerCharacter
//   0x00487B70, CGameScriptInterface::EntityTeleportToThing (slot 472, +0x760; CWorld +0x38), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, free 0x00BFEA14.
// Shape: the landed CPlayer::LocateCharacterStartHolySite 0x00488B20 model (owner local before the vector, the inline
//   find_if / bind2nd / ptr_fun layers) with the STLport 4.6 std classes of this chunk's __find_if row 0x0048CC60 (same
//   callee symbol) and a same-TU noinline __find_if body (the tag homes in the dead `name` slot); CScriptThing
//   temporaries with the inline destructor of the landed CQ_HangingTreeEvilScript::CleanUp 0x00D6B720. The script
//   interface and the player manager are named locals read before the two GetScriptThing calls (retail loads both, and
//   the interface's vtable, ahead of the argument evaluation).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);                    // 0x00BFE9BC (MSVCR71)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {           // 12 bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;            // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                             // out of line 0x004AA840
};

class CThing {
public:
    CScriptThing GetScriptThing();                 // 0x004C7E50
};
class CPlayer {
public:
    CThing* GetPlayerCharacter();                  // 0x00487B70
};
class CPlayerManager {
public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

template <class T>
struct CRetailVector {                             // STLport vector<T>: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CRetailVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
    T* begin() const { return m_pStart; }
    T* end() const { return m_pFinish; }
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& c) : container(&c) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& c) { return back_insert_iterator<C>(c); }

struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; CThing* _M_data; };
struct SThingListIterator {
    SThingNode* _M_node;
    SThingListIterator(SThingNode* node) : _M_node(node) { }
};
struct CThingList {
    SThingNode* _M_node;
    SThingListIterator begin() const { return SThingListIterator(_M_node->_M_next); }
    SThingListIterator end() const { return SThingListIterator(_M_node); }
};
struct CThingListOwner {
    unsigned char _pad00[0x8c];
    CThingList    HolySites;                       // +0x8c
};
class CThingManager {
public:
    unsigned char    _pad00[4];
    CThingListOwner* Owner;                        // +0x04
};
// 0x004ADCB0: copies the list's things (those whose +0xD4 flag is set) to the output iterator (/Gr template)
template <class It, class Out> Out __fastcall CopyHolySites(It first, It last, Out out);

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Arg1, class _Arg2, class _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
protected:
    _Result (*_M_ptr)(_Arg1, _Arg2);
public:
    pointer_to_binary_function() { }
    explicit pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2)) : _M_ptr(__x) { }
    _Result operator()(_Arg1 __x, _Arg2 __y) const { return _M_ptr(__x, __y); }
};
template <class _Arg1, class _Arg2, class _Result>
inline pointer_to_binary_function<_Arg1, _Arg2, _Result> ptr_fun(_Result (*__x)(_Arg1, _Arg2))
{
    return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x);
}
template <class _Operation>
class binder2nd {
protected:
    _Operation op;
    typename _Operation::second_argument_type value;
public:
    binder2nd(const _Operation& __x, const typename _Operation::second_argument_type& __y) : op(__x), value(__y) { }
    typename _Operation::result_type operator()(typename _Operation::first_argument_type __x) const
    {
        return op(__x, value);
    }
};
template <class _Operation, class _Tp>
inline binder2nd<_Operation> bind2nd(const _Operation& __fn, const _Tp& __x)
{
    typedef typename _Operation::second_argument_type _Arg2_type;
    return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}

#pragma auto_inline(off)
// 0x0048CC60 (the chunk's own __find_if row, same symbol): a same-TU noinline body so VC7.1 homes the iterator tag
// in the dead `name` argument slot, as retail.
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                                 const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _Tp> inline random_access_iterator_tag __ITERATOR_CATEGORY(_Tp* const&) { return random_access_iterator_tag(); }
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}
}

bool IsThingScriptNamed(CThing* thing, CCharString name);   // 0x0048BC70 (/Gr: __fastcall)

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CThingManager& PeekThingManager() const;   // slot 4 (+0x10) 0x0049E1C0
    unsigned char         _pad04[0x38 - 4];
    CGameScriptInterface* PGameScriptInterface_ptr;          // +0x38
    bool TeleportHeroToHSP(const CCharString& name);
};

bool CWorld::TeleportHeroToHSP(const CCharString& name)
{
    CThingListOwner* owner = PeekThingManager().Owner;
    CRetailVector<CThing*> sites;
    CopyHolySites(owner->HolySites.begin(), owner->HolySites.end(), back_inserter(sites));
    CThing** found = std::find_if(sites.begin(), sites.end(), std::bind2nd(std::ptr_fun(&IsThingScriptNamed), name));
    if (found != sites.end()) {
        CGameScriptInterface* gsi = PGameScriptInterface_ptr;
        CPlayerManager* players = g_pMainGameComponent->PlayerManager;
        gsi->EntityTeleportToThing(players->GetMainPlayer().GetPlayerCharacter()->GetScriptThing(),
                                   (*found)->GetScriptThing(), false);
        return true;
    }
    return false;
}