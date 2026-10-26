#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* PItem;                        // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
typedef CIntelligentPointerConstThing CPtr;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CPtr* __fastcall find(CPtr* first, CPtr* last, const CPtr& value, const random_access_iterator_tag& tag);   // 0x007379D0

struct CPtrVector {                             // STLport vector<CIntelligentPointer<const CThing> > (12 bytes)
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    CPtr* begin() { return _M_start; }
    CPtr* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void push_back(const CPtr& value);          // 0x007398B0
};

class CTCGatherPointBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CPtrVector Users;                           // +0x0C
    long MaxOccupation;                         // +0x18
    bool AddAsUser(const CThing& user);
};

bool CTCGatherPointBase::AddAsUser(const CThing& user)
{
    CPtr* it;
    {
        CPtr pointer(&user);
        it = find(Users.begin(), Users.end(), pointer, random_access_iterator_tag());
    }
    if (it == Users.end()) {
        if (Users.size() <= (unsigned long)MaxOccupation)
            Users.push_back(CPtr(&user));
        else
            return false;
    }
    return true;
}

// ---- test stubs for the retail callees ----
class CThing { public: int Id; };
static CPtr* g_storage; static int g_dtors;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
void CBaseIntelligentPointer::SetPItem(const CThing* item) { PItem = item; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerConstThing::v00() {}
CPtr* __fastcall find(CPtr* first, CPtr* last, const CPtr& value, const random_access_iterator_tag&)
{ for (; first != last; ++first) if (first->PItem == value.PItem) break; return first; }
void CPtrVector::push_back(const CPtr& value)
{ _M_finish->PItem = value.PItem; ++_M_finish; }
void CTCGatherPointBase::v00() {}

int main()
{
    int ok = 1;
    unsigned char raw[8 * sizeof(CPtr)];
    CPtr* base = (CPtr*)raw;
    CThing a, b, c;
    CTCGatherPointBase gp; gp.Users._M_start = gp.Users._M_finish = gp.Users._M_end_of_storage = base;
    gp.MaxOccupation = 1;
    if (!gp.AddAsUser(a) || gp.Users.size() != 1 || base[0].PItem != &a) ok = 0;
    if (!gp.AddAsUser(a) || gp.Users.size() != 1) ok = 0;          // already a user
    if (!gp.AddAsUser(b) || gp.Users.size() != 2 || base[1].PItem != &b) ok = 0;   // size 1 <= max 1: room
    if (gp.AddAsUser(c) || gp.Users.size() != 2) ok = 0;            // size 2 > max 1: refused
    if (!ok) { std::printf("007ffeb0_TEST FAIL\n"); return 1; }
    std::printf("007ffeb0_TEST PASS\n");
    return 0;
}