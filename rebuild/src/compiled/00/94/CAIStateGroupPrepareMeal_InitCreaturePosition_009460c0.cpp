// CAIStateGroup_PrepareMeal::InitCreaturePosition() -- if runnable, auto-place the creature within 3m of the cooking pot.
// Leaf: ?InitCreaturePosition@CAIStateGroup_PrepareMeal@@UAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009460C0, length 63 bytes (+ int3 padding). Vtable slot 3 of
// CAIStateGroup_PrepareMeal (0x0128BD7C).
// Callees: this->IsRunnable (vtable slot 11, `call [eax+2Ch]`), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCookingPot +0x14), CThing::GetPosition @ 0x004C73D0, CThingCreatureBase::AutoPlaceNear(C3DVector*, float)
// @ 0x00665DF0 (3.0f pushed first).
// Shape: `IsRunnable() && PlaceNearPot()` -- the creature `PBrain->Creature` is read into a named local (edi) inside
// the inline helper before the pushes (the bare expression allocates differently), and the `&&` of two bools
// returns through a full register (`mov eax,1` / `xor eax,eax`).
class CTCBase;
class CThing;

#include "engine/C3DVector.h"

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the shape of the landed
// GetInterface_0x.. bodies, inlined into the AI bodies through GetTC<T>().
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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

struct CThingCreature : CThing {
    bool AutoPlaceNear(C3DVector* pos, float radius);   // 0x00665DF0
};

struct CAIBrainCreature {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};

struct CAIStateGroup_PrepareMeal : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCookingPot;        // +0x14 CIntelligentPointer<CThing>
    virtual bool InitCreaturePosition();
    bool PlaceNearPot()
    {
        CThingCreature* pCreature = ((CAIBrainCreature*)PBrain)->PCreature;
        return pCreature->AutoPlaceNear((C3DVector*)&PCookingPot.GetPItem()->GetPos(), 3.0f);
    }
};

bool CAIStateGroup_PrepareMeal::InitCreaturePosition()
{
    return IsRunnable() && PlaceNearPot();
}