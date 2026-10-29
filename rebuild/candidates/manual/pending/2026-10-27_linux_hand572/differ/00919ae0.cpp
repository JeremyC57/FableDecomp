// CAIStateGroup_BuyFromSeller::Update() -- find the seller in our trade interaction, walk to him, pick a random stock item from his shop, ask for it, wait for his reply, say thanks or ignore, then leave the interaction
// Grade: DIFFER(2217v2220) -- same 95 calls in retail order, same frame, same shared free tail; residue is register choice and one block placement: (1) participant loop loads *it straight into ebx (retail: mov ecx,[esi+8]; cmp ecx,eax; je; mov ebx,ecx = the 3 missing bytes, then the loop-align pads after it shift); (2) stock-filter loop has node/element in ebx/ebp, retail ebp/ebx; (3) the AIAction_DoNothing()==1 exit (RemoveMessage(request); jmp to the clear tail) sits after the reply path, retail puts it after the scratchpad GetTC has-path
// Leaf: ?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00919AE0, 2,220 bytes, ends at ret 0x0091A38B, no EH frame, ebp frame with and esp,-8.
// Identity (proven): vtable 0x0128FF0C (RTTI CAIStateGroup_BuyFromSeller) slot 12 = this row (slot 11 IsRunnable 0x0091A390,
//   slot 9 Cleanup 0x00919A00), and slot 12 of the sibling vtable 0x0128B78C is the landed
//   ?Update@CAIStateGroup_BanditSitRoundCampfire@@UAEXXZ 0x0093B1D0; Ego_r.pdb name set holds ?Update@CAIStateGroup_BuyFromSeller@@UAEXXZ
//   (donor 0x0093D5D0) next to GetSellersReply (donor 0x0093D4F0, 0x70 bytes = retail 0x00919A70, 0x70 bytes, the row before this one).
// Callees: IP ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90; CThing::GetPos 0x004C73D0;
//   CVectorMap::LowerBound 0x0040F020 (plain inline: VC inlines it at three IsInWorkHours sites and calls it at the rest, as
//   retail does); CTCVillageMember::GetPVillage 0x007EE540; CTCVillage::IsWorkTime 0x00658B20; AIAction_MoveToPosProximity
//   0x0097E890; Terminate 0x008FCF40; CTCShopKeeper::GetShopTC 0x00798F50; malloc 0x00BFEA0E / free 0x00BFEA14;
//   list<CIntelligentPointer<CThing> >::push_back 0x004BA4E0 and _List_base::clear 0x004B8050 (same-TU noinline bodies below);
//   GetPWorldSeed 0x008FCFD0; GFRandom 0x004CDAE0; CTCStockItem::GetBuyersPhrase 0x007F4F80; GetWorld 0x008FCFC0;
//   CMessageEventManager::AddMessage 0x00687540 / RemoveMessage 0x00493B5B; AIAction_SpeakAISpeechToTarget 0x009812C0;
//   CWorld::GetFrame 0x0049D870; GetSellersReply 0x00919A70; AIAction_DoNothing 0x0097E200;
//   CThingCreatureBase::RemoveFromCreatureInteraction 0x006629B0. Globals: CMainGameComponent::ConstantFPS 0x01375550;
//   g_BuyFromSellerProximity 0x0138C690 (float 1.0, read only here; descriptive name).
// Layout (local models, header debt in NOTES.md): CThing (retail, 8-aligned by UniqueID) UsingComponents bitset<274> +0x20,
//   Components CVectorMap +0x44, PlayerNumber +0x90, Dead bit 0 of +0x91; CThingCreatureBase PCreatureInteraction +0x12C;
//   CWorld PMessageEventManager CCountedPointer +0x60 (engine/CWorld.h flattens it and lacks GetMessageEventManager);
//   CTCBase ParentThing +4 (engine/CTCBase.h); CTCThingOwner ObjectList +0xC; CTCAIScratchpad ThankingPhrase +0xC, IgnoringPhrase +0x10;
//   CMessageEvent ExtraData +0x3C; CAIStateGroup_BuyFromSeller FrameLastBought +0x14 (engine/CAIStateGroup_BuyFromSeller.h is flat).
//   TC interface numbers (vtable slot 21 of each TC): VillageMember 0x23, ThingOwner 0x21, StockItem 0x54, ShopKeeper 0x90,
//   AIScratchpad 0xAA.
// Shape: (a) the list-scope exits share retail's single `call free ; add esp,4 ; seller dtor ; epilogue` tail only when every
//   `return` before the list scope comes from its own `if` (an `a || b` fail before the list stops VC7.1 merging the
//   list-scope tails at all: each exit then gets its own free + epilogue); inside the list scope the fails are one `||`
//   each, as retail's single fail block per loop shows. (b) the free tail also needs same-TU noinline bodies for both
//   clear and push_back, which let VC keep the list sentinel in esi / its spill slot [esp+18h]. (c) CWorld::GetMessageEventManager()
//   (FableWin.pdb name) gives GetWorld before the argument push. (d) one `bool arrived` per move loop gives the [esp+12h] /
//   [esp+13h] swap between the two loops. (e) the stock-item test is a named bool (stored to [esp+13h] like GetTC's has).
//   (f) list::size() is STLport 4.6's distance(begin(), end()).
#include "engine/CAIBrain.h"
#include "engine/CTCBase.h"                            // vptr +0, ParentThing +4 (flat generated header)
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: __forceinline allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    __forceinline _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    __forceinline iterator begin() { return this->_M_start; }
    __forceinline iterator end() { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    __forceinline static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    __forceinline static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    __forceinline _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    __forceinline bool operator[](size_t __pos) const { return _Unchecked_test(__pos); }
    __forceinline bool _Unchecked_test(size_t __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    __forceinline _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    __forceinline void _M_incr() { _M_node = _M_node->_M_next; }
    __forceinline bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    __forceinline _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    __forceinline _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
    __forceinline typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    __forceinline typename _Traits::pointer operator->() const { return &(operator*()); }
    __forceinline _Self& operator++() { this->_M_incr(); return *this; }
};
struct input_iterator_tag { };
template <class _InputIterator> __forceinline ptrdiff_t __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    ptrdiff_t __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it; ++__n;
    }
    return __n;
}
template <class _InputIterator> __forceinline ptrdiff_t distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, input_iterator_tag());
}
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    __forceinline _List_base(const _Alloc& __a) : _M_node(__a, (_Node*)0)
    {
        _Node* __n = (_Node*)malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    __forceinline ~_List_base()
    {
        clear();
        free(_M_node._M_data);
    }
    void clear();                                       // 0x004B8050 (<CIntelligentPointer<CThing> >)
    _STLP_alloc_proxy<_Node*, _Node, _Alloc> _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef _Alloc allocator_type;
    __forceinline list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    __forceinline ~list() { }
    __forceinline iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    __forceinline iterator end() { return iterator(this->_M_node._M_data); }
    __forceinline const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    __forceinline const_iterator end() const { return const_iterator((_Node*)this->_M_node._M_data); }
    __forceinline size_t size() const
    {
        size_t __result = distance(begin(), end());
        return __result;
    }
    __forceinline iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_data) _Tp(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x);                     // 0x004BA4E0 (<CIntelligentPointer<CThing> >)
};
}   // namespace std

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    __forceinline const C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    __forceinline float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
};

