#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cfloat>
#ifdef _MSC_VER
typedef unsigned __int64 TestId;
#else
typedef unsigned long long TestId;
#endif
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"line%d case%d: %s\n",__LINE__,gCase,#x);std::exit(1);}}while(0)
int gCase=0;
struct C3DVector {float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector&b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}};
struct CRGBColour {unsigned char B,G,R,A;CRGBColour():B(0),G(0),R(0),A(0){}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a=255):B(b),G(g),R(r),A(a){}};
class CWideString {public:std::wstring Value;CWideString(){}CWideString(const wchar_t*p):Value(p){}CWideString(const std::wstring&p):Value(p){}void Sprintf(const wchar_t*,long n){char a[40];std::sprintf(a,"%ld",n);Value.erase();for(int i=0;a[i];++i)Value+=wchar_t(a[i]);}};
CWideString operator+(const wchar_t*a,const CWideString&b){return CWideString(std::wstring(a)+b.Value);}CWideString operator+(const CWideString&a,const wchar_t*b){return CWideString(a.Value+std::wstring(b));}CWideString operator+(const CWideString&a,const CWideString&b){return CWideString(a.Value+b.Value);}
inline CWideString GFIntToWideString(long value) {CWideString text;text.Sprintf(L"%d",value);return text;}
enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_DEFAULT=0,ENGINE_INTERPOLATION_SET_1=1,ENGINE_INTERPOLATION_SET_2=2};enum EEnginePrimitiveBlendMode {ENGINE_PRIMITIVE_BLEND_SOLID=0,TEST_BLEND1,TEST_BLEND2,TEST_BLEND3,TEST_BLEND4,TEST_BLEND5};enum EEnginePrimitiveBlendOp {ENGINE_PRIMITIVE_BLEND_OP_ADD=0,TEST_OP1,TEST_OP2,TEST_OP3,TEST_OP4};
struct CFontBank {int Number;};
struct CEnginePrimitive {int Type;TestId Id;C3DVector Start,End;CRGBColour Colour;float Width;unsigned long Bank;bool Z;int Interpolation,Blend,Op;CWideString Text;const CFontBank* Font;bool ScaleIn3D;CEnginePrimitive():Type(0),Id(0),Width(0),Bank(0),Z(false),Interpolation(0),Blend(0),Op(0),Font(0),ScaleIn3D(false){}};
struct CEnginePrimitiveText:CEnginePrimitive {CEnginePrimitiveText(TestId id,const C3DVector&pos,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,bool scale3d,EEngineInterpolationSet interpolation){Type=5;Id=id;Start=pos;Width=scale;Text=text;Font=&font;Colour=colour;ScaleIn3D=scale3d;Interpolation=interpolation;}};
struct CEnginePrimitiveLine:CEnginePrimitive {CEnginePrimitiveLine(TestId id,const C3DVector&start,const C3DVector&end,const CRGBColour&colour,float width,EEnginePrimitiveBlendMode blend,unsigned long bank,bool z,EEngineInterpolationSet interpolation,EEnginePrimitiveBlendOp op){Type=6;Id=id;Start=start;End=end;Colour=colour;Width=width;Bank=bank;Z=z;Interpolation=interpolation;Blend=blend;Op=op;}};
struct PrimitiveData {int Refs,Notifies,Releases;PrimitiveData():Refs(0),Notifies(0),Releases(0){}};
struct CEnginePrimitiveHandle {PrimitiveData* Data;CEnginePrimitiveHandle():Data(0){}CEnginePrimitiveHandle(const CEnginePrimitiveHandle&b):Data(b.Data){if(Data)++Data->Refs;}CEnginePrimitiveHandle&operator=(const CEnginePrimitiveHandle&b){if(this!=&b){Clear();Data=b.Data;if(Data)++Data->Refs;}return *this;}~CEnginePrimitiveHandle(){Clear();}void Clear(){if(Data){if(Data->Refs==1)++Data->Notifies;--Data->Refs;++Data->Releases;Data=0;}}};
struct CThing {C3DVector Pos;TestId Id;int PosReads,IdReads;CThing():Id(0),PosReads(0),IdReads(0){}const C3DVector&GetPos()const{++const_cast<CThing*>(this)->PosReads;return Pos;}TestId GetUniqueID()const{++const_cast<CThing*>(this)->IdReads;return Id;}};
struct CMainGameComponent {const CFontBank* PFont;int FontReads;CMainGameComponent():PFont(0),FontReads(0){}const CFontBank&PeekDebugFont()const{++const_cast<CMainGameComponent*>(this)->FontReads;return *PFont;}};
struct CIEngine;struct CEnginePrimitivePrepareInfo {CIEngine*Engine;};
struct CTCCreatureGenerator {CThing Parent;CMainGameComponent Component;std::vector<C3DVector>OpenPositions,HiddenPositions;std::vector<CEnginePrimitiveHandle>DebugPrimitives;int ComponentReads;bool Mutate;const CFontBank* ChangedFont;CTCCreatureGenerator():ComponentReads(0),Mutate(false),ChangedFont(0){}CThing&GetParentThing(){return Parent;}const CMainGameComponent&PeekComponent()const{CTCCreatureGenerator*self=const_cast<CTCCreatureGenerator*>(this);++self->ComponentReads;if(Mutate){self->Parent.Id+=TestId(1)<<32;self->Parent.Pos=C3DVector(900,901,902);self->Component.PFont=ChangedFont;}return Component;}void DrawDebugInfo(const CEnginePrimitivePrepareInfo&info);};
struct Record {CEnginePrimitive Primitive;unsigned int Index;int Engine;};
struct CIEngine {CTCCreatureGenerator*Owner;CEnginePrimitivePrepareInfo*Info;CIEngine*Other;std::vector<Record>*Records;PrimitiveData*Pool;int Mode,Number;bool Fired;CIEngine():Owner(0),Info(0),Other(0),Records(0),Pool(0),Mode(0),Number(0),Fired(false){}virtual ~CIEngine(){}virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long bank){CHECK(bank==0);CHECK(!Owner->DebugPrimitives.empty());Record r;r.Primitive=p;r.Index=unsigned(&h-&Owner->DebugPrimitives[0]);r.Engine=Number;Records->push_back(r);if(!Fired){Fired=true;if(Mode){unsigned n=unsigned(Owner->DebugPrimitives.size());Owner->DebugPrimitives.resize(n+unsigned(Mode));for(int i=0;i<Mode;++i)Owner->DebugPrimitives[n+i].Data=&Pool[64+i];}if(Mode==2)Info->Engine=Other;}}};
bool DrawCreatureGeneratorDebug;

