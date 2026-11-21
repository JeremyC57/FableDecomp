// Behaviour test for 0x007805D0 NFire::CFirePoint::SetAsSmoldering() (links the REAL row object with what it carries: the
// CDefClassBase::GetSubDef<CSpecialEffectsDef> and CSpecialEffectsDef::GetEffectFromString bodies and the CIntelligentPointer<CThing>
// temporary). The classes come from the shared headers only so that the out-of-line callees defined here get their retail names;
// every object is raw native-layout storage and every offset used is one the row reads or writes:
//   CFirePoint  HelperName +0x00, HelperIndex +0x04, EffectIndex +0x08, PParentThing +0x18, PSingeEffect +0x24 (vptr, PData)
//   CThing      interface bitset word +0x28 (bit 11 = TCI_PARTICLE_EMITTER 75), PTCDriver +0x5C, PBaseDef +0x70, dead bit +0x91 & 1
//   def         vtable slot 14 GetPSubDefInfo (const), PDefManager +0x1C; effects def: vtable slot 1 Delete, count +4, map +0x28
// Every callee logs a letter; the test compares the whole call sequence, every argument, the def's reference count and Delete, and
// PSingeEffect's final item with what the scene dictates (unless a live singe effect is attached: look up "SINGE"; if the effects
// sub-def has it, create the point's own EffectIndex effect (not the looked-up index) at the parent's position, and when it lives,
// attach its particle emitter (NULL when the thing has none) to the parent's helper and keep it in PSingeEffect).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CThing.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CIntelligentPointer.h"
#include <stdio.h>
#include <string.h>
#pragma comment(linker, "/alternatename:@CallRow@4=?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ")
extern "C" void __fastcall CallRow(void* self);           // thiscall, no stack arguments: the same register contract

extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall OnFault(void*) { ExitProcess(3); return 0; }

template<class T> static T& At(void* p, int off) { return *(T*)((char*)p + off); }
static unsigned g_seed;
static unsigned Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }

static char g_log[512]; static int g_logn;
static void Log(char c) { if (g_logn < 500) g_log[g_logn++] = c; g_log[g_logn] = 0; }

// ---- the scene ----
static unsigned char g_fp[0x2c], g_parent[0x100], g_old[0x100], g_fx[0x100], g_def[0x40], g_effects[0x40], g_mgr[8], g_emitter[0x40];
static C3DVector g_pos;
static struct { long DefIndex, OriginalParentDefIndex; } g_subdef;
static struct RbNode { long color; RbNode* parent; RbNode* left; RbNode* right; unsigned long key; long value; } g_hdr, g_singe;
static bool g_has_sub, g_has_singe; static long g_effect_index, g_helper_index, g_singe_index, g_deletes;
static long g_created_index; static const C3DVector* g_created_pos; static bool g_created_flag;
static struct { void* self; void* thing; long type; const void* name; long index; float offset; } g_attach;

// ---- strings: a CCharString is one pointer to a data block whose CBasicString<char> starts with the text pointer and the length ----
struct StrData { const char* PData; unsigned long StringLength; unsigned long DataLength; long NoRefs; };
static StrData g_strpool[16]; static int g_strn;
static const char* Text(const CCharString& s) { return (*(StrData**)&s)->PData; }
CCharString::CCharString(const char* text, long length)
{
    StrData* d = &g_strpool[g_strn++ & 15]; d->PData = text; d->StringLength = (unsigned long)strlen(text); *(StrData**)this = d;
    Log(length != -1 ? 'X' : strcmp(text, "SINGE") == 0 ? 'S' : strcmp(text, "CSpecialEffectsDef") == 0 ? 'T' : 'X');
}
CCharString::~CCharString() { Log('d'); }
const char CCharString::NullChar = 0;
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };
unsigned long __fastcall CCRC::Calc(unsigned long seed, const void* data, unsigned long n)
{
    Log(seed == 0 && n == 5 && memcmp(data, "SINGE", 5) == 0 ? 'r' : 'X'); return 0x5196EUL;
}

