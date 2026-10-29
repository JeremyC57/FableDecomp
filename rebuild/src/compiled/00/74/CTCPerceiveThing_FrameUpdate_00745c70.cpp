// CTCPerceiveThing::FrameUpdate() -- per frame: age the direct-perception counter, then (every fourth frame on the component's offset, or at once when the creature has just entered melee) refresh the distance and perception of the perceived thing, or reset when it is gone; a component searching for the player starts perceiving the controlled creature
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00745C70, 220 bytes (plain `ret`). CTCPerceiveThing vtable slot 9 (catalog "vfunc_9";
//   PDB ?FrameUpdate@CTCPerceiveThing@@UAEXXZ).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x30), the combat component's
//   melee test @ 0x00733190 (`mov ecx,[ecx+24h] ; jmp 0x00661E80`, a creature flag test; named after the PDB CTCCombat::IsInMelee),
//   CWorld::GetWorldFrame() @ 0x0049D870 (static, the inline IsUpdatingThisFrame of the landed 0x007443F0), GetPItem @ 0x00A01B50,
//   ResetPerceptionVariables @ 0x00744DB0, UpdateDistanceFromPerceivedThing @ 0x007447E0, UpdatePerception @ 0x00745730,
//   CTCBase::PeekWorld() const @ 0x00686A80, CWorld::PeekMyPlayer() const @ 0x0049D9D0 (landed callers say GetCurrentPlayer; PDB
//   ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ), CPlayer::PeekPControlledCreature @ 0x00487DD0, SetThingToPerceive(const CThing*)
//   @ 0x007447A0 (manifest "DeleteData" is propagated; the body stores Perceiving = true and the thing).
// Layout: engine/CTCPerceiveThing.h (Ego_r.pdb, PDB = retail): PerceivedThing +0x0c, FramesSinceDirectlyPerceivedThing +0x1c,
//   Perceiving +0x2c, SearchingForThingToPerceive +0x33, PDef +0x34, UpdateOffset +0x38, PerceptionCombat +0x50; the header
//   carries no virtuals and an opaque intelligent pointer, so the class is modelled locally with its offsets.
//   CPerceivedThingDef Type +0x40.
// Shape: the melee flag is a bool local kept in bl (HasTC test, else 0) and stored into PerceptionCombat on every path; the
//   update test is `IsUpdatingThisFrame() || (inMelee && !PerceptionCombat)`; the reset / update arms each get their own
//   epilogue with the trailing store.
class CTCBase;
class CThing;
class CWorld;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;         // 0x004365B0 (const instantiation)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->first)
            it = map->last;
        return it->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CTCCombat { public:
    bool IsInMelee() const;                             // 0x00733190 (forwards to its creature +0x24, 0x00661E80)
};
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;              // 0x00487DD0 (PDB GetPControlledCreature)
};
class CWorld { public:
    static long __fastcall GetFrame();                        // 0x0049D870
    const CPlayer* PeekMyPlayer() const;                // 0x0049D9D0
};
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
class CPerceivedThingDef { public:
    unsigned char _pad00[0x40];
    long          Type;                                 // +0x40 EPerceivedThingType (0: the player)
};
class CTCBase
{
public:
    virtual void v00();
    virtual void FrameUpdate();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCPerceiveThing : public CTCBase {
public:
    virtual void FrameUpdate();
 public:
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<CThing> PerceivedThing;             // +0x0c
    float         DistanceFromPerceivedThingSquared;    // +0x14
    float         AwarenessTimer;                       // +0x18
    long          FramesSinceDirectlyPerceivedThing;    // +0x1c
    float         LastPerceivedPos[3];                  // +0x20
    bool          Perceiving;                           // +0x2c
    bool          LineOfSight;                          // +0x2d
    bool          ExtendedLineOfSight;                  // +0x2e
    bool          IgnoreFOVLineOfSight;                 // +0x2f
    bool          DirectAwareness;                      // +0x30
    bool          Smelt;                                // +0x31
    bool          Heard;                                // +0x32
    bool          SearchingForThingToPerceive;          // +0x33
    const CPerceivedThingDef* PDef;                     // +0x34 CDefPointer
    long          UpdateOffset;                         // +0x38
    float         Radii[5];                             // +0x3c..+0x4c
    bool          PerceptionCombat;                     // +0x50
    long          FrameHitByPerceivedThing;             // +0x54
    bool IsUpdatingThisFrame() const { return CWorld::GetFrame() % 4 == UpdateOffset; }
    void ResetPerceptionVariables();                    // 0x00744DB0
    void UpdateDistanceFromPerceivedThing();            // 0x007447E0
    void UpdatePerception();                            // 0x00745730
    void SetThingToPerceive(const CThing* thing);       // 0x007447A0
};

void CTCPerceiveThing::FrameUpdate()
{
    if (FramesSinceDirectlyPerceivedThing > -1)
        ++FramesSinceDirectlyPerceivedThing;
    bool inMelee;
    if (ParentThing->HasTC(0x30))
        inMelee = ((const CTCCombat*)ParentThing->GetTC(0x30))->IsInMelee();
    else
        inMelee = false;
    if (Perceiving) {
        if (IsUpdatingThisFrame() || (inMelee && !PerceptionCombat)) {
            if (PerceivedThing.GetPItem() == 0)
                ResetPerceptionVariables();
            else {
                UpdateDistanceFromPerceivedThing();
                UpdatePerception();
            }
        }
    } else if (SearchingForThingToPerceive && PDef->Type == 0)
        SetThingToPerceive(PeekWorld().PeekMyPlayer()->PeekPControlledCreature());
    PerceptionCombat = inMelee;
}