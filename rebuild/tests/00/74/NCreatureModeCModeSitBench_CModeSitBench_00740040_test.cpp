// Test for 0x00740040 NCreatureMode::CModeSitBench::CModeSitBench(CThing&) on engine/NCreatureMode_CModeSitFloor.h (NF242, q12677):
// links the row object (the constructor, its vtable ??_7CModeSitBench@NCreatureMode@@6B@ and its scalar deleting destructor) with
// stubs for every symbol the row leaves undefined: CModeSitBase (ctor, dtor, the slot 2 / 3 / 5 overrides), the CModeBase slots
// 6..11 and ~CModeBase / ctor, CModeSitBench::GetType / GetAnimFilter, CCharString (ctors, dtor, assignment, NullChar), CCRC::Calc and
// the CAnimFilterOfGroup slots. Cases: (1) the base ctor gets the thing; (2) the filter's group is the "SIT_BENCH" string the row
// builds, its CRC is Calc(0, text, 9) and the temporary string is destroyed; (3) every vtable slot 1..11 dispatches to the function its
// PDB name says (slot 9 IsAllowingPlayerSpecialMovementMoves, slot 11 IsRemoveOnDeactivate); (4) deleting through the base pointer
// runs ~Group, then ~CModeSitBase, then ~CModeBase, then operator delete.
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_CTOR
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_FRAMEUPDATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_IMPLICIT_DTOR
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

static int g_events[32];
static int g_count;
static void Note(int e) { if (g_count < 32) g_events[g_count++] = e; }
static int g_hits[16];
static CThing* g_baseThing;
static const void* g_calcData;
static unsigned long g_calcLen;
static int g_strings;

// CBasicString's text pointer / length are protected: a test-only accessor class (never constructed) fills the stub string data.
class CBasicStringProbe : public CBasicString<char> {
public:
    void Set(char* text, unsigned long length) { PData = text; StringLength = length; }
};
static CCharStringData g_data[4];
static char g_text[4][16];
static int g_used;

const char CCharString::NullChar = 0;
CCharString::CCharString() : PStringData(0) { ++g_strings; }
CCharString::CCharString(const char* text, long)
{
    ++g_strings;
    strcpy(g_text[g_used], text);
    static_cast<CBasicStringProbe&>(g_data[g_used].Data).Set(g_text[g_used], (unsigned long)strlen(text));
    PStringData = &g_data[g_used++];
}
CCharString::~CCharString() { --g_strings; Note(10); }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed, const void* data, unsigned long count)
{
    g_calcData = data; g_calcLen = count; return seed + 0x5151u + count;
}
bool CAnimFilterOfGroup::operator()(const CAnimationEntry&) const { return false; }
CAnimFilterBase* CAnimFilterOfGroup::Clone() const { return 0; }

CTCCreatureModeManager::CModeBase::CModeBase(CThing& parent_thing)
    : ParentThing(parent_thing), FramesBeforeRemovingSelf(0), Active(false) { }
CTCCreatureModeManager::CModeBase::~CModeBase() { Note(30); }
void CTCCreatureModeManager::CModeBase::AnimUpdate(CAnimateThing&) { g_hits[2] = -1; }
void CTCCreatureModeManager::CModeBase::FrameUpdate() { g_hits[3] = -1; }
const CAnimFilterBase& CTCCreatureModeManager::CModeBase::GetAnimFilter() const { ++g_hits[12]; static CAnimFilterOfGroup none = CCharString(); return none; }
void CTCCreatureModeManager::CModeBase::OnActivate() { g_hits[5] = -1; }
void CTCCreatureModeManager::CModeBase::OnDeactivate() { ++g_hits[6]; }
float CTCCreatureModeManager::CModeBase::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) const { ++g_hits[7]; return 0.0f; }
bool CTCCreatureModeManager::CModeBase::IsIdleMode() const { ++g_hits[8]; return false; }
bool CTCCreatureModeManager::CModeBase::IsAllowingPlayerSpecialMovementMoves() const { ++g_hits[9]; return false; }
bool CTCCreatureModeManager::CModeBase::IsHoldWeaponWithBothHands() const { ++g_hits[10]; return true; }
bool CTCCreatureModeManager::CModeBase::IsRemoveOnDeactivate() const { ++g_hits[11]; return true; }

NCreatureMode::CModeSitBase::CModeSitBase(CThing& thing) : CTCCreatureModeManager::CModeBase(thing) { g_baseThing = &thing; }
NCreatureMode::CModeSitBase::~CModeSitBase() { Note(20); }
void NCreatureMode::CModeSitBase::AnimUpdate(CAnimateThing&) { ++g_hits[2]; }
void NCreatureMode::CModeSitBase::FrameUpdate() { ++g_hits[3]; }
void NCreatureMode::CModeSitBase::OnActivate() { ++g_hits[5]; }
unsigned long NCreatureMode::CModeSitBench::GetType() const { ++g_hits[1]; return 0x5B; }
const CAnimFilterBase& NCreatureMode::CModeSitBench::GetAnimFilter() const { ++g_hits[4]; return Filter; }

void operator delete(void* p) { Note(40); free(p); }

static int Fail(int n) { printf("00740040_TEST FAIL %d\n", n); return 1; }

int main()
{
    static unsigned char thingBuf[0x400], animBuf[0x40];
    CThing& thing = *reinterpret_cast<CThing*>(thingBuf);
    NCreatureMode::CModeSitBench* bench = new NCreatureMode::CModeSitBench(thing);
    if (g_baseThing != &thing || &bench->GetParentThing() != &thing) return Fail(1);
    if (g_strings != 1 || g_count != 1 || g_events[0] != 10) return Fail(2);           // the "SIT_BENCH" temporary is gone
    CTCCreatureModeManager::CModeBase* mode = bench;
    const CAnimFilterOfGroup& filter = static_cast<const CAnimFilterOfGroup&>(mode->GetAnimFilter());
    if (g_hits[4] != 1 || (const void*)&filter != (const void*)((const unsigned char*)bench + 0x2c)) return Fail(3);
    if (strcmp(filter.Group.GetAsCharArray(), "SIT_BENCH") != 0 || filter.Group.GetLength() != 9) return Fail(4);
    if (g_calcData != g_text[0] || g_calcLen != 9 || filter.GroupCRC != 0x5151u + 9) return Fail(5);
    if (mode->GetType() != 0x5B || g_hits[1] != 1) return Fail(6);
    mode->AnimUpdate(*reinterpret_cast<CAnimateThing*>(animBuf));
    mode->FrameUpdate();
    mode->OnActivate();
    if (g_hits[2] != 1 || g_hits[3] != 1 || g_hits[5] != 1) return Fail(7);              // the CModeSitBase overrides
    mode->OnDeactivate();
    if (mode->IsIdleMode() || g_hits[8] != 1) return Fail(8);
    if (mode->IsAllowingPlayerSpecialMovementMoves() || g_hits[9] != 1) return Fail(9);  // slot 9
    if (!mode->IsHoldWeaponWithBothHands() || g_hits[10] != 1) return Fail(10);
    if (!mode->IsRemoveOnDeactivate() || g_hits[11] != 1) return Fail(11);              // slot 11
    if (g_hits[6] != 1 || g_hits[7] != 0) return Fail(12);
    g_count = 0;
    delete mode;
    if (g_count != 4 || g_events[0] != 10 || g_events[1] != 20 || g_events[2] != 30 || g_events[3] != 40 || g_strings != 0) return Fail(13);
    printf("00740040_TEST PASS\n");
    return 0;
}
