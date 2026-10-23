// NThingShot::CTCShotDefinitionThrow::OnSubShotTrigger() -- spawn the shot def's sub-shots: for each of NumSubShots spread evenly across +-SpreadAngle about the line to the target, create the sub-shot object at the owner and initialise its shot TC from this shot
// Grade: DIFFER(599v598) -- with a same-TU stand-in GetDef body the sub-def now stays in esi (was 613v598); left: frame 0x7c v 0x78 (one extra 4-byte slot), retail loads the team into eax before pushing the Create arguments (ours pushes &name/0/&rh then `push [eax+0x28]`), and addresses the spread angle as [ecx+0xa4] twice (ours CSEs the address); a named team local is worse (605)
// Leaf: OnSubShotTrigger
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD2D1, 598 bytes, ebp frame + `leave` (optimize("y", off) + ("s", on) TU).
// Callees: CMainGameComponent::GetDefinitionManager 0x00415FBC (thunk) on the singleton 0x013B86A0, CDefinitionManager::
//   GetDef<CThingShotDef> 0x0078B230 / GetDefClassTypeNameFromGlobalIndex 0x009ACCE0, CDefString::operator CCharString
//   0x00415DD0, GetTargetPosition 0x007BEDD0, CThing::GetPosition 0x004C73D0 / GetPlayer 0x004C7A30 / AddTC 0x004C9D60,
//   CBasicString<char>::operator== 0x004115A0, CRightHandedSet copy 0x0049CF80, CThingObject::Create 0x00703210,
//   C3DVector::RotateAboutZ 0x00A14040, GetPTargetThing 0x007BEFF0, CTCShotDefinitionBase::Init 0x007BF790,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0. Data: "OBJECT" 0x01235FF4, "" 0x0122D70E, -1.0f 0x0122DEE0.
// Layout: shot base PShooter +0x28, PTargetThing +0x30, PShotDef +0x38, melee def +0x3c, floats +0x40/+0x44/+0x48;
//   CThingShotDef name +0x60, object def +0x7c, sub-shot count +0x98, sub-shot def +0x9c, spread +0xa4.
// Shape: the line to the target is a scalar-replaced C3DVector (user copy constructor); the loop is a do/while count-down
//   inside `if (n > 0)`; the class name is the explicit `.operator CCharString()` call (constructs in place).
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
    void RotateAboutZ(float angle);                          // 0x00A14040
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const CRightHandedSet& other);           // 0x0049CF80
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
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    CPlayer* GetPlayer() const;                              // 0x004C7A30
    CTCBase* AddTC(const CCharString& name, bool flag, void* params);   // 0x004C9D60
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long team,
                                           const CRightHandedSet& orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};

namespace NThingShot {

struct CTCShotDefinitionBase {
    void*                   __vftable;                       // +0x00
    CThing*                 ParentThing;                     // +0x04
    unsigned char           _pad08[0x28 - 0x08];
    CBaseIntelligentPointer PShooter;                        // +0x28
    CBaseIntelligentPointer PTargetThing;                    // +0x30
    const CThingShotDef*    PShotDef;                        // +0x38
    CDefPointer<CMeleeCombatAbilityDef> PMeleeDef;           // +0x3c
    float                   Damage;                          // +0x40
    float                   ParamA;                          // +0x44
    float                   ParamB;                          // +0x48
    C3DVector GetTargetPosition() const;                     // 0x007BEDD0
    CThing* GetPTargetThing() const { return PTargetThing.GetPItem(); }
    void Init(CDefPointer<CThingShotDef> def, CThing* shooter, CThing* target, const C3DVector* start, float damage,
              float a, float b, const CDefPointer<CMeleeCombatAbilityDef>* melee);   // 0x007BF790
};
struct CTCShotDefinitionThrow : CTCShotDefinitionBase {
    void OnSubShotTrigger();
};

void CTCShotDefinitionThrow::OnSubShotTrigger()
{
    long subShotIndex = PShotDef->SubShotDefIndex;
    if (subShotIndex < 0)
        return;
    CDefPointer<CThingShotDef> subDef;
    g_pMainGameComponent->GetDefinitionManager()->GetDef(subShotIndex, &subDef);
    CThingShotDef* def = subDef.Data;
    long objectDefIndex = def->ObjectDefIndex;
    CCharString className = g_pMainGameComponent->GetDefinitionManager()->GetDefClassTypeNameFromGlobalIndex(objectDefIndex).operator CCharString();
    C3DVector target = GetTargetPosition();
    C3DVector offset = GetTargetPosition() - ParentThing->GetPosition();
    long numSubShots = PShotDef->NumSubShots;
    if (numSubShots > 0) {
        float angle = PShotDef->SubShotSpreadAngle * -1.0f;
        float step = 2.0f * PShotDef->SubShotSpreadAngle / (numSubShots - 1);
        long remaining = numSubShots;
        do {
            if (className == "OBJECT") {
                CRightHandedSet orientation(*ParentThing->Physics->GetRHSet());
                CThingObject* object = CThingObject::Create(objectDefIndex, ParentThing->GetPosition(),
                                                            PShooter.GetPItem()->GetPlayer()->Team, orientation, 0, "");
                if (object) {
                    CTCShotDefinitionBase* shot = (CTCShotDefinitionBase*)object->AddTC(def->Name, true, 0);
                    C3DVector direction = offset;
                    direction.RotateAboutZ(angle);
                    C3DVector start = direction + ParentThing->GetPosition();
                    shot->Init(CDefPointer<CThingShotDef>(def), PShooter.GetPItem(), GetPTargetThing(), &start, (float)(long)Damage, ParamA, ParamB, &PMeleeDef);
                }
            }
            angle += step;
        } while (--remaining);
    }
}

}
