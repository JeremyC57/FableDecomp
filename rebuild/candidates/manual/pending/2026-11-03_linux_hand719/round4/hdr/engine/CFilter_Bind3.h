#ifndef FABLE_ENGINE_CFILTER_BIND3_H
#define FABLE_ENGINE_CFILTER_BIND3_H
// Ego_r TPI keeps const references F1/F2/F3, exactly three pointer words (sibling of CFilter_Bind2.h).
// Builder: FableWin 0x01CCAD30 ??$GFFilter_Bind3@VCThingFilter_OverlappingArea@@...@@YI?AV?$CFilter_Bind3@... (__fastcall, by value).
// (2026-11-03, chunk 719 row 0x00509A20, agent719c.)
template<class First,class Second,class Third> class CFilter_Bind3 {
protected:
    const First& F1;
    const Second& F2;
    const Third& F3;
public:
    CFilter_Bind3(const First& a,const Second& b,const Third& c):F1(a),F2(b),F3(c){}
    template<class T> bool operator()(const T& thing) const { return F1(thing) && F2(thing) && F3(thing); }
};
template<class First,class Second,class Third>
CFilter_Bind3<First,Second,Third> __fastcall GFFilter_Bind3(const First& a,const Second& b,const Third& c) {
    return CFilter_Bind3<First,Second,Third>(a,b,c);
}
#endif
