// CTCDExperienceOrb::Create(CThing& creature) -- drop a dead creature's experience as orbs: its stats def's ExperienceWorth (1 without a creature def) scaled by the weapon multiplier of the live creature that last attacked it, split into orb values by the first orb's driver, one THING_EXPERIENCE_ORB created at the creature per value, each given its points and the initial velocity constant rotated by a random XY angle and set moving
// Grade: DIFFER(724v707) -- register allocation only: retail keeps the creature in ebp and forms its zero register in edi after the attacker test (`test eax,eax`, `xor edi,edi` at the def-pointer ctor), gives the loop's orb driver edi (the stale pre-loop load `mov edi,[esp+24h]` from the def-index slot) and leaves `count` in its frame word; VC7.1 here keeps the creature in edi, hoists `xor ebp,ebp` into both arms of the attacker test, gives `count` edi and spills the driver to the dead experience slot (frame 0x40 v 0x3c), and re-forms a zero register for the three epilogue null tests (the def release then compares through a temporary instead of `dec [esi+4] ; jne`). 21 probes (orbTC placement x6, loop forms x5, counter types, physics local, single/double driver variables): every other byte matches; a `while`/`++i` loop form (probe 13, 710 v 707) swaps the residue to `experience` taking ebx (pushed at entry) with i/finish exchanging ebx/ebp.
// Leaf: ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077A890, 707 bytes, static `__fastcall` (creature in ecx), plain `ret`.
// Identity (proven): FableWin.pdb / Ego_r.pdb ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z (the CThing& overload beside the
//   landed ?Create@CTCDExperienceOrb@@SIPAVCThing@@JABVC3DVector@@@Z 0x00779E90, which names this address as its caller);
//   the callers are CThingCreatureBase::OnDie 0x00664300 (landed, which spells it `CreateExperienceOrbs` -- NAME_FIXES) and the
//   experience-orb creators that fetch a creature through CBaseIntelligentPointer::GetPItem. The reloc binding
//   ?CreateExperienceOrbs@@YIXPAVCThing@@@Z is that wrong spelling. Catalog "_global::Create" lacks the class.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(const
//   CThing&) 0x006D76C0 (`__fastcall`), CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) const 0x00485815,
//   __ftol2 0x00BFEA70, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get
//   0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410, CThing::GetPos 0x004C73D0,
//   CTCDExperienceOrb::Create(long, const C3DVector&) 0x00779E90, std::vector<CThing*>::_M_insert_overflow 0x00436AC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetTC of interface 0x69), CTCDExperienceOrb::
//   GetExperienceOrbsToCreateForTotalExperience 0x0077A350, SetExperiencePoints 0x00779E10, CTCBase::GetPWorldSeed 0x00686DA0,
//   GFFloatRandom(float, unsigned long*) 0x004978A0 (`__fastcall`), C3DVector::RotateXY(float) 0x00A14040, CTCPhysicsBase::
//   SetAsMoving(bool) 0x006B0130, free 0x00BFEA14 (the two vectors), the inline CDefPointer release (engine/CDefPointer.h).
//   Literal 0x0126AC98 "THING_EXPERIENCE_ORB"; the initial velocity (0.3, 0.0, 0.4) is the 12-byte .data constant 0x01382A68
//   referenced only here (no PDB data name: g_ExperienceOrbInitialVelocity_01382a68 is a placeholder spelling).
// Layout: retail CThing UsingComponents +0x20, Components +0x44, PhysicsTC +0x60, ThingTypeFlags +0x6c (bit 3 = creature),
//   PBaseDef +0x70, Dead bit 0 of +0x91 (the landed OnCollected / OnDie models, Ego_r less the 0x10 the retail CVectorMap
//   saves); CThingCreatureBase PLastAttackedByCreature +0xf4 (OnDie); CCreatureDef PStats +0x40 (Ego_r 64,
//   CCountedPointer<CCreatureStatsDef>, the pointee first); CCreatureStatsDef::ExperienceWorth +0x34 (engine header);
//   CTCPhysicsBase::Velocity +0x28 (engine header); CTCDExperienceOrb interface type 0x69 (bit 9 of UsingComponents[3]).
// Shape: the multiplier a float local initialised to 1.0; one `long experience` (`if (found) experience = ...; else experience = 1;` then
//   `experience = (long)(experience * multiplier)`, one frame slot) guards the rest; the name string in its own block before the two vectors; the orb driver fetched
//   with the PDB out-pointer `GetTC<T>(T**)` into an uninitialised local (retail reads the stale def-index slot when the
//   driver bit is clear); `count = values.size()` cached; `orb = 0; if (i + 1 < count) { orb = Create(..); orbs.push_back(orb); }`
//   at the loop end (VC7.1 threads the `i + 1 < count` test into the loop exit).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCreatureStatsDef.h"

extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);                               // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);              // 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z
    ~CCharString();                                                // 0x0099EAE0
    CCharStringData* PStringData;
};
class C3DVector { public:
    float X, Y, Z;
    void RotateXY(float angle);                                    // 0x00A14040 ?RotateXY@C3DVector@@QAEXM@Z
};
extern C3DVector g_ExperienceOrbInitialVelocity_01382a68;          // 0x01382A68 (0.3, 0.0, 0.4); placeholder name
float __fastcall GFFloatRandom(float range, unsigned long* seed);  // 0x004978A0 ?GFFloatRandom@@YIMMPAK@Z

namespace std {
struct __true_type { __true_type() {} };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                                     // STLport 4.6 (start, finish, end of storage)
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false);
public:
    __forceinline vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector() { if (_M_start) free(_M_start); }
    __forceinline size_type size() const { return size_type(_M_finish - _M_start); }
    __forceinline _Tp& operator[](size_type __n) const { return _M_start[__n]; }
    __forceinline void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
}
class CThing;
// Same-TU body (not graded) of the STLport COMDAT 0x00436AC0: the growth callee only READS its value through the reference.
#pragma auto_inline(off)
template <>
void std::vector<CThing*>::_M_insert_overflow(CThing** __position, CThing* const& __x, const std::__true_type&, unsigned int __fill_len, bool __atend)
{
    unsigned int __size = _M_finish - _M_start;
    unsigned int __len = __size + (__size < __fill_len ? __fill_len : __size);
    CThing** __new_start = __len ? (CThing**)malloc(__len * sizeof(CThing*)) : 0;
    CThing** __new_finish = __new_start;
    for (CThing** __p = _M_start; __p != __position; ++__p, ++__new_finish) new (__new_finish) CThing*(*__p);
    for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish) new (__new_finish) CThing*(__x);
    if (!__atend) for (CThing** __p = __position; __p != _M_finish; ++__p, ++__new_finish) new (__new_finish) CThing*(*__p);
    if (_M_start) free(_M_start);
    _M_start = __new_start; _M_finish = __new_finish; _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)

