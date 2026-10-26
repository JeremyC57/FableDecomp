// Test for 0x004047A0 CStringParser::~CStringParser: members destroyed in reverse order, then the base.
#include <stdio.h>
static char gOrder[8]; static int gN = 0;
class CBaseClass {
public:
    virtual ~CBaseClass() { gOrder[gN++] = 'B'; }
};
class CCharStringData;
class CCharString {
public:
    ~CCharString() { gOrder[gN++] = (char)(long)PStringData; }
    CCharStringData* PStringData;
};
struct CStringParserCommentBoundaries { long Start; long End; };
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    ~list() { gOrder[gN++] = 'L'; }
    void* _M_node;
};
}
struct CCursorPos { bool InQuotes; long StringPos; long Line; };
class CStringParser : public CBaseClass {               // no user-declared destructor (see Shape)
public:
    char* String;
    unsigned long BaseLineOffset;
    CCharString Filename;
    CCharString TempCharString;
    std::list<CStringParserCommentBoundaries> Comments;
    void (__cdecl* ErrorCallback)(CCharString*);
    CCursorPos CursorPos;
};
int main()
{
    bool ok = sizeof(CStringParser) == 0x28;
    CStringParser* p = new CStringParser;
    p->Filename.PStringData = (CCharStringData*)'F'; p->TempCharString.PStringData = (CCharStringData*)'T';
    CBaseClass* b = p;
    delete b;                                               // through the virtual destructor
    gOrder[gN] = 0;
    ok = ok && gN == 4 && gOrder[0] == 'L' && gOrder[1] == 'T' && gOrder[2] == 'F' && gOrder[3] == 'B';
    if (!ok) { printf("004047a0_TEST FAIL %s\n", gOrder); return 1; }
    printf("004047a0_TEST PASS\n");
    return 0;
}
