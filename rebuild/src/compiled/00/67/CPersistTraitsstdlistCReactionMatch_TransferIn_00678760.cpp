// CPersistTraits<std::list<CReactionMatch*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::list<CReactionMatch*>& value) -- text load of one opinion-reaction match, ".Add(type, Shape(...))": the reaction type, the shape name up to "(", then the shape's values; a new CReactionMatch_<shape> is appended to the list
// Leaf: ?TransferIn@?$CPersistTraits@V?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV?$list@PAVCReactionMatch@@V?$allocator@PAVCReactionMatch@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00678760, 3579 bytes, `ret 0Ch` (traits `this` unused).
// Identity: Ego_r.pdb 0x006AA810 / FableWin.pdb, the exact leaf above (the generic CPersistTraits<T>::TransferIn of engine/CPersistTraits.h,
//   specialised for the opinion-reaction list; same TU as the landed TransferBinaryIn 0x006782B0 / TransferBinaryOut 0x00678250).
// Shapes (strings 0x0125B894 / 0x0125B88C / 0x0125B884 / 0x0125B878 / 0x0125B864): Sphere = COpinionSet of five opinions + radius
//   (ctor 0x0067C2F0, header inline, called), MRBlob = seven floats (CReactionMatch_MoralityRenownBlob 0x00677760), Pyramid = EOpinion,
//   3 floats, EOpinion, 3 floats (0x00677620), Secondary / SecondaryCentred = EOpinion + six floats (0x00677540 / 0x006775B0); any other
//   name -> PersistError("was not expecting matchtype '" + name + "'!"). A missing ".Add" reports the (retail) TOpinionTransientOffsetList
//   message 0x0125B75C. Callees: CPersistTraits<float>::TransferIn 0x0040F7E0, the generic enum reader 0x00679910 (EOpinion, called;
//   the reaction type read is the inline CPersistTraitsSLONG call), list push_back 0x0067BCC0 (called: FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE),
//   COpinionSet() 0x004538EB (called; visible body, header opt-in FABLE_RETAIL_COPINIONSET_CTOR_INDEX).
// EVIDENCED: Pyramid reads into six scalars (retail frame: yMax in the dead `parser` argument slot, the rest packed between the axes,
//   0x006791DF..0x0067920B), the Secondary arms through a float array in a loop (`add esi,4`, 0x00679427); the Sphere loop's unsigned
//   index (`jb`, 0x00678A3C) and float temporary (0x006789DA); the final `)` compare evaluated and dropped (0x006794F1..0x00679526).
// FITTED: the float traits as one named function-scope object (as landed sibling 0x006C9490 names its traits objects): with the
//   temporary spelling the Sphere loop restores edi before esi after the inline strcmp (0x00678A06); EOpinion / reaction-type traits
//   stay temporaries (naming them too flips it back).
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_COPINIONSET_CTOR_INDEX
#define FABLE_RETAIL_CREACTIONMATCH_SPHERE_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_ASYMMETRICPYRAMID_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_MRPYRAMID_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_SECONDARYAXISMRSHIFTED_CLASS
#define FABLE_RETAIL_CREACTIONMATCH_SECONDARYAXISCENTREDMRSHIFTED_CLASS
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CReactionMatch.h"
#include "engine/CReactionMatch_Sphere.h"
#include "engine/CReactionMatch_MoralityRenownBlob.h"
#include "engine/CReactionMatch_AsymmetricPyramid.h"
#include "engine/CReactionMatch_SecondaryAxisMRShifted.h"
#include "engine/CReactionMatch_SecondaryAxisCentredMRShifted.h"

template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

template<> void CPersistTraits<std::list<CReactionMatch*> >::TransferIn(CPersistContext& context, CStringParser& parser, std::list<CReactionMatch*>& value)
{
    CPersistTraits<float> floatTraits;
    if (parser.SkipPastNextItemIfIs(".Add")) {
        if (parser.ReadAsSymbol() == "(") {
            EOpinionReactionType type;
            CPersistTraits<EOpinionReactionType>().TransferIn(context, parser, type);
            if (parser.ReadAsSymbol() == ",") {
                parser.SkipWhitespace();
                CCharString name = parser.ReadAsStringUntilString("(");
                if (parser.ReadAsSymbol() == "(") {
                    if (name == "Sphere") {
                        COpinionSet centre;
                        for (unsigned long i = 0; i < 5; ++i) {
                            parser.SkipWhitespace();
                            float opinion;
                            floatTraits.TransferIn(context, parser, opinion);
                            centre[i] = opinion;
                            if (!(parser.ReadAsSymbol() == ","))
                                return;
                        }
                        parser.SkipWhitespace();
                        float radius;
                        floatTraits.TransferIn(context, parser, radius);
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")"))
                            return;
                        value.push_back(new CReactionMatch_Sphere(type, centre, radius));
                    }
                    else if (name == "MRBlob") {
                        float yMin, yMax, xMin, xMax, scarinessShift, agreeablenessShift, attractivenessShift;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, yMin);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, yMax);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, xMin);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, xMax);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, scarinessShift);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, agreeablenessShift);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, attractivenessShift);
                        parser.SkipWhitespace();
                        value.push_back(new CReactionMatch_MoralityRenownBlob(type, yMin, yMax, xMin, xMax, scarinessShift, agreeablenessShift, attractivenessShift));
                        if (!(parser.ReadAsSymbol() == ")"))
                            return;
                    }
                    else if (name == "Pyramid") {
                        EOpinion xAxis, yAxis;
                        float xMin, xCentre, xMax, yMin, yCentre, yMax;
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, xAxis);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, xMin);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, xCentre);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, xMax);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, yAxis);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, yMin);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, yCentre);
                        if (!(parser.ReadAsSymbol() == ",")) return;
                        parser.SkipWhitespace();
                        floatTraits.TransferIn(context, parser, yMax);
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_AsymmetricPyramid(type, xAxis, yAxis, xMin, xCentre, xMax, yMin, yCentre, yMax));
                    }
                    else if (name == "Secondary") {
                        EOpinion axis;
                        float values[6];
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis);
                        for (unsigned long i = 0; i < 6; ++i) {
                            if (!(parser.ReadAsSymbol() == ",")) return;
                            parser.SkipWhitespace();
                            floatTraits.TransferIn(context, parser, values[i]);
                        }
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_SecondaryAxisMRShifted(type, axis, values[0], values[1], values[2], values[3], values[4], values[5]));
                    }
                    else if (name == "SecondaryCentred") {
                        EOpinion axis;
                        float values[6];
                        parser.SkipWhitespace();
                        CPersistTraits<EOpinion>().TransferIn(context, parser, axis);
                        for (unsigned long i = 0; i < 6; ++i) {
                            if (!(parser.ReadAsSymbol() == ",")) return;
                            parser.SkipWhitespace();
                            floatTraits.TransferIn(context, parser, values[i]);
                        }
                        parser.SkipWhitespace();
                        if (!(parser.ReadAsSymbol() == ")")) return;
                        value.push_back(new CReactionMatch_SecondaryAxisCentredMRShifted(type, axis, values[0], values[1], values[2], values[3], values[4], values[5]));
                    }
                    else {
                        context.PersistError(parser, "was not expecting matchtype '" + name + "'!");
                        return;
                    }
                    parser.SkipWhitespace();
                    if (!(parser.ReadAsSymbol() == ")")) return;
                }
            }
        }
    }
    else
        context.PersistError(parser, "Unknown command for TOpinionTransientOffsetList");
}