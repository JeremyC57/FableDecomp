// CAIStateGroup_MinionGuardReactToCriminal::Update() -- confront the criminal hero: decay and refresh the warning level, then chase and warn the hero until they stop being a criminal, flee or run out of warnings; otherwise give the recent-crime remark; finally speak the closing line and flag the village
// Grade: DIFFER(1692v1671) -- block structure follows retail; residue: frame 0x68 v 0x48 (vector temporaries), x87 schedule of the 2D-length/normalise block, the warning-frame advance (ours folds fps+fps*4 into fps*5), MoveToPosProximity vector built through a temporary
// Layout: local model (the engine header flattens the base); PCriminal +0x14, LastCriminalPos +0x1c, WarningLevel +0x28, FrameLastUpdated +0x2c, LastCrimeType +0x30.
extern "C" int __cdecl strcmp(const char*, const char*);
#pragma intrinsic(strcmp)
struct CTCBase;
extern unsigned long g_InvSqrtMantissaTable[128];      // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Get2DLength() const { return (float)sqrt(X * X + Y * Y); }
    __forceinline C3DVector Normalised() const { return *this * FastInvSqrt(X * X + Y * Y + Z * Z); }
};
struct CBasicStringChar {
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharString {
    CBasicStringChar* PData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& o);   // 0x0099EFB0
    bool operator!=(const char* text) const;    // 0x0099E960
    __forceinline bool operator==(const char* text) const
    {
        if (PData == 0) {
            if (strcmp("", text) == 0)
                return true;
            return false;
        }
        return *PData == text;
    }
};
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
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) { SetPItem(item); }
};
struct CTCVillageMember;
struct CTCVillage {
    unsigned char _pad[0x7c];
    bool HeroDeclaredCriminal;                  // +0x7c
    unsigned char _pad7d[0xdc - 0x7d];
    CBaseIntelligentPointer PGuardDealingWithHero;   // +0xdc
    CCharString GetWarningStringForCurrentCrimes(long level, bool* final, long* crimeType);   // 0x0065A075
    bool IsHeroCriminal();                      // 0x0065BFCE
    void SetGuardDealingWithHero(CThing* guard);    // 0x00933280
};
CTCVillage* __fastcall GetVillage(CTCVillageMember* member);   // 0x007EE540
bool __fastcall IsThingRunningAwayFromThing(const CThing* runner, const CThing* from, const C3DVector* oldPos);   // 0x00864170
struct CTCCrimeMemory {                         // interface 0xB4
    unsigned char _pad[0x44];
    long LastCrimeKind;                         // +0x44
    long LastCrimeFrame;                        // +0x48
};
extern CCharString g_GuardTextNotCriminal;      // 0x013BB6C4
extern CCharString g_GuardTextHeedWarning;      // 0x013BB6C8
extern CCharString g_GuardTextRunningAway;      // 0x013BB6CC
extern CCharString g_GuardTextCrime1;           // 0x013BB6D0
extern CCharString g_GuardTextCrime2;           // 0x013BB6D4
extern CCharString g_GuardTextCrime3;           // 0x013BB6D8
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    void Terminate();                           // 0x008FCF40
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* arrived, long movement,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    long AIAction_SpeakAISpeechToTarget(long repeat, CIntelligentPointerCreature speaker,
        CIntelligentPointerConstThing target, const CCharString& speech, bool interrupt, long priority);  // 0x00981EA0
};
struct CAIStateGroup_MinionGuardReactToCriminal : CAIStateGroupBase {
    CBaseIntelligentPointer PCriminal;          // +0x14
    C3DVector LastCriminalPos;                  // +0x1c
    long WarningLevel;                          // +0x28
    long FrameLastUpdated;                      // +0x2c
    long LastCrimeType;                         // +0x30
    void Update();
};

