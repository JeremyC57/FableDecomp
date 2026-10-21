// CAIStateGroup_JackDragon::CreateIBC() -- create the invisible bodge creature at the dragon's mouth dummy (at the dragon's height, pushed out along its facing), strip its TC 0x10, hide it on the minimap and keep it as PInvisibleBodgeCreature
// Grade: DIFFER(476v482) -- retail copies the GetFacing return temporary (read through eax: x, y into x87, z stored back to the same slot) into the sum's left operand at the same frame slot; ours builds the call result straight into the operand (no copy, 6 bytes short), and that shifts the init record's two zero stores past the first push; frame, slots and calls otherwise line up
// Shape: names in blocks (def name, mouth dummy, the two "" strings), the dummy position written into a copy of the creature's position
// with Z restored from GetPosition, spawn = GetFacing() + pos with a by-value left operand (x-first sum), the init record a
// temporary of Create (landed 0x00800540 model), PInvisibleBodgeCreature assigned from a CIntelligentPointer temporary.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FC00, length 482 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0
// ("CREATURE_INVISIBLE_BODGE" @ 0x01293374, "DRAGON_MOUTH_DUMMY" @ 0x01285B4C, "" @ 0x0122D70E), CGameDefinitionManager::Get
// @ 0x0044C6B0 / GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPosition @ 0x004C73D0 (twice),
// CTCGraphicAppearance::GetDummyObject @ 0x004C08C0 (on the creature's +0x64), the physics body's virtual GetFacing (slot 70,
// +0x118, on +0x60), CThingAICreature::Create @ 0x00833800 (__fastcall), CThing::RemoveTC @ 0x004C9840,
// CThingCreatureBase::SetHiddenOnMiniMap @ 0x006626A0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
// GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0).
// Layout: CAIStateGroup_JackDragon from Ego_r.pdb (PInvisibleBodgeCreature +0x44); CThingAICreatureInit and CRightHandedSet
// from Ego_r.pdb; CThing PlayerNumber +0x90 (retail; PDB +0xa0), PTCGraphicAppearance +0x64, physics body +0x60.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
inline C3DVector operator+(C3DVector a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThingPhysical;
struct CThingBuilding;
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                  // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;  // +0x64
    unsigned char _pad68[0x90 - 0x68];
    signed char PlayerNumber;                   // +0x90
    unsigned char m_flags91;                    // +0x91 bit 0: Dead
    const C3DVector& GetPosition();             // 0x004C73D0
    void RemoveTC(long interfaceType);          // 0x004C9840
};
struct CThingCreatureBase : CThing {
    void SetHiddenOnMiniMap(bool hidden);       // 0x006626A0
};
struct CThingAICreatureInit {
    CThingPhysical* OriginalThing;              // +0x00
    CThingPhysical* PreviousThing;              // +0x04
    CCharString     ScriptName;                 // +0x08
    CCharString     OverridingBrainName;        // +0x0c
    bool            PlayerCharacter;            // +0x10
    bool            Drawable;                   // +0x11
    bool            PlaceAtEndOfList;           // +0x12
    bool            EnableCreatureAutoPlacing;  // +0x13
    CThingBuilding* HomeBuilding;               // +0x14
    CThingBuilding* WorkBuilding;               // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
struct CThingAICreature : CThingCreatureBase {
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long flags, const CThingAICreatureInit& init);   // 0x00833800
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // <CThingAICreature>: vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    unsigned char _pad14[0x44 - 0x14];          // PEnemy, the five markers
    CIntelligentPointer<CThingAICreature> PInvisibleBodgeCreature;   // +0x44
    void CreateIBC();
};

void CAIStateGroup_JackDragon::CreateIBC()
{
    long defIndex;
    {
        CCharString name("CREATURE_INVISIBLE_BODGE");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    C3DVector pos = GetCreature()->GetPosition();
    CRightHandedSet orientation;
    {
        CCharString dummy("DRAGON_MOUTH_DUMMY");
        GetCreature()->PTCGraphicAppearance->GetDummyObject(dummy, 0, 0, &pos, &orientation, 1.0f);
    }
    pos.Z = GetCreature()->GetPosition().Z;
    C3DVector spawnPos = GetCreature()->PhysicsBody->GetFacing() + pos;
    CThingAICreature* ibc;
    {
        CCharString brain("");
        CCharString script("");
        ibc = CThingAICreature::Create(defIndex, spawnPos, GetCreature()->PlayerNumber,
            CThingAICreatureInit(0, 0, false, script, brain, true));
    }
    if (ibc != 0 && !(ibc->m_flags91 & 1)) {
        ibc->RemoveTC(0x10);
        ibc->SetHiddenOnMiniMap(true);
        PInvisibleBodgeCreature = CIntelligentPointer<CThingAICreature>(ibc);
    }
}
