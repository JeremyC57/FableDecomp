// CCreatureAction_DropGenericBox::PerformAction() -- the creature drops what it carries in its primary slot (carrying component 0x46) and leaves creature mode 0x22 (mode manager 0x31)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00848D10, length 160 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00, CTCCreatureModeManager::RemoveMode(ulong)
//          @ 0x0073ABA0.
// Shape: PCreature's address is taken once (edi) for both lookups; two out-parameter GetInterface calls (void
//   template, out pointers uninitialised so the absent path reads the shared dead frame word); bit 0x46 is a byte
//   test of +0x28 (`test byte ptr [eax+28h],40h`), bit 0x31 a dword test of +0x24; the first call is reached from a
//   merged join, the second is tail-duplicated.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template <class T> __forceinline void GetInterface(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
};

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCCarrying { public:                        // interface type 0x46 component
    void RemoveThingFromPrimarySlot(bool drop);   // 0x006A1E00
};

class CTCCreatureModeManager { public:             // interface type 0x31 component
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

class CCreatureAction_DropGenericBox : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_DropGenericBox::PerformAction()
{
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    carrying->RemoveThingFromPrimarySlot(true);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x22);
}