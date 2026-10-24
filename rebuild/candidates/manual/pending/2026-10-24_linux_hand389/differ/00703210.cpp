// CThingObject::Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet, unsigned long uid, const CCharString& scriptName) -- allocate an object thing, construct it from the params, place its physics at the position (default orientation when none is given) and run the activation sequence; null on any failure
// Leaf: ?Create@CThingObject@@
// Grade: DIFFER(217v217) -- register swap only: retail keeps `position` in edi and defIndex/physics in ebx; ours the reverse (`mov ebx,edx ; mov edi,ecx`)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00703210, 217 bytes, `ret 0x10` (static __fastcall).
// Callees: CThingObject::Alloc 0x00527320, ConstructFromParams 0x00703050, CTCParamsPhysicsBase(const C3DVector&, const
//   CRightHandedSet&) 0x006A06E0, CTCPhysicsBase::ConstructFromParams (slot 3) 0x006AF700, CBaseClassNonCopyable dtor
//   0x0099A300, CThing::DoActivationSequence 0x004C9CA0.
// Tried (4): `result = 0` before Alloc without a physics local (registers right, but `xor ebp,ebp` before the call and
//   the physics pointer reloaded per branch), this form (result after Alloc, physics local), the physics local declared
//   at the top, `result` declared at the top and zeroed after Alloc -- the last three all swap ebx/edi.
class CCharString;
class CClass;

class C3DVector {                                         // 12 bytes
public:
    float X, Y, Z;
};

class CRightHandedSet {                                   // Ego_r.pdb: Up +0x00, Forward +0x0c (24 bytes)
public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet()
    {
        Up.X = 0.0f;
        Up.Y = 0.0f;
        Up.Z = 1.0f;
        Forward.X = 0.0f;
        Forward.Y = 1.0f;
        Forward.Z = 0.0f;
    }
};

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                              // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                     // 0x0099A300
};

class CTCParamsBase : public CBaseClassNonCopyable {      // vtable 0x0125C810
};

class CTCParamsPhysicsBase : public CTCParamsBase {       // vtable 0x0125C818 (40 bytes)
public:
    C3DVector Position;                                   // +0x04
    CRightHandedSet Rhs;                                  // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);   // 0x006A06E0
};

class CTCPhysicsBase {                                    // vtable 0x0125D2C4
public:
    virtual ~CTCPhysicsBase();
    virtual void v01();
    virtual void v02();
    virtual void ConstructFromParams(const CTCParamsBase& params);   // slot 3: 0x006AF700
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x60 - 4];
    CTCPhysicsBase* PhysicsTC;                            // +0x60 (PDB +0x70)
    bool DoActivationSequence();                          // 0x004C9CA0
};

class CThingObject : public CThing {
public:
    static CClass* __fastcall Alloc();                    // 0x00527320
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet,
                                           unsigned long uid, const CCharString& scriptName);
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, unsigned long uid,
                             const CCharString& scriptName);   // 0x00703050
};

CThingObject* __fastcall CThingObject::Create(long defIndex, const C3DVector& position, long playerNumber, const CRightHandedSet* rhSet,
                                              unsigned long uid, const CCharString& scriptName)
{
    CThingObject* object = (CThingObject*)Alloc();
    CThingObject* result = 0;
    if (object && object->ConstructFromParams(defIndex, position, playerNumber, uid, scriptName)) {
        CTCPhysicsBase* physics = object->PhysicsTC;
        if (rhSet)
            physics->ConstructFromParams(CTCParamsPhysicsBase(position, *rhSet));
        else
            physics->ConstructFromParams(CTCParamsPhysicsBase(position, CRightHandedSet()));
        if (object->DoActivationSequence())
            result = object;
    }
    return result;
}
