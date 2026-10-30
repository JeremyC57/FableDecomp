// CTCPreCalculatedNavigationRoute::LoadRoute(CPersistContext* context, long version) -- load the step count, empty and reserve the route, then load each step's position (converted from local to world-map coordinates from version 2 on) and nav layer and append it
// Grade: DIFFER(425v420): clear()'s random_access_iterator_tag is a named empty object (no zero byte store) and steps is uninitialised before Transfer (one zero store, the default temp); residue is the name-before-pushes order of the per-step Transfer calls (retail builds the + string before pushing &default/&pos; a named string in a {} block moves it but uses the named slot for ToConstChar) and the packed default C2DVector at [esp+37h]
// Retail: Fable.exe @ 0x008008B0, 432 bytes (`ret 8`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0, CalculateRouteFromTo 0x00800540,
//   CPersistContext::Transfer<long> 0x00410BE0 / <C2DVector> 0x00800B90 / <unsigned char> 0x00405A60, CTCBase::GetWorldMap
//   0x00686D20, CWorldMap::ConvertWorldMapCoordsToLocalCoords 0x004FC330, GFIntToCharString 0x0099F830,
//   operator+(const char*, const CCharString&) 0x0099F690, CCharString::ToConstChar 0x0099E4C0, ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Layout: Ego_r.pdb CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo +0x0c, Route +0x20 (12-byte STLport vector of
//   9-byte pair<C2DVector, unsigned char> steps -- retail divides by 9); CThing region index short +0x8e, Dead bit +0x91.
#include <stdlib.h>
#pragma pack(push, 1)
struct C2DVector { float X; float Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} };
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_data;
    ~CCharString();                                 // 0x0099EAE0
    const char* ToConstChar() const;                // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                       // 0x0099F830
CCharString __fastcall operator+(const char* a, const CCharString& b);       // 0x0099F690
struct CThing {
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
struct CWorldMap {
    C2DVector ConvertLocalCoordsToWorldMapCoords(long region, const C2DVector& pos);   // 0x004FC470
};
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, C2DVector& value, const C2DVector& defaultValue);                // 0x00800B90
    void Transfer(const char* name, unsigned char& value, const unsigned char& defaultValue);        // 0x00405A60
};
struct CNavStep { C2DVector Pos; unsigned char Layer; CNavStep(const C2DVector& pos, unsigned char layer) : Pos(pos), Layer(layer) {} };
struct random_access_iterator_tag {};
struct __true_type {};
CNavStep* __fastcall CopyNavSteps(CNavStep* first, CNavStep* last, CNavStep* result, const random_access_iterator_tag&, long*);   // 0x008001F0
struct CNavStepVector {
    CNavStep* _M_start;
    CNavStep* _M_finish;
    CNavStep* _M_end_of_storage;
    CNavStepVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CNavStepVector() { if (_M_start) free(_M_start); }
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned int n);                   // 0x00800CA0
    void _M_insert_overflow(CNavStep* pos, const CNavStep& x, const __true_type&, unsigned int n, bool atEnd);   // 0x00800DA0
    void clear() { random_access_iterator_tag tag; _M_finish = CopyNavSteps(_M_finish, _M_finish, _M_start, tag, (long*)0); }
    void push_back(const CNavStep& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CTCBase {
    void*   vtable;
    CThing* Owner;                                  // +0x04
    unsigned char _pad08[4];
    CWorldMap* GetWorldMap();                       // 0x00686D20
};
struct CTCPreCalculatedNavigationRoute : CTCBase {
    CBaseIntelligentPointer PThingToCalculateRouteTo;   // +0x0c
    unsigned __int64        ThingToCalculateRouteToUID; // +0x18
    CNavStepVector          Route;                      // +0x20
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CNavStepVector& route);   // 0x00800540
    void LoadRoute(CPersistContext* context, long version);
};

#pragma pack(pop)
void CTCPreCalculatedNavigationRoute::LoadRoute(CPersistContext* context, long version)
{
    long region = Owner->RegionIndex;
    long steps;
    context->Transfer("NumberOfStepsOnRoute", steps, 0);
    Route.clear();
    Route.reserve(steps);
    long i = 0;
    if (i < steps)
    {
    C2DVector defaultPos(0.0f, 0.0f);
    unsigned char defaultLayer = 0;
    do
    {
        C2DVector pos;
        context->Transfer(("NavPosition" + GFIntToCharString(i)).ToConstChar(), pos, defaultPos);
        if (version >= 2)
            pos = GetWorldMap()->ConvertLocalCoordsToWorldMapCoords(region, pos);
        unsigned char layer;
        context->Transfer(("NavLayer" + GFIntToCharString(i)).ToConstChar(), layer, defaultLayer);
        Route.push_back(CNavStep(pos, layer));
    } while (++i < steps);
    }
}
