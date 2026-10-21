// CCreatureAction_SnapDragon::PerformAction() -- find the dragon's mouth dummy; at that point (at the dragon's height) create
// an obstruction (radius 1.5) whose driver gets 1.5 and ignores the dragon; then create THING_EXPLOSION_JACK_DRAGON_BITE
// 2.5 units behind the mouth along the dragon's facing, looking at the dragon, and make the dragon its owner
// Leaf: ?PerformAction@CCreatureAction_SnapDragon
// Layout: CThing interface bitset +0x20, PTCDriver +0x5c (the cached driver component), physics +0x60, graphic appearance
//   +0x64; the 0xED driver's ignore list (vector<CIntelligentPointer<CThing>>) at +0x2c.
// Shape: the dummy lookup goes through an inline CThing wrapper (so the name temporary and GetPItem run before the six
//   pushes); operator* / operator- are the constructor forms.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCBase;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    unsigned char  _pad44[0x5c - 0x44];
    CTCBase*       PTCDriver;                         // +0x5c (the cached driver component)
    CPhysicsBody*  PhysicsBody;                       // +0x60
    CTCGraphicAppearance* GraphicAppearance;          // +0x64
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    {
        return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
    }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                        // 0x00A01B10
    ~CBaseIntelligentPointer();                       // 0x00A01C10
    CThing* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CIntelligentPointerVector {                    // STLport vector<CIntelligentPointer<CThing>>, 12 bytes in retail
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    void push_back(const CIntelligentPointerThing& value);   // 0x004AD520
};
struct CTCDriverED {                                  // driver of interface 0xED (the obstruction)
    unsigned char _pad00[0x2c];
    CIntelligentPointerVector IgnoredThings;          // +0x2c
    void SetValue(float value);                       // 0x007FD3D0
};
struct CTCDriver41 {                                  // driver of interface 0x41
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
CThing* __fastcall CreateObstruction(const C3DVector* a, const C3DVector* b, float radius);   // 0x007FD4B0
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position, const C3DVector* lookAt);   // 0x0080C500
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get();             // 0x0044C6B0
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_SnapDragon : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_SnapDragon::PerformAction()
{
    C3DVector mouth = GetCreature()->GetPosition();
    CRightHandedSet orientation;
    GetCreature()->GetDummyObject("DRAGON_MOUTH_DUMMY", &mouth, &orientation);
    mouth.Z = GetCreature()->GetPosition().Z;
    CThing* obstruction = CreateObstruction(&mouth, &GetCreature()->GetPosition(), 1.5f);
    if (obstruction) {
        CTCDriverED* driver = 0;
        if (obstruction->HasTC(0xed))
            driver = (CTCDriverED*)obstruction->PTCDriver;
        driver->SetValue(1.5f);
        driver->IgnoredThings.push_back(CIntelligentPointerThing(GetCreature()));
    }
    long explosion = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("THING_EXPLOSION_JACK_DRAGON_BITE");
    C3DVector position = mouth - GetCreature()->GetPhysics()->GetFacing() * 2.5f;
    CThing* bite = CreateThing(explosion, &position, &GetCreature()->GetPosition());
    if (bite) {
        CTCDriver41* driver;
        if (bite->HasTC(0x41))
            driver = (CTCDriver41*)bite->PTCDriver;
        driver->SetOwner(GetCreature());
    }
}