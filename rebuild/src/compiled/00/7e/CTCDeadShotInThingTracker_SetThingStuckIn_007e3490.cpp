// CTCDeadShotInThingTracker::SetThingStuckIn(const CThing& thing) -- point PThingStuckIn at the thing (through an intelligent-pointer temporary) and set HasThingStuckIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3490, length 75 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (x2),
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01237F1C = CIntelligentPointer.
// PDB CTCDeadShotInThingTracker (0x18 bytes): PThingStuckIn (CIntelligentPointer<const CThing>) +0x0c,
// HasThingStuckIn +0x14. Signature from egor_signatures.tsv (`?SetThingStuckIn@CTCDeadShotInThingTracker@@QAEXABVCThing@@@Z`).
// Shape: an 8-byte stack temporary built by the inline CIntelligentPointer(item) constructor (base ctor, its
// vtable, SetPItem(item)), assigned through the inline operator= (SetPItem(other.GetPItem())), destroyed
// through the out-of-line base destructor (the derived destructor is implicit: no vtable store); the
// flag store comes after the temporary dies (statement order).
class CBaseObject;
class CThing;                                              // a CBaseObject (single inheritance, same address)

struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CThing* item = 0) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCDeadShotInThingTracker { public:
    virtual void v00();
    unsigned char       _base[0x8];                         // CTCBase: ParentThing +4, flags +8/+9
    CIntelligentPointer PThingStuckIn;                      // +0x0c CIntelligentPointer<const CThing>
    bool                HasThingStuckIn;                    // +0x14
    void SetThingStuckIn(const CThing& thing);
};

void CTCDeadShotInThingTracker::SetThingStuckIn(const CThing& thing)
{
    PThingStuckIn = CIntelligentPointer(&thing);
    HasThingStuckIn = true;
}