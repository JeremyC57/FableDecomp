// CPersistTraits<std::pair<EReactionSpeechType, long> >::TransferIn(CPersistContext*, CStringParser*, pair*) -- parse a "(type,value)" voice entry from a text save
// Retail: Fable.exe (TLC, VC7.1) @ 0x00755F40, 227 bytes (+ int3 padding), `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Label: the catalog name "CPersistTraits<CSimVoice>::TransferIn" is propagated; the body parses one two-long record --
//   the pair<EReactionSpeechType, long> entry of CSimVoice's Voices map (the landed CSimVoice::TransferBinaryOut
//   0x006808C0 models it) -- as "(" first "," second ")".
// Callees: CStringParser::ReadAsSymbol() @ 0x009BA790 (x3, hidden CCharString result), CBasicString<char>::operator==
//   (const char*) @ 0x004115A0, ~CCharString @ 0x0099EAE0, TransferIn(CPersistContext*, CStringParser*, long*)
//   @ 0x009E2590 (__fastcall: context in ecx, parser in edx). Strings: "(" 0x0122E028, "," 0x0122E024, "" 0x0122D70E.
// Shape: the symbol compares are CCharString's inline operator== (a null data pointer compares the literal with "" via
//   the strcmp intrinsic, `repe cmpsb ; sete`); each symbol temporary dies before its test. The parsed longs land in
//   the dead parser- and context-argument slots through an inline by-value TransferInLong() helper (two named locals
//   in the body take a fresh frame slot for the second) and are copied into the pair afterwards; the closing ")" is read and
//   discarded. Speed-optimised (default) TU.
#include <string.h>
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharString { public:
    CBasicStringChar* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
class CStringParser { public:
    CCharString ReadAsSymbol();                         // 0x009BA790
};
class CPersistContext;
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
__forceinline long TransferInLong(CPersistContext* context, CStringParser* parser)
{
    long value;
    TransferIn(context, parser, &value);
    return value;
}

enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };
template<class A, class B> struct pair {
    A first;
    B second;
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<pair<EReactionSpeechType, long> > {
    void TransferIn(CPersistContext* context, CStringParser* parser, pair<EReactionSpeechType, long>* value);
};

void CPersistTraits<pair<EReactionSpeechType, long> >::TransferIn(CPersistContext* context, CStringParser* parser, pair<EReactionSpeechType, long>* value)
{
    if (parser->ReadAsSymbol() == "(") {
        value->first = (EReactionSpeechType)TransferInLong(context, parser);
        if (parser->ReadAsSymbol() == ",") {
            value->second = TransferInLong(context, parser);
            parser->ReadAsSymbol();
        }
    }
}