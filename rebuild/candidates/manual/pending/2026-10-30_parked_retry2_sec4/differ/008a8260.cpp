// CGameScriptInterface::GetHeroSummonedCreaturesList(vector<CScriptThing>& creatures) const -- append handles for the hero's live summoned creature (TC 0x91) and every live creature in the hero's summoned-creature list (TC 0x9a); true when any was added
// Grade: DIFFER(473v466) nd=76 (was 481v466 nd=85) -- lever: the list loop's add through an inline helper taking the intelligent pointer by const& and found by bool& (keeps the three GetPItem calls); tried: the helper taking the raw pointer (457v466 nd=83: fewer GetPItem calls than retail), the helper on both blocks (119); remaining: retail homes found in a frame byte in the SUMMONER block (mov byte [esp+0Fh],1 ; mov bl,[esp+0Fh]) and stores found=false to the frame at entry; next: the bool& helper on the summoner block only, plus the list size/index loop shape (retail sar 3 count, ebp/edi first/last)
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

static __forceinline void AddLivingCreature(vector<CScriptThing>& creatures, const CIntelligentPointer<CThing>& creature, bool& found)
{
    if (creature.GetPItem() && !(creature.GetPItem()->LifeFlags & 1)) {
        creatures.push_back(creature.GetPItem()->GetScriptThing());
        found = true;
    }
}
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
                AddLivingCreature(creatures, list[i], found);
            }
        }
    }
    return found;
}
