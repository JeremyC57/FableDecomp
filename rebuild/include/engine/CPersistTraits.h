#ifndef FABLE_ENGINE_CPERSISTTRAITS_H
#define FABLE_ENGINE_CPERSISTTRAITS_H
#ifdef FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
// NF136 partial mode (2026-11-20, q5199 / q5220): only CPersistTraitsSLONG (Ego_r.pdb / FableWin.pdb static __fastcall
// ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z = 0x009E2590 and its inline long-returning overload), for
// landed rows that model CCharString / CStringParser / CPersistContext locally and called 0x009E2590 through a free stand-in declaration.
// The generic CPersistTraits<T> family below needs the retail stream / string models, so this mode leaves it out (like
// FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY). Do not combine with a full include of this header in the same TU.
class CPersistContext;
class CStringParser;
class CCharString;
#else
#include "engine/CPersistContext.h"
#include "engine/C3DVector.h"
#endif
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
#ifndef FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
template<class T> class CPersistTraits {
public:
    T DefaultVal;
#ifdef FABLE_RETAIL_CPERSISTTRAITS
    // Retail opt-in (chunk 732, row 0x004B05C0 CQuestManager::TransferBoasts): the user default constructor that value-initialises
    // DefaultVal. Ego_r.pdb names one per instantiation (??0?$CPersistTraits@H@@QAE@XZ, @J@, @K@, @_N@ ..., the same T set as
    // ??$GetDefaultVal@<T>@CPersistContext@@KI..XZ); GetDefaultVal<T>() returns a traits object's DefaultVal, which is why retail
    // builds the CCharString default as a constructed temporary copied into the argument slot (0x004B067C..0x004B0693) and stores
    // a plain 0 for the scalar defaults.
#ifdef FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT
    // Nested opt-in (agentNF54, row 0x0049F4C0, T = the nested enum CWorld::ESaveType): VC7.1 rejects the value-initialising
    // mem-initializer `DefaultVal()` for a class-nested enum T (C2065 on the unqualified enum name, C2064); the copy of a
    // value-initialised T is the same zero (scalar T: same code).
    CPersistTraits() : DefaultVal(T()) {}
#else
    CPersistTraits() : DefaultVal() {}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_EMPTY_DTOR
    ~CPersistTraits() {}
#endif
#endif
    void TransferBinaryIn(CMemoryDataInputStream&,T&);
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BINARYOUT_CONSTREF
    // Opt-in (p25K, row 0x005C0A98 CPersistTraits<NInventory::CItem>::TransferBinaryOut): the primary template's writer takes its value by
    // const reference, as Ego_r.pdb / FableWin.pdb spell every instance (?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z,
    // ...@M@@QAEXAAVCDataOutputStream@@ABM@Z, ...@_N@@QAEXAAVCDataOutputStream@@AB_N@Z), so a class T's member can be specialised.
    // Declaration only: VC7.1 treats an explicit specialisation of a member defined in the class as inline and emits nothing for it.
    void TransferBinaryOut(CDataOutputStream& stream,const T& value);
#else
    void TransferBinaryOut(CDataOutputStream& stream,T value) { stream.WriteSLONG((long)value); }
#endif
    void TransferIn(CPersistContext& context,CStringParser& parser,T& value);
#ifdef FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
    // Nested opt-in (chunk 1020, row 0x005D8978): the generic text writer, Ego_r.pdb / FableWin.pdb
    // ?TransferOut@?$CPersistTraits@<T>@@QAEXAAVCPersistContext@@AAVCCharString@@AB<T>@Z (called out of line: 0x005D8185 for unsigned
    // long, 0x005CC6C0 for long); body below the class.
    void TransferOut(CPersistContext& context,CCharString& out,const T& value);
#endif
};
#ifdef FABLE_RETAIL_CPERSISTTRAITS_VECTOR_DEFAULT_PASS26
// FableWin 01899CF0 constructs DefaultVal with two zero floats. Its vector writer
// takes a reference at 019C73F6. Integral writers in the same PDB take values.
template<> class CPersistTraits<C2DVector> {
public:
    C2DVector DefaultVal;
    CPersistTraits() : DefaultVal(0.0f, 0.0f) {}
    ~CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream& stream, C2DVector& value) { value=stream.Read2DVector(); }
    void TransferBinaryOut(CDataOutputStream& stream, const C2DVector& value) { stream.Write2DVector(value); }
};
#ifdef FABLE_RETAIL_CPERSISTTRAITS_ROUTE_BYTE_READER_PASS26
// Both PDBs name the unsigned-byte reader. FableWin 018BE700 reads a signed long;
// the retail helper at 00405940 only assigns the low byte to the supplied output.
template<> inline void CPersistTraits<unsigned char>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned char& value) {
    value=static_cast<unsigned char>(stream.ReadSLONG());
}
#endif
#endif

