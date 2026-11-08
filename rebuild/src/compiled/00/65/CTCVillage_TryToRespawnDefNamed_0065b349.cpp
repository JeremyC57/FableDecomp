// CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position) -- finds a live village dummy whose dummy-villager component stands for a dead villager of that def and has it create the villager again; null when none does
// Leaf: TryToRespawnDefNamed
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad24[0x20] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B349, 183 bytes (`ret 8`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb
// `?TryToRespawnDefNamed@CTCVillage@@QAEPAVCThingAICreature@@ABVCCharString@@ABVC3DVector@@@Z`.
// Callees: CGameDefinitionManager::Get() 0x0044C6B0 (static `__fastcall`); CDefinitionManager::GetDefGlobalIndexFromName(
//          const CCharString&) 0x009AD410; CBaseIntelligentPointer::GetPItem() const 0x00A01B50; IS_THING_ALIVE(
//          CBaseObject* const&) 0x0040DCE0 and IS_THING_DEAD(CThing* const&) 0x0065D639 (`__fastcall`); std::bitset<>::
//          test(size_t) const 0x00410DE0 (interface 0xD6, the dummy villager); CVectorMap<ETCInterfaceType, CTCBase*>::
//          LowerBound 0x0040F020 (inlined GetTC); CTCDummyVillager::CreateVillager(CTCVillage*, const C3DVector&, bool)
//          0x0072F750.
// Layout: retail CTCVillage VillageDummies +0x74 (bare list head pointer, PDB +0xf8; as the landed sibling
//         DetachFromDummyVillager 0x0065B2C4); CTCDummyVillager PVillager +0x30, the villager def index +0x0c.
// Shape: `this` is spilled to [ebp-4] for the end re-read; the dummy binds IS_THING_ALIVE's const reference through a
//        derived-to-base temporary homed in the dead `defName` slot, which also holds the GetTC key (0xD6, hoisted into
//        ebx) and the villager pointer temporary of IS_THING_DEAD; the matching dummy's CreateVillager result is returned.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
#include "engine/C3DVector.h"
class CCharString { public: void* m_data; };

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CBaseObject { public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    FABLE_FORCEINLINE CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};
class CThing : public CBaseObject { public: };
class CThingAICreature : public CThing { public: };
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);   // 0x0040DCE0
bool ThingAliveBit_s7(const CBaseObject* thing);   // stand-in: the dead-flag test (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU body of the IS_THING_ALIVE<CBaseObject> instantiation (0x0040DCE0): reads the pointer through its const reference
// only, so the caller's derived-to-base temporary does not escape.
__declspec(noinline) bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing)
{
    if (thing == 0)
        return false;
    return ThingAliveBit_s7(thing);
}
#pragma auto_inline(on)
bool __fastcall IS_THING_DEAD(CThing* const& thing);         // 0x0065D639
#pragma auto_inline(off)
// Same-TU body of the IS_THING_DEAD<CThing> instantiation (0x0065D639): reads the pointer through its const reference only.
__declspec(noinline) bool __fastcall IS_THING_DEAD(CThing* const& thing)
{
    if (thing == 0)
        return true;
    return !ThingAliveBit_s7(thing);
}
#pragma auto_inline(on)

class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};

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
class CTCVillage;
class CTCDummyVillager { public:                       // interface 0xD6
    unsigned char _pad_0x00[0x0c];
    long          VillagerDefIndex;             // +0x0c
    unsigned char _pad_0x10[0x20];
    CIntelligentPointer<CThing> PVillager;          // +0x30
    CThingAICreature* CreateVillager(CTCVillage* village, const C3DVector& position, bool flag);   // 0x0072F750
};
CThingAICreature* CreateVillagerSink_s7(CTCDummyVillager* dummy, CTCVillage* village, float x, float y, float z, bool flag);
#pragma auto_inline(off)
// Same-TU stand-in for CreateVillager (0x0072F750): reads the position through its const reference, lets nothing escape.
__declspec(noinline) CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage* village, const C3DVector& position, bool flag)
{
    return CreateVillagerSink_s7(this, village, position.X, position.Y, position.Z, flag);
}
#pragma auto_inline(on)
struct SThingNode {                             // STLport list node
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CIntelligentPointer<CThing> _M_data;            // +0x08
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x74];
    SThingNode*   VillageDummies;               // +0x74 list<CIntelligentPointer<CThing> > head node
    CThingAICreature* TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position);
};

CThingAICreature* CTCVillage::TryToRespawnDefNamed(const CCharString& defName, const C3DVector& position)
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    for (SThingNode* it = VillageDummies->_M_next; it != VillageDummies; it = it->_M_next) {
        CThing* dummy = it->_M_data.GetPItem();
        if (IS_THING_ALIVE(dummy) && dummy->HasTC(0xd6)) {
            CTCDummyVillager* villager = (CTCDummyVillager*)dummy->GetTC(0xd6);
            if (IS_THING_DEAD(villager->PVillager.GetPItem()) && villager->VillagerDefIndex == defIndex)
                return villager->CreateVillager(this, position, true);
        }
    }
    return 0;
}