// CModeStandBase::UpdateIdleAnim(CAnimateThing& anim) -- idle animation picker while the creature stands still: retarget the
//   queued transition animations (mode 4 -> 2), choose the pose idle (ST_IDLE_MELEE when its weapons are out and its combat
//   component uses the unarmed combat idle, else ST_IDLE_SUBTLE), then a special idle: STANDARD_WAVE_INFO for a creature
//   with information to wave about once it has idled long enough and the living hero is at least 3 units away, else ST_IDLE
//   once it has idled long enough; unless it is performing an action, reset the idle counter for a special idle, play the
//   special idle (type 0x19) over a subtle pose when it is not already looping, else (re)start the pose idle (type 6) when
//   it changed or nothing loops; remember the pose idle name
// Leaf: ?UpdateIdleAnim@CModeStandBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00862780, 798 bytes (`ret 4`). Ego_r.pdb: `?UpdateIdleAnim@CModeStandBase@@AAEXAAVCAnimateThing@@@Z`.
// Callees: CAnimateThing::ResetTransistionAnimationModes 0x0070C0A0 / SetMainAnim 0x0070D580; CCharString() 0x0099E4B0,
//   (const char*, long) 0x0099EBF0, ~ 0x0099EAE0, operator=(const char*) 0x0099EFE0, operator=(const CCharString&) 0x0099EFB0,
//   operator!=(const char*) 0x0099E960, operator!=(const CCharString&) 0x0099E900, CBasicString<char>::operator==(const char*)
//   0x004115A0 (the inline null-block arm is the repe cmpsb); the creature's slot 77 AreWeaponsUnsheathed (+0x134), the const
//   LowerBound 0x004365B0 (TC 0x30), CTCCombat::IsUsingUnarmedCombatIdle 0x00733100, CThingAICreature +0x1c8 getters 0x008300D0
//   (has information) / 0x008300F0 (wave with information), CModeBase::GetNoFramesBeforePlayingWaveAnims 0x0073A7D0 /
//   GetNoFramesBeforePlayingIdleAnims 0x0073A760 (manifest: both "GetNoFramesBeforePlayingCombinationIdleAnims", propagated from
//   0x0073A6F0), CPlayerManager::GetMainPlayer 0x00449970 (g_pMainGameComponent 0x013B86A0 +0x1c), GetPItem 0x00A01B50,
//   CThing::GetPosition 0x004C73D0 (both hero and creature, so the creature model declares no GetPosition of its own),
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0 / GetRandomLoadedAnimFromName 0x00662FA0, CAnimationEntry
//   GetAnimationTransitionInTime 0x005DC1F0 / GetTimeToBlendIn 0x005DC230 / GetAnimateThingFlags 0x005DC390, the out-of-line
//   CAnimInit(7 args) constructor 0x005B33F4 (landed as "CRumbleEffect_005b33f4::Initialise"; member zeroing then Set is a
//   constructor body) and CAnimInit::Set 0x0070C050.
// Data: "ST_IDLE_MELEE" 0x01276DE0, "ST_IDLE_SUBTLE" 0x01276DBC, "STANDARD_WAVE_INFO" 0x01276DCC, "ST_IDLE" 0x0127138C,
//   "" 0x0122D70E, 9.0f 0x0125EF98.
// Layout: engine/CModeStandBase.h is a flattened pack(1) record whose vptr is a plain pointer, so the class is restated with
//   its two virtual slots (as the landed AnimUpdate 0x00862EC0); members IdleCounter +0x1c, ForceAddNewAnims +0x24,
//   PoseAnimName +0x34 as the PDB. Retail CThing: interface bitset +0x20, CVectorMap +0x44, flags +0x6c / +0x91.
// Shape: `!(d2 < 9.0f)` is `test ah,5 ; jnp`; the distance sums dy*dy first; the reset flag is set before the name store;
//   ST_IDLE is a named CCharString assigned through operator=(const CCharString&); the special idle goes through the
//   out-of-line CAnimInit constructor as a temporary argument, the pose idle reads its flags into a local and then builds a
//   default CAnimInit and calls Set (the zero stores land after the pushes), so both share the SetMainAnim tail.
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString();                              // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator=(const char* text);   // 0x0099EFE0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    bool operator!=(const char* text) const;    // 0x0099E960
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

#define VF(n) virtual void v##n();
struct CThing {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71) VF(72) VF(73) VF(74) VF(75) VF(76)
    virtual bool AreWeaponsUnsheathed();                // slot 77 (+0x134)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6c;                              // +0x6c (bit 4: has information to give)
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char Flags91;                              // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};

struct CTCCombat {
    bool IsUsingUnarmedCombatIdle() const;              // 0x00733100
};

struct CAnimationEntry {
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
    long GetAnimationTransitionInTime() const;          // 0x005DC1F0
    long GetTimeToBlendIn(const CThing* thing) const;   // 0x005DC230
};

