// CAIStateGroup_MinionReactToPlayerInteraction::IsTooFarFromThePlayer(const CThing* player) -- is the player more
// Leaf: ?IsTooFarFromThePlayer@CAIStateGroup_MinionReactToPlayerInteraction@@QAE_NPBVCThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local MagnitudeSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
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
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
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

struct CThingPos : public CThing {
};

const float TOO_FAR_FROM_PLAYER_DISTANCE = 3.0f;

struct CAIStateGroup_MinionReactToPlayerInteraction : CAIStateGroupBase {
    bool IsTooFarFromThePlayer(const CThing* player);
};

bool CAIStateGroup_MinionReactToPlayerInteraction::IsTooFarFromThePlayer(const CThing* player)
{
    static const float s_tooFarDistanceSquared = TOO_FAR_FROM_PLAYER_DISTANCE * TOO_FAR_FROM_PLAYER_DISTANCE;    // 0x013BB6B8
    const C3DVector& creaturePos = ((CThingPos*)PBrain->PCreature)->GetPos();
    if ((((const CThingPos*)player)->GetPos() - creaturePos).GetSquaredMagnitude() > s_tooFarDistanceSquared)
        return true;
    return false;
}