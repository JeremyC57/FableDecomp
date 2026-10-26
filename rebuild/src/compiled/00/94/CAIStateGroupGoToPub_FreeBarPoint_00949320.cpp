// CAIStateGroup_GoToPub::FreeBarPoint() -- hand the bar point this creature was using back to the bar.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00949320, length 93 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the bar pointer at +0x1c), CVectorMap::LowerBound @
// 0x0040F020 (the inlined interface lookup, type 0xB9 = bit 25 of the bitset dword at +0x34), then the bar
// component's FreeBarPoint(long) @ 0x007BA4F0 (manifest "CTCActionUseAcceptBoast::OnActionUse" is propagated
// -- it is the interface-0xB9 component's method taking the point index), and the index at +0x74 is reset to -1.
//
// Shape: the component pointer is an UNINITIALISED local filled by the inlined GetInterface: on the hit path it
// is the lookup result in eax, on the miss path retail reads the never-written slot (`mov eax,[esp+4]`, shared
// with the key temporary of the hit path -- nested-scope locals share one frame slot). The frame is one
// `push ecx`. No engine header exists for CAIStateGroup_GoToPub; the layout is local.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the same shape the landed
// GetInterface_0x.. bodies (0x007BEA08 ...) use, here inlined into the caller through GetTC<T>().
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCBarPoints {                           // interface type 0xB9 component
    void FreeBarPoint(long pointIndex);         // 0x007BA4F0
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

class CAIStateGroupBase { public:
    virtual void v00();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    unsigned char _pad14[8];                    // +0x14 CIntelligentPointer (the pub building)
    CIntelligentPointer<CThing> m_pBar;             // +0x1c CIntelligentPointer<CThing> (the bar object)
    unsigned char _pad24[0x50];                 // +0x24
    long m_barPointIndex;                       // +0x74 (-1 = none)
    void FreeBarPoint();
};

void CAIStateGroup_GoToPub::FreeBarPoint()
{
    CTCBarPoints* pBarPoints;
    m_pBar.GetPItem()->GetInterface(&pBarPoints, 0xB9);
    pBarPoints->FreeBarPoint(m_barPointIndex);
    m_barPointIndex = -1;
}