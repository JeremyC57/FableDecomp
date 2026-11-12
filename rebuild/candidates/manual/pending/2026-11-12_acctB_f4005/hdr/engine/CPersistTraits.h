#ifndef FABLE_ENGINE_CPERSISTTRAITS_H
#define FABLE_ENGINE_CPERSISTTRAITS_H
#include "engine/CPersistContext.h"
#include "engine/C3DVector.h"
// Original TPI proves every traits class has DefaultVal at zero and these
// exact overloads. CPersistTraitsSLONG is a stateless static method family.
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferIn(CPersistContext&,CStringParser&,long&);
    static long __fastcall TransferIn(CPersistContext& context,CStringParser& parser) {
        long value; TransferIn(context,parser,value); return value;
    }
#ifdef FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
    // Nested opt-in (chunk 1020, row 0x005D8978): Ego_r.pdb / FableWin.pdb ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z
    // (out of line at 0x009E2560: __fastcall context ecx, out edx, the value pushed by reference). Declaration only.
    static void __fastcall TransferOut(CPersistContext&,CCharString&,const long&);
#endif
};
template<class T> class CPersistTraits {
public:
    T DefaultVal;
#ifdef FABLE_RETAIL_CPERSISTTRAITS
    // Retail opt-in (chunk 732, row 0x004B05C0 CQuestManager::TransferBoasts): the user default constructor that value-initialises
    // DefaultVal. Ego_r.pdb names one per instantiation (??0?$CPersistTraits@H@@QAE@XZ, @J@, @K@, @_N@ ..., the same T set as
    // ??$GetDefaultVal@<T>@CPersistContext@@KI..XZ); GetDefaultVal<T>() returns a traits object's DefaultVal, which is why retail
    // builds the CCharString default as a constructed temporary copied into the argument slot (0x004B067C..0x004B0693) and stores
    // a plain 0 for the scalar defaults.
    CPersistTraits() : DefaultVal() {}
#ifdef FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
    ~CPersistTraits() {}
#endif
#endif
    void TransferBinaryIn(CMemoryDataInputStream&,T&);
    void TransferBinaryOut(CDataOutputStream& stream,T value) { stream.WriteSLONG((long)value); }
    void TransferIn(CPersistContext& context,CStringParser& parser,T& value);
#ifdef FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
    // Nested opt-in (chunk 1020, row 0x005D8978): the generic text writer, Ego_r.pdb / FableWin.pdb
    // ?TransferOut@?$CPersistTraits@<T>@@QAEXAAVCPersistContext@@AAVCCharString@@AB<T>@Z (called out of line: 0x005D8185 for unsigned
    // long, 0x005CC6C0 for long); body below the class.
    void TransferOut(CPersistContext& context,CCharString& out,const T& value);
#endif
};
#ifdef FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
// The generic body (retail 0x005D8185 for unsigned long, a size-TU frame; 0x005CC6C0 for long): the value converted to a long and written
// through CPersistTraitsSLONG::TransferOut 0x009E2560. Visible to the includer, which then knows the call never reads the empty traits
// object (TransferMap<long,unsigned long> 0x005D8978 homes both traits objects of its save loop in the dead map argument slot [ebp+0Ch]).
template<class T> inline void CPersistTraits<T>::TransferOut(CPersistContext& context,CCharString& out,const T& value) {
    long longValue=(long)value; CPersistTraitsSLONG::TransferOut(context,out,longValue);
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY
template<> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& is,long& val) {
 long tval=is.ReadSLONG();val=tval;
}
#endif
template<> class CPersistTraits<unsigned __int64> {
public:
    unsigned __int64 DefaultVal;
    unsigned __int64 TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,unsigned __int64&);
    void TransferBinaryOut(CDataOutputStream& stream,const unsigned __int64& val) { stream.WriteUHUGE(val); }
    unsigned __int64 TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,unsigned __int64&);
    void TransferOut(CPersistContext&,CCharString&,const unsigned __int64&);
};
template<class T> inline void CPersistTraits<T>::TransferIn(CPersistContext& context,CStringParser& parser,T& value) {
    long result; CPersistTraitsSLONG::TransferIn(context,parser,result); value=(T)result;
}
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
// Retail opt-in (chunk 1020, row 0x0067E290 CDegradableDef::Transfer): CPersistTraits<bool> is an explicit class specialisation in
// retail -- Ego_r.pdb / FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@_N@@QAEXAAVCDataOutputStream@@AB_N@Z takes `const bool&` where
// the generic member takes T by value (the scalar instances @C@ @E@ @H@ @J@ @K@ and every enum take the value). The inline save
// reads the bool after the stream (`mov ecx,[esi+28h] ; xor eax,eax ; mov al,[ebx] ; push eax ; call WriteEBOOL`, 0x0067E32B).
// Supersedes FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT (do not define both).
template<> class CPersistTraits<bool> {
public:
    bool DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream&,bool&);     // 0x00403EB0
    void TransferBinaryOut(CDataOutputStream& stream,const bool& value) { stream.WriteEBOOL(value); }
    void TransferIn(CPersistContext&,CStringParser&,bool&);
};
#else
template<> void CPersistTraits<bool>::TransferIn(CPersistContext&,CStringParser&,bool&);
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT
// Retail opt-in (chunk 1018, row 0x0067E290): FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@_N@@QAEXAAVCDataOutputStream@@AB_N@Z (the PDB
// takes `const bool&`; this header's generic member takes T by value, which the specialisation must follow -- header debt), inline
// in retail: CDataOutputStream::WriteEBOOL 0x00993E30 (`xor eax,eax ; mov al,[ebx] ; push eax ; call`, 0x0067E2B7).
template<> inline void CPersistTraits<bool>::TransferBinaryOut(CDataOutputStream& stream,bool value) { stream.WriteEBOOL(value); }
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
// Retail opt-in (chunk 1018, row 0x00632B90): the generic template body of ?TransferBinaryIn@?$CPersistTraits@<T>@@QAEXAAVCMemoryDataInputStream@@AAT@Z
// (the landed CPersistTraits<unsigned long> instance 0x0040F8A0: a local filled by the inline Read, then copied to the output). Visible
// to the includer so VC7.1 knows the call reads nothing through the traits `this`: GFSerialiseMapBinaryIn<CCharString,unsigned long>
// 0x00632B90 still calls 0x0040F8A0 out of line but homes the empty key traits on the value traits' frame word (`lea ecx,[esp+14h]`
// for both). Needs FABLE_RETAIL_CDATAINPUTSTREAM.
#include "engine/CMemoryDataInputStream.h"
template<class T> void CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream& is,T& val) {
    T tval;
    is.Read(&tval,sizeof(T));
    val=tval;
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_BINARYIN
// Retail opt-in (chunk 1018, row 0x00632B90): Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCMemoryDataInputStream@@AAVCCharString@@@Z,
// inline in retail: the virtual CDataInputStream::ReadNullTerminatedString (slot 6, `call [edx+18h]`, hidden result) assigned with
// CCharString::operator= 0x0099EFB0 and the temporary destroyed (0x00632C22..0x00632C3C). Needs FABLE_RETAIL_CCHARSTRING.
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
template<> inline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream& is,CCharString& val) {
    val=is.ReadNullTerminatedString();
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_C2DVECTOR_BINARYIN
// Retail opt-in (chunk 1018, row 0x00475DFA): FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@VC2DVector@@@@QAEXAAVCMemoryDataInputStream@@AAVC2DVector@@@Z,
// inline in retail: GFSerialiseVectorBinaryIn<C2DVector> calls CDataInputStream::Read2DVector 0x00995030 into its frame and copies the two
// words into the element (0x00475E6B..0x00475E83). Needs FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR and FABLE_RETAIL_C2DVECTOR.
#include "engine/C2DVector.h"
#include "engine/CMemoryDataInputStream.h"
template<> inline void CPersistTraits<C2DVector>::TransferBinaryIn(CMemoryDataInputStream& is,C2DVector& val) {
    val=is.Read2DVector();
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS
// Retail opt-in (chunk 4004, row 0x00F2D6E0 CScriptDef::Transfer): CPersistTraits<float> as the explicit class specialisation it is
// in retail -- Ego_r.pdb / FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@M@@QAEXAAVCDataOutputStream@@ABM@Z takes `const float&`
// where the generic member takes T by value (like the bool specialisation below), and both PDBs name its user constructor and
// destructor (??0?$CPersistTraits@M@@QAE@XZ / ??1?$CPersistTraits@M@@QAE@XZ) and the two-overload TransferIn / TransferBinaryIn
// family. Member DefaultVal +0 (Ego_r.pdb, float). FITTED, NOT PROVEN: the `const` on DefaultVal. c2's budget rule refuses
// GetDefaultVal<float> at a share of 48 where it accepts GetDefaultVal<long> (c1 size word 48) -- 0x00F30E76 / 0x00F30EB5 /
// 0x00F30ED6 call 0x0040E250 while 0x00F30E97 / 0x00F30EF7 store an inlined 0 -- so the float instance's size word is at least 49;
// a const-qualified member is the one class shape measured to give that (49; non-const, a base class, out-of-class ctor / dtor
// definitions and a virtual destructor all give 48; tools/w6.sh probes w8 / w10). The PDB layout dump carries no qualifiers.
// Do not combine with FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY (a member specialisation of the generic class).
#ifdef FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#error FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS and FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY are exclusive
#endif
template<> class CPersistTraits<float> {
public:
    CPersistTraits() : DefaultVal(0.0f) {}
    ~CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream&,float&);
    float TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryOut(CDataOutputStream&,const float&);
    void TransferIn(CPersistContext&,CStringParser&,float&);
    float TransferIn(CPersistContext&,CStringParser&);
    void TransferOut(CPersistContext&,CCharString&,const float&);
    const float DefaultVal;
};
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
// Retail opt-in (chunk 1020, row 0x009E81E0 CPersistTraits<C2DVector>::TransferIn): the body of Ego_r.pdb / FableWin.pdb
// ?TransferIn@?$CPersistTraits@M@@QAEXAAVCPersistContext@@AAVCStringParser@@AAM@Z (retail 0x0040F7E0, landed): an unquoted word (item
// type PARSED_ITEM_STRING) is a symbol name looked up through CSymbolMap::FindSymbol 0x009E5170 ("Unable to find symbol " 0x0122DFE8 +
// name reported through PersistError 0x009BAA90), anything else is ReadAsFloat 0x009BA650. Retail calls it out of line; with the body
// visible (noinline under MSC) VC7.1 knows the call leaves the empty traits object alone and shares its frame word with the read value
// (0x009E8250..0x009E8257: `lea ecx,[esp+18h] ; push ecx ; ... ; lea ecx,[esp+24h]`). Needs FABLE_RETAIL_CCHARSTRING,
// FABLE_RETAIL_CSTRINGPARSER, FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP and FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL.
#include "fable_string.h"
#include "engine/CParsedItem.h"
#include "engine/CStringParser.h"
#include "engine/CSymbolMap.h"
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
void CPersistTraits<float>::TransferIn(CPersistContext& context,CStringParser& parser,float& value) {
    if (parser.PeekNextItemType()==PARSED_ITEM_STRING) {
        CCharString name=parser.ReadAsString();
        long symbolValue;
        if (!context.PeekPSymbolMap()->FindSymbol(name,symbolValue))
            context.PersistError(parser,"Unable to find symbol "+name);
        value=(float)symbolValue;
    }
    else
        value=parser.ReadAsFloat();
}
#endif
class CThing;
class COpinionDeed;
template<class T> class CIntelligentPointer;
template<> class CPersistTraits<COpinionDeed*> {
public:
    COpinionDeed* DefaultVal;
#ifdef FABLE_RETAIL_CPERSISTTRAITS_PAIR
    // Retail opt-in (chunk 1018, row 0x006C9350): Ego_r.pdb ??0?$CPersistTraits@PAVCOpinionDeed@@@@QAE@XZ, inline: the pair reader stores
    // DefaultVal 0 into its frame word before the first symbol read (0x006C9360 `mov dword ptr [esp+14h],0`).
    CPersistTraits() : DefaultVal(0) { }
#endif
    void TransferIn(CPersistContext&,CStringParser&,COpinionDeed*&);
};
template<> class CPersistTraits<C3DVector> {
public:
    C3DVector DefaultVal;
    CPersistTraits() : DefaultVal(0.0f,0.0f,0.0f) { }
    void TransferIn(CPersistContext&,CStringParser&,C3DVector&);
};
#ifdef FABLE_RETAIL_CPERSISTTRAITS_PAIR
// Retail opt-in (chunk 1018, row 0x006C9350): the partial specialisation behind Ego_r.pdb / FableWin.pdb
// ?TransferIn@?$CPersistTraits@U?$pair@<A><B>@std@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAU?$pair@<A><B>@std@@@Z: a text pair
// "(first, second)" -- the opening "(" symbol, the first through CPersistTraits<A>::TransferIn, the "," symbol, the second through
// CPersistTraits<B>::TransferIn, then the closing ")" symbol read and compared (the result unused). Needs FABLE_RETAIL_CSTRINGPARSER.
#include "engine/CStringParser.h"
#include "stlport/fable_containers.h"
template<class A,class B> class CPersistTraits<std::pair<A,B> > {
public:
    std::pair<A,B> DefaultVal;
    void TransferIn(CPersistContext& context,CStringParser& parser,std::pair<A,B>& value) {
        CPersistTraits<A> firstTraits;
        CPersistTraits<B> secondTraits;
        if (parser.ReadAsSymbol()=="(") {
            firstTraits.TransferIn(context,parser,value.first);
            if (parser.ReadAsSymbol()==",") {
                secondTraits.TransferIn(context,parser,value.second);
                bool closed=parser.ReadAsSymbol()==")";
                (void)closed;
            }
        }
    }
};
#endif
#ifdef FABLE_RETAIL_GFSERIALISEMAPBINARYIN
// Retail opt-in (chunk 1018): the shared body of the global template ??$GFSerialiseMapBinaryIn@<V><K><C>@@YIXAAVCMemoryDataInputStream@@AAV?$map@...@std@@@Z
// (Ego_r.pdb / FableWin.pdb, __fastcall: stream in ecx, map in edx, plain `ret`): the key and value traits, clear(), the count
// through the inline CDataInputStream::ReadSLONG, then per entry a value-initialised pair<K,V> read through both traits and
// inserted (converted to pair<const K,V>; STLport map::insert = _Rb_tree::insert_unique). Instances reproduced with it:
// 0x00632B90 (<CCharString,unsigned long>). Needs the retail stream models and stlport/fable_map.h; the includer instantiates it.
#if !defined(FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAINPUTSTREAM)
#error FABLE_RETAIL_GFSERIALISEMAPBINARYIN needs the retail stream models
#endif
#include "engine/CMemoryDataInputStream.h"
#include "stlport/fable_map.h"
template<class V,class K,class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream& is,std::map<K,V,C,std::allocator<std::pair<const K,V> > >& m)
{
    CPersistTraits<K> ktraits;
    CPersistTraits<V> vtraits;
    m.clear();
    long count=is.ReadSLONG();
    for (long i=0;i<count;++i) {
        std::pair<K,V> entry;
        ktraits.TransferBinaryIn(is,entry.first);
        vtraits.TransferBinaryIn(is,entry.second);
        m.insert(entry);
    }
}
#endif
#ifdef FABLE_RETAIL_GFSERIALISEVECTORBINARYIN
// Retail opt-in (chunk 1018): the shared body of the global template ??$GFSerialiseVectorBinaryIn@<T>@@YIXAAVCMemoryDataInputStream@@AAV?$vector@<T>...@std@@@Z
// (Ego_r.pdb / FableWin.pdb, __fastcall: stream in ecx, vector in edx, plain `ret`): the count through the inline
// CDataInputStream::ReadSLONG (read into its frame word), STLport's inline resize(n) = resize(n, _Tp()), then
// CPersistTraits<T>::TransferBinaryIn on every element (inline or out of line by T). Instances reproduced with it:
// 0x00475DFA (C2DVector). Needs the retail stream models; the includer instantiates it explicitly.
#if !defined(FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAINPUTSTREAM)
#error FABLE_RETAIL_GFSERIALISEVECTORBINARYIN needs the retail stream models
#endif
#include "engine/CMemoryDataInputStream.h"
#include "stlport/fable_containers.h"
template<class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream& is,std::vector<T,std::allocator<T> >& vec)
{
    CPersistTraits<T> traits;
    long count=is.ReadSLONG();
    vec.resize(count);
    for (long i=0;i<count;++i)
        traits.TransferBinaryIn(is,vec[i]);
}
#endif
#endif
