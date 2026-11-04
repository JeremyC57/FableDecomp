// StaticInitialization::Initialize_CUnprovenGlobal_013CA8A8() -- construct the original static object and register its destructor.
// Leaf: _$E1
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven original CVertexShaderConstantLayoutEmpty from own vtable/base chain and constructor, global013CA8A8 ->original destructor callback. Compiler-generated initializer performs base construction, genuine derived vtable store and atexit registration. Original global spelling unproven, descriptive name retained.
// Layout: true original polymorphic hierarchy and typed named fields, PCsize332. No opaque base/vptr. Original base constructors/destructors external; derived implicit destructor is genuine. CInkeyBuffer CCharEvent4 donor wchar_t Char/boolPressed has no provable class key/qualified scope; explicit CUnprovenCharEvent projection preserves only verified fields/size without claiming original V/U identity. For shader layout, typed range12 and genuine STLport CArray/vector12 reconcile donor336->retail332; CCharString non-POD by-value PeekName ABI retained. Available shader header aliased. No handwritten initializer/callback wrapper.
#include <stddef.h>
#define CVertexShaderConstantLayout CVertexShaderConstantLayout_HeaderEvidence
#include "engine/CVertexShaderConstantLayout.h"
#undef CVertexShaderConstantLayout
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CArray.h"
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();};class CShaderConstantRange:public CBaseClass{public:long Offset;unsigned long Count;};class CVertexShaderConstant;
class CVertexShaderConstantLayout:public CBaseClass{public:CShaderConstantRange Reserved,User,Compression,Ambient,FogColour,CameraPos;unsigned long LightSize;CShaderConstantRange LightArray,LightGlobals,LightAttenuations,CombinedProjectionMatrix,FogTransform,WorldMatrix,SphereMapMatrix;CShaderConstantRange TextureTransform[4];CShaderConstantRange BoneMatrices,ShadowTextureTransform,ShadowFadeTransform,ShadowFadeFactors,ShadowedSpotlightFalloffTransform,ShadowedSpotlightAttenuation,ShadowedSpotlightColour,ScreenSpaceTransformScale,ScreenSpaceTransformOffset;CArray<CVertexShaderConstant> ReservedPresets;CVertexShaderConstantLayout();virtual ~CVertexShaderConstantLayout();virtual CCharString PeekName()const;};class CVertexShaderConstantLayoutEmpty:public CVertexShaderConstantLayout{public:CVertexShaderConstantLayoutEmpty(){}virtual CCharString PeekName()const;};
CVertexShaderConstantLayoutEmpty CUnprovenGlobal_013CA8A8;
#ifdef _MSC_VER
typedef char OwnerSize[sizeof(CVertexShaderConstantLayoutEmpty)==332?1:-1];
#endif