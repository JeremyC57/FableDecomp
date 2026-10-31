// CPersistTraits<std::pair<long, COpinionDeed*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::pair<long, COpinionDeed*>& value) -- parse a "(handle,deed)" entry of the opinion-deed list from a text save
// Grade: RELOCATION_MATCH NAME REVIEW (retry wave 2 sec2b): the "," test goes through a second forceinline CCharString compare whose non-null arm calls the out-of-line CBasicString<char>::operator== 0x004115A0, while "(" / ")" use the CCharString::operator== whose arm inlines Compare -- reproducing retail's per-call inlining split (an inliner budget effect of the retail TU). The two helper names (CBasicString::Equals, CCharString::IsEqualSlow) are invented: not in either PDB (CCharString::Equals there takes a CCharString&). Was 314v312.
// Leaf: ?TransferIn@?$CPersistTraits@U?$pair@JPAVCOpinionDeed@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@JPAVCOpinionDeed@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9350, 312 bytes, thiscall `ret 0Ch` (the traits `this` is unused).
// Identity (proven): the only caller is CPersistContext_TransferableVector<std::pair<long, COpinionDeed*> >::TransferIn
//   0x006C9310 (vtable 0x0125E9DC slot 3, landed in hand955), which calls CPersistTraits<T>::TransferIn on its frame traits
//   object; Ego_r.pdb (donor 0x00700120) and FableWin.pdb hold this exact symbol. The second element goes to
//   CPersistTraits<COpinionDeed*>::TransferIn 0x006C9490 (it news a 0x28-byte COpinionDeed and stores it through the
//   reference), Ego_r donor 0x006FFB90, the next function in both builds. The catalog label
//   "CPersistTraits<CAtmosBankEntry>::TransferIn" is propagated.
// Callees: CStringParser::ReadAsSymbol() @ 0x009BA790 (x3), CBasicString<char>::Compare(const char*, const char*) @ 0x00411570
//   (protected static __fastcall; the inlined operator== at the "(" and ")" tests), CBasicString<char>::operator==(const char*)
//   const @ 0x004115A0 (the "," test), ~CCharString @ 0x0099EAE0, the fastcall long reader 0x009E2590,
//   CPersistTraits<COpinionDeed*>::TransferIn @ 0x006C9490.
// Shape: the frame traits object for the second element (DefaultVal = 0) is built first and passed as `this`; the first
//   element reads through the inline by-value CPersistTraits<long> reader into the dead parser slot.
#include <string.h>
#pragma intrinsic(strcmp)
template <class T> class CBasicString { public:
    T* Data;
    bool operator==(const T* text) const;   // 0x004115A0
    __forceinline bool Equals(const T* text) const { return Compare(Data, text) == 0; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570
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
        return PStringData->Equals(text);
    }
    __forceinline bool IsEqualSlow(const char* text) const
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

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
};
}
class COpinionDeed;

template <class T> class CPersistTraits {
public:
    T DefaultVal;
    CPersistTraits() : DefaultVal(T()) { }
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};
template <> class CPersistTraits<long> {
public:
    long DefaultVal;
    CPersistTraits() : DefaultVal(0) { }
    long TransferIn(CPersistContext& context, CStringParser& parser)
    {
        long value;
        ::TransferIn(&context, &parser, &value);
        return value;
    }
};
template <class A, class B> class CPersistTraits<std::pair<A, B> > {
public:
    std::pair<A, B> DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, std::pair<A, B>& value);
};

template <class A, class B>
void CPersistTraits<std::pair<A, B> >::TransferIn(CPersistContext& context, CStringParser& parser, std::pair<A, B>& value)
{
    CPersistTraits<A> first_traits;
    CPersistTraits<B> second_traits;
    if (parser.ReadAsSymbol() == "(") {
        value.first = first_traits.TransferIn(context, parser);
        if (parser.ReadAsSymbol().IsEqualSlow(",")) {
            second_traits.TransferIn(context, parser, value.second);
            if (parser.ReadAsSymbol() == ")") { }
        }
    }
}
template void CPersistTraits<std::pair<long, COpinionDeed*> >::TransferIn(CPersistContext&, CStringParser&, std::pair<long, COpinionDeed*>&);
