// CEnginePrimitiveHandle::CEnginePrimitiveHandle() -- default constructor: the CBaseClass base, the vtable, then the CIVCountedPointer member's default
// Leaf: ??0CEnginePrimitiveHandle@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BB9E4, 22 bytes, thiscall, plain `ret` (size-optimised module).
// Identity (proven): stores vtable 0x0122F598 (RTTI .?AVCEnginePrimitiveHandle@@); Ego_r.pdb CEnginePrimitiveHandle: Data (CIVCountedPointer<CEnginePersistentPrimitiveBase>, Object = 0) at +4.
// Callees: CBaseClass::CBaseClass() 0x0099A2D0.
// Shape (family M031, representative 0x0044C213): `optimize("s", on)` -- the base constructor out of line, the vtable store,
//   then the member's inline default constructor as a size-form store (`and dword ptr [esi + 4], 0`).
#pragma optimize("s", on)
class CBaseClass {                                        // vptr only (Ego_r.pdb)
public:
    CBaseClass();                                         // 0x0099A2D0
    virtual ~CBaseClass();
};
class CIVCountedPointeeBase;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointeeBase* Object;
};
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};
CEnginePrimitiveHandle::CEnginePrimitiveHandle()
{
}