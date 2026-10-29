// CThingAICreature::UpdateAITurning(const C3DVector& movement) -- pick the creature's look target from its AI turn state (a fixed position, a live target thing's head helper / helper point / position, or its own position plus the movement when that is long enough) and hand it to the look component (TC 0x43)
// Leaf: UpdateAITurning
// Callees: GetPItem 0xA01B50, 0x661E80 (byte-identical to the landed CText::GetAlignment; here a CThingAICreature flag query), CTCGraphicAppearance 0x4C10E0 / 0x4C1060(1.0f) (by-value C3DVector),
//   CThing::GetPos 0x4C73D0, LowerBound 0x40F020, CTCLook::LookAtPos 0x753E90.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) { Z = o.Z; Y = o.Y; X = o.X; }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
__forceinline float GetLengthSquared(C3DVector v) { return v.Z * v.Z + v.Y * v.Y + v.X * v.X; }
class CBaseObject;
struct CBaseIntelligentPointer {
    unsigned char _pad00[0x08];
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
struct CTCGraphicAppearance {
    C3DVector GetHeadPosition() const;                      // 0x004C10E0
    C3DVector GetHelperPointOrCentre(float fraction) const; // 0x004C1060
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    unsigned char _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                                 // +0x91
    const C3DVector& GetPos() const;                   // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
};
class CTCLook { public:
    void LookAtPos(const C3DVector& position, long speed);  // 0x00753E90
};
struct CAITurnState {
    unsigned long Flags;                                    // +0x00
    bool IsFacingPosition() const { return (unsigned char)(Flags >> 1) & 1; }
    bool IsFacingThing() const { return (unsigned char)(Flags >> 2) & 1; }
    bool IsFacingMovement() const { return (unsigned char)(Flags >> 4) & 1; }
    CThing* GetThing() const { return (CThing*)Thing.GetPItem(); }
    bool IsThingValid() const { CThing* thing = GetThing(); return thing && !thing->Dead; }
    long Speed;                                             // +0x04
    unsigned char _pad08[0x34 - 0x08];
    C3DVector Position;                                     // +0x34
    CBaseIntelligentPointer Thing;                          // +0x40
};
class CThingCreatureBase : public CThing { public:
    bool IsInMelee() const;                                 // 0x00661E80 (Ego_r.pdb)
};
class CThingAICreature : public CThingCreatureBase { public:
    unsigned char _pad94[0x1a0 - 0x94];
    CAITurnState* PTurnState;                               // +0x1a0
    void UpdateAITurning(const C3DVector& movement);
};

void CThingAICreature::UpdateAITurning(const C3DVector& movement)
{
    C3DVector target;
    if (PTurnState->IsFacingPosition()) {
        target = PTurnState->Position;
    } else if (PTurnState->IsFacingThing() && PTurnState->IsThingValid()) {
        if (PTurnState->GetThing()->PGraphicAppearance) {
            if (IsInMelee()) {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHeadPosition();
            } else {
                CTCGraphicAppearance* app = PTurnState->GetThing()->PGraphicAppearance;
                target = app->GetHelperPointOrCentre(1.0f);
            }
        } else {
            target = PTurnState->GetThing()->GetPos();
        }
    } else if (PTurnState->IsFacingMovement() && GetLengthSquared(movement) > 0.0001f) {
        target = GetPos() + movement;
    } else {
        return;
    }
    CTCLook* look;
    if (HasTC(0x43))
        look = (CTCLook*)GetTC(0x43);
    look->LookAtPos(target, PTurnState->Speed);
}