// CGameScriptInterface::EntityGetStandingInsideBuilding(const CScriptThing& thing) const -- the building the valid thing stands inside (its kind-0x75 component's intelligent pointer at +0xc), or the null script thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089BC50, length 122 bytes.
// Callees: the script thing's slots 75 (+0x12c) and 11 (+0x2c), the const component-map LowerBound @ 0x004365B0,
//          CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the object-to-script-thing conversion @ 0x004C7E50
//          (manifest label "GFPredicateAnd<...>" is propagated), CGameScriptThing::CreateNullThing() @ 0x008CFF40.
// Shape: a CScriptThing returned by hidden pointer; both returns forward it to the callee.
// Leaf: ?EntityGetStandingInsideBuilding@CGameScriptInterface@@UBE?AVCScriptThing@@ABV2@@Z
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CGameScriptThing : public CScriptThing {      // Ego_r.pdb: CGameScriptThing derives from CScriptThing
public:
    static CScriptThing __fastcall CreateNullThing(); // 0x008CFF40 (?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ)
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();               // 0x00449970
};
class CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
    return t && !t->m_dead;
}
class CBaseObject_Retail { public:
    CScriptThing GetScriptThing();                 // 0x004C7E50
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
struct CTCInsideBuilding_Retail {
    unsigned char _pad[0xc];
    CIntelligentPointer<CBaseObject_Retail> m_building;     // +0x0c
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::EntityGetStandingInsideBuilding(const CScriptThing& thing) const
{
    if (thing.IsAlive()) {
        const CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
        if (t->HasTC(0x75)) {
            CBaseObject_Retail* building = ((CTCInsideBuilding_Retail*)GetTCConst(t, 0x75))->m_building.GetPItem();
            if (building)
                return building->GetScriptThing();
        }
    }
    return CGameScriptThing::CreateNullThing();
}