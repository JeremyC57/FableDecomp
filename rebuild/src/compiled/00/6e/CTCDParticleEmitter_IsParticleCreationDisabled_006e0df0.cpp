// CTCDParticleEmitter::IsParticleCreationDisabled() const -- creation is disabled while the emitter sleeps, while the attached thing is in limbo, or while the attached creature sits idle in creature mode 30
// Leaf: ?IsParticleCreationDisabled@CTCDParticleEmitter@@QBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E0DF0, length 138 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), the const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0
// (inlined GetInterface for type 0x31 = CTCCreatureModeManager, word 1 bit 17), CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0
// (`return Modes.front()`; the manifest's "CPixelShader::CountReferences" is a byte-twin label), the mode's slot 1 (+4, GetType),
// CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0.
// Layout: CTCDParticleEmitter as in the landed SetAlwaysUpdate 0x006E0590 (PAttachedToThing +0x1C, Asleep = bit 2 of +0x45).
// Retail CThing: the PDB bit byte +0xA1 sits at +0x91 (Dead bit 0, InLimbo bit 5).
// Shape: early `return true`s sharing `mov al,1` exits; the mode manager is a named local (esi) and the creature pointer is
// fetched into edi before the mode call; one `xor al,al` exit.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() const;         // 0x006622A0
};
struct CCreatureModeBase {
    virtual void v00();
    virtual long GetType() const;               // slot 1 (+4)
};
class CTCCreatureModeManager { public:                 // interface 0x31
    CCreatureModeBase* GetCurrentMode() const;  // 0x0073A6E0
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
class CTCDParticleEmitter { public:
    virtual void v00();
    unsigned char           _base[0x8];         // CTCBase: ParentThing +4, flags +8/+9
    void*                   EmitterName;        // +0x0c CCharString
    long                    ParticleTypeID;     // +0x10
    CIntelligentPointer<CThing> PEmitter;           // +0x14
    CIntelligentPointer<CThing> PAttachedToThing;   // +0x1c
    unsigned char           _body[0x44 - 0x24]; // AttachmentFlags .. PCreationParameters
    unsigned char           Flags44;            // +0x44 (WaitingForParticleToLoad .. Moving)
    bool AttachedToThing : 1;                   // +0x45 bit 0
    bool AlwaysUpdate : 1;
    bool Asleep : 1;                            // bit 2
    bool IsParticleCreationDisabled() const;
};

bool CTCDParticleEmitter::IsParticleCreationDisabled() const
{
    if (Asleep)
        return true;
    if (PAttachedToThing.GetPItem()->IsInLimbo())
        return true;
    const CTCCreatureModeManager* modes;
    if (PAttachedToThing.GetPItem()->PeekTC(TCI_ENTITY_MODE_MANAGER, &modes)) {
        const CThingCreatureBase* creature = (const CThingCreatureBase*)PAttachedToThing.GetPItem();
        if (modes->GetCurrentMode()->GetType() == 30 && !creature->IsPerformingAnyAction())
            return true;
    }
    return false;
}