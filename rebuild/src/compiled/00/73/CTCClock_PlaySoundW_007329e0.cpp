// CTCClock::PlaySoundW(const CCharString& name) -- play the clock's named sound: look up the "STF_<name>" / "STL_<name>" first/last sample ids in the clock definition's sound map and, when both exist, play a random sample of that range through the owner's sound player
// Leaf: ?PlaySoundW@CTCClock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007329E0, 235 bytes (240 with padding), `ret 4`.
// Label correction: catalog "CTCCreature_PlayMorphSound" (autoname) -- the body sits between CTCClock::FrameUpdate
//   0x00732950 and the CTCClock helpers, reads CTCClock::PDef (+0x0c) and CClockDef::Sound (+0x28), and Ego_r.pdb has
//   ?PlaySoundW@CTCClock@@IAEXABVCCharString@@@Z (`PlaySound` through the <windows.h> macro) and
//   ?GetSound@CClockDef@@QBEJABVCCharString@@@Z.  No direct caller survives in retail (inlined at its call sites).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (interface 0x10 = CTCSoundPlayer, bit 16 of
//   the presence word +0x20), operator+(const char*, const CCharString&) 0x0099F690 (__fastcall, hidden result in ecx),
//   the CCharString map's _M_find 0x0042B467 (returns the node; landed model 0x006E3780), ~CCharString 0x0099EAE0,
//   CSoundInit::CSoundInit() 0x0041C930, CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool,
//   bool, bool) 0x0066B280 (landed).
// Layout: CTCClock (Ego_r.pdb, 0x14): CTCBase, PDef +0x0c, LastTimeUpdated +0x10; CClockDef::Sound
//   map<CCharString,long> +0x28 (STLport: header node pointer first; node key +0x10, value +0x14).
// Shape: CClockDef::GetSound inlined (`it != end() ? it->second : 0`); each concatenation temporary is built in the
//   dead argument slot and dies after its lookup; the sound player is spilled to the frame across the lookups.
class CTCBase;
struct CCharString {
    void* PStringData;
    ~CCharString();                                                                   // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) {}
};
struct CSoundInit {
    float Volume, Pitch;
    long  Pan, NoPlays, PlaybackType, GainFaderBits, PitchFaderBits, OverrideID;
    CSoundInit();                               // 0x0041C930
};
struct CTCSoundPlayer {
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool flag, bool loop, bool attached);   // 0x0066B280
};

struct CSoundMap {                              // STLport map<CCharString, long>
    struct _Node {
        long        _M_color;
        _Node*      _M_parent;
        _Node*      _M_left;
        _Node*      _M_right;
        CCharString first;                      // +0x10
        long        second;                     // +0x14
    };
    _Node*        _M_header;                    // +0x00 end()
    unsigned long _M_node_count;                // +0x04
    _Node* _M_find(const CCharString& key) const;   // 0x0042B467
};
struct CClockDef {
    unsigned char _pad00[0x28];
    CSoundMap     Sound;                        // +0x28
    long GetSound(const CCharString& name) const
    {
        CSoundMap::_Node* it = Sound._M_find(name);
        if (it != Sound._M_header)
            return it->second;
        return 0;
    }
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};

class CTCClock {
public:
    virtual void v00();
    CThing*                      ParentThing;   // +0x04 (CTCBase)
    bool                         ToBeRemoved;   // +0x08
    bool                         IsInGlobalUpdateTCs;
    CDefPointer<const CClockDef> PDef;          // +0x0c
    float                        LastTimeUpdated;   // +0x10
    void PlaySoundW(const CCharString& name);
};

void CTCClock::PlaySoundW(const CCharString& name)
{
    if (ParentThing->HasTC(0x10)) {
        CTCSoundPlayer* player = (CTCSoundPlayer*)ParentThing->GetTC(0x10);
        if (PDef) {
            long first = PDef->GetSound("STF_" + name);
            long last = PDef->GetSound("STL_" + name);
            if (first && last)
                player->PlayAttachedSample(CSoundPair(first, last), CSoundInit(), true, false, false);
        }
    }
}