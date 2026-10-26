// CCombatAction_GenericStrikeResponseBase::FrameUpdate() -- base frame update; on the dust frame (when combat particles apply and CreateDustEffect is set) spawn the dust effect along DustTravelDirection; then, when the base's finish-when-moving flag is set and the creature (type 2) moves faster than 0.1, finish the action
// Leaf: ?FrameUpdate@CCombatAction_GenericStrikeResponseBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00865C20, 208 bytes incl. padding (plain `ret`, the finish call is a tail `jmp`).
// Callees: CCombatActionBase::FrameUpdate @ 0x00859D80, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          IsToCreateCombatParticleEffects(CThing*, CThing*) @ 0x008F7860 (__fastcall), CreateDustEffectForCreature(
//          CThingCreatureBase*, CWorldMap*, CGameDefinitionManager*, C3DVector*, vector<CThing*>*) @ 0x008F7950 (__fastcall),
//          CCreatureActionBase::GetDefinitionManager @ 0x00692910 / GetWorldMap @ 0x00692C10 (manifest labels of 0x00692910
//          propagated), virtual slot 4 (+0x10, FinishAction). Float 0.1f @ 0x0122EE58.
// Layout (Ego_r.pdb names, retail offsets = PDB - 0x18): PCreature +0x08, current frame +0x1C, PTarget +0xA8, base flag +0xDC,
//          CreateDustEffect +0x135, FrameForDust +0x138, DustTravelDirection +0x13C; CThing type byte +0x10, creature velocity +0x1C0.
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char ThingType;                    // +0x10
};
class C3DVector { public:
    float X, Y, Z;
    float GetLength() const;
};
#include <math.h>
inline float C3DVector::GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
class CThingCreatureBase : public CThing { public:
    unsigned char _pad11[0x1c0 - 0x11];
    C3DVector     Velocity;                     // +0x1C0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CWorldMap;
class CGameDefinitionManager;
struct CThingVector;
bool __fastcall IsToCreateCombatParticleEffects(CThing* creature, CThing* target);   // 0x008F7860
void __fastcall CreateDustEffectForCreature(CThing* creature, CWorldMap* map, CGameDefinitionManager* defs,
                                            C3DVector* direction, CThingVector* ignore);   // 0x008F7950

class CCombatActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                // slot 4 (+0x10)
    virtual void FrameUpdate();                 // 0x00859D80
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x0c];
    long                    CurrentFrame;       // +0x1C
    unsigned char           _pad20[0xa8 - 0x20];
    CIntelligentPointer<CThing> PTarget;            // +0xA8
    unsigned char           _padb0[0xdc - 0xb0];
    bool                    FinishWhenMoving;   // +0xDC
    unsigned char           _paddd[0x114 - 0xdd];   // retail sizeof(CCombatActionBase) = 0x114
    CGameDefinitionManager* GetDefinitionManager();   // 0x00692910
    CWorldMap* GetWorldMap();                   // 0x00692C10
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
public:
    unsigned char _pad114[0x134 - 0x114];       // PThingThatHit +0x114, PointOfHit +0x11C, DirectionOfHit +0x128
    bool          MoveOnHitOverride;            // +0x134
    bool          CreateDustEffect;             // +0x135
    long          FrameForDust;                 // +0x138
    C3DVector     DustTravelDirection;          // +0x13C
    virtual void FrameUpdate();
};

void CCombatAction_GenericStrikeResponseBase::FrameUpdate()
{
    CCombatActionBase::FrameUpdate();
    if (CurrentFrame == FrameForDust) {
        CThing* creature = PCreature.GetPItem();
        if (IsToCreateCombatParticleEffects(creature, PTarget.GetPItem()) && CreateDustEffect) {
            CThing* dustCreature = PCreature.GetPItem();
            CreateDustEffectForCreature(dustCreature, GetWorldMap(), GetDefinitionManager(), &DustTravelDirection, 0);
        }
    }
    if (FinishWhenMoving) {
        CThing* creature = PCreature.GetPItem();
        if (creature->ThingType == 2 && ((CThingCreatureBase*)creature)->Velocity.GetLength() > 0.1f)
            FinishAction();
    }
}