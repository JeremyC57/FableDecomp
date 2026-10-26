// CAIStateGroup_NymphFloatAround::OnReplace() -- kill the live float target (+0x1c) and clear it, and stop the nymph
// component's flying-around; a default CBrainOutput local is built and released around it (unused).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C130, length 235 bytes (+ int3 padding).
// Callees: CBrainOutput::CBrainOutput @ 0x00835620, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool)
// @ 0x004C9B80, the converting CIntelligentPointer temporary from 0 (vtable 0x01238C6C): ctor @ 0x00A01B10, SetPItem
// @ 0x00A01B90 (twice), dtor @ 0x00A01C10; CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xF5 =
// CTCNymph, bit 21 of the dword at +0x3c), CTCNymph::StopFlyingAround(long) @ 0x0080F870, then the implicit
// ~CBrainOutput inlined: ~CBaseIntelligentPointer @ 0x00A01C10 on +0x40, the CCountedPointer block release at +0x20
// (count, __fastcall destroy, operator delete @ 0x00BFE9BC).
// Shape: the landed AIAction_PumpCombatActions (0x0097D980) CBrainOutput model with its inline counted-pointer dtor.
// No engine header for this class; PFloatTarget is +0x1c.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
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

struct CCountBlock {                            // CCountedPointer's shared block
    long Count;                                 // +0x00
    void (__fastcall* Destroy)(void* object);   // +0x04
    void* Object;                               // +0x08
};

struct CCountedPointer {                        // CCountedPointer<..> {object, block}
    void* Object;                               // +0x00
    CCountBlock* Block;                         // +0x04
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};

class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;                        // +0x00
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;        // +0x40
    CBrainOutput();                             // 0x00835620
};

struct CThingKill {
    void Kill(bool silent);                     // 0x004C9B80
};
class CTCNymph { public:
    void StopFlyingAround(long mode);           // 0x0080F870
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIStateGroup_NymphFloatAround : CAIStateGroupBase {
    unsigned char _pad_0x14[0x8];               // +0x14
    CIntelligentPointer<CThing> PFloatTarget;      // +0x1c
    virtual void OnReplace();
};

void CAIStateGroup_NymphFloatAround::OnReplace()
{
    CBrainOutput output;
    if (IS_THING_ALIVE(PFloatTarget)) {
        ((CThingKill*)PFloatTarget.GetPItem())->Kill(true);
        PFloatTarget = 0;
    }
    CTCNymph* nymph;
    if (PBrain->PCreature->GetInterface(&nymph, 0xf5))
        nymph->StopFlyingAround(-1);
}