void CAIStateGroup_MinionGuardReactToCriminal::Update()
{
    if (IS_THING_ALIVE(PCriminal)) {
    CTCVillageMember* member;
    PBrain->PCreature->GetInterfaceT(&member, 0x23);
    CTCVillage* village = GetVillage(member);
    if (village != 0) {
    WarningLevel += (GFGetCurrentFrame() - FrameLastUpdated) / (g_FramesPerSecond * -20);
    if (WarningLevel <= 0)
        WarningLevel = 0;
    FrameLastUpdated = GFGetCurrentFrame();
    bool finalWarning = false;
    long crimeType = -1;
    CCharString warning = village->GetWarningStringForCurrentCrimes(0, &finalWarning, &crimeType);
    if (crimeType != -1 && crimeType != LastCrimeType) {
        WarningLevel = 0;
        LastCrimeType = crimeType;
    }
    warning = village->GetWarningStringForCurrentCrimes(WarningLevel, &finalWarning, &crimeType);
    CCharString speech;
    if (warning != "") {
        CThing* dealer = village->PGuardDealingWithHero.GetPItem();
        if (dealer != 0 && !(dealer->m_flags91 & 1)) {
            Terminate();
            return;
        }
        village->SetGuardDealingWithHero(PBrain->PCreature);
        LastCriminalPos = PCriminal.GetPItem()->GetPosition();
        long warnFrame = GFGetCurrentFrame();
        do {
            bool arrived;
            long result = AIAction_MoveToThingProximity(2, PCriminal, 3.0f, &arrived, 5, true, false, true, true, 0.0f);
            if (result == 2) {
                CThing* creature = PBrain->PCreature;
                const C3DVector& target = PCriminal.GetPItem()->GetPosition();
                C3DVector offset = creature->GetPosition() - target;
                if (offset.Get2DLength() < 2.39f) {
                    C3DVector away = offset.Normalised() * 2.4f * 2.0f;
                    const C3DVector& p = PCriminal.GetPItem()->GetPosition();
                    result = AIAction_MoveToPosProximity(2, C3DVector(away.X + p.X, away.Y + p.Y, away.Z + p.Z), 2.4f, &arrived, 3,
                                                         false, false, true, 0, true, 0.0f);
                }
                if (result == 2)
                    result = AIAction_LookAtThing(2, PCriminal, 0xb, 0.0027777778f);
            }
            if (result == 1) {
                Terminate();
                return;
            }
            if (finalWarning) {
                CThing* creature = PBrain->PCreature;
                if (IsThingRunningAwayFromThing(PCriminal.GetPItem(), creature, &LastCriminalPos)) {
                    speech = g_GuardTextRunningAway;
                    break;
                }
            }
            long elapsed = GFGetCurrentFrame() - warnFrame;
            if ((result == 2 && elapsed >= 0) || elapsed > g_FramesPerSecond * 4) {
                if (warning == "") {
                    speech = g_GuardTextHeedWarning;
                    break;
                }
                if (AIAction_SpeakAISpeechToTarget(1, PBrain->PCreature, PCriminal, warning, true, 0x93) == 1) {
                    Terminate();
                    return;
                }
                ++WarningLevel;
                long fps = g_FramesPerSecond;
                warnFrame += fps;
                warnFrame += fps * 4;
                warnFrame = (long)(GFFloatRandom(GetPWorldSeed(), 1.0f) * (fps * 5) + warnFrame);
                crimeType = -1;
                warning = village->GetWarningStringForCurrentCrimes(0, &finalWarning, &crimeType);
                if (crimeType != -1 && crimeType != LastCrimeType) {
                    WarningLevel = 0;
                    LastCrimeType = crimeType;
                }
                warning = village->GetWarningStringForCurrentCrimes(WarningLevel, &finalWarning, &crimeType);
            }
        } while (village->IsHeroCriminal());
        if (!village->IsHeroCriminal())
            speech = g_GuardTextNotCriminal;
    } else {
        CTCCrimeMemory* memory;
        if (PBrain->PCreature->GetInterfaceT(&memory, 0xb4)) {
            long crimeFrame = memory->LastCrimeFrame;
            if (GFGetCurrentFrame() - crimeFrame < g_FramesPerSecond * 3) {
                switch (memory->LastCrimeKind) {
                case 1: speech = g_GuardTextCrime1; break;
                case 2: speech = g_GuardTextCrime2; break;
                case 3: speech = g_GuardTextCrime3; break;
                }
            }
        }
    }
    if (speech != "") {
        CThing* creature = PBrain->PCreature;
        AIAction_SpeakAISpeechToTarget(1, creature, PCriminal, speech, true, 0x93);
    }
    if (village->IsHeroCriminal())
        village->HeroDeclaredCriminal = true;
    }
    }
    Terminate();
}
