// CPersistTraits<CCrimeWaitingForBribe>::TransferIn(CPersistContext& context, CStringParser& parser, CCrimeWaitingForBribe& value) -- parse a "(DeedHandle,Type)" bribe record from a text save
// Leaf: ?TransferIn@?$CPersistTraits@VCCrimeWaitingForBribe@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCrimeWaitingForBribe@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B47B0, 297 bytes, thiscall `ret 0Ch` (the traits `this` is unused).
// Identity (proven): the only caller is CPersistContext_TransferableVector<CCrimeWaitingForBribe>::TransferIn 0x007B4770
//   (vtable 0x0126D7F8 slot 3, landed in hand955), which calls CPersistTraits<T>::TransferIn on its frame traits object;
//   Ego_r.pdb / FableWin.pdb both hold this exact symbol (Ego_r donor 0x007B5070). The catalog label
//   "CPersistTraits<CAtmosBankEntry>::TransferIn" is propagated.
// Callees: CStringParser::ReadAsSymbol() @ 0x009BA790 (x3, hidden CCharString result), CBasicString<char>::operator==
//   (const char*) const @ 0x004115A0, ~CCharString @ 0x0099EAE0, the fastcall long reader
//   TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (landed spelling). Strings: "(" 0x0122E028,
//   "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E.
// Layout: Ego_r.pdb CCrimeWaitingForBribe (8 bytes): DeedHandle +0, Type (EOpinionDeedType) +4.
// Shape: the landed CPersistTraits<pair<EReactionSpeechType,long>>::TransferIn 0x00755F40 model -- inline CCharString
//   operator== (null data compares the literal with "" through the strcmp intrinsic), each long read into the dead
//   argument slots through an inline by-value reader, the closing ")" read and compared with the result discarded.
#include <string.h>
#pragma intrinsic(strcmp)
template <class T> class CBasicString { public:
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharString { public:
    CBasicString<char>* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
};
class CStringParser { public:
    CCharString ReadAsSymbol();                         // 0x009BA790
};
class CPersistContext;
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
__forceinline long TransferInLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    TransferIn(&context, &parser, &value);
    return value;
}

enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0 };
class CCrimeWaitingForBribe {                           // Ego_r.pdb, 8 bytes
public:
    long DeedHandle;                                    // +0x00
    EOpinionDeedType Type;                              // +0x04
};

template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};

template <>
void CPersistTraits<CCrimeWaitingForBribe>::TransferIn(CPersistContext& context, CStringParser& parser, CCrimeWaitingForBribe& value)
{
    if (parser.ReadAsSymbol() == "(") {
        value.DeedHandle = TransferInLong(context, parser);
        if (parser.ReadAsSymbol() == ",") {
            value.Type = (EOpinionDeedType)TransferInLong(context, parser);
            if (parser.ReadAsSymbol() == ")") { }
        }
    }
}