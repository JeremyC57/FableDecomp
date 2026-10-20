// Behaviour test for 007f8f30: src/007f8f30.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCAnimatingObject::PlayDefaultAnimation()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCAnimatingObject::PlayDefaultAnimation() -- drop the queued animations and play the appearance's first "DEFAULT" animation (type 6, mode 1, looping, percent 100) with no priority
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8F30, length 222 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (on the thing's base def, +0x70; result ignored),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CAnimationSet::GetAnimation(const CCharString&,
// long, const CAnimFilterBase&) @ 0x005DC340, free @ 0x00BFEA14 (the inlined STLport list clear), CAnimInit::Set @ 0x0070C050,
// CTCAnimatingObject::PlayAnimation(const CAnimInit&, long) @ 0x007F8DC0, the def's virtual Destroy (slot 1).
// Identity: the one-word filter temporary is CAnimFilterNoGroup (vtable 0x0122FFDC); its inline destructor stores the
// CAnimFilterBase vtable 0x0122FFCC.
// Layout: Ego_r.pdb CTCAnimatingObject QueuedAnims +0x14 (STLport list head node); CAppearanceDef Animation +0x34.
// Shape: the def is read from the out pointer as the object expression of the GetAnimation call (after the name string
// is built) and kept in ebx for the release at the end.
extern "C" void __cdecl free(void* p);                         // 0x00BFEA14

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
struct CAnimationSet {
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};

struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
struct CAnimInit {                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct CRetailList {         // STLport list: the head node pointer
    _List_node<T>* _M_node;
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef {
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
};

class CTCAnimatingObject {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Animating;                          // +0x0c
    long    CurrentAnimationPriority;           // +0x10
    CRetailList<CAnimInit> QueuedAnims;         // +0x14
    void PlayAnimation(const CAnimInit& init, long priority);  // 0x007F8DC0
    void PlayDefaultAnimation();
};

void CTCAnimatingObject::PlayDefaultAnimation()
{
    CDefPointer<CAppearanceDef> out;
    ParentThing->PBaseDef->GetSubDef(&out);
    CAppearanceDef* def;
    const CAnimationEntry* entry;
    {
        CCharString name("DEFAULT");
        def = out.Object;
        entry = def->Animation.GetAnimation(name, 0, CAnimFilterNoGroup());
    }
    QueuedAnims.clear();
    CAnimInit init(6, 0, entry->AnimIndex, 1, true, 0, 100);
    PlayAnimation(init, -1);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}

// ---- stubs and driver ----
static void* g_def = 0; static int g_destroyed = 0; static int g_strCtor = 0, g_strDtor = 0; static char g_nameSeen[16];
static long g_lenSeen = 0; static long g_indexSeen = -1; static int g_filterNoGroup = 0; static CAnimationEntry g_entry;
static long g_initType = -1, g_initArg24 = -1, g_initIndex = -1, g_initMode = -1, g_initPct = -1; static bool g_initFlag = false; static unsigned long g_initFlags = 99;
#include <cstdlib>
static int g_play = 0; static long g_prio = 0; static long g_playIndex = 0; static int g_queuedAtPlay = -1;
void* FindDef_CThingBaseDef_GetSubDef_0() { return g_def; }
CCharString::CCharString(const char* text, long length) { ++g_strCtor; strcpy(g_nameSeen, text); g_lenSeen = length; PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtor; }
bool CAnimFilterBase::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterBase::Clone() const { return 0; }
bool CAnimFilterNoGroup::Accept(const CAnimationEntry*) const { return true; }
CAnimFilterBase* CAnimFilterNoGroup::Clone() const { return 0; }
const CAnimationEntry* CAnimationSet::GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const
{ g_indexSeen = index; g_filterNoGroup = filter.Accept(0) ? 1 : 0; return &g_entry; }
void CAnimInit::Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
{ g_initType = animType; g_initArg24 = arg24; g_initIndex = animIndex; g_initMode = mode; g_initFlag = flag05; g_initFlags = flags; g_initPct = percent; AnimIndex = animIndex; }
void CDefPointeeBase::Destroy0() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void CTCAnimatingObject::v00() {}
static int listCount(_List_node<CAnimInit>* h) { int n = 0; for (_List_node<CAnimInit>* p = h->_M_next; p != h; p = p->_M_next) ++n; return n; }
void CTCAnimatingObject::PlayAnimation(const CAnimInit& init, long priority) { ++g_play; g_prio = priority; g_playIndex = init.AnimIndex; g_queuedAtPlay = listCount(QueuedAnims._M_node); }
int main()
{
    CAppearanceDef def; def.RefCount = 2; g_def = &def; g_entry.AnimIndex = 0x31;
    CThingBaseDef base;
    CThing thing; memset(&thing, 0, sizeof(thing)); thing.PBaseDef = &base;
    _List_node<CAnimInit>& head = *(_List_node<CAnimInit>*)malloc(sizeof(_List_node<CAnimInit>)); head._M_next = head._M_prev = &head;
    for (int i = 0; i < 2; ++i) {   // two queued animations allocated with malloc (freed by clear)
        _List_node<CAnimInit>* n = (_List_node<CAnimInit>*)malloc(sizeof(_List_node<CAnimInit>));
        n->_M_next = &head; n->_M_prev = head._M_prev; head._M_prev->_M_next = n; head._M_prev = n;
    }
    CTCAnimatingObject o; o.ParentThing = &thing; o.QueuedAnims._M_node = &head;
    o.PlayDefaultAnimation();
    CHECK(g_strCtor == 1 && g_strDtor == 1); CHECK(strcmp(g_nameSeen, "DEFAULT") == 0); CHECK(g_lenSeen == -1); CHECK(g_indexSeen == 0); CHECK(g_filterNoGroup == 1);
    CHECK(g_play == 1); CHECK(g_prio == -1); CHECK(g_playIndex == 0x31); CHECK(g_queuedAtPlay == 0);
    CHECK(g_initType == 6 && g_initArg24 == 0 && g_initIndex == 0x31 && g_initMode == 1 && g_initFlag && g_initFlags == 0 && g_initPct == 100);
    CHECK(def.RefCount == 1); CHECK(g_destroyed == 0); CHECK(head._M_next == &head && head._M_prev == &head);
    o.PlayDefaultAnimation(); CHECK(def.RefCount == 0); CHECK(g_destroyed == 1); CHECK(g_play == 2);
    if (fails == 0) printf("007f8f30_TEST PASS\n"); else printf("007f8f30_TEST FAIL\n");
    return fails;
}