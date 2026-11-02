// Standalone C++03 fixture: the exact static array definition below is the target.
// The string constructor/destructor are traced test boundaries, not game implementations.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
class CDefPointeeBase;
class CThing;
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
#endif

class CTCOracleMinigame {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned long    CurrentText;                            // +0x0c
    long             Level;                                  // +0x10
    unsigned char    Stage[0x4];                             // +0x14 EOracleMinigameState
    unsigned char    PostWaitStage[0x4];                     // +0x18 EOracleMinigameState
    long             WaitTime;                               // +0x1c
    float            TimeBetweenOutputs;                     // +0x20
    long             NumOutputsFired;                        // +0x24
    long             Output[32];                             // +0x28
    bool             Success;                                // +0xa8
    unsigned char    _pad_0xa9[0x3];                         // +0xa9
    long             NextOutputFrame;                        // +0xac
    CThing*          Symbols[4];                             // +0xb0
    long             SymbolsFiring[4];                       // +0xc0
    long             FramesFired[4];                         // +0xd0
    unsigned char    InputsPending[0x4];                     // +0xe0 list<EOracleMinigameInput,std::allocator<EOracleMinigameInput>_>
    CThing*          SpeechMarker;                           // +0xe4
    CDefPointeeBase* PDef_Object;                            // +0xe8 CDefPointer<COracleMinigameDef_const_>::Object
// Retail opt-in (Codex pass 11): Ego_r TPI 0x33b4c field 0x340;
// private const four-element array; native initializer writes four pointer words.
private:
    static const CCharString SymbolStrings[4];

};
static const void* constructed[4];
static const char* supplied[4];
static long lengths[4];
static unsigned constructCount;
static unsigned destroyCount;
static int fixtureFailed;
static int mainReached;
static void finishAudit();
class TestAuditRegistration {
public:
    TestAuditRegistration() { if (atexit(finishAudit) != 0) abort(); }
};
static TestAuditRegistration auditRegistration;

const CCharString CTCOracleMinigame::SymbolStrings[4] = {
    "HUD_EXPR_TABLET_Y",
    "HUD_EXPR_TABLET_C",
    "HUD_EXPR_TABLET_A",
    "HUD_EXPR_TABLET_M"
};

CCharString::CCharString(const char* text, long length) : PStringData(0) {
    if (constructCount >= 4) abort();
    constructed[constructCount] = this;
    supplied[constructCount] = text;
    lengths[constructCount] = length;
    ++constructCount;
}
CCharString::CCharString(const CCharString&) : PStringData(0) {
    // Static conversion construction must not rely on elision of a named return value.
    abort();
}
CCharString::~CCharString() {
    if (destroyCount >= 4 || this != constructed[3 - destroyCount]) abort();
    ++destroyCount;
}
int main() {
    const char* expected[4] = {
        "HUD_EXPR_TABLET_Y",
        "HUD_EXPR_TABLET_C",
        "HUD_EXPR_TABLET_A",
        "HUD_EXPR_TABLET_M"
    };
    mainReached = 1;
    if (constructCount != 4 || destroyCount != 0) fixtureFailed = 1;
    for (unsigned i = 0; i != 4; ++i) {
        if (lengths[i] != -1 || strcmp(supplied[i], expected[i])) fixtureFailed = 1;
        if (i && (const unsigned char*)constructed[i] -
                 (const unsigned char*)constructed[i-1] != sizeof(CCharString)) fixtureFailed = 1;
    }
    return fixtureFailed;
}
static void finishAudit() {
    if (!mainReached || fixtureFailed || constructCount != 4 || destroyCount != 4) abort();
    puts("0121f280_TEST PASS");
}
