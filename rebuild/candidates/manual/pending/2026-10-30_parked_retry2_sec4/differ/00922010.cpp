// CAIStateGroup_MinionWalkToMarker::Update() -- while the walk-to-marker component stays in mode 8 or 9, pick the next marker with the component's script name (mode 8: a random one; mode 9: the nearest one that is neither of the last two) and walk to it; terminate when the mode changes or the state group is stopping
// Grade: DIFFER(646v651) nd=217 (was 261) -- levers: same-TU LowerBound stand-in (lbstand) + named CAIBrain* brain for the first interface lookup (retail ebx=brain). Residue: this in ebx vs retail edi; retail homes walk in ebp across the loop (stored and reloaded on the absent path) while ours keeps it in its frame slot; frame 0x50 v 0x4c; mode-8 block placement. Next: GetInterfaceWord written with an explicit else store, walk register-promoted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00922010, length 651 bytes (+ int3 padding), plain `ret`, frame 0x4c.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (0xAA walk-to-marker component), CThingCreatureBase::GetPosition @ 0x006A4D00,
// CCharString copy ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThing
// <HasScriptName, Nearest> @ 0x00922320 / GetAllThings<HasScriptName> @ 0x005ED860 / GetPBestThing<Bind3<IsNot, IsNot,
// HasScriptName>, Nearest> @ 0x009223A0 (manifest names propagated), GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
// free @ 0x00BFEA14, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90, AIAction_MoveToThingProximity
// @ 0x0097E380, IsStopping @ 0x008FD010, Terminate @ 0x008FCF40.
// Layout: as the landed IsRunnable 0x00921630 (component Mode +0x38, MarkerName +0x50); STLport vector 12 bytes.
// Shape: the component lookup is the word-local HasTC (its byte homed); the marker filters copy the name twice (named copy +
// the filter's own copy).
extern "C" void __cdecl free(void* block);
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceWord(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CTCWalkToMarker {                        // interface 0xAA
    unsigned char _pad[0x38];
    long Mode;                                  // +0x38
    unsigned char _pad_0x3c[0x14];
    CCharString MarkerName;                     // +0x50
};
struct CThingFilter_HasScriptName {
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
struct CThingFilter_IsNotThing {
    const CThing* Thing;
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
template<class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* PF1; const F2* PF2; const F3* PF3;
    CFilter_Bind3(const F1& f1, const F2& f2, const F3& f3) : PF1(&f1), PF2(&f2), PF3(&f3) { }
};
typedef CFilter_Bind3<CThingFilter_IsNotThing, CThingFilter_IsNotThing, CThingFilter_HasScriptName> CNextMarkerFilter;
struct CThingVector {                           // STLport vector<CThing*>
    CThing** Start;
    CThing** Finish;
    CThing** EndOfStorage;
    CThingVector() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CThingVector() { if (Start) free(Start); }
    unsigned long size() const { return Finish - Start; }
};
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_HasScriptName& filter, const CThingCompare_Nearest& compare, CThing** best);   // 0x00922320
    void GetAllThings(const CThingFilter_HasScriptName& filter, CThingVector& things);                                  // 0x005ED860
    bool GetPBestThing(const CNextMarkerFilter& filter, const CThingCompare_Nearest& compare, CThing** best);            // 0x009223A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    __forceinline unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
struct CAIStateGroup_MinionWalkToMarker : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_MinionWalkToMarker::Update()
{
    CAIBrain* brain = PBrain;
    CTCWalkToMarker* walk;
    brain->Creature->GetInterfaceWord(&walk, 0xaa);
    CThing* target;
    {
        CThingCompare_Nearest nearest(GetCreature()->GetPosition());
        CCharString name(walk->MarkerName);
        CThingFilter_HasScriptName filter(name);
        PeekThingSearchTools()->GetPBestThing(filter, nearest, &target);
    }
    CThing* previous = 0;
    CThing* beforePrevious = 0;
    while (walk->Mode == 8 || walk->Mode == 9) {
        if (walk->Mode == 8) {
            CThingVector things;
            {
                CCharString name(walk->MarkerName);
                CThingFilter_HasScriptName filter(name);
                PeekThingSearchTools()->GetAllThings(filter, things);
            }
            target = things.Start[Random(things.size())];
        } else {
            CThingCompare_Nearest nearest(GetCreature()->GetPosition());
            CCharString name(walk->MarkerName);
            CThingFilter_HasScriptName filter(name);
            CThingFilter_IsNotThing notPrevious = { previous };
            CThingFilter_IsNotThing notBeforePrevious = { beforePrevious };
            PeekThingSearchTools()->GetPBestThing(CNextMarkerFilter(notPrevious, notBeforePrevious, filter), nearest, &target);
        }
        bool arrived;
        while (walk->Mode == 8 || walk->Mode == 9) {
            if (AIAction_MoveToThingProximity(2, target, 0.0f, &arrived, 3, true, false, true, true, 0.0f) != 4)
                break;
        }
        if (IsStopping())
            break;
        beforePrevious = previous;
        previous = target;
    }
    Terminate();
}
