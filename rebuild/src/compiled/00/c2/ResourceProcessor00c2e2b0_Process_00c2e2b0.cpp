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