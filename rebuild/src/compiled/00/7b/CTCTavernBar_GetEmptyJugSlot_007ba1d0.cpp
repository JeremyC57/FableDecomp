// CTCTavernBar::GetEmptyJugSlot(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& slot) -- walk the bar's jug slots and claim the first free ACTION_POINT_BARMAN_0<n> action point whose slot has no jug or a filled jug with nothing in it
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BA1D0, 494 bytes (+ int3 padding), `ret 14h`, aligned (`and esp,-8`) frame.
// Callees: CVectorMap::LowerBound 0x0040F020 (interfaces 0x2E CTCNamedUserActionPoints, 0xBB CTCJug), CBaseIntelligentPointer
//   GetPItem 0x00A01B50 / dtor 0x00A01C10, ctor 0x00A01B10, SetPItem 0x00A01B90 (vtable 0x0125A858); CCharString default 0x0099E4B0, literal 0x0099EBF0, copy 0x0099EC30, dtor
//   0x0099EAE0, operator+=(const char*) 0x0099F100, operator+=(const CCharString&) 0x0099F0A0; GFIntToCharString 0x0099F830;
//   CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190 (name by value).
// Layout: CTCTavernBar Jugs vector<CIntelligentPointer<CThingObject> > +0x0c (8-byte elements, `sar 3`); CTCJug +0x0c contents
//   pointer, +0x10 filled flag (descriptive names; landed 0x0084A180 has the +0x10 byte); strings 0x0126DCC0/0x0126DCD4 + "_0".
// Shape: GetJug is inlined here (`__forceinline`, early-out `return CIntelligentPointer(0)` then `first[index]` through a
//   named begin pointer: VC7.1 then spills `this` and reuses edi for the vector start like retail); `slot++` gives the
//   load/inc/store of the reference.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
class CThingObject : public CThing { };
class C3DVector;
class CRightHandedSet;
class CCharString { public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
    CCharString& operator+=(const char* text);      // 0x0099F100
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CJugVector {                                 // retail STLport vector<CIntelligentPointer<CThingObject> > (12 bytes)
    CIntelligentPointer<CThingObject>* first;
    CIntelligentPointer<CThingObject>* last;
    CIntelligentPointer<CThingObject>* endOfStorage;
    unsigned long size() const { return last - first; }
    const CIntelligentPointer<CThingObject>& operator[](unsigned long i) const { return first[i]; }
};
struct CTCJug {                                     // interface 0xBB
    unsigned char _pad00[0x0c];
    void* Contents;                                 // +0x0c (descriptive)
    bool Filled;                                    // +0x10 (descriptive)
};
class CTCNamedUserActionPoints { public:                   // interface 0x2E
    bool GetFreeNamedActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, CCharString name, long& pointIndex, long flags = 0, bool claim = true);   // 0x007FF190
};
class CTCTavernBar { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    CJugVector Jugs;                                // +0x0c (PDB +0x0c; retail vector 12 bytes)
    __forceinline CIntelligentPointer<CThingObject> GetJug(long index) const
    {
        if ((unsigned long)index >= Jugs.size())
            return CIntelligentPointer<CThingObject>(0);
        CIntelligentPointer<CThingObject>* first = Jugs.first;
        return first[index];
    }
    bool GetEmptyJugSlot(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& slot);
};

bool CTCTavernBar::GetEmptyJugSlot(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& slot)
{
    CTCNamedUserActionPoints* points;
    if (ParentThing->HasTC(0x2e))
        points = ParentThing->GetTC<CTCNamedUserActionPoints>(0x2e);
    for (slot = 0; (unsigned long)slot < Jugs.size(); slot++) {
        CIntelligentPointer<CThingObject> jug = GetJug(slot);
        if (jug.GetPItem()) {
            CThingObject* item = jug.GetPItem();
            if (!item->HasTC(0xbb))
                continue;
            CTCJug* tc = item->GetTC<CTCJug>(0xbb);
            if (!tc->Filled || tc->Contents)
                continue;
        }
        CCharString unused;
        CCharString name("ACTION_POINT_BARMAN");
        name += "_0";
        name += GFIntToCharString(slot + 1);
        if (points->GetFreeNamedActionPoint(user, pos, rh, name, pointIndex))
            return true;
    }
    return false;
}