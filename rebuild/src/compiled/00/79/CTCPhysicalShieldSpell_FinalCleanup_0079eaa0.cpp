// CTCPhysicalShieldSpell::FinalCleanup() -- end of the physical-shield power: clear this spell's mesh effect from the caster's graphic appearance, mark the spell unpausable and stop its looping sound
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined GetInterface tail is the F88 form (`if (found != map->last && !(type < found->type)) it = found; else it = map->last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component lands straight in the register retail uses; FinalCleanup is protected as in Ego_r.pdb
// Leaf: ?FinalCleanup@CTCPhysicalShieldSpell@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079EAA0, 138 bytes (+ int3 padding to 144), __fastcall, plain `ret`.
// Identity: CTCPhysicalShieldSpell's OnRemove / OnKill vtable slots are `jmp` stubs 0x0079EC80 / 0x0079EC70 to it (landed as
//   forwarders), as for CTCDivineWrath / CTCUnholyPower / CTCBulletTime; Ego_r.pdb `?FinalCleanup@CTCPhysicalShieldSpell@@IAEXXZ`.
//   The catalog's "CThing_OnDestroy" is descriptive.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inline GetInterface: 0x5B = CTCGraphicAppearance,
//   bitset word +0x28 bit 27; 0x10 = the sound player, word +0x20 bit 16), CTCGraphicAppearance::ClearMeshEffect(void* key)
//   0x004BFFC0 (key = this, as CTCMultiArrow::RemoveEffectOnWeapon), CTCSoundPlayer::StopSample(unsigned long) 0x0066ADD0.
// Layout: Ego_r.pdb = retail: LoopingSound +0x40, Pausable +0x44.
// Shape: the appearance through the out-parameter GetInterface; `Pausable = false` then the inline StopSound(LoopingSound)
//   (the sound id is read into ebx before the flag store, the lookup re-reads the caster); the out-of-line StopSound at
//   0x0079DDB0 (called by the landed Pause) is inlined here.
class CTCBase;

struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                          // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* found = map->LowerBound(&type);
            CInterfaceMap::SEntry* it;
            if (found != map->last && !(interfaceType < found->type))
                it = found;
            else
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCGraphicAppearance {                             // interface 0x5B
public:
    void ClearMeshEffect(void* key);                     // 0x004BFFC0
};

class CTCSoundPlayer {                                   // interface 0x10
public:
    void StopSample(unsigned long sampleID);             // 0x0066ADD0
};

class CTCPhysicalShieldSpell {
public:
    virtual ~CTCPhysicalShieldSpell();
    CThing*       ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x40 - 0x08];
    unsigned long LoopingSound;                          // +0x40
    bool          Pausable;                              // +0x44
    bool          Paused;                                // +0x45
    bool          CutSceneCancelled;                     // +0x46

    __forceinline void StopSound(unsigned long sound)    // out of line at 0x0079DDB0
    {
        CTCSoundPlayer* player;
        if (ParentThing->GetInterface(&player, 0x10))
            player->StopSample(sound);
    }
protected:
    void FinalCleanup();
};

void CTCPhysicalShieldSpell::FinalCleanup()
{
    CTCGraphicAppearance* appearance;
    if (ParentThing->GetInterface(&appearance, 0x5b))
        appearance->ClearMeshEffect(this);
    Pausable = false;
    StopSound(LoopingSound);
}