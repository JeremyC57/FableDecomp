// NCreatureMode::CModeBlock::OnActivate() -- base activation, remember the creature, suspend its animation while it is in state 1, play its "START_BLOCKING" sound pair, then run one FrameUpdate
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8CE0, 231 bytes (`ret`; vtable 0x01267C3C slot 5).
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0 (qualified base call); CThingAnimatable::SetSuspendAnimation
// (bool, long) 0x00833A10; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, type 0x10 = bit 16
// of the dword at +0x20); CCharString(const char*, long) 0x0099EBF0 ("START_BLOCKING"), ~CCharString 0x0099EAE0; the creature's
// virtual slot 89 (+0x164, the sound pair for a name, hidden-pointer return); the sound component's play call 0x0066B280
// (manifest "CAnimateThing_PlayRandomSound"); this mode's own virtual slot 3 (+0x0c, FrameUpdate 0x008D8EA0).
// Layout (PDB CModeBlock 0x20): CModeBase 0x1c (ParentThing +0x04), PCreature +0x1c. Creature: state byte +0x10.
// Shape: the landed CCreatureAction_BlockBase::Initialise 0x00855430 sound block (string temporary first, the play-parameter
// block assigned in its ctor body) inside the GetInterface test, after the suspend test.
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

class CSoundPair { public:
    long First;
    long Second;
};

struct CSoundPlayParams {                  // as the landed 0x00855430
    float Volume;                          // +0x00
    float Pitch;                           // +0x04
    float Delay;                           // +0x08
    long  Loop;                            // +0x0c
    long  Priority;                        // +0x10
    long  Pan;                             // +0x14
    long  Flags;                           // +0x18
    long  Handle;                          // +0x1c
    CSoundPlayParams() { Volume = 1.0f; Pitch = 1.0f; Delay = 0.0f; Loop = 1; Pan = 0; Flags = 0; Handle = -1; Priority = 1; }
};

class CTCSoundPlayer { public:                    // the interface-0x10 component
    void PlaySound(const CSoundPair& sound, const CSoundPlayParams& params, long a, long b, long c);   // 0x0066B280
};

class CThing;

struct CThingAnimatable {                  // the thing's polymorphic root here (vptr at +0); only its member is called
    virtual void v000();
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

class CThingCreatureBase : public CThingAnimatable { public:
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual CSoundPair GetSoundPair(const CCharString& name);   // slot 89 (+0x164)
    unsigned char _pad04[0x0c];
    unsigned char m_state;                 // +0x10 creature state byte (compared with 1)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    virtual unsigned long GetModeType();   // slot 1
    virtual void AnimUpdate(long);         // slot 2
    virtual void FrameUpdate();            // slot 3
    virtual void v04();
    virtual void OnActivate();             // slot 5, 0x0073AAC0
    virtual void OnDeactivate();           // slot 6
    CThingCreatureBase* ParentThing;       // +0x04 (a CThing*)
    unsigned char       _body[0x14];       // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
};

namespace NCreatureMode {

struct CModeBlock : CModeBase {            // vtable 0x01267C3C
    CThingCreatureBase* PCreature;         // +0x1c
    virtual void OnActivate();
};

void CModeBlock::OnActivate()
{
    CModeBase::OnActivate();
    PCreature = ParentThing;
    if (PCreature->m_state == 1)
        PCreature->SetSuspendAnimation(true, 0);
    CTCSoundPlayer* player;
    if (ParentThing->GetInterface(&player, 0x10)) {
        player->PlaySound(PCreature->GetSoundPair("START_BLOCKING"), CSoundPlayParams(), 1, 0, 0);
    }
    FrameUpdate();
}

}