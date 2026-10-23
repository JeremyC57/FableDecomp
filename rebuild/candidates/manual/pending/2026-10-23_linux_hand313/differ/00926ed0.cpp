// CAIStateGroup_OpenAndCloseShop::Update() -- walk to the inside or outside face of the nearest unlocked shop door, look back at it, then lock or unlock every door and wait (up to 75 frames) until the money bags are counted or the shop is unlocked
// Grade: DIFFER(1389v1373) -- first pass: this in ebx not ebp (whole-function register roles), the first shop-dead exit moved to the end, the wait condition is tested as al not as the neg/sbb/inc logical-not value
extern "C" void __cdecl free(void*);
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;                               // +0x0C
    __forceinline C3DVector UpScaled(float f) const { return C3DVector(Up.X * f, Up.Y * f, Up.Z * f); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThingGraphic {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CRightHandedSet* GetRHSet();  // slot 72 (+0x120)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingGraphic* PGraphic;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91
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
    __forceinline unsigned long HasTCWord(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(T*, long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
    void LockAllDoors(bool lock, bool b);       // 0x0082DB00
    bool CountMoneyBags();                      // 0x0082DE20
    bool IsUnlocked();                          // 0x0082DEF0
};
struct CBaseIntelligentPointer {
    virtual void* DeletingDtor(unsigned int flags);
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01237F1C
    virtual void* DeletingDtor(unsigned int flags);
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {  // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void* DeletingDtor(unsigned int flags);
    CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingBuilding* operator->() const { return (CThingBuilding*)GetPItem(); }
};
struct CThingPtrVector {                        // std::vector<CIntelligentPointer<CThing> > (12-byte STLport)
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector()
    {
        for (CIntelligentPointerThing* p = _M_start; p != _M_finish; ++p)
            p->DeletingDtor(0);
        if (_M_start)
            free(_M_start);
    }
    void push_back(const CIntelligentPointerThing& item);   // 0x007398B0
};
bool CompareDoors(CIntelligentPointerThing a, CIntelligentPointerThing b);   // 0x00926980
void __fastcall SortDoors(CIntelligentPointerThing* first, CIntelligentPointerThing* last, bool (*compare)(CIntelligentPointerThing, CIntelligentPointerThing));   // 0x00927B60
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
struct CTCDoorList {                            // interface 0x21
    unsigned char _pad[0x0c];
    CThingList Things;                          // +0x0C
};
struct CTCShopSign {                            // interface 0xBD
    unsigned char _pad[0x0c];
    bool Lit;                                   // +0x0C
};
struct CTCShopKeeper {                          // interface 0x90
    bool GetAreOtherPeopleInShop(bool b);       // 0x00799A60
};
struct CThingAICreature : CThing {
    unsigned char _pad94[0x170 - 0x94];
    CBaseIntelligentPointer WorkBuilding;       // +0x170
};
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long a, bool b, bool c, bool d, bool e, long f);   // 0x0097EC00
};
struct CAIStateGroup_OpenAndCloseShop : CAIStateGroupBase {
    bool GoToInsideFace;                        // +0x14
    bool Lock;                                  // +0x15
    virtual void Update();
};

void CAIStateGroup_OpenAndCloseShop::Update()
{
    CIntelligentPointerBuilding shop(GetCreature()->WorkBuilding.GetPItem());
    if (!shop.GetPItem() || (shop.GetPItem()->m_flags91 & 1)) {
        Terminate();
        return;
    }
    if (Lock) {
        CTCShopSign* sign;
        if (shop.GetPItem()->GetInterface(&sign, 0xbd))
            sign->Lit = true;
    }
    C3DVector pos;
    CThingPtrVector doors;
    const CTCDoorList* list;
    if (static_cast<const CThing*>(shop.GetPItem())->GetInterfaceC(&list, 0x21)) {
        for (CThingListNode* node = list->Things.begin(); node != list->Things.end(); node = node->_M_next) {
            CBaseIntelligentPointer& item = node->_M_data;
            if (item.GetPItem() && !(item.GetPItem()->m_flags91 & 1) && item.GetPItem()->HasInterface(0x26)
                && !(item.GetPItem()->m_flags91 & 0x20))
                doors.push_back(CIntelligentPointerThing(item.GetPItem()));
        }
    }
    CIntelligentPointerThing* last = doors._M_finish;
    SortDoors(doors._M_start, last, CompareDoors);
    unsigned int i = 0;
    CIntelligentPointerThing* door = doors._M_start;
    unsigned int count = last - door;
    long result = 3;
    for (; i < count; ++i, ++door) {
        if (!shop.GetPItem() || (shop.GetPItem()->m_flags91 & 1))
            break;
        if (!door->GetPItem() || (door->GetPItem()->m_flags91 & 1))
            continue;
        pos = door->GetPItem()->GetPosition();
        C3DVector front = pos + door->GetPItem()->PGraphic->GetRHSet()->UpScaled(1.1f);
        C3DVector back = pos - door->GetPItem()->PGraphic->GetRHSet()->UpScaled(1.1f);
        CThingBuilding* building = (CThingBuilding*)shop.GetPItem();
        if (building->IsPointInside(front) == GoToInsideFace) {
            bool reached;
            result = AIAction_MoveToPos(1, front, &reached, 5, true, true, false, true, 0);
        } else if ((building = (CThingBuilding*)shop.GetPItem())->IsPointInside(back) == GoToInsideFace) {
            bool reached;
            result = AIAction_MoveToPos(1, back, &reached, 5, true, true, false, true, 0);
        } else
            continue;
        if (result == 2)
            break;
    }
    if (result == 2 && shop.GetPItem() && !(shop.GetPItem()->m_flags91 & 1)) {
        AIAction_LookAtPos(1, pos, 1, 0.0027777f);
        if (Lock) {
            CThingAICreature* creature = GetCreature();
            if (creature->HasTCWord(0x90) && creature->GetTC((CTCShopKeeper*)0, 0x90)->GetAreOtherPeopleInShop(false)) {
                Terminate();
                return;
            }
        }
        shop->LockAllDoors(Lock, false);
        if (!Lock) {
            CThing* building = shop.GetPItem();
            if (building->HasTCWord(0xbd))
                building->GetTC((CTCShopSign*)0, 0xbd)->Lit = false;
        }
        long start = CWorld::GetWorldFrame();
        while (!(Lock ? shop->CountMoneyBags() : shop->IsUnlocked()) && CWorld::GetWorldFrame() - start <= 0x4b) {
            if (AIAction_DoNothing() != 4) {
                Terminate();
                return;
            }
        }
    }
    Terminate();
}
