// CRandomAppearanceMorph::GetRandomBodyParts(unsigned long&, CMorphSet&, bool) const -- choose body meshes and their texture morphs.
// Grade: DIFFER(294v294) -- empty branch precedes three zero stores; original-lifetime form292 bytes materialises element address before folded loads
// Retail: 0x0086BE40, 294 bytes; thiscall.
// Leaf: ?GetRandomBodyParts@CRandomAppearanceMorph@@IBEXAAKAAVCMorphSet@1@_N@Z
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CRandomAppearanceMorph.h"
#include "engine/GFRandom.h"
void CRandomAppearanceMorph::GetRandomBodyParts(unsigned long& seed,CMorphSet& morph_set,bool ignore_head) const
{
 unsigned long texture_group_id=0;
 if(MaxTextureGroupID)
  texture_group_id=GFRandom(MaxTextureGroupID,&seed)+1;
 for(long cbody_part_type=0;cbody_part_type!=3;++cbody_part_type) {
  if(!BodyParts[cbody_part_type].empty()) {
   CMorphSet::CMeshTextureSet mesh_texture_set;
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
