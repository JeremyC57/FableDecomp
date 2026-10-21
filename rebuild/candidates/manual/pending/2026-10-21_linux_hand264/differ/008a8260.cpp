// CGameScriptInterface::GetHeroSummonedCreaturesList(vector<CScriptThing>& creatures) const -- append handles for the hero's live summoned creature (TC 0x91) and every live creature in the hero's summoned-creature list (TC 0x9a); true when any was added
// Grade: DIFFER(481v466) -- the found flag: retail homes it in a frame byte (stored there in the loop, reloaded into bl), ours keeps it in bl; plus edi/ebp pushes sunk into the guarded blocks (lane gap). Two tries.
// Leaf: GetHeroSummonedCreaturesList
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A8260, 464 bytes, `ret 4`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroThing 0x00487DC0, the const component-map LowerBound
//   0x004365B0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CThing::GetScriptThing 0x004C7E50, ~CScriptThing 0x004AA840, vector<CScriptThing>::push_back 0x008ADF90, the
//   intelligent-pointer vector copy constructor 0x008AC540, free 0x00BFEA14.
// Layout: CThing native projection (UsingComponents +0x20, Components +0x44, LifeFlags +0x91); the summoner component keeps
//   its creature pointer at +0x11 (packed) and the list component its vector at +0x10 (descriptive names). The generated
//   CGameScriptInterface header has PlayerManager at +0x14, used through a local projection.
extern "C" void free(void* p);                           // 0x00BFEA14
struct CScriptThing {
    void* VTable; void* Data; void* Info;
    ~CScriptThing();                                     // 0x004AA840
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void push_back(const T& value);                      // 0x008ADF90
};
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CThing {
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                            // +0x91 (bit 0 = dead)
    CScriptThing GetScriptThing();                       // 0x004C7E50
};
struct CBaseIntelligentPointer {
    void* Info;
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual ~CIntelligentPointer() { }
};
template <class T> struct IPVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    IPVector(const IPVector& other);                     // 0x008AC540
    ~IPVector()
    {
        for (T* p = _M_start; p != _M_finish; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
};
#pragma pack(push, 1)
struct CTCSummoner {
    unsigned char                Base[0x11];
    CIntelligentPointer<CThing>  SummonedCreature;       // +0x11
};
#pragma pack(pop)
struct CTCSummonedCreatures {
    unsigned char                         Base[0x10];
    IPVector<CIntelligentPointer<CThing> > Creatures;    // +0x10
};
struct CPlayer { CThing* GetHeroThing(); };              // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };     // 0x00449970
template <class T, long TYPE> __forceinline T* GetComponent(CThing* thing)
{
    const ComponentMap* map = &thing->Components;
    const ComponentEntry* it = map->LowerBound(TYPE);
    const ComponentEntry* end = map->Finish;
    return (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
}
struct CGameScriptInterface {
    virtual void v000();
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    bool GetHeroSummonedCreaturesList(vector<CScriptThing>& creatures) const;
};

bool CGameScriptInterface::GetHeroSummonedCreaturesList(vector<CScriptThing>& creatures) const
{
    bool found = false;
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero && !(hero->LifeFlags & 1)) {
        if (hero->UsingComponents[4] & 0x20000) {
            CIntelligentPointer<CThing> creature = GetComponent<CTCSummoner, 0x91>(hero)->SummonedCreature;
            if (creature.GetPItem() && !(creature.GetPItem()->LifeFlags & 1)) {
                creatures.push_back(creature.GetPItem()->GetScriptThing());
                found = true;
            }
        }
        if (hero->UsingComponents[4] & 0x4000000) {
            IPVector<CIntelligentPointer<CThing> > list = GetComponent<CTCSummonedCreatures, 0x9a>(hero)->Creatures;
            for (long i = 0; i < (long)list.size(); ++i) {
                if (list[i].GetPItem() && !(list[i].GetPItem()->LifeFlags & 1)) {
                    creatures.push_back(list[i].GetPItem()->GetScriptThing());
                    found = true;
                }
            }
        }
    }
    return found;
}
