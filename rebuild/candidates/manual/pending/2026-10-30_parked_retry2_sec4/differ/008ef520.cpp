// CTCTavernGameCoinBase::UpdateCoins(float dt) -- integrate every coin (position, spin, surface friction), give up to state 0x2F when a coin's thing has died, and collide every pair of live, moving, overlapping coins, playing the coin-hit sound on one of them
// Grade: DIFFER(999v998) nd=242 (was 1010v998 nd=247) -- lever: the position integration written per component (Pos.X += dt*Vel.X; Pos.Y += ...); tried: scalar-on-the-right product (247), a user C2DVector copy ctor (978v998 nd=238; with per-component 979 nd=236); remaining: frame 0x44 v 0x34 (16 bytes: the two contact-point C2DVector temporaries), the coin-pair loop register allocation
// Leaf: ?UpdateCoins@CTCTavernGameCoinBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EF520, 996 bytes (+ int3 padding), `ret 4`, 8-aligned frame.
#include <math.h>
class CThing;
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
inline C2DVector operator*(float s, const C2DVector& v) { return C2DVector(s * v.X, s * v.Y); }
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool IsHeld() const;                // slot 27 (+0x6c) (descriptive)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long Flags6C;                      // +0x6c bit 15 (descriptive)
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                      // +0x91 bit 0 dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
inline bool IsResting(const CBaseIntelligentPointer& p)
{
    return IsThingAlive(p) && !p.GetPItem()->IsHeld() && !(p.GetPItem()->Flags6C & 0x8000);
}
#pragma pack(push, 1)
struct Coin {                                   // 0x3c bytes
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    C2DVector Velocity;                         // +0x0c
    float     Speed;                            // +0x14 (descriptive)
    float     Angle;                            // +0x18
    float     AngularVelocity;                  // +0x1c (descriptive)
    unsigned char _pad_0x20[0x4];
    bool      Owned;                            // +0x24
    CBaseIntelligentPointer Thing;              // +0x25
    unsigned char _pad_0x2d[0x3];
    float     Radius;                           // +0x30
    int       Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
    void BackplaneFriction(float friction, float radius);   // 0x008EEBE0
};
#pragma pack(pop)
bool __fastcall Collide(Coin& a, Coin& b, C2DVector contactA, C2DVector contactB, C2DVector normal,
                        float restitution, float friction, long flags, float dt);   // 0x008EEC70
struct CTCSoundPlayer {                         // interface 0x10
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTavernGameCoinBaseDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x20];
    float         CoinIDensity;                 // +0x28
    float         CoinRadius;                   // +0x2c
    float         CoinEdgeRestitution;          // +0x30
    float         CoinEdgeFriction;             // +0x34
    float         CoinSurfaceFriction;          // +0x38
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCoinBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameCoinBaseDef* operator->() const { return Object; }
};
struct CThingDefHolder {
    bool GetDef(CDefPointer* out);              // 0x008F00F0
};
struct CThingDefOwner {
    unsigned char    _pad_0x00[0x70];
    CThingDefHolder* DefHolder;                 // +0x70
};
struct CCoinVector {
    Coin* _M_start;
    Coin* _M_finish;
    Coin* _M_end_of_storage;
};
struct CTCTavernGameCoinBase {
    virtual void v00();
    CThingDefOwner* ParentThing;                // +0x04
    unsigned char   _pad_0x08[0xf4];
    CCoinVector     Coins;                      // +0xfc
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
    void UpdateCoins(float dt);
};

void CTCTavernGameCoinBase::UpdateCoins(float dt)
{
    CDefPointer def;
    ParentThing->DefHolder->GetDef(&def);
    for (Coin* coin = Coins._M_start; coin != Coins._M_finish; ++coin) {
        coin->Pos.X += dt * coin->Velocity.X;
        coin->Pos.Y += dt * coin->Velocity.Y;
        coin->Angle += dt * coin->AngularVelocity;
        coin->BackplaneFriction(dt * def->CoinSurfaceFriction, coin->Radius);
    }
    for (Coin* a = Coins._M_start; a != Coins._M_finish; ++a) {
        if (!IsThingAlive(a->Thing)) {
            SetState(0x2f);
            return;
        }
        if (IsResting(a->Thing))
            continue;
        for (Coin* b = Coins._M_start; b != a; ++b) {
            if (!IsThingAlive(b->Thing)) {
                SetState(0x2f);
                return;
            }
            if (IsResting(b->Thing))
                continue;
            if (a->Speed == 0.0f && b->Speed == 0.0f)
                continue;
            float dy = a->Pos.Y - b->Pos.Y;
            float dx = a->Pos.X - b->Pos.X;
            float distSq = dx * dx + dy * dy;
            if (distSq == 0.0f)
                continue;
            float radii = b->Radius + a->Radius;
            if (!(distSq < radii * radii))
                continue;
            float inv = 1.0f / (float)sqrt(distSq);
            C2DVector normal(dx * inv, dy * inv);
            if (Collide(*a, *b, C2DVector(a->Pos.X - normal.X * a->Radius, a->Pos.Y - normal.Y * a->Radius),
                        C2DVector(b->Pos.X + normal.X * b->Radius, b->Pos.Y + normal.Y * b->Radius), normal,
                        def->CoinEdgeRestitution, def->CoinEdgeFriction, 0, dt)) {
                CIntelligentPointer thing;
                thing.SetPItem(a->Thing.GetPItem());
                if (!thing.GetPItem())
                    thing.SetPItem(b->Thing.GetPItem());
                if (thing.GetPItem()) {
                    CTCSoundPlayer* sound;
                    thing.GetPItem()->GetInterface(&sound, 0x10);
                    sound->PlaySound(CCharString("TAVERN_GAME_COIN_HIT_COIN"), 1, 0, 0, -1);
                }
            }
        }
    }
}
