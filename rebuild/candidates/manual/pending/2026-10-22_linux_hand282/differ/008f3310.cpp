// CCreatureAction_DragonFireBlastBase::PerformAction() -- fire the weapon's shot from the creature's DRAGON_MOUTH_DUMMY toward a point level with the target, distance-matched along the mouth heading
// Grade: DIFFER(796v796) -- one scheduling slot: after the target GetPosition call retail emits fld [eax+8] ; fsub [esp+2Ch] ; mov ecx,edi where the lane hoists mov ecx,edi above the fsub; all else byte-identical (the C3DVector memberwise copy ctor fixes the target X/Y add order)
// Rescue 282: byte-neutral source changes never move the slot (a __fastcall inline helper as the FableWin PDB's
//   GFFastOneOverSqrt, GetCreature()/operator CThing*() accessors, inline member getters, named GetPItem/GetPosition
//   result locals, union and pointer pun forms, commutative operand swaps, GetSubDef/GetDef stand-in body variants,
//   unrelated functions in the TU, /Oa /Ow /Ob1 /Ox /GS). Byte-changing edits inside the if-body block flip it to
//   retail's order: an extra GetPItem call before the direction code, the GetString temp passed straight to AddTC,
//   the ability declared early, a Y-first by-value operator*, or a union pun that keeps lengthSquared on the x87 stack.
//   So the slot is scheduler state carried across the whole call-spanning block, and retail's identical stream with
//   the other decision points to a compile-time input that the bytes do not show (lane gap).
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline void Normalise() { *this *= FastInvSqrt(Z * Z + Y * Y + X * X); }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
__forceinline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
__forceinline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefStringTable {
    CCharString GetString(long index);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0xa0 - 0x08];
    long Shot;                                         // +0xa0
};
struct CThingShotDef : CDefBase {
    unsigned char _pad08[0x60 - 0x08];
    long TCName;                                       // +0x60
    unsigned char _pad64[0x7c - 0x64];
    long Thing;                                        // +0x7c
};

template <class T> struct CDefPointerByValue {
    T* m_p;
    CDefPointerByValue(T* p) : m_p(p) { ++p->RefCount; }
    CDefPointerByValue(const CDefPointerByValue& o) : m_p(o.m_p) { ++m_p->RefCount; }
    ~CDefPointerByValue();
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CDefClassBase {
    bool GetSubDef(CWeaponDef** out);                  // 0x0042B1A6
};
void* FindSubDef_CWeaponDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWeaponDef** out)
{
    void* found = FindSubDef_CWeaponDef(this);
    if (!found)
        return false;
    *out = (CWeaponDef*)found;
    return true;
}
#pragma auto_inline(on)

struct CDefinitionManager {
    bool GetDef(long index, CThingShotDef** out);      // 0x0078B230
};
void* FindDef_CThingShotDef(CDefinitionManager* manager, long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CThingShotDef** out)
{
    void* found = FindDef_CThingShotDef(this, index);
    if (!found)
        return false;
    *out = (CThingShotDef*)found;
    return true;
}
#pragma auto_inline(on)

struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* PDefinitionManager;            // +0x10
};

struct CTCBase;
struct CTCParamsBase;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char         _pad0[0x64];
    CTCGraphicAppearance* PGraphicAppearance;          // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                        // +0x70
    unsigned char         _pad74[0x90 - 0x74];
    signed char           PlayerNumber;                // +0x90
    const C3DVector& GetPosition() const;              // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool init, CTCParamsBase* params);   // 0x004C9D60
};
__forceinline bool GetDummyObject(const CThing* thing, const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
{
    CTCGraphicAppearance* appearance = thing->PGraphicAppearance;
    return appearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
}
struct CThingCreatureBase : CThing {
    long GetShotAccuracyPercentage();                  // 0x00663B10
};
CThing* __fastcall Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                          unsigned long flags, const CCharString& scriptName);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return Create(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}

struct CTCShotDefinitionBase {
    void Init(CDefPointerByValue<CThingShotDef> def, CThing* shooter, CThing* origin, const C3DVector* target, float accuracy,
              float a, float b, CDefPointer<CMeleeCombatAbilityDef>* ability);   // 0x007BF790
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CCombatActionBase {
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CWorld* GetPWorld() const;                         // 0x00692940
};

struct CCreatureAction_DragonFireBlastBase : CCombatActionBase {
    CIntelligentPointer PShot;                         // +0x114 (PDB +0x12c)
    virtual void PerformAction();
};

void CCreatureAction_DragonFireBlastBase::PerformAction()
{
    CWeaponDef* weaponDef = 0;
    CDefClassBase* creatureDef = PCreature.GetPItem()->PDef;
    creatureDef->GetSubDef(&weaponDef);
    CThingShotDef* shotDef;
    {
        CThingShotDef* out = 0;
        CDefinitionManager* manager = GetPWorld()->PDefinitionManager;
        manager->GetDef(weaponDef->Shot, &out);
        shotDef = out;
    }
    C3DVector position = PCreature.GetPItem()->GetPosition();
    CRightHandedSet orientation;
    GetDummyObject(PCreature.GetPItem(), "DRAGON_MOUTH_DUMMY", &position, &orientation);
    CThing* thing = CreateThingObject(shotDef->Thing, position, PCreature.GetPItem(), "");
    if (thing) {
        PShot = CIntelligentPointer(thing);
        long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
        CTCShotDefinitionBase* shot;
        {
            CCharString name = g_DefStringTable.GetString(shotDef->TCName);
            shot = (CTCShotDefinitionBase*)PShot.GetPItem()->AddTC(name, true, 0);
        }
        C3DVector direction(orientation.Up.X, orientation.Up.Y, 0.0f);
        direction *= -1.0f;
        direction.Normalise();
        float distance = (PTarget.GetPItem()->GetPosition() - position).GetLength();
        direction *= distance;
        C3DVector target = position + direction;
        target.Z = PTarget.GetPItem()->GetPosition().Z;
        {
            CDefPointer<CMeleeCombatAbilityDef> ability;
            shot->Init(shotDef, PCreature.GetPItem(), 0, &target, (float)accuracy, 1.0f, 0.0f, &ability);
        }
    }
    shotDef->Release();
    weaponDef->Release();
}
