// Behaviour test for 00771660: src/00771660.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCOpinionSource::OnCreate()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCOpinionSource::OnCreate() -- give the source a fresh opinion set (five zero opinion values), turn on every reaction flag from 18 up, keep the thing frame-updating unless its def always updates, and add the thing to the world opinion manager's source list
// Leaf: ?OnCreate@CTCOpinionSource@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771660, 265 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A (the 12-byte COpinionSet), malloc 0x00BFEA0E (the five-float opinion buffer and the
//   16-byte list node), CCountedPointer<COpinionSet>::Reset(T*) 0x00771B60 (landed), CThing::SetForceFrameUpdate
//   0x004C7C60, CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   ~CBaseIntelligentPointer 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01237F1C), CTCBase::GetWorld 0x00686A70.
// Layout: retail CTCOpinionSource (as the landed ctor 0x00771360): ParentThing +0x04, ReactionFlags[79] +0x25,
//   POpinionProduced +0x78; COpinionSet = one 12-byte CArray<float>; CThing PBaseDef +0x70; CThingBaseDef
//   AlwaysFrameUpdate +0x41 bit 1 (PDB +0x45); CWorld POpinionManager +0x74, its source list (STLport list of
//   CIntelligentPointer<CThing>, sentinel pointer) +0x71c (as the landed CleanupSource 0x00771770).
// Shape: the opinion array is the STLport vector(n) shape: a _Vector_base with the end pointer in an alloc-proxy
//   member (its init store lands after the malloc push), start/finish/end in STLport order, then the fill through a
//   cursor copy; the reaction flags are `i >= 18` over an unsigned index; the source-list push_back is an inline insert
//   at the sentinel taking the node pointer (a member push_back leaves a dead `add eax,71Ch` of the list address), with
//   the intelligent-pointer temporary built before GetWorld() as the argument.
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

class CThing;
class CWorld;

template <class T> __forceinline T* FillN(T* first, unsigned long count, const T& value)
{
    T* cur = first;
    for (; count > 0; --count, ++cur)
        *cur = value;
    return cur;
}
template <class T> struct SAllocProxy {             // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    __forceinline SAllocProxy(T* data) : _M_data(data) { }
};
template <class T> struct CArrayBase {              // STLport _Vector_base
    T*              m_pData;
    T*              m_pLast;
    SAllocProxy<T>  m_End;
    __forceinline CArrayBase(unsigned long count) : m_pData(0), m_pLast(0), m_End(0)
    {
        m_pData = (T*)malloc(count * sizeof(T));
        m_pLast = m_pData;
        m_End._M_data = m_pData + count;
    }
};
template <class T> struct CArray : CArrayBase<T> {  // retail CArray: 12 bytes
    __forceinline CArray(unsigned long count) : CArrayBase<T>(count)
    {
        this->m_pLast = FillN(this->m_pData, count, T());
    }
};
struct COpinionSet {                                // retail 12 bytes
    CArray<float> OpinionValues;
    __forceinline COpinionSet() : OpinionValues(5) { }
};
template <class T> struct CCountedPointer {
    T*    Data;                                     // +0x00
    void* Info;                                     // +0x04
    void Reset(T* object);                          // 0x00771B60
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void* GetPItem() const;                         // 0x00A01B50
    void SetPItem(void* item);                      // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing> vtable 0x01237F1C
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

template <class T> struct SListNode {
    SListNode* Next;
    SListNode* Prev;
    T          Data;
};
template <class T> struct CList {                   // STLport list: sentinel pointer only
    SListNode<T>* Node;
};
template <class T> __forceinline void ListInsert(SListNode<T>* position, const T& value)
{
    SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
    new (&node->Data) T(value);
    SListNode<T>* prev = position->Prev;
    node->Next = position;
    node->Prev = prev;
    prev->Next = node;
    position->Prev = node;
}
inline void* operator new(unsigned int, void* where) { return where; }

struct COpinionManager {
    unsigned char _pad00[0x71c];
    CList<CIntelligentPointer<CThing> > OpinionSources;   // +0x71c
};
class CWorld {
public:
    unsigned char    _pad00[0x74];
    COpinionManager* POpinionManager;               // +0x74
};
struct CThingBaseDef {
    unsigned char _pad00[0x41];
    bool AvailableInEditor : 1;                     // +0x41 (PDB +0x45)
    bool AlwaysFrameUpdate : 1;
};
class CThing {
public:
    unsigned char  _pad00[0x70];
    CThingBaseDef* PBaseDef;                        // +0x70
    void SetForceFrameUpdate(bool force);           // 0x004C7C60
};

class CTCOpinionSource {
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x25 - 0x08];
    bool          ReactionFlags[79];                // +0x25
    bool          ReactToSelfPermitted;             // +0x74
    bool          AlwaysAwareOf;                    // +0x75
    bool          UnknownThreatSource;              // +0x76
    bool          ReactNonstopFlag;                 // +0x77
    CCountedPointer<COpinionSet> POpinionProduced;  // +0x78
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void OnCreate();
};

