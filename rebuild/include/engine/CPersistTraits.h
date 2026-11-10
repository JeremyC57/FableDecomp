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
class CThing;
class COpinionDeed;
template<class T> class CIntelligentPointer;
template<> class CPersistTraits<COpinionDeed*> {
public:
    COpinionDeed* DefaultVal;
    void TransferIn(CPersistContext&,CStringParser&,COpinionDeed*&);
};
template<> class CPersistTraits<C3DVector> {
public:
    C3DVector DefaultVal;
    CPersistTraits() : DefaultVal(0.0f,0.0f,0.0f) { }
    void TransferIn(CPersistContext&,CStringParser&,C3DVector&);
};
#endif
