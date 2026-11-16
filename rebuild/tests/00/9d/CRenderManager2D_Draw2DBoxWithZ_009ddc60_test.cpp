// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum _D3DRENDERSTATETYPE{};enum _D3DTEXTURESTAGESTATETYPE{};enum _D3DSAMPLERSTATETYPE{};enum EIndexBufferFormat{};enum EIndexBufferPool{};
#endif
#include "engine/EPlayerMode.h"
#define private public
#define protected public

#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_LIFETIME_PASS23
#define FABLE_RETAIL_CRENDERMANAGER2D_LIFETIME_PASS23
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPIXELFORMAT
#include "engine/EPrimitiveType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EVertexType.h"
// 009ddc60 CRenderManager2D::Draw2DBoxWithZ
// Leaf: ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z
// Flags: /d2inlT200
// Inline-budget: native expands SetRHWFromZ four times; ordinary inline defaults473B, T200 restores all four expansions and899B.
// Original GetTLX accessor keeps the native input home and converted-edge allocation.
#define FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DBOXWITHZ
#define FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ
#define FABLE_RETAIL_CGENERICVERTEX_ORIGINAL_RHW_PASS23
#define FABLE_RETAIL_CGENERICVERTEX_RHW_PLAIN_PASS23
#include "fable_compiler.h"
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CTVertexRHWColSpecTex1.h"
#include "engine/CRenderManager2D.h"
float FABLE_FASTCALL GFVXToAX(float x);           // 0x009E1E10
float FABLE_FASTCALL GFVYToAY(float y);           // 0x009E1E40

bool CRenderManager2D::Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags)
{
    long mode = 1;
    unsigned long argb = ((unsigned long)colour.GetA()<<24)|((unsigned long)colour.GetR()<<16)|((unsigned long)colour.GetG()<<8)|colour.GetB();
    float left = GFVXToAX(box.GetTLX());
    float right;
    float top = GFVYToAY(box.TLY);
    right = GFVXToAX(box.BRX);
    float bottom = GFVYToAY(box.BRY);
    CTVertexRHWColSpecTex1 v[4];
    v[0].X = left;
    v[0].Y = top;
    v[0].Z = z;
    v[0].DiffuseColour = argb;
    v[0].U1 = 0.0f;
    v[0].V1 = 0.0f;
    v[0].SetRHWFromZ();
    v[1].X = right;
    v[1].Y = top;
    v[1].Z = z;
    v[1].DiffuseColour = argb;
    v[1].U1 = 0.0f;
    v[1].V1 = 0.0f;
    v[1].SetRHWFromZ();
    v[2].X = right;
    v[2].Y = bottom;
    v[2].Z = z;
    v[2].DiffuseColour = argb;
    v[2].U1 = 0.0f;
    v[2].V1 = 0.0f;
    v[2].SetRHWFromZ();
    v[3].X = left;
    v[3].Y = bottom;
    v[3].Z = z;
    v[3].DiffuseColour = argb;
    v[3].U1 = 0.0f;
    v[3].V1 = 0.0f;
    v[3].SetRHWFromZ();
    if (flags & 1) {
        if (colour.A != 0xff)
            mode = 3;
    } else
        mode = (flags & 2) ? 2 : 1;
    Add2DTri(v[0], v[1], v[2], mode, 0);
    Add2DTri(v[0], v[2], v[3], mode, 0);
    return true;
}


#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/C3DVector.h"
#include "engine/CRGBFloatColour.h"


CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CSystemManager::CSystemManager(){}CSystemManager::~CSystemManager(){}
CBusyResourceManager::CBusyResourceManager():PendingFrame(0),FinishedFrame(0),D3DDevice(0){}CBusyResourceManager::~CBusyResourceManager(){assert(FreeQueoryObjects.empty()&&PendingQueories.empty());}
CSurface::~CSurface(){assert(!PD3DSurface);}
void CTexture::Uninitialise(){assert(!PD3DTexture);}
namespace boost {template<> scoped_ptr<CRenderManager>::~scoped_ptr(){assert(!ptr);}template<> scoped_ptr<CVsyncCallbackTimer>::~scoped_ptr(){assert(!ptr);}}
CDisplayManager::CDisplayManager(CSystemManager&s):SystemManager(s){}CDisplayManager::~CDisplayManager(){}
CRenderStateManager::CDeviceState::CDeviceState(){}CRenderStateManager::CStateInfo::CStateInfo(){}
CRenderStateManager::CRenderStateManager(unsigned long f):InitFlags(f){}CRenderStateManager::~CRenderStateManager(){}
CRenderManagerCore::CRenderManagerCore(CDisplayManager&d,unsigned long f):CRenderStateManager(f),DisplayManager(&d){}CRenderManagerCore::~CRenderManagerCore(){}
CRenderManager2D::CRenderManager2D(CDisplayManager&d,const CWideString&,unsigned long f):CRenderManagerCore(d,f){}CRenderManager2D::~CRenderManager2D(){}
const float BIGGEST_RHW_VERTEX_VALUE=1048576.0f;
static C2DBoxF ExpectedBox;static CRenderManager2D*ExpectedManager;static CRGBColour ExpectedColour;static unsigned long ExpectedFlags;static float ExpectedZ;static int Conversions,Triangles;
float FABLE_FASTCALL GFVXToAX(float x){assert(Conversions==0||Conversions==2);assert(x==(Conversions==0?ExpectedBox.TLX:ExpectedBox.BRX));++Conversions;return x*0.5f-8.0f;}
float FABLE_FASTCALL GFVYToAY(float y){assert(Conversions==1||Conversions==3);assert(y==(Conversions==1?ExpectedBox.TLY:ExpectedBox.BRY));++Conversions;return y*2.0f+4.0f;}
static bool SameFloat(float a,float b){return a==b||(a!=a&&b!=b);}
static void CheckVertex(const CTVertexRHWColSpecTex1&v,int corner){float x=corner==0||corner==3?ExpectedBox.TLX:ExpectedBox.BRX;float y=corner<2?ExpectedBox.TLY:ExpectedBox.BRY;assert(v.X==x*0.5f-8&&v.Y==y*2+4&&SameFloat(v.Z,ExpectedZ));float rhw;if(ExpectedZ>=0&&ExpectedZ<0.01f)rhw=BIGGEST_RHW_VERTEX_VALUE;else if(ExpectedZ>=-0.01f&&ExpectedZ<0)rhw=-BIGGEST_RHW_VERTEX_VALUE;else rhw=1.0f/ExpectedZ;assert(SameFloat(v.RHW,rhw)&&v.U1==0&&v.V1==0);unsigned long argb=((unsigned long)ExpectedColour.A<<24)|((unsigned long)ExpectedColour.R<<16)|((unsigned long)ExpectedColour.G<<8)|ExpectedColour.B;assert(v.DiffuseColour==argb);}
void CRenderManager2D::Add2DTri(const CTVertexRHWColSpecTex1&a,const CTVertexRHWColSpecTex1&b,const CTVertexRHWColSpecTex1&c,long mode,CTexture*t){assert(this==ExpectedManager&&!t&&Conversions==4&&Triangles<2);long expected=ExpectedFlags&1?(ExpectedColour.A==255?1:3):(ExpectedFlags&2?2:1);assert(mode==expected);CheckVertex(a,0);CheckVertex(b,Triangles==0?1:2);CheckVertex(c,Triangles==0?2:3);++Triangles;}
static float FloatBits(unsigned n){float f;memcpy(&f,&n,4);return f;}
int main(){CSystemManager system;CDisplayManager display(system);CWideString empty;CRenderManager2D manager(display,empty,0);ExpectedManager=&manager;unsigned zbits[]={0,0x80000000u,0x3ba3d70au,0xbba3d70au,0x3c23d70au,0xbc23d70au,0x3c23d709u,0xbc23d709u,0x3c23d70bu,0xbc23d70bu,0x3f800000u,0xbf800000u,0x40000000u,0x7f800000u,0xff800000u,0x7fc12345u};unsigned cases=0;
 for(int seed=0;seed<1024;++seed)for(int zi=0;zi<16;++zi){ExpectedBox.TLX=(float)(seed-512)*0.25f;ExpectedBox.TLY=(float)(seed%37-18)*0.5f;ExpectedBox.BRX=ExpectedBox.TLX+16.0f;ExpectedBox.BRY=ExpectedBox.TLY+8.0f;ExpectedColour.R=(unsigned char)seed;ExpectedColour.G=(unsigned char)(seed*3);ExpectedColour.B=(unsigned char)(seed*7);ExpectedColour.A=seed%3==0?0:seed%3==1?127:255;ExpectedFlags=(unsigned long)(seed%8);ExpectedZ=FloatBits(zbits[zi]);Conversions=Triangles=0;bool result=manager.Draw2DBoxWithZ(ExpectedBox,ExpectedZ,ExpectedColour,ExpectedFlags);assert(result&&Conversions==4&&Triangles==2);++cases;}
 assert(cases==16384);puts("009ddc60_TEST PASS");return 0;}

