// Behaviour test for retail 0x008c35d0: the candidate source below is embedded verbatim (from src/008c35d0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// NCreatureMode::CModeBriarRoseTeleportInvulnerable::CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target)
//   -- CModeStandBase(thing), own vtable, the "INVULNERABLE" animation filter, PTarget = target, PEffect = null
// Leaf: ??0CModeBriarRoseTeleportInvulnerable
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C35D0 (`ret 8`); called from 0x008C1F3E. Vtable 0x01280228 =
//   NCreatureMode::CModeBriarRoseTeleportInvulnerable (rtti-vtables.tsv).
// Callees: CModeStandBase::CModeStandBase(CThing*) @ 0x00861E00 (manifest label "CTCCreatureNavigation" is propagated),
//   CCharString @ 0x0099EBF0 / 0x0099E4B0 / operator= 0x0099EFB0 / ~ 0x0099EAE0, CRC Calc @ 0x004014A0,
//   CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90.
// Shape: the filter (vtable 0x01267568 = CAnimFilterOfGroup) is built from an implicit CCharString conversion of the
//   literal (temporary destroyed after the filter); its CRC is ONE `p ? .. : ..` pair of guards that VC folds into a
//   single null test; then PTarget (vtable 0x0123E7D8, CIntelligentPointer<CThingCreatureBase>) and PEffect (vtable
//   0x01238C6C, CIntelligentPointer<CThing>). Timer is left uninitialised.
// Layout: PDB (Ego_r) CModeBriarRoseTeleportInvulnerable; retail offsets equal the PDB: CModeStandBase 0x68,
//   Filter +0x68 (0xc), Timer +0x74, PTarget +0x78, PEffect +0x80. Local model: no engine/ header for these classes.
struct CThing;
struct CBaseObject;

struct CCharStringData {
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (default argument: the literal converts implicitly)
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);   // one shared null test (CSE of the two guards)
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
struct CThingCreatureBase;

struct CModeStandBase {                    // retail sizeof 0x68 (CModeBase 0x1c + the stand-mode members)
    virtual void v00();
    unsigned char _body[0x64];
    CModeStandBase(CThing* thing);         // 0x00861E00
};

namespace NCreatureMode {

struct CModeBriarRoseTeleportInvulnerable : CModeStandBase {
    CAnimFilterOfGroup Filter;                          // +0x68
    long Timer;                                         // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;    // +0x78
    CIntelligentPointer<CThing> PEffect;                // +0x80
    virtual void v00();
    CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target);
};

CModeBriarRoseTeleportInvulnerable::CModeBriarRoseTeleportInvulnerable(CThing* thing, CThingCreatureBase* target)
    : CModeStandBase(thing)
    , Filter("INVULNERABLE")
    , PTarget(target)
    , PEffect()
{
}

}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_stand, g_live, g_ipctor, g_set; static CThing* g_standthing; static CBaseObject* g_item; static unsigned long g_crc_len; static const char* g_crc_text;
static CCharStringData g_datas[8]; static int g_ndata;
CCharString::CCharString() : m_pData(0) { ++g_live; }
CCharString::CCharString(const char* t, long) { ++g_live; CCharStringData* d = &g_datas[g_ndata++]; d->m_pData = t; d->m_length = (unsigned long)strlen(t); m_pData = d; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& o) { m_pData = o.m_pData; return *this; }
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length) {
    g_crc_text = (const char*)data; g_crc_len = length; unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + ((const unsigned char*)data)[i]; return h; }
void CAnimFilterOfGroup::v00() {}
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ipctor; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* i) { ++g_set; m_pItem = i; g_item = i; }
void CModeStandBase::v00() {}
CModeStandBase::CModeStandBase(CThing* t) { ++g_stand; g_standthing = t; }
void NCreatureMode::CModeBriarRoseTeleportInvulnerable::v00() {}
static unsigned char g_buf[sizeof(NCreatureMode::CModeBriarRoseTeleportInvulnerable)];
int main() {
    typedef NCreatureMode::CModeBriarRoseTeleportInvulnerable M;
    memset(g_buf, 0xcd, sizeof g_buf);
    CThing* thing = (CThing*)0x1000; CThingCreatureBase* target = (CThingCreatureBase*)0x2000;
    M* m = new (g_buf) M(thing, target);
    unsigned long expect = 0; for (const char* p = "INVULNERABLE"; *p; ++p) expect = expect * 31 + (unsigned char)*p;
    int ok = sizeof(M) == 0x88 && offsetof(M, Filter) == 0x68 && offsetof(M, Timer) == 0x74 && offsetof(M, PTarget) == 0x78 && offsetof(M, PEffect) == 0x80;
    ok = ok && g_stand == 1 && g_standthing == thing && g_live == 1;          /* only the filter's Group string survives */
    ok = ok && m->Filter.Group.m_pData && !strcmp(m->Filter.Group.m_pData->m_pData, "INVULNERABLE") && m->Filter.GroupCRC == expect && g_crc_len == 12;
    ok = ok && g_ipctor == 2 && g_set == 1 && m->PTarget.m_pItem == (CBaseObject*)target && m->PEffect.m_pItem == 0;
    ok = ok && m->Timer == (long)0xcdcdcdcd;                                     /* not initialised by the constructor */
    ok = ok && *(void**)&m->PTarget != *(void**)&m->PEffect;                    /* two template instances, two vtables */
    printf(ok ? "008c35d0_TEST PASS\n" : "008c35d0_TEST FAIL\n"); return ok ? 0 : 1;
}