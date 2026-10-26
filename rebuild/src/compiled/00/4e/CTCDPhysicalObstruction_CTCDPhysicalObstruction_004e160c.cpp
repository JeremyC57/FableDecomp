// CTCDPhysicalObstruction::~CTCDPhysicalObstruction() -- physical-obstruction thing component teardown: clear the three primitive handles (each tells its primitive when it held the last reference), destroy the obstruction-thing vector, then the CTCBase base
// Leaf: ??1CTCDPhysicalObstruction
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E160C, 278 bytes (ends in a `jmp` to the base destructor); called only by
//   its scalar deleting destructor 0x004E15F0 (manifest label "CLandscapeBackgroundPatch::vector_deleting_destructor"
//   is propagated).
// Callees: the primitive's virtual slots 3 (+0xc, OnLastHandleReleased(handle)) and 1 (+4, destroy at the last
//   reference), ~CBaseClass 0x0099A2E0 (each handle's base), vector<CIntelligentPointer<CThing> >::~vector 0x004406D0
//   (out of line), CTCBase::~CTCBase 0x00686830 (tail jump; manifest "~CChunkedFileChunk" is propagated).
// Layout = Ego_r.pdb CTCDPhysicalObstruction (as the landed ctor 0x004DF14B, RTTI vtable 0x01241C0C): CTCDriverBase
//   (CTCBase) +0x00..+0x0b, StartPos/EndPos +0x0c/+0x18, LifeTimer +0x24, Radius +0x28, ThingsToIgnoreObstruction
//   (vector<CIntelligentPointer<CThing> >, 12 bytes in retail STLport) +0x2c, line1/line2/line3 (CEnginePrimitiveHandle,
//   vtable 0x0122F598 + counted Data pointer) +0x38/+0x40/+0x48 (PDB +0x3c.., 4 high for the 16-byte PDB vector).
//   The handle model is the landed size-TU CTCStockItem::~CTCStockItem 0x004DE44D one. CTCDriverBase declares no
//   destructor, so it is folded into the CTCBase model here.
// Pragma: optimize("s", on) -- the 0x4Dxxxx..0x4Exxxx band is a size TU (odd start address, vtable held in ebx).
// Shape: no vtable store of the class's own (the class declares no destructor; VC7.1 drops the vtable reset in a
//   compiler-generated destructor, which is emitted out of line here because the scalar deleting destructor calls it);
//   members die in reverse order, each handle's inline destructor is Clear() (`GetRefCount() == 1` through the null-safe
//   ternary count -- the size TU keeps `mov eax,[eax+4] / xor eax,eax ; cmp eax,1` -- then `Data = 0` = release + two
//   zero stores) followed by the counted pointer's own null-tested release, then ~CBaseClass. The empty ctor and the
//   qualified destructor call only make the TU emit the implicit destructor out of line.
#pragma optimize("s", on)
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    __forceinline void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    __forceinline long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline bool IsUnique() const { return GetRefCount() == 1; }
    __forceinline void ReleaseRef() { if (Object) { Object->Release(); Object = 0; } }
    __forceinline CIVCountedPointer& operator=(int) { ReleaseRef(); Object = 0; return *this; }
    __forceinline ~CIVCountedPointer() { ReleaseRef(); }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
};
class CThing;
class CThingPointerVector {                        // vector<CIntelligentPointer<CThing> >, 12 bytes in retail
public:
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    ~CThingPointerVector();                        // 0x004406D0
};
class CTCBase {
public:
    virtual ~CTCBase();                            // 0x00686830
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};
class CTCDPhysicalObstruction : public CTCBase {
public:
    float                  StartPos[3];            // +0x0c (C3DVector)
    float                  EndPos[3];              // +0x18 (C3DVector)
    long                   LifeTimer;              // +0x24
    float                  Radius;                 // +0x28
    CThingPointerVector    ThingsToIgnoreObstruction; // +0x2c
    CEnginePrimitiveHandle line1;                  // +0x38 (PDB +0x3c)
    CEnginePrimitiveHandle line2;                  // +0x40 (PDB +0x44)
    CEnginePrimitiveHandle line3;                  // +0x48 (PDB +0x4c)
    CTCDPhysicalObstruction();
};

CTCDPhysicalObstruction::CTCDPhysicalObstruction()
{
}

void DestroyPhysicalObstruction(CTCDPhysicalObstruction* p)
{
    p->CTCDPhysicalObstruction::~CTCDPhysicalObstruction();
}