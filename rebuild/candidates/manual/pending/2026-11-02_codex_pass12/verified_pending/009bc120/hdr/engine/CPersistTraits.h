#ifndef FABLE_ENGINE_CPERSISTTRAITS_H
#define FABLE_ENGINE_CPERSISTTRAITS_H
#include "engine/CPersistContext.h"
#include "engine/C3DVector.h"
// Original TPI proves every traits class has DefaultVal at zero and these
// exact overloads. CPersistTraitsSLONG is a stateless static method family.
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferIn(CPersistContext&,CStringParser&,long&);
#if defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_PERSIST_OPINION)
    static long __fastcall TransferIn(CPersistContext&,CStringParser&);
#else
    static long __fastcall TransferIn(CPersistContext& context,CStringParser& parser) {
        long value; TransferIn(context,parser,value); return value;
    }
#endif
};
template<class T> class CPersistTraits {
public:
    T DefaultVal;
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    CPersistTraits():DefaultVal(T()){}
    T TransferIn(CPersistContext&,CStringParser&);
#endif
#if defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    void TransferBinaryIn(CMemoryDataInputStream&,T&);
    void TransferBinaryOut(CDataOutputStream& stream,T value) { stream.WriteSLONG((long)value); }
#endif
    void TransferIn(CPersistContext& context,CStringParser& parser,T& value);
};
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_ENTITY_PERSIST)
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
#endif
#ifdef FABLE_RETAIL_SHAPE_PERSIST
// Retail opt-in: original float specialization, TPIc4cb; default field+0.
template<> class CPersistTraits<float> {
public:
    float DefaultVal;
    CPersistTraits() {}
    float TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,float&);
    void TransferBinaryOut(CDataOutputStream&,const float&);
    float TransferIn(CPersistContext&,CStringParser&);
    void TransferIn(CPersistContext&,CStringParser&,float&);
    void TransferOut(CPersistContext&,CCharString&,const float&);
};
#endif
template<class T> inline void CPersistTraits<T>::TransferIn(CPersistContext& context,CStringParser& parser,T& value) {
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    value=(T)CPersistTraitsSLONG::TransferIn(context,parser);
#else
    long result; CPersistTraitsSLONG::TransferIn(context,parser,result); value=(T)result;
#endif
}
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
// Original scalar/string specializations, including original user lifetimes.
template<> class CPersistTraits<long> {
public:
 long DefaultVal;
 CPersistTraits():DefaultVal(0){}
 ~CPersistTraits(){}
 long TransferBinaryIn(CMemoryDataInputStream&);
 void TransferBinaryIn(CMemoryDataInputStream&,long&);
 void TransferBinaryOut(CDataOutputStream&,long);
 long TransferIn(CPersistContext& context,CStringParser& parser) {long tval;CPersistTraitsSLONG::TransferIn(context,parser,tval);return tval;}
 void TransferIn(CPersistContext& context,CStringParser& parser,long& val) {long tval;CPersistTraitsSLONG::TransferIn(context,parser,tval);val=tval;}
 void TransferOut(CPersistContext&,CCharString&,const long&);
};
template<> class CPersistTraits<float> {
public:
 float DefaultVal;
 CPersistTraits():DefaultVal(0.0f){}
 ~CPersistTraits(){}
 float TransferBinaryIn(CMemoryDataInputStream&);
 void TransferBinaryIn(CMemoryDataInputStream&,float&);
 void TransferBinaryOut(CDataOutputStream&,const float&);
 float TransferIn(CPersistContext&,CStringParser&);
 void TransferIn(CPersistContext&,CStringParser&,float&);
 void TransferOut(CPersistContext&,CCharString&,const float&);
};
template<> class CPersistTraits<CCharString> {
public:
 CCharString DefaultVal;
 CPersistTraits(){}
 ~CPersistTraits(){}
 CCharString TransferBinaryIn(CMemoryDataInputStream&);
 void TransferBinaryIn(CMemoryDataInputStream&,CCharString&);
 void TransferBinaryOut(CDataOutputStream&,const CCharString&);
 CCharString TransferIn(CPersistContext&,CStringParser&);
 void TransferIn(CPersistContext&,CStringParser&,CCharString&);
 void TransferOut(CPersistContext&,CCharString&,const CCharString&);
};
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) && !defined(FABLE_RETAIL_MINIMAP_TRAITS)
// Original long specialization: real tval wrappers and user lifetime.
template<> class CPersistTraits<long> {
public:
    long TransferBinaryIn(CMemoryDataInputStream&);
    void TransferBinaryIn(CMemoryDataInputStream&,long&);
    void TransferBinaryOut(CDataOutputStream&,long);
    long TransferIn(CPersistContext& persist_context,CStringParser& parser) {
        long tval; CPersistTraitsSLONG::TransferIn(persist_context,parser,tval); return tval;
    }
    void TransferIn(CPersistContext& persist_context,CStringParser& parser,long& val) {
        long tval; CPersistTraitsSLONG::TransferIn(persist_context,parser,tval); val=tval;
    }
    void TransferOut(CPersistContext&,CCharString&,const long&);
    long DefaultVal;
    CPersistTraits():DefaultVal(0){}
    ~CPersistTraits(){}
};
#endif
template<> void CPersistTraits<bool>::TransferIn(CPersistContext&,CStringParser&,bool&);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
template<> class CPersistTraits<CTCInventoryMap::CUIMiniMapMarker> {
public:
    void TransferIn(CPersistContext&,CStringParser&,CTCInventoryMap::CUIMiniMapMarker&);
    CTCInventoryMap::CUIMiniMapMarker DefaultVal;
};

#endif
class CThing;
class COpinionDeed;
template<class T> class CIntelligentPointer;
#if !defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) && !defined(FABLE_RETAIL_MINIMAP_TRAITS) && !defined(FABLE_RETAIL_SHAPE_PERSIST)
template<> void CPersistTraits<CIntelligentPointer<CThing> >::TransferIn(CPersistContext&,CStringParser&,CIntelligentPointer<CThing>&);
#endif
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
