// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_PRIMITIVE_ENUMS_H
#define FABLE_ENGINE_PRIMITIVE_ENUMS_H
enum EEnginePrimitiveType {
 ENGINE_PRIMITIVE_NULL=0,
 ENGINE_PRIMITIVE_GROUP=1,
 ENGINE_PRIMITIVE_CHANGER=2,
 ENGINE_PRIMITIVE_FILLED_RECT=3,
 ENGINE_PRIMITIVE_OUTLINE_RECT=4,
 ENGINE_PRIMITIVE_TEXT=5,
 ENGINE_PRIMITIVE_LINE=6,
 ENGINE_PRIMITIVE_SPRITE=7,
 ENGINE_PRIMITIVE_STATIC_MESH=8,
 ENGINE_PRIMITIVE_ANIMATING_MESH=9,
 ENGINE_PRIMITIVE_MORPHED_STATIC_MESH=10,
 ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH=11,
 ENGINE_PRIMITIVE_STATIC_MESH_FROM_FILE=12,
 ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE=13,
 ENGINE_PRIMITIVE_TARGET=14,
 ENGINE_PRIMITIVE_LIGHT=15,
 ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT=16,
 ENGINE_PRIMITIVE_FLARE_SPRITE=17,
 ENGINE_PRIMITIVE_GAME_MESH=18,
 ENGINE_PRIMITIVE_WEAPON_TRAIL=19,
 ENGINE_PRIMITIVE_SPRITE_TRAIL=20,
 ENGINE_PRIMITIVE_PARTICLE_LINE=21,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE=22,
 ENGINE_PRIMITIVE_PARTICLE_MESH=23,
 ENGINE_PRIMITIVE_PARTICLE_GROUP=24,
 ENGINE_PRIMITIVE_PARTICLE_DECAL=25,
 ENGINE_PRIMITIVE_PARTICLE_LINE_GROUP=26,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE_GROUP=27,
 ENGINE_PRIMITIVE_PARTICLE_MESH_GROUP=28,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE_TRAIL_GROUP=29,
 ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP=30,
 ENGINE_PRIMITIVE_WEATHER_MASK=31,
 ENGINE_PRIMITIVE_2D_PARTICLE_GROUP=32,
 ENGINE_PRIMITIVE_2D_VIEWPORT=33,
 ENGINE_PRIMITIVE_2D_SPRITE=34,
 ENGINE_PRIMITIVE_2D_CLOCK_SPRITE=35,
 ENGINE_PRIMITIVE_2D_STATIC_MESH=36,
 ENGINE_PRIMITIVE_2D_ANIMATING_MESH=37,
 ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH=38,
 ENGINE_PRIMITIVE_2D_TEXT=39,
 ENGINE_PRIMITIVE_DECAL=40,
 ENGINE_PRIMITIVE_MAX=41
};
enum EEngineInterpolationSet {
 ENGINE_INTERPOLATION_SET_0=0,
 ENGINE_INTERPOLATION_SET_1=1,
 ENGINE_INTERPOLATION_SET_2=2,
 ENGINE_INTERPOLATION_SET_NUM=3,
 ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0,
 ENGINE_INTERPOLATION_SET_PAUSED=1,
 ENGINE_INTERPOLATION_SET_BULLETTIME=2,
 ENGINE_INTERPOLATION_SET_DEFAULT=0
};
enum EEngineRenderLayer {
 RENDER_LAYER_NULL=0,
 RENDER_LAYER_SETUP_LIGHTS=1,
 RENDER_LAYER_SHADOW_BUFFER_RENDER=2,
 RENDER_LAYER_LANDSCAPE_BASE=4,
 RENDER_LAYER_MESHES_OCCLUSION=8,
 RENDER_LAYER_MESHES_NO_OCCLUSION=16,
 RENDER_LAYER_NON_SORTED_PRIMITIVES=32,
 RENDER_LAYER_LANDSCAPE_POST_SOLID_SCENE=64,
 RENDER_LAYER_OUTLINE_FILL=128,
 RENDER_LAYER_OUTLINE_STENCIL=256,
 RENDER_LAYER_OUTLINE_ALPHA_STENCIL_MASK=512,
 RENDER_LAYER_STENCIL_MESHES=1024,
 RENDER_LAYER_ALPHA_MESHES=2048,
 RENDER_LAYER_BOUNDING_BOXES=4096,
 RENDER_LAYER_BACKGROUND=8192,
 RENDER_LAYER_DECAL=16384,
 RENDER_LAYER_PREWATER_EFFECTS=32768,
 RENDER_LAYER_WATER_DISPLACEMENT=65536,
 RENDER_LAYER_WATER=131072,
 RENDER_LAYER_POSTWATER_MESHES=262144,
 RENDER_LAYER_POSTWATER_EFFECTS=524288,
 RENDER_LAYER_WATER_EFFECTS=1048576,
 RENDER_LAYER_EFFECTS=2097152,
 RENDER_LAYER_FLARE_EFFECTS=4194304,
 RENDER_LAYER_FLARE_VISIBILITY=8388608,
 RENDER_LAYER_COLOUR_FILTER=16777216,
 RENDER_LAYER_DEBUG=33554432,
 RENDER_LAYER_OUTLINE_APPLY_TO_SCREEN=67108864,
 RENDER_LAYER_DISPLACEMENT_BUFFER=134217728,
 RENDER_LAYER_DISPLACEMENT_STENCIL=268435456,
 RENDER_LAYER_SCREEN_EFFECTS=536870912,
 RENDER_LAYER_GRAPHICS_FRAME_END=1073741824,
 RENDER_LAYER_2D=-2147483648,
 RENDER_LAYER_MAX=-2147483647
};
#endif