struct CThingCreatureBase : CThing {
    bool IsPerformingAnyAction();                       // 0x006622A0
    const CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool loadedOnly) const;   // 0x00662FA0
};
struct CThingAICreature : CThingCreatureBase {
    bool GetHasInformation() const;                     // 0x008300D0
    bool GetWaveWithInformation() const;                // 0x008300F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer PCharacterThing;            // +0x34
    CThing* GetPCharacterThing() const { return PCharacterThing.GetPItem(); }
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CMainGameComponent {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CBlendPair {                                     // the two floats at CAnimInit +0x18
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                                      // 0x2c bytes
    long          Mode;
    unsigned char AnimType;
    bool          Flag05;
    long          AnimIndex;
    float         Scales[3];
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;
    long          Arg24;
    long          Percent;
    CAnimInit() { }
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x005B33F4
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
};

enum EAnimUpdateMode { AUM_NONE };
struct CAnimateThing {
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
    void ResetTransistionAnimationModes(EAnimUpdateMode from, EAnimUpdateMode to);   // 0x0070C0A0
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition);         // 0x0070D580
};

enum EMode { EMODE_NONE = 0 };

class CModeBase {
public:
    virtual ~CModeBase();
    virtual EMode GetModeType() const = 0;      // slot 1
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x14];
    long GetNoFramesBeforePlayingIdleAnims(EMode mode);   // 0x0073A760
    long GetNoFramesBeforePlayingWaveAnims(EMode mode);   // 0x0073A7D0
};

class CModeStandBase : public CModeBase {
public:
    long        IdleCounter;                    // +0x1c
    long        FramesToKeepUpdatingMovement;   // +0x20
    bool        ForceAddNewAnims;               // +0x24
    float       MaxRunSpeed;                    // +0x28
    float       MaxJogSpeed;                    // +0x2c
    float       MaxWalkSpeed;                   // +0x30
    CCharString PoseAnimName;                   // +0x34
    void UpdateIdleAnim(CAnimateThing& anim);
};

void CModeStandBase::UpdateIdleAnim(CAnimateThing& anim)
{
    CThingAICreature* creature = (CThingAICreature*)ParentThing;
    anim.ResetTransistionAnimationModes((EAnimUpdateMode)4, (EAnimUpdateMode)2);
    bool subtle = false;
    CCharString poseName;
    if (creature->AreWeaponsUnsheathed() && creature->HasTC(0x30)
        && ((const CThing*)creature)->GetConstTC<CTCCombat>(0x30)->IsUsingUnarmedCombatIdle())
        poseName = "ST_IDLE_MELEE";
    else {
        subtle = true;
        poseName = "ST_IDLE_SUBTLE";
    }
    CCharString specialName;
    bool resetIdleCounter = false;
    if ((creature->Flags6c & 0x10) && creature->GetHasInformation() && creature->GetWaveWithInformation()
        && IdleCounter > GetNoFramesBeforePlayingWaveAnims(GetModeType())) {
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetPCharacterThing();
        if (hero && !(hero->Flags91 & 1)) {
            const C3DVector& heroPos = hero->GetPosition();
            const C3DVector& pos = creature->GetPosition();
            float dx = pos.X - heroPos.X;
            float dy = pos.Y - heroPos.Y;
            if (!(dy * dy + dx * dx < 9.0f)) {
                resetIdleCounter = true;
                specialName = "STANDARD_WAVE_INFO";
            }
        }
    }
    if (specialName == "" && IdleCounter > GetNoFramesBeforePlayingIdleAnims(GetModeType())) {
        resetIdleCounter = true;
        CCharString idle("ST_IDLE");
        specialName = idle;
    }
    if (!creature->IsPerformingAnyAction()) {
        if (resetIdleCounter)
            IdleCounter = 0;
        if (subtle && specialName != ""
            && (anim.AnimsBegin == anim.AnimsEnd || (anim.CurrentAnimFlags & 2) || anim.CurrentAnimType != 0x19
                || (anim.CurrentAnimFlags & 1) || ForceAddNewAnims)) {
            const CAnimationEntry* entry = creature->GetRandomLoadedAnimFromName(specialName, false);
            if (entry) {
                CThing* thing = ParentThing;
                anim.SetMainAnim(CAnimInit(0x19, 0, entry->AnimIndex, 1, false, entry->GetAnimateThingFlags(), 100),
                                 entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
            }
        } else if (poseName != ""
                   && (poseName != PoseAnimName || anim.AnimsBegin == anim.AnimsEnd || (anim.CurrentAnimFlags & 3)
                       || ForceAddNewAnims)) {
            const CAnimationEntry* entry = creature->GetRandomLoadedAnimFromName(poseName, false);
            if (entry) {
                CThing* thing = ParentThing;
                unsigned long flags = entry->GetAnimateThingFlags();
                CAnimInit init;
                init.Set(6, 0, entry->AnimIndex, 1, false, flags, 100);
                anim.SetMainAnim(init, entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
            }
        }
    }
    PoseAnimName = poseName;
}