template <class K, class V> class CKeyPairCompareLess { };
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    CArray<value_type> Data;
    TCompare           Compare;
    bool               Dirty;
    iterator LowerBound(const K& key)                   // 0x0040F020
    {
        long count = Data._M_finish - Data._M_start;
        iterator first = Data._M_start;
        while (count > 0) {
            long half = count / 2;
            iterator middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};

enum ETCInterfaceType { };
class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseObject : public CBaseClass { };
class CClass : public CBaseObject { };
class CTCBase;

class CThing : public CClass {
public:
    unsigned char      _pad04[0x14];
    unsigned __int64   UniqueID;                        // +0x18
    std::bitset<274>   UsingComponents;                 // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x3c];                    // +0x54 UpdateComponentsList .. CurrentMapNumber
    signed char        PlayerNumber;                    // +0x90
    unsigned char      Dead : 1;                        // +0x91
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        bool has = HasTC((ETCInterfaceType)T::INTERFACE_TYPE);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            *out = (T*)((it == map->Data.end() || T::INTERFACE_TYPE < it->first) ? map->Data.end() : it)->second;
        }
        return has;
    }
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CCreatureInteraction;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline T* operator->() const { return Data; }
    __forceinline T& operator*() const { return *Data; }
};
class CThingCreatureBase : public CThingGameObject {
public:
    unsigned char _pad98[0x12c - 0x98];
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;   // +0x12C
    void RemoveFromCreatureInteraction();              // 0x006629B0
    __forceinline CCountedPointer<CCreatureInteraction>& GetPCreatureInteraction() { return PCreatureInteraction; }
};
class CThingAICreature : public CThingCreatureBase { };