#ifndef FABLE_ENGINE_CENGINERENDERENUMS_H
#define FABLE_ENGINE_CENGINERENDERENUMS_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_SOLID=0, ENGINE_PRIMITIVE_BLEND_BOOLEAN_ALPHA=1, ENGINE_PRIMITIVE_BLEND_ALPHA=2, ENGINE_PRIMITIVE_BLEND_ADDITIVE=3, ENGINE_PRIMITIVE_BLEND_ADDSMOOTH=4, ENGINE_PRIMITIVE_BLEND_CONST_COLOUR=5, ENGINE_PRIMITIVE_BLEND_COUNT=6 };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_ADD=0, ENGINE_PRIMITIVE_BLEND_OP_SUB=1, ENGINE_PRIMITIVE_BLEND_OP_REVSUB=2, ENGINE_PRIMITIVE_BLEND_OP_ADDSIGNED=3, ENGINE_PRIMITIVE_BLEND_OP_REVSUBSIGNED=4, ENGINE_PRIMITIVE_BLEND_OP_COUNT=5 };
enum ERendered { RENDERED_NOTHING=0, RENDERED_BOUNDING_VOLUME=1, RENDERED_OBJECT=2 };
enum ESubPrimitiveType { SUBPRIM_TYPE_OPAQUE=0, SUBPRIM_TYPE_ALPHA=1, SUBPRIM_TYPE_2D=2, SUBPRIM_TYPE_2D_VIEWPORT=3, SUBPRIM_TYPE_GLOW_BUFFER=4, SUBPRIM_TYPE_DISPLACEMENT_BUFFER=5, SUBPRIM_TYPE_DISPLACEMENT_STENCIL=6, SUBPRIM_TYPE_BOUNDING_VOLUME=7, SUBPRIM_TYPE_OUTLINE_STENCIL_SMOOTH=8, SUBPRIM_TYPE_OUTLINE_ALPHA_STENCIL_MASK_SMOOTH=9, SUBPRIM_TYPE_OUTLINE_STENCIL=10, SUBPRIM_TYPE_STENCIL_MESHES=11, SUBPRIM_TYPE_POSTWATER_MESHES=12, SUBPRIM_TYPE_OUTLINE_SMOOTH_FILL=13, SUBPRIM_TYPE_OUTLINE_FILL=14, SUBPRIM_TYPE_COUNT=15 };
enum ERenderPassType { RENDER_PASS_NULL=0, RENDER_PASS_Z_WRITE=1, RENDER_PASS_COMBINED=2, RENDER_PASS_PREWATER_EFFECT=3, RENDER_PASS_POSTWATER_EFFECT=4, RENDER_PASS_GLOW_BUFFER=5, RENDER_PASS_DISPLACEMENT_BUFFER=6, RENDER_PASS_DISPLACEMENT_STENCIL=7, RENDER_PASS_OUTLINE_SMOOTH_STENCIL=8, RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK=9, RENDER_PASS_OUTLINE_SMOOTH_FILL=10, RENDER_PASS_OUTLINE_INNER_ALPHA_MASK=11, RENDER_PASS_OUTLINE_STENCIL=12, RENDER_PASS_OUTLINE_FILL=13, RENDER_PASS_COUNT=14 };

#endif

#ifndef FABLE_ENGINE_RENDERSTATEENUMS_H
#define FABLE_ENGINE_RENDERSTATEENUMS_H


