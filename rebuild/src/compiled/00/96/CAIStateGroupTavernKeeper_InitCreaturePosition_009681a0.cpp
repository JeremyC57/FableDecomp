// CAIStateGroup_TavernKeeper::InitCreaturePosition() -- place the keeper beside the pub when the state is runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009681A0, length 56 bytes (+ int3 padding).
// Callees: virtual slot 11 (+0x2c, IsRunnable) on this, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PPub at +0x24, engine header CAIStateGroup_TavernKeeper.h), 
// CThingCreatureBase::AutoPlaceNearBuilding(CThingBuilding*, float) @ 0x00666EF0.
// Shape: the runnable test first, then `creature->AutoPlaceNearBuilding(PPub.GetPItem(), 5.0f)` with
// the creature in a named local (edi) read before the radius push; both false exits share
// `xor eax,eax` and the true exit is `mov eax,1` -- a long-valued override (same shape as the landed
// IdleAtHome::InitCreaturePosition @ 0x00917330). The base is modelled locally: the generated
// header flattens it and has no vtable.
class CThingBuilding;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingAICreature { public:
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    long InitCreaturePosition();
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00666EF0 ?AutoPlaceNearBuilding@CThingCreatureBase@@QAE_NPBVCThingBuilding@@M@Z (was ?AutoPlaceNearBuilding@CThingAICreature@@QAE_NPAVCThingBuilding@@M@Z)
class CThingCreatureBase { public: bool AutoPlaceNearBuilding(CThingBuilding const*, float); };
long CAIStateGroup_TavernKeeper::InitCreaturePosition()
{
    if (IsRunnable()) {
        CThingAICreature* creature = PBrain->Creature;
        if (reinterpret_cast<CThingCreatureBase*>(creature)->AutoPlaceNearBuilding(PPub.GetPItem(), 5.0f))
            return 1;
    }
    return 0;
}