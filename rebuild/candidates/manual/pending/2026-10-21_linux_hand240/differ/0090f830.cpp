// CAIStateGroup_MinionListenToStory::SitDown() -- pick a random sit animation through an animation filter, build a sit mode named after it (wrapped in a counted pointer) and perform the sit-down action with it; true when the action finished
// Grade: DIFFER(ours v retail) -- first draft: the new-expression's conditional CCharString temporary (bl flag), the CModeSit construction and the counted pointer plumbing are only approximated (372 vs 390 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F830, length 400 bytes.
// Callees: CThingCreatureBase::GetRandomAnimFromNameWithFilter @ 0x00662EF0 (the global sit name @ 0x013BB66C, a filter
//          object with vtable 0x0127261C / base 0x0122FFCC), operator new @ 0x00BFEA1A, CDefStringTable::GetString
//          @ 0x009D49B0 (the table @ 0x013CA828), CModeSitBase ctor @ 0x008D7CA0 (two vtables 0x01267BBC / 0x01267568),
//          CCharString default ctor @ 0x0099E4B0 / operator= @ 0x0099EFB0 / dtor @ 0x0099EAE0, the name hash @ 0x004014A0
//          (__fastcall), the counted pointer's deleter @ 0x008AA810, CAIStateGroupBase::IsStopping @ 0x008FD010,
//          CCreatureAction_SitDown ctor @ 0x008AB9D0 / dtor @ 0x008ABAA0, AIAction_PerformAction @ 0x0097D8B0,
//          operator delete @ 0x00BFE9BC.
// Shape: first draft (parked): a new-expression whose argument is a conditionally built CCharString temporary (bl flag),
//          the counted pointer built from the new-expression, passed by value.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04).
struct CCharStringData { const char* Chars; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
unsigned long __fastcall CalcHash(const char* text, long seed, long length);   // 0x004014A0
struct CAnimationEntry {
    unsigned char _pad[8];
    long NameIndex;                             // +0x08 (descriptive)
};
struct CDefStringTable {
    CCharString GetString(long index);          // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
extern CCharString g_SitAnimName;               // 0x013BB66C
struct CAnimFilterBase {
    virtual void v00();                         // vtable 0x0122FFCC
    ~CAnimFilterBase() { }
};
struct CAnimFilterSit : CAnimFilterBase {       // vtable 0x0127261C (descriptive)
    long m_value;
    CAnimFilterSit() : m_value(0) { }
    virtual void v00();
};
struct CThing {
    CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662EF0
};
struct CModeSitBase {
    virtual void v00();
    unsigned char _pad[0x28];
    CModeSitBase(CThing* creature);             // 0x008D7CA0
};
struct CModeSitSecond { virtual void w00(); };
struct CModeSitListen : CModeSitBase, CModeSitSecond {   // 0x38 bytes
    CCharString AnimName;                       // +0x30
    unsigned long NameHash;                     // +0x34
    __forceinline CModeSitListen(CThing* creature, const CCharString& anim) : CModeSitBase(creature)
    {
        AnimName = anim;
        const char* text;
        long length;
        if (anim.PStringData == 0) { length = 0; text = ""; }
        else { length = anim.PStringData->Length; text = anim.PStringData->Chars; }
        NameHash = CalcHash(text, 0, length);
    }
    virtual void v00();
    virtual void w00();
    static void* Allocate(unsigned int size);   // 0x00BFEA1A (operator new)
    static void Free(void* p);                  // 0x00BFE9BC (operator delete)
};
struct CCountBlock { long Count; void (__fastcall* Deleter)(void*); void* Object; };
void __fastcall DeleteSitMode(void* object);   // 0x008AA810
struct CCountedPointer {
    CModeSitBase* Object;
    CCountBlock* Block;
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    unsigned char _own[0x10];
    CCreatureAction_SitDown(CThing* creature, CAnimationEntry* anim, CCountedPointer mode);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionListenToStory : CAIStateGroupBase {
    bool SitDown();
};

bool CAIStateGroup_MinionListenToStory::SitDown()
{
    CAnimationEntry* anim = GetCreature()->GetRandomAnimFromNameWithFilter(g_SitAnimName, CAnimFilterSit());
    CModeSitListen* mode = new CModeSitListen(GetCreature(), g_DefStringTable.GetString(anim->NameIndex));
    CCountedPointer pointer;
    pointer.Object = mode;
    pointer.Block = 0;
    if (mode) {
        CCountBlock* block = (CCountBlock*)CModeSitListen::Allocate(sizeof(CCountBlock));
        if (block) {
            block->Object = mode;
            block->Count = 1;
            block->Deleter = DeleteSitMode;
        }
        pointer.Block = block;
    }
    bool ok = false;
    if (!IsStopping()) {
        if (pointer.Block) ++pointer.Block->Count;
        ok = AIAction_PerformAction(&CCreatureAction_SitDown(GetCreature(), anim, pointer)) == 2;
    }
    if (pointer.Block && --pointer.Block->Count == 0) {
        pointer.Block->Deleter(pointer.Block->Object);
        CModeSitListen::Free(pointer.Block);
    }
    return ok;
}
