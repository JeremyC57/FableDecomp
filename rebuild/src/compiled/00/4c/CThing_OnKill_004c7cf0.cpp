// CThing::OnKill() -- tell the quest manager the thing is deactivated (passing whether its level is unloading), call OnKill on every component with component adds forbidden, then notify the thing manager of the kill
// Leaf: ?OnKill@CThing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7CF0, 90 bytes. CThing vtable 0x01239ABC slot 12.
// Callees: CQuestManager::OnDeactivateEntity(CThing*, bool) @ 0x004AFB00 on the quest manager @ 0x013B89FC;
// for_each<pair<ETCInterfaceType, CTCBase*>*, CCallOnKill> @ 0x004CB9F0 (SGI algorithm built /Gr: first in ecx,
// last in edx, the 1-byte functor by value on the stack; manifest label "_Copy_backward<CPixelFormat*>" is
// propagated); CThingManager::OnThingKill(CThing*) @ 0x00520C80 on the thing manager @ 0x013B8A1C.
// Shape: the landed CThing::OnDie (0x004C7D50) model: Components first/last at +0x44/+0x48, AddingTCsPermitted bit 6
// of +0x93 cleared around the for_each with a value-initialised functor (`mov byte ptr [esp+4],0` into the
// `push ecx` slot). LevelIsUnloading (bit 4 of +0x93) is widened to the bool argument with
// `xor eax,eax ; mov al ; shr al,4 ; and eax,0FFFFFF01h`.
class CTCBase;
class CThing;

struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>, 8 bytes
    long     first;
    CTCBase* second;
};

class CCallOnKill { public:                                 // empty functor: p.second->OnKill()
    void operator()(SComponentPair& p) const;
};

CCallOnKill __fastcall for_each(SComponentPair* first, SComponentPair* last, CCallOnKill f);   // 0x004CB9F0

class CQuestManager { public:
    void OnDeactivateEntity(CThing* thing, bool levelUnloading);   // 0x004AFB00
};

class CThingManager { public:
    void OnThingKill(CThing* thing);                 // 0x00520C80
};

extern CQuestManager* g_pQuestManager;              // 0x013B89FC
extern CThingManager* g_pThingManager;              // 0x013B8A1C

class CThing { public:
    unsigned char   m_pad00[0x44];
    SComponentPair* m_componentsFirst;               // +0x44  Components (CVectorMap)
    SComponentPair* m_componentsLast;                // +0x48
    SComponentPair* m_componentsEnd;                 // +0x4c
    unsigned char   m_pad50[0x43];
    unsigned char   NotPausedDuringCutscenes : 1;    // +0x93 bit 0
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;            // bit 4
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;          // bit 6
    void OnKill();
};

void CThing::OnKill()
{
    g_pQuestManager->OnDeactivateEntity(this, LevelIsUnloading);
    AddingTCsPermitted = false;
    for_each(m_componentsFirst, m_componentsLast, CCallOnKill());
    AddingTCsPermitted = true;
    g_pThingManager->OnThingKill(this);
}