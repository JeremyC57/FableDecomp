// Grade: DIFFER(672v675) -- the 007bd2d1 residue: frame 0x90 v 0x8c (retail shares the IS_THING_ALIVE pointer temporary's slot [ebp-4] with the spilled AddTC result and puts the sub-def at [ebp-8]), the spread angle addressed as [ecx+0xa4] twice (ours CSEs the address with `add ecx,0xa4`), and the AddTC/name temporaries scheduled differently; shot declared at function scope and the raw def local removed are identical
// NThingShot::CTCShotDefinitionProjectilePredictive::OnSubShotTrigger() -- spawn the shot def's sub-shots fanned across +-SpreadAngle about the shot's forward axis, 30 units out along the rotated up vector, each initialised from this shot and told about every thing already hit
// Leaf: OnSubShotTrigger
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD527, 675 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: as 007bd2d1 plus IS_THING_ALIVE 0x0040DCE0, CRightHandedSet rotate-about-axis 0x00A22F80 (catalog
//   "CMatrix3x3::MultiplyByQuaternionRotation"), CTCShotDefinitionBase::AddHitThing 0x007C0FB0 (catalog
//   "AddDependentThing"), GetPTargetThing 0x007BEFF0. Floats -1.0f 0x0122DEE0, 30.0f 0x01248018.
// Layout: shot base hit list begin +0x14 / end +0x18 (8-byte intelligent pointers), PShooter +0x28, PShotDef +0x38,
//   melee def +0x3c, damage/params +0x40..+0x48; CThing team byte +0x90 (movsx).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
struct CBasicStringChar {
    bool operator==(const char* text) const;                 // 0x004115A0
};
struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                          // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
};
struct CDefString {
    long Index;
    operator CCharString() const;                            // 0x00415DD0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const CRightHandedSet& other);           // 0x0049CF80
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80 (catalog "CMatrix3x3::MultiplyByQuaternionRotation")
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +4
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    CDefPointer(const CDefPointer& other) : Data(other.Data) { ++Data->RefCount; }
    explicit CDefPointer(T* data) : Data(data) { ++Data->RefCount; }
    ~CDefPointer() { if (--Data->RefCount == 0) Data->Destroy(); }
    T* operator->() const { return Data; }
};
struct CThingShotDef : CDefBase {
    unsigned char _pad08[0x60 - 0x08];
    CDefString    Name;                                      // +0x60
    unsigned char _pad64[0x7c - 0x64];
    long          ObjectDefIndex;                            // +0x7c
    unsigned char _pad80[0x98 - 0x80];
    long          NumSubShots;                               // +0x98
    long          SubShotDefIndex;                           // +0x9c
    unsigned char _padA0[0xa4 - 0xa0];
    float         SubShotSpreadAngle;                        // +0xa4
};
struct CMeleeCombatAbilityDef;
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingShotDef>* out);   // 0x0078B230
    CDefString GetDefClassTypeNameFromGlobalIndex(long index);  // 0x009ACCE0
};
CThingShotDef* FindRef_GetDef_0(long index);
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(long index, CDefPointer<CThingShotDef>* out)
{
    CThingShotDef* def = FindRef_GetDef_0(index);
    if (!def)
        return false;
    out->Data = def;
    return true;
}
#pragma auto_inline(on)
struct CMainGameComponent {
    CDefinitionManager* GetDefinitionManager();              // 0x00415FBC
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CPlayer {
    unsigned char _pad00[0x28];
    long          Team;                                      // +0x28
};
struct CPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet* GetRHSet();               // slot 72 (+0x120)
};
struct CTCBase;
struct CThing {
    unsigned char _pad00[0x60];
    CPhysics*     Physics;                                   // +0x60
    unsigned char _pad64[0x90 - 0x64];
    char          Team;                                      // +0x90
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    CPlayer* GetPlayer() const;                              // 0x004C7A30
    CTCBase* AddTC(const CCharString& name, bool flag, void* params);   // 0x004C9D60
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long team,
                                           const CRightHandedSet& orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);       // 0x0040DCE0
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                       // +0x00
    CThing*                 ParentThing;                     // +0x04
    unsigned char           _pad08[0x14 - 0x08];
    CBaseIntelligentPointer* HitStart;                       // +0x14
    CBaseIntelligentPointer* HitFinish;                      // +0x18
    unsigned char           _pad1c[0x28 - 0x1c];
    CBaseIntelligentPointer PShooter;                        // +0x28
    CBaseIntelligentPointer PTargetThing;                    // +0x30
    const CThingShotDef*    PShotDef;                        // +0x38
    CDefPointer<CMeleeCombatAbilityDef> PMeleeDef;           // +0x3c
    float                   Damage;                          // +0x40
    float                   ParamA;                          // +0x44
    float                   ParamB;                          // +0x48
    CThing* GetPTargetThing() const;                         // 0x007BEFF0
    void Init(CDefPointer<CThingShotDef> def, CThing* shooter, CThing* target, const C3DVector* start, float damage,
              float a, float b, const CDefPointer<CMeleeCombatAbilityDef>* melee);   // 0x007BF790
    void AddHitThing(CThing* thing);                         // 0x007C0FB0
    unsigned int NumHitThings() const { return HitFinish - HitStart; }
};
struct CTCShotDefinitionProjectilePredictive : CTCShotDefinitionBase {
    void OnSubShotTrigger();
};

void CTCShotDefinitionProjectilePredictive::OnSubShotTrigger()
{
    long subShotIndex = PShotDef->SubShotDefIndex;
    if (subShotIndex < 0)
        return;
    CDefPointer<CThingShotDef> subDef;
    g_pMainGameComponent->GetDefinitionManager()->GetDef(subShotIndex, &subDef);
    CThingShotDef* def = subDef.Data;
    long objectDefIndex = def->ObjectDefIndex;
    CCharString className = g_pMainGameComponent->GetDefinitionManager()->GetDefClassTypeNameFromGlobalIndex(objectDefIndex).operator CCharString();
    CRightHandedSet baseOrientation(*ParentThing->Physics->GetRHSet());
    long numSubShots = PShotDef->NumSubShots;
    if (numSubShots > 0) {
        float angle = PShotDef->SubShotSpreadAngle * -1.0f;
        float step = 2.0f * PShotDef->SubShotSpreadAngle / (numSubShots - 1);
        long remaining = numSubShots;
        do {
            if (className == "OBJECT") {
                CRightHandedSet orientation(*ParentThing->Physics->GetRHSet());
                long team = ParentThing->Team;
                if (IS_THING_ALIVE(PShooter.GetPItem()))
                    team = PShooter.GetPItem()->Team;
                CThingObject* object = CThingObject::Create(objectDefIndex, ParentThing->GetPosition(), team, orientation, 0, "");
                if (object) {
                    CTCShotDefinitionBase* shot = (CTCShotDefinitionBase*)object->AddTC(def->Name, true, 0);
                    CRightHandedSet rotated(baseOrientation);
                    rotated.RotateAboutAxis(baseOrientation.Forward, angle);
                    C3DVector start = rotated.Up * 30.0f + ParentThing->GetPosition();
                    shot->Init(CDefPointer<CThingShotDef>(def), PShooter.GetPItem(), GetPTargetThing(), &start, (float)(long)Damage, ParamA, ParamB, &PMeleeDef);
                    for (unsigned int i = 0; i < NumHitThings(); ++i)
                        shot->AddHitThing(HitStart[i].GetPItem());
                }
            }
            angle += step;
        } while (--remaining);
    }
}

}
