#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#define __forceinline inline
#endif
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
class CTCBase;
class CThing;
class CThingCreatureBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
#undef VF
#define VF(n) void CThing::v##n() {}
VF10() VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF(80) VF(81) VF(82) VF(83) VF(84) VF(85) VF(86) VF(87)
static bool g_isPlayer;
bool CThing::IsPlayerCreature() { return g_isPlayer; }
static int g_kills; static CThing* g_killed[8]; static bool g_killArg;
void CThing::Kill(bool b) { if (g_kills < 8) g_killed[g_kills] = this; g_kills++; g_killArg = b; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CThingManager g_thingManager; CThingManager* g_pThingManager = &g_thingManager;
static int g_setActions; static CThingCreatureBase* g_setActionOn; static const CCreatureActionBase* g_setAction;
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { g_setActions++; g_setActionOn = this; g_setAction = &action; return true; }
static CThing* g_queued; static int g_queues;
void CThingCreatureBase::SetPItemToUnsheatheAfterCutscene(CThing* item) { g_queued = item; g_queues++; }
static int g_erases; static CCreatureActionBase* g_erasedKey; static int g_recalcs;
unsigned long CActionKeyedMap::Erase(CCreatureActionBase* const& key) { g_erases++; g_erasedKey = key; return 1; }
void CTCHaste::RecalculateSpeed() { g_recalcs++; }
static long g_finishedFrames; static int g_setFinished;
void CAnimSet::SetAsFinished(long framesToFadeOver) { g_finishedFrames = framesToFadeOver; g_setFinished++; }
static CAnimSet* g_animSet; static CThingCreatureBase* g_animCreature; static long g_animAction;
CAnimSet* __fastcall GetSingleActionAnimSet(CThingCreatureBase* creature, long action) { g_animCreature = creature; g_animAction = action; return g_animSet; }
static CThing* g_carried[3]; static int g_carriedCount; static int g_removes; static CThing* g_removed[4];
void CTCCarrying::GetAllItems(CThingVector& items)
{
    items._M_start = (CThing**)malloc(sizeof(CThing*) * 4);
    for (int i = 0; i < g_carriedCount; ++i) items._M_start[i] = g_carried[i];
    items._M_finish = items._M_start + g_carriedCount;
    items._M_end_of_storage = items._M_start + 4;
}
void CTCCarrying::RemoveThing(CThing* thing) { if (g_removes < 4) g_removed[g_removes] = thing; g_removes++; }
#define CV(n) void CCreatureActionBase::v##n() {}
CV(00) CV(01) CV(02) CV(03) CV(05) CV(06) CV(07) CV(08) CV(10) CV(11) CV(12) CV(13) CV(14) CV(15) CV(16) CV(17) CV(18) CV(19)
CV(20) CV(21) CV(22) CV(23) CV(24) CV(25) CV(26) CV(27) CV(28) CV(29) CV(30) CV(31) CV(32) CV(33) CV(34) CV(35) CV(36)
static long g_fadeFrames; static bool g_reEquip;
long CCreatureActionBase::GetFadeOutFrames() { return g_fadeFrames; }
bool CCreatureActionBase::ReEquipRemovedItem() { return g_reEquip; }
static CThingCreatureBase* g_unsheatheOwner; static CThing* g_unsheatheItem; static long g_unsheatheTimeout; static int g_unsheatheDtors;
CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* next, long timeout)
{ g_unsheatheOwner = owner; g_unsheatheItem = item; g_unsheatheTimeout = timeout; (void)next; }
CCreatureAction_UnsheatheItemFromInventory::~CCreatureAction_UnsheatheItemFromInventory() { g_unsheatheDtors++; }
static void SetTC(CThing* t, long type) { t->m_presentBits[type >> 5] |= 1UL << (type & 31); }
static void Reset() { g_kills = g_setActions = g_queues = g_erases = g_recalcs = g_setFinished = g_removes = g_unsheatheDtors = 0; g_queued = 0; g_erasedKey = 0; g_animCreature = 0; g_finishedFrames = -1; }
int main()
{
    static CThingCreatureBase creature; static CThing item, carriedA, carriedB;
    static CTCHaste haste; static CTCCarrying carrying; static CAnimSet animSet;
    static CAnimationEntry entry; entry.BankIndex = 77;
    CInterfaceMap::SEntry ce[] = { { 0x46, (CTCBase*)&carrying }, { 0x8c, (CTCBase*)&haste } };
    creature.m_interfaces.first = ce; creature.m_interfaces.last = ce + 2;
    SetTC(&creature, 0x46); SetTC(&creature, 0x8c);
    static CCreatureActionBase action; CCreatureActionBase* a = &action;
    a->PCreature.m_pItem = &creature; a->PItemRemoved.m_pItem = &item;
    a->PAnimEntry_Data = &entry; a->AddedActionAnim = true; a->Finished = false;
    // full path: haste entry dropped, anim set fades over 12 frames, item re-equipped now (no pause), nothing killed
    g_animSet = &animSet; g_fadeFrames = 12; g_reEquip = true; Reset();
    a->FinishAction();
    CHECK(g_erases == 1 && g_erasedKey == a && g_recalcs == 1);
    CHECK(g_animCreature == &creature && g_animAction == 77 && animSet.UpdateMode == 1 && g_setFinished == 1 && g_finishedFrames == 12);
    CHECK(a->Finished == true);
    CHECK(g_setActions == 1 && g_setActionOn == &creature && g_unsheatheOwner == &creature && g_unsheatheItem == &item && g_unsheatheTimeout == 150 && g_unsheatheDtors == 1);
    CHECK(g_queues == 0 && g_kills == 0 && g_removes == 0);
    // zero fade frames: the set's Finished bit is set directly; paused, player: item queued for after the cutscene
    animSet.Finished = 0; g_fadeFrames = 0; g_thingManager.AllEntitesPauseMode = true; g_isPlayer = true; Reset();
    a->FinishAction();
    CHECK(g_setFinished == 0 && animSet.Finished == 1 && g_setActions == 0 && g_queues == 1 && g_queued == &item);
    // paused, AI creature: nothing; no anim action -> no anim set lookup
    g_isPlayer = false; a->AddedActionAnim = false; Reset();
    a->FinishAction();
    CHECK(g_animCreature == 0 && g_setActions == 0 && g_queues == 0);
    // die-with-action things: carried B has 0xFD, A does not; the creature has it too
    g_thingManager.AllEntitesPauseMode = false; g_reEquip = false;
    SetTC(&carriedB, 0xfd); g_carried[0] = &carriedA; g_carried[1] = &carriedB; g_carriedCount = 2; SetTC(&creature, 0xfd); Reset();
    a->FinishAction();
    CHECK(g_setActions == 0 && g_removes == 1 && g_removed[0] == &carriedB && g_kills == 2 && g_killed[0] == &carriedB && g_killed[1] == &creature && g_killArg == true);
    // dead creature: only the finished flag and the re-equip test
    creature.Dead = 1; a->Finished = false; Reset();
    a->FinishAction();
    CHECK(g_erases == 0 && g_kills == 0 && g_removes == 0 && a->Finished == true);
    // item removed is dead: no re-equip
    creature.Dead = 0; item.Dead = 1; g_reEquip = true; g_carriedCount = 0; Reset();
    a->FinishAction();
    CHECK(g_setActions == 0 && g_queues == 0 && g_erases == 1);
    if (fails == 0) printf("00694430_TEST PASS\n"); else printf("00694430_TEST FAIL\n");
    return fails;
}