void CTCOpinionSource::OnCreate()
{
    POpinionProduced.Reset(new COpinionSet);
    for (unsigned long i = 0; i < 79; ++i)
        ReactionFlags[i] = i >= 18;
    CThingBaseDef* def = ParentThing->PBaseDef;
    if (def && !def->AlwaysFrameUpdate)
        ParentThing->SetForceFrameUpdate(true);
    ListInsert(GetWorld()->POpinionManager->OpinionSources.Node, CIntelligentPointer<CThing>(ParentThing));
}

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static COpinionSet* g_reset = 0;
template <> void CCountedPointer<COpinionSet>::Reset(COpinionSet* object) { Log("Reset"); g_reset = object; Data = object; }
static int g_live = 0;
void CBaseIntelligentPointer::v00() {}
template <> void CIntelligentPointer<CThing>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; ++g_live; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_live; }
void* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(void* item) { m_pItem = item; }
static int g_forced = 0;
void CThing::SetForceFrameUpdate(bool force) { Log("Force"); if (force) ++g_forced; }
static CWorld g_world;
CWorld* CTCOpinionSource::GetWorld() { Log("GetWorld"); return &g_world; }
void CTCOpinionSource::v00() {}

int main()
{
    static COpinionManager mgr;
    g_world.POpinionManager = &mgr;
    SListNode<CIntelligentPointer<CThing> >* sentinel = (SListNode<CIntelligentPointer<CThing> >*)malloc(sizeof *sentinel);
    sentinel->Next = sentinel->Prev = sentinel;
    mgr.OpinionSources.Node = sentinel;
    static CThingBaseDef def; memset(&def, 0, sizeof def);
    static CThing thing; memset(&thing, 0, sizeof thing); thing.PBaseDef = &def;
    CTCOpinionSource src; memset(&src, 0xcd, sizeof src); src.ParentThing = &thing;
    // 1: def without AlwaysFrameUpdate: opinion set of five zeros, flags from 18 on, forced update, appended to the list
    {
        src.OnCreate();
        CHECK(strcmp(g_log, "1:Reset;2:Force;3:GetWorld;") == 0);
        CHECK(g_reset != 0 && g_reset->OpinionValues.m_pLast - g_reset->OpinionValues.m_pData == 5);
        CHECK(g_reset->OpinionValues.m_End._M_data == g_reset->OpinionValues.m_pLast);
        int zeros = 1; for (int i = 0; i < 5; ++i) if (g_reset->OpinionValues.m_pData[i] != 0.0f) zeros = 0;
        CHECK(zeros);
        int flagsOk = 1; for (int i = 0; i < 79; ++i) if (src.ReactionFlags[i] != (i >= 18)) flagsOk = 0;
        CHECK(flagsOk && *(unsigned char*)&src.ReactToSelfPermitted == 0xcd);
        CHECK(g_forced == 1);
        CHECK(sentinel->Next != sentinel && sentinel->Prev == sentinel->Next && sentinel->Next->Next == sentinel);
        CHECK(sentinel->Next->Data.m_pItem == &thing && g_live == 1);
    }
    // 2: def with AlwaysFrameUpdate: no forced update; second node appended at the back
    {
        g_log[0] = 0; g_step = 0; def.AlwaysFrameUpdate = true;
        SListNode<CIntelligentPointer<CThing> >* first = sentinel->Next;
        src.OnCreate();
        CHECK(strcmp(g_log, "1:Reset;2:GetWorld;") == 0);
        CHECK(g_forced == 1);
        CHECK(sentinel->Next == first && first->Next == sentinel->Prev && sentinel->Prev->Prev == first && sentinel->Prev->Next == sentinel);
        CHECK(g_live == 2);
    }
    // 3: no def: no forced update
    {
        g_log[0] = 0; g_step = 0; thing.PBaseDef = 0;
        src.OnCreate();
        CHECK(strcmp(g_log, "1:Reset;2:GetWorld;") == 0 && g_forced == 1 && g_live == 3);
    }
    if (fails == 0) printf("00771660_TEST PASS\n");
    return fails;
}