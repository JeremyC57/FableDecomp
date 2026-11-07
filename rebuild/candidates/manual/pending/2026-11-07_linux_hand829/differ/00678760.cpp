// CPersistTraits<std::list<CReactionMatch*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::list<CReactionMatch*>& value) -- text load of one ".Add(type, Shape(...))" opinion-reaction match
// Grade: DIFFER(3554v3579) -- 1215 md lines, 25 bytes short: retail keeps the name's PStringData in esi across every CBasicString::operator== call and threads the name-compare chain (null path: five repe cmpsb with a direct je to each shape), ours reloads PStringData per compare and keeps one shared null test; everything after the first compare shifts
// Next idea: try the five shape tests as a classifier function returning an index switched on (retail threading looks like a switch-free chain on a cached pointer), or the CCharString operator== spelled with a local const char* copy of PStringData taken once before the if/else chain
// Leaf: ?TransferIn@?$CPersistTraits@V?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00678760, 3579 bytes, `ret 0Ch`.
#include <string.h>
#include <stdlib.h>
#pragma intrinsic(strcmp)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    bool operator==(const char* text) const;           // 0x004115A0
};
class CCharString { public:
    CBasicString<char>* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        return PStringData ? *PStringData == text : strcmp(text, "") == 0;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CStringParser { public:
    long PeekNextItemType();                                     // 0x009BA900
    CCharString ReadAsString();                                  // 0x009BA6F0
    float ReadAsFloat();                                         // 0x009BA650
    bool SkipPastNextItemIfIs(const CCharString& item);          // 0x009B9E00
    CCharString ReadAsSymbol();                                  // 0x009BA790
    long SkipWhitespace();                                       // 0x009B9C60
    CCharString ReadAsStringUntilString(const CCharString& end); // 0x009BA330
};
class CSymbolMap { public:
    bool Lookup(const CCharString& name, long* value) const;     // 0x009E5170
};
extern const char g_msgUnknownSymbol_0122dfe8[];                 // 0x0122DFE8
class CPersistContext { public:
    void*       PDefinitionManager;                              // +0x00
    CSymbolMap* PSymbolMap;                                      // +0x04
    void PersistError(const CStringParser& parser, const CCharString& message) const; // 0x009BAA90
};
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

enum EOpinionReactionType { OPINION_REACTION_NONE = 0 };
enum EOpinion { OPINION_NONE = 0 };
template <class T> class CArray { public:
    T* Data; long Count; long Capacity;
};
class COpinionSet { public:
    CArray<float> OpinionValues;
    COpinionSet();                                     // 0x004538EB
    ~COpinionSet() { if (OpinionValues.Data) free(OpinionValues.Data); }
    float& operator[](long index) { return OpinionValues.Data[index]; }
};
class CReactionMatch { public: void* __vftable; EOpinionReactionType Type; };
class CReactionMatch_Sphere : public CReactionMatch { public:
    unsigned char _pad[0x10];
    CReactionMatch_Sphere(EOpinionReactionType type, const COpinionSet& centre, float radius);   // 0x0067C2F0
};
class CReactionMatch_MoralityRenownBlob : public CReactionMatch { public:
    unsigned char _pad[0x2c];
    CReactionMatch_MoralityRenownBlob(EOpinionReactionType type, float, float, float, float, float, float, float);   // 0x00677760
};
class CReactionMatch_AsymmetricPyramid : public CReactionMatch { public:
    unsigned char _pad[0x20];
    CReactionMatch_AsymmetricPyramid(EOpinionReactionType type, EOpinion, EOpinion, float, float, float, float, float, float);   // 0x00677620
};
class CReactionMatch_SecondaryAxisMRShifted : public CReactionMatch { public:
    unsigned char _pad[0x1c];
    CReactionMatch_SecondaryAxisMRShifted(EOpinionReactionType type, EOpinion, float, float, float, float, float, float);   // 0x00677540
};
class CReactionMatch_SecondaryAxisCentredMRShifted : public CReactionMatch { public:
    unsigned char _pad[0x1c];
    CReactionMatch_SecondaryAxisCentredMRShifted(EOpinionReactionType type, EOpinion, float, float, float, float, float, float);   // 0x006775B0
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class list { public:
    void* _M_node;
    void push_back(const T& value);                    // 0x0067BCC0
};
}
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<float> {
    float DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, float& value);     // 0x0040F7E0
};
template <> struct CPersistTraits<EOpinion> {
    EOpinion DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, EOpinion& value);  // 0x00679910
};
// The two element traits with their real bodies in the TU (GOTCHAS 24l recipe: auto_inline(off) + noinline) so VC7.1 homes
// the never-initialised traits `this` on the value slot as retail does. Not landed from here.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<float>::TransferIn(CPersistContext& context, CStringParser& parser, float& value)
{
    if (parser.PeekNextItemType() == 2) {
        CCharString name = parser.ReadAsString();
        long symbol;
        if (!context.PSymbolMap->Lookup(name, &symbol))
            context.PersistError(parser, g_msgUnknownSymbol_0122dfe8 + name);
        value = (float)symbol;
    }
    else
        value = parser.ReadAsFloat();
}
__declspec(noinline) void CPersistTraits<EOpinion>::TransferIn(CPersistContext& context, CStringParser& parser, EOpinion& value)
{
    long key;
    ::TransferIn(&context, &parser, &key);
    value = (EOpinion)key;
}
#pragma auto_inline(on)
template <> struct CPersistTraits<std::list<CReactionMatch*> > {
    void TransferIn(CPersistContext& context, CStringParser& parser, std::list<CReactionMatch*>& value);
};
extern const char g_msgExpectedAdd_0125b75c[];          // 0x0125B75C
extern const char g_msgUnknownReactionMatch_0125b840[]; // 0x0125B840

