// CAIStateGroup_JackDragon::WaitForAnimLoop(float seconds) -- idle a tick at a time until the current animation
// has played to 99.5% and the given number of seconds has passed; terminate the group instead when the enemy
// dies or the group is stopping.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00950950, length 221 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, 0x5a = bit 26 of +0x28, pointer preset to
// null), the animation component's current-anim accessor @ 0x0070B460 (manifest label CWorld::DrawGetEnvironment
// propagated), the brain world accessor @ 0x008FCFC0 (object expression of the frame accessor), GFGetCurrentFrame
// @ 0x0049D870, CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, IsStopping @ 0x008FD010, Terminate
// @ 0x008FCF40; the frame-rate global @ 0x01375550 (15) and the 0.995f fraction @ 0x012933FC.
// Shape: the wait in frames `(float)g_FramesPerSecond * seconds` goes to a frame slot; a do/while whose body
// idles, bails out through Terminate, and computes the elapsed frames (stored into the dead parameter slot for
// the fild) before the `fraction < 0.995f || elapsed < wait` test.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    // the same lookup with the end/hit choice written as one ternary (VC7.1 then loads the component straight
    // into the register its first use wants)
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
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
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C3DVector { float x, y, z; };
struct C2DVector {
    float x, y;
    float Length() const { return (float)sqrt(x * x + y * y); }
};

struct CThingPositioned {
    const C3DVector& GetPosition();             // 0x004C73D0
};

// CDefPointer<T>: the def object's vtable slot 1 (+4) deletes it, the intrusive count is at +4.
struct CDefPointee {
    virtual void v00();
    virtual void Delete() const;                // slot 1 (+4)
    mutable long m_refCount;                    // +0x04
    void Release() const { if (--m_refCount == 0) Delete(); }
};

template<class T> struct CDefPointer {
    T* Object;                                  // +0x00
    CDefPointer() : Object(0) { }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

struct CJackDragonDef : CDefPointee {
    unsigned char _pad08[0x5c];
    float HoverRange;                           // +0x64
};

struct CDefClassBase : CDefPointee {
    bool GetSubDef(CDefPointer<const CJackDragonDef>* out);    // 0x008E3300 GetSubDef<...>
};

struct CThingAICreatureDef {
    unsigned char _pad[0x70];
    CDefClassBase* PDef;                        // +0x70 CDefPointer<const CDefClassBase>::Object
};

struct CWorldFrame;
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PLandedMarker;      // +0x1c CIntelligentPointer<CThing>
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    CBaseIntelligentPointer PStrafeMarker1;     // +0x34
    CBaseIntelligentPointer PStrafeMarker2;     // +0x3c
    CBaseIntelligentPointer PInvisibleBodgeCreature;  // +0x44
    long FrameOfLastSlam;                       // +0x4c
    long FrameWhenLastInRange;                  // +0x50
    long FrameWhenLanded;                       // +0x54
    long FramesInSnapRange;                     // +0x58
    long CurrentState;                          // +0x5c EDragonState
    long GoLeft;                                // +0x60
    CWorldFrame* GetPFrameWorld();              // 0x008FCFC0 (GetPWorld)
    long AIAction_DoNothing();                  // 0x0097E200
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long ChooseHoverOrStrafeState();
    void WaitForAnimLoop(float seconds);
};

struct CAnimInstance {
    unsigned char _pad[0x1c];
    float m_fraction;                           // +0x1c played fraction
};
struct CTCAnim {                                // interface 0x5a
    CAnimInstance* GetPCurrentAnim();           // 0x0070B460
};

extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
struct CWorldFrame {
    long GetCurrentFrame() { return GFGetCurrentFrame(); }
};

void CAIStateGroup_JackDragon::WaitForAnimLoop(float seconds)
{
    CTCAnim* anim = 0;
    PBrain->PCreature->GetInterfaceT(&anim, 0x5a);
    CAnimInstance* current = anim->GetPCurrentAnim();
    long start = GetPFrameWorld()->GetCurrentFrame();
    float wait = (float)g_FramesPerSecond * seconds;
    long elapsed;
    do {
        AIAction_DoNothing();
        if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
            Terminate();
            return;
        }
        elapsed = GetPFrameWorld()->GetCurrentFrame() - start;
    } while (current->m_fraction < 0.995f || (float)elapsed < wait);
}
