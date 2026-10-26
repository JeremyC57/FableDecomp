// Behaviour test for 00488b20: src/00488b20.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CPlayer::LocateCharacterStartHolySite
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayer::LocateCharacterStartHolySite(CThing** site) -- picks the holy site the character starts at: the one script-named by the start holy site name when no respawn point is set, else the site nearest the respawn position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00488B20, 494 bytes, `ret 4`.
// Callees: the /Gr (__fastcall) template copy 0x0048D5C0 (hidden result in ecx; copies the list's things whose +0xD4 flag
//   is set onto back_inserter(sites)), CWideString::ConvertToCharString 0x0099B2C0 / operator const wchar_t* 0x0099AF10 on the
//   global 0x013B866C (the start holy site name), CCharString copy 0x0099EC30 / default 0x0099E4B0 / dtor 0x0099EAE0 /
//   Format 0x0099F1F0 ("*** WARNING : failed to find a holy site with ScriptName %S" 0x01238074), SGI __find_if 0x0048CC60
//   (/Gr: first ecx, last edx, the binder by value, the iterator tag), the bound predicate 0x0048BC70
//   (`bool __fastcall(CThing*, CCharString)`: strcmp of the thing's ScriptName +0x74), CThing::GetPosition 0x004C73D0,
//   free 0x00BFEA14 (vector storage).
// Shape: vector<CThing*> sites filled from ThingManager(+0x20)->(+4)->list(+0x8c); without a respawn point (RespawnInfo
//   +0xE8 pos, +0xF4 flag) find_if(bind2nd(ptr_fun(fn), CCharString(name))) through an inline find_if wrapper (the binder is
//   built in the frame then copied); not found -> warning text formatted and the respawn position zeroed; then the site
//   nearest the respawn position (MagSquared, start 1e10f). The owner pointer is a named local declared BEFORE the vector.
// Shape (__find_if): the SGI __find_if instantiation is defined in this TU (noinline, /Gr, real four-way unrolled body +
//   remainder switch, tag by const reference) as retail compiled it in the caller's TU; with the body visible VC7.1 homes
//   the iterator-tag temporary in the live `site` argument slot and pushes its address, as retail. find_if, bind2nd and
//   ptr_fun are the STLport inline layers (the binder's operator() copies the bound name per call).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    void Format(const char* format, ...);          // 0x0099F1F0
};
class CWideString { public:
    void* PStringData;
    CCharString ConvertToCharString() const;       // 0x0099B2C0
    operator const unsigned short*() const;               // 0x0099AF10
};
extern CWideString g_CWideString_013b866c;         // 0x013B866C (the character start holy site name)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
class CThing {
public:
    const C3DVector& GetPosition() const;          // 0x004C73D0
};

template <class T>
struct CRetailVector {                             // STLport vector<T>: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CRetailVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
    T* begin() const { return m_pStart; }
    T* end() const { return m_pFinish; }
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& c) : container(&c) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& c) { return back_insert_iterator<C>(c); }

struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; CThing* _M_data; };
struct SThingListIterator {
    SThingNode* _M_node;
    SThingListIterator(SThingNode* node) : _M_node(node) { }
};
struct CThingList {
    SThingNode* _M_node;
    SThingListIterator begin() const { return SThingListIterator(_M_node->_M_next); }
    SThingListIterator end() const { return SThingListIterator(_M_node); }
};
struct CThingListOwner {
    unsigned char _pad00[0x8c];
    CThingList    HolySites;                       // +0x8c
};
class CThingManager { public:
    unsigned char    _pad00[4];
    CThingListOwner* Owner;                        // +0x04
};
// 0x0048D5C0: copies the list's things (those whose +0xD4 flag is set) to the output iterator (/Gr template)
template <class It, class Out> Out __fastcall CopyHolySites(It first, It last, Out out);

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class A1, class A2, class R>
struct pointer_to_binary_function {
    typedef A1 first_argument_type;
    typedef A2 second_argument_type;
    typedef R  result_type;
    R (__fastcall* m_pfn)(A1, A2);
    explicit pointer_to_binary_function(R (__fastcall* pfn)(A1, A2)) : m_pfn(pfn) { }
    R operator()(A1 x, A2 y) const { return m_pfn(x, y); }
};
template <class A1, class A2, class R>
inline pointer_to_binary_function<A1, A2, R> ptr_fun(R (__fastcall* pfn)(A1, A2)) { return pointer_to_binary_function<A1, A2, R>(pfn); }
template <class Op>
struct binder2nd {
    Op op;
    typename Op::second_argument_type value;
    binder2nd(const Op& o, const typename Op::second_argument_type& v) : op(o), value(v) { }
    typename Op::result_type operator()(const typename Op::first_argument_type& x) const { return op(x, value); }
};
template <class Op, class T>
inline binder2nd<Op> bind2nd(const Op& op, const T& x) { return binder2nd<Op>(op, typename Op::second_argument_type(x)); }

