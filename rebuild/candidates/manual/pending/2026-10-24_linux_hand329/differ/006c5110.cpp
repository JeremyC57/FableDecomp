// CThingPointerHolder_006c5110::CThingPointerHolder_006c5110() -- default constructor whose CIntelligentPointer<CThing> member is copy-initialised from a default-constructed temporary
// Grade: DIFFER(67v69) -- only retail's `mov eax,esi` return of `this` is missing: the best form is the default-constructor
// closure (??_F) of a holder whose constructor takes `const CIntelligentPointer<CThing>& = CIntelligentPointer<CThing>()`
// (a void closure); a plain `PThing(CIntelligentPointer<CThing>())` initialiser or a novtable base form ELIDES the copy (18 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5110, length 69 bytes.
// Leaf: ??_F
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (temporary, then `this`), GetPItem
// @ 0x00A01B50 (temporary), SetPItem @ 0x00A01B90 (`this`), ~CBaseIntelligentPointer @ 0x00A01C10 (temporary).
// Vtable 0x01238C6C = CIntelligentPointer<CThing> (stored into the temporary and into `this`).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer<const CThing>" is
// propagated (that converting constructor takes an argument; this body takes none and returns `this` with a
// plain `ret`). No caller or data reference survives (xref: 0), so the owning class is unknown and the name is
// descriptive; the shape is the STLport-style `member(T())` initialiser: an 8-byte frame temporary built FIRST,
// the member built by the pointer's copy constructor (`SetPItem(other.GetPItem())`), the temporary destroyed
// through the base destructor (CIntelligentPointer<T> declares no destructor of its own). Nothing else is
// initialised, so the holder has no further constructed members.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

struct CThingPointerHolder_006c5110 {
    CIntelligentPointer<CThing> PThing;    // +0x00
    CThingPointerHolder_006c5110(const CIntelligentPointer<CThing>& thing = CIntelligentPointer<CThing>()) : PThing(thing) { }
};

CThingPointerHolder_006c5110* MakeArray(int n) { return new CThingPointerHolder_006c5110[n]; }

