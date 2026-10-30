// CTCBerserk::Update (vtable slot 9) -- per-frame berserk: on release queue the release action, then (once it is accepted) make the noise, kill the start-up effect, register the berserk opinion source and, at a level, knock back every non-friendly thing in the def's radius; while charging ramp TC 3's strength and the owner controller's tint/scale up; count the timer down (respecting pause and cutscenes), keep the radial blur on the owner, and when the timer runs out clean the ability up and remove TC 0x84, otherwise ramp the strength/tint down
// Grade: DIFFER(1614v1729) -- 580 difflines (base 1592/568); ROUND retail_asm helper (landed 0x006472B2 form) gives retail's bare dword fistp at both sites; first-draft debt remains (knock-back sphere filter, inverse-sqrt form, twice-expanded ramp block)
// Leaf: Update
// Retail-asm: 0x0078D6BC `fld dword ptr [esp+18h]` + 0x0078D6C0 `fistp dword ptr [esp+34h]` (and again at 0x0078D89D / 0x0078D8A1) -- the engine's inline-assembly ROUND helper (the tint level stored to its by-value copy and reloaded for a bare dword fistp; as the landed retail_asm 0x006472B2); VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078D250, 1729 bytes, `ret`; the tint level is a bare `fistp` (inline-asm ROUND in the original: lane gap).
// Literals: "SIMPLE_NOISE" 0x01268A5C, "OPINION_SOURCE_BERSERK" 0x0126BE18; 1.0f 0x0122DED8, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 256.0f 0x0125B240.
struct CTCBase;
struct CThing;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CHitParameters {
    unsigned char _data[0x160];
    CHitParameters(long a, CThing* attacker, long b, const C3DVector& from, const C3DVector& force, bool c, long d, long e, long f);   // 0x00789238
    ~CHitParameters();                                  // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);   // 0x00868CA0
};
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
// the engine's inline-assembly ROUND helper (landed retail_asm 0x006472B2 / 0x00643B9F / 0x00B102C0): the value is stored to
// its by-value copy and reloaded for a bare dword fistp
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
struct CTintColour { long Data[4]; };
CTintColour* __cdecl BlendTint(CTintColour* out, CRGBColour from, CRGBColour to, long level, void* key);   // 0x00A44250
struct CAbilityController {
    void SetTint(const CTintColour& tint);              // 0x004C00B0
    void SetScale(float scale);                         // 0x004BFA50
};
struct CThing {
    virtual void s000();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CAbilityController* Controller;                     // +0x64
    unsigned char _pad68[0x6c - 0x68];
    unsigned char Flags6c;                              // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void Kill(bool immediately);                        // 0x004C9B80
    void RemoveTC(long type);                           // 0x004C9840
    bool AddAction(void* action);                       // 0x006644F0
    void PlaySoundCriteria(const CCharString& name);    // 0x00662110
    void Hit(CHitParameters& hit);                      // vtable +0x104 (stand-in, non-virtual here)
    bool IsDead() const { return (m_flags & 1) != 0; }
    __forceinline bool HasTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CCreatureActionReleaseStrengthBerserk {
    unsigned char _data[0x100];
    CCreatureActionReleaseStrengthBerserk(CThing* thing);   // 0x008E68E0
    ~CCreatureActionReleaseStrengthBerserk();           // 0x0062E900
};
struct SOpinionSource { unsigned char _pad[0x77]; bool Active; };
struct COpinionReactionManager { bool AddOpinionSource(CThing* thing, const CCharString& name, SOpinionSource** out); };   // 0x00700780
struct CRadialBlur { void SetPosition(void* key, const C3DVector& position); };   // 0x006B8310
struct CEngineHolder { unsigned char _pad[0x1964]; CRadialBlur* RadialBlur; };
struct CWorld {
    unsigned char _pad00[0x18];
    CEngineHolder* Engine;                              // +0x18
    unsigned char _pad1c[0x74 - 0x1c];
    COpinionReactionManager* OpinionReactions;          // +0x74
};
struct SThingNode { SThingNode* Next; SThingNode* Prev; CThing* Thing; };
struct CThingList {
    SThingNode* Head;
    CThingList();
    ~CThingList();                                      // 0x006FE660
};
struct CRect { long Left, Top, Right, Bottom; };
struct SSphere { C3DVector Centre; float Radius; };
struct CThingSearchTools { void FindThingsInRect(const CRect& rect, const SSphere& sphere, CThingList& out); };   // 0x0078DB20
struct CTCMode3 { void SetStrength(float strength); };  // 0x0071BEE0
struct CTCMode31 { bool IsModeActive(long mode); void SetMode(long mode); };   // 0x0073A8A0 / 0x0073AE10
struct CBerserkDef {
    unsigned char _pad00[0x40];
    long MaxTimer;                                      // +0x40
    float BaseStrength;                                 // +0x44
    unsigned char _pad48[0x4c - 0x48];
    float BlurHeight;                                   // +0x4c
    unsigned char _pad50[0xbc - 0x50];
    float KnockbackRadius;                              // +0xbc
    unsigned char _padc0[0x15c - 0xc0];
    unsigned char* TintR;                               // +0x15c
    unsigned char _pad160[0x168 - 0x160];
    unsigned char* TintG;                               // +0x168
    unsigned char _pad16c[0x174 - 0x16c];
    unsigned char* TintB;                               // +0x174
};
class CTCBerserk {
public:
    void* __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    long Timer;                                         // +0x0c
    long DarkenEnvTimer;                                // +0x10
    float StrengthPercentage;                           // +0x14
    float MaxIncreaseInStrength;                        // +0x18
    float ReleasedAnimationTimer;                       // +0x1c
    long AnimTimer;                                     // +0x20
    unsigned char _pad24[0x2c - 0x24];
    float BerserkScale;                                 // +0x2c
    unsigned char _pad30[0x44 - 0x30];
    long SpellLevel;                                    // +0x44
    bool JustReleased;                                  // +0x48
    unsigned char _pad49[0x58 - 0x49];
    CBaseIntelligentPointer StartUpEffect;              // +0x58
    unsigned char _pad60[0x68 - 0x60];
    CBerserkDef* PDef;                                  // +0x68
    void* RadialBlurFadeKey;                            // +0x6c
    bool Paused;                                        // +0x70
    bool Pausable;                                      // +0x71
    bool IsInCutscene;                                  // +0x72
    CWorld* GetWorld();                                 // 0x00686A70
    CThingSearchTools* GetThingSearchTools();           // 0x00686D90
    void CleanupAbility(bool inCutscene);               // 0x0078CD80
    __forceinline void ApplyStrength(CThing* thing, float fraction)
    {
        if (thing->HasTC(3)) {
            float strength = PDef->BaseStrength + MaxIncreaseInStrength;
            if (strength > 1.0f)
                strength = 1.0f;
            thing->GetTC<CTCMode3>(3)->SetStrength(strength * fraction);
        }
        CAbilityController* controller = thing->Controller;
        if (controller) {
            float level = fraction * 256.0f;
            CTintColour tint;
            BlendTint(&tint, CRGBColour(0xff, 0xff, 0xff, 0xff),
                      CRGBColour(PDef->TintR[SpellLevel * 4], PDef->TintG[SpellLevel * 4], PDef->TintB[SpellLevel * 4], 0xff), ROUND(level), this);
            controller->SetTint(tint);
            controller->SetScale((BerserkScale - 1.0f) * fraction + 1.0f);
        }
    }
    void Update();
};

void CTCBerserk::Update()
{
    CThing* thing = ParentThing;
    if (JustReleased) {
        bool added = thing->AddAction(&CCreatureActionReleaseStrengthBerserk(thing));
        if (!added)
            return;
        JustReleased = false;
        thing->PlaySoundCriteria("SIMPLE_NOISE");
        if (StartUpEffect.GetPItem() && !StartUpEffect.GetPItem()->IsDead())
            StartUpEffect.GetPItem()->Kill(true);
        COpinionReactionManager* opinions = GetWorld()->OpinionReactions;
        SOpinionSource* source;
        bool registered = opinions->AddOpinionSource(thing, "OPINION_SOURCE_BERSERK", &source);
        if (registered)
            source->Active = true;
        if (SpellLevel >= 0) {
            float radius = PDef->KnockbackRadius;
            C3DVector centre = thing->GetPosition();
            long x = (long)centre.X;
            long y = (long)centre.Y;
            long r = (long)radius;
            CRect rect;
            rect.Left = x - r;
            rect.Top = y - r;
            rect.Right = x + r;
            rect.Bottom = y + r;
            SSphere sphere;
            sphere.Centre = centre;
            sphere.Radius = radius;
            CThingList things;
            GetThingSearchTools()->FindThingsInRect(rect, sphere, things);
            for (SThingNode* node = things.Head->Next; node != things.Head; node = node->Next) {
                CThing* other = node->Thing;
                if (other == thing)
                    continue;
                if ((other->Flags6c & 8) && !AreThingsEnemies(thing, other))
                    continue;
                const C3DVector& from = thing->GetPosition();
                const C3DVector& to = other->GetPosition();
                C3DVector dir;
                dir.X = to.X - from.X;
                dir.Y = to.Y - from.Y;
                dir.Z = to.Z - from.Z;
                float lengthSq = dir.X * dir.X + dir.Y * dir.Y + dir.Z * dir.Z;
                float inv = 1.0f / lengthSq;
                inv = (3.0f - inv * inv * lengthSq) * inv * 0.5f;
                dir.X *= inv;
                dir.Y *= inv;
                dir.Z *= inv;
                const C3DVector& pos = other->GetPosition();
                C3DVector source;
                source.X = pos.X - dir.X;
                source.Y = pos.Y - dir.Y;
                source.Z = pos.Z - dir.Z;
                CHitParameters hit(0, thing, 0, source, dir, true, 0, 0, 0);
                hit.AddDamageWithAttribute(0, 0.001f);
                other->Hit(hit);
            }
        }
        CTCMode31* mode;
        if (ParentThing->m_presentBits[1] & 0x20000)
            mode = ParentThing->GetTC<CTCMode31>(0x31);
        if (!mode->IsModeActive(0x24))
            mode->SetMode(0x24);
        return;
    }
    ++AnimTimer;
    float animTimer = (float)AnimTimer;
    if (animTimer < ReleasedAnimationTimer)
        ApplyStrength(thing, animTimer / ReleasedAnimationTimer);
    if (!(Paused && Timer >= PDef->MaxTimer)) {
        if (!IsInCutscene || Timer >= PDef->MaxTimer)
            --Timer;
    }
    C3DVector blur = thing->GetPosition();
    CRadialBlur* radial = GetWorld()->Engine->RadialBlur;
    blur.Z += PDef->BlurHeight;
    radial->SetPosition(RadialBlurFadeKey, blur);
    long timer = Timer;
    if (timer < 0 && !IsInCutscene) {
        CleanupAbility(false);
        thing->RemoveTC(0x84);
        return;
    }
    if (timer < PDef->MaxTimer)
        ApplyStrength(thing, (float)timer / PDef->MaxTimer);
}
