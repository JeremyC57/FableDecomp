// CGameScriptInterface::GetStealDuration(const CScriptThing& thing) const -- for a stealable thing (component 0x33) whose item has a stealable sub-def and whose own def has a creature-stats sub-def: the steal duration interpolated linearly from MinDuration (skill 0) to MaxDuration (skill MaxStealSkill), clamped to MaxDuration above MaxStealSkill; 2 otherwise
// Leaf: GetStealDuration
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1360, length 553 bytes (virtual, `ret 4`).
// Callees: CScriptThing slot 11 GetThing, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0, GetPItem 0x00A01B50,
//   CDefClassBase::GetSubDef 0x005130A0 / 0x0059350C (same-TU auto_inline(off) stand-ins that write only the out slot), __ftol2 0x00BFEA70, the defs' slot 1.
// Layout: CThing presence bits +0x20, component map +0x44 (as the landed 0x007BB150), def pointer +0x70; the stealable component's item
//   intelligent pointer at the packed offset +0x0D (as CTCTrespasser +0x2D); the stealable def's MaxStealSkill/MinDuration/MaxDuration at +0xA4/A8/AC
//   and the stats def's skill (int) at +0x28 are read from this body (no PDB match checked).
// Shape: `itemDef && itemDef->GetSubDef(...)` with the sub-def declared before the test; the thing's def is re-fetched twice through an inline
//   by-value GetDef() in one && condition (the two conditionally destroyed temporaries keep retail's flag word); the three floats are named
//   locals in retail's load order (Min, Max, then the skill cap).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
    operator T*() const { return Object; }
};
class CShopDef : public CDefBase { public:
    unsigned char _pad08[0xa4 - 0x08];
    float MaxStealSkill;                        // +0xa4
    float MinDuration;                          // +0xa8
    float MaxDuration;                          // +0xac
};
class CStockItemDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long StealSkill;                            // +0x28
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005130A0 GetSubDef<CShopDef> / 0x0059350C GetSubDef<CStockItemDef> (Ego_r.pdb member template)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CShopDef*)def;
    return true;
}
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CStockItemDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x1c];
    CDefPointer<CDefClassBase> Def;             // +0x70
    template<class T> FABLE_FORCEINLINE const T* GetTC(long wanted) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (const T*)((it == map->end() || wanted < it->first) ? map->end() : it)->second;
    }
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CDefPointer<CDefClassBase> GetDef() const { return Def; }
};
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
#pragma pack(push, 1)
struct CTCStealable {
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> Item;               // +0x0d
};
#pragma pack(pop)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CWorld; class CMainGameComponent; class CDisplayEngine; class CDefinitionManager; class CPlayerManager;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

long CGameScriptInterface::GetStealDuration(const CScriptThing& thing) const
{
    CThing* t = thing.GetPThing();
    if (t && t->HasTC(0x33)) {
        const CTCStealable* stealable = t->GetTC<CTCStealable>(0x33);
        if (stealable->Item.GetPItem()) {
            CDefPointer<CDefClassBase> itemDef = stealable->Item.GetPItem()->GetDef();
            CDefPointer<const CShopDef> stealDef;
            if (itemDef && itemDef->GetSubDef(&stealDef)) {
                float minDuration = stealDef->MinDuration;
                float maxDuration = stealDef->MaxDuration;
                float maxSkill = stealDef->MaxStealSkill;
                CDefPointer<const CStockItemDef> stats;
                if (t->GetDef() && t->GetDef()->GetSubDef(&stats)) {
                    float skill = (float)stats->StealSkill;
                    if (skill > maxSkill)
                        return (long)maxDuration;
                    return (long)((maxDuration - minDuration) / maxSkill * skill + minDuration);
                }
            }
        }
    }
    return 2;
}