// ---- smart pointers: the point's PSingeEffect (+0x24) or the row's temporary ----
static char Where(const void* p) { return (const unsigned char*)p == g_fp + 0x24 ? 's' : 't'; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { Log('('); Log(Where(this)); At<void*>(this, 4) = (void*)0xDDDDDDDD; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { Log(')'); Log(Where(this)); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item)
{
    Log('='); Log(Where(this)); Log(item == 0 ? '0' : (void*)item == (void*)g_fx ? 'F' : '?'); At<void*>(this, 4) = item;
}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { Log('?'); Log(Where(this)); return At<CBaseObject*>((void*)this, 4); }

// ---- the out-of-line callees, under their retail names ----
static void* __fastcall Def_GetPSubDefInfo(void* self, int, const CCharString& type_name)
{
    Log(self == (void*)g_def && strcmp(Text(type_name), "CSpecialEffectsDef") == 0 ? 'u' : 'X'); return g_has_sub ? (void*)&g_subdef : (void*)0;
}
static void __fastcall Effects_Delete(void* self, int) { Log(self == (void*)g_effects ? 'Z' : 'X'); ++g_deletes; }
static void* g_def_vtbl[32]; static void* g_effects_vtbl[32];
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index)
{
    Log((void*)this == (void*)g_mgr && index == g_subdef.DefIndex ? 'x' : 'X');
    ++At<long>(g_effects, 4);                                              // the returned handle's reference
    CDefPointer<CDefClassBase> handle; *(void**)&handle = g_effects;
    return handle;
}
namespace std {
typedef _Rb_tree<unsigned long, pair<const unsigned long, long>, _Select1st<pair<const unsigned long, long> >, less<unsigned long>, allocator<pair<const unsigned long, long> > > EffectTree;
template<> EffectTree::const_iterator EffectTree::find(const unsigned long& key) const
{
    Log((const unsigned char*)this == g_effects + 0x28 && key == 0x5196EUL ? 'f' : 'X');
    At<void*>((void*)this, 0) = &g_hdr;                                    // the tree's header node: end()
    return const_iterator((_Rb_tree_node<pair<const unsigned long, long> >*)(void*)(g_has_singe ? &g_singe : &g_hdr));
}
}
const C3DVector& CThing::GetPos() const { Log((void*)this == (void*)g_parent ? 'P' : 'X'); return g_pos; }
CThing* __fastcall CTCDParticleEmitter::Create(long effect, const C3DVector& pos, bool flag)
{
    Log('C'); g_created_index = effect; g_created_pos = &pos; g_created_flag = flag;
    return At<long>(g_fx, 0xF0) == 0 ? 0 : (CThing*)(void*)g_fx;
}
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& name, long index, float offset)
{
    Log('A'); g_attach.self = this; g_attach.thing = &thing; g_attach.type = type; g_attach.name = &name; g_attach.index = index; g_attach.offset = offset;
}

