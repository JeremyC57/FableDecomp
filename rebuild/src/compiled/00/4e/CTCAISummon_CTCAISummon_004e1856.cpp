// CTCAISummon::~CTCAISummon() -- destroy the component's members (list<CIntelligentPointer<CThing> >::~list 0x004B8940 (MySummonedCreatures +0x10), the PSummonDef (+0x0C) release inline), then the CTCBase part
// Leaf: ??1CTCAISummon@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1856, 40 bytes, thiscall, tail-jumps to ~CTCBase 0x00686830.
// Identity (proven): its only caller is the class's scalar deleting destructor 0x004E183A (landed, with the stand-in callee name
//   ?CTCAISummon_Destruct@@YIXPAX@Z); RTTI .?AVCTCAISummon@@ : CTCBase; the destroyed members are Ego_r.pdb CTCAISummon's at their retail offsets.
// Callees: list<CIntelligentPointer<CThing> >::~list 0x004B8940 (MySummonedCreatures +0x10), the PSummonDef (+0x0C) release inline; ~CTCBase 0x00686830 (tail jump).
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
class CSummonDef;
class CBaseIntelligentPointer {                        // 8 bytes: vptr, PData
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport, 4 bytes
public:
    ~list();                                             // 0x004B8940 for list<CIntelligentPointer<CThing> >
    void* _M_node;
};
}
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb); engine/CTCBase.h has no virtual destructor
public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCAISummon : public CTCBase {                             // the destructor is the implicit one
public:
    CDefPointer<const CSummonDef> PSummonDef;            // +0x0C
    std::list<CIntelligentPointer<CThing> > MySummonedCreatures;   // +0x10
    long LastSummonTick;                                 // +0x14
};
CTCAISummon* Make_004e1856() { return new CTCAISummon; }                // emits the vtable, so the ??_G and the out-of-line implicit ??1