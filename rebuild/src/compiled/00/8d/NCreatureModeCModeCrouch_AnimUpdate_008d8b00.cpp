// NCreatureMode::CModeCrouch::AnimUpdate(CAnimateThing&) -- pick the sneak animation (moving: ST_SNEAK / ST_SNEAK_UPRIGHT, standing: the _POSE variants; upright while the stealth component says the creature is viewable) and start it as the main animation unless it is already playing, then remember its name
// Leaf: ?AnimUpdate@CModeCrouch
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8B00, 480 bytes, `ret 4`; vtable 0x01267B34 slot 2.
// Name: FableWin.pdb CModeCrouch (slot 2 = AnimUpdate, as the landed CModeBlock::AnimUpdate 0x008D8DD0).
// Callees: CVectorMap::LowerBound (const) @ 0x004365B0 (inlined const GetInterface, CTCStealth = type 0x36, bit 22 of the +0x24
//   dword), CCharString(const char*, long) @ 0x0099EBF0 ("ST_SNEAK_UPRIGHT" 0x01285958, "ST_SNEAK" 0x01258F14,
//   "ST_SNEAK_UPRIGHT_POSE" 0x01285940, "ST_SNEAK_POSE" 0x01285930), CCharString::operator!= @ 0x0099E900 (manifest label
//   less<CCharString>::operator() is wrong), CThingCreatureBase::GetAnimFromName @ 0x00662A00, CAnimationEntry::
//   GetAnimateThingFlags @ 0x005DC390 / GetAnimationTransitionInTime @ 0x005DC1F0 / GetTimeToBlendIn @ 0x005DC230,
//   CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim @ 0x0070D580, CCharString::operator= @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0.
// Globals: 0.0001f @ 0x0129BA3C (the moving threshold).
// Layout: Ego_r.pdb CModeCrouch (0x34): CModeBase 0x1c (ParentThing +4), SneakAnimName +0x1c, Filter +0x20. CTCStealth::Viewable
//   +0x0c. CThingCreatureBase::MovementVector +0x10c (PDB +0x130 less the CThingPhysical/vector/list shrink). CAnimateThing and
//   CAnimInit as the landed CModeBlock::AnimUpdate.
// Shape: the creature is a named local (esi, reused for GetAnimFromName); the planar speed is `sqrt(X*X + Y*Y)` straight on
//   the members, as the landed CModeStandBase::AnimUpdate 0x00862EC0 (Y loaded first, both kept on the x87 stack; a C2DVector
//   copy loads X first); each branch builds its own name and VC merges the shared `SneakAnimName = name ; ~name` tail; the
//   blend-in thing is ParentThing re-read after GetAnimFromName (a named local there). 4 grades.
#include <math.h>
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const CCharString& other) const;   // 0x0099E900
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
    template<class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBlendPair {                        // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};

class CAnimInit { public:                         // 0x2c bytes
    long          Mode;                    // +0x00
    unsigned char AnimType;                // +0x04
    bool          Flag05;                  // +0x05
    long          AnimIndex;               // +0x08
    float         Scales[3];               // +0x0c
    CBlendPair    Blend;                   // +0x18
    unsigned long Flags;                   // +0x20
    long          Arg24;                   // +0x24
    long          Percent;                 // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

class CAnimationEntry { public:
    long AnimIndex;                        // +0x00
    unsigned long GetAnimateThingFlags();  // 0x005DC390
    long GetAnimationTransitionInTime();   // 0x005DC1F0
    long GetTimeToBlendIn(CThing* thing);  // 0x005DC230
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0x10c - 0x4c];
    C3DVector     MovementVector;          // +0x10c
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};

class CTCStealth { public:                        // interface type 0x36
    unsigned char _pad00[0x0c];
    bool          Viewable;                // +0x0c
};

class CAnimateThing { public:
    long          UpdateMode;              // +0x00
    void*         AnimsBegin;              // +0x04
    void*         AnimsEnd;                // +0x08
    unsigned char _pad0c[0x44];
    unsigned char CurrentAnimType;         // +0x50
    unsigned char CurrentAnimFlags;        // +0x51
    void SetMainAnim(const CAnimInit& init, long blendIn, long transition);   // 0x0070D580
    __forceinline bool IsPlayingMainAnimType(unsigned char type) const
    {
        return AnimsBegin != AnimsEnd && !(CurrentAnimFlags & 2) && CurrentAnimType == type;
    }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {

struct CModeCrouch : CModeBase {           // vtable 0x01267B34
    CCharString   SneakAnimName;           // +0x1c
    unsigned char Filter[0x14];            // +0x20 CAnimFilterOf2Groups
    void AnimUpdate(CAnimateThing& animate);
};

void CModeCrouch::AnimUpdate(CAnimateThing& animate)
{
    CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
    bool upright = true;
    const CTCStealth* stealth;
    if (creature->GetInterface(&stealth, 0x36) && !stealth->Viewable)
        upright = false;
    float speed = (float)sqrt(creature->MovementVector.X * creature->MovementVector.X
        + creature->MovementVector.Y * creature->MovementVector.Y);
    if (speed > 0.0001f) {
        CCharString name(upright ? "ST_SNEAK_UPRIGHT" : "ST_SNEAK");
        if (name != SneakAnimName || !animate.IsPlayingMainAnimType(0x0e)) {
            CAnimationEntry* entry = creature->GetAnimFromName(name, 0);
            CThing* thing = ParentThing;
            animate.SetMainAnim(CAnimInit(0x0e, 0, entry->AnimIndex, 2, true, entry->GetAnimateThingFlags(), 100),
                entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
        }
        SneakAnimName = name;
    }
    else {
        CCharString name(upright ? "ST_SNEAK_UPRIGHT_POSE" : "ST_SNEAK_POSE");
        if (name != SneakAnimName || !animate.IsPlayingMainAnimType(0x0f)) {
            CAnimationEntry* entry = creature->GetAnimFromName(name, 0);
            CThing* thing = ParentThing;
            animate.SetMainAnim(CAnimInit(0x0f, 0, entry->AnimIndex, 1, true, entry->GetAnimateThingFlags(), 100),
                entry->GetTimeToBlendIn(thing), entry->GetAnimationTransitionInTime());
        }
        SneakAnimName = name;
    }
}

}