class CCreatureInteraction {
public:
    std::list<CThingCreatureBase*> Participants;        // +0x00
    __forceinline const std::list<CThingCreatureBase*>& GetParticipants() const { return Participants; }
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline CIntelligentPointer() { }
    __forceinline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    template <class U> __forceinline CIntelligentPointer(CIntelligentPointer<U>& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); return *this; }
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    __forceinline T* operator->() const { return GetPItem(); }
    __forceinline T& operator*() const { return *GetPItem(); }
};
inline void* __cdecl operator new(size_t, void* where) { return where; }
namespace std {
#pragma auto_inline(off)
template <class _Tp, class _Alloc> __declspec(noinline) void _List_base<_Tp, _Alloc>::clear()   // 0x004B8050, same-TU stand-in
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        __tmp->_M_data.~_Tp();
        free(__tmp);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc> __declspec(noinline) void list<_Tp, _Alloc>::push_back(const _Tp& __x)   // 0x004BA4E0, same-TU stand-in
{
    insert(end(), __x);
}
#pragma auto_inline(on)
}
template <class T> __forceinline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing.GetPItem() != 0 && !thing->Dead;
}

class CTCVillage : public CTCBase {
public:
    bool IsWorkTime() const;                            // 0x00658B20
};
class CTCVillageMember : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x23 };
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};
class CTCShop : public CTCBase { };
class CTCShopKeeper : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x90 };
    CTCShop* GetShopTC() const;                         // 0x00798F50
};
class CTCThingOwner : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x21 };
    std::list<CIntelligentPointer<CThing> > ObjectList; // +0x0C
    __forceinline std::list<CIntelligentPointer<CThing> >& GetObjectList() { return ObjectList; }
    __forceinline const std::list<CIntelligentPointer<CThing> >& GetObjectList() const { return ObjectList; }
};
class CTCStockItem : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0x54 };
    unsigned long GetBuyersPhrase();                    // 0x007F4F80
};
class CTCAIScratchpad : public CTCBase {
public:
    enum { INTERFACE_TYPE = 0xaa };
    unsigned long ThankingPhrase;                       // +0x0C
    unsigned long IgnoringPhrase;                       // +0x10
    __forceinline unsigned long GetThankingPhrase() const { return ThankingPhrase; }
    __forceinline unsigned long GetIgnoringPhrase() const { return IgnoringPhrase; }
};

enum EMessageEventType { };
class CMessageEvent {
public:
    unsigned char _pad00[0x3c];
    long          ExtraData;                            // +0x3C
};
class CMessageEventManager {
public:
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator,
        CThing* target, float sightRadius, float hearingRadius, long extraData);   // 0x00687540
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};

class CWorld {
public:
    unsigned char _pad00[0x60];
    CCountedPointer<CMessageEventManager> PMessageEventManager;   // +0x60
    static long __fastcall GetFrame();                  // 0x0049D870
    __forceinline CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager; }
};

class CMainGameComponent {
public:
    __forceinline static long __fastcall GetConstantFPS() { return ConstantFPS; }
protected:
    static long ConstantFPS;                            // 0x01375550
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
extern float g_BuyFromSellerProximity;                  // 0x0138C690 (1.0f)

enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
enum EScriptAIPriority { };
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*         PBrain;                           // +0x04
    EScriptAIPriority ScriptAIPriority;                 // +0x08
    CCharString       Name;                             // +0x0C
    bool              Terminated;                       // +0x10
    unsigned char     Priority;                         // +0x11
    bool              Enabled;                          // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    __forceinline CThingAICreature& GetCreature() const { return *PBrain->Creature; }
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    EAIActionReturn AIAction_DoNothing();               // 0x0097E200
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& arrived,
        EMovementType movement, bool a, bool b, bool c, const CThing* thing, bool d, float e);   // 0x0097E890
    EAIActionReturn AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase> speaker,
        CIntelligentPointer<const CThing> target, unsigned long speech, bool interrupt, long priority);   // 0x009812C0
};

template <class TBase> class CAIStateGroup_WorkBase : public TBase {
protected:
    __forceinline bool IsInWorkHours() const
    {
        CTCVillageMember* member;
        if (this->GetCreature().GetTC(&member)) {
            CTCVillage* village = member->GetPVillage();
            if (village && village->IsWorkTime())
                return true;
        }
        return false;
    }
};

class CAIStateGroup_BuyFromSeller : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
public:
    long FrameLastBought;                               // +0x14
    virtual void Update();
private:
    __forceinline void BoughtSomething() { FrameLastBought = CWorld::GetFrame(); }
    const CMessageEvent* GetSellersReply(const CThingCreatureBase* seller);   // 0x00919A70
};

__forceinline bool IsFurtherThan(const CThing& a, const CThing& b, const float& distance)
{
    return (a.GetPos() - b.GetPos()).GetSquaredMagnitude() > distance * distance;
}

