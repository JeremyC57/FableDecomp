// CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed, CMorphSet& morph_set, bool ignore_head) const -- pick a random texture group, then for each of the three body-part types with meshes pick a random mesh and its random texture morphs (the mesh's own and the type's), and add the set to the morph set (the head is skipped when ignore_head)
// Leaf: ?GetRandomBodyParts@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086BE40, 294 bytes, thiscall, `ret 0Ch`.
// Identity: Ego_r.pdb / FableWin.pdb (protected const); Ego_d.pdb random_appearance_morph.obj record (locals seed, morph_set, ignore_head;
//   texture_group_id; cbody_part_type; mesh_texture_set; body_part, body_part_index -- spelled here in the same scopes).
// Callees: GFRandom (engine/GFRandom.h inline, GFROR13 0x00497890), CRandomAppearanceMorph::GetRandomTextureMorphs 0x0086B810 (same TU,
//   body below), vector<CMorphSet::CMeshTextureSet>::push_back 0x0086E7C0, free 0x00BFEA14 (the set's TextureMorphs destructor).
// Shape: the empty test is `!BodyParts[cbody_part_type].empty()` (no begin / end locals: Ego_d has none). Retail loads the element's
//   begin / end BEFORE mesh_texture_set's three zero stores (0x0086BE87..0x0086BE9A): VC7.1 does that only when it can see that no
//   callee keeps the set's address -- the same-TU body of GetRandomTextureMorphs (defined after the row, as in Ego_d's object order)
//   plus the visible-but-out-of-line STLport push_back (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE; retail calls push_back<CMeshTextureSet>
//   0x0086E7C0). Either one alone leaves the stores first.
// Same-TU helper note: under the TU-wide PUSHBACK_NOINLINE the helper body below calls push_back<CMorphSetTextureMorph> where retail's
//   0x0086B810 expands it (the helper is not graded here; it is the landed row 0x0086B810's logic with Ego_d's names and scopes).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVECTORMAP_UPPERBOUND
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TEXTUREMORPH_CTOR
#include "engine/CRandomAppearanceMorph.h"
#include "engine/GFRandom.h"
void CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed,CMorphSet& morph_set,bool ignore_head) const
{
 unsigned long texture_group_id=0;
 if(MaxTextureGroupID)
  texture_group_id=GFRandom(MaxTextureGroupID,&seed)+1;
 for(long cbody_part_type=0;cbody_part_type!=3;++cbody_part_type) {
  CMorphSet::CMeshTextureSet mesh_texture_set;
  if(!BodyParts[cbody_part_type].empty()) {
   long body_part_index=GFRandom(BodyParts[cbody_part_type].size(),&seed);
   const CBodyPartMesh& body_part=BodyParts[cbody_part_type][body_part_index];
   mesh_texture_set.MeshIndex=body_part.MeshIndex;
   GetRandomTextureMorphs(seed,texture_group_id,body_part.TextureMorphs,mesh_texture_set.TextureMorphs);
   GetRandomTextureMorphs(seed,texture_group_id,BodyPartTextureMorphs[cbody_part_type],mesh_texture_set.TextureMorphs);
   if(!(ignore_head && cbody_part_type==0))
    morph_set.BodyParts.push_back(mesh_texture_set);
  }
 }
}

// Same-TU body (Ego_d random_appearance_morph.obj lists GetRandomTextureMorphs right after this row; landed 0x0086B810), defined
// after the row: visible, VC7.1 knows the callee keeps no address of mesh_texture_set.TextureMorphs.
void CRandomAppearanceMorph::GetRandomTextureMorphs(unsigned long& seed,unsigned long texture_group_id,const TTextureMorphMap& texture_morphs,CArray<CMorphSet::CMorphSetTextureMorph>& morph_set) const
{
 TTextureMorphMap::const_iterator upper_it;
 TTextureMorphMap::const_iterator lower_it=texture_morphs.begin();
 while(lower_it!=texture_morphs.end()) {
  upper_it=texture_morphs.UpperBound(lower_it->first);
  unsigned long no_valid_textures=0;
  for(TTextureMorphMap::const_iterator it=lower_it;it!=upper_it;++it) {
   if(it->second.GroupID==0||it->second.GroupID==texture_group_id)
    ++no_valid_textures;
  }
  if(no_valid_textures) {
   unsigned long wanted_n=GFRandom(no_valid_textures,&seed);
   unsigned long current_n=0;
   for(TTextureMorphMap::const_iterator it=lower_it;it!=upper_it;++it) {
    if(it->second.GroupID==0||it->second.GroupID==texture_group_id) {
     if(current_n==wanted_n) {
      CMorphSet::CMorphSetTextureMorph entry(it->first,it->second.NewTexture);
      morph_set.push_back(entry);
      break;
     }
     ++current_n;
    }
   }
  }
  lower_it=upper_it;
 }
}
