#ifndef FABLE_ENGINE_CCONSOLE_H
#define FABLE_ENGINE_CCONSOLE_H
#include "engine/CTBaseSingleton.h"
#include "engine/CConsoleCommandLine.h"
#include "engine/CInputEvent.h"
#include "engine/CRGBColour.h"
class CFontBank;

// FableWin complete TPI record 0xBCAD. Retail 009ECD80 gives the STLport
// offsets and the 0x9C-byte allocation at 00414C90 confirms the size.
class CConsole : public CTBaseSingleton<CConsole> {
public:
    CConsole();
    virtual ~CConsole();
    void Initialise(char, EInputKey, const CFontBank*);   // 009ED190.
    void RunScript(const CWideString&);                    // 009EC890.
private:
    unsigned char KeyBindingMap[0xC];           // +0x04 TKeyBindingMap
    unsigned char StringBindingMap[0xC];        // +0x10 TStringBindingMap
    CConsoleCommandLine FrontCommandLine;       // +0x1C
    CConsoleCommandLine BackCommandLine;        // +0x2C
    CCharString HelpBarString;                  // +0x3C
    unsigned char RegisteredConsoleInputs[0xC]; // +0x40 TInputsMap
    unsigned char CommandLineHistory[4];        // +0x4C STLport list
    void* VisibleHistoryStart;       // +0x50 STLport list iterator
    void* VisibleHistoryFinish;      // +0x54 STLport list iterator
    long NumberOfHistoryLinesOnscreen;
    bool HistoryMode;
    void* HistoryPos;                // +0x60 STLport list iterator
    bool MovingIntoHistory;
    bool GuessMode;
    void* GuessPos;                  // +0x68 STLport tree iterator
    char ToggleChar;
    EInputKey ToggleKey;
    const CFontBank* PFont;
    bool Active;
    long ConsoleAlpha;
    CRGBColour HistoryWindowColour;
    CRGBColour ClientWindowColour;
    CRGBColour HelpWindowColour;
    CRGBColour ScrollBarColour;
    CRGBColour TextColour;
    long CursorBlink;
    long CursorBlinkRate;
};
FABLE_STATIC_ASSERT(sizeof(CConsole)==0x9c);
#endif
