// Test for 0x00686800 CTCBase::CTCBase(CThing&): links the source object; the base constructor runs once, ParentThing
// is the referenced thing, both flags are cleared and the object's vtable is CTCBase's (its virtual destructor runs).
#include <stdio.h>
#include <string.h>
#include <new>
static int g_base, g_dtor;
class CThing { public: int dummy; };
class CBaseClassNonCopyable { public: CBaseClassNonCopyable(); virtual ~CBaseClassNonCopyable(); };
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_base; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
class CTCBase : public CBaseClassNonCopyable {
public:
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase(CThing& parent_thing);
    virtual ~CTCBase();
};
CTCBase::~CTCBase() { ++g_dtor; }
static unsigned char g_raw[sizeof(CTCBase)];
int main()
{
    CThing owner;
    memset(g_raw, 0x5a, sizeof(g_raw));
    CTCBase* b = new (g_raw) CTCBase(owner);
    bool ok = b == (CTCBase*)g_raw && b->ParentThing == &owner && !b->ToBeRemoved && !b->IsInGlobalUpdateTCs
           && g_base == 1;
    CBaseClassNonCopyable* p = b;
    p->~CBaseClassNonCopyable();                   // virtual: must reach ~CTCBase through the CTCBase vtable
    ok = ok && g_dtor == 1;
    if (!ok) { puts("00686800_TEST FAIL"); return 1; }
    puts("00686800_TEST PASS");
    return 0;
}
