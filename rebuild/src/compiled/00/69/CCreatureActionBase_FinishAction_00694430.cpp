// CCreatureActionBase::FinishAction() -- base action finish: drop this action from the creature's haste component, fade out the action's single anim set, mark the action finished, re-equip the item removed for it (now, or after the cutscene for the player), and kill carried things and the creature itself when they carry the die-with-action interface 0xFD
// Leaf: ?FinishAction@CCreatureActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00694430, 656 bytes, plain `ret`; virtual slot 4 of CCreatureActionBase.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PItemRemoved +0x2c), CVectorMap::LowerBound 0x0040F020
//   (inline lookups 0x8C CTCHaste, 0x46 CTCCarrying), the haste set erase-by-key 0x0058119B and recalculation 0x00800020
//   (manifest labels CBankFile::CompareDataOffset::operator() / CAnimation_RecalculateScale are propagated),
//   GetSingleActionAnimSet 0x00693250 (__fastcall), CAnimSet::SetAsFinished 0x00668A50, this's virtual slots 9 (+0x24, the
//   fade-out frame count) and 37 (+0x94, whether to re-equip PItemRemoved), CCreatureAction_UnsheatheItemFromInventory ctor
//   0x006A0150 and its folded destructor 0x005CC091, CThingCreatureBase::SetCurrentAction 0x006644F0, the creature's
//   virtual slot 88 (+0x160: CThingPlayerCreature returns true, CThingAICreature false), CThingCreatureBase::
//   SetPItemToUnsheatheAfterCutscene 0x00662450 (sets +0x150; manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is
//   wrong), CTCCarrying::GetAllItems 0x006A2220 (manifest "COptimisedPrimitive::RemapGroups" is propagated) / RemoveThing
//   0x006A19B0, free 0x00BFEA14 (the item vector's buffer), CThing::Kill(bool) 0x004C9B80.
// Globals: g_pThingManager 0x013B8A1C (NonScriptedEntitesPauseMode +0x54, AllEntitesPauseMode +0x55).
// Layout: Ego_r.pdb CCreatureActionBase agrees with retail below +0x84 (PCreature +8, PItemRemoved +0x2c, AddedActionAnim
//   +0x60, Finished +0x61, PAnimEntry +0x74) but its vectors/map are 16 bytes (retail 12, sizeof 0xa8) and the generated
//   header carries the intelligent pointers as opaque bytes, so `this` is modelled locally. CThing: interface bitset +0x20
//   (0xFD = bit 29 of +0x3c), map +0x44, dead bit +0x91; CAnimSet UpdateMode +0, Finished bit 1 of +0x51.
// Shape: the anim entry is a named local read after the alive test (retail keeps it in esi across GetPItem); the pause test
//   reads the thing manager through a STATIC creature accessor (`PCreature.GetPItem()->PeekThingManager()`): the call is
//   still made for its side effects and the global load follows it (a non-static inline hoists the global above the call);
//   the haste and carrying components use the out-parameter GetInterface; the carried-thing vector is released with free.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
class CTCBase;
class CThing;
class CThingCreatureBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10() VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF(80) VF(81) VF(82) VF(83) VF(84) VF(85) VF(86) VF(87)
    virtual bool IsPlayerCreature();        // slot 88 (+0x160): CThingPlayerCreature true, CThingAICreature false
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                 // +0x91 bit 0
    void Kill(bool b);                      // 0x004C9B80
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
class CThingCreatureBase;
struct CCreaturePointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)GetPItem(); }
};
class CThingManager { public:
    unsigned char _pad00[0x54];
    bool          NonScriptedEntitesPauseMode;          // +0x54
    bool          AllEntitesPauseMode;                  // +0x55
};
extern CThingManager* g_pThingManager;                  // 0x013B8A1C
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    static CThingManager* PeekThingManager() { return g_pThingManager; }
    void SetPItemToUnsheatheAfterCutscene(CThing* item);        // 0x00662450
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CActionKeyedMap {                    // std::map keyed by the action pointer
    unsigned long Erase(CCreatureActionBase* const& key);   // 0x0058119B
};
class CTCHaste { public:                           // interface type 0x8C component
    unsigned char _pad00[0x18];
    CActionKeyedMap SpeedSources;           // +0x18
    void RecalculateSpeed();                // 0x00800020
};
class CAnimSet { public:
    long UpdateMode;                        // +0x00
    unsigned char _pad04[0x51 - 0x04];
    unsigned char Looping : 1;              // +0x51
    unsigned char Finished : 1;
    void SetAsFinished(long framesToFadeOver);   // 0x00668A50
};
CAnimSet* __fastcall GetSingleActionAnimSet(CThingCreatureBase* creature, long action);   // 0x00693250
class CAnimationEntry { public:
    long BankIndex;                         // +0x00
};
struct CThingVector {                       // vector<CThing*> over a malloc'd buffer
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
};
class CTCCarrying { public:                        // interface type 0x46
    void GetAllItems(CThingVector& items);  // 0x006A2220
    void RemoveThing(CThing* thing);        // 0x006A19B0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetFadeOutFrames();        // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36();
    virtual bool ReEquipRemovedItem();      // slot 37 (+0x94)
    unsigned char _pad04[4];                // +0x04
    CCreaturePointer PCreature;             // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x2c - 0x10];
    CBaseIntelligentPointer PItemRemoved;   // +0x2c CIntelligentPointer<CThing>
    unsigned char _pad34[0x60 - 0x34];
    bool AddedActionAnim;                   // +0x60
    bool Finished;                          // +0x61
    unsigned char _pad62[0x74 - 0x62];
    CAnimationEntry* PAnimEntry_Data;       // +0x74 CCountedPointer<CAnimationEntry>
    void* PAnimEntry_Info;                  // +0x78
    unsigned char _pad7c[0xa8 - 0x7c];
};
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* next,
                                               long timeout);   // 0x006A0150
    ~CCreatureAction_UnsheatheItemFromInventory();      // 0x005CC091
};

