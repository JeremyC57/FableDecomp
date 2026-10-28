// CTCElectrocuteOnTouch::OnHit(CThing& thing) -- electrocute what touched the owner: give it a reset CTCElectrocutedEffect if it has none, then strike it with a knock-down hit carrying one point of damage attribute 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2600, length 219 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?OnHit@CTCElectrocuteOnTouch@@QAEXAAVCThing@@@Z`.
// Callees: CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) @ 0x004C9D60, CCharString(const char*, long) @
//          0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("CTCElectrocutedEffect" @ 0x0123D394), CTCElectrocutedEffect::Reset @
//          0x007CE7A0, CThing::GetPos @ 0x004C73D0 (x3), CHitParameters ctor (9 args) @ 0x00789238,
//          CHitParameters::AddDamageWithAttribute(EDamageAttribute, float) @ 0x00868CA0, ~CHitParameters @ 0x00789358, the
//          thing's virtual OnHit(CHitParameters&) (slot 65, +0x104) for a game object.
// Shape: the effect test is the raw bitset word (interface 0x8A, word 4 bit 10: `test ah,4`); the class name is a named
//        CCharString in its own block, homed in the dead `thing` argument slot; the hit direction is filled member by
//        member from two position references (owner first; a `return C3DVector(...)` operator computes Z first, a named
//        result adds a copy) and the owner is a named local read between (retail keeps it in ebx across GetPos);
//        MoveBack = false is stored before the game-object test.
// Layout: CHitParameters 0x60 bytes (landed 0x00789238 model): MoveBack +0x22; retail CThing ThingTypeFlags +0x6C (bit 2 game object).
class CThing;
class CTCBase;
class CTCParamsBase;
namespace NHitLocations { class CHitLocation; }

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    unsigned char _pad23[0x60 - 0x23];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0 (EDamageAttribute)
};

class CTCElectrocutedEffect {
public:
    void Reset();                                   // 0x007CE7A0
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                   // +0x6C
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPos() const;           // 0x004C73D0
};

class CTCElectrocuteOnTouch {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    void OnHit(CThing& thing);
};

void CTCElectrocuteOnTouch::OnHit(CThing& thing)
{
    if (!(thing.UsingComponents[0x8a >> 5] & (1UL << (0x8a & 31)))) {
        CTCElectrocutedEffect* effect;
        {
            CCharString name("CTCElectrocutedEffect", -1);
            effect = (CTCElectrocutedEffect*)thing.AddTC(name, true, 0);
        }
        effect->Reset();
    }
    const C3DVector& from = ParentThing->GetPos();
    const C3DVector& to = thing.GetPos();
    CThing* owner = ParentThing;
    C3DVector direction;
    direction.X = to.X - from.X;
    direction.Y = to.Y - from.Y;
    direction.Z = to.Z - from.Z;
    CHitParameters hit(0, owner, 0, thing.GetPos(), direction, true, false, false, false);
    hit.AddDamageWithAttribute(2, 1.0f);
    hit.MoveBack = false;
    if (thing.ThingTypeFlags & 4)
        thing.OnHit(hit);
}