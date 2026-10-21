// CDefendingCombatantInfo::UpdateCombatState(CCombatantInfo* info, bool active) -- when active: stamp the current frame and the defender's position, build a local map of each attacker's unique id to its entry (found in the manager's combatant map), rebuild the combat wheel rings from it and the defender's position; always update the wheel
// Grade: DIFFER(390v385) -- best effort: inline STLport map ctor/dtor around the attacker walk; the uint64 key copy and the wheel-centre virtual call differ in scheduling
// Leaf: UpdateCombatState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006ECEA0, length 383 bytes (+ int3 padding), `ret 8` (8-byte aligned frame for the 64-bit key).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`; label "GFGetPreMainMemoryUsed" propagated), CThing::GetPosition @ 0x004C73D0, malloc
// @ 0x00BFEA0E (the local map header), the combatant map find @ 0x006F0250 (uint64 key by reference), the local map insert_unique @ 0x006F2A40,
// CCombatWheel::PopulateCombatRings @ 0x006EC850, the local map erase @ 0x006EFCE0, free @ 0x00BFEA14, CCombatWheel::Update @ 0x006E8560.
// Layout: CCombatantInfo Parent +0, Attackers tree +0x34 (map<uint64, CThingCreatureBase*>; creature at node +0x18, its UniqueID at +0x18),
// Position +0x4c, CombatWheel +0x40, LastUpdateFrame +0x58; CDefendingCombatantInfo manager +0x20 (virtual slot 0x30 = GetCombatantInfo
// (position)); combatant-map node value at +0x18 with a flag byte at +0x38.
// Shape: inline STLport map ctor/dtor around the attacker walk (inline iterator increment); the 64-bit key is copied into the aligned frame slot.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
struct C3DVector { float X, Y, Z; };
long __fastcall GFGetCurrentFrame();                       // 0x0049D870
struct CThing {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                             // +0x18
    const C3DVector& GetPosition();                        // 0x004C73D0
};
struct SAttackerNode { long colour; SAttackerNode* parent; SAttackerNode* left; SAttackerNode* right; unsigned __int64 key; CThing* creature; };
struct CAttackerSet { SAttackerNode* header; unsigned long count; };
struct SCombatantNode { long colour; SCombatantNode* parent; SCombatantNode* left; SCombatantNode* right; unsigned __int64 key; unsigned char value[0x20]; bool active; };
struct CCombatantMap {
    SCombatantNode* header;
    SCombatantNode* find(const unsigned __int64& key);     // 0x006F0250
};
struct SLocalNode { unsigned char colour; SLocalNode* parent; SLocalNode* left; SLocalNode* right; unsigned __int64 key; void* value; };
struct SLocalValue { unsigned __int64 key; void* value; };
struct SPairIB { SLocalNode* node; bool inserted; };
struct CLocalMap {
    SLocalNode* header; unsigned long count; unsigned char comp[4];
    CLocalMap() : header(0)
    {
        header = (SLocalNode*)malloc(sizeof(SLocalNode));
        count = 0;
        header->colour = 0;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
    SPairIB insert_unique(const SLocalValue& value);       // 0x006F2A40
    void erase(SLocalNode* root);                          // 0x006EFCE0
    ~CLocalMap()
    {
        if (count) {
            erase(header->parent);
            header->left = header;
            header->parent = 0;
            header->right = header;
            count = 0;
        }
        if (header)
            free(header);
    }
};
struct CCombatWheel {
    void PopulateCombatRings(void* defender, void* centre, CLocalMap* attackers);   // 0x006EC850
    void Update();                                         // 0x006E8560
};
struct CCombatantInfo {
    CThing* Parent;                                        // +0x00
    unsigned char _pad[0x34 - 4];
    CAttackerSet Attackers;                                // +0x34
    unsigned char _pad2[0x40 - 0x3c];
    CCombatWheel Wheel;                                    // +0x40
    unsigned char _pad3[0x4c - 0x40 - sizeof(CCombatWheel)];
    C3DVector Position;                                    // +0x4c
    long LastUpdateFrame;                                  // +0x58
};
struct CCombatManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void* GetWheelCentre(C3DVector* position, CThing* thing);   // slot 0x30
    unsigned char _pad[0x10 - 4];
    CCombatantMap Combatants;                              // +0x10
};
struct CDefendingCombatantInfo {
    unsigned char _pad[0x20];
    CCombatManager* Manager;                               // +0x20
    void UpdateCombatState(CCombatantInfo* info, bool active);
};

void CDefendingCombatantInfo::UpdateCombatState(CCombatantInfo* info, bool active)
{
    if (active) {
        info->LastUpdateFrame = GFGetCurrentFrame();
        info->Position = info->Parent->GetPosition();
        CLocalMap attackers;
        SAttackerNode* header = info->Attackers.header;
        for (SAttackerNode* it = header->left; it != header; ) {
            CThing* creature = it->creature;
            unsigned __int64 key = creature->UniqueID;
            SCombatantNode* found = Manager->Combatants.find(key);
            if (found->active) {
                SLocalValue value;
                value.value = &found->value[4];
                value.key = key;
                attackers.insert_unique(value);
            }
            if (it->right) {
                it = it->right;
                while (it->left) it = it->left;
            } else {
                SAttackerNode* y = it->parent;
                while (it == y->right) { it = y; y = y->parent; }
                if (it->right != y) it = y;
            }
        }
        C3DVector centre;
        info->Wheel.PopulateCombatRings(this, Manager->GetWheelCentre(&centre, info->Parent), &attackers);
    }
    info->Wheel.Update();
}
