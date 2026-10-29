// CAIStateGroup_MinionHasInformation::Update() -- while the minion still wants the hero's attention, look at the (living) hero; every 5 seconds wave (playing ATTRACT_HERO_ATTENTION through the sound component, but after 20 seconds only every fifth wave), every 10 seconds wander; terminate when the hero is gone, attention is no longer wanted or the wander fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F470, length 418 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CAIStateGroupBase brain accessor @ 0x008FCF80 (brain +0x20), the main-player accessor @ 0x00449960,
// CPlayer::GetPControlledCreature @ 0x00487DC0, CreatureWantsPlayersAttention @ 0x0090F290, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C, by value),
// CAIStateGroupBase::AIAction_LookAtThing @ 0x0097D6F0, CThingAICreature WaveWithInformation getter @ 0x008300F0 and
// WanderWithInformation getter @ 0x008300E0 (out of line), CWorld::GetWorldFrame @ 0x0049D870, CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 0x10), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// the sound component's PlaySound @ 0x0066BD00, Wander @ 0x0090F320, AIAction_DoNothing @ 0x0097E200,
// Terminate @ 0x008FCF40.
// Data: g_framesPerSecond @ 0x01375550; "ATTRACT_HERO_ATTENTION" @ 0x0128F574.
// Layout: Ego_r.pdb CAIStateGroup_MinionHasInformation (LastFrameWaved +0x14, LastFrameWandered +0x18, FrameInitialised
// +0x1c, WavesWithoutCalling +0x20); retail CThing interface bitset +0x20 / map +0x44. The engine header flattens the base,
// so the model is local.
// Shape: the elapsed-time tests go through an inline helper that reads the frame rate into a local before calling
// GetWorldFrame and takes the member by reference (so it is re-read after the call); the 20-second rule is a flag.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingAICreature : public CThing { public:
    bool GetWanderWithInformation() const;      // 0x008300E0 (+0x1c8 bit 3)
    bool GetWaveWithInformation() const;        // 0x008300F0 (+0x1c8 bit 4)
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CTCSound {                               // the interface-0x10 component
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // <const CThing>: vtable 0x01237F1C
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                  // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPMainPlayer();                  // 0x00449960
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
inline bool HasTimePassed(const long& lastFrame, long seconds)
{
    long fps = g_framesPerSecond;
    return CWorld::GetFrame() - lastFrame > seconds * fps;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager_008fcf80();        // 0x008FCF80
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   protected: // 0x0097D6F0
    EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200
};
struct CAIStateGroup_MinionHasInformation : CAIStateGroupBase {
    long LastFrameWaved;                        // +0x14
    long LastFrameWandered;                     // +0x18
    long FrameInitialised;                      // +0x1c
    long WavesWithoutCalling;                   // +0x20
    bool CreatureWantsPlayersAttention();       // 0x0090F290
    bool Wander();                              // 0x0090F320
    virtual void Update();
};

void CAIStateGroup_MinionHasInformation::Update()
{
    CThing* hero = GetPPlayerManager_008fcf80()->GetPMainPlayer()->GetPControlledCreature();
    if (hero == 0 || (hero->m_flags91 & 1)) {
        Terminate();
        return;
    }
    do {
        if (!CreatureWantsPlayersAttention()) {
            Terminate();
            return;
        }
        if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(hero), 0xb, 0.013888889f) != 2)
            return;
        if (GetCreature()->GetWaveWithInformation()) {
            if (LastFrameWaved == 0 || HasTimePassed(LastFrameWaved, 5)) {
                LastFrameWaved = CWorld::GetFrame();
                bool attract = true;
                if (LastFrameWaved - FrameInitialised > 20 * g_framesPerSecond) {
                    if (++WavesWithoutCalling > 4)
                        WavesWithoutCalling = 0;
                    else
                        attract = false;
                }
                CTCSound* sound;
                if (attract && GetCreature()->GetInterface(&sound, 0x10)) {
                    CCharString name("ATTRACT_HERO_ATTENTION");
                    sound->PlaySound(name, 0, 0, 0, -1);
                }
            }
        }
        if (GetCreature()->GetWanderWithInformation()) {
            if (LastFrameWandered == 0 || HasTimePassed(LastFrameWandered, 10)) {
                LastFrameWandered = CWorld::GetFrame();
                if (!Wander()) {
                    Terminate();
                    return;
                }
            }
        }
    } while (AIAction_DoNothing() == 4);
}