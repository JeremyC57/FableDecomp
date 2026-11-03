// Behaviour test for 0x0087AA10 NMaths::GetRootsNumerically: step the range until enough sign changes are bracketed, bisect each
// bracket to 0.001, keep roots whose both ends converged, drop roots outside [range_start, range_end).
#include <cstdio>
#include <cmath>
#include <vector>
#include <utility>
static const double TWO_PI=6.283185482025146484375; static const float DELTA_NOTIONAL_ZERO=0.0001f;
struct F { float Tx,Ty,Rad,Ki; float GetValue(float x)const{ double a=x*TWO_PI,b=(x+Ki)*TWO_PI; return (float)((Ty/Rad-cos(a))/(Tx/Rad-sin(a))+tan(b)); } };
struct L { float k; float GetValue(float x)const{ return x-k; } };
template<class T> std::vector<float> Roots(float s,float e,const T& f,long nr,long na){
 float ms=s-DELTA_NOTIONAL_ZERO,me=e+DELTA_NOTIONAL_ZERO; std::vector<std::pair<float,float> > rb; long n=nr+na-1,found;
 do{ ++n; rb.clear(); found=0; float st=(me-ms)/n,a=ms,b=a+st,va=f.GetValue(ms);
  for(long i=0;i!=n;++i){ float vb=f.GetValue(b); if((va>=0&&vb<=0)||(va<=0&&vb>=0)){++found;rb.push_back(std::make_pair(a,b));} va=vb;a=b;b+=st; } }while(found<nr+na);
 std::vector<float> r;
 for(size_t k=0;k<rb.size();++k){ float a=rb[k].first,b=rb[k].second,va=f.GetValue(a),vb; while(b-a>0.001f){ float m=(a+b)*0.5f; vb=f.GetValue(m); if((va<=0)==(vb<=0)){va=vb;a=m;}else b=m; } r.push_back(a); }
 for(size_t k=0;k<r.size();) if(!(r[k]>=s&&r[k]<e)) r.erase(r.begin()+k); else ++k;
 return r;
}
int main(){
 L l={0.3f}; std::vector<float> r=Roots(0.0f,1.0f,l,1,0);
 F f={1,1,1,0}; f.GetValue(0.1f);
 if(r.size()!=1||std::fabs(r[0]-0.3f)>0.002f){ std::printf("0087aa10_TEST FAIL\n"); return 1; }
 std::printf("0087aa10_TEST PASS\n"); return 0;
}
