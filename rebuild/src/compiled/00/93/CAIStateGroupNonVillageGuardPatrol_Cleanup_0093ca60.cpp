// CAIStateGroup_NonVillageGuardPatrol::Cleanup() -- clear CurrentGuardpostThing: `CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(0)` through a converting frame temporary
// Leaf: ?Cleanup@CAIStateGroup_NonVillageGuardPatrol@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093CA60, 66 bytes, thiscall, plain `ret`.
// Identity (proven): vtable 0x0128B87C (RTTI .?AVCAIStateGroup_NonVillageGuardPatrol@@) slot 9 = Cleanup; PDB ?Cleanup@CAIStateGroup_NonVillageGuardPatrol@@UAEXXZ; member CurrentGuardpostThing (CIntelligentPointer<const CThingTrackNode>, vtable 0x0125A894) at +0x14.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, SetPItem 0x00A01B90 (twice), GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10; the temporary's CIntelligentPointer<const CThingTrackNode> vtable 0x0125A894.
// Shape (family K015, representative 0x006E8B10): the inline converting constructor (base ctor, vtable store, SetPItem(0)),
//   the inline operator= `SetPItem(other.GetPItem())` into the member, then the base destructor called directly (the
//   derived destructor is implicit) -- the landed CAIStateGroup_EnemyCreatureBase::Cleanup 0x0091D850 / CCombatRing::Reset
//   0x006E8B60 form.
class CThingTrackNode;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<const CThingTrackNode>, vtable 0x0125A894
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CAIBrain;
class CAIStateGroupBase {                                // Ego_r.pdb, 0x14 bytes (retail = PDB: the CCharString is 4 bytes)
public:
    virtual ~CAIStateGroupBase();
    virtual void Cleanup();                              // vtable slot 9 in retail
    CAIBrain* PBrain;                                    // +0x04
    long ScriptAIPriority;                               // +0x08 EScriptAIPriority
    void* Name;                                          // +0x0C CCharString (one data pointer)
    bool Terminated;                                     // +0x10
    unsigned char Priority;                              // +0x11
    bool Enabled;                                        // +0x12
};
class CAIStateGroup_NonVillageGuardPatrol : public CAIStateGroupBase {   // Ego_r.pdb, 0x1C bytes
public:
    virtual void Cleanup();
    CIntelligentPointer<const CThingTrackNode> CurrentGuardpostThing;   // +0x14
};

void CAIStateGroup_NonVillageGuardPatrol::Cleanup()
{
    CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(0);
}