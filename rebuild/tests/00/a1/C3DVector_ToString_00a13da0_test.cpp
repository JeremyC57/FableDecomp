// C3DVector::ToString() const -- format the vector as "(X, Y, Z)" with two decimals
// Retail 00A13DA0. Calls CCharString::FormatDouble 009A08B0, constructor 0099EBF0,
// concatenation 0099F570/0099F600 and destructor 0099EAE0. No shared C3DVector header exists.
// Verified literals: '(' at0122E028, ', ' at012397CC, ')' at0122E020.
#if !defined(_MSC_VER)
#define __fastcall
#endif
class CCharString { public: void* PStringData; CCharString(const char* text, long length=-1); ~CCharString(); static CCharString __fastcall FormatDouble(long digits, double value); };
CCharString __fastcall operator+(const CCharString& left,const CCharString& right);
CCharString __fastcall operator+(const CCharString& left,const char* right);
class C3DVector { public: float X,Y,Z; CCharString ToString() const; };
CCharString C3DVector::ToString() const { return CCharString("(") + CCharString::FormatDouble(2,X) + ", " + CCharString::FormatDouble(2,Y) + ", " + CCharString::FormatDouble(2,Z) + ")"; }

#include <cstdio>
#include <cstring>
static int formats; static bool wrongPrecision;
static char buffers[64][128]; static int next_buffer;
static char* fresh(){char* p=buffers[next_buffer++%64];p[0]=0;return p;}
CCharString::CCharString(const char* text,long length) : PStringData(fresh()) { if(length<0) length=(long)std::strlen(text); if(length>0){std::strncpy((char*)PStringData,text,(size_t)length);((char*)PStringData)[length]=0;} }
CCharString::~CCharString(){}
CCharString CCharString::FormatDouble(long digits,double value){++formats;if(digits!=2)wrongPrecision=true;char b[64];std::sprintf(b,"%.*f",(int)digits,value);return CCharString(b,-1);}
static void add(char* p,const char* s){std::strcat(p,s);}
CCharString __fastcall operator+(const CCharString& a,const CCharString& b){char* p=fresh();add(p,(const char*)a.PStringData);add(p,(const char*)b.PStringData);CCharString r(p,-1);r.PStringData=p;return r;}
CCharString __fastcall operator+(const CCharString& a,const char* b){char* p=fresh();add(p,(const char*)a.PStringData);add(p,b);CCharString r(p,-1);r.PStringData=p;return r;}
int main(){C3DVector v;const float vals[2][3]={{1.2f,-3.4f,5.678f},{0.0f,0.0f,0.0f}};const char* expected[2]={"(1.20, -3.40, 5.68)","(0.00, 0.00, 0.00)"};for(int i=0;i<2;i++){formats=0;wrongPrecision=false;v.X=vals[i][0];v.Y=vals[i][1];v.Z=vals[i][2];CCharString s=v.ToString();if(formats!=3||wrongPrecision||std::strcmp((const char*)s.PStringData,expected[i])){std::printf("00a13da0_TEST FAIL: %s\n",(const char*)s.PStringData);return 1;}}std::puts("00a13da0_TEST PASS");return 0;}