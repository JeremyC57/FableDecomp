#include <cstdio>
// Name-fix pass (2026-11-12, NF29): the row returns the shared CWideString (fable_string.h) and declares GetManualSaveFileName through
//   engine/CUserProfileManager.h (SAVENAMES); this test defines the CWideString members the link needs and observes them with counters.
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#include "engine/CUserProfileManager.h"

struct CWSraw {
    void* storage;
    void combine(const CWSraw& a);
    void fromLit(const unsigned short* lit);
};


extern "C" void __fastcall WSctor_0099aed0(CWSraw* self);
extern "C" void __fastcall WSdtor_0099b510(CWSraw* self);
extern "C" void __cdecl   FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot);
extern      CWideString __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right);

extern const unsigned short kFmtSave_0122de44[] = { 'S','a','v','e',' ','%','d',0 };
extern const unsigned short kSavePrefix_0122de90[] = { 'S','a','v','e',0 };


// ---- fakes to observe modeled behavior ----
static long g_slot = -1;
static int  g_seq = 0;
static int  g_ctorSeen = 0, g_combineSeen = 0, g_fromLitSeen = 0, g_addSeen = 0;
static int  g_dtorCount = 0;
static const CWideString* g_addResult = 0; static const CWideString* g_lastCopyFrom = 0; static int g_wideCopies = 0;

void __fastcall WSctor_0099aed0(CWSraw* s){ s->storage=(void*)0x1; g_ctorSeen = ++g_seq; }
void __fastcall WSdtor_0099b510(CWSraw* s){ (void)s; ++g_dtorCount; }
void __cdecl FmtWide_0099ba70(CWSraw* out, const unsigned short* fmt, long slot){
    out->storage=(void*)0x2; g_slot=slot; (void)fmt;
}
void CWSraw::combine(const CWSraw& a){ this->storage=(void*)0x3; g_combineSeen = ++g_seq; (void)a; }
void CWSraw::fromLit(const unsigned short* lit){ this->storage=(void*)0x4; g_fromLitSeen = ++g_seq; (void)lit; }
CWideString __fastcall AddWide_0099be70(CWSraw* left, CWSraw* right){
    g_addSeen = ++g_seq;
    // left must be the fromLit string (0x4), right the combine string (0x3)
    if (left->storage != (void*)0x4 || right->storage != (void*)0x3) g_addSeen = -1;
    CWideString r; g_addResult = &r; return r;
}
// out-of-line defs for the CWideString special members the link needs (declared in fable_string.h)
CWideString::CWideString(){ }
CWideString::CWideString(const CWideString& o){ g_lastCopyFrom = &o; ++g_wideCopies; }
CWideString::~CWideString(){ }

int main(){
    static double storage[64];                    // the manager's bytes are never read by the method
    const CUserProfileManager* mgr = (const CUserProfileManager*)storage;
    CWideString out = mgr->GetManualSaveFileName(7);
    bool ok = g_addResult != 0
           && g_slot == 7
           && g_ctorSeen == 1
           && g_combineSeen == 2
           && g_fromLitSeen == 3
           && g_addSeen == 4
           && g_dtorCount == 3;   // dtor(a) + ScopedWS lit + ScopedWS combine
    if (ok) printf("00406610_TEST PASS\n");
    else printf("FAIL slot=%ld ctor=%d comb=%d lit=%d add=%d dtor=%d\n",
                g_slot,g_ctorSeen,g_combineSeen,g_fromLitSeen,g_addSeen,g_dtorCount);
    return 0;
}