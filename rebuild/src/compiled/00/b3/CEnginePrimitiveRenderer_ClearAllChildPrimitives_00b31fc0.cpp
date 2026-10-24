// CEnginePrimitiveRenderer::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle) -- forward to the handle's persistent primitive (its virtual ClearAllChildPrimitives) when the handle holds one
// Leaf: ?ClearAllChildPrimitives@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31FC0, 19 bytes. __fastcall (this unused), `ret 4`.
// Identity: FableWin.pdb `?ClearAllChildPrimitives@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@@Z`; the only
//   caller is the CEngine::ClearAllChildPrimitives thunk 0x00B23C70 (`mov ecx,[0x01436E80]` -- the engine's primitive
//   renderer -- then `jmp`). Its siblings 0x00B31F20 / 0x00B31F50 / 0x00B31F70 / 0x00B31FA0 are the renderer's
//   SetChildPrimitive x2 / GetChildPrimitiveHandle / ClearChildPrimitive forwards (vtable slots 7..10). The catalog label
//   "ChildPrimitive_ClearAllViaVtable" is descriptive.
// Callees: CEnginePersistentPrimitiveBase vtable slot 11 (+0x2C), ClearAllChildPrimitives() (Ego_r.pdb
//   `?ClearAllChildPrimitives@CEngineInternalPrimitiveBase@@UAEXXZ`).
// Layout: CEnginePrimitiveHandle is Ego_r's 8 bytes {vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04}
//   (the landed GetChildPrimitiveHandle 0x00BBC3A0 model).
class CEnginePrimitive;
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                   unsigned long flags);                            // slot 7 (+0x1C)
    virtual void SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags); // slot 8
    virtual void GetChildPrimitiveHandle(unsigned long id, CEnginePrimitiveHandle& handle);                  // slot 9
    virtual void ClearChildPrimitive(unsigned long id);                                                       // slot 10
    virtual void ClearAllChildPrimitives();                                                                   // slot 11 (+0x2C)
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};

class CEnginePrimitiveHandle {                          // 8 bytes
public:
    virtual void v000();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};

class CEnginePrimitiveRenderer {
public:
    void ClearAllChildPrimitives(CEnginePrimitiveHandle& handle);
};

void CEnginePrimitiveRenderer::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
{
    if (handle.Data)
        handle.Data->ClearAllChildPrimitives();
}