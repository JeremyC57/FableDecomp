// CTCShop::GetFreeSaleItemPlaces(bool includeOccupied) const -- rebuild the shop's goods places from the parent thing and return the ones that are free (no other thing within 0.2 of the place), or all of them
// Leaf: GetFreeSaleItemPlaces
// Retail: Fable.exe (TLC, VC7.1) @ 0x00774B40, 274 bytes, `ret 8` (hidden result pointer + the bool).
// Callees: vector<GoodsPlace>::erase(first, last) @ 0x00777A90 (manifest "Vector_Erase_CKeyCamera"), CTCShop::InitGoodsPlaces(
//   CThing*) @ 0x007748E0 (manifest "CTCGraphicAppearance_InitGoodsPlaces"), CTCBase::GetWorld() const @ 0x00686A80,
//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>, CThing> @ 0x00777000 (landed
//   with the HasTC/InMap label), vector<GoodsPlace>::push_back @ 0x00778210, vector<GoodsPlace> copy ctor @ 0x00777430
//   (manifest "Vector_Construct_CKeyCamera"), free @ 0x00BFEA14. 0.2f inline (0x3E4CCCCD).
// Layout: Ego_r.pdb CTCShop GoodsPlaces (vector<CTCShop::GoodsPlace>, 36-byte places, position first) +0x2C -> retail +0x28;
//   CWorld's thing search tools at +0x20.
// Shape: indexed loop over the re-read size (unsigned `jb`, the index and its byte offset in ebx/ebp); the two predicates are
//   argument temporaries of the Bind2 filter (two pointers), the found-thing out word a named local; the result vector is a
//   local returned through the copy constructor (no NRVO) and freed. Same-TU stand-ins under auto_inline(off) for the vector
//   copy constructor (0x00777430), push_back (0x00778210, the landed body) and _M_insert_overflow (0x00777FE0, simplified:
//   reads its value, writes only the vector) let VC7.1 see that the result vector's address never escapes, so its storage
//   pointer is loaded once after the loop and kept in ebp across the return copy for the free, as retail's LTCG did.
class CThing;
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
class C3DVector { public: float X, Y, Z; };
struct GoodsPlace {                                 // CTCShop::GoodsPlace, 36 bytes
    C3DVector     Position;                         // +0x00
    unsigned char _pad0c[0x24 - 0x0c];
};
struct CGoodsPlaceVector {                          // STLport vector<CTCShop::GoodsPlace>
    GoodsPlace* _M_start;
    GoodsPlace* _M_finish;
    GoodsPlace* _M_end_of_storage;
    CGoodsPlaceVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CGoodsPlaceVector(const CGoodsPlaceVector& other);       // 0x00777430
    ~CGoodsPlaceVector() { if (_M_start) free(_M_start); }
    GoodsPlace* begin() const { return _M_start; }
    GoodsPlace* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    GoodsPlace& operator[](unsigned int i) { return *(_M_start + i); }
    GoodsPlace* erase(GoodsPlace* first, GoodsPlace* last);  // 0x00777A90
    void push_back(const GoodsPlace& place);        // 0x00778210
    void _M_insert_overflow(GoodsPlace* position, const GoodsPlace& x, unsigned long fillLength, bool atEnd);   // 0x00777FE0
};
GoodsPlace* AllocateGoodsPlaceCopy(const GoodsPlace* first, const GoodsPlace* last);   // stand-in callee of the stand-in below
#pragma auto_inline(off)
// Same-TU stand-in for the vector copy constructor 0x00777430 (allocate + uninitialized_copy): it only reads `other`, so
// VC7.1 knows the result vector's storage pointer survives the call, as retail's LTCG did.
CGoodsPlaceVector::CGoodsPlaceVector(const CGoodsPlaceVector& other)
{
    _M_start = AllocateGoodsPlaceCopy(other._M_start, other._M_finish);
    _M_finish = _M_start + (other._M_finish - other._M_start);
    _M_end_of_storage = _M_finish;
}
GoodsPlace* AllocateGoodsPlaces(unsigned int count);     // stand-in callee
void CGoodsPlaceVector::_M_insert_overflow(GoodsPlace* position, const GoodsPlace& x, unsigned long fillLength, bool atEnd)
{
    unsigned int count = (_M_finish - _M_start) + fillLength;
    GoodsPlace* start = AllocateGoodsPlaces(count);
    if (atEnd && position == _M_finish)
        start[count - 1] = x;
    _M_start = start;
    _M_finish = start + count;
    _M_end_of_storage = _M_finish;
}
void CGoodsPlaceVector::push_back(const GoodsPlace& place)
{
    if (_M_finish != _M_end_of_storage) {
        *_M_finish = place;
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, place, 1, true);
}
#pragma auto_inline(on)
class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) {}
};
class CThingFilter_In2DRange { public:
    C3DVector Position;
    float     Range;
    CThingFilter_In2DRange(const C3DVector& position, float range) : Position(position), Range(range) {}
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* PA;
    const B* PB;
    CFilter_Bind2(const A& a, const B& b) : PA(&a), PB(&b) {}
};
class CThingSearchTools { public:
    bool GetPFirstThing(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>& filter, CThing** out);   // 0x00777000
};
class CWorld { public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;           // +0x20
    CThingSearchTools* GetThingSearchTools() const { return PThingSearchTools; }
};
class CTCShop { public:
    virtual void v00();
    CThing*           ParentThing;                  // +0x04 (CTCBase)
    unsigned char     _pad08[0x28 - 0x08];
    CGoodsPlaceVector GoodsPlaces;                  // +0x28
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    void InitGoodsPlaces(CThing* thing);            // 0x007748E0
    CGoodsPlaceVector GetFreeSaleItemPlaces(bool includeOccupied);
};

CGoodsPlaceVector CTCShop::GetFreeSaleItemPlaces(bool includeOccupied)
{
    GoodsPlaces.erase(GoodsPlaces.begin(), GoodsPlaces.end());
    InitGoodsPlaces(ParentThing);
    CGoodsPlaceVector result;
    for (unsigned int i = 0; i < GoodsPlaces.size(); ++i) {
        if (!includeOccupied) {
            CThing* found;
            if (GetWorld()->GetThingSearchTools()->GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>(
                    CThingFilter_IsNot(ParentThing), CThingFilter_In2DRange(GoodsPlaces[i].Position, 0.2f)), &found))
                continue;
        }
        result.push_back(GoodsPlaces[i]);
    }
    return result;
}