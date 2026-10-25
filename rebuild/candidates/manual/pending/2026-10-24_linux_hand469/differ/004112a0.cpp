// CPersistTraits<CActionInputControl>::TransferIn(CPersistContext* context, CStringParser* parser, CActionInputControl* value) -- text load of a key binding "CActionInputControl(action,controller,key[,(x,y)])": the key field chosen by the controller type, the direction optional
// Grade: DIFFER(584v705) -- VC7.1 inline budget: retail expands CCharString::operator== + CBasicString<char>::operator== (down to a Compare call) at the first three symbol tests and calls 0x004115A0 for the closing ")"; with Compare declared-only and both operators plain inline ours expands only the first two and calls CCharString::operator== out of line from the third on (which also shifts the register choice for context/value)
// Leaf: ?TransferIn@?$CPersistTraits@VCActionInputControl@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004112A0, 720 bytes, `ret 0Ch` (traits `this` unused).
// Identity: the strings name it ("CActionInputControl" 0x0122E02C, "Unknown command for CActionInputControl" 0x0122E094,
//   "Unknown controller type specified in CActionInputControl" 0x0122E0BC); it is the reader of the landed writer
//   0x0040FA20 (same field switch: 1 Xbox button +0xC, 2 keyboard key +8, 3 mouse button +0x10, direction +0x14). The
//   catalog label "CPersistTraits<CSimVoice>::TransferIn" is propagated.
// Callees: CStringParser::SkipPastNextItemIfIs(const CCharString&) @ 0x009B9E00, ReadAsSymbol @ 0x009BA790 (hidden
//   CCharString result), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, TransferIn(CPersistContext*,
//   CStringParser*, long*) @ 0x009E2590 (`__fastcall`), CPersistTraits<C2DVector>::TransferIn @ 0x009E81E0 (thiscall on a
//   zero-defaulted traits object), CPersistContext::Error(const CStringParser&, const CCharString&) @ 0x009BAA90, and this
//   TU's CBasicString<char>::Compare @ 0x00411570 / operator==(const char*) @ 0x004115A0. Strings "(" 0x0122E028,
//   "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E.
// Shape: CCharString's inline operator==(const char*) (a null data pointer compares the literal with "" through the
//   strcmp intrinsic) over CBasicString<char>'s inline operator== and Compare, all with their real bodies: VC7.1's inline
//   budget expands the first three symbol tests down to a Compare call and calls operator== out of line for the closing
//   ")" (whose discarded result lands in the dead context slot). The longs come through the inline by-value
//   TransferInLong() helper of the landed 0x00755F40 (homed in the dead parser slot). Speed-optimised (default) TU.
#include <string.h>
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    static long __fastcall Compare(const char* a, const char* b);   // 0x00411570
    bool operator==(const char* text) const { return Compare(PData, text) == 0; }   // 0x004115A0
};
struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
struct CStringParser {
    bool SkipPastNextItemIfIs(const CCharString& item);  // 0x009B9E00
    CCharString ReadAsSymbol();                          // 0x009BA790
};
struct CPersistContext {
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
};
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
__forceinline long TransferInLong(CPersistContext* context, CStringParser* parser)
{
    long value;
    TransferIn(context, parser, &value);
    return value;
}

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() : X(0.0f), Y(0.0f) {}
};
class CActionInputControl {                            // 0x1C bytes
public:
    long      GameAction;                              // +0x00 EGameAction
    long      ControllerType;                          // +0x04 EControllerType
    long      KeyboardKey;                             // +0x08 EInputKey
    long      XboxButton;                              // +0x0C EXboxControllerButton
    long      MouseButton;                             // +0x10 EMouseButtonControl
    C2DVector ControlDirection;                        // +0x14
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<C2DVector> {
    C2DVector DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, C2DVector* value);   // 0x009E81E0
};
template <> struct CPersistTraits<CActionInputControl> {
    void TransferIn(CPersistContext* context, CStringParser* parser, CActionInputControl* value);
};

void CPersistTraits<CActionInputControl>::TransferIn(CPersistContext* context, CStringParser* parser, CActionInputControl* value)
{
    if (parser->SkipPastNextItemIfIs(CCharString("CActionInputControl"))) {
      if (parser->ReadAsSymbol() == "(") {
        value->GameAction = TransferInLong(context, parser);
        if (parser->ReadAsSymbol() == ",") {
            value->ControllerType = TransferInLong(context, parser);
            if (parser->ReadAsSymbol() == ",") {
                switch (value->ControllerType) {
                case 1:
                    value->XboxButton = TransferInLong(context, parser);
                    break;
                case 2:
                    value->KeyboardKey = TransferInLong(context, parser);
                    break;
                case 3:
                    value->MouseButton = TransferInLong(context, parser);
                    break;
                default:
                    context->Error(*parser, CCharString("Unknown controller type specified in CActionInputControl"));
                    return;
                }
                if (parser->SkipPastNextItemIfIs(CCharString(","))) {
                    CPersistTraits<C2DVector> vectorTraits;
                    vectorTraits.TransferIn(context, parser, &value->ControlDirection);
                }
                parser->ReadAsSymbol() == ")";
            }
        }
      }
    } else {
        context->Error(*parser, CCharString("Unknown command for CActionInputControl"));
    }
}
