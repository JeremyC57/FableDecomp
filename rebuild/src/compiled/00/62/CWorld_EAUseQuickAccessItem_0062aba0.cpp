// CWorld::EAUseQuickAccessItem(const CGameEvent*) -- game-event action: unless the player is in mode 0x2F or its controlled creature's interface 0xA7 blocks it, read the slot from the event and have the hero's inventory use it (-2 = the guild seal, context-sensitive slots first, then quick-access slots)
// Leaf: EAUseQuickAccessItem
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062ABA0, 491 bytes (+ int3 padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?EAUseQuickAccessItem@CWorld@@QAEXPBVCGameEvent@@@Z.
// Callees: CWorld::GetPlayer(long) 0x0049D850 (x2), CPlayer::HasMode 0x00634160, CPlayer::PeekPControlledCreature
//   0x00487DD0 (x2), CPlayer::GetPControlledCreature 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0
//   (interface 0xA7) and 0x0040F020 (interface 0x11 = CTCInventory) -- distinct retail copies, CGameEvent::GetData
//   0x009F17A0, CMemoryBuffer::Read 0x009F18F0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   ("OBJECT_GUILD_SEAL_1" 0x01251B4C), CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CGamePlayerInterface::GetNumContextSensitiveSlots 0x00445960 (x2), CTCInventory::OnItemSelected (vtable slot 33,
//   +0x84), UseContextSensitiveItem 0x00583A92, UseQuickAccessItem 0x005840A6, the current action's slot 39 (+0x9C),
//   __security_check_cookie 0x00BFE9F9.
// Layout: local models at retail offsets, as the landed siblings EAUseQuickAccessItemInCutscene 0x0062AD90 and
//   EAChargeQuickAccessItem 0x00629B10 (engine/ headers carry the PDB's 16-byte containers): CThing interface bitset
//   +0x20, interface map +0x44, Dead +0x91 bit 0, CThingCreatureBase PCurrentAction +0x100 (PDB +0x11C); CPlayer
//   PlayerInterface +0xC; CWorld DefinitionManager +0x10.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline long ReadLong() { long value; Read(4, &value); return value; }
};
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};

class CTCBase;
struct CInterfaceMap {                             // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
    SEntry* LowerBound2(const long* type) const;   // 0x004365B0 (the const LowerBound copy)
};
#pragma auto_inline(off)
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
#pragma auto_inline(on)

class CCreatureActionBase { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24();
    virtual void Slot25(); virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void Slot30(); virtual void Slot31(); virtual void Slot32(); virtual void Slot33(); virtual void Slot34();
    virtual void Slot35(); virtual void Slot36(); virtual void Slot37(); virtual void Slot38();
    virtual bool BlocksQuickAccess();              // slot 39 (+0x9C)
};

struct CTCInterfaceA7 {                            // interface 0xA7: a flag at +0xC vetoes item use
    unsigned char _pad[0xc];
    bool Blocking;                                 // +0xC
};

class CTCInventory { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24();
    virtual void Slot25(); virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void Slot30(); virtual void Slot31(); virtual void Slot32();
    virtual void OnItemSelected(long defIndex, bool* handled);   // slot 33 (+0x84)
    void UseContextSensitiveItem(long slot);       // 0x00583A92
    void UseQuickAccessItem(long slot);            // 0x005840A6
};

class CThingCreatureBase { public:
    void* m_pVtable;
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTC2(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound2(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* PCurrentAction;           // +0x100 CCountedPointer<CCreatureActionBase>::Data
};
class CThingPlayerCreature : public CThingCreatureBase { };

class CGamePlayerInterface { public:
    unsigned long GetNumContextSensitiveSlots() const;   // 0x00445960
};
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
class CPlayer { public:
    unsigned char _pad00[0xc];
    CGamePlayerInterface* PlayerInterface;         // +0xC
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CDefinitionManager { public:
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};
class CWorld { public:
    unsigned char _pad00[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAUseQuickAccessItem(const CGameEvent* event);
};

void CWorld::EAUseQuickAccessItem(const CGameEvent* event)
{
    CPlayer* eventPlayer = GetPlayer(event->Player);
    if (eventPlayer->HasMode(PLAYER_MODE_0x2F))
        return;
    CPlayer* controller = GetPlayer(event->Player);
    const CThingCreatureBase* controlled = controller->PeekPControlledCreature();
    if (controlled && !controlled->Dead) {
        const CThingCreatureBase* creature = controller->PeekPControlledCreature();
        if (creature->HasTC(0xA7) && creature->GetTC2<CTCInterfaceA7>(0xA7)->Blocking)
            return;
    }
    long number = event->Player;
    CPlayer* player = GetPlayer(number);
    CThingCreatureBase* hero = player->GetPControlledCreature();
    if (!hero || hero->Dead)
        return;
    if (hero->PCurrentAction && hero->PCurrentAction->BlocksQuickAccess())
        return;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    if (!hero->HasTC(0x11))
        return;
    CTCInventory* inventory = hero->GetTC<CTCInventory>(0x11);
    if (!inventory)
        return;
    long slot = buffer.ReadLong();
    if (slot == -2) {
        long defIndex = DefinitionManager->GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
        bool handled;
        inventory->OnItemSelected(defIndex, &handled);
        return;
    }
    if (slot > -1) {
        if (slot < (long)player->PlayerInterface->GetNumContextSensitiveSlots())
            inventory->UseContextSensitiveItem(slot);
        else
            inventory->UseQuickAccessItem(slot - player->PlayerInterface->GetNumContextSensitiveSlots());
    }
}