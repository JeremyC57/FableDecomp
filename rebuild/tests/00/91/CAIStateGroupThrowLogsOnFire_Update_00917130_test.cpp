#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CAIStateGroup_ThrowLogsOnFire::Update() -- collect the knothole logs, walk to and throw each onto the fire
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x00917130.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* block);
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
extern CCharString g_CCharString_013bb674;      // "OBJECT_KNOTHOLE_LOG_MOVEABLE"
extern CCharString g_CCharString_013bb678;      // "ST_WORK_GET_CRATE"
extern CCharString g_CCharString_013bb67c;      // "ST_HOLDING_CRATE"
extern CCharString g_CCharString_013bb680;      // "ST_WORK_PUT_CRATE"

struct __true_type { };
template <class T> struct vector {              // STLport vector<T> (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& other);                // 0x00916CE0
    ~vector() { if (_M_start) free(_M_start); }
    template <class Tag> void _M_insert_overflow(T* position, const T& x, const Tag&, unsigned int fillLength, bool atEnd);  // 0x00916D50
    void push_back(const T& x)                  // 0x00916EF0
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    __forceinline void push_back_inline(const T& x)   // the same STLport body, expanded at the second site
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
// Same-TU body of the STLport vector<const CCharString*>::_M_insert_overflow instantiation (tag unused).
template <class T> template <class Tag> __declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const Tag&, unsigned int n, bool atend)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize < n ? n : oldSize);
    T* ns = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* nf = ns;
    T* p;
    for (p = _M_start; p != pos; ++p) *nf++ = *p;
    for (; n != 0; --n) *nf++ = x;
    if (!atend)
        for (p = pos; p != _M_finish; ++p) *nf++ = *p;
    if (_M_start)
        free(_M_start);
    _M_start = ns;
    _M_finish = nf;
    _M_end_of_storage = ns + len;
}
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};
struct CThingCreature : CThing {
    void* GetAnimFromName(const CCharString& name, long index); // 0x00662A00
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
class CAIStateGroupBase { public:
    class CAIFilterBase { public:                      // vtable 0x0128FD20
        virtual bool operator()(CThing* thing) const;   // 0x00916A50
    };
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CDefinitionManager* GetPDefinitionManager() const;  // 0x008FCFB0
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool FetchGoods(CIntelligentPointerThing source, float distance, vector<const CCharString*> anims,
        const CAIFilterBase& filter, CThing* goods);   // 0x00915890
    bool PlaceGoods(CIntelligentPointerThing target, float distance, vector<const CCharString*> anims,
        const CAIFilterBase& filter, CThing* goods);   // 0x00915B60
};
struct CAIStateGroup_ThrowLogsOnFire : CAIStateGroup_MoveGoodsBase {
    CThing* GetPLogPile();                      // 0x00917050
    CThing* GetPBonfire();                      // 0x009170B0
    virtual void Update();
};

void CAIStateGroup_ThrowLogsOnFire::Update()
{
    CThing* pile = GetPLogPile();
    if (pile == 0) {
        Terminate();
        return;
    }
    CThing* fire = GetPBonfire();
    if (fire == 0) {
        Terminate();
        return;
    }
    CTCCarrying* carrying;
    PBrain->PCreature->GetInterface(&carrying, 0x46);
    if (carrying->GetThingInPrimarySlot() == 0) {
        GetPDefinitionManager()->GetDefGlobalIndexFromName(g_CCharString_013bb674);
        PBrain->PCreature->GetAnimFromName(g_CCharString_013bb67c, 0);
        vector<const CCharString*> getAnims;
        getAnims.push_back(&g_CCharString_013bb678);
        bool failed = !FetchGoods(CIntelligentPointerThing(pile), 3.5f, getAnims, CAIFilterBase(), 0);
        if (failed) {
            Terminate();
            return;
        }
    }
    {
        vector<const CCharString*> putAnims;
        putAnims.push_back_inline(&g_CCharString_013bb680);
        PlaceGoods(CIntelligentPointerThing(fire), 4.0f, putAnims, CAIFilterBase(), 0);
        Terminate();
    }
}

// ---- behaviour-test stubs ----
static int g_terminates, g_fetches, g_places, g_defLookups, g_animLookups;
static bool g_fetchResult;
static CThing* g_pile; static CThing* g_fire; static CThing* g_held;
static CThing* g_fetchSource; static CThing* g_placeTarget;
static float g_fetchDist, g_placeDist;
static const CCharString* g_fetchAnim; static const CCharString* g_placeAnim;
static int g_fetchAnimCount, g_placeAnimCount;
static const CCharString* g_defName; static const CCharString* g_animName;
CCharString g_CCharString_013bb674("OBJECT_KNOTHOLE_LOG_MOVEABLE");
CCharString g_CCharString_013bb678("ST_WORK_GET_CRATE");
CCharString g_CCharString_013bb67c("ST_HOLDING_CRATE");
CCharString g_CCharString_013bb680("ST_WORK_PUT_CRATE");
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; }
CCharString::~CCharString() { }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerThing::v00() { }
template <class T> vector<T>::vector(const vector& other)
{
    unsigned int n = other._M_finish - other._M_start;
    _M_start = n ? (T*)malloc(n * sizeof(T)) : 0;
    for (unsigned int i = 0; i < n; ++i) _M_start[i] = other._M_start[i];
    _M_finish = _M_start + n; _M_end_of_storage = _M_finish;
}
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_held; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { ++g_defLookups; g_defName = &name; return 7; }
void* CThingCreature::GetAnimFromName(const CCharString& name, long) { ++g_animLookups; g_animName = &name; return 0; }
bool CAIStateGroupBase::CAIFilterBase::operator()(CThing*) const { return true; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static CDefinitionManager g_defs;
CDefinitionManager* CAIStateGroupBase::GetPDefinitionManager() const { return &g_defs; }
void CAIStateGroupBase::Terminate() { ++g_terminates; }
bool CAIStateGroup_MoveGoodsBase::FetchGoods(CIntelligentPointerThing source, float distance, vector<const CCharString*> anims, const CAIFilterBase&, CThing*)
{
    ++g_fetches; g_fetchSource = source.GetPItem(); g_fetchDist = distance;
    g_fetchAnimCount = anims._M_finish - anims._M_start; g_fetchAnim = g_fetchAnimCount ? anims._M_start[0] : 0;
    return g_fetchResult;
}
bool CAIStateGroup_MoveGoodsBase::PlaceGoods(CIntelligentPointerThing target, float distance, vector<const CCharString*> anims, const CAIFilterBase&, CThing*)
{
    ++g_places; g_placeTarget = target.GetPItem(); g_placeDist = distance;
    g_placeAnimCount = anims._M_finish - anims._M_start; g_placeAnim = g_placeAnimCount ? anims._M_start[0] : 0;
    return true;
}
CThing* CAIStateGroup_ThrowLogsOnFire::GetPLogPile() { return g_pile; }
CThing* CAIStateGroup_ThrowLogsOnFire::GetPBonfire() { return g_fire; }
static void Reset() { g_terminates = g_fetches = g_places = g_defLookups = g_animLookups = 0; g_fetchSource = g_placeTarget = 0; g_fetchAnim = g_placeAnim = 0; }
int main()
{
    int ok = 1;
    static CThingCreature creature;
    static CTCCarrying carrying;
    static CInterfaceMap::SEntry entry[1];
    entry[0].type = 0x46; entry[0].component = (CTCBase*)&carrying;
    creature.m_presentBits[0x46 >> 5] = 1u << (0x46 & 31);
    creature.m_interfaces.first = entry; creature.m_interfaces.last = entry + 1;
    static CAIBrain brain; brain.PCreature = &creature;
    static CThing pile, fire, log;
    CAIStateGroup_ThrowLogsOnFire group; group.PBrain = &brain;
    // A: no log pile
    Reset(); g_pile = 0; g_fire = &fire; group.Update();
    if (g_terminates != 1 || g_fetches || g_places) { ok = 0; printf("caseA\n"); }
    // B: no bonfire
    Reset(); g_pile = &pile; g_fire = 0; group.Update();
    if (g_terminates != 1 || g_fetches || g_places) { ok = 0; printf("caseB\n"); }
    // C: already holding a log -> straight to the fire
    Reset(); g_fire = &fire; g_held = &log; group.Update();
    if (g_terminates != 1 || g_fetches || g_places != 1 || g_placeTarget != &fire || g_placeDist != 4.0f || g_placeAnimCount != 1 ||
        g_placeAnim != &g_CCharString_013bb680 || g_defLookups) { ok = 0; printf("caseC\n"); }
    // D: empty hands, the fetch fails
    Reset(); g_held = 0; g_fetchResult = false; group.Update();
    if (g_terminates != 1 || g_fetches != 1 || g_places || g_fetchSource != &pile || g_fetchDist != 3.5f || g_fetchAnimCount != 1 ||
        g_fetchAnim != &g_CCharString_013bb678 || g_defLookups != 1 || g_defName != &g_CCharString_013bb674 || g_animLookups != 1 ||
        g_animName != &g_CCharString_013bb67c) { ok = 0; printf("caseD\n"); }
    // E: empty hands, the fetch succeeds -> fetch then place
    Reset(); g_fetchResult = true; group.Update();
    if (g_terminates != 1 || g_fetches != 1 || g_places != 1 || g_placeTarget != &fire || g_placeAnim != &g_CCharString_013bb680) { ok = 0; printf("caseE\n"); }
    if (ok) printf("00917130_TEST PASS\n");
    return ok ? 0 : 1;
}