// CTCTavernBar::GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot) -- pick a random jug slot and claim the matching free "ACTION_POINT_BARMAN_0<n>" named action point for the creature (position, orientation, action point index)
// Leaf: GetRandomBarSlot
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BA3C0, 290 bytes, `ret 14h`. Ego_r.pdb
//   `?GetRandomBarSlot@CTCTavernBar@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 0x2E = CTCNamedUserActionPoints), CTCBase::GetPWorldSeed() const
//   @ 0x00686DA0, GFROR13 @ 0x00497890 (`__fastcall`), CCharString() @ 0x0099E4B0, CCharString(const char*, long) @ 0x0099EBF0
//   ("ACTION_POINT_BARMAN" @ 0x0126DCD4), CCharString::operator+=(const char*) @ 0x0099F100 ("_0" @ 0x0126DCBC),
//   GFIntToCharString(long) @ 0x0099F830 (`__fastcall`, hidden result in ecx), CCharString::operator+=(const CCharString&)
//   @ 0x0099F0A0, ~CCharString @ 0x0099EAE0, the by-value copy @ 0x0099EC30, CTCNamedUserActionPoints::
//   GetFreeNamedActionPoint(CThing*, C3DVector*, CRightHandedSet*, CCharString, long*, long, bool) @ 0x007FF190.
// Layout: Ego_r.pdb CTCTavernBar Jugs (vector<CIntelligentPointer<CThingObject> >, 8-byte elements) +0x0C; retail CThing
//   interface bitset +0x20 (0x2E = word +0x24 bit 14), map +0x44.
// Shape: the uninitialised out pointer of the out-parameter GetInterface (a miss reads whatever the dead slot holds); the
//   jug count taken before the seed; the landed WorldRandomRange step inline (`range ? seed % range : 0`); a default
//   CCharString declared first and never used; the point name built by += and passed by value (declared copy ctor).
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public: float m[6]; };
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830
long __fastcall GFROR13(unsigned long);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCNamedUserActionPoints { public:
    bool GetFreeNamedActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, CCharString name, long& index,
                                 long flags, bool reserve);   // 0x007FF190 ?GetFreeNamedActionPoint@CTCNamedUserActionPoints@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@VCCharString@@AAJJ_N@Z (Ego_r.pdb; NF28: references)
};
struct CJugPointer { void* m_vfptr; void* m_pItem; };    // CIntelligentPointer<CThingObject>
class CTCBase
{
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
};

class CTCTavernBar : public CTCBase { public:
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    CJugPointer*  Jugs_start;                           // +0x0c vector<CIntelligentPointer<CThingObject> >
    CJugPointer*  Jugs_finish;                          // +0x10
    CJugPointer*  Jugs_end_of_storage;                  // +0x14
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);
};

bool CTCTavernBar::GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot)
{
    CTCNamedUserActionPoints* points;
    ParentThing->GetTC(TCI_NAMED_USER_ACTION_POINTS, &points);
    unsigned long count = Jugs_finish - Jugs_start;
    slot = WorldRandomRange(*GetPWorldSeed(), count);
    CCharString unused;
    CCharString pointName("ACTION_POINT_BARMAN", -1);
    pointName += "_0";
    pointName += GFIntToCharString(slot + 1);
    if (points->GetFreeNamedActionPoint(creature, position, orientation, pointName, actionPoint, 0, true))
        return true;
    return false;
}