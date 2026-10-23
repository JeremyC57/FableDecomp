// Grade: DIFFER(884v848) -- structure and frame: retail homes the by-value group-entry pointer and the GetText result in the dead text/flag argument slots, tail-merges the front-end plain-string path into the game path's (je into the other branch), releases the intrusive pointers through a register copy of the count (mov/dec/mov) and caches this in a frame slot; ours keeps a 0x10 frame, duplicates the branches and releases with dec [mem]; the decoded flow (game bank or front-end bank, plain entry vs group joined by space/newline, FormatTextWithGameActionNames, FitInWindow, font measure) is modelled
// NUISystem::CText::SetValue(unsigned long textID, const CWideString& text, bool spaceSeparated) -- set the displayed
// text: a game-text entry (a plain string, or a group whose entries are joined with a space or a newline) looked up in
// the game's text bank (or the front end's when there is no game), else the given text; then substitute the game
// action names, word-wrap it to the window and cache the line count and the font's measured width
// Leaf: ?SetValue@CText@NUISystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FBC0, `ret 0xc`.
// Callees: CBankFile::GetEntryDataType 0x009CC210, NGameText::CDataBank::GetPGroupEntry 0x009C9070 (string entry) /
//   CDataBank::GetPGroupEntry 0x009C8890 (group entry), CGroupEntry::GetText 0x009C7A40, CWideString operator=
//   0x0099B7D0 / operator+=(const CWideString&) 0x0099B8D0 / operator+=(const wchar*) 0x0099B940 / GetLength 0x0099B220 /
//   ~CWideString 0x0099B510 / operator const wchar* 0x0099AF10, vector<unsigned long> copy 0x00514160, free 0x00BFEA14,
//   FormatTextWithGameActionNames 0x004917B5 (__fastcall, hidden result), CText::FitInWindow 0x0054F8E0, the font's
//   virtual +4 (measure).
// Layout: retail NUISystem::CText = Ego_r.pdb minus 0x30: line count +0xcc, height +0xd0, Text +0x15c, PFont +0x160,
//   window +0x178. Globals: g_pMainGameComponent 0x013B86A0 (+0x14 text bank), the front end 0x013B871C (+0x60 text
//   bank), empty wide string 0x013BCA24, L" " 0x01249F28, L"\n" 0x01231728.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
typedef unsigned short wchar16;
struct CWideString {
    void* PStringData;
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);    // 0x0099B8D0
    const CWideString& operator+=(const wchar16* text);     // 0x0099B940
    long GetLength() const;                                 // 0x0099B220
    ~CWideString();                                         // 0x0099B510
    const wchar16* GetString(long start) const;             // 0x0099AF10
};
CWideString __fastcall FormatTextWithGameActionNames(const CWideString& text, bool flag);   // 0x004917B5
extern CWideString g_EmptyWideString;                       // 0x013BCA24
struct CIVCountedPointee {
    virtual void v0();
    virtual void Destroy();                                 // slot 1
    long RefCount;                                          // +4
};
template<class T> struct CIVCountedPointer {
    T* Obj;
    ~CIVCountedPointer()
    {
        if (Obj) {
            if (--Obj->RefCount == 0)
                Obj->Destroy();
            Obj = 0;
        }
    }
    T* operator->() const { return Obj; }
};
struct CULongVector {
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CIDVector : CULongVector {
    CIDVector(const CULongVector& other);                   // 0x00514160
    ~CIDVector() { if (_M_start) free(_M_start); }
};
struct CGroupEntry : CIVCountedPointee {
    CWideString GetText() const;                            // 0x009C7A40
};
struct CGroupListEntry : CIVCountedPointee {
    unsigned char _pad08[0x1c - 0x08];
    CULongVector  Entries;                                  // +0x1c
};
struct CTextBank {
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
    CIVCountedPointer<CGroupListEntry> GetPGroupListEntry(unsigned long index, bool load) const;   // 0x009C8890
};
struct CMainGameComponent { unsigned char _pad[0x14]; CTextBank* TextBank; };
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
struct CFrontEnd { unsigned char _pad[0x60]; CTextBank* TextBank; };
extern CFrontEnd* g_pFrontEnd;                              // 0x013B871C
struct CFontMetrics { long Width; };
struct CFont {
    virtual void v0();
    virtual CFontMetrics Measure(const wchar16* text) const;   // slot 1
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };

namespace NUISystem {
class CText {
public:
    unsigned char _pad00[0xcc];
    long          Width;                                    // +0xcc
    float         LineCount;                                // +0xd0
    unsigned char _padD4[0x15c - 0xd4];
    CWideString   Text;                                     // +0x15c
    CFont*        PFont;                                    // +0x160
    unsigned char _pad164[0x178 - 0x164];
    C2DBoxF       Window;                                   // +0x178
    long FitInWindow(CWideString& text, const C2DBoxF& window);   // 0x0054F8E0
    void SetValue(unsigned long textID, const CWideString& text, bool spaceSeparated);
};

void CText::SetValue(unsigned long textID, const CWideString& text, bool spaceSeparated)
{
    if (textID) {
        if (g_pMainGameComponent) {
            if (g_pMainGameComponent->TextBank->GetEntryDataType(textID)) {
                CULongVector* entries = &g_pMainGameComponent->TextBank->GetPGroupListEntry(textID, true)->Entries;
                Text = g_EmptyWideString;
                for (unsigned long i = 0; i < entries->size(); ++i) {
                    if (Text.GetLength())
                        Text += spaceSeparated ? (const wchar16*)L" " : (const wchar16*)L"\n";
                    Text += g_pMainGameComponent->TextBank->GetPGroupEntry(entries->_M_start[i], true)->GetText();
                }
            } else {
                Text = g_pMainGameComponent->TextBank->GetPGroupEntry(textID, true)->GetText();
            }
        } else if (g_pFrontEnd) {
            if (g_pFrontEnd->TextBank->GetEntryDataType(textID)) {
                CIDVector entries(g_pFrontEnd->TextBank->GetPGroupListEntry(textID, true)->Entries);
                Text = g_EmptyWideString;
                for (unsigned long i = 0; i < entries.size(); ++i) {
                    if (Text.GetLength())
                        Text += spaceSeparated ? (const wchar16*)L" " : (const wchar16*)L"\n";
                    Text += g_pFrontEnd->TextBank->GetPGroupEntry(entries._M_start[i], true)->GetText();
                }
            } else {
                Text = g_pFrontEnd->TextBank->GetPGroupEntry(textID, true)->GetText();
            }
        } else {
            Text = text;
        }
    } else {
        Text = text;
    }
    if (g_pMainGameComponent)
        Text = FormatTextWithGameActionNames(Text, false);
    LineCount = (float)FitInWindow(Text, Window);
    Width = PFont->Measure(Text.GetString(0)).Width;
}
}
