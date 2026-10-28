// CAIStateGroup_MinionPutOutFires::GoToWell() -- find the nearest well (object property 0x800); while the well and the burning building both live and the building is still on fire, run to within 4 units of the well, then fill a bucket there; true when the bucket was filled
// Grade: DIFFER(499v502) -- residue: frame slot order (retail: filter temporary 0x14 shared with the well intelligent pointer, well 0x20, compare 0x24, arrived 0x13, HasTC byte 0x1f; ours well lowest), `push esi` scheduled before the GetPos call, the vtable store of CIntelligentPointer<CThingObject> and the loop-align pad | tried: 2026-10-21 plain-inline vs forceinline lookup, nested-if/while loop, arrived first; 2026-10-28 real-name rewrite (CThingFilter_ObjectHasProperty, CThingCompare_Nearest, PeekTC<const CTCFlammable> with an inline const lower_bound, IS_THING_ALIVE, FillBucket by const reference), same-TU noinline GetPBestLeafThing stand-in (VC folded its trivial body: 459 bytes, unusable), search-tools reference local, named compare local, loop-scoped arrived | next: an opaque same-TU GetPBestLeafThing (an extern call in its body so the result is not folded), or the retail order of the three temporaries via a CFilter object built before the call
// Leaf: ?GoToWell@CAIStateGroup_MinionPutOutFires@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935C40, length 502 bytes (+ int3 padding), `and esp,-8` frame, bool in al.
// Name: Ego_r.pdb `?GoToWell@CAIStateGroup_MinionPutOutFires@@AAE_NXZ` (private).
// Callees (Ego_r spellings): CThing::GetPos 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
//   CThingSearchTools::GetPBestLeafThing<CThingFilter_ObjectHasProperty, CThingCompare_Nearest, CThingObject> 0x00936100 (catalog
//   label "<CIsFreeHopscotchBoard,...>" is propagated; the 0x800 dword filter is the PDB's only CThingObject instantiation with a
//   one-dword filter, CThingFilter_ObjectHasProperty(EObjectProperty)), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem
//   0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThingObject> vtable 0x0125A858;
//   CIntelligentPointer<const CThing> vtable 0x01237F1C, by value), CTCFlammable::IsOnFire 0x00780400 (TC 0x6D, bit 13 of +0x2C,
//   inline const lower_bound), AIAction_MoveToThingProximity 0x0097E380, CCreatureAction_FillBucket(CThingAICreature&) 0x00843840
//   (PAICreature +0xA8 destroyed inline), ~CCreatureActionBase 0x00693EF0, AIAction_PerformAction 0x0097D8B0.
// Layout: Ego_r CAIStateGroup_MinionPutOutFires PBuildingOnFire +0x14; retail CThing bitset +0x20 / Components +0x44.
class C3DVector { public: float X, Y, Z; };
class CTCBase;
enum ETCInterfaceType { TC_FLAMMABLE = 0x6d };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const_iterator end() const { return this->_M_finish; }
    __forceinline const_iterator LowerBound(const K& key) const
    {
        const_iterator first = this->_M_start;
        long count = this->_M_finish - this->_M_start;
        while (count > 0) {
            long half = count / 2;
            const_iterator middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                      // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;     // +0x44
    unsigned char _pad_0x50[0x91 - 0x50];
    bool Dead : 1;                                         // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
    const C3DVector& GetPos() const;                       // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        bool present = HasTC((ETCInterfaceType)T::InterfaceType);
        if (present) {
            CVectorMap<ETCInterfaceType, CTCBase*>::const_iterator p = Components.LowerBound((ETCInterfaceType)T::InterfaceType);
            if (p == Components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
                p = Components.end();
            *out = (const T*)p->second;
        }
        return present;
    }
};
class CThingObject : public CThing { };
class CThingBuilding : public CThingObject { };
class CThingAICreature : public CThing { };
template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

class CTCFlammable {
public:
    enum { InterfaceType = TC_FLAMMABLE };
    bool IsOnFire() const;                                 // 0x00780400
};

class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject* item);                      // 0x00A01B90
private:
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    template <class U> CIntelligentPointer(CIntelligentPointer<U>& other) { SetPItem((CBaseObject*)other.Get()); }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
};

enum EObjectProperty { };
class CThingFilter_ObjectHasProperty {
public:
    EObjectProperty Property;
    CThingFilter_ObjectHasProperty(EObjectProperty property) : Property(property) { }
};
class CThingCompare_Nearest {
public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools {
public:
    template <class F, class C, class T> bool GetPBestLeafThing(const F& filter, const C& compare, T** best) const;
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                        // 0x00693EF0
    unsigned char _pad04[0xa8 - 0x04];
};
class CCreatureAction_FillBucket : public CCreatureActionBase {
public:
    CCreatureAction_FillBucket(CThingAICreature& creature);   // 0x00843840
    CIntelligentPointer<CThingAICreature> PAICreature;     // +0xa8
};

class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    const CThingSearchTools& PeekThingSearchTools() const; // 0x008FD000
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, float proximity,
        bool& arrived, EMovementType movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    EAIActionReturn AIAction_PerformAction(const CCreatureActionBase& action);   // 0x0097D8B0
};
class CAIStateGroup_MinionPutOutFires : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;   // +0x14
private:
    bool GoToWell();
};

bool CAIStateGroup_MinionPutOutFires::GoToWell()
{
    CThingObject* well = 0;
    if (!PeekThingSearchTools().GetPBestLeafThing(CThingFilter_ObjectHasProperty((EObjectProperty)0x800),
                                                  CThingCompare_Nearest(GetCreature().GetPos()), &well))
        return false;
    CIntelligentPointer<CThingObject> p_well(well);
    bool arrived = false;
    for (;;) {
        if (!IS_THING_ALIVE(p_well))
            break;
        if (!IS_THING_ALIVE(PBuildingOnFire))
            break;
        const CTCFlammable* flammable;
        if (!PBuildingOnFire->PeekTC(&flammable))
            break;
        if (!flammable->IsOnFire())
            break;
        EAIActionReturn result = AIAction_MoveToThingProximity((EAIActionRepeat)2, p_well, 4.0f, arrived, (EMovementType)5,
                                                               true, false, true, true, 0.0f);
        if (result == 4)
            continue;
        if (result != 2)
            break;
        if (AIAction_PerformAction(CCreatureAction_FillBucket(GetCreature())) == 2)
            return true;
        break;
    }
    return false;
}
