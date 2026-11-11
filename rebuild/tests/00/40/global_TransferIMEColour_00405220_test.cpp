#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
static void Check(bool b){if(!b)abort();}
static int LiveStrings;
class CCharString {
 char Text[96];
public:
 CCharString(const char* p){Check(strlen(p)<sizeof(Text));strcpy(Text,p);++LiveStrings;}
 CCharString(const CCharString& x){strcpy(Text,x.Text);++LiveStrings;}
 ~CCharString(){--LiveStrings;}
 operator const char*()const{return Text;}
};
CCharString FABLE_FASTCALL operator+(const CCharString& x,const char* y){char text[96];strcpy(text,x);strcat(text,y);return CCharString(text);}
class CRGBColour {
public:
 union {struct{unsigned char B,G,R,A;};unsigned int IntValue;};
 CRGBColour(){}
 CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}
 CRGBColour(const CRGBColour& x):IntValue(x.IntValue){}
};
unsigned char IMERenderAlpha;
class CPersistContext {
public:
 unsigned char Defaults[4],Values[4];unsigned int Mask;
 const char* Prefix;int Calls;unsigned char* FirstValue;const unsigned char* FirstDefault;
 template<class T>void Transfer(const char* name,T& value,const T& def){
  const char* suffix="RGBA";char expected[96];strcpy(expected,Prefix);int len=(int)strlen(expected);expected[len]=suffix[Calls];expected[len+1]=0;
  Check(strcmp(name,expected)==0);Check(Calls>=0&&Calls<4);Check(def==Defaults[Calls]);Check(&value!=&def);
  if(Calls==0){FirstValue=&value;FirstDefault=&def;}else{
   int offsets[]={0,-1,-2,1};Check(&value==FirstValue+offsets[Calls]);Check(&def==FirstDefault+offsets[Calls]);
  }
  value=(Mask&(1u<<Calls))?Values[Calls]:def;
  ++Calls;IMERenderAlpha^=255;
 }
};
CRGBColour FABLE_FASTCALL IMEColour2RGB(unsigned long colour)
{
 unsigned char blue=(unsigned char)colour;
 colour>>=8;
 unsigned char green=(unsigned char)colour;
 colour>>=8;
 unsigned char red=(unsigned char)colour;
 return CRGBColour(red,green,blue,IMERenderAlpha);
}
unsigned long FABLE_FASTCALL RGB2IMEColour(CRGBColour& colour)
{
 return 0xff000000UL|((unsigned long)colour.R<<16)|((unsigned long)colour.G<<8)|colour.B;
}
unsigned long FABLE_FASTCALL TransferIMEColour(CPersistContext& context,const CCharString& token,unsigned long defaultColour)
{
 CRGBColour defCol,col;
 defCol=IMEColour2RGB(defaultColour);
 context.Transfer(token+"R",col.R,defCol.R);
 context.Transfer(token+"G",col.G,defCol.G);
 context.Transfer(token+"B",col.B,defCol.B);
 context.Transfer(token+"A",col.A,defCol.A);
 return RGB2IMEColour(col);
}
int main(){
 const unsigned long colours[]={0,0xffffffffUL,0x12345678UL,0x80818283UL,0x00010203UL,0xffaabbccUL,0x00ff00ffUL,0xff00ff00UL};
 const unsigned char alphas[]={0,1,2,63,127,128,254,255};unsigned int count=0;
 for(int c=0;c<8;++c)for(int a=0;a<8;++a)for(unsigned int mask=0;mask<16;++mask)for(int pattern=0;pattern<4;++pattern)for(int p=0;p<2;++p){
  Check(LiveStrings==0);{
   const char* prefix=p?"SymbolColorTarget":"";CCharString token(prefix);CPersistContext context;
   context.Defaults[0]=(unsigned char)(colours[c]>>16);context.Defaults[1]=(unsigned char)(colours[c]>>8);context.Defaults[2]=(unsigned char)colours[c];context.Defaults[3]=alphas[a];
   for(int i=0;i<4;++i)context.Values[i]=(unsigned char)(pattern*71+i*37);
   context.Mask=mask;context.Prefix=prefix;context.Calls=0;IMERenderAlpha=alphas[a];
   unsigned long want=0xff000000UL;
   for(int channel=0;channel<3;++channel){unsigned long v=(mask&(1u<<channel))?context.Values[channel]:context.Defaults[channel];want|=v<<(16-8*channel);}
   unsigned long got=TransferIMEColour(context,token,colours[c]);
   Check(got==want&&context.Calls==4&&LiveStrings==1);Check(IMERenderAlpha==alphas[a]);
  }Check(LiveStrings==0);++count;
 }
 Check(count==8192);
 // Independent channel conversion checks include every byte and return-copy semantics.
 for(unsigned int value=0;value<256;++value){
  IMERenderAlpha=(unsigned char)(255-value);CRGBColour c=IMEColour2RGB((value<<16)|((255-value)<<8)|value);
  Check(c.R==value&&c.G==255-value&&c.B==value&&c.A==255-value);
  Check(RGB2IMEColour(c)==(0xff000000UL|(value<<16)|((255-value)<<8)|value));
 }
 puts("00405220_TEST PASS");return 0;
}