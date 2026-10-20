// CThingObject::OnDie() -- vtable slot 11: when the object def names an on-death object, create it at this object's position and give it a small random spin, then run CThingPhysical::OnDie
// Leaf: ?OnDie@CThingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007038D0, length 199 bytes, `ret`; slot 11 of vtable 0x01264DCC =
//   CThingObject (rtti-vtables.tsv); CThingPhysical::OnDie is slot 11 of 0x01244E84 (0x006A4CF0).
// Callees: the game random seed getter @ 0x004C7AE0 (`[[0x013B8A1C]+0x2C]`; manifest "CViewBase::GetDisplayEngine" is a
//   byte twin), GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0 (`__fastcall`: the float on the stack, the
//   seed in ecx), CThing::GetPosition @ 0x006A4D00, CThingObject::Create @ 0x00703210 (`__fastcall`; declared as the
//   landed callers spell it, CreateObject), the created object's spin setter @ 0x006A4DC0 (`const C3DVector*`; the
//   manifest's _Cons_val label is propagated, name unproven), CThingPhysical::OnDie @ 0x006A4CF0,
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Float pool: 0x0122EE58 = 0.1f, 0x012305A0 = 0.2f; the random ranges 0.2f / 0.3f are pushed as immediates; literal
//   0x0122D70E = "".
// Layout: retail CThingObject PDef +0xd0; retail CThingObjectDef OnDeathObject +0x8c (PDB +0x90).
// Shape: the spin is a frame C3DVector filled component by component (X, Y, Z in order); the "" script name is an
//   implicit conversion temporary built before the other arguments; the base OnDie is a qualified call at the end.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CThingObject;
CThingObject* __fastcall CreateObject(long definition, C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags,
                                      const CCharString& name);  // 0x00703210

struct CThingObjectDef {
    unsigned char _pad00[0x8c];
    long OnDeathObject;                         // +0x8c
};

class CThing {
public:
    unsigned long* GetRandomSeed_004c7ae0();    // 0x004C7AE0
    const C3DVector& GetPosition() const;       // 0x006A4D00
    void SetSpin_006a4dc0(const C3DVector* spin);   // 0x006A4DC0
};

class CThingPhysical : public CThing {
public:
    void OnDie();                               // 0x006A4CF0 (slot 11)
};

class CThingObject : public CThingPhysical {
public:
    unsigned char _pad00[0xd0];
    const CThingObjectDef* PDef;                // +0xd0 CDefPointer<CThingObjectDef const>::Object
    void OnDie();
};

void CThingObject::OnDie()
{
    const CThingObjectDef* def = PDef;
    if (def->OnDeathObject) {
        C3DVector spin;
        spin.X = 0.1f - GFFloatRandom(0.2f, GetRandomSeed_004c7ae0());
        spin.Y = 0.1f - GFFloatRandom(0.2f, GetRandomSeed_004c7ae0());
        spin.Z = GFFloatRandom(0.3f, GetRandomSeed_004c7ae0()) + 0.2f;
        CThingObject* created = CreateObject(def->OnDeathObject, const_cast<C3DVector*>(&GetPosition()), -1, 0, 0, "");
        if (created)
            created->SetSpin_006a4dc0(&spin);
    }
    CThingPhysical::OnDie();
}
