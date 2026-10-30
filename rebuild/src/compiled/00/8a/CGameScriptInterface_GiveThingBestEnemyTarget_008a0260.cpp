// CGameScriptInterface::GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const -- point a valid thing's AI targeting (TC 8, when it is a CTCTargetingAI) at the target creature, add the target to its enemy list (TC 0x49), and when the target changed for the player creature notify it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A0260, length 397 bytes (`ret 8`).
// Callees: the script things' slots 75 (+0x12C IsValid) and 11 (+0x2C GetThing), the component-map LowerBound @ 0x0040F020,
//   the targeting component's slots 22 (+0x58 GetClassTypeName, CCharString by value) and 29 (+0x74, current target),
//   CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, the string compare @ 0x00411570, CTCTargetingAI::SetScriptTargetOverride
//   @ 0x008ABBA0 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated; name from the landed
//   ClearThingBestEnemyTarget 0x008A03F0), the TC 0x49 enemy-list add @ 0x0076D1B0, the player-creature notify @ 0x006D5AB0.
// Layout: CThing type byte +0x10, UsingComponents +0x20, component map +0x44, flags +0x6C (bit 3 = creature).
// Shape: a same-TU LowerBound stand-in packs the key, literal and class-name result into the dead argument slots as retail;
//   if/else SetScriptTargetOverride calls; the class-name compare is the landed IsActionRunning 0x00712D70 operator== (nested SameText), its bool in bl
//   across the two string dtors; the previous target is a frame word zeroed before the first GetThing.
struct CThing_Retail;
class CCharStringData { public:
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
};
long __fastcall CompareStrings(const char* a, const char* b);   // 0x00411570
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->StringLength != other.PStringData->StringLength)
            return false;
        return CompareStrings(PStringData->PData, other.PStringData->PData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const     // the landed IsActionRunning 0x00712D70 form
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};
struct CTCBase_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;       // slot 22 (+0x58)
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing_Retail* GetCurrentTarget();          // slot 29 (+0x74)
};
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (STLport lower_bound over the 8-byte entries):
// compiled above the caller so VC7.1 knows the key's address does not escape and packs the frame words as retail
// (the chunk 242 / landed GetItemDefNamesFromContainer 0x008A8F20 idiom).
CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing_Retail {
    unsigned char         _pad00[0x10];
    unsigned char         m_type;                   // +0x10 (1 = the player creature)
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x20];
    unsigned char         m_flags;                  // +0x6c (bit 3 = creature)
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCTargetingAI : public CTCBase_Retail { public: void SetScriptTargetOverride(CThing_Retail* target); };   // 0x008ABBA0
struct CTCEnemyList { void AddEnemy(CThing_Retail* enemy); };                                       // 0x0076D1B0
struct CThingPlayerCreature_Retail { void OnBestEnemyTargetChanged(); };                            // 0x006D5AB0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const
{
    if (!thing.IsAlive())
        return;
    CThing_Retail* previous = 0;
    CThing_Retail* owner = ((CThing_Retail*)thing.GetPThing());
    if (owner->HasInterface(8)) {
        CTCBase_Retail* targeting = owner->GetInterface(8);
        bool isAI;
        {
            CCharString aiClass("CTCTargetingAI");
            isAI = targeting->GetClassTypeName() == aiClass;
        }
        if (isAI) {
            previous = targeting->GetCurrentTarget();
            if (target.IsAlive() && (((CThing_Retail*)target.GetPThing())->m_flags & 8))
                static_cast<CTCTargetingAI*>(targeting)->SetScriptTargetOverride(((CThing_Retail*)target.GetPThing()));
            else
                static_cast<CTCTargetingAI*>(targeting)->SetScriptTargetOverride(0);
        }
    }
    if (target.IsAlive()) {
        CThing_Retail* owner2 = ((CThing_Retail*)thing.GetPThing());
        if (owner2->HasInterface(0x49))
            ((CTCEnemyList*)owner2->GetInterface(0x49))->AddEnemy(((CThing_Retail*)target.GetPThing()));
    }
    if (previous != ((CThing_Retail*)target.GetPThing()) && ((CThing_Retail*)thing.GetPThing())->m_type == 1)
        ((CThingPlayerCreature_Retail*)((CThing_Retail*)thing.GetPThing()))->OnBestEnemyTargetChanged();
}