#pragma auto_inline(off)
// 0x0048CC60: the SGI/STLport random-access __find_if instantiation (compiled in this TU, /Gr): four-way unrolled loop
// plus the remainder switch; the iterator tag by const reference.
template <class It, class Pred>
__declspec(noinline) It __fastcall __find_if(It first, It last, Pred pred, const random_access_iterator_tag&)
{
    long tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
template <class T> inline random_access_iterator_tag iterator_category(T* const&) { return random_access_iterator_tag(); }
template <class It, class Pred> inline It find_if(It first, It last, Pred pred) { return __find_if(first, last, pred, iterator_category(first)); }

bool __fastcall IsThingScriptNamed(CThing* thing, CCharString name);   // 0x0048BC70

class CPlayer { public:
    unsigned char  _pad00[0x20];
    CThingManager* ThingManager;                   // +0x20
    unsigned char  _pad24[0xe8 - 0x24];
    C3DVector      RespawnPos;                     // +0xe8 (CThingRespawnInfo)
    bool           RespawnValid;                   // +0xf4
    bool LocateCharacterStartHolySite(CThing** site);
};

bool CPlayer::LocateCharacterStartHolySite(CThing** site)
{
    CThingListOwner* owner = ThingManager->Owner;
    CRetailVector<CThing*> sites;
    CopyHolySites(owner->HolySites.begin(), owner->HolySites.end(), back_inserter(sites));
    if (!RespawnValid) {
        CThing** found = find_if(sites.begin(), sites.end(), bind2nd(ptr_fun(&IsThingScriptNamed), g_CWideString_013b866c.ConvertToCharString()));
        if (found != sites.end()) {
            *site = *found;
            return true;
        }
        CCharString message;
        message.Format("*** WARNING : failed to find a holy site with ScriptName %S", (const unsigned short*)g_CWideString_013b866c);
        RespawnPos = C3DVector(0.0f, 0.0f, 0.0f);
    }
    CThing* best = 0;
    float bestDistance = 1e10f;
    for (CThing** it = sites.begin(); it != sites.end(); ++it) {
        CThing* thing = *it;
        float distance = (thing->GetPosition() - RespawnPos).MagSquared();
        if (distance < bestDistance) {
            bestDistance = distance;
            best = thing;
        }
    }
    if (best) {
        *site = best;
        return true;
    }
    return false;
}

// ---- stubs ----
static int g_live; static int g_fmt; static const char* g_fmtText; static const unsigned short* g_fmtArg;
CCharString::CCharString() { PStringData = 0; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; ++g_live; }
CCharString::~CCharString() { --g_live; }
void CCharString::Format(const char* format, ...)
{
    va_list ap; va_start(ap, format); g_fmtArg = va_arg(ap, const unsigned short*); va_end(ap);
    ++g_fmt; g_fmtText = format;
}
static const char* g_startName = "HS_Start";
static const unsigned short g_wide[] = { 'H', 'S', 0 };
CCharString CWideString::ConvertToCharString() const { CCharString r; r.PStringData = (void*)g_startName; return r; }
CWideString::operator const unsigned short*() const { return g_wide; }
CWideString g_CWideString_013b866c;
static CThing g_things[8]; static const char* g_names[8]; static C3DVector g_pos[8]; static int g_named;
const C3DVector& CThing::GetPosition() const { return g_pos[this - g_things]; }
bool __fastcall IsThingScriptNamed(CThing* thing, CCharString name)
{ ++g_named; return !strcmp(g_names[thing - g_things], (const char*)name.PStringData); }
template <class It, class Out> Out __fastcall CopyHolySites(It first, It last, Out out)
{
    CRetailVector<CThing*>* v = out.container;
    v->m_pStart = v->m_pFinish = (CThing**)malloc(16 * sizeof(CThing*)); v->m_pEndOfStorage = v->m_pStart + 16;
    for (SThingNode* n = first._M_node; n != last._M_node; n = n->_M_next) *v->m_pFinish++ = n->_M_data;
    return out;
}
static SThingNode g_head, g_nodes[8];
static void BuildList(int count)
{
    SThingNode* prev = &g_head;
    for (int i = 0; i < count; ++i) { g_nodes[i]._M_data = &g_things[i]; prev->_M_next = &g_nodes[i]; g_nodes[i]._M_prev = prev; prev = &g_nodes[i]; }
    prev->_M_next = &g_head; g_head._M_prev = prev;
}
static void Reset() { g_fmt = 0; g_fmtText = 0; g_fmtArg = 0; g_named = 0; }
int main()
{
    static unsigned char ownerBuf[sizeof(CThingListOwner)]; CThingListOwner* owner = (CThingListOwner*)ownerBuf;
    owner->HolySites._M_node = &g_head;
    CThingManager tm; tm.Owner = owner;
    static unsigned char playerBuf[sizeof(CPlayer)]; CPlayer* p = (CPlayer*)playerBuf; p->ThingManager = &tm;
    const char* names[7] = { "A", "B", "C", "D", "E", "F", "G" };
    for (int i = 0; i < 7; ++i) { g_names[i] = names[i]; g_pos[i] = C3DVector((float)i * 10.0f, 0.0f, 0.0f); }
    CThing* site;
    // no respawn point, named site inside the unrolled part (index 2 of 7)
    BuildList(7); g_names[2] = "HS_Start"; p->RespawnValid = false; p->RespawnPos = C3DVector(1, 2, 3);
    Reset(); site = 0;
    CHECK(p->LocateCharacterStartHolySite(&site) && site == &g_things[2] && g_named == 3 && g_fmt == 0 && g_live == 0);
    CHECK(p->RespawnPos.X == 1 && p->RespawnPos.Y == 2 && p->RespawnPos.Z == 3);
    // named site in the remainder (index 5 of 7 -> after one unrolled trip, remainder 3)
    g_names[2] = "C"; g_names[5] = "HS_Start";
    Reset(); site = 0;
    CHECK(p->LocateCharacterStartHolySite(&site) && site == &g_things[5] && g_named == 6 && g_live == 0);
    // last element of the remainder (index 6)
    g_names[5] = "F"; g_names[6] = "HS_Start";
    Reset(); site = 0;
    CHECK(p->LocateCharacterStartHolySite(&site) && site == &g_things[6] && g_named == 7 && g_live == 0);
    // not found: warning formatted, respawn zeroed, nearest to origin chosen (site 0 at x=0)
    g_names[6] = "G"; g_pos[0] = C3DVector(50, 0, 0); g_pos[3] = C3DVector(1, 1, 0);
    Reset(); site = 0;
    CHECK(p->LocateCharacterStartHolySite(&site) && site == &g_things[3] && g_named == 7);
    CHECK(g_fmt == 1 && g_fmtArg == g_wide && g_fmtText && !strncmp(g_fmtText, "*** WARNING", 11) && g_live == 0);
    CHECK(p->RespawnPos.X == 0 && p->RespawnPos.Y == 0 && p->RespawnPos.Z == 0);
    // respawn point set: nearest to it, no name search
    p->RespawnValid = true; p->RespawnPos = C3DVector(41, 0, 0);
    Reset(); site = 0;
    CHECK(p->LocateCharacterStartHolySite(&site) && site == &g_things[4] && g_named == 0 && g_fmt == 0);
    // empty list: nothing found
    BuildList(0); site = (CThing*)1;
    Reset();
    CHECK(!p->LocateCharacterStartHolySite(&site) && site == (CThing*)1);
    // empty list without respawn point: warning, false
    p->RespawnValid = false; Reset();
    CHECK(!p->LocateCharacterStartHolySite(&site) && g_fmt == 1 && g_named == 0 && g_live == 0);
    if (fails == 0) printf("00488b20_TEST PASS\n"); else printf("00488b20_TEST FAIL\n");
    return fails;
}