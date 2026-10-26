// CTCVillageMember::vfunc_14() -- CTCBase slot-14 override: when the member still has its village thing, have the village's CTCVillage component (interface 34) remove the owner thing
// Retail: Fable.exe @ 0x007EE4D0, 106 bytes; CTCVillageMember vtable 0x0124007C slot 14 (CTCBase slot, real name unknown).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//          (inlined GetInterface: interface 34 = bit 2 of the bitset word at CThing+0x24); CTCVillage::
//          RemoveThingAndUpdateCameraPoints(CThing*) 0x0065B6DD.
// Layout: CTCVillageMember: ParentThing +0x04, PVillage (CIntelligentPointer<CThing>) +0x10; retail CThing: bitset +0x20,
//         interface map +0x44.
// Shape: the out-pointer GetInterface with its result ignored: the component local is UNinitialised in the `push ecx`
//        slot (shared with the lookup key) and the call follows on both arms, each with its own epilogue.
class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
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

class CTCVillage { public:
    void RemoveThingAndUpdateCameraPoints(CThing* thing);   // 0x0065B6DD
};

class CTCVillageMember { public:
    virtual void v00();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad_0x08[0x8];
    CIntelligentPointer<CThing> PVillage;           // +0x10
    void vfunc_14();
};

void CTCVillageMember::vfunc_14()
{
    if (PVillage.GetPItem()) {
        CTCVillage* village;
        PVillage.GetPItem()->GetInterface(&village, 34);
        village->RemoveThingAndUpdateCameraPoints(ParentThing);
    }
}