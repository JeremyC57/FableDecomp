// CAnimComponentCombatFlourishTargetOffset::Alloc() const -- virtual factory: a new default CAnimComponentCombatFlourishTargetOffset (reference count 1)
// Leaf: ?Alloc@CAnimComponentCombatFlourishTargetOffset@@UBEPAVCAnimComponentBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DF790, 31 bytes (int3 padding 0x005DF786..0x005DF78F before, one int3 at 0x005DF7AF after),
//   thiscall (this unused), `push 11h ; call operator new` 0x00BFEA1A, null test, inlined default ctor, `ret`.
// Identity (proven, two sources): the only reference is slot 4 of the CAnimComponentCombatFlourishTargetOffset vtable 0x01255760
//   (0x01255770; RTTI .?AVCAnimComponentCombatFlourishTargetOffset@@; slot 2 = the landed GetType 0x005DF780 returning 7, slot 3
//   Initialise 0x005DC8B0, slot 5 Clone 0x005DB3B0, slot 7 TransferBinaryOut 0x005DB3D0, slot 8 SetDefaultValue 0x005DB3A0); the
//   sibling factories CAnimComponentCombatMisc 0x005DF750 / CombatRecoil 0x005DF710 sit in the same slot. Ego_r.pdb and FableWin.pdb
//   both list ?Alloc@CAnimComponentCombatFlourishTargetOffset@@UBEPAVCAnimComponentBase@@XZ (const virtual, base-pointer return);
//   the allocation size 0x11 is the PDB sizeof(CAnimComponentCombatFlourishTargetOffset).
// Callee: operator new 0x00BFEA1A (MSVCR71 scalar new import thunk). Vtable slots follow 0x01255760 (Ego_r / FableWin spellings).
// Layout: Ego_r.pdb CIVCountedPointeeBase {vptr, ulong IVRefCount +0x4} (8) <- CAnimComponentBase (8, 1-packed) <-
//   CAnimComponentCombatFlourishTargetOffset {bool TargetOffset +0x8, C2DVector TargetOffsetVector +0x9} = 0x11 bytes
//   (engine/CAnimComponentCombatFlourishTargetOffset.h has the same offsets but flattens the polymorphic base into
//   `void* __vftable` + `_base_0x04`, so it cannot be newed with its vtable: header debt). Local models in the landed
//   CAnimComponentCombatMisc::Alloc 0x005DF750 form (IV base outside the pack, the anim classes 1-packed).
// Shape: the reference count is set by the inlined base ctor before the vtable store (the base vtable stores are dead); the
//   members stay uninitialised (the ctor 0x005DF770 stores only IVRefCount and the vtable; SetDefaultValue 0x005DB3A0 sets them).
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A

class CIVCountedPointeeBase {                                // vtable 0x012354D0 (engine/CIVCountedPointeeBase.h retail form)
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }             // FableWin ??0CIVCountedPointeeBase@@QAE@XZ (inline in retail)
    virtual ~CIVCountedPointeeBase() { }                    // slot 0
    virtual void Delete() const;                            // slot 1: 0x0042D930
    mutable unsigned long IVRefCount;                       // +0x04
};

class C2DVector {
public:
    float X;
    float Y;
};

class CPersistContext;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
enum EAnimComponent { };

#pragma pack(push, 1)
class CAnimComponentBase : public CIVCountedPointeeBase {    // vtable 0x01255648: slots 2..8 are _purecall 0x00BFEA08
public:
    virtual EAnimComponent GetType() const = 0;                                        // slot 2
    virtual bool Initialise(CPersistContext& context, const CCharString& name) = 0;   // slot 3
    virtual CAnimComponentBase* Alloc() const = 0;                                    // slot 4
    virtual CAnimComponentBase* Clone() const = 0;                                    // slot 5
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;                // slot 6
    virtual void TransferBinaryOut(CDataOutputStream& stream) const = 0;              // slot 7
    virtual void SetDefaultValue() = 0;                                               // slot 8
};

class CAnimComponentCombatFlourishTargetOffset : public CAnimComponentBase {   // vtable 0x01255760
public:
    virtual EAnimComponent GetType() const;                                        // 0x005DF780
    virtual bool Initialise(CPersistContext& context, const CCharString& name);   // 0x005DC8B0
    virtual CAnimComponentBase* Alloc() const;                                    // 0x005DF790 (this row)
    virtual CAnimComponentBase* Clone() const;                                    // 0x005DB3B0
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);                // 0x005DBDA0
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;              // 0x005DB3D0
    virtual void SetDefaultValue();                                               // 0x005DB3A0
    bool TargetOffset;                                      // +0x08
    C2DVector TargetOffsetVector;                           // +0x09
};
#pragma pack(pop)

CAnimComponentBase* CAnimComponentCombatFlourishTargetOffset::Alloc() const
{
    return new CAnimComponentCombatFlourishTargetOffset;
}