#ifdef FABLE_RETAIL_CPERSISTTRAITS_COMBOMULTIPLIERDATA_BINARYOUT
// NF81 opt-in (round 7, q10986 binder, row 0x006DC350 GFSerialiseMapBinaryOut<CComboMultiplierData, long>): the traits of
// CHeroExperienceDef::ComboMultiplierMap's value, as the CWoundMorphs / CExpressionSet specialisations below: DefaultVal and the
// writer Ego_r.pdb 0x0058ACE0 / FableWin.pdb 0x01F0ADD0 ?TransferBinaryOut@?$CPersistTraits@VCComboMultiplierData@@@@QAEXAAVCDataOutputStream@@ABVCComboMultiplierData@@@Z
// (by const reference; retail 0x006DB060, out of line). Needs engine/CComboMultiplierData.h and engine/CDataOutputStream.h first. Do
// not combine with a CPersistTraits<CComboMultiplierData> member specialisation of the primary template (0x006DCFA0's TransferBinaryIn).
template<> class CPersistTraits<CComboMultiplierData> {
public:
    CComboMultiplierData DefaultVal;
    CPersistTraits():DefaultVal() {}
    void TransferBinaryOut(CDataOutputStream& os,const CComboMultiplierData& value);              // 0x006DB060
};
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_HEROSTATINCREASEDATA_BINARYOUT
// NF88 opt-in (round 7, q11152 binder, row 0x006DC450 GFSerialiseMapBinaryOut<CHeroStatIncreaseData, EDamageAttribute>): the traits of
// CHeroExperienceDef::StatIncreasePerHitType's value, as the CComboMultiplierData opt-in above: DefaultVal and the writer Ego_r.pdb
// 0x0058AD90 / FableWin.pdb 0x01F0B550 ?TransferBinaryOut@?$CPersistTraits@VCHeroStatIncreaseData@@@@QAEXAAVCDataOutputStream@@ABVCHeroStatIncreaseData@@@Z
// (by const reference; retail 0x006DB0F0, out of line). Needs engine/CHeroStatIncreaseData.h and engine/CDataOutputStream.h first. Do
// not combine with a CPersistTraits<CHeroStatIncreaseData> instantiation of the primary template in the same TU.
template<> class CPersistTraits<CHeroStatIncreaseData> {
public:
    CHeroStatIncreaseData DefaultVal;
    CPersistTraits():DefaultVal() {}
    void TransferBinaryOut(CDataOutputStream& os,const CHeroStatIncreaseData& value);             // 0x006DB0F0
};
#endif
#ifdef FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "engine/CMemoryDataInputStream.h"
#include "engine/CWoundMorphs.h"
#include "engine/CExpressionSet.h"
#include "engine/CDefString.h"
template<> class CPersistTraits<CWoundMorphs> {
public:
    CWoundMorphs DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream& is,CWoundMorphs& value) { value.TransferBinaryIn(is); }
    void TransferBinaryOut(CDataOutputStream& os,const CWoundMorphs& value) { value.TransferBinaryOut(os); }
};
template<> class CPersistTraits<CExpressionSet> {
public:
    CExpressionSet DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream& is,CExpressionSet& value) { value.TransferBinaryIn(is); }
    void TransferBinaryOut(CDataOutputStream& os,const CExpressionSet& value) { value.TransferBinaryOut(os); }
};
template<> class CPersistTraits<CDefString> {
public:
    CDefString DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& is,CDefString& value) { value.TransferBinaryIn(is); }
    void TransferBinaryOut(CDataOutputStream& os,const CDefString& value) { value.TransferBinaryOut(os); }
};
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22
// Both raw TPI class records: complete native DefaultVal plus original binary signatures.
#include "engine/CRandomAppearanceMorph.h"
#include "engine/CFloatRange.h"
template<> class CPersistTraits<CRandomAppearanceMorph> {
public:
 CRandomAppearanceMorph DefaultVal;
 CPersistTraits():DefaultVal() {}
 // Inline-budget: native00676764/006767A3/006767E2 expand this wrapper and the actual member destructor; plain wrapper remained a call.
 FABLE_FORCEINLINE ~CPersistTraits() {}
 void TransferBinaryIn(CMemoryDataInputStream& is,CRandomAppearanceMorph& value) {value.TransferBinaryIn(is);}
 void TransferBinaryOut(CDataOutputStream& os,const CRandomAppearanceMorph& value) {value.TransferBinaryOut(os);}
};
template<> class CPersistTraits<CFloatRange> {
public:
 CFloatRange DefaultVal;
 CPersistTraits():DefaultVal() {}
 ~CPersistTraits() {}
 void TransferBinaryIn(CMemoryDataInputStream&,CFloatRange&);
#ifdef FABLE_RETAIL_CPERSISTTRAITS_CFLOATRANGE_BINARYOUT_FORCEINLINE
 // Per-TU inline opt-in (agentNF54, row 0x00458F2B CPersistContext::Transfer<CFloatRange>, a size TU): retail expands this body in
 // place (two CDataOutputStream::WriteFloat 0x00993EE0 calls, LowValue +4 first, 0x00458F4D..0x00458F6C); the plain inline member is
 // called out of line there. Same body.
 FABLE_FORCEINLINE
#endif
 void TransferBinaryOut(CDataOutputStream& os,const CFloatRange& value) {os.WriteFloat(value.LowValue);os.WriteFloat(value.HighValue);}
};
#endif
#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
// Full original specialisations, Ego_r TPI0xD74E/0xC4DE/0xF77B.
#include "engine/CRGBColour.h"
template<> class CPersistTraits<bool> {public:
    bool DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    bool TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,bool&);
    void TransferBinaryOut(CDataOutputStream&,const bool&);
    bool TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,bool&);
    void TransferOut(CPersistContext&,CCharString&,const bool&);
};
template<> class CPersistTraits<float> {public:
    float DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    float TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,float&);
    void TransferBinaryOut(CDataOutputStream&,const float&);
    float TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,float&);
    void TransferOut(CPersistContext&,CCharString&,const float&);
};
template<> class CPersistTraits<CRGBColour> {public:
    CRGBColour DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    CRGBColour TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,CRGBColour&);
    void TransferBinaryOut(CDataOutputStream&,const CRGBColour&);
    CRGBColour TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,CRGBColour&);
    void TransferOut(CPersistContext&,CCharString&,const CRGBColour&);
};
template<> class CPersistTraits<CCharString> {public:
    CCharString DefaultVal;
    CPersistTraits():DefaultVal() {}
    ~CPersistTraits() {}
    CCharString TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,CCharString&);
    void TransferBinaryOut(CDataOutputStream&,const CCharString&);
    CCharString TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,CCharString&);
    void TransferOut(CPersistContext&,CCharString&,const CCharString&);
};
#endif
#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#include "engine/CMemoryDataInputStream.h"
inline void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& is,bool& value) {value=is.ReadEBOOL();}
inline void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream& is,float& value) {float temp;is.Read(&temp,sizeof(temp));value=temp;}
inline void CPersistTraits<CRGBColour>::TransferBinaryIn(CMemoryDataInputStream& is,CRGBColour& value) {value=is.ReadColour();}
inline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream& is,CCharString& value) {value=is.ReadNullTerminatedString();}
inline void CPersistTraits<bool>::TransferBinaryOut(CDataOutputStream& os,const bool& value) {os.WriteEBOOL(value);}
inline void CPersistTraits<float>::TransferBinaryOut(CDataOutputStream& os,const float& value) {os.WriteFloat(value);}
inline void CPersistTraits<CRGBColour>::TransferBinaryOut(CDataOutputStream& os,const CRGBColour& value) {os.WriteColour(value);}
inline void CPersistTraits<CCharString>::TransferBinaryOut(CDataOutputStream& os,const CCharString& value) {os.WriteNullTerminatedString(value);}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_TRANSFEROUT
// The generic body (retail 0x005D8185 for unsigned long, a size-TU frame; 0x005CC6C0 for long): the value converted to a long and written
// through CPersistTraitsSLONG::TransferOut 0x009E2560. Visible to the includer, which then knows the call never reads the empty traits
// object (TransferMap<long,unsigned long> 0x005D8978 homes both traits objects of its save loop in the dead map argument slot [ebp+0Ch]).
template<class T> inline void CPersistTraits<T>::TransferOut(CPersistContext& context,CCharString& out,const T& value) {
    long longValue=(long)value; CPersistTraitsSLONG::TransferOut(context,out,longValue);
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_OBJECTAUGMENTATIONPARTICLESET
// NF62 opt-in (2026-11-17, rows 0x004E9550 / 0x004E9642): the explicit specialisation CPersistTraits<CObjectAugmentationParticleSet>
// with its Ego_r.pdb / FableWin.pdb members (the generic template's by-value TransferBinaryOut cannot take the class):
//   ?TransferBinaryIn@?$CPersistTraits@VCObjectAugmentationParticleSet@@@@QAEXAAVCMemoryDataInputStream@@AAVCObjectAugmentationParticleSet@@@Z (0x004E9642)
//   ?TransferBinaryOut@?$CPersistTraits@VCObjectAugmentationParticleSet@@@@QAEXAAVCDataOutputStream@@ABVCObjectAugmentationParticleSet@@@Z (0x004E959D)
//   ?TransferIn@..QAEXAAVCPersistContext@@AAVCStringParser@@AAVCObjectAugmentationParticleSet@@@Z,
//   ?TransferOut@..QAEXAAVCPersistContext@@AAVCCharString@@AAVCObjectAugmentationParticleSet@@@Z.
// DefaultVal at +0 (0x2C bytes); no user constructor / destructor declared: a traits local runs DefaultVal's out-of-line
// ??0CObjectAugmentationParticleSet@@QAE@XZ (retail 0x004E9627) and ??1CObjectAugmentationParticleSet@@QAE@XZ (0x004E25F1, the landed row).
#include "engine/CObjectAugmentationParticleSet.h"
template<> class CPersistTraits<CObjectAugmentationParticleSet> {
public:
    CObjectAugmentationParticleSet DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream&,CObjectAugmentationParticleSet&);
    void TransferBinaryOut(CDataOutputStream&,const CObjectAugmentationParticleSet&);
    void TransferIn(CPersistContext&,CStringParser&,CObjectAugmentationParticleSet&);
    void TransferOut(CPersistContext&,CCharString&,CObjectAugmentationParticleSet&);
};
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_SCALAR_BINARYIN_INLINE
// NF62 opt-in (2026-11-17, row 0x004E9642 CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn): the generic member
// ?TransferBinaryIn@?$CPersistTraits@J@@QAEXAAVCMemoryDataInputStream@@AAJ@Z / ..@M@@..AAM@Z (Ego_r.pdb / FableWin.pdb) as an inline explicit
// specialisation for long and float (FABLE_FORCEINLINE: a plain inline specialisation stays a call under optimize("s"), 99 v 460 bytes): retail expands CMemoryDataInputStream::Read in place for each scalar member (0x004E9642: five
// 4-byte reads, ReadWithSrcChunkOverflow 0x00993CA0 on the overflow arm), the read value copied out of a local. Needs
// FABLE_RETAIL_CDATAINPUTSTREAM + FABLE_RETAIL_CMEMORYDATAINPUTSTREAM; not with FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS /
// FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE (their float class) or FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY.
#include "engine/CMemoryDataInputStream.h"
template<> FABLE_FORCEINLINE void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& is,long& val) { long tval; is.Read(&tval,sizeof(tval)); val=tval; }
template<> FABLE_FORCEINLINE void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream& is,float& val) { float tval; is.Read(&tval,sizeof(tval)); val=tval; }
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_LONG_BINARY_BODY
template<> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& is,long& val) {
 long tval=is.ReadSLONG();val=tval;
}
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS
// Retail opt-in (wf16f row 0x006D8F10 CHeroExperienceDef::Transfer): the float traits as their own class, because the float writer
// takes its value by const reference (FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@M@@QAEXAAVCDataOutputStream@@ABM@Z; the
// generic template passes T by value) and writes through CDataOutputStream::WriteFloat 0x00993EE0, inline (retail 0x006D9014:
// `mov eax,[ebx] ; mov ecx,[esi+0x28] ; push eax ; call 0x993EE0`). The other members are the Ego_r.pdb / FableWin.pdb set:
// ?TransferBinaryIn@?$CPersistTraits@M@@QAEXAAVCMemoryDataInputStream@@AAM@Z (0x0040EFB0), ...QAEMAAVCMemoryDataInputStream@@@Z,
// ?TransferIn@...AAVCPersistContext@@AAVCStringParser@@AAM@Z, ?TransferOut@...AAVCPersistContext@@AAVCCharString@@ABM@Z.
// DefaultVal is left uninitialised here: the retail inline expansions store nothing into the traits object (the PDB constructor's
// store only disappears when the body of TransferBinaryIn is visible to the includer).
#include "engine/CDataOutputStream.h"
template<> class CPersistTraits<float> {
public:
    float DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream&,float&);
    float TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryOut(CDataOutputStream& stream,const float& value) { stream.WriteFloat(value); }
    void TransferIn(CPersistContext&,CStringParser&,float&);
    void TransferOut(CPersistContext&,CCharString&,const float&);
};
#endif
template<> class CPersistTraits<unsigned __int64> {
public:
    unsigned __int64 DefaultVal;
#ifdef FABLE_RETAIL_CPERSISTTRAITS_UID_DEFAULT_PASS27
    // FableWin 01A628D0 and 01A62900 are the UID default constructor and destructor.
    CPersistTraits() : DefaultVal() {}
    ~CPersistTraits() {}
#endif
    unsigned __int64 TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,unsigned __int64&);
    void TransferBinaryOut(CDataOutputStream& stream,const unsigned __int64& val) { stream.WriteUHUGE(val); }
    unsigned __int64 TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,unsigned __int64&);
    void TransferOut(CPersistContext&,CCharString&,const unsigned __int64&);
};
#if defined(FABLE_RETAIL_SYMBOLMAP_ENUM_TRAIT_OWNER_PASS29) && defined(_MSC_VER)
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
template<class T> inline void CPersistTraits<T>::TransferIn(CPersistContext& context,CStringParser& parser,T& value) {
    long result; CPersistTraitsSLONG::TransferIn(context,parser,result); value=(T)result;
}
#ifdef FABLE_RETAIL_SYMBOLMAP_ENUM_TRAIT_OWNER_PASS29
template<> inline void CPersistTraits<long>::TransferIn(CPersistContext& context,CStringParser& parser,long& value) {
    long result; CPersistTraitsSLONG::TransferIn(context,parser,result); value=result;
}
#endif
#if defined(FABLE_RETAIL_SYMBOLMAP_ENUM_TRAIT_OWNER_PASS29) && defined(_MSC_VER)
#pragma optimize("",on)
#endif
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
// Retail opt-in (chunk 1020, row 0x0067E290 CDegradableDef::Transfer): CPersistTraits<bool> is an explicit class specialisation in
// retail -- Ego_r.pdb / FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@_N@@QAEXAAVCDataOutputStream@@AB_N@Z takes `const bool&` where
// the generic member takes T by value (the scalar instances @C@ @E@ @H@ @J@ @K@ and every enum take the value). The inline save
// reads the bool after the stream (`mov ecx,[esi+28h] ; xor eax,eax ; mov al,[ebx] ; push eax ; call WriteEBOOL`, 0x0067E32B).
// Supersedes FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT (do not define both).
template<> class CPersistTraits<bool> {
public:
    bool DefaultVal;
#ifdef FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYIN_NOINLINE
    // NF78 opt-in (round 7, row 0x007ADBF0 GFSerialiseMapBinaryIn<bool, CCharString>): the reader's body (the landed 0x00403EB0 text:
    // the inline 1-byte CMemoryDataInputStream::ReadEBOOL into the value) visible to the TU but kept out of line, as retail calls the
    // 0x00403EB0 COMDAT there; seeing it reads only the stream and writes only the value, VC7.1 shares the count's and the traits'
    // frame words as retail does (frame 0x20; declared only, 0x28). Same function, same spelling.
#ifdef _MSC_VER
    __declspec(noinline)
#endif
    void TransferBinaryIn(CMemoryDataInputStream& is,bool& value) { value=is.ReadEBOOL(); }   // 0x00403EB0
#else
    void TransferBinaryIn(CMemoryDataInputStream&,bool&);     // 0x00403EB0
#endif
    void TransferBinaryOut(CDataOutputStream& stream,const bool& value) { stream.WriteEBOOL(value); }
    void TransferIn(CPersistContext&,CStringParser&,bool&);
};
#else
#ifndef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
template<> void CPersistTraits<bool>::TransferIn(CPersistContext&,CStringParser&,bool&);
#endif
#endif
#if defined(FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_TRANSFERIN_DECL) && !defined(FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE)
// Opt-in (p25A, row 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn): Ego_r.pdb / FableWin.pdb
// ?TransferIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCharString@@@Z, the quoted-string reader,
// out of line in retail at 0x0040E510 (landed; called at 0x005C22F8 / 0x005C25E4 with `this` = the traits object). The generic member
// body (a long read cast to T) does not apply to CCharString. Declaration only.
template<> void CPersistTraits<CCharString>::TransferIn(CPersistContext&,CStringParser&,CCharString&);   // 0x0040E510
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
#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
    long tval=is.ReadSLONG();
    val=(T)tval;
