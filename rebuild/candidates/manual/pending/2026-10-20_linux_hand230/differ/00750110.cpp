// CTCChest::DisplayLockedMessage() -- for a chest that needs openers, show the "locked" subtitle: the single-key text, or the multiple-keys text with the number of keys filled in
// Grade: DIFFER(299v298) -- register choice only: retail reads ParentThing into eax (`mov eax,[edi+4] ; mov ecx,[eax+70h] ; lea edx`) and the single-key arm loads g_pMainGameComponent into eax (`a1` form) where ours uses ecx; a named thing-def local fixes the prologue but scrambles both arms (probe v3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750110, 298 bytes (+ int3 padding), plain `ret`.
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) @ 0x0074F7D0 (manifest
//   "GetSubDef<CAICreatureWillPowerIndicatorDef>" is a byte-twin propagation; landed 0x00750240 names it),
//   CWideString() @ 0x0099AED0 (manifest "CCharString" is propagated), CCharString(const char*, long) @ 0x0099EBF0,
//   CDataBank::GetTextBySymbol @ 0x009C95E0 (the text bank, g_pMainGameComponent 0x013B86A0 -> +0x14),
//   CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0, ConstructString(CWideString*,
//   long) @ 0x009C7B70 (__fastcall), CTCBase::GetGameSubsystem @ 0x00686D60 (its +0x1614C font), CSubtitleRenderer::SetText
//   @ 0x00492E4B (g_pPlayerGui 0x013B8790 -> +0x18), the def's virtual slot 1 (+0x04, delete at the last reference).
// Literals: "TEXT_GUI_CHEST_LOCKED_SINGLE_KEY" @ 0x01269538, "TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS" @ 0x01269514.
// Layout: retail CThing PDef +0x70; CChestDef (Ego_r.pdb 0x3C): OpenerObject +0x34, OpenersRequired +0x38.
// Shape: the def taken out of the out pointer into a holder right after GetSubDef (landed CTCChest::
//   GetOpenAnimationForCreature 0x00750240 idiom, released with a null test at the end); the text is a named
//   CWideString assigned from each arm's GetTextBySymbol temporary (implicit CCharString conversion of the literal);
//   SetText's flags 0x122, no time, not click-past, pause.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    unsigned short* m_pData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
bool __fastcall ConstructString(CWideString* text, long value);   // 0x009C7B70
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) const;  // 0x009C95E0
};
class CFontBank;
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank* Font;                                    // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CSubtitleRenderer {
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time,
                 bool clickPast, bool pauseGame);       // 0x00492E4B
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

struct CDefPointeeBase {
    virtual void Destroy();                             // slot 0
    virtual void Delete();                              // slot 1 (+0x04)
    long m_refCount;                                    // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def taken out of the out pointer, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CChestDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    long OpenerObject;                                  // +0x34
    long OpenersRequired;                               // +0x38
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CChestDef>* out);        // 0x0074F7D0
};
void* FindDef_CDefClassBase_GetSubDef();       // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 / 0x00744ED0 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CChestDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
struct CTCChest {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CMainGameComponent* GetGameSubsystem();             // 0x00686D60 (CTCBase)
    void DisplayLockedMessage();
};

void CTCChest::DisplayLockedMessage()
{
    CDefPointer<CChestDef> out;
    bool found = ParentThing->PDef->GetSubDef(&out);
    CDefHolder<CChestDef> def(out.Object);
    if (found && def->OpenerObject > 0 && def->OpenersRequired > 0) {
        CWideString text;
        if (def->OpenersRequired == 1) {
            text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_SINGLE_KEY");
        } else {
            text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS");
            ConstructString(&text, def->OpenersRequired);
        }
        g_pPlayerGui->SubtitleRenderer.SetText(*GetGameSubsystem()->Font, text, 0x122, 0.0f, false, true);
    }
}