class CTCBase;
enum ETCInterfaceType { };
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:   // CThing::Components (retail 8 bytes)
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);        // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicsBase { public:
    unsigned char _pad00[0x28];
    C3DVector Velocity;                                            // +0x28 (engine/CTCPhysicsBase.h)
    void SetAsMoving(bool moving);                                 // 0x006B0130 ?SetAsMoving@CTCPhysicsBase@@QAEX_N@Z
};
class CBaseObject;
struct CBaseIntelligentPointer {                                   // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                                                 // +0x04
    CBaseObject* GetPItem() const;                                 // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
};
class CThingBaseDef : public CDefClassBase { };
class CCreatureStatsDefPointer { public: CCreatureStatsDef* Data; };   // CCountedPointer<CCreatureStatsDef>, the pointee first
class CCreatureDef : public CDefClassBase { public:
    unsigned char _pad[0x40 - sizeof(CDefClassBase)];
    CCreatureStatsDefPointer PStats;                               // +0x40 (Ego_r 64)
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                              // +0x20 bitset<274>
    CInterfaceMap Components;                                      // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                                     // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                                  // +0x6c
    CDefPointer<const CThingBaseDef> PBaseDef;                     // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Dead : 1;                                        // +0x91 bit 0
    const C3DVector& GetPos() const;                               // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ
    __forceinline unsigned long HasTC(long type) const
    {
        return UsingComponents[type >> 5] & (1UL << (type & 31));
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)type;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad94[0xf4 - 0x94];
    CBaseIntelligentPointer PLastAttackedByCreature;               // +0xf4
};
namespace NHeroExperience {
float __fastcall GetWeaponModifiedExperienceMultiplierForThing(const CThing& thing);   // 0x006D76C0
}
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const; // 0x009AD410 ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();               // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
};

class CTCBase { public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;                          // 0x00686DA0 ?GetPWorldSeed@CTCBase@@IBEPAKXZ
};
class CTCDExperienceOrb : public CTCBase { public:
    CThing* ParentThing;                                           // +0x04
    static void __fastcall Create(CThing& creature);               // 0x0077A890 (this row)
    static CThing* __fastcall Create(long defIndex, const C3DVector& position);   // 0x00779E90
    void SetExperiencePoints(long points);                         // 0x00779E10 ?SetExperiencePoints@CTCDExperienceOrb@@QAEXJ@Z
private:
    void GetExperienceOrbsToCreateForTotalExperience(long totalExperience, std::vector<long>& orbs) const;   // 0x0077A350
};

void __fastcall CTCDExperienceOrb::Create(CThing& creature)
{
    float multiplier = 1.0f;
    if (creature.ThingTypeFlags & 8) {
        CThing* attacker = (CThing*)static_cast<CThingCreatureBase&>(creature).PLastAttackedByCreature.GetPItem();
        if (attacker && !attacker->Dead)
            multiplier = NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(*attacker);
    }
    long experience;
    CDefPointer<const CCreatureDef> creatureDef;
    if (creature.PBaseDef->GetSubDef(&creatureDef))
        experience = creatureDef->PStats.Data->ExperienceWorth;
    else
        experience = 1;
    experience = (long)(experience * multiplier);
    if (experience) {
        long defIndex;
        {
            CCharString name("THING_EXPERIENCE_ORB", -1);
            defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
        }
        std::vector<CThing*> orbs;
        std::vector<long> orbValues;
        CThing* orb = Create(defIndex, creature.GetPos());
        if (orb && !orb->Dead) {
            orbs.push_back(orb);
            CTCDExperienceOrb* firstOrbTC;
            if (orb->HasTC(0x69))
                firstOrbTC = orb->GetTC<CTCDExperienceOrb>(0x69);
            firstOrbTC->GetExperienceOrbsToCreateForTotalExperience(experience, orbValues);
        }
        unsigned long count = orbValues.size();
        CTCDExperienceOrb* orbTC;
        for (unsigned long i = 0; i < count; i++) {
            if (orb && !orb->Dead) {
                if (orb->HasTC(0x69))
                    orbTC = orb->GetTC<CTCDExperienceOrb>(0x69);
                orbTC->SetExperiencePoints(orbValues[i]);
                C3DVector velocity = g_ExperienceOrbInitialVelocity_01382a68;
                velocity.RotateXY(GFFloatRandom(1.0f, orbTC->GetPWorldSeed()));
                CTCPhysicsBase* physics = orb->PhysicsTC;
                physics->Velocity = velocity;
                physics->SetAsMoving(true);
            }
            orb = 0;
            if (i + 1 < count) {
                orb = Create(defIndex, creature.GetPos());
                orbs.push_back(orb);
            }
        }
    }
}
