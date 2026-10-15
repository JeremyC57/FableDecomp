// CPlayerGui::PeekGlobalDef() -- look up the "GLOBAL" definition and return it without keeping a reference (null when absent)
// Grade: DIFFER(117v123) -- lane gap: the inline counted release `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` (ours `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`); also retail calls CGameDefinitionManager::Get before pushing the name address (ours pushes &name first: wants a GetWorld-style inline helper); one try (known lane gap)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004384F0, length 123 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0
// (`__fastcall`, static), the definition manager's name lookup @ 0x009ADE10 (a CDefPointer by value through the hidden
// pointer; manifest label GetPDefFromGlobalIndex), ~CCharString @ 0x0099EAE0, the def's virtual Destroy (slot 1).
// String 0x01231734 "GLOBAL".
// Shape: the result counted pointer starts null (esi); on a hit it takes a reference, the lookup temporary is
// released by hand and nulled; the name dies, then the result's reference is dropped and its raw pointer returned.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long m_count;                                           // +0x04
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
        CDefBase* p = m_p;
        long count = p->m_count - 1;
        p->m_count = count;
        if (count == 0)
            p->Destroy();
        m_p = 0;
    }
};

struct CDefinitionManager {
    CDefPointer GetDef(const CCharString& name, bool flag); // 0x009ADE10
};

struct CGameDefinitionManager {
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};

struct CPlayerGui {
    static CDefBase* PeekGlobalDef();
};

CDefBase* CPlayerGui::PeekGlobalDef()
{
    CDefPointer result;
    {
        CCharString name("GLOBAL", -1);
        CDefPointer def = CGameDefinitionManager::Get().GetDef(name, true);
        if (def.m_p) {
            result.m_p = def.m_p;
            ++def.m_p->m_count;
            def.Reset();
        }
    }
    CDefBase* p = result.m_p;
    return p;
}
