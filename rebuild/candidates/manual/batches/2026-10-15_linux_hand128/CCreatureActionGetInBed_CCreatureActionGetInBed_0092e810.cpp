// CCreatureAction_GetInBed::CCreatureAction_GetInBed(const CCreatureAction_GetInBed&) -- copy constructor: base copy, bed pointer re-pointed, the two animation names and the two animation ids copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E810, length 127 bytes, `ret 4`. Vtable 0x01291BFC.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C),
//          CCharString::CCharString(const CCharString&) @ 0x0099EC30 (out of line: the shared fable_string.h inlines it,
//          so CCharString is modelled locally).
// Shape: the DowseFire copy shape (0x0084C540) for PBed, then each string member copy-constructed from the source's
//   (`lea eax,[edi+N] ; push eax ; lea ecx,[esi+N]`), then the two longs.
// Layout: Ego_r.pdb puts PBed/AnimGroup/SleepAnimName/BedGetInAnim/BedSleepAnim at +0xb4..+0xc8 (PDB
//   CCreatureActionBase 0xb4 bytes, retail 0xa8): +0xa8..+0xbc here; engine/CCreatureAction_GetInBed.h is not used.
struct CBaseObject;
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

struct CCreatureAction_GetInBed : CCreatureActionBase {   // vtable 0x01291BFC
    CIntelligentPointer<CThing> PBed;       // +0xa8 (PDB +0xb4)
    CCharString AnimGroup;                  // +0xb0
    CCharString SleepAnimName;              // +0xb4
    long BedGetInAnim;                      // +0xb8
    long BedSleepAnim;                      // +0xbc
    virtual void v00();
    CCreatureAction_GetInBed(const CCreatureAction_GetInBed& other);
};

CCreatureAction_GetInBed::CCreatureAction_GetInBed(const CCreatureAction_GetInBed& other)
    : CCreatureActionBase(other)
    , PBed(other.PBed)
    , AnimGroup(other.AnimGroup)
    , SleepAnimName(other.SleepAnimName)
    , BedGetInAnim(other.BedGetInAnim)
    , BedSleepAnim(other.BedSleepAnim)
{
}