void CPersistTraits<std::list<CReactionMatch*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::list<CReactionMatch*>& value)
{
    if (parser.SkipPastNextItemIfIs(".Add")) {
        if (parser.ReadAsSymbol() == "(") {
            long type;
            ::TransferIn(&context, &parser, &type);
            if (parser.ReadAsSymbol() == ",") {
                parser.SkipWhitespace();
                CCharString name = parser.ReadAsStringUntilString("(");
                if (parser.ReadAsSymbol() == "(") {
                    if (name == "Sphere") {
                        COpinionSet centre;
                        for (unsigned long i = 0; i < 5; ++i) {
                            parser.SkipWhitespace();
                            float v;
                            CPersistTraits<float>().TransferIn(context, parser, v);
                            centre[i] = v;
                            if (!(parser.ReadAsSymbol() == ","))
                                return;
                        }
                        parser.SkipWhitespace();
                        float radius;
                        CPersistTraits<float>().TransferIn(context, parser, radius);
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")"))
                            return;
                        value.push_back(new CReactionMatch_Sphere((EOpinionReactionType)type, centre, radius));
                    }
                    else if (name == "MRBlob") {
                        float f0, f1, f2, f3, f4, f5, f6;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f0);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f1);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f2);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f3);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f4);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f5);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f6);
                        parser.SkipWhitespace();
                        value.push_back(new CReactionMatch_MoralityRenownBlob((EOpinionReactionType)type, f0, f1, f2, f3, f4, f5, f6));
                        if (!(parser.ReadAsSymbol() == ")"))
                            return;
                    }
                    else if (name == "Pyramid") {
                        EOpinion axis1, axis2;
                        float f[6];
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis1);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[0]);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[1]);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[2]);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis2);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[3]);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[4]);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<float>().TransferIn(context, parser, f[5]);
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_AsymmetricPyramid((EOpinionReactionType)type, axis1, axis2, f[0], f[1], f[2], f[3], f[4], f[5]));
                    }
                    else if (name == "Secondary") {
                        EOpinion axis;
                        float f[6];
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis);
                        for (unsigned long i = 0; i < 6; ++i) {
                            if (!(parser.ReadAsSymbol() == ",")) return;
                            parser.SkipWhitespace();
                            CPersistTraits<float>().TransferIn(context, parser, f[i]);
                        }
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_SecondaryAxisMRShifted((EOpinionReactionType)type, axis, f[0], f[1], f[2], f[3], f[4], f[5]));
                    }
                    else if (name == "SecondaryCentred") {
                        EOpinion axis;
                        float f[6];
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis);
                        for (unsigned long i = 0; i < 6; ++i) {
                            if (!(parser.ReadAsSymbol() == ",")) return;
                            parser.SkipWhitespace();
                            CPersistTraits<float>().TransferIn(context, parser, f[i]);
                        }
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_SecondaryAxisCentredMRShifted((EOpinionReactionType)type, axis, f[0], f[1], f[2], f[3], f[4], f[5]));
                    }
                    else {
                        context.PersistError(parser, g_msgUnknownReactionMatch_0125b840 + name + "'!");
                        return;
                    }
                    parser.SkipWhitespace();
                    parser.ReadAsSymbol() == ")";
                }
            }
        }
    }
    else
        context.PersistError(parser, CCharString(g_msgExpectedAdd_0125b75c));
}
