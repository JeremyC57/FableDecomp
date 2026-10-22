// CAIStateGroup_WhisperPotionCombat::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name) -- base initialise, clear the last-run frame, then collect every live spear-trap and blade-trap object within 80 units of Whisper into TrapThings
// Leaf: ?Initialise@CAIStateGroup_WhisperPotionCombat
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager {
    static CDefinitionManager* Get();           // 0x0044C6B0
};
struct C2DBoxI {
    long Left, Top, Right, Bottom;
};
struct CThingFilter_HasDefGlobalIndex {
    long Index;
    CThingFilter_HasDefGlobalIndex(long index) : Index(index) { }
};
struct CThingPointerVector {                    // std::vector<CThing*>, 12 bytes
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPointerVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    CThing* operator[](unsigned int i) const { return _M_start[i]; }
};
struct CThingSearchTools {
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingPointerVector& out);   // 0x0096BCB0
};
void* SearchSink_96BCB0(const void* tools, const void* area, long index);
#pragma auto_inline(off)
bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingPointerVector& out)
{
    out._M_finish = (CThing**)SearchSink_96BCB0(this, &area, filter.Index);
    return out._M_finish != out._M_start;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointerThing(CThing* item);
    CIntelligentPointerThing(const CIntelligentPointerThing& other);
    virtual void v00();
};
__forceinline CIntelligentPointerThing::CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
__forceinline CIntelligentPointerThing::CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
struct __false_type_ { __false_type_() { } };
struct CIntelligentPointerVector {              // std::vector<CIntelligentPointer<CThing> >, 12 bytes
    CIntelligentPointerThing* _M_start;
    CIntelligentPointerThing* _M_finish;
    CIntelligentPointerThing* _M_end_of_storage;
    void _M_insert_overflow(CIntelligentPointerThing* pos, const CIntelligentPointerThing& x, const __false_type_& t, unsigned int n, bool atend);   // 0x004AC940
    __forceinline void push_back(const CIntelligentPointerThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CIntelligentPointerThing(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type_(), 1, true);
    }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CThingAICreature;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name);   // 0x0091D820
};
struct CAIStateGroup_WhisperPotionCombat : CAIStateGroup_EnemyCreatureBase {
    CIntelligentPointerVector TrapThings;       // +0x1c (retail 12-byte vector)
    long FrameLastRun;                          // +0x28
    C3DVector Jump_to_pos;                      // +0x2c
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name);
};

void CAIStateGroup_WhisperPotionCombat::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, EScriptAIPriority priority, const CCharString& name)
{
    CAIStateGroup_EnemyCreatureBase::Initialise(brain, creature, id, priority, name);
    FrameLastRun = 0;
    CThingPointerVector traps;
    long spearIndex = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_SPEAR_TRAP_01");
    long bladesIndex = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_BLADES_TRAP_01");
    {
        CThingFilter_HasDefGlobalIndex filter(spearIndex);
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area;
        area.Left = x - 80;
        area.Top = y - 80;
        area.Right = x + 80;
        area.Bottom = y + 80;
        PeekThingSearchTools()->GetAllThingsInRoughArea(area, filter, traps);
    }
    {
        CThingFilter_HasDefGlobalIndex filter(bladesIndex);
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI area;
        area.Left = x - 80;
        area.Top = y - 80;
        area.Right = x + 80;
        area.Bottom = y + 80;
        PeekThingSearchTools()->GetAllThingsInRoughArea(area, filter, traps);
    }
    unsigned int count = traps.size();
    for (unsigned int i = 0; i < count; ++i) {
        CThing* trap = traps[i];
        if (trap && !(trap->m_flags91 & 1))
            TrapThings.push_back(CIntelligentPointerThing(trap));
    }
}
