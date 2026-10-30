// CAIStateGroup_WorkInWarehouse::MoveCrates() -- find the warehouse crate stack ("Crate<n>") whose route end is nearer the stack than the creature, then either fetch goods from the stack (when the warehouse wants crates moved in) or place the carried crate and add it to the stack
// Grade: DIFFER(1575v1541) nd=202 (was 1612v1541 nd=328) -- the place-goods path as the else arm of the moving-in test with one shared 'return false' (frame 0x48 now matches: the two goods vectors and filter temporaries share slots); residue: this in ebx v ebp (+ retail's [esp+0x14] spill), list node kept in a frame slot, 0xA6 bit word reuse
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline void* operator new(unsigned int, void* where) { return where; }
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float LengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CDefBase {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
};
struct CCrateDef : CDefBase {
    unsigned char _pad08[0x28 - 0x08];
    char CrateIndex;                            // +0x28
};
struct CDefClassBase {
    bool GetSubDef(CCrateDef** out);            // 0x0065F7B4
};
struct CCharStringData {
    const char* Text;                           // +0x00
    long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* PData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator+=(char c);                         // 0x0099F180
};
__forceinline bool operator==(CCharStringData* a, const CCharString& b)
{
    if (a == b.PData)
        return true;
    if (a == 0 || b.PData == 0 || a->Length != b.PData->Length)
        return false;
    return Compare(a->Text, b.PData->Text) == 0;
}
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsActive();                    // slot 3 (+0x0c)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDefClass;                   // +0x70
    CCharStringData* PScriptName;               // +0x74
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    __forceinline bool HasInterface(long interfaceType) const
    {
        const bool& found = HasTC(interfaceType);
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
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        const bool& found = HasTC(interfaceType);
        if (found) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<int V> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
typedef CIntelligentPointer<0x1238c6c> CThingPointer;           // vtable 0x01238C6C
typedef CIntelligentPointer<0x123e7e0> CCreaturePointer;        // vtable 0x0123E7E0
typedef CIntelligentPointer<0x128b0b0> CWarehousePointer;       // vtable 0x0128B0B0
struct CThingListNode {
    CThingListNode* _M_next;
    CThingListNode* _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};
struct SGoodsType;
extern const SGoodsType g_goodsCrate;           // 0x013BB71C
extern const SGoodsType g_goodsCrateFull;       // 0x013BB720
extern const SGoodsType g_goodsBarrel;          // 0x013BB724
extern const SGoodsType g_goodsBarrelFull;      // 0x013BB728
struct __true_type { __true_type() { } };
struct CGoodsTypeVector {
    const SGoodsType** _M_start;
    const SGoodsType** _M_finish;
    const SGoodsType** _M_end_of_storage;
    CGoodsTypeVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CGoodsTypeVector(const CGoodsTypeVector& other);    // 0x00916CE0
    ~CGoodsTypeVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(const SGoodsType** pos, const SGoodsType* const& x, const __true_type&, unsigned int fillLen, bool atEnd);   // 0x00916D50
    void push_back(const SGoodsType* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) const SGoodsType*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CAIFilter {
    virtual bool operator()(CThing* thing) const;
};
struct CAIFilter_ShouldStillBeWorkingInWarehouse : CAIFilter {   // vtable 0x01292A8C
    CThing* PStack;                             // +0x04
    CWarehousePointer PWarehouse;               // +0x08
    CCreaturePointer PCreature;                 // +0x10
    CAIFilter_ShouldStillBeWorkingInWarehouse(CWarehousePointer warehouse, CCreaturePointer creature, CThing* stack);   // 0x00944470
};
struct CTCWarehouseWorker {                     // interface 0x23
};
CThing* __fastcall GetWorkBuilding(CTCWarehouseWorker* worker);   // 0x007EE540
struct CTCRoute {                               // interface 0xA6
    C3DVector* GetRouteDestination(C3DVector* out);   // 0x008004D0
};
struct CTCCrateStack {                          // interface 0xB5
    void AddCrate(CThing* crate);               // 0x007B66C0
};
struct CTCCarrying {                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
struct CThingAICreature : CThing {
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
extern const float g_routeRatio;                // 0x0125A690 (1.2f)
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
};
struct CWarehouseThing {
    unsigned char _pad[0x10];
    CBaseIntelligentPointer PCrateStack;        // +0x10
    bool MovingIn;                              // +0x18
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CThingPointer thing, float radius, CGoodsTypeVector types, const CAIFilter& filter, CThing* stack);   // 0x00915890
    bool PlaceGoods(CThingPointer thing, float radius, CGoodsTypeVector types, const CAIFilter& filter, CThing* stack);   // 0x00915B60
};
struct CAIStateGroup_WorkInWarehouse : CAIStateGroup_MoveGoodsBase {
    unsigned char _pad14[0x1c - 0x14];
    CBaseIntelligentPointer PWarehouse;         // +0x1C
    bool MoveCrates();
};
struct CDefPointerCrate {
    CCrateDef* P;
    CDefPointerCrate() : P(0) { }
    ~CDefPointerCrate() { if (--P->RefCount == 0) P->DeleteThis(); }
};

bool CAIStateGroup_WorkInWarehouse::MoveCrates()
{
    if (!PWarehouse.GetPItem() || !PWarehouse.GetPItem()->IsActive())
        return false;
    CTCWarehouseWorker* worker;
    static_cast<const CThing*>(GetCreature())->GetInterfaceC(&worker, 0x23);
    CThing* building = GetWorkBuilding(worker);
    CThing* best = 0;
    CDefPointerCrate def;
    CWarehouseThing* warehouse = (CWarehouseThing*)PWarehouse.GetPItem();
    CDefClassBase* defClass = warehouse->PCrateStack.GetPItem()->PDefClass;
    defClass->GetSubDef(&def.P);
    char index = def.P->CrateIndex + '0';
    CCharString name("Crate");
    name += index;
    CThingListNode* head = *(CThingListNode**)((char*)building + 0x70);
    for (CThingListNode* node = head->_M_next; node != head; node = node->_M_next) {
        CThing* thing = node->_M_data.GetPItem();
        if (!thing || !thing->HasInterface(0xa6))
            continue;
        if (!(thing->PScriptName == name))
            continue;
        CTCRoute* route = 0;
        thing->GetInterface(&route, 0xa6);
        C3DVector a;
        float creatureLengthSquared = (*route->GetRouteDestination(&a) - GetCreature()->GetPosition()).LengthSquared();
        float creatureDistance = (float)sqrt(creatureLengthSquared);
        C3DVector b;
        float stackLengthSquared = (*route->GetRouteDestination(&b) - thing->GetPosition()).LengthSquared();
        float stackDistance = (float)sqrt(stackLengthSquared);
        if (stackDistance < creatureDistance * g_routeRatio) {
            best = thing;
            break;
        }
    }
    CThing* stackThing = ((CWarehouseThing*)PWarehouse.GetPItem())->PCrateStack.GetPItem();
    CTCCrateStack* stack = 0;
    stackThing->GetInterface(&stack, 0xb5);
    if (((CWarehouseThing*)PWarehouse.GetPItem())->MovingIn) {
        CGoodsTypeVector types;
        types.push_back(&g_goodsCrate);
        types.push_back(&g_goodsBarrel);
        if (FetchGoods(CThingPointer(stackThing), 4.0f, types,
                CAIFilter_ShouldStillBeWorkingInWarehouse(CWarehousePointer(PWarehouse.GetPItem()), CCreaturePointer(GetCreature()), building), best))
            return true;
    } else {
        CTCCarrying* carrying;
        static_cast<const CThing*>(GetCreature())->GetInterfaceC(&carrying, 0x46);
        CThingPointer carried(carrying->GetThingInPrimarySlot());
        CGoodsTypeVector types;
        types.push_back(&g_goodsCrateFull);
        types.push_back(&g_goodsBarrelFull);
        if (PlaceGoods(CThingPointer(stackThing), 4.0f, types,
                CAIFilter_ShouldStillBeWorkingInWarehouse(CWarehousePointer(PWarehouse.GetPItem()), CCreaturePointer(GetCreature()), 0), best)) {
            if (stack)
                stack->AddCrate(carried.GetPItem());
            return true;
        }
    }
    return false;
}

