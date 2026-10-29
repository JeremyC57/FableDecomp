// CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase() -- the dragon fire-blast action's implicit (compiler-generated) dtor: the member dtors in reverse order (the Line1 primitive handle: notify the primitive of its last handle and drop it; the PShot intelligent pointer) and the tail jump to ~CCombatActionBase
// Leaf: ??1CCreatureAction_DragonFireBlastBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009536D0, 119 bytes, tail jump to the base destructor.
// Identity: no vptr store of its own (the only store is the member handle's CEnginePrimitiveHandle vtable 0x0122F598), so the
//   dtor is implicit; the members are CCreatureAction_DragonFireBlastBase's (Ego_r.pdb PShot +0x12C, Line1 +0x134 = retail +0x114
//   / +0x11C: CCombatActionBase is 0x18 narrower); the derived DragonFireBlast dtor 0x00953770 is a bare `jmp` here and the
//   ctor-built locals at 0x009524D6 / 0x009539E3 / 0x00953A03 / 0x00953A23 are destroyed through it. Ego_r.pdb has
//   `??1CCreatureAction_DragonFireBlastBase@@UAE@XZ`; the catalog label is right.
// Callees: the primitive's NotifyReleased (slot 3, +0x0C; Ego_r.pdb protected `?NotifyReleased@CEnginePersistentPrimitiveBase@@
//   MAEXABVCEnginePrimitiveHandle@@@Z`, vtable 0x012A3D60) and Delete (slot 1, `?Delete@CIVCountedPointeeBase@@UBEXXZ`) on the
//   last release, CBaseClass::~CBaseClass 0x0099A2E0 (settled), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10,
//   CCombatActionBase::~CCombatActionBase 0x0062E900 (implicit too: no vptr store; it destroys LastWeaponUsedInStrike +0x100,
//   CombatAnimations +0xF0, StrikeEvents +0xBC, PMeleeAbilityDef +0xB8, PCameraPoint +0xB0, PTarget +0xA8 -- the Ego_r.pdb
//   CCombatActionBase members less 0x18 -- then jumps to ~CCreatureActionBase 0x00693EF0; Ego_r.pdb `??1CCombatActionBase@@UAE@XZ`).
// Inlines: CEnginePrimitiveHandle::~CEnginePrimitiveHandle (vptr store, Clear(): `if (Data.GetRefCount() == 1)
//   Data->NotifyReleased(*this); Data.Reset(0);`, then the CIVCountedPointer member dtor), as the landed handle ??_G 0x0041C500.
// Shape: the implicit dtor is emitted through a qualified explicit call in a one-line helper (chunk 562 device).
class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;                          // +0x00
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    T* operator->() const { return (T*)Object; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    virtual void CUnprovenSlot02_012a3d60() = 0;            // slot 2 (_purecall in the base vtable)
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4 (+0x10)
public:
    long Type;                                              // +0x08
    friend class CEnginePrimitiveHandle;
};

class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CBaseObjectPointer;
class CThing;
class CBaseIntelligentPointer { public:                     // 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCombatActionBase : public CCreatureActionBase { public:   // retail 0x114 bytes
    unsigned char _pad_0x04[0x114 - 0x04];
    virtual ~CCombatActionBase();                           // 0x0062E900
};
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PShot;                      // +0x114
    CEnginePrimitiveHandle      Line1;                      // +0x11c
};

void EmitImplicitDestructor(CCreatureAction_DragonFireBlastBase* action)   // emission device only (not a retail function)
{
    action->CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase();
}