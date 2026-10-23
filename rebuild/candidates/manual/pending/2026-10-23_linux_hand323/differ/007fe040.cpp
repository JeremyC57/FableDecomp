// CTCAISummon::SummonCreatures(const C3DVector* centre) -- spawn the number of creatures the summoner may still create, spread round a circle (the def's offset rotated by 1/count of a turn each step) about the given centre or the summoner: each spot must be on walkable, unblocked ground (and pass the summoner's spawn-region check); pick the def's creature (or a random one of its group), create it facing outwards, queue its appear action, track it in the summoner's and the global summoned lists and play the def's spawn effect; returns the number created
// Grade: DIFFER(1281v1250) -- first draft: retail frame sub esp,0x1bc vs ours 0x1cc (8-aligned frame), local slot order and block layout differ
// Leaf: SummonCreatures
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FE040, 1250 bytes, `ret 4`, 8-aligned frame.
// Literals: "" 0x0122D70E, "SUMMONED_CREATURE" 0x0127214C; 1.0f 0x0122DED8, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0x0122DEE0.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DCoordI { long X, Y; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct SCreateParams {
    CCharString Name;
    CCharString Script;
    long A, B;
    bool Persistent, Visible, Active, Global;
    long C, D;
    SCreateParams(const CCharString& name, const CCharString& script) : Name(name), Script(script), A(0), B(0),
        Persistent(false), Visible(true), Active(true), Global(false), C(0), D(0) { }
};
struct SOrientation { C3DVector Up; C3DVector Forward; };
struct CThingController {
    virtual void s000(); virtual void s004(); virtual void s008(); virtual void s00c(); virtual void s010(); virtual void s014(); virtual void s018(); virtual void s01c();
    virtual void s020(); virtual void s024(); virtual void s028(); virtual void s02c(); virtual void s030(); virtual void s034(); virtual void s038(); virtual void s03c();
    virtual void s040(); virtual void s044(); virtual void s048(); virtual void s04c(); virtual void s050(); virtual void s054(); virtual void s058(); virtual void s05c();
    virtual void s060(); virtual void s064(); virtual void s068(); virtual void s06c(); virtual void s070(); virtual void s074(); virtual void s078(); virtual void s07c();
    virtual void s080(); virtual void s084(); virtual void s088(); virtual void s08c(); virtual void s090(); virtual void s094(); virtual void s098(); virtual void s09c();
    virtual void s0a0(); virtual void s0a4(); virtual void s0a8(); virtual void s0ac(); virtual void s0b0(); virtual void s0b4(); virtual void s0b8(); virtual void s0bc();
    virtual void s0c0(); virtual void s0c4(); virtual void s0c8(); virtual void s0cc(); virtual void s0d0(); virtual void s0d4(); virtual void s0d8(); virtual void s0dc();
    virtual void s0e0(); virtual void s0e4(); virtual void s0e8(); virtual void s0ec(); virtual void s0f0(); virtual void s0f4(); virtual void s0f8(); virtual void s0fc();
    virtual void s100(); virtual void s104(); virtual void s108(); virtual void s10c(); virtual void s110(); virtual void s114(); virtual void s118(); virtual void s11c();
    virtual void s120(); virtual void s124();
    virtual void SetOrientation(const SOrientation& orientation);   // +0x128
};
struct CTCBase;
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last; SEntry* LowerBound(const long* type); } m_interfaces;   // +0x44 (0x004365B0)
    unsigned char _pad4c[0x60 - 0x4c];
    CThingController* Controller;                       // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    bool AddAction(void* action);                       // 0x006644F0
    bool IsDead() const { return (m_flags & 1) != 0; }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) { SetPItem(item); } };
