// Grade: DIFFER(388v435) -- inline-once destructor lane gap (advisor family of 00f09260): retail expands the by-value trader's inline ~CScriptThing at three returns and calls the out-of-line one (0x004AA840) at the hit-block terminate return; the bool temporary of the inlined CCharString == (frame byte [esp+13h]) is also not reproduced
// CQR_EscortTraderScript::WatchForExpressionFollow(CScriptThing trader) -- spawned per trader: each frame, stop when the trader dies; when the hero performs EXPRESSION_FOLLOW to it, restart the random-speech timer and set ExpressionDetectedFollow
// Leaf: ?WatchForExpressionFollow@CQR_EscortTraderScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1FCF0, 435 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, the thing's
//   implementation slots 75 IsAlive (+0x12c) and 29 MsgExpressionPerformedTo(CCharString&) (+0x74, wrapper 0x004AABF0),
//   Compare 0x00411570 (__fastcall, the inlined CCharString == const char*; the null-string arm is the strcmp intrinsic
//   against "" 0x0122D70E), EXPRESSION_FOLLOW 0x012757EC, RestartRandomSpeechTimer 0x00E1FEB0 (sets TraderRandomSpeechTimer +0x84 from
//   rand()), NewScriptFrame (interface slot 7), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840.
// Layout (retail = Ego_r.pdb - 0x14): ExpressionDetectedFollow +0x70.
#include <string.h>
#pragma intrinsic(strcmp)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (!PStringData)
            return strcmp("", text) == 0;
        return Compare_00411570(PStringData->m_pData, text) ? false : true;
    }
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
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
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;   // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028();
    virtual bool MsgExpressionPerformedTo(CCharString& expression) const;   // slot 29 (+0x74)
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const;   // slot 43 (+0xac)
    virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051();
    virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059();
    virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067();
    virtual void i068(); virtual void i069(); virtual void i070(); virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
    bool MsgExpressionPerformedTo(CCharString& expression) const   // 0x004AABF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgExpressionPerformedTo(expression);
    }
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CQR_EscortTraderScript : CQuestScriptBase {
    unsigned char _pad48[0x70 - 0x48];
    bool          ExpressionDetectedFollow;                 // +0x70
    bool          ExpressionDetectedWait;                   // +0x71
    void RestartRandomSpeechTimer();                        // 0x00E1FEB0 (TraderRandomSpeechTimer +0x84)
    void WatchForExpressionFollow(CScriptThing trader);
};

void CQR_EscortTraderScript::WatchForExpressionFollow(CScriptThing trader)
{
    CCharString expression;
    while (!IsActiveThreadTerminating()) {
        if (!trader.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            break;
        }
        if (IsActiveThreadTerminating())
            return;
        if (trader.MsgExpressionPerformedTo(expression) && expression == "EXPRESSION_FOLLOW") {
            if (IsActiveThreadTerminating())
                return;
            RestartRandomSpeechTimer();
            ExpressionDetectedFollow = true;
        }
        Game->NewScriptFrame();
    }
}
