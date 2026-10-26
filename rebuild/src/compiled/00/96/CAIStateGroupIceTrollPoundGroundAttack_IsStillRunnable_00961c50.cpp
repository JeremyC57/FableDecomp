// CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable() const -- keep pounding while the target is alive and in range.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961C50, length 70 bytes.
// Leaf: ?IsStillRunnable@CAIStateGroup_IceTrollPoundGroundAttack@@ABE_NXZ
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times, on PTarget at +0x14),
// `anonymous namespace'::IsThingInRangeOfPound(const CThing& creature, const CThing& target) @ 0x00961B90 (__fastcall:
// ecx = &creature, edx = &target).
// Signatures: Ego_r.pdb ?IsStillRunnable@CAIStateGroup_IceTrollPoundGroundAttack@@ABE_NXZ (private const) and
// ?IsThingInRangeOfPound@?A0x9e6dcc01@@YI_NABVCThing@@0@Z (file-local, const references); ?GetPItem@CBaseIntelligentPointer@@
// QBEPAVCBaseObject@@XZ. FINDINGS chunk 407 ("0x961C50 takes pointers vs PDB const refs").
//
// Shape: the liveness test is the landed IS_THING_DEAD pattern (GetPItem null, then the dead bit +0x91 & 1,
// nested ifs with one shared false tail), then the brain's creature is read into a local BEFORE the third
// GetPItem (so it survives the call in edi) and the range predicate decides. `mov al,1` / `xor al,al`: bool.
// The engine header keeps PTarget as an opaque byte array, so the layout is modelled locally with its name.
class CBaseObject { };
class CThing : public CBaseObject {
public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                         // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return static_cast<T*>(GetPItem()); }
    T& operator*() const { return *static_cast<T*>(GetPItem()); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase {
public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

namespace {
bool __fastcall IsThingInRangeOfPound(const CThing& creature, const CThing& target);   // 0x00961B90
}

class CAIStateGroup_IceTrollPoundGroundAttack : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThing> PTarget;        // +0x14
private:
    bool IsStillRunnable() const;
};

bool CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable() const
{
    if (PTarget.GetPItem() != 0) {
        if (!PTarget->IsDead()) {
            const CThing& creature = *PBrain->PCreature;
            if (IsThingInRangeOfPound(creature, *PTarget))
                return true;
        }
    }
    return false;
}
