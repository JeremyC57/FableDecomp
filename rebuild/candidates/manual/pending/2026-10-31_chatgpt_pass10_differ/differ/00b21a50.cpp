// 00b21a50 NParticleEngine::CPSCSplineBase::CSplinePosParamData::DatabaseSerialise
// Leaf: ?DatabaseSerialise@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXAAVCPersistContext@@ABVCEmitterDatabaseSerialiseInfo@3@@Z
// Flags:
#include <stddef.h>
extern "C" void*__cdecl memmove(void*,const void*,unsigned int);
class C3DVector{public:float X,Y,Z;};
// Layout: only the original string-data prefix is used; remaining allocation and
// reference-count metadata is not materialized or claimed as a full object layout.
template<class T>class CBasicString{protected:T*PData;unsigned long StringLength;public:unsigned long GetLength()const{return StringLength;}const T*PeekData()const{return PData;}};
class CCharStringData{public:CBasicString<char>Data;};
class CCharString{protected:CCharStringData*PStringData;public:CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();operator const char*()const;
long GetLength()const{return PStringData==0?0:PStringData->Data.GetLength();}
const char*GetAsCharArray()const{return PStringData==0?"":PStringData->Data.PeekData();}
unsigned long GetCRC()const;
};
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall GFIntToCharString(long);
extern const unsigned long CRC32TAB[256];
class CCRC{public:static unsigned long __fastcall Calc(unsigned long seed,const void*data,unsigned long length){const char*bytes=static_cast<const char*>(data);if(length)for(unsigned long k=0;k<length;++k)seed=CRC32TAB[static_cast<unsigned char>(bytes[k])^(seed&0xff)]^(seed>>8);return seed;}static unsigned long __fastcall CalcFromString(const CCharString&s){return Calc(0,s.GetAsCharArray(),s.GetLength());}};
inline unsigned long CCharString::GetCRC()const{return CCRC::CalcFromString(*this);}
template<class T>class CPersistTraits{public:T DefaultVal;};
class CPersistContext{public:enum EMode{};protected:unsigned char unobservedContextPrefix[0x18];EMode Mode;
 template<class T>static T __fastcall GetDefaultVal(){CPersistTraits<T>traits;return traits.DefaultVal;}
public:bool IsSaving()const{return Mode==1||Mode==3;}
 template<class T>void Transfer(const char*,T&,const T&);
 template<class T>__forceinline void Transfer(const char*name,T&value){Transfer(name,value,GetDefaultVal<T>());}
};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
inline void*__copy_trivial(const void*first,const void*last,void*result){return last==first?result:static_cast<char*>(memmove(result,first,static_cast<const char*>(last)-static_cast<const char*>(first)))+(static_cast<const char*>(last)-static_cast<const char*>(first));}
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}const T*begin()const{return this->_M_start;}const T*end()const{return this->_M_finish;}unsigned int size()const{return end()-begin();}T&operator[](unsigned int n){return *(this->_M_start+n);}void resize(unsigned int,const T&);
__forceinline T*erase(T*first,T*last){T*i=static_cast<T*>(__copy_trivial(last,this->_M_finish,first));this->_M_finish=i;return first;}
__forceinline void clear(){erase(begin(),end());}
};
}
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return std::vector<T>::operator[](n);}};
class CCardinalSpline{private:CArray<C3DVector>ControlPoints;float Tension;bool EndPointsConnected;public:long GetControlPointCount()const{return ControlPoints.size();}};
namespace NParticleEngine{
class CParticleSystem;class CEmitterDatabaseSerialiseInfo;
class CPSCSplineBase{public:class CSplinePosParamData{private:const CParticleSystem*PParentParticleSystem;const CCardinalSpline&Spline;bool MaintainSplineShape;CArray<unsigned long>SplinePosParam;public:void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);};};
void CPSCSplineBase::CSplinePosParamData::DatabaseSerialise(CPersistContext&context,const CEmitterDatabaseSerialiseInfo&info){
 context.Transfer("MaintainSplineShape",MaintainSplineShape,false);
 if(!context.IsSaving()){
 long pos_param_size;context.Transfer("SplinePosParamSize",pos_param_size,0L);
 SplinePosParam.resize(pos_param_size>Spline.GetControlPointCount()?pos_param_size:Spline.GetControlPointCount(),CCharString().GetCRC());
 bool have_pos_param=false;
 for(long ci=0;ci<pos_param_size;++ci){CCharString temp;context.Transfer(CCharString("SplinePosParamSize_")+GFIntToCharString(ci),temp);SplinePosParam[ci]=temp.GetCRC();have_pos_param|=SplinePosParam[ci]!=0;}
 if(!have_pos_param)SplinePosParam.clear();
}
}
}
