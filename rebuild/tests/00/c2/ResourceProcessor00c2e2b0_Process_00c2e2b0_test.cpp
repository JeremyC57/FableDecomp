#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 348fca606477657311099fea7aa3394b7af91cfcf37e0e000f12c17801f7f574
// 00c2e2b0 ResourceProcessor_00c2e2b0::Process
// Flags: /O2 /Oy
// Leaf: Process
// Layout: receiver manager4,context8,map+c,lookup3c,options74/75/77/78; input packed offsets0..2f. Actual owner uncertain; C3DAnimationSequence label unsupported.
struct Params{unsigned data[5];Params();void Mode(bool);void Kind(unsigned);void Value14(unsigned);void Value28(unsigned);void Value24(unsigned);void Scale1(float);void Scale2(float);void Value10(unsigned);void Inverse(float);void Angle(float);void Option1(unsigned short);void Option2(unsigned short);void Flag1(bool);void Flag2(bool);};
struct Log{Log& __fastcall Text(const char*);Log& __fastcall Number(unsigned);};struct Context{Log& Error();};
struct Manager{void* Create(void*,const Params&);};struct Lookup{void* Find(unsigned);};struct Map{void Add(Context*,void*,unsigned);};
struct Input{unsigned id,resource,unused;unsigned short padc,kind;unsigned value10,value14;unsigned short option2,option1,angle,scale1;unsigned char flags,pad21;unsigned short scale2;unsigned value24,value28;int divisor;};
extern const float Scale_01230a5c,AngleScale_0129f780;
struct ResourceProcessor_00c2e2b0{unsigned pad0;Manager* manager;Context* context;char map[48];char lookup[56];bool option1,option2,pad76,flag1,flag2;void Process(const Input*);};
void ResourceProcessor_00c2e2b0::Process(const Input* input){
 void* resource=((Lookup*)lookup)->Find(input->resource);if(!resource){context->Error().Text("Could not find resource number ").Number(input->resource);return;}
 if(input->flags&4){Params p;p.Mode((input->flags>>3)&1);p.Kind(input->kind);p.Value14(input->value14);if(input->flags&32)p.Value28(input->value28);if(input->flags&16)p.Value24(input->value24);p.Scale1(input->scale1*Scale_01230a5c);p.Scale2(input->scale2*Scale_01230a5c);if(input->value10)p.Value10(input->value10);if(input->divisor!=-1)p.Inverse(1.0f/input->divisor);p.Angle(input->angle*AngleScale_0129f780);if(option1)p.Option1(input->option1);if(option2)p.Option2(input->option2);if(flag1 && (input->flags&1))p.Flag1(true);if(flag2 && (input->flags&2))p.Flag2(true);void* result=manager->Create(resource,p);((Map*)map)->Add(context,result,input->id);}
}

// END exact source snapshot
const float Scale_01230a5c=0.00999999977648258209228515625f,AngleScale_0129f780=0.0078740157186985015869140625f;static float scale1Seen,scale2Seen,inverseSeen,angleSeen;static unsigned mask;static int creates,adds,errors;static bool found=true;static Log logObject;Params::Params(){memset(data,0,sizeof(data));}
void Params::Mode(bool value){mask|=1U<<0;data[0]=(unsigned)value;}
void Params::Kind(unsigned value){mask|=1U<<1;data[0]=(unsigned)value;}
void Params::Value14(unsigned value){mask|=1U<<2;data[0]=(unsigned)value;}
void Params::Value28(unsigned value){mask|=1U<<3;data[0]=(unsigned)value;}
void Params::Value24(unsigned value){mask|=1U<<4;data[0]=(unsigned)value;}
void Params::Scale1(float value){scale1Seen=value;mask|=1U<<5;data[0]=(unsigned)value;}
void Params::Scale2(float value){scale2Seen=value;mask|=1U<<6;data[0]=(unsigned)value;}
void Params::Value10(unsigned value){mask|=1U<<7;data[0]=(unsigned)value;}
void Params::Inverse(float value){inverseSeen=value;mask|=1U<<8;data[0]=(unsigned)value;}
void Params::Angle(float value){angleSeen=value;mask|=1U<<9;data[0]=(unsigned)value;}
void Params::Option1(unsigned short value){mask|=1U<<10;data[0]=(unsigned)value;}
void Params::Option2(unsigned short value){mask|=1U<<11;data[0]=(unsigned)value;}
void Params::Flag1(bool value){mask|=1U<<12;data[0]=(unsigned)value;}
void Params::Flag2(bool value){mask|=1U<<13;data[0]=(unsigned)value;}
Log& Context::Error(){++errors;return logObject;}Log& __fastcall Log::Text(const char* s){assert(s && *s);return *this;}Log& __fastcall Log::Number(unsigned n){assert(n==17);return *this;}void* Lookup::Find(unsigned){return found?this:0;}void* Manager::Create(void* p,const Params&){++creates;return p;}void Map::Add(Context*,void* p,unsigned id){assert(p && id==9);++adds;}
int main(){ResourceProcessor_00c2e2b0 self;memset(&self,0,sizeof(self));Manager manager;Context context;self.manager=&manager;self.context=&context;Input input;memset(&input,0,sizeof(input));input.resource=17;input.id=9;input.divisor=-1;self.Process(&input);assert(creates==0);found=false;self.Process(&input);assert(errors==1);found=true;input.scale1=100;input.scale2=200;input.angle=127;input.flags=63;input.value10=1;input.divisor=2;self.option1=self.option2=self.flag1=self.flag2=true;self.Process(&input);assert(creates==1 && adds==1 && mask==16383);assert(scale1Seen==1.0f && scale2Seen==2.0f && inverseSeen==0.5f && angleSeen==1.0f);puts("00c2e2b0_TEST PASS");return 0;}