// CTCPhysicsBase::OnCreate() -- on creation: remember the start position, fetch the physics sub-def of the thing's definition (or drop it when the thing has no definition), derive the radius (mesh bounding-circle radius, else half the def's diameter, else 1), stop the body and mark it moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0780, 198 bytes (plain `ret`). CTCPhysicsBase vtable 0x0125D2C4 slot 4 (PDB ?OnCreate@CTCPhysicsBase@@UAEXXZ).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0 (PDB IBEABVCGameDefinitionManager@@XZ), CGameDefinitionManager::
//   GetDef<T>(long, CDefPointer<T>&) @ 0x0042AF3C, CDefClassBase::GetSubDef<CPhysicsDef>(CDefPointer&) @ 0x006B0FA0, the def
//   release through vtable slot 1, virtual GetPhysicsMeshInfo slot 93 (+0x174). Floats: 0.5f @ 0x0122F59C, 1.0f @ 0x0122DED8.
// Layout: Ego_r.pdb CTCPhysicsBase (0x40); CThing DefGlobalIndex at retail +0x8c (PDB +0x9c); CPhysicsDef Diameter +0x28 (the
//   retail CPhysicsDef keeps the PDB offsets: GetAirResistance 0x006AFDE0 reads +0x34 = AirResistance); CThingPhysicsMeshInfo
//   BoundingCircleRadius +0x18.
// Shape: `PhysicsDef.Clear()` is the `release ; p = 0` plus a second unconditional zero store with the object in a pointer local;
//   CalculateRadius() is the inline nested ternary (one shared fstp); `Velocity.Zero()` keeps the zero stores before the bit
//   update (`X = Y = Z = 0` written in OnCreate hoists the +0x3c byte load above them).
// Tried: pointer local + ReleaseDef, RAII CDefPointer destructor (re-reads the slot), holder struct, holder + Zero().
//   Next: lane gap: copy-register allocation (retail loads the def into the callee-saved register first; the 0x0079DCC0 shape).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class C3DVector { public:
    float X, Y, Z;
    void Zero() { X = Y = Z = 0.0f; }
};
class C2DVector { public: float X, Y; };
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
    void Clear()
    {
        T* object = Object;
        if (object) {
            if (DecRefToZero(object->m_refCount))
                object->Destroy();
            Object = 0;
        }
        Object = 0;
    }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && DecRefToZero(def->m_refCount))
        def->Destroy();
}
template <class T> struct CDefHolder {                  // owns the reference GetDef handed over
    T* Object;
    __forceinline ~CDefHolder() { ReleaseDef(Object); }
};
class CPhysicsDef : public CDefBase { public:
    unsigned char _pad08[0x28 - 0x08];
    float Diameter;                                     // +0x28
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x006B0FA0 (T = CPhysicsDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CThingDef : CDefClassBase { };
class CGameDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x0042AF3C (T = CThingDef)
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CThingPhysicsMeshInfo { public:
    unsigned short BankIndex;                           // +0x00
    float     RenderSizeX;                              // +0x04
    C3DVector ToCentre;                                 // +0x08
    float     BoundingRadius;                           // +0x14
    float     BoundingCircleRadius;                     // +0x18
};
class CThing { public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;                      // +0x8c (PDB +0x9c)
};
class CTCPhysicsBase { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void OnCreate();                                // slot 4
    virtual void InitialActivate();                         // slot 5
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual C2DVector GetOffsetVector() const;              // slot 28 (+0x70)
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual CThingPhysicsMeshInfo* GetPhysicsMeshInfo();    // slot 93 (+0x174)
    CThing*   ParentThing;                              // +0x04 (CTCBase)
    bool      ToBeRemoved;                              // +0x08
    bool      IsInGlobalUpdateTCs;                      // +0x09
    unsigned char _pad0a[2];
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector Velocity;                                 // +0x28
    float     Radius;                                   // +0x34
    CDefPointer<CPhysicsDef> PhysicsDef;                // +0x38
    bool      Moving : 1;                               // +0x3c bit 0
    bool      OldMoving : 1;                            // +0x3c bit 1
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    float CalculateRadius()
    {
        CThingPhysicsMeshInfo* mesh = GetPhysicsMeshInfo();
        return mesh ? mesh->BoundingCircleRadius : (PhysicsDef.Object ? PhysicsDef.Object->Diameter * 0.5f : 1.0f);
    }
};

void CTCPhysicsBase::OnCreate()
{
    OldPosition = Position;
    unsigned short defIndex = ParentThing->DefGlobalIndex;
    if (defIndex != 0) {
        CDefPointer<CThingDef> thingDef;
        GetDefinitionManager().GetDef(defIndex, thingDef);
        CDefHolder<CThingDef> def = { thingDef.Object };
        def.Object->GetSubDef(PhysicsDef);
    } else
        PhysicsDef.Clear();
    Radius = CalculateRadius();
    Velocity.Zero();
    Moving = true;
    OldMoving = true;
}