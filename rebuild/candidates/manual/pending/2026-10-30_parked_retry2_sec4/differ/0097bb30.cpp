// CAIStateGroup_ScorpionKing::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- base initialise, then collect every arena spear trap and blade trap within 40 units of the scorpion king, keep each live one as a trap thing and switch its trap off; reset the charge count
// Grade: DIFFER(616v622) nd=77 (was 103) -- lever: the intelligent-pointer copy ctor / operator= read the item into a named local before SetPItem (as 0x00926690). Tried: traps vector declared after the name blocks (85) or between them (93). Residue: the name CCharString slot (+0x14 v +0x10), the trap count in edi vs retail's [esp+14h] spill with &TrapThings in ebx across the inline push_back. Next: push_back through the shared STLport vector model.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097BB30, length 624 bytes (+ int3 padding; `and esp,-8` frame), `ret 0x14`.
// Callees: CAIStateGroupBase::Initialise @ 0x008FCEC0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
// ("OBJECT_HERO_ARENA_SPEAR_TRAP_01" @ 0x0129672C, "OBJECT_HERO_ARENA_BLADES_TRAP_01" @ 0x01296708), CGameDefinitionManager::Get
// @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, __ftol2
// @ 0x00BFEA70, PeekThingSearchTools @ 0x008FD000, the rough-area collector @ 0x0096BCB0 (three arguments: box, filter, output
// vector; the manifest "GetPBestThingInRoughArea<HasTrackName, Nearest, CThingTrackNode>" is a propagated guess), twice,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), the member vector's _M_insert_overflow @ 0x004AC940, CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 0x7d = bit 29 of +0x2c), CTCTrapBase::ManualDeactivate @ 0x00787BFE, free @ 0x00BFEA14.
// Layout: CAIStateGroup_ScorpionKing from Ego_r.pdb (engine/CAIStateGroup_ScorpionKing.h) with the STLport vector 12 bytes, so
// retail sits 4 below the PDB from bIsRangeAttack on (ChargeCount at +0x2c); push_back / box / vector models as the landed
// Guildmaster OnActivate 0x00977C40.
struct C3DVector {
    float X, Y, Z;
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    unsigned char _pad68[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CThingAICreature : CThing { };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(const C3DVector& centre, long radius)
    {
        long x = (long)centre.X;
        long y = (long)centre.Y;
        MinX = x - radius; MinY = y - radius; MaxX = x + radius; MaxY = y + radius;
    }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) { }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();           // 0x0044C6B0
};
extern "C" void __cdecl free(void* block);    // 0x00BFEA14
struct CThingPtrVector {                        // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingPtrVector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CThing*& operator[](unsigned int n) { return _M_start[n]; }
};
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefGlobalIndex& filter, CThingPtrVector& out) const;   // 0x0096BCB0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { CThing* item_ = other.GetPItem(); SetPItem(item_); }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CTCTrapBase {                            // interface 0x7d
    void ManualDeactivate();                    // 0x00787BFE
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
                            const CCharString& name);   // 0x008FCEC0
};
struct CAIStateGroup_ScorpionKing : CAIStateGroupBase {
    CIntelligentPointer PArenaCentre;           // +0x14
    CRetailVector<CIntelligentPointer> TrapThings;   // +0x1c
    bool bIsRangeAttack;                        // +0x28
    long ChargeCount;                           // +0x2c
    CBaseIntelligentPointer PEnemy;             // +0x30 CIntelligentPointer<CThingCreatureBase>
    long EnemyKnockDownFrame;                   // +0x38
    float HealthAtLastTailStrike;               // +0x3c
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
                            const CCharString& name);
};

void CAIStateGroup_ScorpionKing::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority,
                                            EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    CThingPtrVector traps;
    long spearIndex;
    {
        CCharString spearName("OBJECT_HERO_ARENA_SPEAR_TRAP_01");
        spearIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(spearName);
    }
    long bladesIndex;
    {
        CCharString bladesName("OBJECT_HERO_ARENA_BLADES_TRAP_01");
        bladesIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(bladesName);
    }
    PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(GetCreature()->GetPosition(), 40),
                                                    CThingFilter_HasDefGlobalIndex(spearIndex), traps);
    PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(GetCreature()->GetPosition(), 40),
                                                    CThingFilter_HasDefGlobalIndex(bladesIndex), traps);
    unsigned int count = traps.size();
    for (unsigned int i = 0; i < count; ++i) {
        CThing* trap = traps[i];
        if (trap && !(trap->m_flags91 & 1)) {
            TrapThings.push_back(CIntelligentPointer(trap));
            CTCTrapBase* trapTC;
            if (traps[i]->GetInterface(&trapTC, 0x7d))
                trapTC->ManualDeactivate();
        }
    }
    ChargeCount = 0;
}
