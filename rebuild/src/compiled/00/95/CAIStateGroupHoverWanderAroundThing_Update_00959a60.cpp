// CAIStateGroup_HoverWanderAroundThing::Update() -- three times: pick a wander position within 5m of the live thing, look at the thing, then hover to the position facing it (random movement type 8..10), terminating on any failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00959A60, length 400 bytes (vtable 0x0128D1BC slot 12).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 7 = CTCCreatureNavigation, bit 7 of +0x20 as a
//          bitset::test byte), CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/0x00A01B50/0x00A01B90,
//          CThing::GetPos @ 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CAIStateGroupBase::AIAction_LookAtThing @ 0x0097D6F0 (CIntelligentPointer<const CThing> by value, vtable
//          0x01237F1C), CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890,
//          virtual CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos (slot 22, 0x0091D7F0),
//          CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the navigation out pointer shares its home with the lookup key (written only on a hit); the loop counter is
//          spilled; the world-seed Random is the *seed form; the move result loop re-tests the thing before each retry.
// Layout: the generated engine/CAIStateGroup_HoverWanderAroundThing.h spells the vptr as a data member, so the class is
//          modelled locally with a polymorphic base (PBrain +0x04, PThing intelligent pointer +0x1c as in the header):
//          the function makes a virtual call through slot 22.
class CTCBase;
class CThingCreatureBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class C3DVector { public:
    float X, Y, Z;
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};

class CTCCreatureNavigation { public:                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11
    virtual void Update();                      // slot 12
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorld()->GetPRandomSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    unsigned char _pad_0x14[0x8];               // +0x14
    virtual long AIAction_MoveToPosAndFacePos(long repeat, const C3DVector& pos, const C3DVector& face,
                                              bool& reached, long movement);   // slot 22 (0x0091D7F0)
};

struct CAIStateGroup_HoverWanderAroundThing : CAIStateGroup_HoverMovementBase {
    CBaseIntelligentPointer PThing;             // +0x1c CIntelligentPointer<CThingGameObject const>
    virtual void Update();
};

void CAIStateGroup_HoverWanderAroundThing::Update()
{
    for (long i = 0; i != 3; ++i) {
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterfaceT(&navigation, 7);
        C3DVector target;
        if (IS_THING_ALIVE(PThing)
            && navigation->GetNearbyWanderPosition(PThing.GetPItem()->GetPos(), 5.0f, &target, false)) {
            if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(PThing), 0x11, 0.05f) != 2) {
                Terminate();
                return;
            }
            long movement = Random(3) + 8;
            bool reached;
            long result;
            do {
                if (!IS_THING_ALIVE(PThing)) {
                    Terminate();
                    return;
                }
                result = AIAction_MoveToPosAndFacePos(2, target, PThing.GetPItem()->GetPos(), reached, movement);
            } while (result == 4);
            if (result != 2) {
                Terminate();
                return;
            }
        }
    }
}