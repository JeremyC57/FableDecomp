// CCreatureAction_PerformExpression::FrameUpdate() -- track whether the expression's quick-access / context-sensitive button is still held; run the base frame update and, on the frame the action is performed, play the expression's sound (the looping sound when the button is held, its handle kept)
// Grade: DIFFER(527v527) -- register allocation: retail keeps the input interface in ebx and spills the inventory to the frame slot; ours keeps the inventory in ebx and spills the interface
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00846BC0, length 527 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound const @ 0x004365B0 (inventory, type 0x11)
//          and @ 0x0040F020 (sound, type 0x10), CTCInventory::GetQuickAccessItemButtonIndex @ 0x00581FD1 /
//          GetContextSensitiveItemButtonIndex @ 0x00581D4E, CThing::GetPlayer @ 0x004C7A30, CPlayer::GetPlayerNumber
//          @ 0x00487510, CGamePlayerInterface::IsQuickAccessEventInQueue @ 0x004460C0 / IsContextSensitiveEventInQueue
//          @ 0x004461D0, CCreatureActionBase::GetPerformedAction @ 0x006925A0, CCreatureActionBase::FrameUpdate
//          @ 0x00694020, CDefString::operator!=(const char*) @ 0x005FA740, CDefString::operator CCharString @ 0x00415DD0,
//          CDefStringTable::GetString @ 0x009D49B0 (0x013CA828), CTCSound::PlaySound @ 0x0066BD00, ~CCharString @ 0x0099EAE0.
// Layout: Ego_r.pdb CCreatureAction_PerformExpression less 0xc (retail CCreatureActionBase 0xa8): PExpressionDef +0xa8,
//   ExpressionItemDefIndex +0xb4, ButtonHeld +0xbb, SoundEffectID +0xbc; CExpressionDef sound names at +0x68 / +0x6c
//   (PDB SoundCriteria / LoopSoundCriteria +0x6c / +0x70: the retail def base is 4 smaller); CPlayer's interface +0xc.
struct CCharString {
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id) const;                    // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                     // 0x013CA828
struct CDefString {
    long Index;
    bool operator!=(const char* text) const;                 // 0x005FA740
    operator CCharString() const;                            // 0x00415DD0
};

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    SEntry* LowerBound(const long* type) const;              // 0x004365B0
};
struct CGamePlayerInterface {
    bool IsQuickAccessEventInQueue(long button, long eventType, long player) const;         // 0x004460C0
    bool IsContextSensitiveEventInQueue(long button, long eventType, long player) const;    // 0x004461D0
};
struct CPlayer {
    unsigned char         _pad00[0xc];
    CGamePlayerInterface* PlayerInterface;                   // +0xc
    long GetPlayerNumber() const;                            // 0x00487510
};
struct CTCInventory {
    long GetQuickAccessItemButtonIndex(long item);           // 0x00581FD1
    long GetContextSensitiveItemButtonIndex(long item);      // 0x00581D4E
};
struct CTCSound {
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    CPlayer* GetPlayer() const;                              // 0x004C7A30
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
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};

struct CExpressionDef {
    unsigned char _pad00[0x68];
    long          SoundCriteria;                             // +0x68
    CDefString    LoopSoundCriteria;                         // +0x6c
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

struct CCreatureActionBase {
    virtual void FrameUpdate();                              // 0x00694020
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    bool GetPerformedAction() const;                         // 0x006925A0
};
struct CCreatureAction_PerformExpression : CCreatureActionBase {
    CDefPointer<CExpressionDef> PExpressionDef;              // +0xa8
    CBaseIntelligentPointer     PPerformToThing;             // +0xac
    long                        ExpressionItemDefIndex;      // +0xb4
    bool                        EmoteIconAdded;              // +0xb8
    bool                        ReplacedAction;              // +0xb9
    bool                        Extended;                    // +0xba
    bool                        ButtonHeld;                  // +0xbb
    unsigned long               SoundEffectID;               // +0xbc
    virtual void FrameUpdate();
};

void CCreatureAction_PerformExpression::FrameUpdate()
{
    ButtonHeld = false;
    if (ExpressionItemDefIndex > 0) {
        CTCInventory* inventory;
        const CThing* creature = PCreature.GetPItem();
        if (creature->GetInterface(&inventory, 0x11)) {
            long button = inventory->GetQuickAccessItemButtonIndex(ExpressionItemDefIndex);
            if (button != -1) {
                CPlayer* player = PCreature.GetPItem()->GetPlayer(); CGamePlayerInterface* input = player->PlayerInterface;
                if (input->IsQuickAccessEventInQueue(button, 2, PCreature.GetPItem()->GetPlayer()->GetPlayerNumber()))
                    ButtonHeld = true;
            }
            if (!ButtonHeld) {
                button = inventory->GetContextSensitiveItemButtonIndex(ExpressionItemDefIndex);
                if (button != -1) {
                    CPlayer* player = PCreature.GetPItem()->GetPlayer(); CGamePlayerInterface* input = player->PlayerInterface;
                    if (input->IsContextSensitiveEventInQueue(button, 2, PCreature.GetPItem()->GetPlayer()->GetPlayerNumber()))
                        ButtonHeld = true;
                }
            }
        }
    }
    if (!GetPerformedAction()) {
        CCreatureActionBase::FrameUpdate();
        CTCSound* sound;
        if (GetPerformedAction() && PCreature.GetPItem()->GetInterface(&sound, 0x10)) {
            if (ButtonHeld && PExpressionDef->LoopSoundCriteria != "")
                SoundEffectID = sound->PlaySound(PExpressionDef->LoopSoundCriteria, 1, 0, 0, -1);
            else {
                CCharString name = g_DefStringTable.GetString(PExpressionDef->SoundCriteria);
                sound->PlaySound(name, 1, 0, 0, -1);
            }
        }
    } else
        CCreatureActionBase::FrameUpdate();
}
