// Grade: DIFFER(60v60) -- unchanged best (debakeB): only `lea eax,[ebp-4]` is scheduled before `push 1` (retail after). debakeB2 tried 6 more, all the same swap: bool index, unsigned long index, same-TU noinline GetDef stand-in (sibling 0x0047D5A3 style), /G5, /G7, an inline m_pFrontEndDef accessor.
// CNewFrontendGameComponent::InitialiseDefs() -- compile the defs, then fetch the "FRONT_END" front-end def
// Retail: Fable.exe @ 0x0042F722, 60 bytes, frame-pointer size TU (`push ebp ; mov ebp,esp ; push ecx` ... `leave`).
// Calls the static CompileDefs (0x0042F70B, landed), then CDefinitionManager::GetDef<CFrontEndDef>(name, 1,
// m_pFrontEndDef) (0x004300FB) on the manager returned by CMainGameComponent::GetDefinitionManager (0x0043368D,
// reads 0x013B8760). The CCharString("FRONT_END") temporary (ctor 0x0099EBF0, dtor 0x0099EAE0) lives in [ebp-4];
// The literal converts implicitly, so VC builds the temporary before the first argument push (an explicit
// CCharString(...) argument interleaves with the pushes). m_pFrontEndDef is at +0x3C
// (Ego_r.pdb CNewFrontendGameComponent); no engine header exists, local model.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
private:
    char* m_pData;
};
class CFrontEndDef;
template <class T> struct CDefPointer {
    const T* Object;
};
class CDefinitionManager {
public:
    template <class T> bool GetDef(const CCharString& name, long index, CDefPointer<T>& def);   // 0x004300FB (T = CFrontEndDef)
};
class CMainGameComponent {
public:
    static CDefinitionManager* GetDefinitionManager();                   // 0x0043368D
};
class CNewFrontendGameComponent {
public:
    static void __fastcall CompileDefs();                                // 0x0042F70B
    void InitialiseDefs();
    unsigned char             _pad00[0x3c];
    CDefPointer<CFrontEndDef> m_pFrontEndDef;                            // +0x3C
};
void CNewFrontendGameComponent::InitialiseDefs()
{
    CompileDefs();
    CMainGameComponent::GetDefinitionManager()->GetDef("FRONT_END", 1, m_pFrontEndDef);
}
