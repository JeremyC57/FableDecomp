#ifndef FABLE_ENGINE_CTCWEAPONTRAIL_H
#define FABLE_ENGINE_CTCWEAPONTRAIL_H
// Retail opt-in (Codex pass18): full PDB fields, CTCBase hierarchy, native STLport12-byte vectors/4-byte lists.
#include "engine/CTCBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitiveWeaponTrail.h"
#include "fable_string.h"
#ifdef FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
class CThingCreatureBase;
class CEnginePrimitivePrepareInfo;
#endif
class CTCWeaponTrail : public CTCBase {
#ifdef FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
public: static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
public: static CCharString FABLE_FASTCALL StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCWeaponTrail(const CTCWeaponTrail & p0);
private: CTCWeaponTrail(CThing & p0);
public: void UpdateWeaponTrails();
public: void ClearAllWeaponTrails();
public: long GetNoTrails() const;
public: void GetAllTrailsLastWeaponTrailPoints(std::vector<std::vector<std::pair<C3DVector,C3DVector>,std::allocator<std::pair<C3DVector,C3DVector> > >,std::allocator<std::vector<std::pair<C3DVector,C3DVector>,std::allocator<std::pair<C3DVector,C3DVector> > > > > & p0) const;
private: bool GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector>,std::allocator<std::pair<C3DVector,C3DVector> > > & p0,const std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,std::allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > & p1) const;
public: bool GetLastWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector>,std::allocator<std::pair<C3DVector,C3DVector> > > & p0,long p1) const;
public: bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector>,std::allocator<std::pair<C3DVector,C3DVector> > > & p0) const;
public: void SetLastProjectileWeaponTrailPoint(const C3DVector & p0);
public: const std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,std::allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > & GetWeaponTrailList(long p0) const;
public: void DrawAddWeaponTrails(const CEnginePrimitivePrepareInfo & p0) const;
public: long GetStrikeEventIndexFromEventName(const CCharString & p0);
public: bool HasWeaponTrailPoints() const;
private: virtual void OnCreate();
private: virtual void OnSetInLimbo(bool p0);
private: void InitialiseTrails();
private: void CalculateInterpolatedWeaponHelpers(long p0,float p1,C3DVector & p2,C3DVector & p3) const;
private: bool IsTrailActiveForThing(long p0,const CThing * p1,const CThingCreatureBase * p2) const;
private: const std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,std::allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > & GetTrail(long p0) const;
private: std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry,std::allocator<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > & GetTrail(long p0);
public: virtual ~CTCWeaponTrail();
public: CTCWeaponTrail & operator=(const CTCWeaponTrail & p0);
#else
public:
    void SetLastProjectileWeaponTrailPoint(const C3DVector& point);
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
bool GetLastProjectileWeaponTrailPoints(std::vector<std::pair<C3DVector,C3DVector> >&)const;
#endif
#endif
private:
    long NoTrails;
    long NoProjectileTrails;
    long WeaponTrailGraphicIndex;
    CEnginePrimitiveHandle ProjectileWeaponTrailHandle;
    std::vector<CEnginePrimitiveHandle> WeaponTrailHandles;
    CEnginePrimitiveWeaponTrail::EWeaponTrailType TrailType;
    std::vector<bool> ActiveTrail;
    std::vector<long> InactiveTicks;
    std::vector<std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> > Trail;
    std::vector<std::pair<CCharString,CCharString> > HelperNames;
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> ProjectileWeaponTrail;
    long MinProjectileTrailTimer;
    bool WeaponTrailActive;
};
FABLE_STATIC_ASSERT(sizeof(CTCWeaponTrail)==108);
#endif
