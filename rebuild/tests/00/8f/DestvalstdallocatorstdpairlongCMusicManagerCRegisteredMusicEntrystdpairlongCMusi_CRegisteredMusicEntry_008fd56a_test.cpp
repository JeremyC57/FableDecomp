#include <cstdio>
// CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase() (retail 0x008FD56A): the implicit destructor -- members in reverse declaration order, then the base chain
//   (~CAIStateGroupBase: Name +0x0C, then ~CBaseClass). (No vptr check: at the test's /Od VC7.1 stores it, the optimised retail form does not.)
// NF471 (T3 nf303 0x008FD288): self-contained -- the row is now a thin member of its owner's TU, so the test models the retail classes with
//   IMPLICIT destructors (both PDBs flag them compiler-generated) and VC7.1 generates the chain here, as it does in the row's object.
static long g_seq[16]; static int g_n = 0; static char* g_base = 0;
static void Note(void* p) { if (g_n < 16) g_seq[g_n] = (long)((char*)p - g_base); g_n++; }
class CBaseClass { public: virtual ~CBaseClass(); };
class CCharString { public: ~CCharString(); void* PString; };
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); void* PData; };
class CAIBrain;
class CAIStateGroupBase : public CBaseClass { public: CAIBrain* PBrain; long ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled; };
CBaseClass::~CBaseClass() { Note(this); g_seq[15] = 1; }
CCharString::~CCharString() { Note(this); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { Note(this); }
class CAIStateGroup_UsesSeat : public CAIStateGroupBase { public: CBaseIntelligentPointer PSeat; char _pad1c[24]; };
class CAIStateGroup_MinionSitOnSeatBase : public CAIStateGroup_UsesSeat { public: CBaseIntelligentPointer POwnSeat; };
int main()
{
    void* buf[0x60] = {(void*)0x1234};
    g_base = (char*)buf;
    ((CAIStateGroup_MinionSitOnSeatBase*)buf)->CAIStateGroup_MinionSitOnSeatBase::~CAIStateGroup_MinionSitOnSeatBase();
    static const long want[] = {0x34, 0x14, 0xc, 0x0};
    if (g_n != 4) { std::printf("BAD_COUNT %d\n", g_n); return 1; }
    for (int i = 0; i < 4; i++) if (g_seq[i] != want[i]) { std::printf("BAD_ORDER %d %ld\n", i, g_seq[i]); return 1; }
    std::printf("008fd56a_TEST PASS\n"); return 0;
}
