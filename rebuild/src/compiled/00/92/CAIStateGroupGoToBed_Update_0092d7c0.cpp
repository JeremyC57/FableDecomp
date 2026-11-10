// CAIStateGroup_GoToBed::Update() -- find a free bed (own, then bandit); yawn first if the creature has the anim, walk (run when a human is more than 15 units away) to the bed position, look along the bed's forward axis and mark the creature ready to sleep (CTCSleep 0xd4); always terminates
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local Magnitude respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092D7C0, length 604 bytes (+ int3 padding), plain `ret`.
// Callees: FindFreeBed(bool) @ 0x0092D1F0, FindFreeBanditBed @ 0x0092C8A0, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0 ("STANDARD_YAWN"), CThingCreatureBase::HasAnim @ 0x00662C70 (CAnimFilterAnyGroup 0x0127261C /
// CAnimFilterBase 0x0122FFCC), CCreatureAction_PlayAnimation @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0,
// AIAction_PerformAction @ 0x0097D8B0, CThingCreatureBase::GetCreatureType @ 0x00662000, CThing::PeekPos @ 0x004C73D0,
// AIAction_MoveToPos @ 0x0097EC00, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThingCreatureBase::PeekPos
// @ 0x006A4D00, AIAction_LookAtPos @ 0x0097D580, CVectorMap::LowerBound @ 0x0040F020, Terminate @ 0x008FCF40.
// Shape: the yawn is `failed = HasAnim(...) && PerformAction(&PlayAnimation(...)) != 2` with conditional temporaries
// (ebx flag mask 3 / 0xF); the name goes through an inline (name, filter) wrapper (landed 0x00970610).
// Layout: Ego_r.pdb CAIStateGroup_GoToBed PBed +0x14, BedPos +0x3c, Rhset +0x48 (Up, Forward +0x54); CTCSleep
// IsReadyToSleep +0x0c.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float) (Ego_r.pdb / FableWin.pdb).
enum EMovementType;                           // MS forward enum
enum EAIActionRepeat;                         // MS forward enum
enum EAIActionReturn;                         // MS forward enum
#include "fable_compiler.h"
#include <math.h>
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CAnimFilterBase { public:                               // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {         // vtable 0x0127261C
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCSleep { public:                                      // interface 0xd4
    unsigned char _pad0[0x0c];
    bool IsReadyToSleep;                               // +0x0c
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
    FABLE_FORCEINLINE unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;              // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x00662C70
    long GetCreatureType() const;                      // 0x00662000
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                            // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                     // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool CreatureHasAnim(const CCharString& name, const CAnimFilterBase& filter) const { return GetCreature()->HasAnim(name, filter); }
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    protected:
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& arrived, EMovementType movement, bool a, bool b, const CThing* avoid, bool c, float speed);   // 0x0097EC00
    public:
    protected:
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    public:
    void Terminate();                                  // 0x008FCF40
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBed;                      // +0x14
    bool IsBedSleepPointMarker;                        // +0x1c
    long SleepStartTimeVariation;                      // +0x20
    long SleepEndTimeVariation;                        // +0x24
    bool BedToRightOfCreature;                         // +0x28
    long ActionPointIndex;                             // +0x2c
    CCharString BedSide;                               // +0x30
    bool PlayEmergencyGetOutOfBed;                     // +0x34
    long BedGetOutOfBedFastAnim;                       // +0x38
    C3DVector BedPos;                                  // +0x3c
    CRightHandedSet Rhset;                             // +0x48
    bool FindFreeBed(bool allowOwned);                 // 0x0092D1F0
    bool FindFreeBanditBed();                          // 0x0092C8A0
    void Update();
};

void CAIStateGroup_GoToBed::Update()
{
    if (FindFreeBed(false) || FindFreeBanditBed()) {
        bool arrived;
        bool failed = CreatureHasAnim("STANDARD_YAWN", CAnimFilterAnyGroup())
            && AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "STANDARD_YAWN", CSoundPair(),
                   false, true, false, 0, 0)) != 2;
        if (!failed) {
            long movement = 3;
            if (GetCreature()->GetCreatureType() == 1 && (BedPos - GetCreature()->GetPos()).GetMagnitude() > 15.0f)
                movement = 5;
            arrived = false;
            long result;
            do {
                result = AIAction_MoveToPos((EAIActionRepeat)(2), BedPos, arrived, (EMovementType)(movement), true, true, 0, true, 0.0f);
            } while (result == 4);
            if (result == 2 && PBed.GetPItem() != 0 && !(PBed.GetPItem()->m_flags91 & 1)
                && AIAction_LookAtPos((EAIActionRepeat)(1), GetCreature()->PeekPos() + Rhset.Forward, 0x11, 0.01f) == 2
                && GetCreature()->HasTC(0xd4))
                GetCreature()->GetTC<CTCSleep>(0xd4)->IsReadyToSleep = true;
        }
    }
    Terminate();
}