#else
    T tval;
    is.Read(&tval,sizeof(T));
    val=tval;
#endif
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
#ifdef FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS_CONST_DEFAULTVAL
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
#error FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS_CONST_DEFAULTVAL and FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY are exclusive
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
#ifdef FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
    // Original Ego_r TPI0x14385, declaration-only methods needed by the real transfer template.
    void TransferBinaryIn(CMemoryDataInputStream&,C3DVector&);
    void TransferBinaryOut(CDataOutputStream&,const C3DVector&);
    void TransferOut(CPersistContext&,CCharString&,const C3DVector&);
#endif
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
#endif // FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY
#endif

#if defined(FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE) && !defined(FABLE_PASS22_CPSCLIGHT_VECTOR_TRAITS_BODY)
#define FABLE_PASS22_CPSCLIGHT_VECTOR_TRAITS_BODY
#if defined(_MSC_VER) && defined(FABLE_RETAIL_SPLINE_TRAITS_SIZE_PASS29)
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
inline void CPersistTraits<C3DVector>::TransferBinaryIn(CMemoryDataInputStream& is,C3DVector& value) {value=is.Read3DVector();}
#if defined(_MSC_VER) && defined(FABLE_RETAIL_SPLINE_TRAITS_SIZE_PASS29)
#pragma optimize("",on)
#endif
inline void CPersistTraits<C3DVector>::TransferBinaryOut(CDataOutputStream& os,const C3DVector& value) {os.Write3DVector(value);}
#endif