// The CRenderStateManager setter enums (Codex pass 13: Ego_r.pdb LF_ENUM field lists, evidence/009da9f0_Ego_r_final_enums.json),
// one model for every render row. The enumerator order is the order of the D3D tables the setters index (chunk 723b/723c read
// the same tables: TEXTURE_UV_MODE 0x01396DC8 {WRAP, MIRROR, CLAMP, BORDER}, POLY_CULL_MODE 0x01396FB0 {CCW 3, CW 2, NONE 1},
// ALPHA_BLEND_MODE 0x01396F68 {D3DBLEND_ZERO..}, ALPHA_TEST_FUNC 0x01396DD8 {D3DCMP_NEVER..}, BLEND_OP 0x01396ED0 {ADD..}).
enum POLY_CULL_MODE {
POLY_CULL_CCW=0,
POLY_CULL_CW=1,
POLY_CULL_NONE=2,
POLY_CULL_MODE_COUNT=3
};
enum POLY_FILL_MODE {
POLY_FILL_SOLID=0,
POLY_FILL_WIRE=1,
POLY_FILL_POINT=2,
POLY_FILL_MODE_COUNT=3
};
enum POLY_SHADE_MODE {
POLY_SHADE_FLAT=0,
POLY_SHADE_GOURAUD=1,
POLY_SHADE_MODE_COUNT=2
};
enum TEXTURE_FILTER_MODE {
TEXTURE_FILTER_POINT=0,
TEXTURE_FILTER_LINEAR=1,
TEXTURE_FILTER_ANISOTROPIC=2,
TEXTURE_FILTER_MODE_COUNT=3
};
enum TEXTURE_MIPMAP_MODE {
TEXTURE_MIPMAP_POINT=0,
TEXTURE_MIPMAP_LINEAR=1,
TEXTURE_MIPMAP_DISABLE=2,
TEXTURE_MIPMAP_MODE_COUNT=3
};
enum TEXTURE_UV_MODE {
TEXTURE_UV_WRAP=0,
TEXTURE_UV_MIRROR=1,
TEXTURE_UV_CLAMP=2,
TEXTURE_UV_BORDER=3,
TEXTURE_UV_MODE_COUNT=4
};
enum ZBUFFER_FUNC {
ZBUFFER_FUNC_NEVER=0,
ZBUFFER_FUNC_LESS=1,
ZBUFFER_FUNC_EQUAL=2,
ZBUFFER_FUNC_LESS_EQUAL=3,
ZBUFFER_FUNC_GREATER=4,
ZBUFFER_FUNC_NOT_EQUAL=5,
ZBUFFER_FUNC_GREATER_EQUAL=6,
ZBUFFER_FUNC_ALWAYS=7,
ZBUFFER_FUNC_COUNT=8
};
enum TEXTURE_OP_MODE {
TEXTURE_OP_DISABLE=0,
TEXTURE_OP_SELECT_ARG1=1,
TEXTURE_OP_SELECT_ARG2=2,
TEXTURE_OP_MODULATE=3,
TEXTURE_OP_MODULATE_2X=4,
TEXTURE_OP_MODULATE_4X=5,
TEXTURE_OP_ADD=6,
TEXTURE_OP_ADD_SIGNED=7,
TEXTURE_OP_ADD_SIGNED2X=8,
TEXTURE_OP_SUBTRACT=9,
TEXTURE_OP_ADD_SMOOTH=10,
TEXTURE_OP_BLEND_DIFFUSE_ALPHA=11,
TEXTURE_OP_BLEND_TEXTURE_ALPHA=12,
TEXTURE_OP_BLEND_FACTOR_ALPHA=13,
TEXTURE_OP_BLEND_TEXTURE_ALPHA_PM=14,
TEXTURE_OP_BLEND_CURRENT_ALPHA=15,
TEXTURE_OP_PREMODULATE=16,
TEXTURE_OP_MODULATE_ALPHA_ADD_COLOR=17,
TEXTURE_OP_MODULATE_COLOR_ADD_ALPHA=18,
TEXTURE_OP_MODULATE_INV_ALPHA_ADD_COLOR=19,
TEXTURE_OP_MODULATE_INV_COLOR_ADD_ALPHA=20,
TEXTURE_OP_BUMP_ENV_MAP=21,
TEXTURE_OP_BUMP_ENV_MAP_LUMINANCE=22,
TEXTURE_OP_DOT_PRODUCT3=23,
TEXTURE_OP_MODE_COUNT=24
};
enum TEXTURE_ARG_MODE {
TEXTURE_ARG_CURRENT=0,
TEXTURE_ARG_DIFFUSE=1,
TEXTURE_ARG_TEXTURE=2,
TEXTURE_ARG_TFACTOR=3,
TEXTURE_ARG_SPECULAR=4,
TEXTURE_ARG_COMPLEMENT_CURRENT=5,
TEXTURE_ARG_COMPLEMENT_DIFFUSE=6,
TEXTURE_ARG_COMPLEMENT_TEXTURE=7,
TEXTURE_ARG_COMPLEMENT_TFACTOR=8,
TEXTURE_ARG_COMPLEMENT_SPECULAR=9,
TEXTURE_ARG_ALPHA_REPLICATE_CURRENT=10,
TEXTURE_ARG_ALPHA_REPLICATE_DIFFUSE=11,
TEXTURE_ARG_ALPHA_REPLICATE_TEXTURE=12,
TEXTURE_ARG_ALPHA_REPLICATE_TFACTOR=13,
TEXTURE_ARG_ALPHA_REPLICATE_SPECULAR=14,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_CURRENT=15,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_DIFFUSE=16,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_TEXTURE=17,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_TFACTOR=18,
TEXTURE_ARG_ALPHA_REPLICATE_COMPLEMENT_SPECULAR=19,
TEXTURE_ARG_MODE_COUNT=20
};
enum STENCIL_OP_MODE {
STENCIL_OP_MODE_KEEP=0,
STENCIL_OP_MODE_ZERO=1,
STENCIL_OP_MODE_REPLACE=2,
STENCIL_OP_MODE_INCREMENT_SAT=3,
STENCIL_OP_MODE_DECREMENT_SAT=4,
STENCIL_OP_MODE_INVERT=5,
STENCIL_OP_MODE_INCREMENT=6,
STENCIL_OP_MODE_DECREMENT=7,
STENCIL_OP_MODE_COUNT=8
};
enum FOG_MODE {
FOG_MODE_NONE=0,
FOG_MODE_LINEAR=1,
FOG_MODE_EXP=2,
FOG_MODE_EXP2=3,
FOG_MODE_COUNT=4
};
enum ALPHA_BLEND_MODE {
ALPHA_BLEND_ZERO=0,
ALPHA_BLEND_ONE=1,
ALPHA_BLEND_SRC_COLOR=2,
ALPHA_BLEND_INV_SRC_COLOR=3,
ALPHA_BLEND_SRC_ALPHA=4,
ALPHA_BLEND_INV_SRC_ALPHA=5,
ALPHA_BLEND_DEST_ALPHA=6,
ALPHA_BLEND_INV_DEST_ALPHA=7,
ALPHA_BLEND_DEST_COLOR=8,
ALPHA_BLEND_INV_DEST_COLOR=9,
ALPHA_BLEND_SRC_ALPHA_SAT=10,
ALPHA_BLEND_BOTH_SRC_ALPHA=11,
ALPHA_BLEND_BOTH_INV_SRC_ALPHA=12,
ALPHA_BLEND_MODE_COUNT=13
};
enum ALPHA_TEST_FUNC {
ALPHA_TEST_NEVER=0,
ALPHA_TEST_LESS=1,
ALPHA_TEST_EQUAL=2,
ALPHA_TEST_LESS_EQUAL=3,
ALPHA_TEST_GREATER=4,
ALPHA_TEST_NOT_EQUAL=5,
ALPHA_TEST_GREATER_EQUAL=6,
ALPHA_TEST_ALWAYS=7,
ALPHA_TEST_MODE_COUNT=8
};
enum STENCIL_FUNC {
STENCIL_FUNC_NEVER=0,
STENCIL_FUNC_LESS=1,
STENCIL_FUNC_EQUAL=2,
STENCIL_FUNC_LESS_EQUAL=3,
STENCIL_FUNC_GREATER=4,
STENCIL_FUNC_NOT_EQUAL=5,
STENCIL_FUNC_GREATER_EQUAL=6,
STENCIL_FUNC_ALWAYS=7,
STENCIL_FUNC_COUNT=8
};
enum SHADOW_FUNC {
SHADOW_FUNC_NEVER=0,
SHADOW_FUNC_LESS=1,
SHADOW_FUNC_EQUAL=2,
SHADOW_FUNC_LESS_EQUAL=3,
SHADOW_FUNC_GREATER=4,
SHADOW_FUNC_NOT_EQUAL=5,
SHADOW_FUNC_GREATER_EQUAL=6,
SHADOW_FUNC_ALWAYS=7,
SHADOW_FUNC_COUNT=8
};
enum VERTEX_BLEND_MODE {
VERTEX_BLEND_DISABLE=0,
VERTEX_BLEND_1WEIGHT=1,
VERTEX_BLEND_2WEIGHTS=2,
VERTEX_BLEND_3WEIGHTS=3,
VERTEX_BLEND_MODE_COUNT=4
};
enum TEXTURE_TRANSFORM_MODE {
TEXTURE_TRANSFORM_DIABLED=0,
TEXTURE_TRANSFORM_COUNT1=1,
TEXTURE_TRANSFORM_COUNT2=2,
TEXTURE_TRANSFORM_COUNT3=3,
TEXTURE_TRANSFORM_COUNT4=4,
TEXTURE_TRANSFORM_PROJECTED=5,
TEXTURE_TRANSFORM_COUNT1_PROJECTED=6,
TEXTURE_TRANSFORM_COUNT2_PROJECTED=7,
TEXTURE_TRANSFORM_COUNT3_PROJECTED=8,
TEXTURE_TRANSFORM_COUNT4_PROJECTED=9,
TEXTURE_TRANSFORM_MODE_COUNT=10
};
enum MATERIAL_COLOUR_SOURCE {
MATERIAL_COLOUR_SOURCE_MATERIAL=0,
MATERIAL_COLOUR_SOURCE_COLOUR1=1,
MATERIAL_COLOUR_SOURCE_COLOUR2=2,
MATERIAL_COLOUR_SOURCE_COUNT=3
};
enum BLEND_OP {
BLEND_OP_ADD=0,
BLEND_OP_SUBTRACT=1,
BLEND_OP_REVSUBTRACT=2,
BLEND_OP_MIN=3,
BLEND_OP_MAX=4,
BLEND_OP_MODE_COUNT=5
};
enum MULTI_SAMPLE_MODE {
MULTI_SAMPLE_MODE_1X=0,
MULTI_SAMPLE_MODE_2X=1,
MULTI_SAMPLE_MODE_4X=2,
MULTI_SAMPLE_MODE_COUNT=3
};
enum TEXTURE_COLOUR_SIGN {
TEXTURE_COLOUR_SIGN_NONE=0,
TEXTURE_COLOUR_SIGN_R=1,
TEXTURE_COLOUR_SIGN_G=2,
TEXTURE_COLOUR_SIGN_B=3,
TEXTURE_COLOUR_SIGN_A=4,
TEXTURE_COLOUR_SIGN_RGB=5,
TEXTURE_COLOUR_SIGN_RGBA=6,
TEXTURE_COLOUR_SIGN_COUNT=7
};
#endif

