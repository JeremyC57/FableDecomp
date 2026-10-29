// CTCDParticleEmitter::IsParticleCreationDisabled() const -- creation is disabled while the emitter sleeps, while the attached thing is in limbo, or while the attached creature sits idle in creature mode 30
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;                  // bit 5
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBaseInterface*)it->second);
            return true;
        }
        return false;
    }
};
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
    if (PAttachedToThing.GetPItem()->InLimbo)
        return true;
    CTCCreatureModeManager* modes;
    if (PAttachedToThing.GetPItem()->GetInterface(&modes, 0x31)) {
        const CThingCreatureBase* creature = (const CThingCreatureBase*)PAttachedToThing.GetPItem();
        if (modes->GetCurrentMode()->GetType() == 30 && !creature->IsPerformingAnyAction())
            return true;
    }
    return false;
}