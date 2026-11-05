// CAIStateGroup_ScorpionKing::IsStillRunnable() -- PEnemy is alive, the group is not stopping, and the creature's targeting component still targets PEnemy
// Leaf: ?IsStillRunnable@CAIStateGroup_ScorpionKing@@IAE_NXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097B870, 135 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?IsStillRunnable@CAIStateGroup_ScorpionKing@@IAE_NXZ (donor 0x009A0500) sits beside the same neighbours; callers CAIStateGroup_ScorpionKing::UpdateCombat 0x0097BDA0 (x3);
//   PEnemy is the Ego_r.pdb CIntelligentPointer<CThingCreatureBase> at retail +0x30.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CAIStateGroupBase::IsStopping 0x008FD010, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound const 0x004365B0, the targeting component's virtual slot 29 (+0x74; CTCTargetingAI's returns its
//   targeted-thing pointer -- named GetPTargetedThing, the Ego_r.pdb CTCTargetingBase virtual of that shape: probable).
// Shape (family K047, representative 0x0094D930): IS_THING_ALIVE(PEnemy); `if (IsStopping()) return false;`; the GetTC<T>(T**)
//   lookup with the bitset reference test (`mov ecx,[eax+20h] ; test ch,1`) and the const lower bound, its result ignored (a
//   missing component reads the unset local, as the landed J018 GetTC rows); `if (PEnemy.GetPItem() == targeting->GetPTargetedThing()) return true; return false;` (the right operand -- the virtual
//   call -- is evaluated first; `sete`), the entry picked by the ternary so the component loads straight into ecx.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      LifeFlags;                                         // +0x91 bit 0: dead
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            *out = (T*)((it == map->end() || T::INTERFACE_TYPE < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                               // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                                // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->LifeFlags & 1))
            return true;
    }
    return false;
}
class CTCTargetingBase {                                      // interface 8
public:
    enum { INTERFACE_TYPE = 8 };
    virtual ~CTCTargetingBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetPTargetedThing() const;                // slot 29 (+0x74)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                                        // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                         // +0x04
    long ScriptAIPriority;                                    // +0x08
    void* Name_PStringData;                                   // +0x0C
    unsigned char Terminated;                                 // +0x10
    unsigned char Priority;                                   // +0x11
    unsigned char Enabled;                                    // +0x12
protected:
    bool IsStopping() const;                                        // 0x008FD010 (Ego_r.pdb protected const)
public:
};
class CAIStateGroup_ScorpionKing : public CAIStateGroupBase {
protected:
    bool IsStillRunnable();
public:
    unsigned char _pad_0x14[0x30 - 0x14];            // PArenaCentre, TrapThings (12-byte vector), bIsRangeAttack, ChargeCount
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x30 (Ego_r.pdb +52)
};
bool CAIStateGroup_ScorpionKing::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    CTCTargetingBase* targeting;
    PBrain->PCreature->GetTC(&targeting);
    if (PEnemy.GetPItem() == targeting->GetPTargetedThing())
        return true;
    return false;
}