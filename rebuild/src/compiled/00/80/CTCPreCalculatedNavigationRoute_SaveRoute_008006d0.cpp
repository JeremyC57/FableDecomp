// CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context) -- when the target thing exists and is alive, recompute the owner->target route and save its step count, then each step's local position and nav layer; otherwise save a step count of 0
// Leaf: ?SaveRoute@CTCPreCalculatedNavigationRoute@@QAEXPAVCPersistContext@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x008006D0, 480 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, CalculateRouteFromTo 0x00800540,
//   CPersistContext::Transfer<long> 0x00410BE0 / <C2DVector> 0x00800B90 / <unsigned char> 0x00405A60, CTCBase::PeekWorldMap
//   0x00686D20, CWorldMap::ConvertWorldMapCoordsToLocalCoords 0x004FC330, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, CCharString::ToConstChar 0x0099E4C0, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// NF179 (PRIORITY q11932): the route is now the real STLport std::vector<std::pair<C2DVector, unsigned char> > (stlport/fable_containers.h)
//   over the shared byte-packed C2DVector (engine/C2DVector.h FABLE_RETAIL_C2DVECTOR: 9-byte pairs, retail divides by 9), passed to
//   CalculateRouteFromTo 0x00800540 as the PDB vector& with no cast; the local CNavStepVector / CNavStep stand-ins, the cast and the
//   row-local C2DVector model are gone. Release: ~_Vector_base's null-tested free (retail 0x0080086E `cmp eax,edi ; je`).
// Layout: Ego_r.pdb CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo +0x0c, Route +0x20 (12-byte STLport vector of
//   9-byte pair<C2DVector, unsigned char> steps -- retail divides by 9); CThing region index short +0x8e, Dead bit +0x91.
// Callee respelled (NF154, PRIORITY q4533 family): CalculateRouteFromTo 0x00800540 is Ego_r.pdb 0x007F3F10 / FableWin.pdb 0x026C1C20
//   ?CalculateRouteFromTo@CTCPreCalculatedNavigationRoute@@ABEXABVC3DVector@@0AAV?$vector@U?$pair@VC2DVector@@E@std@@V?$allocator@U?$pair@VC2DVector@@E@std@@@2@@std@@@Z
//   (private const, STLport vector<pair<C2DVector, unsigned char> >&; was ?CalculateRouteFromTo@CTCPreCalculatedNavigationRoute@@QAEXABVC3DVector@@0AAUCNavStepVector@@@Z);
//   the self row 0x00800540 takes the same spelling.
#include "fable_compiler.h"
#include <stdlib.h>
#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall operator+(const char* a, const CCharString& b);       // 0x0099F690

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
class CWorldMap { public:
    C2DVector ConvertWorldMapCoordsToLocalCoords(long region, const C2DVector& pos) const;   // 0x004FC330
};
class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: long 0x00410BE0; C2DVector 0x00800B90; unsigned char 0x00405A60
};
class CTCBase { public:
    void*   vtable;
    CThing* Owner;                                  // +0x04
    unsigned char _pad08[4];
protected:
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};
class CTCPreCalculatedNavigationRoute : public CTCBase { public:
    CIntelligentPointer<CThing> PThingToCalculateRouteTo;   // +0x0c
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > > Route;   // +0x20
    void SaveRoute(CPersistContext* context);
private:
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > >& route) const;   // 0x00800540
};

template <class T>
FABLE_FORCEINLINE void SaveValue(CPersistContext* context, const char* name, T& value, const T& defaultValue)
{
    context->Transfer(name, value, defaultValue);
}

void CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context)
{
    if (PThingToCalculateRouteTo.GetPItem() && !PThingToCalculateRouteTo.GetPItem()->IsDead())
    {
        CThing* owner = Owner;
        long region = owner->GetCurrentMapNumber();
        std::vector<std::pair<C2DVector, unsigned char>, std::allocator<std::pair<C2DVector, unsigned char> > > route;
        CalculateRouteFromTo(owner->GetPos(), PThingToCalculateRouteTo.GetPItem()->GetPos(),
                             route);
        long steps = route.size();
        context->Transfer<long>("NumberOfStepsOnRoute", steps, 0);
        long i = 0;
        if (i < steps)
        {
        C2DVector defaultPos(0.0f, 0.0f);
        unsigned char defaultLayer = 0;
        do
        {
            C2DVector pos = PeekWorldMap().ConvertWorldMapCoordsToLocalCoords(region, route[i].first);
            SaveValue(context, (const char*)("NavPosition" + GFIntToCharString(i)), pos, defaultPos);
            unsigned char layer = route[i].second;
            SaveValue(context, (const char*)("NavLayer" + GFIntToCharString(i)), layer, defaultLayer);
        } while (++i < steps);
        }
    }
    else
    {
        long steps = 0;
        context->Transfer<long>("NumberOfStepsOnRoute", steps, 0);
    }
}