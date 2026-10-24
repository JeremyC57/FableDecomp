// CTCPhysicalShieldSpell::FinalCleanup() -- end of the physical-shield power: clear this spell's mesh effect from the caster's graphic appearance, mark the spell unpausable and stop its looping sound
// Grade: DIFFER(138v138) -- register only: the second (0x10 sound-player) lookup loads ParentThing into the dead `this` register (mov edi,[edi+4]) where retail uses eax; same lane gap as 0079d2d0 (tried: forceinline StopSound helper, named sound/caster locals)
// Leaf: ?FinalCleanup@CTCPhysicalShieldSpell@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0079EAA0, 138 bytes (+ int3 padding to 144), __thiscall, plain `ret`.
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
    SEntry* LowerBound(const long* type);                // 0x0040F020
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
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
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
