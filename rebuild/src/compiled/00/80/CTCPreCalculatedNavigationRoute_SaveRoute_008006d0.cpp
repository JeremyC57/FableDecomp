// CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context) -- when the target thing exists and is alive, recompute the owner->target route and save its step count, then each step's local position and nav layer; otherwise save a step count of 0
// Retail: Fable.exe @ 0x008006D0, 480 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, CalculateRouteFromTo 0x00800540,
//   CPersistContext::Transfer<long> 0x00410BE0 / <C2DVector> 0x00800B90 / <unsigned char> 0x00405A60, CTCBase::GetWorldMap
//   0x00686D20, CWorldMap::ConvertWorldMapCoordsToLocalCoords 0x004FC330, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, CCharString::ToConstChar 0x0099E4C0, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout: Ego_r.pdb CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo +0x0c, Route +0x20 (12-byte STLport vector of
//   9-byte pair<C2DVector, unsigned char> steps -- retail divides by 9); CThing region index short +0x8e, Dead bit +0x91.
#include <stdlib.h>
class C2DVector { public: float X; float Y; C2DVector(float x, float y) : X(x), Y(y) {} };
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* m_data;
    ~CCharString();                                 // 0x0099EAE0
    const char* ToConstChar() const;                // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall operator+(const char* a, const CCharString& b);       // 0x0099F690
class CThing { public:
    unsigned char  _pad00[0x8e];
    short          RegionIndex;                     // +0x8e
    unsigned char  _pad90;
    unsigned char  Dead : 1;                        // +0x91 bit 0
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CBaseIntelligentPointer {
    unsigned char _pad[0xc];
    CThing* GetPItem() const;                       // 0x00A01B50
};
class CWorldMap { public:
    C2DVector ConvertWorldMapCoordsToLocalCoords(long region, const C2DVector& pos);   // 0x004FC330
};
class CPersistContext { public:
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, C2DVector& value, const C2DVector& defaultValue);                // 0x00800B90
    void Transfer(const char* name, unsigned char& value, const unsigned char& defaultValue);        // 0x00405A60
};
#pragma pack(push, 1)
struct CNavStep { C2DVector Pos; unsigned char Layer; };
#pragma pack(pop)
struct CNavStepVector {
    CNavStep* _M_start;
    CNavStep* _M_finish;
    CNavStep* _M_end_of_storage;
    CNavStepVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CNavStepVector() { if (_M_start) free(_M_start); }
    long size() const { return _M_finish - _M_start; }
};
class CTCBase { public:
    void*   vtable;
    CThing* Owner;                                  // +0x04
    unsigned char _pad08[4];
    CWorldMap* GetWorldMap();                       // 0x00686D20
};
class CTCPreCalculatedNavigationRoute : public CTCBase { public:
    CBaseIntelligentPointer PThingToCalculateRouteTo;   // +0x0c
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    CNavStepVector          Route;                      // +0x20
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CNavStepVector& route);   // 0x00800540
    void SaveRoute(CPersistContext* context);
};

template <class T>
__forceinline void SaveValue(CPersistContext* context, const char* name, T& value, const T& defaultValue)
{
    context->Transfer(name, value, defaultValue);
}

void CTCPreCalculatedNavigationRoute::SaveRoute(CPersistContext* context)
{
    if (PThingToCalculateRouteTo.GetPItem() && !PThingToCalculateRouteTo.GetPItem()->Dead)
    {
        CThing* owner = Owner;
        long region = owner->RegionIndex;
        CNavStepVector route;
        CalculateRouteFromTo(owner->GetPosition(), PThingToCalculateRouteTo.GetPItem()->GetPosition(), route);
        long steps = route.size();
        context->Transfer("NumberOfStepsOnRoute", steps, 0);
        long i = 0;
        if (i < steps)
        {
        C2DVector defaultPos(0.0f, 0.0f);
        unsigned char defaultLayer = 0;
        do
        {
            C2DVector pos = GetWorldMap()->ConvertWorldMapCoordsToLocalCoords(region, route._M_start[i].Pos);
            SaveValue(context, ("NavPosition" + GFIntToCharString(i)).ToConstChar(), pos, defaultPos);
            unsigned char layer = route._M_start[i].Layer;
            SaveValue(context, ("NavLayer" + GFIntToCharString(i)).ToConstChar(), layer, defaultLayer);
        } while (++i < steps);
        }
    }
    else
    {
        long steps = 0;
        context->Transfer("NumberOfStepsOnRoute", steps, 0);
    }
}