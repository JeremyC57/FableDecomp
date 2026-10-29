// CAIStateGroup_EarthTroll::IsRunnable() -- runnable when the troll's live enemy is within AttackRange (XY distance); remember the enemy and the frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094E6B0, 243 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_EarthTroll@@MAE_NXZ.
// Callees: CVectorMap::LowerBound 0x004365B0 (const; inlined out-parameter GetInterface type 8, result unchecked), the
// enemy source's virtual slot 29 (+0x74), CThing::GetPosition 0x006A4D00 (this TU's copy; enemy first), the converting
// CIntelligentPointer<CThingCreatureBase> temporary (vtable 0x0123E7D8: ctor 0x00A01B10, SetPItem 0x00A01B90 x2, GetPItem
// 0x00A01B50, dtor 0x00A01C10), the brain world accessor 0x008FCFC0, the static frame counter 0x0049D870.
// Shape: the landed AI state-group model of 0x00937F70; `sqrt(dx*dx + dy*dy) < AttackRange` (intrinsic fsqrt, Y term
// first); the miss path of the lookup reads the never-written source pointer from the key's frame word.
// Layout: the engine header CAIStateGroup_EarthTroll.h is flattened (base 0x30); retail PEnemy +0x14, the frame +0x1c,
// AttackRange +0x24 (local model).
// Leaf: IsRunnable@CAIStateGroup_EarthTroll
// Shape: the CIntelligentPointer operator= names the GetPItem() CALL result (`item = other.GetPItem(); SetPItem(item);`), which
//   keeps the member's address (this of SetPItem) after the source's GetPItem call and colours the registers as retail; the
//   inline `SetPItem(other.GetPItem())` form evaluates the inlined member's `this` first.
#include <math.h>
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector;
class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;   // 0x006A4D00 ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ (Ego_r donor order; byte twin of CThing::GetPos 0x004C73D0)
};
class CThing : public CThingPhysical { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;   // one ternary: straight into ecx
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline const CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        const CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

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

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCEnemySource {                         // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

class C3DVector { public: float X, Y, Z; };


class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ

struct CAIStateGroup_EarthTroll : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14 CIntelligentPointer<CThingCreatureBase>
    long LastRunFrame;                          // +0x1c
    unsigned char _pad20[4];
    float AttackRange;                          // +0x24
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual bool IsRunnable();
};

bool CAIStateGroup_EarthTroll::IsRunnable()
{
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        CThing* creature = PBrain->PCreature;
        const C3DVector& enemyPos = enemy->PeekPos();
        const C3DVector& pos = creature->PeekPos();
        float dx = pos.X - enemyPos.X;
        float dy = pos.Y - enemyPos.Y;
        if ((float)sqrt(dx * dx + dy * dy) < AttackRange) {
            PEnemy = enemy;
            LastRunFrame = ((CWorld*)GetPWorld())->GetFrame();
            return true;
        }
    }
    return false;
}