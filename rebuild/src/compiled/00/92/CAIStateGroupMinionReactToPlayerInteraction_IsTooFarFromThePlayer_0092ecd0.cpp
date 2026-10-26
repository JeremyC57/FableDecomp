// CAIStateGroup_MinionReactToPlayerInteraction::IsTooFarFromThePlayer(const CThing* player) -- is the player more
// than 3 units (squared distance against a function-static 9.0) from the creature.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092ECD0, length 111 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPosition() const @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated) on
// PBrain->PCreature and on the argument. Globals: the function-static float @ 0x013BB6B8 with its init-guard
// byte @ 0x013BB6BC (bit 0): the static is initialised at first call from a `const float` product the front end
// does not treat as a constant expression (dynamic init of 9.0f = 0x41100000, folded by the back end).
// Shape: the delta (player - creature) is built on the x87 stack by the by-value `operator-`, the squared
// magnitude summed as `(z*z + x*x) + y*y` (VC7.1's re-association of `X*X + Y*Y + Z*Z`, landed 0x00805920 note),
// compared with `fcomp [static] ; test ah,41h` (greater-than), the three delta words popped afterwards; the bool
// is materialised per branch (`mov al,1` / `xor al,al`).
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
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

class CAIStateGroupBase { public:
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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};

struct CThingPos {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

const float TOO_FAR_FROM_PLAYER_DISTANCE = 3.0f;

struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    bool IsTooFarFromThePlayer(const CThing* player);
};

bool CAIStateGroup_MinionReactToPlayerInteraction::IsTooFarFromThePlayer(const CThing* player)
{
    static const float s_tooFarDistanceSquared = TOO_FAR_FROM_PLAYER_DISTANCE * TOO_FAR_FROM_PLAYER_DISTANCE;    // 0x013BB6B8
    const C3DVector& creaturePos = ((CThingPos*)PBrain->PCreature)->GetPosition();
    if ((((const CThingPos*)player)->GetPosition() - creaturePos).MagnitudeSquared() > s_tooFarDistanceSquared)
        return true;
    return false;
}