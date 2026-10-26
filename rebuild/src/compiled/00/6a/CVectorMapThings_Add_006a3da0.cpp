// CVectorMap_Things::Add(const unsigned long&, const CIntelligentPointer&) -- CVectorMap<unsigned long, CIntelligentPointer<CThing>>: appends a {key, thing} pair to the vector at +0 and marks the map dirty (+0xd)
// Leaf: Add
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3DA0, length 84 bytes (+12 int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10, the record vector's push_back @ 0x006A3CD0 (it copy-constructs the 12-byte record
// {key, CIntelligentPointer} in place: key word, base constructor, vtable 0x01238C6C, SetPItem(other.GetPItem())).
// Label: the manifest "CTCScriptedControl::ClearLocked" is propagated (no direct call sites). Its neighbours are the sort
// helpers of a {unsigned long, T} key-pair vector (make_heap / sift-down 0x006A3B90..0x006A3E00, CKeyPairCompareLess), and
// the shape is the CVectorMap<K,V,CKeyPairCompareLess<K,V>>::Add family of 0x006FCF40: vector {first, last, end} at +0,
// the empty Compare at +0xc, Dirty at +0xd.
// Shape: the 12-byte stack record is a pair built by an inline constructor: the key word is stored first, then the
// CIntelligentPointer copy constructor (base ctor, the source's GetPItem, the vtable store scheduled after that call,
// SetPItem); the record is passed by address to push_back and destroyed before the flag store (inner scope).
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct SKeyedThing {
    unsigned long       m_key;             // +0
    CIntelligentPointer m_thing;           // +4
    SKeyedThing(const unsigned long& key, const CIntelligentPointer& thing) : m_key(key), m_thing(thing) { }
};

struct CKeyedThingVector {
    SKeyedThing* m_pFirst;
    SKeyedThing* m_pLast;
    SKeyedThing* m_pEnd;
    void PushBack(const SKeyedThing& record);   // 0x006A3CD0
};

struct CVectorMap_Things {
    CKeyedThingVector m_records;           // +0
    unsigned char     Compare;             // +0xc (empty CKeyPairCompareLess)
    bool              Dirty;               // +0xd
    void Add(const unsigned long& key, const CIntelligentPointer& thing);
};

void CVectorMap_Things::Add(const unsigned long& key, const CIntelligentPointer& thing)
{
    {
        SKeyedThing record(key, thing);
        m_records.PushBack(record);
    }
    Dirty = true;
}