void CTCCreatureGenerator::DrawDebugInfo(const CEnginePrimitivePrepareInfo& info)
{
    if (!DrawCreatureGeneratorDebug)
        return;
    C3DVector centre=GetParentThing().GetPos();
    DebugPrimitives.resize((HiddenPositions.size()+OpenPositions.size())*2+1);
    CWideString label=L"Points (Open/Hidden): "+GFIntToWideString(OpenPositions.size())+L" / "+GFIntToWideString(HiddenPositions.size());
    C3DVector labelPos=centre+C3DVector(0,0,2);
    const CFontBank& font=PeekComponent().PeekDebugFont();
    unsigned int index=0;
    info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
        CEnginePrimitiveText(GetParentThing().GetUniqueID(),labelPos,1.0f,label,font,CRGBColour(255,255,255),true,ENGINE_INTERPOLATION_SET_DEFAULT),0);
    for (std::vector<C3DVector>::iterator it=OpenPositions.begin();it!=OpenPositions.end();++it) {
        CRGBColour openColour(0,255,0);
        CRGBColour openTopColour(128,255,128);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),centre,*it,openColour,0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),*it,*it+C3DVector(0,0,1),openTopColour,0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
    }
    for (std::vector<C3DVector>::iterator it=HiddenPositions.begin();it!=HiddenPositions.end();++it) {
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),centre,*it,CRGBColour(255,0,0),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
        info.Engine->SetPersistentPrimitive(DebugPrimitives[index++],
            CEnginePrimitiveLine(GetParentThing().GetUniqueID(),*it,*it+C3DVector(0,0,1),CRGBColour(255,128,128),0.05f,ENGINE_PRIMITIVE_BLEND_SOLID,0,true,ENGINE_INTERPOLATION_SET_DEFAULT,ENGINE_PRIMITIVE_BLEND_OP_ADD),0);
    }
    for (std::vector<CEnginePrimitiveHandle>::iterator it=DebugPrimitives.begin()+index;it!=DebugPrimitives.end();++it)
        it->Clear();
}


