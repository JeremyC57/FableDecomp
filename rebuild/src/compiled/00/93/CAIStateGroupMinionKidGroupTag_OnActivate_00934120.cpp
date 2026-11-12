// CAIStateGroup_MinionKidGroupTag::OnActivate() -- put the kid in creature mode 1 (remembering whether this
// Leaf: ?OnActivate@CAIStateGroup_MinionKidGroupTag@@UAEXXZ
// Naming (agentNF31, 2026-11-12): callee 0x008137B0 respelled SetTagger(CThing*) -> SetTagger(const CThingAICreature*) (Ego_r.pdb
//   0x00811460 + FableWin.pdb 0x026C3B20); the brain's creature (Ego_r CAIBrain::Creature, CThingAICreature, +0x18) is passed as is.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// state group added it), register the kid with the tag game and make it the tagger when it started the game or
// nobody is "it".
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00934120, length 151 bytes (+ int3 padding). Vtable slot 2.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = CTCCreatureModeManager, bit
// 17 of the bitset dword at +0x24), CTCCreatureModeManager::IsModeActive @ 0x0073A8A0 and AddMode @ 0x0073AE10,
// the group-tag component's virtual slot 31 (+0x7c) with the creature, CTCCreatureGroupTag::GetTagger
// @ 0x008138E0 and SetTagger @ 0x008137B0.
// Shape: the mode manager is an uninitialised local filled by the inlined lookup (miss path reads the slot);
// `if (!HasMode(1)) { StandModeAddedToCreature = true; AddMode(1); } else StandModeAddedToCreature = false;`
// then the slot-31 call, then `if (InitiatedTag || GetTagger() == 0) SetTagger(creature)` (bool read into al).
// Members are the engine header names (InitiatedTag +0x14, PTCGroupTag +0x18, StandModeAddedToCreature +0x28);
// the header flattens the base, so the class is modelled locally like its landed siblings.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CTCCreatureModeManager { public:                 // interface type 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CThingAICreature;
class CTCCreatureGroupTag { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void AddPlayer(CThing* creature);   // slot 31 (+0x7c)
    CThing* GetTagger();                        // 0x008138E0
    bool SetTagger(const CThingAICreature* tagger);   // 0x008137B0 Ego_r / FableWin ?SetTagger@CTCCreatureGroupTag@@QAE_NPBVCThingAICreature@@@Z
};

struct CAIStateGroup_MinionKidGroupTag : CAIStateGroupBase {
    bool InitiatedTag;                          // +0x14
    CTCCreatureGroupTag* PTCGroupTag;           // +0x18
    float InitialTagPos_X, InitialTagPos_Y, InitialTagPos_Z;    // +0x1c
    bool StandModeAddedToCreature;              // +0x28
    long FrameLastPlayedIn;                     // +0x2c
    virtual void OnActivate();
};

void CAIStateGroup_MinionKidGroupTag::OnActivate()
{
    CTCCreatureModeManager* pModes;
    PBrain->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
    if (!pModes->IsModeActive(1)) {
        StandModeAddedToCreature = true;
        pModes->AddMode(1);
    } else {
        StandModeAddedToCreature = false;
    }
    PTCGroupTag->AddPlayer(PBrain->PCreature);
    if (InitiatedTag || PTCGroupTag->GetTagger() == 0)
        PTCGroupTag->SetTagger((const CThingAICreature*)PBrain->PCreature);   // Ego_r CAIBrain::Creature +0x18 is the CThingAICreature (this local model types it CThing)
}