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
};
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
template<> void CPersistTraits<bool>::TransferIn(CPersistContext&,CStringParser&,bool&);
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
