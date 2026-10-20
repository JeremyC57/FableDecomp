// CTCDPhysicalObstruction::UpdateAdditionalImpulse() -- collect the creatures in the obstruction line's search area that are not on the ignore list, within the line's radius and affected by obstructions, and push each one off the smeared-out line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD580, 194 bytes (plain `ret`, 0x44-byte frame).
// Callees: GetLineThingSearchArea(const C3DVector&, const C3DVector&, float) @ 0x008F58C0 (`__fastcall`, C2DBoxI by hidden pointer),
// CTCBase::GetThingSearchTools() @ 0x00686D80, CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,
// vector<CThingCreatureBase*>, CFilter_Bind3<CThingFilter_IsNotList, CThingFilter_IsWithinArea, CThingFilter_IsAffectedByObstructions>>
// @ 0x007FDB10 (manifest label carries another instantiation), std::for_each<CThingCreatureBase**, CKeepOutOffSmearedOutLine>
// @ 0x007FDA80 (`__fastcall`, the functor returned by hidden pointer; manifest "_For_each<..., CApplyAdditionalImpulseIf...>" is
// a propagated name), free @ 0x00BFEA14 (the vector storage).
// Layout: Ego_r.pdb CTCDPhysicalObstruction: StartPos +0x0c, EndPos +0x18, Radius +0x28, ThingsToIgnoreObstruction +0x2c; the
// filter types and sizes are Ego_r.pdb (CThingFilter_IsNotList {PNotList, PNotList2}, CThingFilter_IsWithinArea {StartPos*,
// EndPos*, Radius, IncludeThingRad}, the one-byte anonymous-namespace CThingFilter_IsAffectedByObstructions, CFilter_Bind3 =
// three filter pointers; CKeepOutOffSmearedOutLine {StartPos*, EndPos*, Radius}).
// Shape: the creature vector and the area filter are named locals built before the area call; the list filter, the
// obstruction filter and the binder are temporaries of the search call (their frame words are reused by the for_each functor
// and its returned copy); the functor has the implicit copy (integer moves); the vector storage is freed with the shared zero
// register. Retail keeps the vector's first pointer in esi across for_each, which needs VC7.1 to know that neither the search
// nor for_each lets the vector's address escape: both instantiations live in this TU (0x007FDB10 / 0x007FDA80) and were known
// to the original compile. A for_each body and a STAND-IN search body (it only touches the vector inline; not the real
// 0x007FDB10 code) are compiled above under `auto_inline(off)` for that knowledge -- review at landing.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI() { }
};
C2DBoxI __fastcall GetLineThingSearchArea(const C3DVector& start, const C3DVector& end, float radius);   // 0x008F58C0
struct CThing;
struct CThingCreatureBase;
struct CIntelligentPointer { void* vptr; CThing* item; };
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
struct CThingFilter_IsNotList {
    const CRetailVector<const CThing*>*        PNotList;
    const CRetailVector<CIntelligentPointer>*  PNotList2;
    CThingFilter_IsNotList(const CRetailVector<CIntelligentPointer>& list) : PNotList(0), PNotList2(&list) { }
};
struct CThingFilter_IsWithinArea {
    const C3DVector* StartPos;
    const C3DVector* EndPos;
    float            Radius;
    bool             IncludeThingRad;
    CThingFilter_IsWithinArea(const C3DVector& start, const C3DVector& end, float radius, bool includeThingRad)
        : StartPos(&start), EndPos(&end), Radius(radius), IncludeThingRad(includeThingRad) { }
};
struct CThingFilter_IsAffectedByObstructions {          // anonymous-namespace filter in the original
    unsigned char _empty;
    CThingFilter_IsAffectedByObstructions() : _empty(0) { }
};
template <class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* f1;
    const F2* f2;
    const F3* f3;
    CFilter_Bind3(const F1& a, const F2& b, const F3& c) : f1(&a), f2(&b), f3(&c) { }
};
typedef CFilter_Bind3<CThingFilter_IsNotList, CThingFilter_IsWithinArea, CThingFilter_IsAffectedByObstructions> CObstructionFilter;
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
struct CThingSearchTools {
    CThing* FirstThingInArea(const C2DBoxI& area);      // stand-in callees of the stand-in body below
    CThing* NextThingInArea(const C2DBoxI& area, CThing* previous);
    template <class TFilter> bool Accepts(const TFilter& filter, CThing* thing);
    template <class T, class TVector, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& things);   // 0x007FDB10
};
// Stand-in body (the real one is the 0x007FDB10 instantiation of this TU): it touches `things` only inline, so VC7.1 sees that
// the vector's address does not escape the call.
#pragma auto_inline(off)
template <class T, class TVector, class TFilter>
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& things)
{
    for (CThing* thing = FirstThingInArea(area); thing; thing = NextThingInArea(area, thing)) {
        if (!Accepts(filter, thing))
            continue;
        if (things._M_finish == things._M_end_of_storage) {
            unsigned long size = things._M_finish - things._M_start;
            unsigned long len = size ? 2 * size : 8;
            T** start = (T**)malloc(len * sizeof(T*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = things._M_start[i];
            if (things._M_start)
                free(things._M_start);
            things._M_start = start;
            things._M_finish = start + size;
            things._M_end_of_storage = start + len;
        }
        *things._M_finish++ = (T*)thing;
    }
}
#pragma auto_inline(on)
struct CKeepOutOffSmearedOutLine {
    const C3DVector* StartPos;
    const C3DVector* EndPos;
    float            Radius;
    CKeepOutOffSmearedOutLine(const C3DVector& start, const C3DVector& end, float radius) : StartPos(&start), EndPos(&end), Radius(radius) { }
    void operator()(CThingCreatureBase* creature);      // 0x007FD8D0
};
#pragma auto_inline(off)
CKeepOutOffSmearedOutLine __fastcall ForEach(CThingCreatureBase** first, CThingCreatureBase** last, CKeepOutOffSmearedOutLine f)   // 0x007FDA80
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
#pragma auto_inline(on)
struct CTCDPhysicalObstruction {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    C3DVector StartPos;                                 // +0x0c
    C3DVector EndPos;                                   // +0x18
    long  LifeTimer;                                    // +0x24
    float Radius;                                       // +0x28
    CRetailVector<CIntelligentPointer> ThingsToIgnoreObstruction;   // +0x2c
    CThingSearchTools* GetThingSearchTools();           // 0x00686D80 (CTCBase)
    void UpdateAdditionalImpulse();
};

void CTCDPhysicalObstruction::UpdateAdditionalImpulse()
{
    CRetailVector<CThingCreatureBase*> creatures;
    CThingFilter_IsWithinArea withinArea(StartPos, EndPos, Radius, false);
    C2DBoxI area = GetLineThingSearchArea(StartPos, EndPos, Radius);
    GetThingSearchTools()->GetAllThingsInRoughArea<CThingCreatureBase>(area,
        CObstructionFilter(CThingFilter_IsNotList(ThingsToIgnoreObstruction), withinArea, CThingFilter_IsAffectedByObstructions()), creatures);
    ForEach(creatures.begin(), creatures.end(), CKeepOutOffSmearedOutLine(StartPos, EndPos, Radius));
}
