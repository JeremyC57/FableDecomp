// CAIStateGroup_JackOfBladesBase::IsStillRunnable() -- Jack keeps fighting while his target and the arena centre exist.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971B00, length 85 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice on PTargetThing at +0x24, twice on PArenaCentre
// at +0x1c), CAIStateGroupBase::IsStopping @ 0x008FD010.
//
// Shape: two landed IS_THING_DEAD patterns (null, then the dead bit +0x91 & 1) around an IsStopping test, all
// as nested ifs with ONE shared `xor al,al` tail and `mov al,1` on the success path. The engine header keeps
// the pointers as opaque byte arrays, so the layout is modelled locally with their names.
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    bool IsStopping();                          // 0x008FD010
};

struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSwordEmitter;      // +0x14 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PArenaCentre;       // +0x1c CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PTargetThing;       // +0x24 CIntelligentPointer<CThingCreatureBase>
    bool IsStillRunnable();
};

bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (PTargetThing.GetPItem() != 0) {
        if (!PTargetThing.GetPItem()->IsDead()) {
            if (!IsStopping()) {
                if (PArenaCentre.GetPItem() != 0) {
                    if (!PArenaCentre.GetPItem()->IsDead())
                        return true;
                }
            }
        }
    }
    return false;
}