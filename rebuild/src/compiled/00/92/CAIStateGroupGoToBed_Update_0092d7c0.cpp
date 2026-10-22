// CAIStateGroup_GoToBed::Update() -- find a free bed (own, then bandit); yawn first if the creature has the anim, walk (run when a human is more than 15 units away) to the bed position, look along the bed's forward axis and mark the creature ready to sleep (CTCSleep 0xd4); always terminates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092D7C0, length 604 bytes (+ int3 padding), plain `ret`.
// Callees: FindFreeBed(bool) @ 0x0092D1F0, FindFreeBanditBed @ 0x0092C8A0, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0 ("STANDARD_YAWN"), CThingCreatureBase::HasAnim @ 0x00662C70 (CAnimFilterAnyGroup 0x0127261C /
// CAnimFilterBase 0x0122FFCC), CCreatureAction_PlayAnimation @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0,
// AIAction_PerformAction @ 0x0097D8B0, CThingCreatureBase::GetCreatureType @ 0x00662000, CThing::GetPosition @ 0x004C73D0,
// AIAction_MoveToPos @ 0x0097EC00, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingCreatureBase::GetPosition
// @ 0x006A4D00, AIAction_LookAtPos @ 0x0097D580, CVectorMap::LowerBound @ 0x0040F020, Terminate @ 0x008FCF40.
// Shape: the yawn is `failed = HasAnim(...) && PerformAction(&PlayAnimation(...)) != 2` with conditional temporaries
// (ebx flag mask 3 / 0xF); the name goes through an inline (name, filter) wrapper (landed 0x00970610).
// Layout: Ego_r.pdb CAIStateGroup_GoToBed PBed +0x14, BedPos +0x3c, Rhset +0x48 (Up, Forward +0x54); CTCSleep
// IsReadyToSleep +0x0c.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float Magnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CAnimFilterBase {                               // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {         // vtable 0x0127261C
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
struct CTCSleep {                                      // interface 0xd4
    unsigned char _pad0[0x0c];
    bool IsReadyToSleep;                               // +0x0c
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;              // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x00662C70
    long GetCreatureType() const;                      // 0x00662000
    const C3DVector& GetPosition() const;              // 0x006A4D00
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                            // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                     // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool CreatureHasAnim(const CCharString& name, const CAnimFilterBase& filter) const { return GetCreature()->HasAnim(name, filter); }
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* arrived, long movement, bool a, bool b,
                            CThing* avoid, bool c, float speed);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    void Terminate();                                  // 0x008FCF40
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;                      // +0x14
    bool IsBedSleepPointMarker;                        // +0x1c
    long SleepStartTimeVariation;                      // +0x20
    long SleepEndTimeVariation;                        // +0x24
    bool BedToRightOfCreature;                         // +0x28
    long ActionPointIndex;                             // +0x2c
    CCharString BedSide;                               // +0x30
    bool PlayEmergencyGetOutOfBed;                     // +0x34
    long BedGetOutOfBedFastAnim;                       // +0x38
    C3DVector BedPos;                                  // +0x3c
    CRightHandedSet Rhset;                             // +0x48
    bool FindFreeBed(bool allowOwned);                 // 0x0092D1F0
    bool FindFreeBanditBed();                          // 0x0092C8A0
    void Update();
};

void CAIStateGroup_GoToBed::Update()
{
    if (FindFreeBed(false) || FindFreeBanditBed()) {
        bool arrived;
        bool failed = CreatureHasAnim("STANDARD_YAWN", CAnimFilterAnyGroup())
            && AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "STANDARD_YAWN", CSoundPair(),
                   false, true, false, 0, 0)) != 2;
        if (!failed) {
            long movement = 3;
            if (GetCreature()->GetCreatureType() == 1 && (BedPos - GetCreature()->CThing::GetPosition()).Magnitude() > 15.0f)
                movement = 5;
            arrived = false;
            long result;
            do {
                result = AIAction_MoveToPos(2, BedPos, &arrived, movement, true, true, 0, true, 0.0f);
            } while (result == 4);
            if (result == 2 && PBed.GetPItem() != 0 && !(PBed.GetPItem()->m_flags91 & 1)
                && AIAction_LookAtPos(1, GetCreature()->GetPosition() + Rhset.Forward, 0x11, 0.01f) == 2
                && GetCreature()->HasTC(0xd4))
                GetCreature()->GetTC<CTCSleep>(0xd4)->IsReadyToSleep = true;
        }
    }
    Terminate();
}