#include <vector>
#include <list>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FABLE_FORCEINLINE inline
static unsigned long cases=0,uploads_total=0,draws_total=0;
static void check(bool b,int line){if(!b){std::printf("FAIL line%d case%lu\n",line,cases);std::exit(1);}}
#define CHECK(x) check((x),__LINE__)
static bool equal(float a,float b){return a==b||std::fabs(double(a)-double(b))<=0.000001*(1.0+std::fabs(double(a))+std::fabs(double(b)));}
static unsigned bits(float f){unsigned v;std::memcpy(&v,&f,4);return v;}
struct Event{int code,a,b,c,d;Event(int q,int w=0,int e=0,int r=0,int t=0):code(q),a(w),b(e),c(r),d(t){}};
static std::vector<Event> events;
static void event(int q,int w=0,int e=0,int r=0,int t=0){events.push_back(Event(q,w,e,r,t));}
struct C3DVector{float X,Y,Z;};
struct CMatrix3x4{float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;};
struct CRGBColour{unsigned char B,G,R,A;};
struct CRGBFloatColour{float R,G,B,A;explicit CRGBFloatColour(const CRGBColour&c):R(c.R*(1.0f/255.0f)),G(c.G*(1.0f/255.0f)),B(c.B*(1.0f/255.0f)),A(c.A*(1.0f/255.0f)){}};
struct CEngineParticleConstantBlock{float X,Y,Z,W;};
struct Upload{unsigned long index;long count;std::vector<float> values;};
static std::vector<Upload> uploads;
struct CVertexShader{int id;explicit CVertexShader(int i=0):id(i){}};
struct CPixelShader{int id;explicit CPixelShader(int i=0):id(i){}};
struct CVertexBufferWin32{int id;};struct CIndexBuffer{int id;};
struct CTexture{int id;};
struct FrameData{int refs,uninitialises,texture_releases,deletes;bool initialised;unsigned short TLX,TLY;CTexture texture;FrameData():refs(0),uninitialises(0),texture_releases(0),deletes(0),initialised(false),TLX(0),TLY(0){texture.id=39;}void Release(){CHECK(refs>0);if(--refs==0){if(initialised)++uninitialises;++texture_releases;++deletes;}}};
struct CGraphicFrame{FrameData*data;CGraphicFrame():data(0){}~CGraphicFrame(){if(data){data->Release();data=0;}}CTexture*GetTexture()const{return &data->texture;}float GetFloatTLX()const{return float(data->TLX)*(1.0f/32767.0f);}float GetFloatTLY()const{return float(data->TLY)*(1.0f/32767.0f);}};
struct CGraphicInfo{int id,mode;unsigned short width,height,fw,fh;FrameData data;CGraphicInfo():id(0),mode(0),width(64),height(32),fw(16),fh(8){}bool GetFrame(CGraphicFrame&f,unsigned long a,unsigned long b,unsigned long c)const{CHECK(a==0&&b==0&&c==0);CGraphicInfo*self=const_cast<CGraphicInfo*>(this);event(2,id);if(mode!=0){f.data=&self->data;++self->data.refs;}return mode>=2;}float GetFrameWidthFactor()const{return float(fw)/width;}float GetFrameHeightFactor()const{return float(fh)/height;}};
struct CGraphicDataBank{CGraphicInfo infos[3];const CGraphicInfo*PeekGraphic(unsigned long i)const{CHECK(i<3);return infos+i;}};
enum EGraphicBankNo{TEST_GRAPHIC_BANK=0};
struct CEngineResourceManager{CGraphicDataBank bank;const CGraphicDataBank*PeekGraphicBank(EGraphicBankNo i)const{CHECK(int(i)==0);return &bank;}};
struct CEngineInternalPrimitiveBase{virtual ~CEngineInternalPrimitiveBase(){}};
struct CEngineInternalPrimitiveParticleLine:CEngineInternalPrimitiveBase{C3DVector Start,End;float Width;CRGBColour Colour;};
struct CEnginePrimitiveHandle{CEngineInternalPrimitiveBase*p;explicit CEnginePrimitiveHandle(CEngineInternalPrimitiveBase*q=0):p(q){}CEngineInternalPrimitiveBase&GetData(){CHECK(p);return *p;}};
struct CEngineInternalPrimitiveParticleLineGroup:CEngineInternalPrimitiveBase{unsigned long BankIndex;unsigned short Flags;EEnginePrimitiveBlendMode BlendMode;EEnginePrimitiveBlendOp BlendOp;std::list<CEnginePrimitiveHandle>Primitives;};
struct CEngineSubPrimitiveBase{CEngineInternalPrimitiveBase*p;explicit CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase*q):p(q){}CEngineInternalPrimitiveBase*GetParentBase(){return p;}};
struct CEnginePrimitiveManager{virtual ~CEnginePrimitiveManager(){}};
struct CEnginePrimitiveManagerParticleGroup:CEnginePrimitiveManager{long lights;int lightCalls,pixelCalls;CEnginePrimitiveManagerParticleGroup():lights(0),lightCalls(0),pixelCalls(0){}long SetUpPointLights(){++lightCalls;event(10,int(lights));return lights;}void SetupPixelShader(unsigned long index,const CPixelShader&s,const CPixelShader&n){CHECK(index==0&&s.id==22&&n.id==21);++pixelCalls;event(11,int(index),s.id,n.id);}};
struct CEnginePrimitiveTypeInfo{CEnginePrimitiveManagerParticleGroup*p;CEnginePrimitiveManager*GetPrimitiveManager(long i)const{CHECK(i==24);return p;}};
struct CEngineCamera{CMatrix3x4 view,inverse;const CMatrix3x4&PeekNonHomogenousViewTransform()const{return view;}const CMatrix3x4&PeekInverseNonHomogenousViewTransform()const{return inverse;}};
struct CRenderManager{CTexture*texture;POLY_CULL_MODE cull;BLEND_OP blend;ALPHA_BLEND_MODE src,dst;void AttachTextureToStage(long n,const CTexture*t){CHECK(n==0&&t);texture=const_cast<CTexture*>(t);event(3,n,t->id);}void SetPolyCullMode(POLY_CULL_MODE m){cull=m;event(4,int(m));}void SetBlendOp(BLEND_OP m){blend=m;event(5,int(m));}void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE m){src=m;event(6,int(m));}void SetDestAlphaBlendMode(ALPHA_BLEND_MODE m){dst=m;event(7,int(m));}void SelectVertexBuffer(unsigned long n,CVertexBufferWin32*p){CHECK(n==0&&p->id==31);event(12);}void SelectIndexBuffer(CIndexBuffer*p,unsigned long n){CHECK(n==0&&p->id==32);event(13);}void DrawCurrentBuffersAsIndexedTris(unsigned long a,unsigned long b,unsigned long c,unsigned long d){CHECK(a==0&&c==0);event(16,int(b),int(d));++draws_total;}};
struct CShaderRenderManager{void ApplyVertexShader(const CVertexShader&s,bool f){CHECK(!f);event(8,s.id);}void ApplyPixelShader(const CPixelShader&s){event(9,s.id);}void SetUserVertexShaderConstants(unsigned long i,long n,float*p){CHECK(p&&n>=0&&n<=30);Upload u;u.index=i;u.count=n;u.values.assign(p,p+4*n);uploads.push_back(u);event(14,int(i),int(n));++uploads_total;}void PrepareForRender(unsigned long f){CHECK(f==0x11);event(15,int(f));}};
struct CEngineLightingManager{long percentage;bool faded;unsigned char channel[3];unsigned long updateFlags;CEngineLightingManager():percentage(19),faded(true),updateFlags(0){channel[0]=4;channel[1]=5;channel[2]=6;}void SetSelfIlluminationAmbient(long n){if(percentage!=n){percentage=n;updateFlags|=2;}event(17,int(n));}void UpdateGlobalLightingStates(){event(18,int(percentage));}void RestoreDefaultPrimitiveParameters(){event(19);faded=false;channel[0]=channel[1]=channel[2]=0;percentage=0;updateFlags|=0xf;}};
struct CEngineScreenEffectDisplacementRenderer{bool enabled;bool PeekRenderEnabled()const{return enabled;}};
struct CEnginePrimitiveRenderUtil{static void ApplyVertexShader(const CVertexShader&);static void ApplyPixelShader(const CPixelShader&);static CEngineCamera&GetCurrentCamera();void SetupBlendOpState(EEnginePrimitiveBlendOp);bool SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode);};
struct CEnginePrimitiveManagerParticleLineGroup:CEnginePrimitiveRenderUtil{std::vector<CVertexShader>VertexShadersLinePointLights;CVertexShader VertexShaderDisplacement;CPixelShader PixelShader,PixelShaderStipple,PixelShaderDisplacement;CVertexBufferWin32 VertexBuffer;CIndexBuffer IndexBuffer;CEngineParticleConstantBlock ConstantBlock[30];void ApplyLinePointLightVertexShader(unsigned long);void ApplyDisplacementVertexShader();void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);};
namespace NEngineInternal{CEngineResourceManager*GPResourceManager;CRenderManager*GPRenderManager;CShaderRenderManager*GPShaderRenderManager;CEnginePrimitiveTypeInfo*GPPrimitiveTypeInfo;CEngineCamera*GPCamera;CEngineLightingManager*GPLightingManager;CEngineScreenEffectDisplacementRenderer*GPScreenEffectDisplacementRenderer;}

