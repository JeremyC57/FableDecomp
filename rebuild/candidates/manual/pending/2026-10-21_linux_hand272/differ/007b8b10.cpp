// CTCTavernTable::GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat) -- claim free ACTION_POINT_SIT points in turn until one whose seat number (the digits after the prefix) has food on the table; give up when the number is out of range
// Grade: DIFFER(408v417) -- register webs: retail keeps &pointIndex in ebp and the start index in ebx (ours swaps), loads the interface-map start into eax before copying it to the search pointer, and so aligns the loop head with a 2-byte pad; control flow, calls and frame match. Tried: search as member/free inline, startIndex scopes, named parent.
struct CTCBase;
struct CInterfaceEntry { long type; CTCBase* component; };
class CThing {
public:
    unsigned char    _pad00[0x20];
    unsigned long    m_presentBits[9];              // +0x20
    CInterfaceEntry* m_first;                       // +0x44
    CInterfaceEntry* m_last;                        // +0x48
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        CInterfaceEntry* last = m_last;
        long count = last - m_first;
        CInterfaceEntry* it = m_first;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < interfaceType) {
                it += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return (T*)(it == last || interfaceType < it->type ? last : it)->component;
    }
};
class CThingObject : public CThing { };
struct C3DVector;
struct CRightHandedSet;
struct CCharStringData {
    const char*   m_pChars;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    unsigned long Length() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThingObjectPtrVector {                      // retail STLport vector<CIntelligentPointer<CThingObject> > (12 bytes)
    CIntelligentPointer<CThingObject>* first;
    CIntelligentPointer<CThingObject>* last;
    CIntelligentPointer<CThingObject>* endOfStorage;
    unsigned long size() const { return last - first; }
    const CIntelligentPointer<CThingObject>& operator[](unsigned long i) const { return first[i]; }
};
struct CTCNamedUserActionPoints {                   // interface 0x2E
    bool GetFreeNamedActionPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, CCharString name, long& pointIndex, long startIndex = 0, bool claim = true);   // 0x007FF190
    void FreeActionPoint(long pointIndex);          // 0x007FEF40
    void GetName(long pointIndex, CCharString& name);   // 0x007FEFC0
};
struct CTCTavernTable {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    CThingObjectPtrVector Food;                     // +0x28 (PDB +0x30; retail vectors 12 bytes)
    unsigned char _pad34[0x48 - 0x34];
    long ActionPointSitStrlen;                      // +0x48 (PDB +0x5c)
    unsigned char _pad4c[0x5c - 0x4c];
    bool AvailableForUse;                           // +0x5c (PDB +0x70)
    bool GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat);
};

bool CTCTavernTable::GetEatingPoint(CThing& user, C3DVector& pos, CRightHandedSet& rh, long& pointIndex, long& seat)
{
    if (AvailableForUse) {
        long startIndex = 0;
        for (;;) {
            CTCNamedUserActionPoints* points;
            if (ParentThing->HasTC(0x2e))
                points = ParentThing->GetTC<CTCNamedUserActionPoints>(0x2e);
            if (!points->GetFreeNamedActionPoint(user, pos, rh, "ACTION_POINT_SIT", pointIndex, startIndex)) {
                pointIndex = -1;
                return false;
            }
            CCharString name;
            points->GetName(pointIndex, name);
            CCharString number = name.GetSubString(ActionPointSitStrlen, name.Length());
            long chair = GFCharStringToInt(number);
            if (Food.size() <= (unsigned long)(chair - 1) || chair - 1 < 0) {
                points->FreeActionPoint(pointIndex);
                pointIndex = -1;
                break;
            }
            if (Food[chair - 1].GetPItem()) {
                seat = chair - 1;
                return true;
            }
            points->FreeActionPoint(pointIndex);
            startIndex = pointIndex + 1;
        }
    }
    return false;
}
