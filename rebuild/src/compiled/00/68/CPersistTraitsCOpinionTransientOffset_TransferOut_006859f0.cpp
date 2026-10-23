// 006859f0 CPersistTraits<COpinionTransientOffset>::TransferOut
// Flags: /O2 /Oy /Gr
// Leaf: TransferOut
// Layout: retail opinion record24 has int+0,float+4,int+8,int+12,float+16,int+20; punctuation literals at0122e020/24/28.
struct CCharString {void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator+=(const char*);const CCharString& operator+=(const CCharString&);};
struct CPersistContext;
void __fastcall AppendIntegerText_009e2560(CPersistContext*,CCharString&,const int&);
CCharString __fastcall FormatRealText_009a08b0(int,double);
struct COpinionTransientOffset{int a;float b;int c,d;float e;int f;};
template<class T>struct CPersistTraits{void TransferOut(CPersistContext*,CCharString&,const T&);};
template<class T>void CPersistTraits<T>::TransferOut(CPersistContext* context,CCharString& text,const T& value){
 text.operator+=("(");{int number=value.a;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(FormatRealText_009a08b0(6,value.b));
 text.operator+=(",");{int number=value.c;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");{int number=value.d;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(FormatRealText_009a08b0(6,value.e));
 text.operator+=(",");{int number=value.f;AppendIntegerText_009e2560(context,text,number);}text.operator+=(")");
}
template void CPersistTraits<COpinionTransientOffset>::TransferOut(CPersistContext*,CCharString&,const COpinionTransientOffset&);