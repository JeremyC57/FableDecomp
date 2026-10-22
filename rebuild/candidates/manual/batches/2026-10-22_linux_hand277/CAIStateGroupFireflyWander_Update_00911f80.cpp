// CAIStateGroup_FireflyWander::Update() -- by day hide the firefly and switch its light off; by night aim at home (more than 14 from it) or the hero (within 10 of home), fly a random path there and now and then show the firefly and light it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00911F80, 594 bytes. ?Update@CAIStateGroup_FireflyWander@@UAEXXZ (Ego_r.pdb).
// Callees: CAIStateGroupBase::GetPWorld 0x008FCFA0 / GetPPlayerManager 0x008FCF90 / GetPWorldSeed 0x008FCFD0 /
//   Terminate 0x008FCF40, the environment's day test 0x006BB2C0, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetHeroCreature 0x00487DC0, CThing::GetPosition 0x004C73D0, GFROR13 0x00497890, LowerBound 0x0040F020,
//   CTCLight::SetActive 0x006412B0, GetRandomFlightPos 0x00912700, ExecuteFlyingPath 0x009130D0, the creature's
//   virtual slots 27 (+0x6c, shown test) and 28 (+0x70, show/hide). Constants 196.0f @ 0x0128F99C, 100.0f @ 0x01230A08.
// Layout: StartPos +0x24 (Ego_r.pdb); creature home position +0xe8 (descriptive: read as the 2D anchor), world
//   environment +0x1c (descriptive); retail CThing interface bitset +0x20, map +0x44, flags +0x91.
// Shape: the two 2D offsets are C3DVector differences (the hero one is spilled, giving retail's 0x28 frame); the night
//   branch comes first; the show toggle calls GetCreature() twice; the day branch uses the out-parameter GetInterface
//   (component left in eax).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
unsigned long __fastcall GFROR13(unsigned long value);    // 0x00497890
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Get2DLengthSquared() const { return Y * Y + X * X; }
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a();
    virtual bool IsShown();                     // slot 27 (+0x6c)
    virtual void SetShown(bool shown);          // slot 28 (+0x70)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[2];
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CThingAICreature : CThing {
    unsigned char _pad94[0xe8 - 0x94];
    C3DVector InitialPos;                       // +0xe8
};
struct CTCLight {
    void SetActive(bool active);                // 0x006412B0
};
struct CPlayer { CThing* GetHeroCreature();     // 0x00487DC0
};
struct CPlayerManager { CPlayer* GetMainPlayer();   // 0x00449970
};
struct CEnvironment { bool IsDaytime();         // 0x006BB2C0
};
struct CWorld {
    unsigned char _pad00[0x1c];
    CEnvironment* PEnvironment;                 // +0x1c
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CPlayerManager* GetPPlayerManager() const;  // 0x008FCF90
    CWorld* GetPWorld() const;                  // 0x008FCFA0
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    unsigned long Random(unsigned long range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    float MinHeightToKeepAboveGround;           // +0x14
    float MinHeightToKeepAboveGroundInFlight;   // +0x18
    float MaxHeightAboveGround;                 // +0x1c
    float FlightRange;                          // +0x20
    bool GetRandomFlightPos(const C3DVector& centre, float range, C3DVector& out, bool flag) const;   // 0x00912700
    void ExecuteFlyingPath(const C3DVector& destination);   // 0x009130D0
};
struct CAIStateGroup_FireflyWander : CAIStateGroup_FlyingThingBase {
    C3DVector StartPos;                         // +0x24
    unsigned long FrameLastChanged;             // +0x30
    void Update();
};

void CAIStateGroup_FireflyWander::Update()
{
    if (!GetPWorld()->PEnvironment->IsDaytime()) {
        C3DVector target = StartPos;
        CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (hero && !(hero->m_flags91 & 1)) {
            CThingAICreature* me = GetCreature();
            C3DVector toHero = me->InitialPos - hero->GetPosition();
            CThingAICreature* self = GetCreature();
            C3DVector toHome = self->InitialPos - self->GetPosition();
            if (toHome.Get2DLengthSquared() > 196.0f)
                target = StartPos;
            else if (toHero.Get2DLengthSquared() < 100.0f)
                target = hero->GetPosition();
        }
        C3DVector destination;
        if (!GetRandomFlightPos(target, 6.0f, destination, true)) {
            Terminate();
            return;
        }
        if (!GetCreature()->IsShown() && Random(100) <= 45) {
            GetCreature()->SetShown(!GetCreature()->IsShown());
            CThingAICreature* lit = GetCreature();
            if (lit->HasTC(0x27))
                ((CTCLight*)lit->GetTC(0x27))->SetActive(lit->IsShown());
        }
        ExecuteFlyingPath(destination);
    } else if (GetCreature()->IsShown()) {
        GetCreature()->SetShown(false);
        CThingAICreature* creature = GetCreature();
        CTCLight* light;
        if (creature->GetInterface(&light, 0x27))
            light->SetActive(false);
    }
}
