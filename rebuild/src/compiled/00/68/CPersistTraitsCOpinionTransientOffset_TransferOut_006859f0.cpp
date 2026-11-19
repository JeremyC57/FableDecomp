// 006859f0 CPersistTraits<COpinionTransientOffset>::TransferOut
// Flags: /O2 /Oy /Gr
// Leaf: TransferOut
// Layout: retail opinion record24 has int+0,float+4,int+8,int+12,float+16,int+20; punctuation literals at0122e020/24/28.
class CCharString { public:void* data;CCharString():data(0){} CCharString(const CCharString&);~CCharString();const CCharString& operator+=(const char*);const CCharString& operator+=(const CCharString&);};
class CPersistContext;
void __fastcall AppendIntegerText_009e2560(CPersistContext*,CCharString&,const int&);
CCharString __fastcall GFFloatToCharString(double n,long precision); // 0x009A08B0: Ego_r.pdb + FableWin.pdb ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z (NF94, q11248)
class COpinionTransientOffset{ public:int a;float b;int c,d;float e;int f;};
template<class T>struct CPersistTraits{void TransferOut(CPersistContext*,CCharString&,const T&);};
template<class T>void CPersistTraits<T>::TransferOut(CPersistContext* context,CCharString& text,const T& value){
 text.operator+=("(");{int number=value.a;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(GFFloatToCharString(value.b,6));
 text.operator+=(",");{int number=value.c;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");{int number=value.d;AppendIntegerText_009e2560(context,text,number);}
 text.operator+=(",");text.operator+=(GFFloatToCharString(value.e,6));
 text.operator+=(",");{int number=value.f;AppendIntegerText_009e2560(context,text,number);}text.operator+=(")");
}
template void CPersistTraits<COpinionTransientOffset>::TransferOut(CPersistContext*,CCharString&,const COpinionTransientOffset&);