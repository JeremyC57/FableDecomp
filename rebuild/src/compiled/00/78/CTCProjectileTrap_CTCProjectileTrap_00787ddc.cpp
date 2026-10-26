// CTCProjectileTrap::CTCProjectileTrap(CThing* thing) -- CTCTrapBase(thing), shot / idle timers -1, no shots this round, own vtable, an empty target intelligent pointer
// Leaf: ??0CTCProjectileTrap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787DDC, length 54 bytes; size TU (unaligned entry, pass-through
// `push dword ptr [esp+0Ch]`, `or dword ptr [..],-1` / `and dword ptr [..],0` immediates): `#pragma optimize("s", on)`.
// Callees: CTCTrapBase::CTCTrapBase(CThing*) @ 0x0078918D, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10.
// Vtables 0x0126B90C = CTCProjectileTrap, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCProjectileTrap (0x80) less 0xc bytes: CTCTrapBase holds three std::vectors (HitThings, ImmuneThings,
// TriggerThings) that are 12 bytes in retail, so CTCTrapBase is 0x60 and ShotTimer +0x60, IdleTimer +0x64,
// NumShotsThisRound +0x68, PTarget (CIntelligentPointer<CThing>) +0x6c. engine/CTCTrapBase.h follows the PDB offsets, so the
// base is modelled locally as an opaque 0x60-byte polymorphic base.
// Shape: the three scalar initialisers are scheduled BEFORE the derived vtable store (plain init-list members in this
// module), the intelligent pointer after it (out-of-line base constructor + inline vtable store through `edi`).
#pragma optimize("s", on)
class CThing;

struct CTCTrapBase_Calls {                                  // retail 0x60 bytes
    virtual void v00();
    unsigned char _body[0x5c];                              // +0x04 CTCBase, HitThings .. TrapType
    CTCTrapBase_Calls(CThing* thing);                       // 0x0078918D
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // vtable 0x01238C6C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

struct CTCProjectileTrap : CTCTrapBase_Calls {              // vtable 0x0126B90C
    long                ShotTimer;                          // +0x60
    long                IdleTimer;                          // +0x64
    long                NumShotsThisRound;                  // +0x68
    CIntelligentPointer<CThing> PTarget;                            // +0x6c
    virtual void v00();
    CTCProjectileTrap(CThing* thing);
};

CTCProjectileTrap::CTCProjectileTrap(CThing* thing)
    : CTCTrapBase_Calls(thing)
    , ShotTimer(-1)
    , IdleTimer(-1)
    , NumShotsThisRound(0)
    , PTarget()
{
}