// ---- one scene: old 0 none / 1 dead / 2 alive; fx 0 none / 1 dead / 2 alive; emitter bit; sub-def; "SINGE" entry; count ----
static int Scene(int old, bool has_sub, bool has_singe, int fx, bool emitter, long count, int line)
{
    memset(g_fp, 0xCC, sizeof g_fp); memset(g_parent, 0, sizeof g_parent); memset(g_old, 0, sizeof g_old); memset(g_fx, 0, sizeof g_fx);
    memset(g_def, 0, sizeof g_def); memset(g_effects, 0, sizeof g_effects); memset(&g_attach, 0, sizeof g_attach);
    g_logn = 0; g_log[0] = 0; g_deletes = 0; g_created_index = -7; g_created_pos = 0; g_created_flag = true;
    g_effect_index = (long)(Rnd() % 900) + 1; g_helper_index = (long)(Rnd() % 9); g_singe_index = (long)(Rnd() % 900) + 1000;
    g_has_sub = has_sub; g_has_singe = has_singe; g_subdef.DefIndex = (long)(Rnd() % 50000);
    g_singe.key = 0x5196EUL; g_singe.value = g_singe_index;
    At<long>(g_fp, 0x04) = g_helper_index; At<long>(g_fp, 0x08) = g_effect_index; At<void*>(g_fp, 0x18) = g_parent;
    At<void*>(g_fp, 0x24) = (void*)0x01238C6C; At<void*>(g_fp, 0x28) = old ? (void*)g_old : (void*)0;
    g_old[0x91] = old == 1 ? 1 : 0; g_old[0x91] |= 0xF6 & (unsigned char)Rnd();       // only bit 0 is the dead flag
    At<void*>(g_parent, 0x70) = g_def;
    At<void*>(g_def, 0) = g_def_vtbl; At<void*>(g_def, 0x1c) = g_mgr;
    At<void*>(g_effects, 0) = g_effects_vtbl; At<long>(g_effects, 4) = count;
    At<long>(g_fx, 0xF0) = fx != 0;                                                     // Create's answer
    g_fx[0x91] = (fx == 1 ? 1 : 0) | (0xF6 & (unsigned char)Rnd());
    At<unsigned long>(g_fx, 0x28) = (emitter ? 0x800UL : 0) | (Rnd() & ~0x800UL); At<void*>(g_fx, 0x5c) = g_emitter;
    CallRow(g_fp);
    // the expected call sequence
    char want[128]; int n = 0;
    want[n++] = '?'; want[n++] = 's';
    bool alive = false;
    if (old) { want[n++] = '?'; want[n++] = 's'; alive = old == 2; }
    bool attached = false;
    if (!alive) {
        want[n++] = 'T'; want[n++] = 'u'; want[n++] = 'd';
        if (has_sub) want[n++] = 'x';
        if (has_sub) { want[n++] = 'S'; want[n++] = 'r'; want[n++] = 'f'; want[n++] = 'd'; }
        if (has_sub && has_singe) {
            want[n++] = 'P'; want[n++] = 'C';
            if (fx == 2) {
                want[n++] = 'A'; attached = true;
                const char* s = "(t=tF?t=sF)t"; while (*s) want[n++] = *s++;
            }
        }
        if (has_sub && count == 0) want[n++] = 'Z';
    }
    want[n] = 0;
    int bad = strcmp(g_log, want) != 0;
    bad |= At<long>(g_effects, 4) != count;                          // handle +1, copy +1, handle released, copy released
    bad |= g_deletes != (has_sub && !alive && count == 0 ? 1 : 0);
    if (has_sub && has_singe && !alive) bad |= g_created_index != g_effect_index || g_created_pos != &g_pos || g_created_flag;
    else bad |= g_created_index != -7;
    if (attached) bad |= g_attach.self != (emitter ? (void*)g_emitter : (void*)0) || g_attach.thing != g_parent || g_attach.type != 0xc
                      || g_attach.name != (void*)g_fp || g_attach.index != g_helper_index || g_attach.offset != 0.0f;
    else bad |= g_attach.self != 0 || g_attach.thing != 0;
    void* item_want = attached ? (void*)g_fx : old ? (void*)g_old : (void*)0;
    bad |= At<void*>(g_fp, 0x28) != item_want || At<void*>(g_fp, 0x24) != (void*)0x01238C6C;
    bad |= At<long>(g_fp, 0x04) != g_helper_index || At<long>(g_fp, 0x08) != g_effect_index;
    if (bad) printf("scene failed at line %d (old %d sub %d singe %d fx %d emitter %d count %ld): log %s want %s\n",
                    line, old, has_sub, has_singe, fx, emitter, count, g_log, want);
    return bad;
}

int main()
{
    SetUnhandledExceptionFilter((void*)OnFault);
    g_def_vtbl[14] = (void*)Def_GetPSubDefInfo; g_effects_vtbl[1] = (void*)Effects_Delete;
    int fails = 0;
    for (g_seed = 1; g_seed < 4000; ) {
        int k = (int)(Rnd() % 144);
        int old = k % 3, fx = (k / 3) % 3; bool sub = ((k / 9) & 1) != 0, singe = ((k / 18) & 1) != 0, em = ((k / 36) & 1) != 0;
        long count = (k / 72) ? (long)(Rnd() % 5) + 1 : 0;
        fails += Scene(old, sub, singe, fx, em, count, __LINE__);
        if (fails > 5) break;
    }
    for (int k = 0; k < 144; ++k) {                                   // and every combination once
        int old = k % 3, fx = (k / 3) % 3; bool sub = ((k / 9) & 1) != 0, singe = ((k / 18) & 1) != 0, em = ((k / 36) & 1) != 0;
        fails += Scene(old, sub, singe, fx, em, (k / 72) ? 3 : 0, __LINE__);
    }
    if (!fails) printf("007805d0_TEST PASS\n");
    return fails != 0;
}