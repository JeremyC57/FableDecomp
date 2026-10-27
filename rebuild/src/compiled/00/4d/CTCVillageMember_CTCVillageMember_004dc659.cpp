// CTCVillageMember::~CTCVillageMember() -- destroy the component's members (~CBaseIntelligentPointer 0x00A01C10 (PVillage +0x10), the PDef (+0x0C) release inline), then the CTCBase part
// Leaf: ??1CTCVillageMember@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC659, 40 bytes, thiscall, tail-jumps to ~CTCBase 0x00686830.
// Identity (proven): its only caller is the class's scalar deleting destructor 0x004DC63D (landed, with the stand-in callee name
//   ?CTCVillageMember_Destruct@@YIXPAX@Z); RTTI .?AVCTCVillageMember@@ : CTCBase; the destroyed members are Ego_r.pdb CTCVillageMember's at their retail offsets.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (PVillage +0x10), the PDef (+0x0C) release inline; ~CTCBase 0x00686830 (tail jump).
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
class CVillageMemberDef;
class CBaseIntelligentPointer {                        // 8 bytes: vptr, PData
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb); engine/CTCBase.h has no virtual destructor
public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCVillageMember : public CTCBase {                             // the destructor is the implicit one
public:
    CDefPointer<const CVillageMemberDef> PDef;           // +0x0C
    CIntelligentPointer<CThing> PVillage;                // +0x10
    unsigned __int64 VillageUID;                         // +0x18
};
CTCVillageMember* Make_004dc659() { return new CTCVillageMember; }                // emits the vtable, so the ??_G and the out-of-line implicit ??1