// Grade: DIFFER(867v867) -- frame packing only: retail gives the interface-present/arrived byte its own word (+0x13) with the names and loop counter at +0x14 and the interface key, sound pair and can-see filter sharing +0x18; ours packs the byte into the key's word (+0x17) with names/filter/counter at +0x10; plus one 'mov ecx,esi' scheduled before the result==1 test (11 tries)
// CAIStateGroup_KidHurtSelfRunToMother::Update() -- trip over and get up (entering the hurt mode); then, one time in four, run to a living mother within 80 units, telling her once she can see the kid, and cry at her five times (looking at her for 2..4 seconds each); otherwise or when she is gone, just cry; terminate at the end or when an action fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00915350, length 867 bytes (+ int3 padding), plain `ret`.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetXYLengthSquared() const { return Y * Y + X * X; }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CThingCreatureBase : CThing {
    unsigned char _pad94[0x180 - 0x94];
    CBaseIntelligentPointer PMother;            // +0x180
};
__forceinline bool IsAlive(const CBaseIntelligentPointer& p)
{
    CThing* item = p.GetPItem();
    if (item == 0) return false;
    if (item->Flags91 & 1) return false;
    return true;
}
struct CThingFilter_CanSeeEachOther {
    CThingCreatureBase* Creature;
    CThingFilter_CanSeeEachOther(CThingCreatureBase* creature) : Creature(creature) { }
    bool operator()(CThing* other);             // 0x006A78D0
};
struct CTCCreatureModeManager {
    void AddMode(unsigned long mode);           // 0x0073AE10
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long g_FramesPerSecond;                  // 0x01375550
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
                                       bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float tolerance);   // 0x0097D6F0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_KidHurtSelfRunToMother : CAIStateGroupBase {
    bool Hurt;                                  // +0x14
    void SendMessageToMother();                 // 0x00915290
    long PerformRandomLengthCrying();           // 0x00914F00
    __forceinline bool IsMotherClose() const
    {
        CThingCreatureBase* creature = GetCreature();
        const C3DVector& mother = creature->PMother.GetPItem()->GetPosition();
        const C3DVector& me = creature->GetPosition();
        float dx = me.X - mother.X;
        float dy = me.Y - mother.Y;
        float distanceSquared = dy * dy + dx * dx;
        return distanceSquared < 6400.0f;
    }
    void Update();
};

void CAIStateGroup_KidHurtSelfRunToMother::Update()
{
    long result;
    {
        CCharString animation("BOY_TRIP_FALL_OVER_01");
        result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            true, true, false, 0, 0), false);
    }
    if (result == 1 || result == 3) {
        Terminate();
        return;
    }
    {
        CTCCreatureModeManager* modes;
        GetCreature()->GetInterface(&modes, 0x31);
        modes->AddMode(0x14);
    }
    Hurt = true;
    bool failed;
    {
        CCharString animation("BOY_TRIP_GET_UP_01");
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            false, true, false, 0, 0), false) != 2;
    }
    if (failed) {
        Terminate();
        return;
    }
    unsigned long* seed = GetPWorldSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    if (*seed % 100 < 75) {
        Terminate();
        return;
    }
    if (!IsAlive(GetCreature()->PMother) || !IsMotherClose()) {
        PerformRandomLengthCrying();
        Terminate();
        return;
    }
    bool told;
    {
        bool arrived = false;
        CThingFilter_CanSeeEachOther canSee(GetCreature());
        told = false;
        for (;;) {
            CThing* item = GetCreature()->PMother.GetPItem();
            result = AIAction_MoveToThingProximity(2, item, 1.0f, &arrived, 5,
                                                   true, false, true, true, 0.0f);
            if (result != 4)
                break;
            if (!told && canSee(GetCreature()->PMother.GetPItem())) {
                SendMessageToMother();
                told = true;
            }
        }
    }
    if (result != 2) {
        Terminate();
        return;
    }
    if (!told)
        SendMessageToMother();
    for (long i = 0; i < 5; ++i) {
        if (result == 1) {
            Terminate();
            return;
        }
        long fps = g_FramesPerSecond;
        unsigned long* s = GetPWorldSeed();
        *s = *s * 0x24a1 + 0x24df;
        *s = GFROR13(*s);
        long wait = (long)(*s % 30);
        long end = CWorld::GetWorldFrame() + wait + fps * 2;
        while (end > CWorld::GetWorldFrame() && result != 1)
        {
            CThing* item = GetCreature()->PMother.GetPItem();
            result = AIAction_LookAtThing(2, item, 1, 0.0027777f);
        }
        result = PerformRandomLengthCrying();
    }
    Terminate();
}