#if !defined(_MSC_VER)
#define __int64 long long
#endif
namespace OriginalConstructors {
enum EEnginePrimitiveType {ENGINE_PRIMITIVE_TEXT=5,ENGINE_PRIMITIVE_LINE=6};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(EEnginePrimitiveType type,unsigned __int64,EEngineInterpolationSet interpolation_set):Type(type),InterpolationSet(interpolation_set){}};
class CEnginePrimitiveLine:public CEnginePrimitive {public:
 CEnginePrimitiveLine(unsigned __int64 id,const C3DVector& start,const C3DVector& end,const CRGBColour& colour,float width,EEnginePrimitiveBlendMode blend_mode,unsigned long bank_index,bool use_z,EEngineInterpolationSet interpolation_set,EEnginePrimitiveBlendOp blend_op)
 :CEnginePrimitive(ENGINE_PRIMITIVE_LINE,id,interpolation_set),Start(start),End(end),Colour(colour),Width(width),BankIndex(bank_index),BlendMode(blend_mode),BlendOp(blend_op),UseZBuffer(use_z){}
C3DVector Start,End;CRGBColour Colour;float Width;unsigned long BankIndex;EEnginePrimitiveBlendMode BlendMode;EEnginePrimitiveBlendOp BlendOp;bool UseZBuffer;};
class CEnginePrimitiveText:public CEnginePrimitive {public:
C3DVector Pos;float Scale;CRGBColour Colour;CWideString Text;const CFontBank*PFont;bool ScaleIn3D;
CEnginePrimitiveText(unsigned __int64,const C3DVector&,float,const CWideString&,const CFontBank&,const CRGBColour&,bool,EEngineInterpolationSet);
};
inline CEnginePrimitiveText::CEnginePrimitiveText(unsigned __int64 id,const C3DVector&pos,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,bool scale_in_3d,EEngineInterpolationSet interpolation_set):CEnginePrimitive((EEnginePrimitiveType)5,id,interpolation_set),Pos(pos),Scale(scale),Colour(colour),Text(text),PFont(&font),ScaleIn3D(scale_in_3d){}
}
static void TestOriginalConstructors(){
 for(int n=0;n<512;++n){C3DVector a(float(n)*0.25f,float(n-3)*0.5f,float(n+4)),b(float(7-n),float(n)*2.0f,float(n)*0.125f);CRGBColour c((unsigned char)n,(unsigned char)(n*3),(unsigned char)(n*7),(unsigned char)(n*11));CFontBank font;font.Number=n;CWideString text;text.Sprintf(L"%d",long(n));std::wstring saved=text.Value;float width=float(n+1)*0.25f;EEngineInterpolationSet set=EEngineInterpolationSet(n%3);EEnginePrimitiveBlendMode blend=EEnginePrimitiveBlendMode(n%6);EEnginePrimitiveBlendOp op=EEnginePrimitiveBlendOp(n%5);TestId id=(TestId(n)<<32)|TestId(0x87654321U);unsigned long bank=0x12340000UL+(unsigned)n;bool z=(n&1)!=0;
 OriginalConstructors::CEnginePrimitiveLine line(id,a,b,c,width,blend,bank,z,set,op);OriginalConstructors::CEnginePrimitiveText label(id,a,width,text,font,c,z,set);text.Value=L"changed";
 CHECK(line.Type==6&&line.InterpolationSet==set);CHECK(line.ThemeChannelDesc.ChannelIndex0==0&&line.ThemeChannelDesc.ChannelIndex1==0&&line.ThemeChannelDesc.ChannelBlendFactor==0);CHECK(line.Start.X==a.X&&line.Start.Y==a.Y&&line.Start.Z==a.Z&&line.End.X==b.X&&line.End.Y==b.Y&&line.End.Z==b.Z);CHECK(line.Colour.R==c.R&&line.Colour.G==c.G&&line.Colour.B==c.B&&line.Colour.A==c.A);CHECK(line.Width==width&&line.BlendMode==blend&&line.BankIndex==bank&&line.UseZBuffer==z&&line.BlendOp==op);
 CHECK(label.Type==5&&label.InterpolationSet==set);CHECK(label.ThemeChannelDesc.ChannelIndex0==0&&label.ThemeChannelDesc.ChannelIndex1==0&&label.ThemeChannelDesc.ChannelBlendFactor==0);CHECK(label.Pos.X==a.X&&label.Pos.Y==a.Y&&label.Pos.Z==a.Z);CHECK(label.Scale==width&&label.Colour.R==c.R&&label.Colour.G==c.G&&label.Colour.B==c.B&&label.Colour.A==c.A);CHECK(label.Text.Value==saved&&label.PFont==&font&&label.ScaleIn3D==z);
 }
}


