// CAIStateGroup_WanderAroundShops::Update() -- register as a shopper of the live shop, then for the configured number of browses: walk up to the pretty thing, look at it, keep looking at it for a random time, and pick the next pretty thing near the shop; terminates when anything dies or an action fails
// Leaf: ?Update@CAIStateGroup_WanderAroundShops
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009474F0, length 647 bytes (aligned `and esp,-8` frame for the
//   intelligent-pointer temporary, virtual slot 12).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (interface 0x53 CTCShop,
//   ternary pick), CTCShop::AddShopper @ 0x00773DC0, CThing::GetPosition @ 0x004C73D0, AIAction_MoveToPosProximity
//   @ 0x0097E890, AIAction_LookAtPos @ 0x0097D580 (x2), GetPWorldSeed @ 0x008FCFD0, GFRandom @ 0x004CDAE0 (__fastcall),
//   ftol @ 0x00BFEA70, GFGetCurrentFrame @ 0x0049D870, the pretty-thing finder @ 0x00947110 (manifest
//   "CAIStateGroup_WanderAroundShops_FindRandomNPC"), CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0 / operator=
//   @ 0x004A9760, ~CBaseIntelligentPointer @ 0x00A01C10, Terminate @ 0x008FCF40; globals: frame rate 0x01375550, browse
//   count 0x0138CE20 (unsigned), browse radius 0x0138CE24 (float), browse seconds 0x0138CE28 (float).
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//   CAIStateGroupBase model (as the landed IsRunnable 0x00947280).
// Shape: the pretty thing's GetPItem is a named local before each action call (retail calls GetPItem before the
//   constant pushes); the brain is a named local for AddShopper (eax reused); the look speed literal is 0.0027777f
//   (0x3B360A13, not 1/360); the browse loop is `for (unsigned i = 0; i < count; ++i)` with `break` exits that share the
//   final Terminate, while the timed look loop's timeout keeps its own `Terminate(); return;` copy.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class C3DVector { public:
    float x, y, z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
public:
    CIntelligentPointer(T* item);     // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CThingCreatureBase;
class CTCShop { public:                                // interface 0x53
    void AddShopper(CThingCreatureBase* shopper);   // 0x00773DC0
};
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870
extern long g_FramesPerSecond;                  // 0x01375550
extern unsigned long g_ShopBrowseCount;         // 0x0138CE20
extern float g_ShopBrowseRadius;                // 0x0138CE24
extern float g_ShopBrowseSeconds;               // 0x0138CE28
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* arrived, long movement, bool b1,
        bool b2, bool b3, CThing* faceThing, bool b4, float f);    // 0x0097E890
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
};
struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    CIntelligentPointer<CThing> Shop;              // +0x14
    CIntelligentPointer<CThing> PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    CThing* SelectPrettyThing(CThing* shop);    // 0x00947110
    virtual void Update();
};

void CAIStateGroup_WanderAroundShops::Update()
{
    if (IS_THING_ALIVE(Shop)) {
        CThing* shop = Shop.GetPItem();
        CTCShop* shopTC;
        bool has = ((shop->m_presentBits[0x53 >> 5] >> (0x53 & 31)) & 1) != 0;
        if (has) {
            long type = 0x53;
            CInterfaceMap* map = &shop->m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            shopTC = (CTCShop*)((it == map->last || 0x53 < it->type) ? map->last : it)->component;
        }
        CAIBrain* brain = PBrain;
        shopTC->AddShopper(brain->PCreature);
        for (unsigned long i = 0; i < g_ShopBrowseCount; ++i) {
            long result;
            do {
                if (!IS_THING_ALIVE(PrettyThing)) {
                    Terminate();
                    return;
                }
                bool arrived;
                CThing* pretty = PrettyThing.GetPItem();
                result = AIAction_MoveToPosProximity(2, pretty->GetPosition(), g_ShopBrowseRadius, &arrived,
                    3, true, false, true, 0, true, 0.0f);
            } while (result == 4);
            if (result != 2)
                break;
            do {
                if (!IS_THING_ALIVE(PrettyThing)) {
                    Terminate();
                    return;
                }
                CThing* pretty = PrettyThing.GetPItem();
                result = AIAction_LookAtPos(2, pretty->GetPosition(), 0x11, 0.0027777f);
            } while (result == 4);
            if (result != 2)
                break;
            long fps = g_FramesPerSecond;
            long endFrame = GFRandom((long)(fps * g_ShopBrowseSeconds), GetPWorldSeed()) + CWorld::GetFrame();
            for (;;) {
                if (!IS_THING_ALIVE(PrettyThing)) {
                    Terminate();
                    return;
                }
                CThing* pretty = PrettyThing.GetPItem();
                result = AIAction_LookAtPos(2, pretty->GetPosition(), 0x11, 0.0027777f);
                if (result != 4)
                    break;
                if (CWorld::GetFrame() >= endFrame) {
                    Terminate();
                    return;
                }
            }
            if (result != 2)
                break;
            PrettyThing = SelectPrettyThing(Shop.GetPItem());
            if (!PrettyThing.GetPItem())
                break;
        }
    }
    Terminate();
}