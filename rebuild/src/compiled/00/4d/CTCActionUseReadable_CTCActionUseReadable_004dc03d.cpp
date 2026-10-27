// CTCActionUseReadable::~CTCActionUseReadable() -- destroy the component's members (~CWideString 0x0099B510 (TextTag +0x18), the PDef (+0x14) release inline), then the CTCActionUseBase part
// Leaf: ??1CTCActionUseReadable@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC03D, 40 bytes, thiscall, tail-jumps to ~CTCActionUseBase 0x004DA967.
// Identity (proven): its only caller is the class's scalar deleting destructor 0x004DC021 (landed, with the stand-in callee name
//   ?CTCActionUseReadable_Destruct@@YIXPAX@Z); RTTI .?AVCTCActionUseReadable@@ : CTCActionUseBase; the destroyed members are Ego_r.pdb CTCActionUseReadable's at their retail offsets.
// Callees: ~CWideString 0x0099B510 (TextTag +0x18), the PDef (+0x14) release inline; ~CTCActionUseBase 0x004DA967 (tail jump).
// Shape (family K119, representative 0x004DC03D): an out-of-line destructor with no vptr store (the compiler-generated one,
//   kept out of line by the ??_G that calls it): members in reverse order, the CDefPointer's inline `if (Object) {
//   Object->Release(); Object = 0; }` (Release: `if (--RefCount == 0) Destroy();`), then the base destructor; size TU (`and [m],0`).
#pragma optimize("s", on)
class CDefPointeeBase {                                  // counted def object: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long RefCount;                                       // +0x04
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer {                   // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
class CReadableDef;
class CWideStringData;
class CWideString {                                      // 4 bytes: PStringData
public:
    ~CWideString();                                      // 0x0099B510
    CWideStringData* PStringData;
};
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb); engine/CTCBase.h has no virtual destructor
public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CActionUseDef;
class CTCActionUseBase : public CTCBase {                // retail 0x14 bytes (Ego_r.pdb)
public:
    CTCActionUseBase();
    virtual ~CTCActionUseBase();                         // 0x004DA967
    CDefPointer<const CActionUseDef> PActionUseDef;      // +0x0C
    bool Usable;                                         // +0x10
};
class CTCActionUseReadable : public CTCActionUseBase {                             // the destructor is the implicit one
public:
    CDefPointer<const CReadableDef> PDef;                // +0x14
    CWideString TextTag;                                 // +0x18
};
CTCActionUseReadable* Make_004dc03d() { return new CTCActionUseReadable; }                // emits the vtable, so the ??_G and the out-of-line implicit ??1