// CAIStateGroup_SetupWares::Update() -- shopkeeper setup: outside opening hours walk the village's stock things to the nearest free goods place and pick them up; during opening hours walk to each free goods place and put out a sale item
// Grade: DIFFER(1331v1331) nd 120->18 -- lever: same-TU noinline CInterfaceMap::LowerBound stand-in (GOTCHAS chunk 240) closes the frame layout. Residue: reached byte at +0x1b (retail +0x13) and the per-place retry count in eax (retail ecx, 1.4f pushed after the &reached lea). Tried a GoodsPlace* place local (1256v1331, worse), reached at function top (no change). Next: the retry count as an inline helper argument
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float LengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct GoodsPlace {                                 // CTCShop::GoodsPlace, 36 bytes
    C3DVector     Position;                         // +0x00
    C3DVector     Facing;                           // +0x0c
    unsigned char _pad18[0x24 - 0x18];
};
struct CGoodsPlaceVector {                          // STLport vector<CTCShop::GoodsPlace>
    GoodsPlace* _M_start;
    GoodsPlace* _M_finish;
    GoodsPlace* _M_end_of_storage;
    ~CGoodsPlaceVector() { if (_M_start) free(_M_start); }
    GoodsPlace* begin() const { return _M_start; }
    GoodsPlace* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};
struct CLongVector {                                // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    ~CLongVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
};
template<class T> __forceinline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CDefRefCounted {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CStockItemDef : CDefRefCounted {
};
template<class T> struct CDefPointer {
    T* P;
    CDefPointer() : P(0) { }
    ~CDefPointer() { T* p = P; if (p != 0 && --p->RefCount == 0) p->DeleteThis(); }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CStockItemDef>* out);   // 0x0059350C
};
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CStockItemDef>* out)
{
    out->P = 0;
    return false;
}
#pragma auto_inline(on)
__forceinline bool GetSubDef(CDefClassBase* defClass, CDefPointer<CStockItemDef>& def) { return defClass->GetSubDef(&def); }
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDefClass;                   // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType)
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    __forceinline bool HasInterface(long interfaceType)
    {
        const unsigned long& word = m_presentBits[interfaceType >> 5];
        bool found = (word >> (interfaceType & 31)) & 1;
        return found;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& found = HasTC(interfaceType);
        if (found) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
};
struct CThingCreatureBase : CThing {
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_DEAD(const CBaseIntelligentPointer& p);   // 0x005CC1B0
struct CThingListNode {
    CThingListNode* _M_next;
    CThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};
struct CThingList {
    CThingListNode* _M_node;
    CThingListNode* begin() const { return _M_node->_M_next; }
    CThingListNode* end() const { return _M_node; }
};
struct CTCStockHolder {                         // interface 0x21
    unsigned char _pad[0x0c];
    CThingList Things;                          // +0x0c
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool IsOpeningHours();                      // 0x00773950
    void SetStockState(long state);             // 0x00773C00
    CLongVector GetSaleItemsToPlace();          // 0x00773EE0
    CGoodsPlaceVector GetFreeSaleItemPlaces(bool includeOccupied);   // 0x00774B40
};
struct CTCShopKeeper {                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
};
struct CCreatureActionBase {
    unsigned char _data[0xd4];
};
struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    CCreatureAction_AddRealObjectToInventory(CThingCreatureBase* creature, CThing* item);   // 0x00843670
    ~CCreatureAction_AddRealObjectToInventory();   // 0x00927DD0
};
struct CCreatureAction_PlaceSaleItem : CCreatureActionBase {
    CCreatureAction_PlaceSaleItem(CThingCreatureBase* creature, CTCShop* shop, const C3DVector& pos, const C3DVector& facing, long item);   // 0x00842C70
    ~CCreatureAction_PlaceSaleItem();           // 0x00693EF0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool b);  // 0x0097DB70
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f, float g);   // 0x0097E890
};
struct CAIStateGroup_SetupWares : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_SetupWares::Update()
{
    CTCShopKeeper* keeper;
    if (!GetCreature()->GetInterface(&keeper, 0x90) || !keeper->GetShopTC()) {
        Terminate();
        return;
    }
    CTCShop* shop = keeper->GetShopTC();
    shop->SetStockState(2);
    bool reached;
    if (!shop->IsOpeningHours()) {
        CGoodsPlaceVector places = shop->GetFreeSaleItemPlaces(true);
        CTCStockHolder* holder;
        if (shop->ParentThing->GetInterface(&holder, 0x21)) {
            for (CThingListNode* node = holder->Things.begin(); node != holder->Things.end(); node = node->_M_next) {
                CBaseIntelligentPointer& item = node->_M_data;
                CDefPointer<CStockItemDef> def;
                if (item.GetPItem() && !(item.GetPItem()->m_flags91 & 1) && item.GetPItem()->HasInterface(0x32)
                    && item.GetPItem()->PDefClass && GetSubDef(item.GetPItem()->PDefClass, def)) {
                    float best = 0.0f;
                    C3DVector target = item.GetPItem()->GetPosition();
                    for (GoodsPlace* it = places.begin(); it != places.end(); ++it) {
                        float lengthSquared = (it->Position - item.GetPItem()->GetPosition()).LengthSquared();
                        float dist = (float)sqrt(lengthSquared);
                        if (it == places.begin() || dist < best) {
                            best = dist;
                            target = it->Position;
                        }
                    }
                    if (AIAction_MoveToPosProximity(1, target, 1.4f, &reached, 3, true, false, true, 0, true, 0.0f) != 2 || IS_THING_DEAD(item)) {
                        Terminate();
                        return;
                    }
                    if (AIAction_LookAtPos(1, target, 0x11, 0.0027777f) != 2 || IS_THING_DEAD(item)) {
                        Terminate();
                        return;
                    }
                    AIAction_PerformActionAndWait(CCreatureAction_AddRealObjectToInventory(GetCreature(), item.GetPItem()), false);
                }
            }
        }
        shop->SetStockState(1);
    } else {
        CGoodsPlaceVector places = shop->GetFreeSaleItemPlaces(false);
        CLongVector items = shop->GetSaleItemsToPlace();
        for (unsigned int i = 0; i < Min(places.size(), items.size()); ++i) {
            long tries = 3;
            if (i == 0)
                tries = 5;
            if (AIAction_MoveToPosProximity(1, (places.begin() + i)->Position, 1.4f, &reached, tries, true, false, true, 0, true, 0.0f) != 2) {
                Terminate();
                return;
            }
            if (AIAction_LookAtPos(1, (places.begin() + i)->Position, 0x11, 0.0027777f) != 2) {
                Terminate();
                return;
            }
            AIAction_PerformActionAndWait(CCreatureAction_PlaceSaleItem(GetCreature(), shop, (places.begin() + i)->Position, (places.begin() + i)->Facing, items._M_start[i]), false);
        }
        shop->SetStockState(0);
    }
    Terminate();
}
