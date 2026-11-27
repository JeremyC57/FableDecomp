#ifndef FABLE_ENGINE_CENGINETEXTUREMORPH_H
#define FABLE_ENGINE_CENGINETEXTUREMORPH_H
// New header (chunk 2018, CEngineSurfaceCompositionManager rows). Ego_r.pdb CEngineTextureMorph: OriginalTextureIndex +0x00,
// MorphID +0x04, Blends +0x08 (vector<CCompositeBlendDesc>; the retail STLport vector is 12 bytes, so sizeof 0x14 -- the node of
// GeneratedMorphs, map<pair<long,long>,CEngineTextureMorph>, is malloc(0x2C) at 0x00B2CBDD = 0x10 + 8 + 0x14; RenderToTexture
// 0x00B2B458 reads Blends at +0x08 / +0x0C with the 16-byte CCompositeBlendDesc stride). The landed ~CEngineSurfaceCompositionManager
// 0x00B2D1B0 models the same record locally.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CCompositeBlendDesc.h"
class CEngineTextureMorph {
public:
    long                             OriginalTextureIndex;   // +0x00
    long                             MorphID;                // +0x04
    std::vector<CCompositeBlendDesc> Blends;                 // +0x08
#ifdef FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS
    // hand4f opt-in (2026-11-19, row 0x006CA8C0 CEngineTextureMorphSet::AddEngineMorph): Ego_d.pdb CEngineTextureMorph (field list 0xB9D4)
    // lists three constructors: the compiler-generated copy, the user (long, long) one -- FableWin.pdb ??0CEngineTextureMorph@@QAE@JJ@Z 0x01E5F690
    // (tc_appearance_base.obj #4, 44 bytes /Od; Ego_d original_texture, morph_id; retail inlines it into AddEngineMorph; the init list is byte-required) --
    // and the user default one, FableWin ??0CEngineTextureMorph@@QAE@XZ 0x02D87870 (engine_surface_composition_manager.obj #2, 44 bytes /Od; body unread: declaration only).
    CEngineTextureMorph();
    CEngineTextureMorph(long original_texture, long morph_id) : OriginalTextureIndex(original_texture), MorphID(morph_id) { }
#endif
};
FABLE_STATIC_ASSERT_AT(17, sizeof(CEngineTextureMorph) == 0x14);
#ifdef FABLE_RETAIL_CENGINETEXTUREMORPHSET
// NF62 opt-in (2026-11-17, rows 0x00722270 / 0x007222B0, the CTCAppearanceMorphBase::CMeshMorphSet destructors): Ego_r.pdb CEngineTextureMorphSet
// (one member, CArray<CEngineTextureMorph> Morphs +0; retail 12 bytes) -- no header of its own. Its implicit destructor destroys Morphs, whose
// ~vector<CEngineTextureMorph> retail calls out of line at 0x005BC0D3 (FableWin.pdb ??1?$vector@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@XZ,
// `lea ecx,[esi+0Ch] ; call 0x005BC0D3`): declared here as an explicit specialisation, so only this instantiation leaves the TU and the other
// vectors of the includer keep the inline STLport destructor. Needs engine/CArray.h.
#include "engine/CArray.h"
#ifndef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
template<> std::vector<CEngineTextureMorph,std::allocator<CEngineTextureMorph> >::~vector();
#endif   // 0x005BC0D3
class CEngineTextureMorphSet {
public:
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
    const CArray<CEngineTextureMorph>& PeekEngineMorphs() const { return Morphs; }
#endif
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
protected:
#endif
    CArray<CEngineTextureMorph> Morphs;
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
public:
#endif                       // +0x00
#ifdef FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDTEXTUREMORPH_PASS26
    void AddTextureMorph(long original_texture,const CCompositeBlendDesc& blend);
#endif
#ifdef FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDENGINEMORPH
    // hand4f opt-in (2026-11-19, row 0x006CA8C0): Ego_r.pdb / FableWin.pdb ?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z
    // (protected; Ego_d field list 0x270FB; out of line in retail at 0x006CA8C0, called by AddTextureMorph 0x006CA950). Needs
    // FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS for the body.
protected:
    CEngineTextureMorph* AddEngineMorph(long original_texture);
public:
#endif
};
FABLE_STATIC_ASSERT_AT(30, sizeof(CEngineTextureMorphSet) == 0xC);
#endif
#ifdef FABLE_RETAIL_HERO_MODIFIER_DRAW_PASS29
// Retail destruction owners 005bbbce, 005bbf80 and 005bc0d3 are size-compiled.
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
namespace std {
template<> inline void _Destroy(CEngineTextureMorph* p) { p->~CEngineTextureMorph(); }
template<> inline void FABLE_FASTCALL __destroy_aux(CEngineTextureMorph* first,CEngineTextureMorph* last,const __false_type&) {for(;first!=last;++first) _Destroy(&*first);}
template<> inline vector<CEngineTextureMorph>::~vector(){_Destroy(this->_M_start,this->_M_finish);}
}
#if defined(_MSC_VER)
#pragma optimize("t",on)
#endif
#endif
#endif // FABLE_ENGINE_CENGINETEXTUREMORPH_H
