// CTCKrakenTentacle::GetShouldStrike() -- whether the tentacle may strike now: it is allowed to, its kraken is alive, the owning creature is in neither of the two blocking modes, and more frames than the def's time between attacks have passed since the last strike
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F1410, length 224 bytes (+ int3 padding); speed TU (no frame pointer, esp-relative frame).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls on PKraken, the third unused), CVectorMap::LowerBound const
// @ 0x004365B0 (the inline GetInterface for type 0x31 = CTCCreatureModeManager; the bitset test folds to `test [thing+24h],20000h`),
// CTCCreatureModeManager::HasMode(unsigned long) @ 0x0073A8A0 (modes 0x54 and 0x53), CWorld::GetWorldFrame @ 0x0049D870;
// g_framesPerSecond @ 0x01375550. CThing: components bitset +0x20, interface map +0x44, Dead = +0x91 bit 0.
// Layout (PDB = retail): ParentThing +0x04, PTentacleDef +0x0C, PKraken +0x10, CanStrike +0x28, LastStrikeTick +0x30;
// CKrakenTentacleDef::TentacleTimeBetweenAttacks +0x44.
// Shape: the mode manager comes from the unchecked out-parameter GetInterface (the absent path reads the frame word); the frame rate
// is read before the world frame call and both integers go through the frame for `fild`; the compare is `elapsed > fps * delay`
// (`fcompp ; test ah,5 ; jp` to the false tail); the kraken pointer is fetched once more, unused, before the frame maths. The body is
// one nested condition with a single `return false`, so the five failure edges share the one tail after the prologue's pushes.
extern "C" int  __cdecl puts(const char* text);    // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CPresenceBits {                             // std::bitset<274>
    unsigned long words[9];
    bool test(unsigned long position) const
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (words[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits UsingComponents;                   // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char Dead : 1;                          // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCCreatureModeManager {                      // interface type 0x31
    bool HasMode(unsigned long mode);                // 0x0073A8A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};
struct CKrakenTentacleDef {
    unsigned char _pad00[0x44];
    float TentacleTimeBetweenAttacks;                // +0x44
};
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
struct CWorld {
    static long GetWorldFrame();                     // 0x0049D870
};
extern long g_framesPerSecond;                       // 0x01375550

struct CTCKrakenTentacle {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x04];
    CDefPointer<CKrakenTentacleDef> PTentacleDef;    // +0x0c
    CBaseIntelligentPointer PKraken;                 // +0x10
    unsigned char _pad18[0x28 - 0x18];
    bool          CanStrike;                         // +0x28
    bool          Tracking;                          // +0x29
    bool          IsStriking;                        // +0x2a
    long          QueuedStrikeTick;                  // +0x2c
    long          LastStrikeTick;                    // +0x30
    bool GetShouldStrike();
};
bool CTCKrakenTentacle::GetShouldStrike()
{
    if (CanStrike && PKraken.GetPItem() && !PKraken.GetPItem()->Dead) {
        CTCCreatureModeManager* modes;
        ParentThing->GetInterface(&modes, 0x31);
        if (!modes->HasMode(0x54) && !modes->HasMode(0x53)) {
            CThing* kraken = PKraken.GetPItem();
            long framesPerSecond = g_framesPerSecond;
            CKrakenTentacleDef* def = PTentacleDef.Object;
            long elapsed = CWorld::GetWorldFrame() - LastStrikeTick;
            if ((float)elapsed > (float)framesPerSecond * def->TentacleTimeBetweenAttacks)
                return true;
        }
    }
    return false;
}
