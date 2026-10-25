// CTCShop::AddShopper(CThingCreatureBase* shopper) -- add the creature to the shop's shopper list unless it is already there
// Leaf: ?AddShopper@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074AC80, 96 bytes, __fastcall `ret 4`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90 / ~CBaseIntelligentPointer
//   @ 0x00A01C10 (the CIntelligentPointer<CThingCreatureBase> temporary, vtable 0x0123E7D8 whose RTTI is
//   `.?AV?$CIntelligentPointer@VCThingCreatureBase@@@@`), STLport __find(first, last, val, random_access_iterator_tag) @
//   0x0074CEE0 (__fastcall, out of line), vector::push_back @ 0x0074DA40.
// Label: the manifest's "CTCShop_AddShopper" is right: Ego_r.pdb `?AddShopper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z`.
// Layout: Shoppers (vector<CIntelligentPointer<CThingCreatureBase> >) at +0x1C in retail (begin +0x1C, end +0x20; PDB
//   +0x18 with Dinkumware's 16-byte vector whose first pointer sits at +0x1C); PShopKeeper +0x10 as the landed
//   CCreatureActionBuyThingFromShop 0x00849380 reads it.
// Shape: `CIntelligentPointer<CThingCreatureBase> p(shopper); it = find(begin, end, p); if (it == Shoppers.end())
//   Shoppers.push_back(p);` -- the find result must be NAMED: with `find(...) == end()` VC7.1 evaluates the right-hand
//   end() first and compares against the pre-call value, retail re-reads +0x20 after the call. STLport's find passes an
//   empty random_access_iterator_tag temporary (user empty ctor, else a zero byte store) homed in the dead `shopper` slot.
class CBaseObject;
class CThingCreatureBase;

class CBaseIntelligentPointer {                 // 8 bytes: vptr + the pointed-to item
public:
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class T>
RandomAccessIter __fastcall __find(RandomAccessIter first, RandomAccessIter last, const T& val,
                                   const random_access_iterator_tag&);                     // 0x0074CEE0

template <class InputIter, class T>
inline InputIter find(InputIter first, InputIter last, const T& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}

template <class T> class vector {
public:
    T* _M_start;                                 // +0x00
    T* _M_finish;                                // +0x04
    T* _M_end_of_storage;                        // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& x);                  // 0x0074DA40
};
}

class CThing;
class CTCShop {
public:
    virtual void v00();
    CThing*         ParentThing;                 // +0x04
    bool            ToBeRemoved;                 // +0x08
    bool            IsInGlobalUpdateTCs;         // +0x09
    long            StockState;                  // +0x0C
    CBaseIntelligentPointer PShopKeeper;         // +0x10
    unsigned char   _pad18[0x1c - 0x18];
    std::vector<CIntelligentPointer<CThingCreatureBase> > Shoppers;   // +0x1C
    void AddShopper(CThingCreatureBase* shopper);
};

void CTCShop::AddShopper(CThingCreatureBase* shopper)
{
    CIntelligentPointer<CThingCreatureBase> pShopper(shopper);
    CIntelligentPointer<CThingCreatureBase>* it = std::find(Shoppers.begin(), Shoppers.end(), pShopper);
    if (it == Shoppers.end())
        Shoppers.push_back(pShopper);
}