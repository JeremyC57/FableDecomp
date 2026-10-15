// CAIStateGroup_MazeShieldedChase::Cleanup() -- clear the target pointer held by the creature's interface-8 (targeting) component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954900, length 124 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 8, `mov ecx,[eax+20h] ; test ch,1`),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10. Vtable
// 0x0123E7D8 = CIntelligentPointer<CThingCreatureBase>.
//
// Shape: the out-parameter GetInterface with its result ignored (the arms join in esi; the miss arm reads the
// never-written local from the key slot), then `component->m_target = CIntelligentPointer<CThingCreatureBase>(0)`
// through the stack temporary that shares the key's 8-byte frame slot (base ctor, push 0, vtable, SetPItem; copied
// with SetPItem(GetPItem()) into the member at +0x40; base destructor). The component class (interface type 8) is
// not identified: descriptive names. The engine header for the state group is flattened data-only: local model.
class CTCBase;
class CThingCreatureBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
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

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

struct CTCTargeting {                           // interface type 8 (descriptive)
    unsigned char _pad[0x40];
    CIntelligentPointer<CThingCreatureBase> m_target;   // +0x40 (descriptive)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_MazeShieldedChase : CAIStateGroupBase {
    void Cleanup();
};

void CAIStateGroup_MazeShieldedChase::Cleanup()
{
    CTCTargeting* targeting;
    PBrain->PCreature->GetInterface(&targeting, 8);
    targeting->m_target = CIntelligentPointer<CThingCreatureBase>(0);
}