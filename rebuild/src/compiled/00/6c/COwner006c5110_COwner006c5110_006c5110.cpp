// COwner_006c5110::COwner_006c5110() -- default constructor of a class whose only constructed member is a holder of a
// CIntelligentPointer<CThing>; the holder's constructor takes `const CIntelligentPointer<CThing>& = CIntelligentPointer<CThing>()`
// and is expanded here, so the default-argument temporary is built in this frame and copied into the member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5110, length 69 bytes (`ret`, returns `this` in eax: a real constructor).
// Leaf: ??0COwner_006c5110
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (temporary, then the member), GetPItem
// @ 0x00A01B50 (temporary), SetPItem @ 0x00A01B90 (member), ~CBaseIntelligentPointer @ 0x00A01C10 (temporary).
// Vtable 0x01238C6C = CIntelligentPointer<CThing> (RTTI `.?AV?$CIntelligentPointer@VCThing@@@@`), stored into the temporary
// and into the member: both objects are the same instantiation, so the copy is NOT a direct `member(T())` initialiser
// (VC7.1 elides that to 18 bytes) but a copy through a const reference parameter: a holder constructor with a defaulted
// `const CIntelligentPointer<CThing>&` argument, inlined into the owning class's default constructor (the 8-byte temporary
// is built FIRST at [esp+4], the member by the pointer's copy constructor `SetPItem(other.GetPItem())`, the temporary
// destroyed through the base destructor; CIntelligentPointer<T> declares no destructor of its own).
// Label correction: the manifest's "CIntelligentPointer<const CThing>::CIntelligentPointer<const CThing>" is propagated
// (that converting constructor takes an argument and would store the `$$CBVCThing` vtable; this body takes none and stores
// the CIntelligentPointer<CThing> vtable). No caller or data reference survives (xref: 0), so the owning class and the
// holder are named descriptively (COwner_006c5110 / CThingPointerHolder_006c5110); nothing else is initialised.
class CThing;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CThingPointerHolder_006c5110 {
    CIntelligentPointer<CThing> PThing;    // +0x00
    CThingPointerHolder_006c5110(const CIntelligentPointer<CThing>& thing = CIntelligentPointer<CThing>()) : PThing(thing) { }
};

struct COwner_006c5110 {
    CThingPointerHolder_006c5110 Holder;   // +0x00
    COwner_006c5110();
};

COwner_006c5110::COwner_006c5110() { }