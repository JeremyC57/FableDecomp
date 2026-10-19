// NCreatureMode::CModeDrained::OnDeactivate() -- the drain ends: a state-1 creature resumes its animation and resets its brain, a state-2 (player) creature leaves player mode 0x1f; a still-alive drainer drops its own drain mode, the drainer pointer is cleared, then the stand-mode deactivation
// Leaf: OnDeactivate
// Grade: DIFFER(ours v retail) -- register allocation only: `this` lands in edi and &PDrainer in ebx (retail ebx / edi, edi pushed after the state test); tried if/else-if and a switch on the state byte
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA7A0, 227 bytes (`ret`).
// Callees: CThingAnimatable::SetSuspendAnimation(bool, long) 0x00833A10; the creature's brain-reset forwarder 0x006D5AB0
// (`mov ecx,[ecx+1A8h] ; jmp 0x0088BB60`; manifest "_Cons_val<...>" is a byte-twin label); CThing::GetPlayer 0x004C7A30;
// CPlayer::RemoveMode 0x00634190; CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
// dtor 0x00A01C10; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, CTCCreatureModeManager =
// type 0x31, bit 17 of +0x24); CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0; CModeStandBase::OnDeactivate
// 0x00861D00 (qualified base call).
// Layout (retail, as the ctors 0x007413C0 / 0x00868220): Filter +0x68, PDrainer +0x74, DrainerMode +0x7c. Creature: state byte
// +0x10, dead bit 0 of +0x91.
// Shape: the state byte is read once into al and tested 1 / 2; IS_THING_ALIVE re-calls GetPItem (as landed 0x008C92C0); the
// pointer is cleared by assigning a CIntelligentPointer<CThing>(0) temporary (inline operator= = SetPItem(GetPItem())), which
// shares the frame slot of the interface key.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CPlayer {
    void RemoveMode(long mode);            // 0x00634190
};

struct CThing {
    unsigned char   _pad00[0x10];
    unsigned char   m_state;               // +0x10 creature state byte
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   m_flags91;             // +0x91 bit 0: dead
    CPlayer* GetPlayer();                  // 0x004C7A30
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

struct CThingCreatureBase : CThing {
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10 (CThingAnimatable)
    void ForceResetBrain();                // 0x006D5AB0
};

struct CTCCreatureModeManager {            // interface type 0x31
    void RemoveMode(unsigned long mode);   // 0x0073ABA0
};

struct CBaseIntelligentPointer {           // vptr + item
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CThing* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);           // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x60];
    void OnDeactivate();                   // 0x00861D00
};

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    void*         Group;                   // +4 (CCharString)
    unsigned long GroupCRC;                // +8
};

namespace NCreatureMode {

struct CModeDrained : CModeStandBase {     // vtable 0x01268234
    CAnimFilterOfGroup        Filter;      // +0x68
    CIntelligentPointerThing  PDrainer;    // +0x74
    unsigned long             DrainerMode; // +0x7c
    void OnDeactivate();
};

void CModeDrained::OnDeactivate()
{
    CThingCreatureBase* creature = ParentThing;
    unsigned char state = creature->m_state;
    if (state == 1) {
        creature->SetSuspendAnimation(false, 0);
        creature->ForceResetBrain();
    }
    else if (state == 2)
        creature->GetPlayer()->RemoveMode(0x1f);
    if (IS_THING_ALIVE(PDrainer)) {
        CTCCreatureModeManager* modes;
        if (PDrainer.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(DrainerMode);
        PDrainer = CIntelligentPointerThing(0);
    }
    CModeStandBase::OnDeactivate();
}

}
