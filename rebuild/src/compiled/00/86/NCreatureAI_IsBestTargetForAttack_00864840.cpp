// NCreatureAI::IsBestTargetForAttack(const CThingAICreature&, const CThingCreatureBase*) -- true when the target is alive and is the creature's targeting AI's best enemy for attack
// Leaf: ?IsBestTargetForAttack@
// Retail 0x00864840, 0x56 bytes (+ int3 padding), __fastcall (creature in ecx, target in edx). PDB (Ego_r):
// ?IsBestTargetForAttack@NCreatureAI@@YI_NABVCThingAICreature@@PBVCThingCreatureBase@@@Z. Callers 0x00864D32/0x00864D92
// (same TU) and CAIStateGroup_CreatureRangedCombat2::IsRunnable 0x0091EB60 (landed, calls it __fastcall).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0; CTCTargetingAI vtable slot 29 (+0x74)
// = GetPBestEnemyCreatureForAttack() const (Ego_r ?GetPBestEnemyCreatureForAttack@CTCTargetingAI@@UBEPAVCThingCreatureBase@@XZ).
// Interface type 8 = CTCTargetingAI (GetInterfaceType, vtable slot 21): bitset word 0 bit 8 (`mov eax,[ecx+20h] ; test ah,1`).
// Layout (retail CThing, as the landed CThingSwitch rows): interface bitset +0x20, interface map +0x44, Dead bit 0 of +0x91.
// Shape: target null test, Dead bitfield test, HasTC(8), then the landed 0x0091EB60 FindEntry lookup (entry returned,
// `->component` read at the call site) so the component loads straight into ecx for the virtual call; the GetInterface
// out-pointer form and a PeekTC returning the component both load it into eax and add `mov ecx,eax`. Two `return`
// exits share `xor al,al`.
// CThing is modelled locally: rebuild/include/engine has no CThing.h and the CThingCreatureBase/CThingAICreature
// headers pad the CThing bytes.
class CTCBase;
class CThingCreatureBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTargetingAI                                 // interface type 8
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const;   // slot 29 (+0x74)
};

class CThing
{
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 UsingComponents bitset<274>
    CInterfaceMap m_interfaces;                      // +0x44 Components
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;

    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CInterfaceMap::SEntry* FindEntry(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        const CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
};

class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase { };

namespace NCreatureAI
{
    bool __fastcall IsBestTargetForAttack(const CThingAICreature& creature, const CThingCreatureBase* target);
}

bool __fastcall NCreatureAI::IsBestTargetForAttack(const CThingAICreature& creature, const CThingCreatureBase* target)
{
    if (target && !target->Dead)
    {
        if (creature.HasTC(8) && ((const CTCTargetingAI*)creature.FindEntry(8)->second)->GetPBestEnemyCreatureForAttack() == target)
            return true;
    }
    return false;
}