void CCreatureActionBase::FinishAction()
{
    if (PCreature.GetPItem() && !PCreature.GetPItem()->Dead) {
        CTCHaste* haste;
        if (PCreature.GetPItem()->GetInterface(&haste, 0x8c)) {
            haste->SpeedSources.Erase(this);
            haste->RecalculateSpeed();
        }
    }
    if (PCreature.GetPItem() && !PCreature.GetPItem()->Dead) {
        CAnimationEntry* entry = PAnimEntry_Data;
        if (entry && entry->BankIndex && AddedActionAnim) {
            CAnimSet* animSet = GetSingleActionAnimSet(PCreature.GetCreature(), entry->BankIndex);
            if (animSet) {
                animSet->UpdateMode = 1;
                long frames = GetFadeOutFrames();
                if (frames > 0)
                    animSet->SetAsFinished(frames);
                else
                    animSet->Finished = 1;
            }
        }
    }
    Finished = true;
    if (ReEquipRemovedItem()) {
        if (PItemRemoved.GetPItem() && !PItemRemoved.GetPItem()->Dead) {
            if (!PCreature.GetCreature()->PeekThingManager()->NonScriptedEntitesPauseMode &&
                !PCreature.GetCreature()->PeekThingManager()->AllEntitesPauseMode) {
                PCreature.GetCreature()->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(PCreature.GetCreature(), PItemRemoved.GetPItem(), 0, 150));
            }
            else if (PCreature.GetCreature()->IsPlayerCreature()) {
                PCreature.GetCreature()->SetPItemToUnsheatheAfterCutscene(PItemRemoved.GetPItem());
            }
        }
    }
    if (PCreature.GetPItem() && !PCreature.GetPItem()->Dead) {
        CTCCarrying* carrying;
        if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
            CThingVector items;
            carrying->GetAllItems(items);
            for (CThing** it = items._M_start; it != items._M_finish; ++it) {
                if ((*it)->HasTC(0xfd)) {
                    carrying->RemoveThing(*it);
                    (*it)->Kill(true);
                }
            }
        }
        if (PCreature.GetPItem()->HasTC(0xfd))
            PCreature.GetPItem()->Kill(true);
    }
}