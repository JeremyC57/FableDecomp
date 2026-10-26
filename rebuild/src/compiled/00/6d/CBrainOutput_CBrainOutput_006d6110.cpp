// CBrainOutput::CBrainOutput(const CBrainOutput& other) -- copy a creature brain's output record: flags, navigation request, the counted action pointer (shared, count bumped), the two target positions and the intelligent look-at pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6110, length 149 bytes (ret 4).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90; vtable CIntelligentPointer<CThing const> @ 0x01237F1C.
// Layout (Ego_r.pdb CBrainOutput, 0x48 bytes): Types +0x00, LookFlags +0x04, NavigationInput +0x08 (CNavigationInput,
// 24 bytes: Destination, Proximity, MovementType, Speed), PAction +0x20 (CCountedPointer<CCreatureActionBase>:
// object +0x20, count block +0x24), MoveToPos +0x28, LookAtPos +0x34, LookAtThing +0x40.
// Shape: every dword is copied through a rotating eax/ecx/edx (memberwise copy constructors, no block move);
// the counted pointer's copy constructor bumps the count word at the block's +0 under a null test on the stored
// block (`test eax,eax ; mov [esi+24h],eax ; je ; inc [eax]`); the intelligent pointer is the family-44 inline copy
// constructor: base constructor, vtable store, SetPItem(other.GetPItem()).
class CBaseObject;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CNavigationInput { public:                     // 24 bytes
    C3DVector Destination;                    // +0x00
    float     Proximity;                      // +0x0c
    long      MovementType;                   // +0x10 EMovementType
    float     Speed;                          // +0x14
    CNavigationInput(const CNavigationInput& o)
        : Destination(o.Destination), Proximity(o.Proximity), MovementType(o.MovementType), Speed(o.Speed) { }
};
class CCreatureActionBase;
struct CCountedPointerAction {                // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Object;              // +0x00
    long*                PCount;              // +0x04 shared count block (count at +0)
    CCountedPointerAction(const CCountedPointerAction& o) : Object(o.Object), PCount(o.PCount)
    {
        if (PCount)
            ++*PCount;
    }
};
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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
public:
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CBrainOutput {
public:
    unsigned long            Types;           // +0x00
    long                     LookFlags;       // +0x04
    CNavigationInput         NavigationInput; // +0x08
    CCountedPointerAction    PAction;         // +0x20
    C3DVector                MoveToPos;       // +0x28
    C3DVector                LookAtPos;       // +0x34
    CIntelligentPointer<const CThing> LookAtThing;     // +0x40
    CBrainOutput(const CBrainOutput& other);
};

CBrainOutput::CBrainOutput(const CBrainOutput& other)
    : Types(other.Types), LookFlags(other.LookFlags), NavigationInput(other.NavigationInput),
      PAction(other.PAction), MoveToPos(other.MoveToPos), LookAtPos(other.LookAtPos),
      LookAtThing(other.LookAtThing)
{
}