static bool Vec(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static bool Col(const CRGBColour&a,const CRGBColour&b){return a.R==b.R&&a.G==b.G&&a.B==b.B&&a.A==b.A;}
static std::wstring CountLabel(int a,int b){char t[100];std::sprintf(t,"Points (Open/Hidden): %d / %d",a,b);std::wstring out;for(int i=0;t[i];++i)out+=wchar_t(t[i]);return out;}
static void Run(int op,int hi,int prior,int refs,int mode,bool mutate,bool enabled){
 ++gCase;PrimitiveData pool[100];for(int q=0;q<100;++q)pool[q].Refs=refs;
 CFontBank oldFont;oldFont.Number=41;CFontBank newFont;newFont.Number=73;
 CTCCreatureGenerator g;CEnginePrimitivePrepareInfo info;CIEngine e0,e1;std::vector<Record>records;
 e0.Owner=e1.Owner=&g;e0.Info=e1.Info=&info;e0.Other=e1.Other=&e1;e0.Records=e1.Records=&records;e0.Pool=e1.Pool=pool;e0.Mode=mode;e1.Mode=0;e0.Number=0;e1.Number=1;e1.Fired=true;info.Engine=&e0;
 g.Mutate=mutate;g.ChangedFont=&newFont;g.Component.PFont=&oldFont;
 C3DVector centre(float((gCase%13)-6)*0.25f,float((gCase%11)-5)*2.0f,float((gCase%7)-3)*0.5f);g.Parent.Pos=centre;TestId id=(TestId(0x98765432U)<<32)|TestId(0xabcdef00U+unsigned(gCase));g.Parent.Id=id;
 for(int i=0;i<op;++i)g.OpenPositions.push_back(C3DVector(float(i+1)*1.5f,float(i-4)*0.5f,float(i)*0.25f));
 for(int i=0;i<hi;++i)g.HiddenPositions.push_back(C3DVector(float(-i-1)*2.5f,float(i+2)*0.25f,float(i-1)*0.5f));
 const std::vector<C3DVector>open=g.OpenPositions,hidden=g.HiddenPositions;
 g.DebugPrimitives.resize(unsigned(prior));for(int i=0;i<prior;++i)if(i%3!=1)g.DebugPrimitives[i].Data=&pool[i];
 DrawCreatureGeneratorDebug=enabled;g.DrawDebugInfo(info);
 int needed=1+2*(op+hi);int expected=enabled?needed:0;
 CHECK(int(records.size())==expected);CHECK(g.Parent.PosReads==(enabled?1:0));CHECK(g.ComponentReads==(enabled?1:0));CHECK(g.Component.FontReads==(enabled?1:0));CHECK(g.Parent.IdReads==expected);
 CHECK(g.OpenPositions.size()==open.size()&&g.HiddenPositions.size()==hidden.size());for(int i=0;i<op;++i)CHECK(Vec(g.OpenPositions[i],open[i]));for(int i=0;i<hi;++i)CHECK(Vec(g.HiddenPositions[i],hidden[i]));
 if(!enabled){CHECK(g.DebugPrimitives.size()==unsigned(prior));CHECK(Vec(g.Parent.Pos,centre));CHECK(g.Parent.Id==id);CHECK(info.Engine==&e0);}
 else {
  TestId wantedId=id+(mutate?(TestId(1)<<32):0);const CFontBank*wantedFont=mutate?&newFont:&oldFont;
  CHECK(g.DebugPrimitives.size()==unsigned(needed+mode));CHECK(info.Engine==(mode==2?&e1:&e0));
  for(int n=0;n<expected;++n){const Record&r=records[n];CHECK(r.Index==unsigned(n));CHECK(r.Engine==((mode==2&&n>0)?1:0));const CEnginePrimitive&p=r.Primitive;CHECK(p.Id==wantedId);CHECK(p.Interpolation==0);
   if(n==0){CHECK(p.Type==5);CHECK(Vec(p.Start,C3DVector(centre.X,centre.Y,centre.Z+2.0f)));CHECK(p.Width==1.0f);CHECK(Col(p.Colour,CRGBColour(255,255,255)));CHECK(p.Text.Value==CountLabel(op,hi));CHECK(p.Font==wantedFont);CHECK(p.ScaleIn3D);}
   else {int v=(n-1)/2;bool top=((n-1)&1)!=0;bool isHidden=v>=op;const C3DVector&at=isHidden?hidden[v-op]:open[v];CHECK(p.Type==6);CHECK(Vec(p.Start,top?at:centre));CHECK(Vec(p.End,top?C3DVector(at.X,at.Y,at.Z+1.0f):at));CRGBColour c=isHidden?(top?CRGBColour(255,128,128):CRGBColour(255,0,0)):(top?CRGBColour(128,255,128):CRGBColour(0,255,0));CHECK(Col(p.Colour,c));CHECK(p.Width==0.05f&&p.Bank==0&&p.Z&&p.Blend==0&&p.Op==0);}
  }
  for(int i=needed;i<needed+mode;++i)CHECK(g.DebugPrimitives[i].Data==0);
 }
 for(int i=0;i<prior;++i){int removed=enabled&&i>=needed&&i%3!=1?1:0;CHECK(pool[i].Refs==refs-removed);CHECK(pool[i].Notifies==((removed&&refs==1)?1:0));if(i%3!=1&&(!enabled||i<needed))CHECK(g.DebugPrimitives[i].Data==&pool[i]);}
 for(int i=0;i<mode;++i){int removed=enabled?1:0;CHECK(pool[64+i].Refs==refs-removed);CHECK(pool[64+i].Notifies==((removed&&refs==1)?1:0));CHECK(pool[64+i].Releases==removed);}
}
int main(){int ps[]={0,1,2,7,17};int rs[]={1,2,4};for(int a=0;a<5;++a)for(int b=0;b<5;++b)for(int p=0;p<5;++p)for(int r=0;r<3;++r)for(int mode=0;mode<3;++mode)for(int mutate=0;mutate<2;++mutate)for(int enabled=0;enabled<2;++enabled)Run(a,b,ps[p],rs[r],mode,mutate!=0,enabled!=0);CHECK(gCase==4500);TestOriginalConstructors();std::puts("0074bc30_TEST PASS");return 0;}