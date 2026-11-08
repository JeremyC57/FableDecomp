// CGameScriptInterface::CreateCreatureNearby(const CCharString& defName, const C3DVector& position, float radius, const CCharString& scriptName, bool spawnEffect) const -- create the creature at the position, then, if it lives and has a navigation component, move it to a nearby wander position within a random radius (0.8 when the radius is no larger)
// Leaf: ?CreateCreatureNearby@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@M0_N@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "fable_compiler.h"
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
};
#include "engine/C3DVector.h"
class CTCBase;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
class CThing;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites: defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CThing* CScriptThing::GetPThing() const { if (PImp.IsNull()) return 0; return PImp->GetPThing(); }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }

enum ETCInterfaceType { TC_CREATURE_NAVIGATION = 7 };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
#pragma auto_inline(off)
__declspec(noinline) const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::SEntry*
CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::LowerBound(const ETCInterfaceType& type) const
{
    const SEntry* lo = first;
    int count = last - first;
    while (count > 0) {
        int half = count >> 1;
        const SEntry* mid = lo + half;
        if (mid->first < type) {
            lo = mid + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};
class CThing { public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    const C3DVector& GetPos() const;                    // 0x004C73D0
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE const CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};
class CTCCreatureNavigation { public:
    bool GetNearbyWanderPosition(const C3DVector& position, float radius, C3DVector& result, bool onlyNavigable) const;   // 0x006CB6E0
};
class CWorld { public:
    unsigned long* GetWorldSeed() const;                // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::CreateCreatureNearby(const CCharString& defName, const C3DVector& position, float radius, const CCharString& scriptName, bool spawnEffect) const
{
    CScriptThing creature = CreateCreature(defName, position, scriptName, spawnEffect);
    if (creature.IsAlive()) {
        CThing* thing = creature.GetPThing();
        if (thing->HasTC(TC_CREATURE_NAVIGATION)) {
            const CTCCreatureNavigation* navigation = (const CTCCreatureNavigation*)thing->GetTC(TC_CREATURE_NAVIGATION);
            float range = 0.8f;
            if (radius > 0.8f)
                range = GFFloatRandom(radius - 0.8f, World->GetWorldSeed()) + 0.8f;
            C3DVector wanderPosition;
            if (navigation->GetNearbyWanderPosition(creature.GetPThing()->GetPos(), range, wanderPosition, false))
                creature.GetPThing()->PhysicsTC->ResetPosition(wanderPosition);
        }
    }
    return creature;
}