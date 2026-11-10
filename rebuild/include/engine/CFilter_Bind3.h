#ifndef FABLE_ENGINE_CFILTER_BIND3_H
#define FABLE_ENGINE_CFILTER_BIND3_H
// Original PDB fields are protected const references, with no functor copies.
template<class TF1,class TF2,class TF3> class CFilter_Bind3 {
protected:
 const TF1& F1;
 const TF2& F2;
 const TF3& F3;
public:
 CFilter_Bind3(const TF1& f1,const TF2& f2,const TF3& f3):F1(f1),F2(f2),F3(f3){}
 template<class T> bool operator()(const T& t)const{return F1(t)&&F2(t)&&F3(t);}
};
#endif