struct CThingPtrList { void push_back(const CBaseIntelligentPointer& p); };   // 0x004BA4E0
extern CThingPtrList g_SummonedCreatures;               // 0x013BAF14
struct CNavigationMap { bool IsWalkable(); bool IsBlocked(); };   // 0x004FC5B0 / 0x004FCBD0
struct CFrameTimer { long GetFrame(); };                // 0x0049D810
struct CWorld {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1c();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2c();
    virtual CNavigationMap* GetNavAt(const C2DCoordI& position);   // +0x30
};
struct CTCSpawnRegion { bool CanSpawnAt(const C3DVector& position, const C3DVector& from, long flags); };   // 0x006CB620
struct CDefBase { virtual void v00(); virtual void Release(); long RefCount; };
struct CCreatureGroupDef : CDefBase { unsigned char _pad08[0x3c - 0x08]; long* MembersFirst; long* MembersLast; };
struct CCreatureDef : CDefBase { unsigned char _pad08[0x11c - 0x08]; long Script; };
struct CDefLoader { void LoadGroup(CCreatureGroupDef** out); void LoadCreature(CCreatureDef** out); };   // 0x0074D220 / 0x00513160
CDefLoader* GetDefLoader(long index, void* out);        // 0x0044C6B0
long __fastcall RandomBelow(long count, long seed);     // 0x004CDAE0
CThing* __fastcall CreateCreature(long index, const C3DVector& position, long script, SCreateParams* params);   // 0x00833800
struct CActionAppear {
    unsigned char _data[0x100];
    CActionAppear(CThing* creature);                    // 0x0074DBB0
    ~CActionAppear();                                   // 0x0062E900
};
void* __fastcall CreateParticleEffect(long effect, const C3DVector& position, long flags);   // 0x006E0880
struct CRotation2D { void Rotate(float fraction); };    // 0x00A14040
extern const float g_SummonFacingScale;                 // 0x0122DEE0
struct CSummonDef {
    unsigned char _pad00[0x28];
    long CreatureIndex;                                 // +0x28
    long GroupIndex;                                    // +0x2c
    unsigned char _pad30[0x34 - 0x30];
    C3DVector Offset;                                   // +0x34
    unsigned char _pad40[0x48 - 0x40];
    long SpawnEffect;                                   // +0x48
};
class CTCAISummon {
public:
    void* __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CSummonDef* PDef;                                   // +0x0c
    CThingPtrList Summoned;                             // +0x10
    unsigned long LastSummonFrame;                      // +0x14
    long GetNumberToSummon();                           // 0x007FDF60
    CWorld* GetWorld();                                 // 0x00686A70
    long SummonCreatures(const C3DVector* centre);
};
unsigned long GetFrameNumber();                         // 0x0049D870

long CTCAISummon::SummonCreatures(const C3DVector* centre)
{
    long count = GetNumberToSummon();
    long created = 0;
    if (count <= 0)
        return created;
    float turn = 1.0f / (float)count;
    long effect = PDef->SpawnEffect;
    C3DVector offset = PDef->Offset;
    offset.Y = 0.0f;
    offset.Z = 0.0f;
    if (!centre)
        centre = &ParentThing->GetPosition();
    C3DVector origin = *centre;
    CTCSpawnRegion* region = 0;
    CThing* parent = ParentThing;
    if ((signed char)((unsigned char*)parent->m_presentBits)[0] < 0) {
        long key = 7;
        CThing::CInterfaceMap* map = &parent->m_interfaces;
        CThing::CInterfaceMap::SEntry* it = map->LowerBound(&key);
        region = (CTCSpawnRegion*)((it == map->last || 7 < it->type) ? map->last : it)->component;
    }
    for (long i = count; i != 0; --i) {
        C3DVector spot(origin.X + offset.X, origin.Y + offset.Y, origin.Z + offset.Z);
        C2DCoordI cell;
        cell.X = (long)spot.X;
        cell.Y = (long)spot.Y;
        if (GetWorld()->GetNavAt(cell)->IsWalkable() && GetWorld()->GetNavAt(cell)->IsBlocked() &&
            (!region || region->CanSpawnAt(origin, spot, 0))) {
            SCreateParams params("", "");
            params.Name = "SUMMONED_CREATURE";
            long creatureIndex = PDef->CreatureIndex;
            long group = PDef->GroupIndex;
            if (group > 0) {
                CCreatureGroupDef* groupDef = 0;
                GetDefLoader(group, &groupDef)->LoadGroup(&groupDef);
                long members = groupDef->MembersLast - groupDef->MembersFirst;
                if (members)
                    creatureIndex = groupDef->MembersFirst[RandomBelow(members, GetWorld() ? 0 : 0)];
                if (groupDef && --groupDef->RefCount == 0)
                    groupDef->Release();
            }
            CCreatureDef* def = 0;
            GetDefLoader(creatureIndex, &def)->LoadCreature(&def);
            CThing* creature = CreateCreature(creatureIndex, spot, def->Script, &params);
            if (creature && !creature->IsDead()) {
                C3DVector facing = offset;
                if (centre)
                    facing = C3DVector(offset.X * g_SummonFacingScale, offset.Y * g_SummonFacingScale, offset.Z * g_SummonFacingScale);
                float lengthSq = facing.X * facing.X + facing.Y * facing.Y + facing.Z * facing.Z;
                float inv = 1.0f / lengthSq;
                inv = (3.0f - inv * inv * lengthSq) * inv * 0.5f;
                SOrientation orientation;
                orientation.Up = C3DVector(0.0f, 0.0f, 1.0f);
                orientation.Forward = C3DVector(facing.X * inv, facing.Y * inv, facing.Z * inv);
                creature->Controller->SetOrientation(orientation);
                creature->AddAction(&CActionAppear(creature));
                Summoned.push_back(CIntelligentPointer(creature));
                g_SummonedCreatures.push_back(CIntelligentPointer(creature));
                ++created;
                if (effect > 0)
                    CreateParticleEffect(effect, spot, 0);
            }
            if (--def->RefCount == 0)
                def->Release();
        }
        ((CRotation2D*)&offset)->Rotate(turn);
    }
    LastSummonFrame = GetFrameNumber();
    return created;
}
