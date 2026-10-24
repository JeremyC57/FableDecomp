#include <cstdio>
// Behaviour test for 0x00809D60: STLport remove_copy_if over CTargetPoints with CFilter_Bind2<IsInFOV,
// IsStealableStockItem>: drop points in view whose thing is shop stock flagged stealable.
enum ETCInterfaceType { TC_STOCK_ITEM = 0x54, TC_OTHER = 0x10 };
struct CTCBase { };
struct CTCShop { };
template <class K, class V> struct pair { K first; V second; };
struct CTCMap {
    typedef pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start; value_type* _M_finish; value_type* _M_end_of_storage;
    value_type* LowerBound(const ETCInterfaceType* key) const
    {
        value_type* p = _M_start;
        while (p != _M_finish && p->first < *key) ++p;
        return p;
    }
};
struct CThing {
    unsigned long UsingComponents[9];
    CTCMap Components;
    bool HasTC(ETCInterfaceType type) const { return (UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    CTCBase* GetTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        CTCMap::value_type* it = Components.LowerBound(&key);
        CTCMap::value_type* end = Components._M_finish;
        return (it == end || type < it->first ? end : it)->second;
    }
};
struct CTCStockItem : CTCBase {
    bool ForSale; bool Stealable; CTCShop* shop;
    CTCShop* GetShopTC() const { return shop; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CTargetPoint { CIntelligentPointer<CThing> PThing; long Index; C3DVector TargetingPosition; long FrameRejected; };
struct CThingFilter_IsInFOV {
    float MinX;
    bool operator()(const CTargetPoint& tp) const { return tp.TargetingPosition.X >= MinX; }
};
struct CTargetPointFilter_IsStealableStockItem {
    bool operator()(const CTargetPoint& tp) const
    {
        const CThing* thing = tp.PThing.GetPItem();
        if (!thing->HasTC(TC_STOCK_ITEM)) return false;
        const CTCStockItem* item = (const CTCStockItem*)thing->GetTC(TC_STOCK_ITEM);
        return item->GetShopTC() != 0 && item->Stealable;
    }
};
template <class TF1, class TF2> struct CFilter_Bind2 {
    TF1* F1; TF2* F2;
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t); }
};
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) { *__result = *__first; ++__result; }
    return __result;
}
static void MakeThing(CThing& t, CTCMap::value_type* entries, int n, CTCStockItem* stock)
{
    for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.Components._M_start = entries; t.Components._M_finish = entries + n; t.Components._M_end_of_storage = entries + n;
    for (int i = 0; i < n; ++i) {
        ETCInterfaceType type = entries[i].first;
        t.UsingComponents[type >> 5] |= 1UL << (type & 31);
    }
    (void)stock;
}
int main()
{
    CTCShop shop;
    CTCStockItem stealable; stealable.ForSale = true; stealable.Stealable = true; stealable.shop = &shop;
    CTCStockItem locked; locked.ForSale = true; locked.Stealable = false; locked.shop = &shop;
    CTCStockItem noshop; noshop.ForSale = true; noshop.Stealable = true; noshop.shop = 0;
    CTCBase other;
    CTCMap::value_type e0[2] = { { TC_OTHER, &other }, { TC_STOCK_ITEM, &stealable } };
    CTCMap::value_type e1[1] = { { TC_STOCK_ITEM, &locked } };
    CTCMap::value_type e2[1] = { { TC_STOCK_ITEM, &noshop } };
    CTCMap::value_type e3[1] = { { TC_OTHER, &other } };
    CThing t[4];
    MakeThing(t[0], e0, 2, 0); MakeThing(t[1], e1, 1, 0); MakeThing(t[2], e2, 1, 0); MakeThing(t[3], e3, 1, 0);
    CTargetPoint v[6];
    CThing* who[6] = { &t[0], &t[0], &t[1], &t[2], &t[3], &t[0] };
    float xs[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f };     // v[5] out of view: kept
    for (int i = 0; i < 6; ++i) { v[i].PThing.SetPItem(who[i]); v[i].Index = i; v[i].FrameRejected = 20 + i; v[i].TargetingPosition.X = xs[i]; }
    CThingFilter_IsInFOV fov; fov.MinX = 0.0f;
    CTargetPointFilter_IsStealableStockItem stock;
    CFilter_Bind2<CThingFilter_IsInFOV, CTargetPointFilter_IsStealableStockItem> pred; pred.F1 = &fov; pred.F2 = &stock;
    CTargetPoint* e = remove_copy_if(v + 1, v + 6, v + 0, pred);   // v[0] and v[1] are removable
    if (e != v + 4) { std::printf("00809d60_TEST FAIL\n"); return 1; }
    if (v[0].Index != 2 || v[0].PThing.GetPItem() != &t[1] || v[1].Index != 3 || v[2].Index != 4 || v[2].FrameRejected != 24
        || v[3].Index != 5 || v[3].TargetingPosition.X != -1.0f || v[3].PThing.GetPItem() != &t[0]) {
        std::printf("00809d60_TEST FAIL\n"); return 1;
    }
    std::printf("00809d60_TEST PASS\n");
    return 0;
}