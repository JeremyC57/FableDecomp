// CTCDecapitation::Decapitate() -- once only: credit the killing hero's decapitation count (and his online scoreboard), then for a creature with a random appearance morph switch to the headless morph set, find the head (the "r" dummy, else position + head offset with the physics orientation), spawn the head object (not solid to the hero, flung upwards, spinning, hidden flag), and attach the blood-spurt emitter at the neck
// Grade: DIFFER(1157v1244) -- first draft: frame 0x54 v 0x3c (head set/position/temp slot sharing), this in ebp (retail edi); the head-orientation copy and the fast-inverse-sqrt fling sequence differ; flow and callees as retail
// Leaf: Decapitate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EF710, 1244 bytes, plain `ret`, no EH.
// Callees: GetPItem 0x00A01B50, LowerBound 0x0040F020 / const 0x004365B0, CTCHeroOnlineScoreboard::UpdateScoreForActivity
// 0x0055F3B0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 / Compare 0x00411570, the morph's type-name getter (slot 22),
// CTCRandomAppearanceMorph::UpdateMorphSet 0x007283F0, CTCGraphicAppearance::GetDummyObject 0x004C08C0, CThing::GetPosition
// 0x004C73D0, physics GetOrientation (slot 72) / GetVelocity (slot 70), CThingObject::Create 0x00703210, CThing::AddTC
// 0x004C9D60, CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetCharacterThing
// 0x00487DC0, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, CTCPhysicsBase::ApplyAcceleration 0x006B0210,
// C3DVector::SetFromSpherical 0x00A14260, CTCRotate::SetRotationInfo 0x0072A530, CreateEffectThing 0x006E0880,
// CTCDParticleEmitter::AttachToThing 0x006E0BE0. Globals: head dummy name 0x013BAF0C, head offset 0x01384374.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void SetFromSpherical(float a, float b);                 // 0x00A14260
};
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct CStringData { const char* Data; unsigned long Length; };
long __fastcall Compare(const char* a, const char* b);      // 0x00411570
struct CCharString {
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (!PStringData || !other.PStringData)
            return false;
        if (PStringData->Length != other.PStringData->Length)
            return false;
        return Compare(PStringData->Data, other.PStringData->Data) == 0;
    }
};
extern CCharString g_HeadDummyName;                          // 0x013BAF0C
extern C3DVector g_HeadOffset;                               // 0x01384374
extern unsigned long g_InvSqrtTable[];                       // 0x013CE558
inline float FastInvSqrt(float value)
{
    unsigned long bits = *(unsigned long*)&value;
    unsigned long result = ((0x17c - ((bits >> 23) & 0xff)) << 22) & 0xff800000;
    result |= g_InvSqrtTable[(bits >> 17) & 0x7f] << 15;
    float y = *(float*)&result;
    return (3.0f - y * y * value) * y * 0.5f;
}
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    const SEntry* LowerBound(const long* type) const;        // 0x004365B0
};
struct CTCPhysicsBase {
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CPhysics : CTCPhysicsBase {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31) VF(32) VF(33)
    VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50)
    VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67)
    VF(68) VF(69)
#undef VF
    virtual const C3DVector& GetVelocity(C3DVector* out);   // slot 70 (+0x118)
    virtual void v71();
    virtual const CRightHandedSet& GetOrientation();         // slot 72 (+0x120)
};
struct CTCGraphicAppearance {
    unsigned char _pad00[0x59];
    unsigned char Flags59;                                   // +0x59
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set,
                        float scale);                        // 0x004C08C0
};
struct CTCParamsBase;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    struct CTCDParticleEmitter* Emitter;                     // +0x5c
    CPhysics* Physics;                                       // +0x60
    CTCGraphicAppearance* Appearance;                        // +0x64
    unsigned char _pad68[0x6c - 0x68];
    unsigned char Flags6c;                                   // +0x6c
    unsigned char _pad6d[0x90 - 0x6d];
    signed char Region;                                      // +0x90
    unsigned char Flags91;                                   // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool persist, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            tc = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return tc;
    }
    template<class T> __forceinline const T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (const T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
CThing* __fastcall CreateEffectThing(long effectIndex, const C3DVector& pos, bool persistent);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CCreature : CThing {
    unsigned char _pad94[0xf4 - 0x94];
    CBaseIntelligentPointer Killer;                          // +0xf4
};
struct CTCHeroStats { unsigned char _pad[0x24]; long Decapitations; };   // interface 4
struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float value, long a);   // 0x0055F3B0
};
struct CTCAppearanceMorph {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21)
#undef VF
    virtual CCharString GetClassTypeName() const;            // slot 22 (+0x58)
};
struct CTCRandomAppearanceMorph { void UpdateMorphSet(); };  // 0x007283F0
struct CTCSolidToThing { void AddThingNotSolidTo(CThing* thing); };   // 0x00770FA0
struct CTCRotate {
    unsigned char _pad00[0x28];
    bool Active;                                             // +0x28
    void SetRotationInfo(const C3DVector& axis, float speed, float duration);   // 0x0072A530
};
struct CPlayer { CThing* GetCharacterThing(); };            // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };        // 0x00449970
struct CDecapitationDef {
    unsigned char _pad00[0x28];
    long BloodEffect;                                        // +0x28
    long HeadDefIndex;                                       // +0x2c
};
struct CTCDecapitation {
    virtual void v00();
    CCreature* ParentThing;                                  // +0x04
    unsigned char _pad08[0x0c - 0x08];
    bool Decapitated;                                        // +0x0c
    unsigned char _pad0d[0x10 - 0x0d];
    const CDecapitationDef* PDef;                            // +0x10
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
    void Decapitate();
};

