// CGameScriptInterface::GiveHeroItem  (retail 0x00898bb0, 129 bytes)
// VC7.1 / MSVC 2003, x86-32, /O2 /Oy /W3.
// Member method modelled as a free __fastcall(thisptr in ecx, unused edx, ...args).
//
// Byte-exact reconstruction of the retail disassembly.

// ---- inline struct decls referenced by the disasm --------------------------

// arg1 is an "item query" object with a vtable.  We only touch two slots:
//   vtbl[0x12c/4] -> bool  ShouldGive(this)          (call [eax+0x12c])
//   vtbl[0x2c /4] -> void* GetItemDef(this)          (call [edx+0x2c])
struct HeroItemQueryVtbl;
struct HeroItemQuery {
    HeroItemQueryVtbl* vtbl;   // [edi+0]
};
struct HeroItemQueryVtbl {
    // padded so that the two slots we call land at the exact byte offsets.
    unsigned char pad_00[0x2c];
    void* (__fastcall* GetItemDef)(HeroItemQuery*);      // +0x2c
    unsigned char pad_30[0x12c - 0x2c - 4];
    unsigned char (__fastcall* ShouldGive)(HeroItemQuery*); // +0x12c
};

// The object returned by the two engine helpers (the hero/container thing).
//   +0x38  dword flags     (bit 0x0800 selects the map-lookup path)
//   +0x44  std::map header  (a red-black tree keyed by int)
//   +0x91  byte flags       (bit 0x1 == "invalid", bail if set)
struct HeroThing {
    unsigned char pad_00[0x38];
    unsigned long flags38;       // +0x38
    unsigned char pad_3c[0x44 - 0x38 - 4];
    void*         mapRoot;       // +0x44  (map header base)  -> used as &[0x44]
    unsigned char pad_48[0x91 - 0x44 - 4];
    unsigned char flags91;       // +0x91
};
class CThingPlayerCreature : public HeroThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

// A red-black tree node as MSVC lays it out for the lower_bound epilogue.
//   node[0x0] == key (int),  node[0x4] == mapped value (pointer)
struct MapNode {
    long  key;       // +0x0
    void* value;     // +0x4
};

// std::map header object living at HeroThing+0x44. lower_bound is a __fastcall
// member: ecx = this(&header), key pushed on the stack.  (NOT __fastcall: retail
// does `push ecx ; mov ecx,esi ; mov [esp+0x10],0xcb ; call` rather than edx.)
struct HeroMap {
    void*    pad_00;                               // +0x0
    void*    endNode;                              // +0x4 accessed as [this+4]
    MapNode* lower_bound(const long* key);         // __fastcall
    __forceinline void* Find(long kind)            // the inlined map find: end when the key is absent
    {
        long key = kind;
        MapNode* found = lower_bound(&key);
        if (found == (MapNode*)endNode || found->key > kind)
            found = (MapNode*)endNode;
        return found->value;
    }
};
__forceinline void* GetTCIfPresent(HeroThing* thing, long kind)
{
    void* tc;                                      // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->flags38 & 0x0800)
        tc = ((HeroMap*)((unsigned char*)thing + 0x44))->Find(kind);
    return tc;
}

// The "give" target object. The final sink is a __fastcall member:
//   push 0 ; push eax ; mov ecx,target ; call   (args on stack, ecx=this),
// NOT a __fastcall with itemDef in edx.
struct GiveTarget {
    void Give(void* itemDef, int zero);            // __fastcall
};

// ---- engine helpers / globals (relocation-masked: extern only) --------------

extern CPlayer* __fastcall GiveHeroItem_HelperA(void* thisA);            // 0x00449970

// ---- the function ----------------------------------------------------------

void __fastcall CGameScriptInterface_GiveHeroItem(void* thisPtr, unsigned long /*edx*/,
                                                  HeroItemQuery* query)
{
    // push esi ; push edi ; mov edi,[esp+0xc] ; mov eax,[edi] ; mov esi,ecx ; mov ecx,edi
    // call [eax+0x12c] ; test al,al ; je ret
    if (!query->vtbl->ShouldGive(query))
        return;

    // mov ecx,[esi+0x14] ; call HelperA ; mov ecx,eax ; call HelperB
    CPlayer* a = GiveHeroItem_HelperA(*(void**)((unsigned char*)thisPtr + 0x14));
    HeroThing* h = a->GetPControlledCreature();

    // test eax,eax ; je ret
    if (h == 0)
        return;

    // test byte [eax+0x91],1 ; jne ret
    if (h->flags91 & 0x1)
        return;

    void* target = GetTCIfPresent(h, 0xcb);   // left unset when the component is absent

    // mov edx,[edi] ; mov ecx,edi ; call [edx+0x2c]
    void* itemDef = query->vtbl->GetItemDef(query);

    // push 0 ; push eax ; mov ecx,esi ; call sink ; ret 4
    ((GiveTarget*)target)->Give(itemDef, 0);
}