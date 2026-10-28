// CAIStateGroup_MinionSitAtSchool::IsStateGroupStillValid() -- the school component's teacher is still alive.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FCD0, length 75 bytes (+ int3 padding). Vtable slot 14
// of CAIStateGroup_MinionSitAtSchool (0x0128B674).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0x23 = bit 3 of the bitset
// byte at +0x24), IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>> @ 0x007EE540 (__fastcall, the
// component's pointer member at +4).
// Shape: `GetInterface(...) && IS_THING_ALIVE(pSchool)` -- the `&&` of two bools returns through
// a full register (`mov eax,1` / `xor eax,eax`) with both misses sharing the false tail. Frame: one `push ecx`.
class CTCVillage;
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
class CTCBase;
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the shape of the landed
// GetInterface_0x.. bodies, inlined into the AI bodies through GetTC<T>().
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
    const C3DVector& GetPos() const;    // 0x004C73D0 (`[this+60h]+0Ch`)
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
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

class CWorld;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0 (PBrain->+0x1c->+0x24)
public:
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 (world random seed)
    void Terminate();                           // 0x008FCF40 (Terminated = true)
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200 (EAIActionReturn)
};

struct CTCSchool;                               // interface type 0x23 component

// Out-of-line IS_THING_ALIVE<...> @ 0x007EE540: __fastcall, the component pointer itself in ecx (retail loads the
// map entry's component straight into ecx), a full-register result (`test eax,eax`).

struct CAIStateGroup_MinionSitAtSchool : CAIStateGroupBase {
    virtual bool IsStateGroupStillValid();
};

bool CAIStateGroup_MinionSitAtSchool::IsStateGroupStillValid()
{
    CTCSchool* pSchool;
    return PBrain->PCreature->GetInterface(&pSchool, 0x23) && ((long)(((CTCVillageMember*)(pSchool))->GetPVillage()));
}