void CTCDecapitation::Decapitate()
{
    if (Decapitated)
        return;
    if (ParentThing->Flags6c & 8) {
        CThing* killer = ParentThing->Killer.GetPItem();
        if (killer && !(killer->Flags91 & 1) && killer->HasTC(4)) {
            long count = ++killer->GetTC<CTCHeroStats>(4)->Decapitations;
            killer->GetTC<CTCHeroOnlineScoreboard>(0x10d)->UpdateScoreForActivity(1, (float)count, 0);
        }
    }
    if (!ParentThing->HasTC(3))
        return;
    const CTCAppearanceMorph* morph = ParentThing->GetTCConst<CTCAppearanceMorph>(3);
    bool random = morph->GetClassTypeName() == CCharString("CTCRandomAppearanceMorph");
    if (!random)
        return;
    Decapitated = true;
    ParentThing->GetTC<CTCRandomAppearanceMorph>(3)->UpdateMorphSet();
    C3DVector headPos;
    CRightHandedSet headSet;
    bool found = ParentThing->Appearance->GetDummyObject(g_HeadDummyName, 0, 0, &headPos, &headSet, 1.0f);
    if (!found) {
        const C3DVector& pos = ParentThing->GetPosition();
        headPos = C3DVector(g_HeadOffset.X + pos.X, g_HeadOffset.Y + pos.Y, g_HeadOffset.Z + pos.Z);
        headSet = ParentThing->Physics->GetOrientation();
    }
    if (PDef->HeadDefIndex) {
        CThing* head = CreateThingObject(PDef->HeadDefIndex, &headPos, ParentThing->Region, &headSet, 0, "");
        if (head && !(head->Flags91 & 1)) {
            CTCSolidToThing* solid = (CTCSolidToThing*)head->AddTC("CTCSolidToThing", true, 0);
            solid->AddThingNotSolidTo(GetPlayerManager()->GetMainPlayer()->GetCharacterThing());
            C3DVector velocity;
            const C3DVector& v = ParentThing->Physics->GetVelocity(&velocity);
            float z = v.Z + 0.3f;
            float inv = FastInvSqrt(v.X * v.X + z * z + v.Y * v.Y);
            C3DVector fling(v.X * inv * 0.4f, v.Y * inv * 0.4f, z * inv * 0.4f);
            head->Physics->ApplyAcceleration(fling);
            CTCRotate* rotate = (CTCRotate*)head->AddTC("CTCRotate", false, 0);
            if (rotate) {
                C3DVector axis(0.0f, 0.0f, 1.0f);
                axis.SetFromSpherical(0.0f, 0.0f);
                rotate->SetRotationInfo(axis, 0.05f, 2.2f);
                rotate->Active = true;
            }
            head->Appearance->Flags59 |= 2;
        }
    }
    if (PDef->BloodEffect > 0) {
        CThing* blood = CreateEffectThing(PDef->BloodEffect, headPos, false);
        if (blood && !(blood->Flags91 & 1)) {
            CTCDParticleEmitter* emitter;
            if (blood->HasTC(0x4b))
                emitter = blood->Emitter;
            if (found)
                emitter->AttachToThing(ParentThing, 0xa, g_HeadDummyName, 0, 0.0f);
            else
                emitter->AttachToThing(ParentThing, 0xa8, "", 0, g_HeadOffset.Z);
        }
    }
}
