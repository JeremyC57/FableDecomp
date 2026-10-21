// Grade: DIFFER(275v371) -- retail inlines ~CScriptThing at the loop end and calls 0x004AA840 at the terminating returns; the lane merges three out-of-line calls and keeps mov bl,1 where retail stores immediates
// CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy) -- every script frame until the thread ends: when the hero performs EXPRESSION_FOLLOW at the buddy flag ExpressionDetectedFollow and BuddyHelped, else when he performs EXPRESSION_WAIT flag ExpressionDetectedWait
// Leaf: WatchForExpressions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E32080, 384 bytes, `ret 0Ch` (the 12-byte CScriptThing by value).
// Callees: CCharString() @ 0x0099E4B0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, the inlined
//          CBasicString<char>::operator== calling Compare @ 0x00411570 (`__fastcall`; same-TU stand-in as the landed
//          CV_ChapelOfEvilScript::WatchForExpressions 0x00E6A000), CScriptThing::MsgExpressionPerformedTo (inline; the
//          game-side thing's slot 29, +0x74), ~CScriptThing @ 0x004AA840 / inline (counted release, operator delete
//          @ 0x00BFE9BC, ~CBaseClass @ 0x0099A2E0), CGameScriptInterface slot 7 NewScriptFrame (+0x1c).
// Layout: Ego_r.pdb CV_AmbushScamScript - 0x14: ExpressionDetectedFollow +0x62, ExpressionDetectedWait +0x63,
//          BuddyHelped +0x6a. CScriptThing model as the landed CQ_HangingTreeGoodScript (0x00D74F80).
// Shape: one CCharString local for the loop; `while (!terminating)` with an if / else-if over two `Msg && ==` tests;
//          the terminating returns inside the loop call the out-of-line ~CScriptThing, the loop end expands it.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (same-TU stand-in of the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                   // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const;
    virtual bool MsgExpressionPerformedTo(CCharString& expression) const { return PImp.Data && PImp.Data->MsgExpressionPerformedTo(expression); }   // slot 29 (+0x74)
};
struct CGameScriptInterface {   // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
};
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    unsigned char _pad48[0x62 - 0x48];                   // +0x48 QuestFinished .. +0x61 AllAmbushersDead
    bool ExpressionDetectedFollow;                       // +0x62
    bool ExpressionDetectedWait;                         // +0x63
    bool HeroAttackOnBuddy;                              // +0x64
    bool HeroAttackOnTricksterBeforeAmbush;              // +0x65
    bool HeroAttackOnTricksterAfterAmbush;               // +0x66
    bool HeroAttackOnTricksterKillingBlow;               // +0x67
    bool HeroTalkToTrickster;                            // +0x68
    bool HeroAttachOnTricksterWarning;                   // +0x69
    bool BuddyHelped;                                    // +0x6a
    void WatchForExpressions(CScriptThing buddy);
};

void CV_AmbushScamScript::WatchForExpressions(CScriptThing buddy)
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
            if (IsActiveThreadTerminating())
                return;
            ExpressionDetectedFollow = true;
            BuddyHelped = true;
        } else if (buddy.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_WAIT") {
            if (IsActiveThreadTerminating())
                return;
            ExpressionDetectedWait = true;
        }
        GSI->NewScriptFrame();
    }
}
