// CThingFilter_CombatIsStrikeToStrike::operator()(CThing* thing) const -- a non-creature passes when the striker is of type 2 or its definition's strike flag is set; a creature that evades all hits or is rolling fails, a creature whose definition faces strikes fails when it faces away from the striker, otherwise it passes when vulnerable to the striker's melee strikes
// Grade: DIFFER(383v406) -- control flow and the facing/delta x87 math are right; differs in the def pointer: retail copies the thing's def through a by-value temporary and an assignment (three separate count stores +1,+1,-1 with the def kept in ebx from a zeroed start, add-ref load/inc/store, RMW `dec` releases) and the non-creature arm add-refs with `inc dword ptr` then releases with load/dec/store; ours inlines a different mix (operator= called out of line here), plus frame-slot and register differences around the striker (retail spills `this` to [esp+0xc])
// Leaf: ??RCThingFilter_CombatIsStrikeToStrike
// Callees: IsToEvadeAllHits @ 0x0085FA00 (__fastcall), IsRolling @ 0x0085EEB0 (__fastcall), CThing::GetPosition() const
// @ 0x004C73D0, the striker physics component's slot 70 (+0x118, facing by hidden pointer), IsThingVulnerableToMeleeStrikesFromThing
// @ 0x007F59F0 (__fastcall), the def's virtual slot 1 (+4) at count zero.
// Layout: CThingFilter_CombatIsStrikeToStrike {CThing* Striker} (descriptive); retail CThing type byte +0x10, PhysicsTC +0x60,
// ThingTypeFlags +0x6c (bit 3 creature), PDef +0x70; the def's count +4, flag byte +0x42 bit 7, flag byte +0x138 bit 0.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

struct CThingDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x42 - 0x08];
    unsigned char Flags42;                      // +0x42
    unsigned char _pad43[0x138 - 0x43];
    unsigned char Flags138;                     // +0x138
};
struct CDefPointer {
    CThingDef* Object;
    CDefPointer(CThingDef* object) : Object(object)
    {
        if (Object)
            Object->RefCount = Object->RefCount + 1;
    }
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->RefCount = Object->RefCount + 1;
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (other.Object)
            other.Object->RefCount = other.Object->RefCount + 1;
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
        Object = other.Object;
        return *this;
    }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    CThingDef* operator->() const { return Object; }
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                    // +0x10
    unsigned char _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;               // +0x6c
    CThingDef* PDef;                            // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CDefPointer GetPDef() const { return CDefPointer(PDef); }
};
bool __fastcall IsToEvadeAllHits(CThing* thing);     // 0x0085FA00
bool __fastcall IsRolling(CThing* thing);            // 0x0085EEB0
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(CThing* thing, CThing* striker);   // 0x007F59F0

struct CThingFilter_CombatIsStrikeToStrike {
    CThing* Striker;                            // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_CombatIsStrikeToStrike::operator()(CThing* thing) const
{
    if (thing->ThingTypeFlags & 8) {
        if (IsToEvadeAllHits(thing))
            return false;
        if (IsRolling(thing))
            return false;
        CDefPointer def;
        def = thing->GetPDef();
        if (def.Object && (def->Flags138 & 1)) {
            const C3DVector& position = thing->GetPosition();
            C3DVector delta = position - Striker->GetPosition();
            C3DVector facing = Striker->PhysicsTC->GetFacing();
            if (facing.Z * delta.Z + facing.Y * delta.Y + delta.X * facing.X < 0.0f)
                return false;
        }
        if (!(thing->ThingTypeFlags & 8))
            return true;
        return IsThingVulnerableToMeleeStrikesFromThing(thing, Striker);
    }
    if (Striker->ThingType == 2)
        return true;
    CDefPointer def(thing->PDef);
    return (def->Flags42 >> 7) != 0;
}
