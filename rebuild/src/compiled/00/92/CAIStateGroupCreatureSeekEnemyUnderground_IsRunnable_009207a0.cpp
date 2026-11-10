// CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable() -- seek a live enemy underground while it is beyond half the creature's surfacing range (XY distance); true when there is nothing to seek
// Flags: /d2inlS20
// Retail: Fable.exe (TLC, VC7.1) @ 0x009207A0, 308 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_CreatureSeekEnemyUnderground@@MAE_NXZ.
// Callees: CVectorMap::LowerBound 0x004365B0 (const; inlined GetInterface types 8 and 0x34), the enemy source's virtual
// slot 29 (+0x74), CThing::GetPos 0x004C73D0 (creature first), the 0x34 component's range getter 0x007448F0, the
// converting CIntelligentPointer temporary (vtable 0x01237F1C: ctor 0x00A01B10, SetPItem 0x00A01B90 x2, GetPItem 0x00A01B50,
// dtor 0x00A01C10); 0.5f pool 0x0122F59C.
// Shape: the landed AI state-group models (0x00937F70; one-ternary GetInterface select); `sqrt(dx*dx + dy*dy)` of
// enemy - creature through an XY helper written straight into the comparison with `range * 0.5f` (a named distance
// local lets VC sink the sqrt below the range call and spill dx/dy instead).
// Name-debt wave (agentNF4, 2026-11-10): the local CBaseIntelligentPointerCtor / CIntelligentPointerThing stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (retail branch, FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN: the inline `SetPItem(other.GetPItem())` assignment);
//   the member is CIntelligentPointer<const CThing> (the temporary's vtable store names the instantiation; Ego_r.pdb member type). CThing is the
//   retail CClass / CBaseObject chain (FABLE_RETAIL_CCLASS) so the pointer converts to CBaseObject*.
//   /d2inlS20: an inline-budget residue (owner 2026-11-08): the shared header's converting constructor CIntelligentPointer(T*) is plain
//   `inline` (retail keeps an out-of-line copy, 0x004A96F0 for CThing), and this caller's size leaves it called out of line at the
//   default budget where retail expands it; /d2inlS20 is the smallest caller-size value that expands it (S15..S19 do not).
// Leaf: IsRunnable@CAIStateGroup_CreatureSeekEnemyUnderground
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "fable_compiler.h"
#include <math.h>
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/C3DVector.h"

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"


struct CTCEnemySource {                         // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy() const;                 // slot 29 (+0x74)
};

struct CTCUndergroundMover {                    // the interface-0x34 component
    float GetSurfaceRange() const;                    // 0x007448F0
};

static inline float XYDistance(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return (float)sqrt(dx * dx + dy * dy);
}

struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroupBase {
    CIntelligentPointer<const CThing> PEnemy;            // +0x14 (vtable 0x01237F1C)
protected:
    virtual bool IsRunnable();                  // Ego_r.pdb ?IsRunnable@CAIStateGroup_CreatureSeekEnemyUnderground@@MAE_NXZ (protected)
};

bool CAIStateGroup_CreatureSeekEnemyUnderground::IsRunnable()
{
    const CTCEnemySource* source;
    PBrain->PCreature->PeekTC(TCI_TARGETING, &source);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !enemy->IsDead()) {
        CThing* creature = PBrain->PCreature;
        const CTCUndergroundMover* mover;
        if (creature->PeekTC(TCI_PERCEIVE_THING, &mover)) {
            if (XYDistance(enemy->GetPos(), creature->GetPos()) > mover->GetSurfaceRange() * 0.5f) {
                PEnemy = enemy;
                return true;
            }
            return false;
        }
    }
    return true;
}