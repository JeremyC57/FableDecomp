// CTCSolidToThing::IsSolidToThing(const CThing* thing) const -- true when the not-solid-to list is empty or does not hold the thing (a linear find of an intelligent pointer to it)
// Leaf: ?IsSolidToThing@CTCSolidToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00770DE0, 105 bytes (+7 int3), `ret 4`.
// Name: FableWin.pdb `?IsSolidToThing@CTCSolidToThing@@QBE_NPBVCThing@@@Z`; the landed CTCSolidToThing::AddThingNotSolidTo
//   0x00770FA0 calls it at this address; other callers are the collision filters 0x006A8300 / 0x00857250 and
//   CTCPhysicsStandard::IsCollisionWithThingToCauseResponse 0x008E5750 (tail jump). Label correction: catalog
//   "ContainerFindElement" -> CTCSolidToThing::IsSolidToThing.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer
//   0x00A01C10; STLport __find(first, last, const CIntelligentPointer<const CThing>&, const random_access_iterator_tag&)
//   0x00737880 (/Gr __fastcall, landed).
// Layout: Ego_r.pdb CTCSolidToThing: NotSolidToThings (vector<CIntelligentPointer<CThing const> >, retail 12-byte STLport)
//   +0x0c. CIntelligentPointer<CThing const> vtable 0x01237F1C (the landed AddThingNotSolidTo model).
// Shape: the empty list answers true first; the temporary intelligent pointer is built for the find (base constructor,
//   derived vtable, SetPItem) and destroyed at the end of that statement, so end() is re-read after the destructor call.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)thing); }
};

namespace std {
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
CIntelligentPointer* __fastcall __find(CIntelligentPointer* first, CIntelligentPointer* last, const CIntelligentPointer& value,
                                       const random_access_iterator_tag&);   // 0x00737880
inline CIntelligentPointer* find(CIntelligentPointer* first, CIntelligentPointer* last, const CIntelligentPointer& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}
}

struct CThingPointerVector {                                // retail std::vector: first, last, end
    CIntelligentPointer* m_pFirst;
    CIntelligentPointer* m_pLast;
    CIntelligentPointer* m_pEnd;
    CIntelligentPointer* begin() const { return m_pFirst; }
    CIntelligentPointer* end() const { return m_pLast; }
    bool empty() const { return m_pFirst == m_pLast; }
};

class CTCSolidToThing {
public:
    void*               __vftable;                          // +0x00
    CThing*             ParentThing;                        // +0x04
    bool                ToBeRemoved;                        // +0x08
    bool                IsInGlobalUpdateTCs;                // +0x09
    CThingPointerVector NotSolidToThings;                   // +0x0c
    bool IsSolidToThing(const CThing* thing) const;
};

bool CTCSolidToThing::IsSolidToThing(const CThing* thing) const
{
    if (NotSolidToThings.empty())
        return true;
    CIntelligentPointer* it = std::find(NotSolidToThings.begin(), NotSolidToThings.end(), CIntelligentPointer(thing));
    return it == NotSolidToThings.end();
}