void CAIStateGroup_BuyFromSeller::Update()
{
    const std::list<CThingCreatureBase*>& participants = GetCreature().GetPCreatureInteraction()->GetParticipants();
    CIntelligentPointer<CThingAICreature> seller;
    for (std::list<CThingCreatureBase*>::const_iterator it = participants.begin(); it != participants.end(); ++it) {
        if (*it != &GetCreature())
            seller = (CThingAICreature*)*it;
    }
    if (!seller.GetPItem()) {
        Terminate();
        return;
    }
    if (!IsFurtherThan(*seller, GetCreature(), g_BuyFromSellerProximity)) {
        Terminate();
        return;
    }
    EAIActionReturn result;
    do {
        if (!IsInWorkHours()) {
            Terminate();
            return;
        }
        if (!IS_THING_ALIVE(seller)) {
            Terminate();
            return;
        }
        bool arrived;
        result = AIAction_MoveToPosProximity((EAIActionRepeat)2, seller->GetPos(), g_BuyFromSellerProximity, arrived,
            (EMovementType)3, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2) {
        Terminate();
        return;
    }
    CTCShopKeeper* shopKeeper;
    if (!seller->GetTC(&shopKeeper)) {
        Terminate();
        return;
    }
    CTCShop* shop = shopKeeper->GetShopTC();
    if (!shop) {
        Terminate();
        return;
    }
    {
    std::list<CIntelligentPointer<CThing> > stock;
    CTCThingOwner* owner;
    if (shop->ParentThing->GetTC(&owner)) {
        for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
            if (IS_THING_ALIVE(*it)) {
                bool isStockItem = (*it)->HasTC((ETCInterfaceType)CTCStockItem::INTERFACE_TYPE);
                if (isStockItem)
                    stock.push_back(*it);
            }
        }
    }
    if (stock.size() == 0) {
        Terminate();
        return;
    }
    unsigned long index = GFRandom(stock.size(), GetPWorldSeed());
    CThing* item = 0;
    for (std::list<CIntelligentPointer<CThing> >::iterator it = stock.begin(); it != stock.end(); ++it) {
        if (index == 0) {
            item = it->GetPItem();
            break;
        }
        --index;
    }
    CTCStockItem* stockItem;
    item->GetTC(&stockItem);
    unsigned long phrase = stockItem->GetBuyersPhrase();
    CMessageEvent* request = GetWorld().GetMessageEventManager().AddMessage((EMessageEventType)0x13, 1000,
        C3DVector(0.0f, 0.0f, 0.0f), &GetCreature(), seller.GetPItem(), 0.0f, 0.0f, 0);
    AIAction_SpeakAISpeechToTarget((EAIActionRepeat)2, &GetCreature(), seller, phrase, true, 0x93);
    bool accepted;
    {
        long timeout = CWorld::GetFrame() + 20 * CMainGameComponent::GetConstantFPS();
        const CMessageEvent* reply;
        while ((reply = GetSellersReply(seller.GetPItem())) == 0) {
            if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
                GetWorld().GetMessageEventManager().RemoveMessage(request);
                Terminate();
                return;
            }
            if (timeout < CWorld::GetFrame())
                break;
            if (IsFurtherThan(*seller, GetCreature(), g_BuyFromSellerProximity)) {
                do {
                    if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
                        Terminate();
                        return;
                    }
                    bool arrived;
                    result = AIAction_MoveToPosProximity((EAIActionRepeat)2, seller->GetPos(), g_BuyFromSellerProximity,
                        arrived, (EMovementType)3, true, false, true, 0, true, 0.0f);
                } while (result == 4);
                if (result == 1) {
                    GetWorld().GetMessageEventManager().RemoveMessage(request);
                    Terminate();
                    return;
                }
            } else if (AIAction_DoNothing() == 1) {
                GetWorld().GetMessageEventManager().RemoveMessage(request);
                return;
            }
        }
        if (reply) {
            GetWorld().GetMessageEventManager().RemoveMessage(reply);
            accepted = reply->ExtraData != 0;
        } else {
            GetWorld().GetMessageEventManager().RemoveMessage(request);
            accepted = false;
        }
    }
    CTCAIScratchpad* scratchpad;
    GetCreature().GetTC(&scratchpad);
    unsigned long reaction = accepted ? scratchpad->GetThankingPhrase() : scratchpad->GetIgnoringPhrase();
    do {
        if (!IsInWorkHours() || !IS_THING_ALIVE(seller)) {
            Terminate();
            return;
        }
        result = AIAction_SpeakAISpeechToTarget((EAIActionRepeat)2, &GetCreature(), seller, reaction, false, 0x93);
    } while (result == 4);
    BoughtSomething();
    GetCreature().RemoveFromCreatureInteraction();
    Terminate();
    }
}
