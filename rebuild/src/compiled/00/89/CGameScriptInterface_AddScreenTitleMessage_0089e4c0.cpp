struct AstmTarget;
struct AstmNode { int key; AstmTarget* value; };
struct AstmMap {
    AstmNode* lb(int* pKey);
    AstmNode* first;
    AstmNode* end;
    __forceinline AstmTarget* Find(int kind)   // the inlined map find: end when the key is absent
    {
        int key = kind;
        AstmNode* node = lb(&key);
        return ((node == end || kind < node->key) ? end : node)->value;
    }
};
__forceinline AstmTarget* GetTCIfPresent(unsigned char* thing, int kind)
{
    AstmTarget* tc;                                // deliberately unset when the component is absent
    if (*(unsigned int*)(thing + 0x30) & 0x4000)
        tc = ((AstmMap*)(thing + 0x44))->Find(kind);
    return tc;
}
struct AstmTarget { void d(void* a0, float a1, bool a2, int z, int one); };
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx,
    unsigned long,
    void* areaName,
    float fadeTime,
    bool messageFlag)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    CPlayer* a = (&((CPlayerManager*)chain)->GetMainPlayer());
    unsigned char* obj = (unsigned char*)a->GetPControlledCreature();
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target = GetTCIfPresent(obj, 0x8e);
    if (target == 0) return;
    target->d(areaName, fadeTime, messageFlag, 0, 1);
}
