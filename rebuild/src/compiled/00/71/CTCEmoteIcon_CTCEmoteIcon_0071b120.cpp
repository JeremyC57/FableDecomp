// CTCEmoteIcon::~CTCEmoteIcon() -- emote icon TC dtor (compiler-generated): the member dtors in reverse order (the engine primitive handle, the emote text) and ~CTCBase
// Leaf: ??1CTCEmoteIcon@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B120, 113 bytes, tail jump to the base destructor.
// Identity: the only call is from 0x0071B103, slot 0 (the scalar deleting destructor 0x0071B100) of vtable 0x01265ABC, RTTI
//   `.?AVCTCEmoteIcon@@` (rtti_bases: CTCEmoteIcon : CTCBase); slot 22 of that vtable, 0x0071B0E0, builds the CCharString
//   "CTCEmoteIcon" (GetClassTypeName). FableWin.pdb has `??1CTCEmoteIcon@@UAE@XZ` and `??_GCTCEmoteIcon@@UAEPAXI@Z`, and the
//   Ego_r.pdb CTCEmoteIcon layout puts EmoteText (CWideString) at +0x10 and PrimitiveHandle (CEnginePrimitiveHandle) at +0x28,
//   the two members this body destroys. No vtable store of its own: the destructor is implicit. The catalog label
//   `CCreatureAction_DragonFireBlastBase::~CCreatureAction_DragonFireBlastBase` is wrong (that class is a CCombatActionBase,
//   not a CTCBase).
// Callees: ~CBaseClass 0x0099A2E0 (the handle's base, out of line), ~CWideString 0x0099B510, CTCBase::~CTCBase 0x00686830
//   (tail jump); virtual calls: the primitive's NotifyReleased (slot 3) and CIVCountedPointeeBase::Delete (slot 1).
// Layout: Ego_r.pdb CTCEmoteIcon (0x30; no STL members, so the retail offsets are the PDB ones): ActiveTimer +0x0C,
//   EmoteText +0x10, EmoteIcon +0x14, EmoteScale +0x18, EmoteType +0x1C, EmotePriority +0x20, EmoteAlpha +0x24, Replaceable
//   +0x25, Enabled +0x26, PrimitiveHandle +0x28. No engine header for CTCEmoteIcon; engine/CTCBase.h flattens the vptr.
// Shape: the implicit destructor, emitted out of line through a qualified explicit destructor call in a one-line helper
//   (EmitImplicitDestructor, not a retail function; chunk 562 0x005EDA50). The inline virtual ~CEnginePrimitiveHandle stores the
//   handle vtable 0x0122F598, runs Clear() (null-safe GetReferenceCount() == 1 test, NotifyReleased(*this), Data.Reset(0)), the
//   CIVCountedPointer destructor and ~CBaseClass (the agent 561/562 CEnginePrimitiveHandle model).
class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
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
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};

class CTCEmoteIcon : public CTCBase { public:
    enum EEmoteType { };                                    // values not needed here
    enum EEmotePriority { };                                // values not needed here
    long                    ActiveTimer;                    // +0x0C
    CWideString             EmoteText;                      // +0x10
    long                    EmoteIcon;                      // +0x14
    float                   EmoteScale;                     // +0x18
    EEmoteType              EmoteType;                      // +0x1C
    EEmotePriority          EmotePriority;                  // +0x20
    unsigned char           EmoteAlpha;                     // +0x24
    bool                    Replaceable;                    // +0x25
    bool                    Enabled;                        // +0x26
    CEnginePrimitiveHandle  PrimitiveHandle;                // +0x28
};

void EmitImplicitDestructor(CTCEmoteIcon* p) { p->CTCEmoteIcon::~CTCEmoteIcon(); }