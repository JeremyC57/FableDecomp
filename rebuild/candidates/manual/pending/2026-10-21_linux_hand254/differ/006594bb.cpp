// Grade: DIFFER(363v362) -- control flow, call order and the parent-thing reloads match (same-TU impure bitset test + pure lower bounds); left: frame-slot order (retail name temp -8, crowd -0xc, player/key -0x10), the const GetTC result register (edi vs ecx) and the sound-def release form (retail mov ecx,[ebp-4] ; dec [ecx+4] ; jne)
// CTCVillage::UpdateCrowds() -- when the village owner's crowd interface allows a new awe (cheer) or lynch (boo) crowd event, stamp the event frame and play the matching CROWD_CHEER / CROWD_BOO sound of the owner's entity sound def through its sound player
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006594BB, length 362 bytes (`leave`, plain `ret`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed siblings (`push 42h ; pop ebx`).
// Callees: std::bitset<>::test 0x00410DE0; CVectorMap LowerBound const 0x004365B0 (interface 0x42) and non-const
//   0x0040F020 (interface 0x10, the sound player); CDefClassBase::GetSubDef<CEntitySoundDef> 0x0065F604; CSoundMap::
//   GetPSoundPair 0x0065FBA4; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the crowd
//   interface's event gate 0x00811380 (manifest "GetPActiveStrikeEvent" is propagated); GFGetCurrentFrame 0x0049D870;
//   CSoundInit() 0x0041C930; CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, const CSoundInit&, bool, bool, bool)
//   0x0066B280 (as landed CTCDoor::PlaySoundW 0x00730A00); the def's slot 1 on the last release.
// Data: "CROWD_CHEER" @ 0x0125A980, "CROWD_BOO" @ 0x0125A974.
// Layout: CTCBase ParentThing +0x04; Ego_r.pdb CTCVillage LastAweCrowdEventFrame +0x20 / LastLynchCrowdEventFrame +0x24
//   (before the narrowed STLport members, unshifted); retail CThing PBaseDef +0x70 and CEntitySoundDef SoundMap +0x28
//   (as the landed CTCDoor::PlaySoundW).
// Shape: the sound def pointer is declared first (zeroed up front, null-tested release at the end, `dec [ecx+4]`); the
//   crowd interface comes through the const GetTC (end kept unless a match is found), the sound player through the
//   non-const one; each name is a scoped temporary.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
class CSoundPair;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CBitset {                                        // std::bitset<274>
    unsigned long m_bits[9];
    bool test(unsigned long position) const;            // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
    __forceinline SEntry* Find(long interfaceType)
    {
        long type = interfaceType;
        SEntry* it = LowerBound(&type);
        SEntry* found = last;
        if (it != found && !(interfaceType < it->type))
            found = it;
        return found;
    }
    __forceinline const SEntry* Find(long interfaceType) const
    {
        long type = interfaceType;
        const SEntry* it = LowerBound(&type);
        const SEntry* found = last;
        if (it != found && !(interfaceType < it->type))
            found = it;
        return found;
    }
};

// Same-TU bodies of the template members this TU instantiates (std::bitset<>::test, the CVectorMap lower bounds): VC7.1
// sees they write no memory, so the zeroed sound def is still known null on the early exits.
#pragma auto_inline(off)
void __cdecl ThrowOutOfRange();                         // std::_Xran (the bitset range check)
__declspec(noinline) bool CBitset::test(unsigned long position) const
{
    if (position >= 274)
        ThrowOutOfRange();
    return (m_bits[position >> 5] & (1UL << (position & 31))) != 0;
}
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CSoundInit {
public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit();                                       // 0x0041C930
};
class CTCSoundPlayer {                                  // interface 0x10
public:
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool attach, bool a, bool b);   // 0x0066B280
};
class CTCCrowdEvents {                                  // interface 0x42 (descriptive)
public:
    bool CanStartEvent(long eventType, long lastEventFrame);   // 0x00811380
};
class CSoundMap {
public:
    const CSoundPair* GetPSoundPair(const CCharString& name) const;   // 0x0065FBA4
};
class CEntitySoundDef {
public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CSoundMap SoundMap;                                 // +0x28
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CDefClassBase {
public:
    template <class T> bool GetSubDef(CDefPointer<T>* out) const;   // 0x0065F604 for T = const CEntitySoundDef
};

struct CThing {
    unsigned char  _pad00[0x20];
    CBitset        m_present;                           // +0x20 (0x24 bytes)
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                            // +0x70 CDefPointer<const CThingBaseDef>::Object
    bool HasTC(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetTC(long type) { return (T*)m_interfaces.Find(type)->component; }
    template <class T> __forceinline T* GetTC(long type) const { return (T*)m_interfaces.Find(type)->component; }
};
long __fastcall GFGetCurrentFrame();                    // 0x0049D870

struct CTCVillage {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x18];
    long    LastAweCrowdEventFrame;                     // +0x20
    long    LastLynchCrowdEventFrame;                   // +0x24
    void UpdateCrowds();
};

void CTCVillage::UpdateCrowds()
{
    CDefPointer<CEntitySoundDef> soundDef;
    const CThing* parent = ParentThing;
    if (!parent->HasTC(0x42))
        return;
    CTCCrowdEvents* crowd = parent->GetTC<CTCCrowdEvents>(0x42);
    CThing* owner = ParentThing;
    if (!owner->HasTC(0x10))
        return;
    CTCSoundPlayer* player = owner->GetTC<CTCSoundPlayer>(0x10);
    if (ParentThing->PBaseDef->GetSubDef(&soundDef)) {
        const CSoundMap& sounds = soundDef->SoundMap;
        const CSoundPair* cheer = sounds.GetPSoundPair(CCharString("CROWD_CHEER"));
        if (crowd->CanStartEvent(0xe, LastAweCrowdEventFrame) && cheer) {
            LastAweCrowdEventFrame = GFGetCurrentFrame();
            player->PlayAttachedSample(*cheer, CSoundInit(), true, false, false);
        }
        const CSoundPair* boo = sounds.GetPSoundPair(CCharString("CROWD_BOO"));
        if (crowd->CanStartEvent(0xf, LastLynchCrowdEventFrame) && boo) {
            LastLynchCrowdEventFrame = GFGetCurrentFrame();
            player->PlayAttachedSample(*boo, CSoundInit(), true, false, false);
        }
    }
}
