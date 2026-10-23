// Grade: DIFFER(582v589) -- node/def/0 kept in frame+ebx in retail (ours ebp=node, no ebx zero), position X/Y read straight from GetPosition with only Z copied, CrossProduct second-term operand order
// CTCTavernGameCoinBase::FindObstacles() -- for every live thing in the table's obstacle list that has an obstacle def, add a fixed coin of the def's radius at the thing's position in board space, owned by that thing
// Leaf: ?FindObstacles@CTCTavernGameCoinBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EFB00, 589 bytes (+ int3 padding), plain `ret`, 8-aligned frame.
class CThing;
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(b.X * pa[2] - pa[0] * pb[2]), (float)(double)(pb[1] * pa[0] - pa[1] * b.X));
}
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CRightHandedSet {
    C3DVector Up, Forward;
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CRefCounted {
    long m_count;
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CObstacleDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x28 - 0x08];
    float Radius;                               // +0x28 (descriptive)
};
struct CDefPointer {
    CObstacleDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    CObstacleDef* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);           // 0x008F01B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    bool HasTC(long t) const { return (((const unsigned char*)m_presentBits)[t >> 3] & (1 << (t & 7))) != 0; }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    CBaseIntelligentPointer& operator=(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) { SetPItem(item); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
struct CCoinBase {                              // vtable 0x01288230
    virtual void v00();
    ~CCoinBase() { }
};
#pragma pack(push, 1)
struct Coin : CCoinBase {                       // 0x3c bytes
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    float     Speed;                            // +0x14
    float     Angle;                            // +0x18
    unsigned char _pad_0x1c[0x8];
    bool      Owned;                            // +0x24
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0x3];
    float     Radius;                           // +0x30
    int       Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
    Coin(float radius, float density, CThing* thing, bool owned);   // 0x008EF200
    Coin(const Coin& other);                    // 0x008EFF40
};
#pragma pack(pop)
struct SScalarTag { SScalarTag() { } };
inline void* operator new(unsigned int, void* where) { return where; }
struct CCoinVector {
    Coin* _M_start;
    Coin* _M_finish;
    Coin* _M_end_of_storage;
    void _M_insert_overflow(Coin* position, const Coin& x, const SScalarTag& tag, unsigned long fill_len, bool atend);   // 0x008F0450
    void push_back(const Coin& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) Coin(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
};
template <class T> struct SNode {
    SNode* _M_next;
    SNode* _M_prev;
    T      _M_data;
};
struct CTCObstacleList {                        // interface 0x21 (descriptive)
    unsigned char _pad00[0x0c];
    SNode<CBaseIntelligentPointer>* Things;     // +0x0c
};
struct CTCTavernGameCoinBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char   _pad_0x08[0xf4];
    CCoinVector     Coins;                      // +0xfc
    void*           Held;                       // +0x108
    C3DVector       BoardPos;                   // +0x10c
    CRightHandedSet BoardRHSet;                 // +0x118
    void FindObstacles();
};

void CTCTavernGameCoinBase::FindObstacles()
{
    if (!ParentThing->HasTC(0x21))
        return;
    CTCObstacleList* list = (CTCObstacleList*)ParentThing->GetTC(0x21);
    for (SNode<CBaseIntelligentPointer>* node = list->Things->_M_next; node != list->Things; node = node->_M_next) {
        CBaseIntelligentPointer& thing = node->_M_data;
        if (IsThingAlive(thing)) {
            CDefPointer def;
            if (thing.GetPItem()->PDef->GetSubDef(&def)) {
                C3DVector position = thing.GetPItem()->GetPosition();
                C3DVector offset = position - BoardPos;
                C2DVector pos(DotProduct(offset, BoardRHSet.GetRight()), DotProduct(offset, BoardRHSet.Forward));
                Coin coin(def->Radius, 0.0f, 0, false);
                coin.Pos = pos;
                coin.Thing = CIntelligentPointer(thing.GetPItem());
                Coins.push_back(coin);
            }
        }
    }
}
