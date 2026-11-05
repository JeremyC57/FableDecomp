// CTCDParticleEmitter::ClearAttachments() -- detach the emitter: a still-living attached thing gets this emitter's thing as a dependent (interface 0xd8), then the attachment pointer, name, flags and index are reset and the emitter is flagged to reset its position and orientation
// Leaf: ?ClearAttachments@CTCDParticleEmitter@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0AF0, 225 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020,
// CTCDependentThings::AddDependentThing(CThing*) @ 0x0071A8F0 (as the landed OnKill 0x006E0A60), CCharString::operator=
// (const char*) @ 0x0099EFE0 (literal "" @ 0x0122D70E).
// Layout: Ego_r.pdb CTCDParticleEmitter: PAttachedToThing +0x1c, AttachmentFlags +0x24, AttachmentPosName +0x28,
// AttachmentPosIndex +0x2c, the bool:1 group at +0x44/+0x45 (ParticleCreationDisabled bit 3, ResetEmitterPosition bit 4,
// ResetEmitterOrientation bit 5, AttachedToCamera bit 6, Moving bit 7; AttachedToThing +0x45 bit 0).
// Shape: the OnKill alive-and-has-interface test (GetPItem three times, the out-parameter lookup leaving the component in
// eax); the bit-field stores on each side of the calls merge into one read-modify-write per byte; the pointer is cleared
// through a CIntelligentPointer(0) temporary and the inline operator= (`SetPItem(other.GetPItem())`).
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCDependentThings {                             // interface 0xd8
    void AddDependentThing(CThing* thing);              // 0x0071A8F0
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CTCDParticleEmitter { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x1c - 0x08];
    CIntelligentPointer<CThing> PAttachedToThing;               // +0x1c
    unsigned long AttachmentFlags;                      // +0x24
    CCharString AttachmentPosName;                      // +0x28
    long AttachmentPosIndex;                            // +0x2c
    unsigned char _pad30[0x44 - 0x30];
    bool WaitingForParticleToLoad : 1;                  // +0x44 bit 0
    bool ForceParticleToLoad : 1;
    bool IndependantObject : 1;
    bool ParticleCreationDisabled : 1;                  // bit 3
    bool ResetEmitterPosition : 1;                      // bit 4
    bool ResetEmitterOrientation : 1;                   // bit 5
    bool AttachedToCamera : 1;                          // bit 6
    bool Moving : 1;                                    // bit 7
    bool AttachedToThing : 1;                           // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;
    void ClearAttachments();
};

void CTCDParticleEmitter::ClearAttachments()
{
    if (IS_THING_ALIVE(PAttachedToThing)) {
        CTCDependentThings* dependents;
        if (PAttachedToThing.GetPItem()->GetTC(TCI_PARTICLE_ATTACHMENT_TRACKER, &dependents))
            dependents->AddDependentThing(ParentThing);
    }
    AttachmentFlags = 1;
    ResetEmitterPosition = true;
    ResetEmitterOrientation = true;
    Moving = false;
    PAttachedToThing = CIntelligentPointer<CThing>(0);
    AttachmentPosName = "";
    AttachmentPosIndex = -1;
    ParticleCreationDisabled = false;
    AttachedToCamera = false;
    AttachedToThing = false;
}