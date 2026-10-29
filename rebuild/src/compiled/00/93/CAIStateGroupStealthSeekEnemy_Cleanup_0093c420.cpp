// CAIStateGroup_StealthSeekEnemy::Cleanup() -- reset the creature's interface-0x45 state (mode 8, full weight, 0xFF),
// and if the enemy is still alive with interface 0x36, tell that component the creature stopped seeking it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C420, length 180 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined lookups: type 0x45, bit 5 of the dword at +0x28; type 0x36,
// bit 22 of +0x24), the 0x45 component's setter @ 0x0071AAB0 (8, 0, 0, 1.0f, 0xFF), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (IS_THING_ALIVE, then the thing), the 0x36 component's RemoveSeeker @ 0x00747B80.
// Shape: both lookups are the out-parameter GetInterface whose entry pick is one end/hit ternary (the 0x45 component
// lands in eax, `mov ecx,eax` after the pushes; the 0x36 component loads straight into ecx between the two brain
// loads); IS_THING_ALIVE inlined. The engine header flattens the base, so the class is modelled locally; PEnemy is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
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

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCStealthState {                        // the interface-0x45 component
    void SetState(long mode, long a, long b, float weight, long alpha);   // 0x0071AAB0
};
struct CTCSeekTarget {                          // the interface-0x36 component
    void RemoveSeeker(CThing* seeker, long flags);  // 0x00747B80
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;            // +0x14
    virtual void Cleanup();
};

void CAIStateGroup_StealthSeekEnemy::Cleanup()
{
    CTCStealthState* stealth;
    if (PBrain->PCreature->GetInterface(&stealth, 0x45))
        stealth->SetState(8, 0, 0, 1.0f, 0xff);
    if (IS_THING_ALIVE(PEnemy)) {
        CThing* enemy = PEnemy.GetPItem();
        CTCSeekTarget* seek;
        if (enemy->GetInterface(&seek, 0x36))
            seek->RemoveSeeker(PBrain->PCreature, 0);
    }
}