// Grade: DIFFER(ours v retail) -- 219v218: only the box arithmetic and its stores are scheduled differently (retail `add esi,eax` / `add edi,eax` in place with the MinY store after the first push; ours `lea edx,[eax+esi]` and stores before it); the query, filter slot, loop, smash call and free match. Tried: ctor-built box, body-assigned box, TU-local template bodies for the query (needed for the cached vector pointers), one-ternary find.
// CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint(const C3DVector& point, float radius) -- collect every thing with a smashable-window component (kind 0x4e) in the integer box of half-size `radius` around the point's X/Y, and smash each one (component slot 28); the result vector is freed
// Leaf: ?SmashAllWindowsWithinRadiusOfPoint@CGameScriptInterface@@UAEXABVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8010, 218 bytes, `ret 8`.
// Callees: __ftol2 @ 0x00BFEA70 (x, y, radius truncated), the world's thing-search tools (+0x20) area query @ 0x008ADCC0
//   (a CThingSearchTools template member: box, filter and output vector by reference; its label
//   "GetPBestThingInRoughArea<CThingFilter_HasTrackName,...>" belongs to another instantiation), the component-map
//   LowerBound @ 0x0040F020, the component's virtual slot 28 (+0x70), free @ 0x00BFEA14 (the vector storage).
// Layout: CGameScriptInterface World +4 (engine/CGameScriptInterface.h; local polymorphic model because the header
//   flattens the vptr), CWorld +0x20 the thing-search tools; CThing component bitset +0x20 (kind 0x4e = word +0x28
//   bit 14), component map +0x44.
// Shape: the box corners are computed from three truncated longs (x - r, x + r, y - r, y + r) into a C2DBoxI local; the
//   filter is a CThingFilter_HasTC temporary homed in the dead `point` slot (as is the lookup key later); the output is a
//   12-byte STLport vector zeroed by one register; the loop caches begin/end (the body makes a virtual call); the
//   vector's inline destructor frees the cached first pointer (only once the query's template body is in the TU, below);
//   the component is the one-ternary map find so it loads straight into ecx for the virtual call.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI() { }
};
struct CTCBase_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void Smash();                          // slot 28 (+0x70)
};
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return (it == m_pLast || kind < it->m_kind ? m_pLast : it)->m_pTC;
    }
};
struct CThing_Retail {
    long           m_cellX;                        // (simplified position words for the TU-local query body)
    long           m_cellY;
    unsigned char  _pad08[0x18];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }

struct CThingFilter_HasTC {
    long Type;
    CThingFilter_HasTC(long type) : Type(type) { }
    bool operator()(const CThing_Retail* thing) const { return thing->HasTC(Type) != 0; }
};
template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> struct CRetailVector : _Vector_base<T> {   // STLport vector<T>: three pointers (12 bytes in retail)
    CRetailVector() : _Vector_base<T>(allocator<T>()) { }
    T* begin() const { return this->_M_start; }
    T* end() const { return this->_M_finish; }
    void push_back(const T& value);
    void Grow(const T& value);
};
struct CThingSearchTools {
    CThing_Retail** m_pFirst;                      // (simplified candidate set for the TU-local template body)
    CThing_Retail** m_pLast;
    template <class TFilter> bool GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter,
                                                          CRetailVector<CThing_Retail*>& out);   // 0x008ADCC0
};

// The area query 0x008ADCC0 and the vector growth it uses are templates compiled in this TU as well; simplified bodies
// are given here under auto_inline(off) so VC7.1 knows the query only fills `out` (retail keeps the vector's first and
// last pointers in ebx/ebp across the loop's virtual calls and frees the cached first pointer).
#pragma auto_inline(off)
template <class T> void CRetailVector<T>::Grow(const T& value)
{
    unsigned int count = this->_M_finish - this->_M_start;
    T* block = (T*)malloc((count * 2 + 1) * sizeof(T));
    for (unsigned int i = 0; i < count; ++i)
        block[i] = this->_M_start[i];
    block[count] = value;
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = block;
    this->_M_finish = block + count + 1;
    this->_M_end_of_storage._M_data = block + count * 2 + 1;
}
template <class T> void CRetailVector<T>::push_back(const T& value)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        *this->_M_finish = value;
        ++this->_M_finish;
    } else {
        Grow(value);
    }
}
template <class TFilter> bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter,
                                                                         CRetailVector<CThing_Retail*>& out)
{
    for (CThing_Retail** it = m_pFirst; it != m_pLast; ++it) {
        long x = (*it)->m_cellX;
        long y = (*it)->m_cellY;
        if (x >= area.MinX && x <= area.MaxX && y >= area.MinY && y <= area.MaxY && filter(*it))
            out.push_back(*it);
    }
    return out.begin() != out.end();
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;          // +0x20
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                                 // +0x04
    virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector& point, float radius);
};

void CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint(const C3DVector& point, float radius)
{
    long x = (long)point.X;
    long y = (long)point.Y;
    long r = (long)radius;
    C2DBoxI area;
    area.MinX = x - r;
    area.MaxX = x + r;
    area.MinY = y - r;
    area.MaxY = y + r;
    CRetailVector<CThing_Retail*> things;
    World->PThingSearchTools->GetAllThingsInRoughArea(area, CThingFilter_HasTC(0x4e), things);
    CThing_Retail** end = things.end();
    for (CThing_Retail** it = things.begin(); it != end; ++it) {
        CThing_Retail* thing = *it;
        if (thing->HasTC(0x4e))
            GetTC(thing, 0x4e)->Smash();
    }
}
