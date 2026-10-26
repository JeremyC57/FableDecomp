// CAIStateGroup_MinionSitOnSeatBase::Cleanup() -- when the group uses a gather point and the owner is still alive,
// leave the owner's gather point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FA60, length 118 bytes (+ int3 padding). Vtable slot 9.
// Callees: this class's virtual slot 17 (+0x44, bool "uses a gather point"), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (three times on POwner at +0x34: the inlined IS_THING_ALIVE pair, then the object for the lookup),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the owner, type 0x7B = CTCGatherPointBase, bit 27 of
// the bitset dword at +0x2c), CTCGatherPointBase::RemoveUser(CThing*) @ 0x007FFDB0 with PBrain->PCreature.
// Shape: nested ifs, no return value; `POwner.GetPItem()->GetInterface(&gp, 0x7b)` re-fetches the item for the
// lookup (lea esi over the owner's map), the creature argument is loaded before the component for the call.
// Layout local (the engine header flattens the base and has no vtable).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CTCGatherPointBase { public:                     // interface type 0x7B
    void RemoveUser(CThing* user);              // 0x007FFDB0
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBase {
    unsigned char _seat[0x20];                  // +0x14 CAIStateGroup_UsesSeat members
    CIntelligentPointer<CThing> POwner;             // +0x34 CIntelligentPointer<CThing>
    virtual bool IsTimeToSit();                 // slot 14 (+0x38)
    virtual void v15();                         // slot 15
    virtual bool GetPOwnerObject(CThing** ppOwner);     // slot 16 (+0x40)
    virtual bool UsesGatherPoint();             // slot 17 (+0x44)
    virtual void GetSeat(CThing* owner);        // slot 18 (+0x48)
    virtual void Cleanup();
};

void CAIStateGroup_MinionSitOnSeatBase::Cleanup()
{
    if (UsesGatherPoint()) {
        if (IS_THING_ALIVE(POwner)) {
            CTCGatherPointBase* pGatherPoint;
            if (POwner.GetPItem()->GetInterface(&pGatherPoint, 0x7b))
                pGatherPoint->RemoveUser(PBrain->PCreature);
        }
    }
}