#ifndef FABLE_ENGINE_CFILTER_BIND2_H
#define FABLE_ENGINE_CFILTER_BIND2_H
// Ego_r TPI keeps const references F1/F2, exactly two pointer words.
template<class First,class Second> class CFilter_Bind2 {
protected:
    const First& F1;
    const Second& F2;
public:
    CFilter_Bind2(const First& a,const Second& b):F1(a),F2(b){}
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing); }
};
template<class First,class Second>
CFilter_Bind2<First,Second> __fastcall GFFilter_Bind2(const First& a,const Second& b) {
    return CFilter_Bind2<First,Second>(a,b);
}
#endif
