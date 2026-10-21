// CCreatureAction_DropGift::PerformAction() -- when the creature's def carries a COpinionOfHeroDef sub-def, create the gift object (sub-def's gift index +0x28) at the creature's position, fling it with a random sideways acceleration (0.25*sin(random angle), same, 1.0), give it a random spin via the physics virtual slot 66 and make it not solid to the creature (adding a CTCSolidToThing)
// Grade: DIFFER(342v350) -- retail multiplies sin(angle) by 0.25 twice (`fld 0.25 ; fmul st(1) ; fstp X ; fmul 0.25 ; fstp Y`), ours CSEs the product (`fmul ; fst X ; fstp Y`); tried: named double, two sin() calls, s/4.0, float s, member-wise stores
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844740, length 350 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CDefClassBase::GetSubDef<COpinionOfHeroDef> @ 0x00685C00, CThing::GetPosition
// @ 0x004C73D0, CCreatureActionBase::GetPWorld @ 0x00692940 and GetPWorldMap @ 0x00692950 (manifest labels propagated), GetPRandomSeed
// @ 0x0049D810, GFFloatRandom(float, unsigned long*) @ 0x004978A0 (`__fastcall`), CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0,
// CThingObject::Create @ 0x00703210 (static `__fastcall`), CTCPhysicsBase::ApplyAcceleration @ 0x006B0210, the physics virtual slot 66
// (+0x108, one float), CThing::AddTC @ 0x004C9D60, CTCSolidToThing::AddThingNotSolidTo @ 0x00770FA0. Constants: 2*pi (double
// 0x0124F2B8), 0.25 (double 0x01238010), 1.0f.
// Shape: the sub-def is a CDefPointer local released by the inline dtor at the end (DecRefToZero); the position is copied into a
// C3DVector local; the acceleration is a C3DVector built from one sin() of the random angle; the script name is an empty literal
// temporary. Retail CCreatureActionBase is 0xa8 bytes; `this` only needs PCreature at +8.
extern "C" double __cdecl sin(double x);
#pragma intrinsic(sin)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct COpinionOfHeroDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long GiftDefIndex;                                // +0x28
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointer<COpinionOfHeroDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<COpinionOfHeroDef>* out); // 0x00685C00
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<COpinionOfHeroDef> 0x00685C00 (the landed body shape): VC7.1 then knows the def pointer's
// address does not escape, so the def rides in ebp across the rest of the body like retail.
bool CDefClassBase::GetSubDef(CDefPointer<COpinionOfHeroDef>* out)
{
    long* pIndex;
    {
        CCharString name("COpinionOfHeroDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<COpinionOfHeroDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CTCPhysicsBase {
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
    virtual void SetSpin(float amount);               // slot 66 (+0x108)
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CTCSolidToThing;
struct CTCParamsBase;
struct CThing {
    unsigned char _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;                       // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* m_pDef;                            // +0x70
    const C3DVector& GetPosition() const;             // 0x004C73D0
    CTCSolidToThing* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
};
struct CRightHandedSet;
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
struct CTCSolidToThing {
    void AddThingNotSolidTo(CThing* thing);           // 0x00770FA0
};
struct CRandomOwner {
    unsigned long* GetPRandomSeed();                  // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CRandomOwner* GetPWorld();                        // 0x00692940
    CRandomOwner* GetPWorldMap();                     // 0x00692950
};
struct CCreatureAction_DropGift : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_DropGift::PerformAction()
{
    CDefPointer<COpinionOfHeroDef> opinionDef;
    CDefClassBase* creatureDef = PCreature.GetPItem()->m_pDef;
    if (creatureDef->GetSubDef(&opinionDef)) {
        long giftIndex = opinionDef->GiftDefIndex;
        C3DVector pos = PCreature.GetPItem()->GetPosition();
        double angle = GFFloatRandom(1.0f, GetPWorld()->GetPRandomSeed()) * 6.2831854820251465;
        C3DVector accel((float)(sin(angle) * 0.25), (float)(sin(angle) * 0.25), 1.0f);
        CThingObject* gift = CThingObject::Create(giftIndex, pos, 5, 0, 0, "");
        if (gift) {
            gift->m_pPhysics->ApplyAcceleration(accel);
            float spin = GFFloatRandom(1.0f, GetPWorldMap()->GetPRandomSeed());
            gift->m_pPhysics->SetSpin(spin);
            CTCSolidToThing* solid = gift->AddTC("CTCSolidToThing", false, 0);
            solid->AddThingNotSolidTo(PCreature.GetPItem());
        }
    }
}
