#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::ClearSoundEffects() -- stop the charging sound on the hero's sound component (interface 0x10) and forget its ID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B668, length 114 bytes.
// Callees: CPlayerManager::GetMainPlayer() @ 0x00449970 and CPlayer::GetCharacterThing() @ 0x00487DC0 (manifest labels on both are
// propagated junk), IS_THING_ALIVE<CThing*>(CThing* const&) @ 0x0043DBD0 (`__fastcall`, by reference), std::bitset::test @ 0x00410DE0
// and CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, as the landed 0x007BEA08 family),
// the component's StopSound(unsigned long) @ 0x0066AD90 (manifest "CGameScriptThing::SetToKillOnLevelUnload" is propagated).
// Layout: CDrawBase PlayerManager +8; CDrawQuickAccessIcons ChargingSoundID +0x28 (PDB +0x2c); CThing interface bitset +0x20,
// interface map +0x44.
// Shape: frame-pointer size TU; the sound ID test is `> 0` on an unsigned (`jbe`); the hero is a creature pointer kept in esi, and
// IS_THING_ALIVE<CThing*> binds its reference to a converted CThing* TEMPORARY in the frame slot (binding the local itself forces
// it to memory and re-reads it), which the interface key temporary then reuses; the presence test is a plain inline over the
// out-of-line bitset::test and the lookup a `__forceinline` template RETURNING the component, so it is loaded straight into ecx
// for the call (the out-pointer GetInterface form goes through eax); the ID is cleared with `and [edi+28h],0`.
struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThing*)

struct CThingCreatureBase : CThing { };

struct CPlayer {
    CThingCreatureBase* GetCharacterThing();       // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};

struct CTCSound {                                  // the thing's interface 0x10
    void StopSound(unsigned long soundID);         // 0x0066AD90
};

enum { TC_SOUND = 0x10 };

namespace NPlayerGui {

struct CDrawQuickAccessIcons {
    void*           _vtable;                       // +0x00
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    unsigned char   _base0c[0x28 - 0x0c];          // World .. PrimitiveHandles
    unsigned long   ChargingSoundID;               // +0x28
    void ClearSoundEffects();
};

void CDrawQuickAccessIcons::ClearSoundEffects()
{
    if (ChargingSoundID > 0) {
        CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetCharacterThing();
        if (IS_THING_ALIVE<CThing*>(hero)) {
            if (hero->HasInterface(TC_SOUND)) {
                hero->GetInterfaceUnchecked<CTCSound>(TC_SOUND)->StopSound(ChargingSoundID);
                ChargingSoundID = 0;
            }
        }
    }
}

}
