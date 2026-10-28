// CAIStateGroup_MinionGuardBribed::Update() -- hold the main player's character; while the state stays valid and the hero lives, look away from the hero (at the point mirrored through the creature) and idle, whistling (WHISTLE_GUARD_BRIBED) at random intervals; afterwards stop the whistle, clear the village's bribe frame and Terminate()
// Grade: RELOCATION_MATCH (retry A 11a) -- the mirrored look point is a named C3DVector local (materialised, Z-first), the player character goes through an inline GetPCharacter() accessor (lea ecx,[eax+34h]), the first delay is an unsigned long added last (frame + min + delay) and the repeat target is `next = frame + delay; next += min;`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929B10, length 785 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8` frame.
// Callees: GetPPlayerManager @ 0x008FCF80, CPlayerManager::GetMainPlayer const @ 0x00449960, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, GetPWorld @ 0x008FCFC0, GetPWorldSeed
// @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CWorld::GetWorldFrame @ 0x0049D870 (static), the state's virtual slot 11
// (+0x2c, still valid), CThing::GetPos @ 0x004C73D0, AIAction_LookAtPos @ 0x0097D580, AIAction_DoNothing @ 0x0097E200,
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the sound component's PlaySound @ 0x0066BD00 /
// StopSample @ 0x0066ADD0, CVectorMap::LowerBound @ 0x0040F020, CTCVillageMember::GetPVillage @ 0x007EE540, Terminate @ 0x008FCF40.
// Data: whistle delay ranges @ 0x0138CB98/0x0138CB9C (first) and 0x0138CBA0/0x0138CBA4 (repeat); g_InvalidSoundHandle @ 0x0129D25C;
// "WHISTLE_GUARD_BRIBED" @ 0x01291A00; 0.0027777f turn speed (immediate).
// Layout: local model (PBrain +4; brain PCreature +0x18; player +0x34 CIntelligentPointer to the character; presence bits +0x20,
// interface map +0x44; interface 0x10 = sound component, 0x23 = CTCVillageMember; village bribe frame +0xA8).
// Shape: the random delays are a static CWorld random called through GetPWorld() (the discarded call); the in-loop sound lookup
// inlines the vector-map lower_bound, the after-loop one calls it.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    static __forceinline SEntry* LowerBoundInline(SEntry* first, SEntry* end, long key)
    {
        long len = end - first;
        SEntry* it = first;
        while (len > 0) {
            long half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < key) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            CInterfaceMap::SEntry* end = m_interfaces.last;
            CInterfaceMap::SEntry* it = CInterfaceMap::LowerBoundInline(m_interfaces.first, end, interfaceType);
            *ppOut = (T*)((it == end || interfaceType < it->type) ? end : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { public: };
class CTCSoundPlayer { public:                           // interface 0x10
public:
    void StopAttachedSound(unsigned long);   // 0x0066ADD0
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};
struct CVillage {
    unsigned char _pad[0xa8];
    long BribedFrame;                           // +0xA8
};
class CTCVillageMember { public:                       // interface 0x23
    CVillage* GetPVillage();                    // 0x007EE540
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerPlayerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    __forceinline CIntelligentPointerPlayerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CPlayer { public:
    unsigned char _pad[0x34];
    CBaseIntelligentPointer PCharacter;         // +0x34
    const CBaseIntelligentPointer& GetPCharacter() const { return PCharacter; }
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;       // 0x00449960
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
    static __forceinline unsigned long Random(unsigned long range, unsigned long* seed)
    {
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
extern long g_BribedWhistleFirstMin;            // 0x0138CB98
extern long g_BribedWhistleFirstMax;            // 0x0138CB9C
extern long g_BribedWhistleRepeatMin;           // 0x0138CBA0
extern long g_BribedWhistleRepeatMax;           // 0x0138CBA4
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
#define VF(n) virtual void v##n();
    VF(0) VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    virtual bool IsStillValid();                // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    const CPlayerManager* GetPPlayerManager() const;    // 0x008FCF80
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float turnSpeed);   // 0x0097D580
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_MinionGuardBribed : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionGuardBribed::Update()
{
    CIntelligentPointerPlayerCreature hero(GetPPlayerManager()->GetMainPlayer().GetPCharacter().GetPItem());
    unsigned long delay = GetPWorld()->Random(g_BribedWhistleFirstMax - g_BribedWhistleFirstMin, GetPWorldSeed());
    long nextWhistle = CWorld::GetWorldFrame() + g_BribedWhistleFirstMin + delay;
    unsigned long sample = g_InvalidSoundHandle;
    while (IsStillValid() && IS_THING_ALIVE(hero)) {
        C3DVector heroPos = hero.GetPItem()->GetPos();
        const C3DVector& here = GetCreature()->GetPos();
        C3DVector lookPos = here * 2.0f - heroPos;
        if (AIAction_LookAtPos(1, lookPos, 0x11, 0.0027777f) != 2)
            break;
        if (AIAction_DoNothing() != 4)
            break;
        if (GetPWorld()->GetWorldFrame() >= nextWhistle) {
            CTCSoundPlayer* sound;
            if (GetCreature()->GetInterfaceInline(&sound, 0x10))
                sample = sound->PlayAttachedCriteria("WHISTLE_GUARD_BRIBED", 0, 0, 0, -1);
            delay = GetPWorld()->Random(g_BribedWhistleRepeatMax - g_BribedWhistleRepeatMin, GetPWorldSeed());
            nextWhistle = CWorld::GetWorldFrame() + delay;
            nextWhistle += g_BribedWhistleRepeatMin;
        }
    }
    if (sample != g_InvalidSoundHandle) {
        CTCSoundPlayer* sound;
        if (GetCreature()->GetInterface(&sound, 0x10))
            sound->StopAttachedSound(sample);
    }
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member, 0x23) && member->GetPVillage())
        member->GetPVillage()->BribedFrame = 0;
    Terminate();
}