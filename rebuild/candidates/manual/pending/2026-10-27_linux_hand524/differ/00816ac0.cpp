// CTCHitLocations::OnInitialActivate() -- fetch the hit-locations sub-def, clear the default hit location, build one (hit location, priority) pair per listed hit-location def (with its armour def), then enable each hit location as its def says (a default one becomes the owner's default hit location)
// Grade: DIFFER(520v421) -- def-pointer lifetimes: retail keeps the hit-location def in esi with unchecked add-refs, reuses ONE out slot ([esp+10h]) for both GetDef calls and keeps no temporary CHitLocation in the frame; our CDefPointer locals stay in memory with null tests and the temporary is materialised. 1 grade; needs the Ego source shape of the def locals (see LESSONS).
// Leaf: ?OnInitialActivate@CTCHitLocations@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00816AC0, 421 bytes, thiscall, plain `ret`.
// Identity (proven): retail 0x00816AC0 is slot 6 of the vtable 0x012434E4 whose RTTI is .?AVCTCHitLocations@@ (slot 6 is
//   OnInitialActivate: the landed CTCDExplosiveTrail::OnInitialActivate 0x008100A0 sits in slot 6 of 0x01242B4C);
//   Ego_r.pdb (0x0080F970) and FableWin.pdb (0x024E7720) hold ?OnInitialActivate@CTCHitLocations@@UAEXXZ. The body reads
//   the CHitLocationsDef sub-def into PDef and fills HitLocations (Ego_r.pdb members).
// Label: the catalog's "CParticleMorphs_PopulateFromDefinitions" is propagated and wrong.
// Callees: CDefClassBase::GetSubDef<CHitLocationsDef> 0x00817BB0, CTCBase::GetDefinitionManager() const 0x006869D0,
//   CDefinitionManager::GetDef<CHitLocationDef> 0x00817C70 / GetDef<CArmourDef> 0x00817B40, vector<pair<CHitLocation, long>
//   >::push_back 0x00818DC0, the defs' release (slot 1), CVectorMap LowerBound 0x0040F020 (the inline SetEnabled's
//   component lookup, interface 0x7F).
// Layout: retail CTCHitLocations {CTCBase vptr +0, ParentThing +4; HitLocations +0xC (12-byte STLport vector),
//   PDefaultHitLocation +0x18, PDef +0x1C}; CHitLocationsDef::HitLocations (vector<long>) +0x28; CHitLocationDef Armour
//   +0x44, Priority +0x70, Default +0x74, EnableOnCreate +0x75 (Ego_r.pdb less 4); CHitLocation 0x18 bytes (Ego_r.pdb);
//   CThing PDef +0x70, UsingComponents +0x20, Components +0x44.
// Shape: CHitLocation::SetEnabled is the landed 0x00816390 body, inline.
#define CDefClassBase CDefClassBasePdb
#define CTCBase CTCBasePdb
#include "engine/CDefClassBase.h"
#include "engine/CTCBase.h"
#undef CDefClassBase
#undef CTCBase

void* __cdecl operator new(unsigned int, void* where);
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy();                                   // slot 1 (+0x04)
    long RefCount;                                            // +0x04
    void Release() const
    {
        if (--const_cast<CDefClassBase*>(this)->RefCount == 0)
            const_cast<CDefClassBase*>(this)->Destroy();
    }
};
template <class T> class CDefPointer {
public:
    T* Object;                                                // +0x00
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++const_cast<CDefClassBase*>((const CDefClassBase*)Object)->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};

class CHitLocationsDef;
class CThingDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CHitLocationsDef>* out) const;   // GetSubDef<CHitLocationsDef> 0x00817BB0
};
class CHitLocationsDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long*         HitLocationsStart;                          // +0x28 vector<long> HitLocations
    long*         HitLocationsFinish;                         // +0x2c
    long*         HitLocationsEnd;                            // +0x30
};
enum EActionRegisteredType { ACTION_REGISTERED_TYPE_NULL };
class CHitLocationDef : public CDefClassBase {
public:
    unsigned char _pad08[0x44 - 0x08];
    long          Armour;                                     // +0x44
    unsigned char _pad48[0x70 - 0x48];
    long          Priority;                                   // +0x70
    bool          Default;                                    // +0x74
    bool          EnableOnCreate;                             // +0x75
};
class CArmourDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // <CHitLocationDef> 0x00817C70, <CArmourDef> 0x00817B40
};
class CGameDefinitionManager : public CDefinitionManager { };

