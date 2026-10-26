// CPlayerGui::PeekGlobalDef() -- look up the "GLOBAL" definition and return it without keeping a reference (null when absent)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004384F0, length 123 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0
// (`__fastcall`, static), the definition manager's name lookup @ 0x009ADE10 (a CDefPointer by value through the hidden
// pointer; manifest label GetPDefFromGlobalIndex), ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// String 0x01231734 "GLOBAL".
// Shape: the result counted pointer starts null (esi); on a hit it takes a reference, the lookup temporary is
// released by hand and nulled; the name dies, then the result's reference is dropped and its raw pointer returned.
// The manager reference is a named local (Get() runs before the GetDef pushes); the hand release calls the def's own
// inline Release() member (`if (--m_count == 0) Destroy();`), which gives retail's `mov ecx,eax ; add eax,4 ; dec edx`
// form (the object copied to ecx for the virtual call, the count addressed through eax).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long m_count;                                           // +0x04
    void Release() { if (--m_count == 0) Destroy(); }
};

struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p && --m_p->m_count == 0)
            m_p->Destroy();
    }
    void Reset()                                            // the hand-written release of the lookup temporary
    {
        m_p->Release();
        m_p = 0;
    }
};

class CDefinitionManager { public:
    CDefPointer GetDef(const CCharString& name, bool flag); // 0x009ADE10
};

class CGameDefinitionManager { public:
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

class CPlayerGui { public:
    static CDefBase* PeekGlobalDef();
};

CDefBase* CPlayerGui::PeekGlobalDef()
{
    CDefPointer result;
    {
        CCharString name("GLOBAL", -1);
        CDefinitionManager& manager = CGameDefinitionManager::Get();
        CDefPointer def = manager.GetDef(name, true);
        if (def.m_p) {
            result.m_p = def.m_p;
            ++def.m_p->m_count;
            def.Reset();
        }
    }
    CDefBase* p = result.m_p;
    return p;
}