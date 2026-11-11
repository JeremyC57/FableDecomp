// CAIStateGroup_MinionWatchFires::Update() -- run to within 5 units of the burning building while it lives and burns; once there, keep alerting the player (AlertToPlayer, then DoNothing) while it still burns; terminate the group when the building is gone, out or the alert fails
// Grade: DIFFER(ours v retail) -- DIFFER(442v438): regraded as it sits with FABLE_HDR_OVERLAY (2026-11-11), residue unchanged from the prior draft: 10 lines over 12 probes: the first-site LowerBound eax copy (same residue as 0x00935E40), a 2-byte vs 3-byte alignment pad that follows from it, and the tail: retail loads this into ecx before the PerformActionAndWait-failed jne and jumps straight to `call Terminate`, the build reloads ebx on that path. Lane tie.
// Earlier grade (prior draft): DIFFER(442v438) -- 10 lines over 12 probes: the first-site LowerBound eax copy (same residue as 0x00935E40), a 2-byte vs 3-byte alignment pad that follows from it, and the tail: retail loads this into ecx before the PerformActionAndWait-failed jne and jumps straight to `call Terminate`, the build reloads ebx on that path. Lane tie.
// Leaf: ?Update@CAIStateGroup_MinionWatchFires@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1); PutOutFire @ 0x00935E40 (468 bytes, bool in al), Update @ 0x009363D0 (438 bytes, virtual).
//   One retail TU: CAIStateGroup_MinionPutOutFires methods 0x935A80-0x936020, COMDATs (IS_THING_DEAD @ 0x00936180), then
//   CAIStateGroup_MinionWatchFires 0x9361C0-0x9365E0; both bodies are authored here.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCFlammable::IsOnFire @ 0x00780400 (interface 0x6d, bit 13 of +0x2c;
//   first lookup CVectorMap::LowerBound @ 0x0040F020 inlined, second the const LowerBound @ 0x004365B0 called), CThing::GetPos
//   @ 0x004C73D0, AIAction_MoveToPosProximity @ 0x0097E890, AIAction_PerformAction @ 0x0097D8B0, AIAction_PerformActionAndWait
//   @ 0x0097DB70, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40, CCreatureAction_PlayAnimation ctor @ 0x008424F0
//   ("ST_IDLE" @ 0x0127138C), CCreatureAction_AlertToPlayer ctor @ 0x00842270, ~CCreatureActionBase @ 0x00693EF0.
// Names (PDB): all member and template names from FableWin.pdb / Ego_r.pdb (IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> >,
//   operator!=(const CIntelligentPointer<T>&, int), GetTC/PeekTC<CTCFlammable>, CAIStateGroupBase::GetCreature() -> CThingAICreature&).
// Layout: Ego_r.pdb PBuildingOnFire +0x14 in both classes, CAIBrain::Creature +0x18; retail CThing bitset +0x20 / map +0x44 / Dead +0x91.
#include "rebuild_abi.h"
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCBase;
enum ETCInterfaceType { };                              // enumerator names not in the name sets (values as casts)
namespace std {                                         // STLport 4.6: the vector base of CVectorMap
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {   // the landed 0x0040F020 / 0x004365B0 bodies
public:
    typedef std::pair<K, V> value_type;
    value_type* LowerBound(const K& key)                // 0x0040F020
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    const value_type* LowerBound(const K& key) const    // 0x004365B0
    {
        long count = this->_M_finish - this->_M_start;
        const value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            const value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C    Compare;                                       // +0x0C
    bool Dirty;                                         // +0x0D
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;
class CBaseObject { };
class CThing : public CBaseObject { public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];                  // +0x20 std::bitset<274>
    CInterfaceMap  Components;                          // +0x44
    unsigned char  _pad54[0x91 - 0x54];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const C3DVector& GetPos() const;                    // 0x004C73D0
    template <class T> __forceinline bool GetTC(T** out)          // FableWin ??$GetTC@VCTCFlammable@@@CThing@@QAE_NPAPAVCTCFlammable@@@Z
    {
        ETCInterfaceType type = T::StatGetInterfaceType();
        bool present = ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0;
        if (present) {
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    template <class T> __forceinline bool PeekTC(T** out) const   // FableWin ??$PeekTC@$$CBVCTCFlammable@@@CThing@@QBE_NPAPBVCTCFlammable@@@Z
    {
        ETCInterfaceType type = T::StatGetInterfaceType();
        if ((UsingComponents[type >> 5] & (1UL << (type & 31))) != 0) {
            ETCInterfaceType key = type;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::value_type* it = map->LowerBound(key);
            *out = (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };
class CThingBuilding : public CThing { };
class CTCFlammable { public:                                   // interface 0x6d
    bool IsOnFire() const;                              // 0x00780400
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x6d; }
};
class CBaseObjectPointer;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};
template <class T> inline bool __fastcall operator!=(const CIntelligentPointer<T>& pointer, int value)   // FableWin ??$?9VCThingBuilding@@@@YI_NABV?$CIntelligentPointer@..@@H@Z
{
    return pointer.GetPItem() != (T*)value;
}
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)   // FableWin ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingBuilding@@@@@@YI_NAB..@Z
{
    if (thing != 0 && thing->Dead == 0)
        return true;
    return false;
}
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }   // FableWin ??0CSoundPair@@QAE@JJ@Z
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    unsigned char _pad04[0xa4];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _padA8[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CCharString& animation, const CSoundPair& sound,
                                  bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CCreatureAction_AlertToPlayer : public CCreatureActionBase { public:
    CCreatureAction_AlertToPlayer(CThingAICreature& creature);              // 0x00842270
};
enum EAIActionReturn { };                               // enumerator names not in the name sets (2 done, 4 running)
enum EAIActionRepeat { };
enum EMovementType { };
class CAIBrain { public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                         // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }   // Ego_d ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void Terminate();                                   // 0x008FCF40
protected:
    CAIBrain*     PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                         // ScriptAIPriority, Name, Terminated, Priority, Enabled
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& arrived,
                                                EMovementType movement, bool a, bool b, bool c, const CThing* ignore,
                                                bool d, float e);                               // 0x0097E890
    EAIActionReturn AIAction_PerformAction(const CCreatureActionBase& action);                 // 0x0097D8B0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool b); // 0x0097DB70
    EAIActionReturn AIAction_DoNothing();                                                      // 0x0097E200
};
class CAIStateGroup_MinionPutOutFires : public CAIStateGroupBase { public:
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;   // +0x14
private:
    bool PutOutFire();
};
class CAIStateGroup_MinionWatchFires : public CAIStateGroupBase { public:
    CIntelligentPointer<CThingBuilding> PBuildingOnFire;   // +0x14
    virtual void Update();
};

bool CAIStateGroup_MinionPutOutFires::PutOutFire()
{
    bool arrived = false;
    EAIActionReturn result;
    do {
        if (!IS_THING_ALIVE(PBuildingOnFire))
            return false;
        CTCFlammable* flammable;
        if (!PBuildingOnFire->GetTC(&flammable) || !flammable->IsOnFire())
            return false;
        result = AIAction_MoveToPosProximity((EAIActionRepeat)2, PBuildingOnFire->GetPos(), 3.0f, arrived,
                                             (EMovementType)5, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    if (IS_THING_ALIVE(PBuildingOnFire)) {
        const CTCFlammable* flammable;
        if (PBuildingOnFire->PeekTC(&flammable) && flammable->IsOnFire()) {
            if (AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(0, 0), false, true,
                                                                     false, 0, 0)) == 2)
                return true;
        }
    }
    return false;
}

void CAIStateGroup_MinionWatchFires::Update()
{
    bool arrived = false;
    EAIActionReturn result;
    do {
        if (!IS_THING_ALIVE(PBuildingOnFire))
            goto terminate;
        CTCFlammable* flammable;
        if (!PBuildingOnFire->GetTC(&flammable) || !flammable->IsOnFire())
            goto terminate;
        result = AIAction_MoveToPosProximity((EAIActionRepeat)2, PBuildingOnFire->GetPos(), 5.0f, arrived,
                                             (EMovementType)5, true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result == 2) {
        for (;;) {
            if (!IS_THING_ALIVE(PBuildingOnFire))
                break;
            const CTCFlammable* flammable;
            if (!PBuildingOnFire->PeekTC(&flammable) || !flammable->IsOnFire())
                break;
            if (AIAction_PerformActionAndWait(CCreatureAction_AlertToPlayer(GetCreature()), false) != 2)
                break;
            if (AIAction_DoNothing() != 4)
                return;
        }
    }
terminate:
    Terminate();
}
