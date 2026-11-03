// C3DMeshPartitionBuilderElement::PartitionSpace(const C3DVertex2*, long, float) -- recursively partition triangle groups
// Leaf: ?PartitionSpace@C3DMeshPartitionBuilderElement@@AAEJPBVC3DVertex2@@JM@Z
// Flags: /Gr
#define FABLE_RETAIL_MESH_PARTITION
#define FABLE_RETAIL_CARRAY_GEOMETRY
#define FABLE_RETAIL_VECTOR_GEOMETRY
#define FABLE_RETAIL_VECTOR_COPY_INITIALISE
#define FABLE_RETAIL_VECTOR_SPLINE_CTOR
#include "engine/C3DMeshPartitionBuilderElement.h"
long C3DMeshPartitionBuilderElement::PartitionSpace(const C3DVertex2*verts,long start_tri_index,float parent_radius)
{
 StartTriangle=start_tri_index;
 delete SubElements[0];delete SubElements[1];SubElements[0]=0;SubElements[1]=0;
 CArray<C3DVector>section_verts;CArray<bool>added_mask;
 for(long tri=0;tri<Triangles.size();++tri)for(long id=0;id<3;++id){long vert_id=Triangles[tri].GetVertexIndex(id);if(vert_id>=added_mask.size())added_mask.resize(vert_id+1,false);if(!added_mask[vert_id]){added_mask[vert_id]=true;section_verts.push_back(verts[vert_id].GetPosition());}}
 BoundingBox.BuildFromPoints(section_verts);BoundingSphere.AccurateBuildFromPoints(section_verts,0.001f);
 if(Triangles.size()==1){TriangleCount=1;return 1;}
 float best_axis_child_size=1e30f;long best_axis=-1;
 CArray<C3DTriangle2>left_tri_list[3];CArray<C3DTriangle2>right_tri_list[3];
 for(long axis=0;axis<3;++axis){
  C3DVector left_box_min(1e30f,1e30f,1e30f),left_box_max(-1e30f,-1e30f,-1e30f),right_box_min(1e30f,1e30f,1e30f),right_box_max(-1e30f,-1e30f,-1e30f);
  for(long t=0;t<Triangles.size();++t){
   C3DTriangle2&triangle=Triangles[t];C3DVector vert0=verts[triangle.GetVertexIndex(0)].GetPosition(),vert1=verts[triangle.GetVertexIndex(1)].GetPosition(),vert2=verts[triangle.GetVertexIndex(2)].GetPosition();
   C3DVector tri_min(((vert0.X<vert1.X?vert0.X:vert1.X)<vert2.X?(vert0.X<vert1.X?vert0.X:vert1.X):vert2.X),((vert0.Y<vert1.Y?vert0.Y:vert1.Y)<vert2.Y?(vert0.Y<vert1.Y?vert0.Y:vert1.Y):vert2.Y),((vert0.Z<vert1.Z?vert0.Z:vert1.Z)<vert2.Z?(vert0.Z<vert1.Z?vert0.Z:vert1.Z):vert2.Z)),tri_max(((vert0.X>vert1.X?vert0.X:vert1.X)>vert2.X?(vert0.X>vert1.X?vert0.X:vert1.X):vert2.X),((vert0.Y>vert1.Y?vert0.Y:vert1.Y)>vert2.Y?(vert0.Y>vert1.Y?vert0.Y:vert1.Y):vert2.Y),((vert0.Z>vert1.Z?vert0.Z:vert1.Z)>vert2.Z?(vert0.Z>vert1.Z?vert0.Z:vert1.Z):vert2.Z));
   C3DVector tri_center((tri_min.X+tri_max.X)*0.5f,(tri_min.Y+tri_max.Y)*0.5f,(tri_min.Z+tri_max.Z)*0.5f);
   bool on_right=axis==0?tri_center.X>BoundingBox.GetCenter().X:axis==1?tri_center.Y>BoundingBox.GetCenter().Y:tri_center.Z>BoundingBox.GetCenter().Z;
   C3DVector*box_min;C3DVector*box_max;
   if(on_right){box_min=&right_box_min;box_max=&right_box_max;right_tri_list[axis].push_back(triangle);}else{box_min=&left_box_min;box_max=&left_box_max;left_tri_list[axis].push_back(triangle);}
   box_min->LimitUpper(tri_min);box_max->LimitLower(tri_max);
  }
  if(left_tri_list[axis].size()&&right_tri_list[axis].size()){
   float size_left=left_box_max.X-left_box_min.X+left_box_max.Y-left_box_min.Y+left_box_max.Z-left_box_min.Z;
   float size_right=right_box_max.X-right_box_min.X+right_box_max.Y-right_box_min.Y+right_box_max.Z-right_box_min.Z;
   float max_child_size=size_left>size_right?size_left:size_right;
   if(max_child_size<best_axis_child_size){best_axis_child_size=max_child_size;best_axis=axis;}
  }
 }
 if(best_axis<0){TriangleCount=Triangles.size();return TriangleCount;}
 SubElements[0]=new C3DMeshPartitionBuilderElement;SubElements[1]=new C3DMeshPartitionBuilderElement;
 for(long n=0;n<left_tri_list[best_axis].size();++n)SubElements[0]->Triangles.push_back(left_tri_list[best_axis][n]);
 for(long n=0;n<right_tri_list[best_axis].size();++n)SubElements[1]->Triangles.push_back(right_tri_list[best_axis][n]);
 Triangles.clear();
 long count0=SubElements[0]->PartitionSpace(verts,start_tri_index,BoundingSphere.GetRadius());
 long count1=SubElements[1]->PartitionSpace(verts,start_tri_index+count0,BoundingSphere.GetRadius());
 TriangleCount=count0+count1;return TriangleCount;
}
