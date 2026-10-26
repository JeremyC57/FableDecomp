class CWideString
{ public:
    void* p;
    CWideString(const unsigned short* s);          // ctor helper 0x594a90 (this, str)        [factory ret]
    CWideString(const CWideString& o);             // user copy ctor -> non-POD, memory return
    ~CWideString();                                // dtor helper 0x597ec0 (this)
};

// The (lit,-1) temporary is a CCharString symbol name (relocation identity: ctor 0x0099EBF0, dtor 0x0099EAE0),
// handed to CDataBank::GetTextBySymbol 0x009C95E0.
class CCharString
{ public:
    void* p;
    CCharString(const char* s, long n);            // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

namespace NGameText {
class CDataBank
{ public:
    // struct-returning member: helper 0x5c29c0, __fastcall(this, hidden_ret, CWideString* fmt)
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;

struct NodeA { char pad[0x14]; CDataBank* obj14; };   // esi->[0x14]
struct NodeB { char pad[0x60]; CDataBank* obj60; };   // esi->[0x60]

extern NodeA* g_A;   // ds:0x13b86a0
extern NodeB* g_B;   // ds:0x13b871c

static const char* const LIT_AB = (const char*)0x122dfbc;
static const unsigned short* const LIT_FACTORY = (const unsigned short*)0x122de9c;

class CUserProfileManager
{ public:
    CWideString GetAutoSaveDisplayName();          // __fastcall member, returns CWideString by value
};

CWideString CUserProfileManager::GetAutoSaveDisplayName()
{
    NodeA* a = g_A;
    if (a) {
        CCharString fmt(LIT_AB, -1);
        return a->obj14->GetTextBySymbol(fmt);
    }
    NodeB* b = g_B;
    if (b) {
        CCharString fmt(LIT_AB, -1);
        return b->obj60->GetTextBySymbol(fmt);
    }
    return CWideString(LIT_FACTORY);
}