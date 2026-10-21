// CTCShotDefinitionNymphFun::OnCollision(ECollideType type, const C3DVector& pos, const C3DVector& normal, CThing* other) -- on the first hit by a live shooter with the trap-spawner component (TC 0xF5), spawn its trap object at the hit point (on a live terrain-snapping thing: at that thing, raised one unit and dropped onto the ground below), make the shooter and its summons immune to the trap, trigger it, and mark the shot spent; then the base collision
// Retail: Fable.exe @ 0x007BDB66, 406 bytes (frame-pointer, `leave ; ret 0x10`; unaligned entry = /Os TU).
// Shape: /Os frame-pointer TU (`optimize("y",off)` + `("s",on)`); C3DVector has a user copy ctor and the implicit operator= (the pos copy goes through registers, the GetPosition assignment through movsd); the empty name and the shooter's team are named locals in their own block so the team is read before the Create pushes; the trap component is left uninitialised when TC 0x7D is absent.
// Callees: GetPItem 0x00A01B50, IS_THING_ALIVE 0x0040DCE0 (__fastcall), std::bitset::test 0x00410DE0, LowerBound 0x0040F020, CThing::GetPosition 0x004C73D0,
//   CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetHeightZBelowPos 0x0063E480, CCharString ctor/dtor, CThingObject::Create 0x00703210 (__fastcall),
//   CTCTrapBase::SetThingAsImmune 0x0078839A / ManualTrigger 0x00788388, MakeSummonedCreaturesImmuneToTrap 0x0080F240 (__fastcall), base OnCollision 0x007BF510
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet;
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
struct CTCBitset {
    unsigned long Words[9];
    bool test(unsigned long bit) const;                 // 0x00410DE0
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    CTCBitset TCBits;                                   // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6c;                              // +0x6c (bit 3: snaps to terrain)
    unsigned char _pad6d[0x90 - 0x6d];
    signed char Team;                                   // +0x90
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (CThing*)
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CWorldMap { float GetHeightZBelowPos(const C3DVector& pos); };   // 0x0063E480
struct CTrapSpawnDef { unsigned char _pad[0x3c]; long TrapObjectType; };   // +0x3c
struct CTCTrapSpawner { unsigned char _pad[0x0c]; CTrapSpawnDef* Def; };   // TC 0xF5
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long type, const C3DVector& pos, long team, const CRightHandedSet* rh, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
struct CTCTrapBase {
    void SetThingAsImmune(CThing* thing);               // 0x0078839A
    void ManualTrigger();                               // 0x00788388
};
void __fastcall MakeSummonedCreaturesImmuneToTrap(CTCTrapBase* trap);   // 0x0080F240
struct CTCShotDefinitionBase {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CBaseIntelligentPointer PShooter;                   // +0x28
    CWorldMap* GetWorldMap();                           // 0x00686D30
    void OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other);   // 0x007BF510
};
struct CTCShotDefinitionNymphFun : CTCShotDefinitionBase {
    unsigned char _pad30[0xa4 - 0x30];
    bool Spent;                                         // +0xa4
    void OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other);
};

void CTCShotDefinitionNymphFun::OnCollision(long type, const C3DVector& pos, const C3DVector& normal, CThing* other)
{
    if (!Spent) {
        if (IS_THING_ALIVE(PShooter.GetPItem())) {
            CThing* shooter = PShooter.GetPItem();
            if (shooter->TCBits.test(0xf5)) {
                long trapType = ((CTCTrapSpawner*)shooter->GetTC(0xf5))->Def->TrapObjectType;
                if (trapType > 0) {
                    C3DVector at = pos;
                    if (IS_THING_ALIVE(other) && (other->Flags6c & 8)) {
                        at = other->GetPosition();
                        at.Z += 1.0f;
                        at.Z = GetWorldMap()->GetHeightZBelowPos(at);
                    }
                    CThingObject* trapThing;
                    {
                        CCharString name("");
                        long team = PShooter.GetPItem()->Team;
                        trapThing = CThingObject::Create(trapType, at, team, 0, 0, name);
                    }
                    CTCTrapBase* trap;
                    if (trapThing->TCBits.test(0x7d))
                        trap = (CTCTrapBase*)trapThing->GetTC(0x7d);
                    trap->SetThingAsImmune(PShooter.GetPItem());
                    trap->ManualTrigger();
                    MakeSummonedCreaturesImmuneToTrap(trap);
                    Spent = true;
                }
            }
        }
    }
    CTCShotDefinitionBase::OnCollision(type, pos, normal, other);
}