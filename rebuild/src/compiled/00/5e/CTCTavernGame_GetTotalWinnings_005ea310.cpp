// CTCTavernGame::GetTotalWinnings() -- the "total winnings" text from the tavern-game def followed by the total winnings as a number
// Leaf: ?GetTotalWinnings@CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA310, 166 bytes (`ret 4`, CWideString by hidden pointer); CTCTavernGame
// vtable 0x012560BC slot 49. The catalog's "GetBet" is the neighbouring slot's name (GetBet is slot 47, 0x005EA1A0):
// Ego_r.pdb's source order UpdateValues, GetWinPhrase, GetBet, GetPlayersMoney, GetTotalWinnings, GetBanter maps onto
// 0x005EA010 .. 0x005EA3C0, and the def text read here is CTavernGameDef::TotalWinnings
// (`?GetTotalWinnings@CTCTavernGame@@UAE?AVCWideString@@XZ`).
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (landed as GetSubDef_005ed4a0), reached through the
//          inline CTCTavernGame::GetDef (its out-of-line copy is 0x005EA000); CWideString() 0x0099AED0; the wide
//          formatter 0x0099BA70 (`__cdecl (CWideString*, const wchar_t*, ...)`, L"%d" @ 0x0122DE44);
//          CWideString(const CWideString&) 0x0099B720; ~CWideString 0x0099B510 (x3); CTCTavernGame::GetText(unsigned
//          long) 0x005E9E70; operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall); the def's slot 1.
// Layout (retail CTCTavernGame; engine/CTCTavernGame.h carries Ego_r.pdb offsets, 8 bytes high from LastRenderTime
//         on, so a local model): ParentThing +0x04 (its def at +0x70), TotalWinnings +0x3c; CTavernGameDef::
//         TotalWinnings +0xf4 (PDB offset holds).
// Shape: `GetText(def->TotalWinnings) + IntToWideString(TotalWinnings)`: the right operand first -- the inline
//        IntToWideString (out-of-line copy 0x00405F00) reads its argument before building its local, formats, copies
//        it out (no NRVO) and destroys the local; GetDef is inlined in its own TU. The def pointer stays in edi from
//        the GetText argument to the short `dec [edi+4] ; jne` release with no null test and the zero store sinks below
//        the lookup's push only when VC7.1 can see the GetSubDef body (a stand-in compiled above under
//        `#pragma auto_inline(off)`, the retail build's whole-program knowledge; precedent 0x0075D2B0 / 0x007E4B60).
//        Only GetTotalWinnings is graded.
#include <stddef.h>

struct CWideString {
    void* m_data;
    CWideString();                              // 0x0099AED0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
};
void __cdecl FormatWideString(CWideString* text, const wchar_t* format, ...);   // 0x0099BA70
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);  // 0x0099BE70

inline CWideString IntToWideString(long value)
{
    CWideString text;
    FormatWideString(&text, L"%d", value);      // 0x0122DE44
    return text;
}

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual {
    long m_count;                               // +0x04
    unsigned char _pad_0x08[0xec];
    unsigned long TotalWinnings;                // +0xf4
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};
struct CDefinitionManager {
    CTavernGameDef* PeekDefFromGlobalIndex(long index);   // stand-in only
};
struct CDefClassBase {
    virtual long* GetSubDefIndex(const char* name);       // stand-in only (retail: slot 14, CCharString name)
    unsigned char       _pad_0x04[0x18];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef_005ed4a0(CDefPointer& out);  // 0x005ED4A0 (GetSubDef<CTavernGameDef>)
};

// Stand-in for the same-module GetSubDef<CTavernGameDef> body (0x005ED4A0, landed separately; not graded here):
// compiled above the caller without inlining so VC7.1 sees, as retail's whole-program build did, that the def pointer
// it fills does not escape -- the caller then keeps the def in edi and releases it with `dec [edi+4] ; jne`.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef_005ed4a0(CDefPointer& out)
{
    long* index = GetSubDefIndex("CTavernGameDef");
    if (index == 0)
        return false;
    CTavernGameDef* def = m_pManager->PeekDefFromGlobalIndex(*index);
    if (out.Object != def) {
        if (out.Object)
            out.Object->Release();
        out.Object = def;
        if (def)
            ++def->m_count;
    }
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PDef;                        // +0x70
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x34];
    long          TotalWinnings;                // +0x3c
    void GetDef(CDefPointer* out) { ParentThing->PDef->GetSubDef_005ed4a0(*out); }   // out of line 0x005EA000
    CWideString GetText(unsigned long id);      // 0x005E9E70
    virtual CWideString GetTotalWinnings();
};

CWideString CTCTavernGame::GetTotalWinnings()
{
    CDefPointer def;
    GetDef(&def);
    return GetText(def->TotalWinnings) + IntToWideString(TotalWinnings);
}