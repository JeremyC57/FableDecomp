// Grade: DIFFER(381v383) -- the parent creature is loaded straight into ecx for the combat test (retail: eax, then `mov ecx,eax`, with the max-turn-speed store scheduled after the null test); everything else matches
// CTCLook::ImplementBodySettings() -- turn the body towards BodyAngleXY: the step per frame is the def's max turn speed (the combat one while the parent creature is in combat) in degrees, clamped difference added to the current facing and wrapped to [0, 1); skipped for a creature thing while the main player's live, flagged creature is moving, and for an incapacitated thing
// Leaf: ImplementBodySettings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752300, 383 bytes, plain `ret`. Speed TU (esp frame, no pragma).
//   Ego_r.pdb: `?ImplementBodySettings@CTCLook@@AAEXXZ`.
// Callees: the creature's combat test @ 0x00661E80 (label "NUISystem::CText::GetAlignment" is propagated: a two-bit test
//   of +0x15C), the thing's +0x60 object's virtual slots 63 (+0xFC, current facing) and 66 (+0x108, set facing),
//   AngleDifference(float, float) @ 0x00A441E0 (`__stdcall`), _CIfmod @ 0x00BFEB28 (fmod intrinsic), CTCBase player
//   manager getter @ 0x00686D40 (label "CViewBase::GetDisplayEngine" is propagated), CPlayerManager::GetMainPlayer
//   @ 0x00449970, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, NCreatureMode::IsIncapacitated(const CThing&)
//   @ 0x0085F0C0 (`__fastcall`).
// Constants: 1/360 0x01238E00, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00, 1e-4f 0x0129BA3C.
// Shape: the step is spelled inline twice in the Clamp call (one CSE'd value: retail's frame-slot layout); the main
//   player is a named local (`lea ecx,[eax+34h]`); the step clamp is a const-reference Clamp (its result address picked by `lea eax`), the facing wrap as the
//   landed body twin 0x00751970.
// Layout: Ego_r.pdb CTCLook (matches retail to here): ParentThing +0x04, PParentCreature +0x10, PDef +0x14,
//   BodyAngleXY +0x24; CLookDef MaxTurnSpeed +0x3C, CombatMaxTurnSpeed +0x48; CThing type byte +0x10, +0x60 object
//   (positions +0x0C / +0x18), dead flag bit 0 of +0x91, +0x178 bit 0; CPlayer +0x34 CIntelligentPointer<CThing>.
#include <math.h>
#pragma intrinsic(fmod)
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0
extern float g_epsilon_0129ba3c;                     // 0x0129BA3C (1e-4f)

template <class T> inline const T& Clamp(const T& value, const T& low, const T& high)
{
    return value < low ? low : (value > high ? high : value);
}

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredLength() const { return X * X + Y * Y + Z * Z; }
};
struct CThingPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); 
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); 
    virtual float GetAngleXY();                      // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetAngleXY(float angle);            // slot 66 (+0x108)
    C3DVector Position;                              // +0x0C (after the vptr and two words)
    C3DVector PreviousPosition;                      // +0x18
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char ThingType;                         // +0x10
    unsigned char _pad11[0x60 - 0x11];
    CThingPlacement* PPlacement;                     // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                           // +0x91 (bit 0: dead)
    unsigned char _pad92[0x178 - 0x92];
    unsigned char Flags178;                          // +0x178
};
namespace NCreatureMode { bool __fastcall IsIncapacitated(const CThing& thing); }   // 0x0085F0C0
struct CThingCreatureBase : CThing {
    bool IsInCombat() const;                         // 0x00661E80
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer PCharacter;              // +0x34
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                        // 0x00449970
};
struct CLookDef {
    unsigned char _pad00[0x3c];
    float MaxTurnSpeed;                              // +0x3C
    unsigned char _pad40[0x48 - 0x40];
    float CombatMaxTurnSpeed;                        // +0x48
};

struct CTCLook {
    void*               __vftable;                   // +0x00
    CThing*             ParentThing;                 // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;             // +0x10
    const CLookDef*     PDef;                        // +0x14 CDefPointer<const CLookDef>
    unsigned char       _pad18[0x24 - 0x18];
    float               BodyAngleXY;                 // +0x24
    CPlayerManager* GetPlayerManager();              // 0x00686D40 (CTCBase)
    void ImplementBodySettings();
};

void CTCLook::ImplementBodySettings()
{
    float maxTurnSpeed = PDef->MaxTurnSpeed;
    if (PParentCreature && !(PParentCreature->Flags91 & 1) && PParentCreature->IsInCombat())
        maxTurnSpeed = PDef->CombatMaxTurnSpeed;
    float current = ParentThing->PPlacement->GetAngleXY();
    float newAngle = current + Clamp(AngleDifference(current, BodyAngleXY), -(maxTurnSpeed * (1.0f / 360.0f)), maxTurnSpeed * (1.0f / 360.0f));
    if (newAngle < 0.0f || newAngle >= 1.0f) {
        newAngle = (float)fmod((double)newAngle, 1.0);
        if (newAngle < 0.0f)
            newAngle += 1.0f;
    }
    if (ParentThing->ThingType == 2) {
        CPlayer* player = GetPlayerManager()->GetMainPlayer();
        CThing* character = player->PCharacter.GetPItem();
        if (character && !(character->Flags91 & 1) && (character->Flags178 & 1)) {
            CThingPlacement* placement = ParentThing->PPlacement;
            if ((placement->PreviousPosition - placement->Position).GetSquaredLength() > g_epsilon_0129ba3c)
                return;
        }
    }
    if (!NCreatureMode::IsIncapacitated(*ParentThing))
        ParentThing->PPlacement->SetAngleXY(newAngle);
}
