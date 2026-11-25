// CThingAICreature::OnCreate() -- creature-base create, def initialisation and validation, default weapons; then suspend the AI when the creature has no brain def or its brain def is BRAIN_NULL
// Leaf: ?OnCreate@CThingAICreature@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00833A70, 299 bytes (+5 int3), virtual, `ret`, bool in al.
// Callees: CThingCreatureBase::OnCreate @ 0x00666310, CThingAICreature::InitialiseFromDef @ 0x008338D0,
//   CThingPhysical::ValidateInitialisation @ 0x006A4D60, CThingCreatureBase::CreateDefaultWeapons @ 0x00664680,
//   CThing::GetDefinitionManager @ 0x004C7990, CDefinitionManager::GetDef<CAIBrainDef>(long, CDefPointer*) @ 0x0079BD80
//   (manifest names a propagated CThingPhysicalSwitchDef instantiation), the brain def's virtual GetInstantiationName
//   (slot 10, +0x28; CDefString by value), CDefString::operator==(const char*) @ 0x005B3440 ("BRAIN_NULL" @ 0x01272AE0),
//   the Suspenders set's insert_unique @ 0x00835560 (landed with SetSuspendAnimation 0x00833A10), the def's virtual
//   slot 1 (+4) at a zero count.
// Layout: retail CThingCreatureBase PDef +0xE0 and IsShowingDebugInfo bit 0 of +0x15E (the landed ctor 0x00666090);
//   CThingCreatureDef BrainDef +0xE8 (PDB +0xF0); CThingAICreature AISuspended bit 1 of +0x1C8, Suspenders +0x1CC
//   (the landed OnLevelUnload 0x00833BA0 model).
// Shape: the brain def is a CDefPointer local filled by a same-TU stand-in GetDef (its address does not escape, so the
//   inline AddSuspender(0) key reuses its frame word); dereferencing it drops the release's null test (`dec [edi+4]`)
//   and the no-def arm needs no release at all; each arm returns true through its own tail; both
//   failures share the final `return false`; the GetDef argument goes through an inline const GetCreatureDef()
//   accessor (BrainDef reloaded after the test, as in InitialiseFromDef 0x008338D0).
#include "fable_compiler.h"
class CDefString { public:
    long TablePos;
    bool operator==(const char* text) const;    // 0x005B3440
};
struct CAIBrainDef {
    virtual void v00(); virtual void Destroy();  // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;   // slot 10 (+0x28)
    long m_refCount;                             // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CAIBrainDef>* out);   // 0x0079BD80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CAIBrainDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct SSuspenderNode;
struct SSuspenderIterator {
    SSuspenderNode* m_node;
    SSuspenderIterator() { }
};
struct SSuspenderInsertResult {                 // pair<iterator, bool>
    SSuspenderIterator first;
    bool               second;
    SSuspenderInsertResult() { }
};
struct CSuspenderSet {                          // STLport set<void*>: {header, size}
    SSuspenderNode* m_header;                   // +0
    unsigned long   m_size;                     // +4
    SSuspenderInsertResult InsertUnique(void* const& value);   // 0x00835560
    bool empty() const { return m_size == 0; }
};
class CThingCreatureDef { public:
    unsigned char _pad00[0xe8];
    long BrainDef;                              // +0xe8
};
class CThingCreatureBase { public:
    unsigned char _pad04[0xe0 - 0x04];
    CThingCreatureDef* PDef;                    // +0xe0 CDefPointer<CThingCreatureDef const>::Object
    const CThingCreatureDef* GetCreatureDef() const { return PDef; }
    unsigned char _pad0e4[0x15e - 0xe4];
    bool IsShowingDebugInfo : 1;                // +0x15e bit 0
    protected:
    void CreateDefaultWeapons();
    public:
                // 0x00664680
protected:
    virtual bool OnCreate();                            // 0x00666310 (protected virtual: FableWin / Ego_r ?OnCreate@CThingCreatureBase@@MAE_NXZ)
};
class CThingAICreature : public CThingCreatureBase { public:
    unsigned char _pad160[0x1c8 - 0x160];
    bool          PerformingVillageTaskFlag : 1;    // +0x1c8 bit 0
    bool          AISuspended : 1;                  // +0x1c8 bit 1
    CSuspenderSet Suspenders;                       // +0x1cc
    FABLE_FORCEINLINE void AddSuspender(void* suspender)
    {
        Suspenders.InsertUnique(suspender);
        AISuspended = !Suspenders.empty();
    }
    protected: void InitialiseFromDef(); public:                   // 0x008338D0
protected:
    virtual bool OnCreate();            // protected virtual (both PDBs: MAE)
};

// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3; q1: the PDB member keeps its protected / private access and a friend line gives the caller access):
//   0x004C7990 ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ (was ?GetDefinitionManager@CThingCreatureBase@@QAEPAVCDefinitionManager@@XZ)
//   0x006A4D60 ?ValidateInitialisation@CThingPhysical@@IAE_NXZ (was ?ValidateInitialisation@CThingCreatureBase@@QAE_NXZ)
class CGameDefinitionManager;
class CThingAICreature;
class CThing { public: CGameDefinitionManager const& PeekDefinitionManager() const; };
class CThingPhysical { protected: bool ValidateInitialisation(); friend class CThingAICreature; };
bool CThingAICreature::OnCreate()
{
    if (CThingCreatureBase::OnCreate()) {
        InitialiseFromDef();
        IsShowingDebugInfo = false;
        if (const_cast<CThingPhysical*>(reinterpret_cast<const CThingPhysical*>(this))->ValidateInitialisation()) {
            CreateDefaultWeapons();
            if (PDef->BrainDef > 0) {
                CDefPointer<CAIBrainDef> brainDef;
                const_cast<CDefinitionManager*>(reinterpret_cast<const CDefinitionManager*>(&reinterpret_cast<const CThing*>(this)->PeekDefinitionManager()))->GetDef(GetCreatureDef()->BrainDef, &brainDef);
                if (brainDef->GetInstantiationName() == "BRAIN_NULL")
                    AddSuspender(0);
            } else {
                AddSuspender(0);
            }
            return true;
        }
    }
    return false;
}