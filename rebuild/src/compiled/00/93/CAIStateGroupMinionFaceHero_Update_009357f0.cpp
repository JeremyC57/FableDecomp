// CAIStateGroup_MinionFaceHero::Update() -- for up to six seconds while the hero lives: look at the hero, and stop once the hero (perceived in extended line of sight) is within 2.2m; idle a tick between looks; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009357F0, length 384 bytes (both exits are `jmp Terminate` tail calls).
// Callees: GFGetCurrentFrame @ 0x0049D870 (__fastcall, no object), CBaseIntelligentPointer ctor/GetPItem/SetPItem
//          (PHero +0x14), CAIStateGroupBase::AIAction_LookAtThing @ 0x0097D6F0 (CIntelligentPointer<const CThing> by
//          value, vtable 0x01237F1C), the inlined STLport lower_bound of the interface map (0x34 = CTCPerceiveThing, bit 20
//          of +0x24), CTCPerceiveThing::IsPlayerExtendedLineOfSight @ 0x00744830, CThing::GetPos @ 0x004C73D0,
//          AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Globals: FramesPerSecond @ 0x01375550, 4.84f (2.2 squared) @ 0x012922D4.
// Shape: FramesPerSecond is read into a named local at the top of each pass (before the frame call) and the elapsed
//          test is unsigned (`ja`) against it * 6; the dead-hero / timeout exit is the loop's own exit and the other exits
//          are inner `Terminate(); return;`s (two separate tails); IS_THING_ALIVE reads the dead bit through an inline
//          CThing::IsDead and the interface map's lower_bound is __forceinline; the distance is `creature - hero` built member-wise, squared as one
//          expression (VC7.1 sums Z first).
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04), the hero
//          intelligent pointer at +0x14.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __forceinline SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte entries (inlined in this TU)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector operator-(const C3DVector& v) const { C3DVector r; r.X = X - v.X; r.Y = Y - v.Y; r.Z = Z - v.Z; return r; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    const C3DVector& GetPos() const;             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCPerceiveThing { public:                       // interface 0x34
    bool IsPlayerExtendedLineOfSight();         // 0x00744830
};
unsigned long __fastcall GFGetCurrentFrame();   // 0x0049D870
extern unsigned long FramesPerSecond;           // 0x01375550

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
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionFaceHero : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14
    void Update();
};

void CAIStateGroup_MinionFaceHero::Update()
{
    unsigned long start = GFGetCurrentFrame();
    while (IS_THING_ALIVE(PHero)) {
        unsigned long fps = FramesPerSecond;
        if (GFGetCurrentFrame() - start > fps * 6)
            break;
        if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(PHero), 0xf, 0.0027777f) != 2) {
            Terminate();
            return;
        }
        CTCPerceiveThing* perceive;
        if (GetCreature()->GetInterface(&perceive, 0x34) && perceive->IsPlayerExtendedLineOfSight()
            && IS_THING_ALIVE(PHero)) {
            CThing* creature = GetCreature();
            const C3DVector& hero = PHero.GetPItem()->GetPos();
            const C3DVector& me = creature->GetPos();
            float dx = me.X - hero.X;
            float dy = me.Y - hero.Y;
            float dz = me.Z - hero.Z;
            if (dx * dx + dy * dy + dz * dz < 4.84f) {
                Terminate();
                return;
            }
        }
        if (AIAction_DoNothing() != 4) {
            Terminate();
            return;
        }
    }
    Terminate();
}