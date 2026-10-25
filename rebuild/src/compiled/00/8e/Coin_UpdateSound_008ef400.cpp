// Coin::UpdateSound(bool muted) -- tavern-game coin slide sound: stop it when the coin is at rest, otherwise start "TAVERN_GAME_COIN_SLIDE" if needed and set its pitch and (muted ? 0 : 1) volume
// Leaf: ?UpdateSound@Coin@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EF400, 288 bytes, `ret 4`.
// Ego_r.pdb: ?UpdateSound@Coin@@QAEX_N@Z; Coin (0x3C, packed) = { ... +0x0C velocity (PDB padding words), bool Owned +0x24,
//   CIntelligentPointer<CThing> Thing +0x25, float Radius +0x30, int Owner +0x34, ulong Sound +0x38 }.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   @ 0x0040F020 (the inline CThing::GetTC, interface 16 = CTCSoundPlayer), CTCSoundPlayer::StopAttachedSound @ 0x0066ADD0,
//   the play method @ 0x0066BD00 (name, false, 0, 0, -1), SetSpecificSamplePitch @ 0x0066AFA0 and SetSpecificSampleVolume
//   @ 0x0066AFF0 (Ego_r.pdb `(unsigned long, float)` pair; the emitter forwards them to vtable slots 6 / 7),
//   CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_COIN_SLIDE" 0x01288250), ~CCharString @ 0x0099EAE0.
//   Globals: g_InvalidSoundHandle 0x0129D25C, 0.0f 0x0122DEDC.
// Shape: the GetTC out-pointer is not null-initialised and its result is not tested (retail reads the dead `muted` slot on
//   the absent path, GOTCHAS 24l); the rest test is the squared speed against the pooled 0.0f; the volume is formed in the
//   `muted` slot. Speed-optimised (default) TU.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern const unsigned long g_InvalidSoundHandle;       // 0x0129D25C
extern const float g_fZero;                            // 0x0122DEDC (0.0f)

class CTCBase;
class CTCSoundPlayer {                                 // the thing's interface-16 component
public:
    void StopAttachedSound(unsigned long sound);                                         // 0x0066ADD0
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00
    void SetSpecificSamplePitch(unsigned long sound, float pitch);                       // 0x0066AFA0
    void SetSpecificSampleVolume(unsigned long sound, float volume);                     // 0x0066AFF0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
class CBaseObject;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    template <class T> __forceinline bool GetTC(long interfaceType, T** ppOut)
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
#pragma pack(push, 1)
class CBaseIntelligentPointer {                        // 11 bytes inside the packed Coin (Ego_r.pdb Thing +0x25, Radius +0x30)
public:
    void* __vftable;
    unsigned char _pad04[7];
    CBaseObject* GetPItem() const;                     // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class C2DVector {
public:
    float X;
    float Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
};

#pragma pack(push, 1)
class Coin {                                           // 0x3C bytes
public:
    unsigned char _pad00[0x0C];
    C2DVector     Velocity;                            // +0x0C
    unsigned char _pad14[0x10];
    bool          Owned;                               // +0x24
    CIntelligentPointer<CThing> Thing;                 // +0x25
    float         Radius;                              // +0x30
    long          Owner;                               // +0x34
    unsigned long Sound;                               // +0x38
    void UpdateSound(bool muted);
};
#pragma pack(pop)

void Coin::UpdateSound(bool muted)
{
    if (Thing.Get() == 0)
        return;
    CTCSoundPlayer* player;
    Thing->GetTC(16, &player);
    if (Velocity.GetLengthSquared() == g_fZero) {
        if (Sound != g_InvalidSoundHandle) {
            player->StopAttachedSound(Sound);
            Sound = g_InvalidSoundHandle;
        }
        return;
    }
    if (Sound == g_InvalidSoundHandle) {
        CCharString name("TAVERN_GAME_COIN_SLIDE");
        Sound = player->PlaySound(name, false, 0, 0, -1);
    }
    player->SetSpecificSamplePitch(Sound, 1.0f);
    player->SetSpecificSampleVolume(Sound, muted ? 0.0f : 1.0f);
}