// Grade: DIFFER(749v761) -- every instruction lines up except: VC7.1 drops the CAnimFilterAnyGroup temporary's base-vtable reset (retail `mov [esp+1Ch],0x0122FFCC` after GetRandomAnimFromNameWithFilter) and packs the frame 4 bytes tighter (0xF0 v 0xF4: retail shares the HasTC/reached byte word at +0x14 and keeps the filter/mode slot at +0x18)
// CAIStateGroup_SitRoundCampfire::Update() -- sit down at a live campfire: take a free action point (position + orientation) at it, walk there, look along the point's forward direction, pick a random "sit" animation (any group) and sit down in a floor-sitting mode; once seated stay a random number of frames (doing nothing) and terminate; any failure terminates at once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009392A0, length 761 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCampfire +0x14), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface 0x2D CTCUserActionPointBase, bit 13 of +0x24), CTCUserActionPointBase::GetFreeActionPoint @ 0x007FEDE0,
// AIAction_MoveToPos @ 0x0097EC00, IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0078B2A0 (__fastcall),
// CThingCreatureBase::GetPosition @ 0x006A4D00, AIAction_LookAtPos @ 0x0097D580, CThingCreatureBase::
// GetRandomAnimFromNameWithFilter @ 0x00662EF0 (a CAnimFilterAnyGroup temporary, vtable 0x0127261C, base CAnimFilterBase
// 0x0122FFCC), operator new @ 0x00BFEA1A, CDefString::operator CCharString @ 0x00415DD0 / ~CCharString @ 0x0099EAE0,
// NCreatureMode::CModeSitFloor ctor @ 0x008AC3C0, CCountedPointer<CModeSitBase> ctor @ 0x008AB380 / copy @ 0x008AA9B0 /
// dtor @ 0x008AB3D0, CCreatureAction_SitDown ctor @ 0x008AB9D0 / dtor @ 0x008ABAA0, AIAction_PerformAction @ 0x0097D8B0,
// GetPThingWorld @ 0x008FCFA0, GetPWorldSeed @ 0x008FCFD0, GFRandom @ 0x004CDAE0 (__fastcall), GetWorldFrame @ 0x0049D870,
// AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Data: the sit animation name CCharString @ 0x013BB6EC, the stay range @ 0x013BB6F8 and minimum @ 0x013BB6F4; look speed
// 0.0027777f (immediate).
// Layout: Ego_r.pdb CAIStateGroup_SitRoundCampfire: PCampfire +0x14, SeatPosIndex +0x1c, GotSeatPos +0x20, Sitting +0x21;
// retail CCreatureActionBase is 0xa8 bytes (SitDown keeps its counted mode at +0xa8); CAnimationEntry +0x08 the group name.
// Shape: the presence test reads the bitset word into a named local (no byte narrowing); the look-at point is the forward
// copy plus the creature position through the value constructor (z, y, x); the mode's group-name conversion is a
// conditional temporary inside the new-expression (the flag word zeroed at entry).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present;
    }
};
struct CCharString {
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct CDefString {
    long m_index;
    operator CCharString() const;               // 0x00415DD0
};
struct CAnimationEntry {
    unsigned char _pad[0x08];
    CDefString GroupName;                       // +0x08 (descriptive)
};
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {  // vtable 0x0127261C
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
    const CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662EF0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IsThingAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& p);   // 0x0078B2A0
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThingCreatureBase* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
};
struct CModeSitBase {
    virtual void v00();
    unsigned char _body[0x28];
};
struct CAnimFilterOfGroup {
    virtual void v00();
    unsigned char _body[0x08];
};
struct CModeSitFloor : CModeSitBase {           // 0x38 bytes
    CAnimFilterOfGroup Filter;                  // +0x2c
    CModeSitFloor(CThing* thing, const CCharString& group);   // 0x008AC3C0
};
struct CCountBlock;
struct CCountedPointerModeSit {                 // CCountedPointer<NCreatureMode::CModeSitBase>
    CModeSitBase* Object;
    CCountBlock* Block;
    CCountedPointerModeSit(CModeSitBase* object);                    // 0x008AB380
    CCountedPointerModeSit(const CCountedPointerModeSit& other);     // 0x008AA9B0
    ~CCountedPointerModeSit();                                       // 0x008AB3D0
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCountedPointerModeSit PMode;               // +0xa8
    CCreatureAction_SitDown(CThingCreatureBase* owner, const CAnimationEntry* animation, CCountedPointerModeSit mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
extern CCharString g_SitAnimName;               // 0x013BB6EC (descriptive)
extern unsigned long g_SitStayRange;            // 0x013BB6F8 (descriptive)
extern unsigned long g_SitStayMinimum;          // 0x013BB6F4 (descriptive)
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CWorld {
    static unsigned long GetWorldFrame();       // 0x0049D870
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CThingAI;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movementType, bool a, bool b,
                            CThing* leader, bool c, float d);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
    const CAnimationEntry* GetRandomCreatureAnim(const CCharString& name, const CAnimFilterBase& filter)
    {
        return GetCreature()->GetRandomAnimFromNameWithFilter(name, filter);
    }
};
struct CAIStateGroup_SitRoundCampfire : CAIStateGroupBase {
    CBaseIntelligentPointer PCampfire;          // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    virtual void Update();
};

void CAIStateGroup_SitRoundCampfire::Update()
{
    if (IsThingAlive(PCampfire)) {
        CTCUserActionPointBase* points;
        PCampfire.GetPItem()->GetInterface(&points, 0x2d);
        C3DVector pos;
        CRightHandedSet orientation;
        if (points->GetFreeActionPoint(GetCreature(), &pos, &orientation, &SeatPosIndex)) {
            GotSeatPos = true;
            bool reached;
            long result;
            do {
                if (!IsThingAlive(PCampfire)) {
                    Terminate();
                    return;
                }
                result = AIAction_MoveToPos(2, pos, &reached, 3, true, true, 0, true, 0.0f);
            } while (result == 4);
            if (result == 2 && IS_THING_ALIVE(PCampfire)) {
                C3DVector facing = orientation.Forward;
                C3DVector lookAt = facing + GetCreature()->GetPosition();
                do {
                    if (!IsThingAlive(PCampfire)) {
                        Terminate();
                        return;
                    }
                    result = AIAction_LookAtPos(2, lookAt, 0x11, 0.0027777f);
                } while (result == 4);
                if (result == 2 && IS_THING_ALIVE(PCampfire)) {
                    const CAnimationEntry* anim = GetRandomCreatureAnim(g_SitAnimName, CAnimFilterAnyGroup());
                    CCountedPointerModeSit mode(new CModeSitFloor(GetCreature(), anim->GroupName));
                    Sitting = true;
                    if (AIAction_PerformAction(&CCreatureAction_SitDown(GetCreature(), anim, mode)) != 2) {
                        Terminate();
                        return;
                    }
                    unsigned long endFrame = GetPThingWorld()->GetWorldFrame() + GFRandom(g_SitStayRange, GetPWorldSeed()) + g_SitStayMinimum;
                    while (GetPThingWorld()->GetWorldFrame() < endFrame && AIAction_DoNothing() == 4)
                        ;
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
