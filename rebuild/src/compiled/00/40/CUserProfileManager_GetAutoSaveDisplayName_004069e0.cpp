typedef unsigned short wchar16;

class CWideString { public:
    const wchar16* p;
    CWideString(const wchar16* s);          // 1-arg ctor 0x594cd0 (else branch)
    ~CWideString();                          // dtor 0x598100
};

// The (lit,-1) temporary is a CCharString symbol name (relocation identity: ctor 0x0099EBF0, dtor 0x0099EAE0),
// handed to CDataBank::GetTextBySymbol 0x009C95E0.
class CCharString { public:
    const char* p;
    CCharString(const char* s, long n);      // 0x0099EBF0
    ~CCharString();                          // 0x0099EAE0
};

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol); // 0x009C95E0, returns by value
};

struct SlotA { char pad[0x14]; CDataBank* f14; };
struct SlotB { char pad[0x60]; CDataBank* f60; };

extern SlotA* g_13b86a0;
extern SlotB* g_13b871c;

extern const char k_122df40[];
extern const wchar16 k_122de80[];

class CUserProfileManager { public:
    CWideString GetAutoSaveDisplayName();
};

CWideString CUserProfileManager::GetAutoSaveDisplayName()
{
    SlotA* a = g_13b86a0;
    if (a) {
        CCharString t(k_122df40, -1);
        return a->f14->GetTextBySymbol(t);
    }
    SlotB* b = g_13b871c;
    if (b) {
        CCharString t(k_122df40, -1);
        return b->f60->GetTextBySymbol(t);
    }
    return CWideString(k_122de80);
}