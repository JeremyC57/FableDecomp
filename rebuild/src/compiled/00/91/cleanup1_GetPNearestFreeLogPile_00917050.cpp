// CAIStateGroup_ThrowLogsOnFire::GetPNearestFreeLogPile() const -- the free log pile nearest this group's creature (0 if none)
// Leaf: ?GetPNearestFreeLogPile@CAIStateGroup_ThrowLogsOnFire@@ABEPAVCThingObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917050, 83 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): label correction -- the row is labelled GetPNearestBonfire. Ego_r.pdb donor order GetPNearestFreeLogPile
//   (0x0093A870) / GetPNearestBonfire (0x0093A8D0) = retail 0x00917050 / 0x009170B0, and this body calls 0x00916F30, the
//   GetPBestThing<CIsFreeLogPile, CThingCompare_Nearest, CThingObject> instantiation (its twin 0x00916FC0 is the CIsFreeBonfire
//   one the landed 0x00916FC0 source names, called from 0x009170B0).
// Callees: CThing::GetPos() const 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools() const 0x008FD000,
//   CThingSearchTools::GetPBestThing<CIsFreeLogPile,CThingCompare_Nearest,CThingObject>(const CIsFreeLogPile&,
//   const CThingCompare_Nearest&, CThingObject**) const 0x00916F30.
// Shape: creature local, then `CThingObject* nearest = 0; PeekThingSearchTools().GetPBestThing(CIsFreeLogPile(), CThingCompare_Nearest(pos),
//   &nearest); return nearest;` -- the empty filter temporary sits in the high byte of a slot (`lea eax,[esp+0xf]`); the
//   same-TU GetPBestThing stand-in lets VC7.1 load the three position words before the stores, as retail does.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x14];
    unsigned __int64 UID;                               // +0x18
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x6c - 0x54];
    unsigned long ThingTypeFlags;                       // +0x6C THING_TYPE_* inheritance bits
    unsigned char _pad70[0x91 - 0x70];
    unsigned char StateBits;                            // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20
    enum { ThingTypeMask = 0 };
    bool IsIncludedInThingScans() const                 // Ego_r.pdb ?IsIncludedInThingScans@CThing@@QBE_NXZ (inline in the walk)
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    const C3DVector& GetPos() const;                    // 0x004C73D0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { public: enum { ThingTypeMask = 0x7 }; };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { public: enum { ThingTypeMask = 0x807 }; };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    const CWorld& PeekWorld() const;                    // 0x008FCFA0
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
class CIsFreeLogPile {                                  // empty predicate (Ego_r.pdb operator() 0x0093A670)
public:
    bool operator()(const CThingObject& thing) const;         // 0x00916E30
    CIsFreeLogPile() { }
};
class CThingCompare_Nearest {                           // Ego_r.pdb: Pos C3DVector +0 (12 bytes)
public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
    template <class T> T* operator()(T* a, T* b) const;   // <T> instantiation 0x00916B10 (landed; its body below is compiled here as in retail)
};
// The GetPBestThing instantiation this TU uses is retail 0x00916F30 (landed): its body is compiled here as retail compiled
// it (a template is instantiated in its caller's TU), so the relocation binds to the real symbol and nothing invented
// stands in; reading the comparator's position through the const reference keeps VC7.1 loading the three position words
// before the stores, as retail does (the chunk 517 same-TU rule, LESSONS.md).
template <class T> inline bool IsThingOfType(const CThing* thing)   // the mask goes through a local (VC7.1 folds it late)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
}
// CThingCompare_Nearest::operator()<T> (the landed 0x00916B10): null a loses to b, null b loses to a, else the smaller squared
// 3D distance to Pos wins (a on a strict less, b on a tie).
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return (deltaA.GetLengthSquared() < deltaB.GetLengthSquared()) ? a : b;
}
class CThingSearchTools {                                // Ego_r.pdb 28 bytes (only `this` is passed through here)
public:
    template <class TFilter, class TCompare, class TThing>
    bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const;
protected:
    const std::list<CClass*>& PeekClassList() const;     // 0x0049C770 ?PeekClassList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@XZ
};
#pragma auto_inline(off)
template <class TFilter, class TCompare, class TThing>
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) const
{
    TThing* best = 0;
    const std::list<CClass*>& things = PeekClassList();
    for (std::list<CClass*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        if (((CThing*)*it)->IsIncludedInThingScans() && IsThingOfType<TThing>((CThing*)*it)) {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(*candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}
#pragma auto_inline(on)
class CAIStateGroup_ThrowLogsOnFire : public CAIStateGroupBase {
private:
    CThingObject* GetPNearestFreeLogPile() const;
};

CThingObject* CAIStateGroup_ThrowLogsOnFire::GetPNearestFreeLogPile() const
{
    const CThingAICreature* creature = PBrain->Creature;
    CThingObject* nearest = 0;
    PeekThingSearchTools().GetPBestThing(CIsFreeLogPile(), CThingCompare_Nearest(creature->GetPos()), &nearest);
    return nearest;
}