class CTCHitLocations;
struct STCEntry {                                             // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long             m_kind;
    CTCHitLocations* m_pTC;
};
struct CTCMap {                                               // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);          // 0x0040F020
    __forceinline CTCHitLocations* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                                // +0x20
    unsigned char _pad40[4];
    CTCMap        m_tcs;                                      // +0x44
    unsigned char _pad50[0x70 - 0x50];
    CDefPointer<const CThingDef> PDef;                        // +0x70
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCHitLocations* GetTCIfPresent(CThing* thing, long kind)
{
    CTCHitLocations* tc;                                      // deliberately unset when the component is absent
    if (thing->HasTC(kind))
        tc = thing->m_tcs.Find(kind);
    return tc;
}

namespace NHitLocations {
class CHitLocation {                                          // Ego_r.pdb, 0x18 bytes
public:
    CThing*                             POwnerThing;          // +0x00
    CDefPointer<const CHitLocationDef>  PDef;                 // +0x04
    CDefPointer<const CArmourDef>       PArmourDef;           // +0x08
    EActionRegisteredType               HitResponseAction;    // +0x0C
    EActionRegisteredType               BlockResponseAction;  // +0x10
    bool                                Enabled;              // +0x14
    CHitLocation(CThing& owner, const CDefPointer<const CHitLocationDef>& def, const CDefPointer<const CArmourDef>& armourDef)
        : POwnerThing(&owner), PDef(def), PArmourDef(armourDef), HitResponseAction(ACTION_REGISTERED_TYPE_NULL),
          BlockResponseAction(ACTION_REGISTERED_TYPE_NULL), Enabled(true) { }
    void SetEnabled(bool enabled);
};
}

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
    return pair<_T1, _T2>(__x, __y);
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const _Tp& __x);                           // <pair<CHitLocation, long> > 0x00818DC0
};
}

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                      // +0x04
    unsigned char _pad08[4];
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCHitLocations : public CTCBase {
public:
    std::vector<std::pair<NHitLocations::CHitLocation, long> > HitLocations;   // +0x0C
    NHitLocations::CHitLocation*        PDefaultHitLocation;  // +0x18
    CDefPointer<const CHitLocationsDef> PDef;                 // +0x1C
    virtual void OnInitialActivate();
};

inline void NHitLocations::CHitLocation::SetEnabled(bool enabled)
{
    Enabled = enabled;
    if (PDef->Default) {
        CTCHitLocations* locations = GetTCIfPresent(POwnerThing, 0x7f);
        if (enabled)
            locations->PDefaultHitLocation = this;
        else if (locations->PDefaultHitLocation == this)
            locations->PDefaultHitLocation = 0;
    }
}

void CTCHitLocations::OnInitialActivate()
{
    ParentThing->PDef->GetSubDef(&PDef);
    PDefaultHitLocation = 0;
    for (const long* it = PDef->HitLocationsStart; it != PDef->HitLocationsFinish; ++it) {
        CDefPointer<const CHitLocationDef> hitLocationDef;
        GetDefinitionManager().GetDef(*it, &hitLocationDef);
        CDefPointer<const CArmourDef> armourDef;
        GetDefinitionManager().GetDef(hitLocationDef->Armour, &armourDef);
        HitLocations.push_back(std::make_pair(NHitLocations::CHitLocation(*ParentThing, hitLocationDef, armourDef),
                                              hitLocationDef->Priority));
    }
    for (std::vector<std::pair<NHitLocations::CHitLocation, long> >::iterator it = HitLocations.begin();
         it != HitLocations.end(); ++it)
        it->first.SetEnabled(it->first.PDef->EnableOnCreate);
}
