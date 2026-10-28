// CAIStateGroup_MinionReactToPlayerInteraction::IsTooNearToThePlayer(const CThing* player) -- is the player closer
// than the combat minimum distance (squared compare) to the creature.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092ED40, length 115 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPosition() const @ 0x004C73D0 on PBrain->PCreature and on the argument, the combat
// minimum-distance getter @ 0x00661CC0 (`fld [0x0125AAB8] ; ret`, no arguments; the manifest's
// "CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance" label is propagated; Ego_r.pdb names it CThingCreatureBase::GetPersonalSpaceRadius).
// Shape: the delta (player - creature) is built by the by-value `operator-` and SPILLED to the 12-byte frame
// because the distance call follows; the squared magnitude is then summed from memory, the call result squared
// (`fld st(1) ; fmul st(2)`) and compared with `fcompp ; test ah,41h` (less-than); the bool is materialised per
// branch (`mov al,1` / `xor al,al`).
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

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
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
    float MagnitudeSquared() const { return X * X + (Y * Y + Z * Z); }   // VC7.1 re-associates: retail sums z*z + y*y, then x*x (landed 0x00805920 note)
};

struct CThingPos : CThing {
};

class CThingCreatureBase : public CThing { public:
    static float __fastcall GetPersonalSpaceRadius();   // 0x00661CC0 ?GetPersonalSpaceRadius@CThingCreatureBase@@SIMXZ (Ego_r.pdb; donor 0x0053CA80 in the CThingCreatureBase run before GetPHeroCombatDef/IsPosInFOV)
};

struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    bool IsTooNearToThePlayer(const CThing* player);
};

bool CAIStateGroup_MinionReactToPlayerInteraction::IsTooNearToThePlayer(const CThing* player)
{
    const C3DVector& creaturePos = ((CThingPos*)PBrain->PCreature)->GetPos();
    C3DVector delta = ((const CThingPos*)player)->GetPos() - creaturePos;
    float minDistance = CThingCreatureBase::GetPersonalSpaceRadius();
    if (delta.MagnitudeSquared() < minDistance * minDistance)
        return true;
    return false;
}