using namespace NEngineInternal;
// Original TU-local S_LDATA32 int enable, Ego_r module934/segment3+22A3C; native0139B864.
static int enable = 1;
inline void CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& vshader)
{
    GPShaderRenderManager->ApplyVertexShader(vshader,false);
}
inline void CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& pshader)
{
    GPShaderRenderManager->ApplyPixelShader(pshader);
}
inline CEngineCamera& CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *GPCamera; }
inline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp blend_op)
{
    switch(blend_op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB: GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);break;
    default:GPRenderManager->SetBlendOp(BLEND_OP_ADD);break;
    }
}
// Inline-budget: genuine helper remains called after CPP variants, T64..4096 and scoped MS depth255; retail expands all states.
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode blend_mode)
{
    switch(blend_mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}
inline void CEnginePrimitiveManagerParticleLineGroup::ApplyLinePointLightVertexShader(unsigned long point_lights)
{
    if(point_lights>VertexShadersLinePointLights.size()-1)point_lights=VertexShadersLinePointLights.size()-1;
    CVertexShader& vshader=VertexShadersLinePointLights[point_lights];
    ApplyVertexShader(vshader);
}
inline void CEnginePrimitiveManagerParticleLineGroup::ApplyDisplacementVertexShader()
{
    ApplyVertexShader(VertexShaderDisplacement);
}
void CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& subprim,ERenderPassType render_pass)
{
    if(!enable)return;
    CEngineInternalPrimitiveParticleLineGroup& primitive=static_cast<CEngineInternalPrimitiveParticleLineGroup&>(*subprim.GetParentBase());
    const CGraphicInfo* graphic_info=GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(primitive.BankIndex);
    CGraphicFrame graphic;
    if(!graphic_info->GetFrame(graphic,0,0,0))return;
    GPRenderManager->AttachTextureToStage(0,graphic.GetTexture());
    GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    CEnginePrimitiveManagerParticleGroup* particle_group_prim_manager=static_cast<CEnginePrimitiveManagerParticleGroup*>(GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_PARTICLE_GROUP));
    bool colour_factor_is_alpha;
    if(render_pass==RENDER_PASS_DISPLACEMENT_BUFFER&&GPScreenEffectDisplacementRenderer->PeekRenderEnabled()) {
        colour_factor_is_alpha=false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else if(render_pass==RENDER_PASS_DISPLACEMENT_STENCIL) {
        colour_factor_is_alpha=false;
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(primitive.BlendOp);
        colour_factor_is_alpha=SetupAlphaBlendingStatesFromBlendMode(primitive.BlendMode);
        long point_light_count=0;
        if(primitive.Flags&0x80)point_light_count=particle_group_prim_manager->SetUpPointLights();
        else GPLightingManager->SetSelfIlluminationAmbient(100);
        GPLightingManager->UpdateGlobalLightingStates();
        ApplyLinePointLightVertexShader(point_light_count);
        particle_group_prim_manager->SetupPixelShader(0,PixelShaderStipple,PixelShader);
    }
    GPRenderManager->SelectVertexBuffer(0,&VertexBuffer);
    GPRenderManager->SelectIndexBuffer(&IndexBuffer,0);
    const CMatrix3x4& nh_view=GetCurrentCamera().PeekNonHomogenousViewTransform();
    CEngineParticleConstantBlock& constant_block0=ConstantBlock[0];
    constant_block0.X=nh_view.E11;constant_block0.Y=nh_view.E21;constant_block0.Z=nh_view.E31;constant_block0.W=nh_view.E41;
    CEngineParticleConstantBlock& constant_block1=ConstantBlock[1];
    constant_block1.X=nh_view.E12;constant_block1.Y=nh_view.E22;constant_block1.Z=nh_view.E32;constant_block1.W=nh_view.E42;
    CEngineParticleConstantBlock& constant_block2=ConstantBlock[2];
    constant_block2.X=nh_view.E13;constant_block2.Y=nh_view.E23;constant_block2.Z=nh_view.E33;constant_block2.W=nh_view.E43;
    const CMatrix3x4& inv_nh_view=GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    CEngineParticleConstantBlock& constant_block3=ConstantBlock[3];
    constant_block3.X=inv_nh_view.E11;constant_block3.Y=inv_nh_view.E21;constant_block3.Z=inv_nh_view.E31;constant_block3.W=inv_nh_view.E41;
    CEngineParticleConstantBlock& constant_block4=ConstantBlock[4];
    constant_block4.X=inv_nh_view.E12;constant_block4.Y=inv_nh_view.E22;constant_block4.Z=inv_nh_view.E32;constant_block4.W=inv_nh_view.E42;
    CEngineParticleConstantBlock& constant_block5=ConstantBlock[5];
    constant_block5.X=inv_nh_view.E13;constant_block5.Y=inv_nh_view.E23;constant_block5.Z=inv_nh_view.E33;constant_block5.W=inv_nh_view.E43;
    CEngineParticleConstantBlock& constant_block6=ConstantBlock[6];
    constant_block6.X=graphic.GetFloatTLX();constant_block6.Y=graphic.GetFloatTLY();
    constant_block6.Z=graphic_info->GetFrameWidthFactor();constant_block6.W=graphic_info->GetFrameHeightFactor();
    GPShaderRenderManager->SetUserVertexShaderConstants(1,7,&ConstantBlock[0].X);
    long current_user_constant_index=0;
    for(std::list<CEnginePrimitiveHandle>::iterator it=primitive.Primitives.begin();it!=primitive.Primitives.end();++it) {
        CEngineInternalPrimitiveParticleLine& child_prim=static_cast<CEngineInternalPrimitiveParticleLine&>(it->GetData());
        {
        CRGBFloatColour prim_colour(child_prim.Colour);
        prim_colour.A*=2.0f;
        if(colour_factor_is_alpha){float colour_factor=prim_colour.A;prim_colour.R*=colour_factor;prim_colour.G*=colour_factor;prim_colour.B*=colour_factor;}
        CEngineParticleConstantBlock& block0=ConstantBlock[current_user_constant_index++];
        C3DVector& start=child_prim.Start;
        block0.X=start.X;block0.Y=start.Y;block0.Z=start.Z;block0.W=child_prim.Width;
        CEngineParticleConstantBlock& block1=ConstantBlock[current_user_constant_index++];
        C3DVector& end=child_prim.End;
        block1.X=end.X;block1.Y=end.Y;block1.Z=end.Z;block1.W=1.0f;
        CEngineParticleConstantBlock& block2=ConstantBlock[current_user_constant_index++];
        block2.X=prim_colour.R;block2.Y=prim_colour.G;block2.Z=prim_colour.B;block2.W=prim_colour.A;
        if(current_user_constant_index==30){GPShaderRenderManager->SetUserVertexShaderConstants(8,30,&ConstantBlock[0].X);GPShaderRenderManager->PrepareForRender(0x11);GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,40,0,20);current_user_constant_index=0;}
        }
    }
    if(current_user_constant_index){long num_lines=current_user_constant_index/3UL;GPShaderRenderManager->SetUserVertexShaderConstants(8,num_lines*3,&ConstantBlock[0].X);GPShaderRenderManager->PrepareForRender(0x11);GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,num_lines*4,0,num_lines*2);}
    GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    GPLightingManager->RestoreDefaultPrimitiveParameters();
}


