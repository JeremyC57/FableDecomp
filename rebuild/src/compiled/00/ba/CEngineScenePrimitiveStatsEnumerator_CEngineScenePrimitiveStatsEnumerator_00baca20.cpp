// CEngineScenePrimitiveStatsEnumerator::CEngineScenePrimitiveStatsEnumerator(std::map& destination, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long statFlags) -- the scene-enumerator base, then the destination map, the 2D area and the stat selection
// Leaf: ??0CEngineScenePrimitiveStatsEnumerator@@QAE@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BACA20, 68 bytes, thiscall, `ret 10h`, returns this.
// Identity (proven): stores vtable 0x012A534C (RTTI .?AVCEngineScenePrimitiveStatsEnumerator@@ : CEngineSceneEnumerator :
//   CBaseClassNonCopyable : CBaseClass; slot 1 is the landed ProcessPrimitive 0x00BACA70); Ego_r.pdb
//   ??0CEngineScenePrimitiveStatsEnumerator@@QAE@AAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z
//   (donor right before ProcessPrimitive).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (CEngineSceneEnumerator's inline constructor).
// Layout (Ego_r.pdb): Destination map<unsigned long, unsigned long>* +0x04, Area C2DBoxF +0x08, StatIndex +0x18, StatFlags +0x1C.
// Shape: four member initialisers in declaration order; Destination holds the reference's address; the box copied by C2DBoxF's
//   memberwise user copy constructor (FableWin.pdb ??0C2DBoxF@@QAE@ABV0@@Z; the implicit one copies through `lea ecx`).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};

namespace std {
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map;
}

class C2DBoxF {
public:
    C2DBoxF(const C2DBoxF& b) : TLX(b.TLX), TLY(b.TLY), BRX(b.BRX), BRY(b.BRY) { }
    float TLX, TLY, BRX, BRY;
};
enum EEnginePrimitiveStatIndex { };
class CEngineInternalPrimitiveBase;

class CEngineSceneEnumerator : public CBaseClassNonCopyable {
public:
    virtual void ProcessPrimitive(CEngineInternalPrimitiveBase& primitive) = 0;
};

class CEngineScenePrimitiveStatsEnumerator : public CEngineSceneEnumerator {
public:
    CEngineScenePrimitiveStatsEnumerator(std::map<unsigned long, unsigned long>& destination, const C2DBoxF& area,
                                         EEnginePrimitiveStatIndex statIndex, unsigned long statFlags);
    virtual void ProcessPrimitive(CEngineInternalPrimitiveBase& primitive);
    std::map<unsigned long, unsigned long>* Destination;   // +0x04
    C2DBoxF Area;                                    // +0x08
    EEnginePrimitiveStatIndex StatIndex;             // +0x18
    unsigned long StatFlags;                         // +0x1C
};

CEngineScenePrimitiveStatsEnumerator::CEngineScenePrimitiveStatsEnumerator(std::map<unsigned long, unsigned long>& destination,
    const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long statFlags)
    : Destination(&destination),
      Area(area),
      StatIndex(statIndex),
      StatFlags(statFlags)
{
}