static void match_events(const std::vector<Event>&w){CHECK(events.size()==w.size());for(unsigned i=0;i<w.size();++i)CHECK(events[i].code==w[i].code&&events[i].a==w[i].a&&events[i].b==w[i].b&&events[i].c==w[i].c&&events[i].d==w[i].d);}
static void append_particle(std::vector<float>&v,const CEngineInternalPrimitiveParticleLine&l,bool alpha){v.push_back(l.Start.X);v.push_back(l.Start.Y);v.push_back(l.Start.Z);v.push_back(l.Width);v.push_back(l.End.X);v.push_back(l.End.Y);v.push_back(l.End.Z);v.push_back(1);float a=float(l.Colour.A)*(1.0f/255.0f)*2.0f;float r=float(l.Colour.R)*(1.0f/255.0f),g=float(l.Colour.G)*(1.0f/255.0f),b=float(l.Colour.B)*(1.0f/255.0f);if(alpha){r*=a;g*=a;b*=a;}v.push_back(r);v.push_back(g);v.push_back(b);v.push_back(a);}
static void run(int en,int frame_mode,int pass,bool disp,int blend,int op,bool lit,long lights,int shaders,int count,int seed){
 CEngineResourceManager resources;CRenderManager render;CShaderRenderManager shader;CEnginePrimitiveTypeInfo types;CEngineCamera camera;CEngineLightingManager lighting;CEngineScreenEffectDisplacementRenderer displacement;CEnginePrimitiveManagerParticleGroup group;CEnginePrimitiveManagerParticleLineGroup manager;CEngineInternalPrimitiveParticleLineGroup primitive;CEngineSubPrimitiveBase sub(&primitive);
 GPResourceManager=&resources;GPRenderManager=&render;GPShaderRenderManager=&shader;GPPrimitiveTypeInfo=&types;GPCamera=&camera;GPLightingManager=&lighting;GPScreenEffectDisplacementRenderer=&displacement;types.p=&group;displacement.enabled=disp;group.lights=lights;manager.VertexShaderDisplacement.id=20;manager.PixelShader.id=21;manager.PixelShaderStipple.id=22;manager.PixelShaderDisplacement.id=23;manager.VertexBuffer.id=31;manager.IndexBuffer.id=32;for(int i=0;i<shaders;++i)manager.VertexShadersLinePointLights.push_back(CVertexShader(100+i));for(int i=0;i<30;++i){manager.ConstantBlock[i].X=9000+i;manager.ConstantBlock[i].Y=9100+i;manager.ConstantBlock[i].Z=9200+i;manager.ConstantBlock[i].W=9300+i;}
 for(int i=0;i<3;++i){resources.bank.infos[i].id=i;resources.bank.infos[i].mode=frame_mode;resources.bank.infos[i].data.refs=frame_mode==3?1:0;resources.bank.infos[i].data.initialised=(seed&1)!=0;resources.bank.infos[i].data.TLX=(i&1)?32767:16384;resources.bank.infos[i].data.TLY=(i&1)?8192:0;resources.bank.infos[i].width=64*(i+1);resources.bank.infos[i].height=32*(i+1);resources.bank.infos[i].fw=16*(i+1);resources.bank.infos[i].fh=8*(i+1);}
 CGraphicInfo&info=resources.bank.infos[seed%3];primitive.BankIndex=seed%3;primitive.Flags=lit?0x80:0;primitive.BlendMode=EEnginePrimitiveBlendMode(blend);primitive.BlendOp=EEnginePrimitiveBlendOp(op);
 float*vm=&camera.view.E11;float*im=&camera.inverse.E11;for(int i=0;i<12;++i){vm[i]=float(i+1)*0.25f;im[i]=-float(i+3)*0.5f;}
 std::vector<CEngineInternalPrimitiveParticleLine>lines(count);for(int i=0;i<count;++i){CEngineInternalPrimitiveParticleLine&l=lines[i];l.Start.X=float(i+1);l.Start.Y=-float(i)*0.5f;l.Start.Z=float(i%5)*0.25f;l.End.X=float(i+3)*2;l.End.Y=-float(i+1);l.End.Z=float(i%7)*0.125f;l.Width=(i%3==0)?-0.5f:(i%3==1)?0.0f:2.0f;l.Colour.R=(unsigned char)((i*53+seed*37)&255);l.Colour.G=(unsigned char)((i*97+seed*13)&255);l.Colour.B=(unsigned char)((i*17+seed*71)&255);l.Colour.A=(unsigned char)((i*61+seed*19)&255);primitive.Primitives.push_back(CEnginePrimitiveHandle(&l));}
 std::vector<Event>w;bool success=en!=0&&frame_mode>=2;bool special=(pass==6&&disp)||pass==7;bool alpha=!special&&(blend==3||blend==4);if(en)w.push_back(Event(2,info.id));if(success){w.push_back(Event(3,0,39));w.push_back(Event(4,2));if(pass==6&&disp){w.push_back(Event(5,0));w.push_back(Event(6,4));w.push_back(Event(7,5));w.push_back(Event(8,20));w.push_back(Event(9,23));}else if(pass==7){w.push_back(Event(8,20));w.push_back(Event(9,23));}else{w.push_back(Event(5,op==1?1:op==2?2:0));w.push_back(Event(6,alpha?1:4));w.push_back(Event(7,blend==3?1:blend==4?3:5));unsigned long n=0;if(lit){w.push_back(Event(10,int(lights)));n=lights<0?0xffffffffUL:unsigned(lights);}else w.push_back(Event(17,100));w.push_back(Event(18,lit?19:100));if(n>unsigned(shaders-1))n=shaders-1;w.push_back(Event(8,100+int(n)));w.push_back(Event(11,0,22,21));}w.push_back(Event(12));w.push_back(Event(13));w.push_back(Event(14,1,7));for(int i=0;i<count/10;++i){w.push_back(Event(14,8,30));w.push_back(Event(15,17));w.push_back(Event(16,40,20));}if(count%10){w.push_back(Event(14,8,3*(count%10)));w.push_back(Event(15,17));w.push_back(Event(16,4*(count%10),2*(count%10)));}w.push_back(Event(4,0));w.push_back(Event(19));}
 events.clear();uploads.clear();enable=en;manager.RenderSubPrimitive(sub,ERenderPassType(pass));match_events(w);CHECK(int(primitive.Primitives.size())==count);CHECK(group.lightCalls==(success&&!special&&lit?1:0));CHECK(group.pixelCalls==(success&&!special?1:0));CHECK(info.data.refs==(frame_mode==3?1:0));int deleted=en&&frame_mode!=0&&frame_mode!=3?1:0;CHECK(info.data.deletes==deleted&&info.data.texture_releases==deleted&&info.data.uninitialises==(deleted&&(seed&1)?1:0));if(success){CHECK(!lighting.faded&&lighting.channel[0]==0&&lighting.channel[1]==0&&lighting.channel[2]==0&&lighting.percentage==0&&render.cull==POLY_CULL_CCW);CHECK(uploads.size()==1+unsigned((count+9)/10));const Upload&first=uploads[0];CHECK(first.index==1&&first.count==7);for(int m=0;m<2;++m)for(int c=0;c<3;++c)for(int r=0;r<4;++r)CHECK(first.values[m*12+c*4+r]==(m?im:vm)[r*3+c]);CHECK(equal(first.values[24],float(info.data.TLX)*(1.0f/32767.0f))&&equal(first.values[25],float(info.data.TLY)*(1.0f/32767.0f))&&first.values[26]==0.25f&&first.values[27]==0.25f);int line=0;for(unsigned u=1;u<uploads.size();++u){std::vector<float>expected;int n=count-line;if(n>10)n=10;for(int j=0;j<n;++j)append_particle(expected,lines[line++],alpha);CHECK(uploads[u].index==8&&uploads[u].count==3*n&&uploads[u].values.size()==expected.size());for(unsigned j=0;j<expected.size();++j)CHECK(equal(expected[j],uploads[u].values[j]));}CHECK(line==count);int changed=count>=10?30:count*3;if(changed<7)changed=7;for(int i=changed;i<30;++i)CHECK(manager.ConstantBlock[i].X==9000+i&&manager.ConstantBlock[i].Y==9100+i&&manager.ConstantBlock[i].Z==9200+i&&manager.ConstantBlock[i].W==9300+i);}else{CHECK(uploads.empty());for(int i=0;i<30;++i)CHECK(manager.ConstantBlock[i].X==9000+i&&manager.ConstantBlock[i].Y==9100+i&&manager.ConstantBlock[i].Z==9200+i&&manager.ConstantBlock[i].W==9300+i);}++cases;
}
int main(){int passes[]={0,2,6,7,17};int counts[]={0,1,9,10,11,20,21,37};int blends[]={-1,0,1,2,3,4,5,6};long lightcounts[]={-1,0,1,4,8};for(int p=0;p<5;++p)for(int d=0;d<2;++d)for(int b=0;b<8;++b)for(int o=-1;o<=4;++o)for(int l=0;l<2;++l)for(int n=0;n<8;++n)for(int a=0;a<5;++a)for(int sh=1;sh<=3;++sh){int seed=p+d+b+o+l+n+a+sh;run(1,2+(seed&1),passes[p],d!=0,blends[b],o,l!=0,lightcounts[a],sh,counts[n],seed);}for(int en=-1;en<=1;++en)for(int f=0;f<4;++f)for(int p=0;p<14;++p)for(int n=0;n<8;++n)run(en,f,p,(p&1)!=0,3,1,true,8,3,counts[n],p+n+f);CHECK(cases==116544 && uploads_total==318032 && draws_total==202384);std::puts("00bc3f3d_TEST PASS");return 0;}