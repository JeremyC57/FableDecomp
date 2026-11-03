// CEngineResourceManager::BuildAlphaStippleTexture(long,CArray<unsigned char>&) -- generates wrapped-density-ordered alpha dither texture
// Leaf: ?BuildAlphaStippleTexture@CEngineResourceManager@@AAEXJAAV?$CArray@E@@@Z
#define FABLE_RETAIL_ENGINE_COMPONENT
#define FABLE_RETAIL_STIPPLE
#include "engine/CEngineResourceManager.h"
#include "engine/C2DVector.h"
#include "engine/GFRandom.h"
#include <stdlib.h>
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
void CEngineResourceManager::BuildAlphaStippleTexture(long size,CArray<unsigned char>&buffer)
{
 CArray<long> point_order_list;
 for(long p=0;p<size*size;++p)point_order_list.push_back(p);
 unsigned long seed=0;
 for(long p=0;p<size*size;++p)std::swap(point_order_list[p],point_order_list[GFRandom(size*size,&seed)]);
 float max_density=0.0f;
 for(long sy=0;sy<size;++sy)for(long sx=0;sx<size;++sx)if(sx||sy){
  C2DVector dist((float)min(min(labs(-sx),labs(-sx+size)),labs(-sx-size)),(float)min(min(labs(-sy),labs(-sy+size)),labs(-sy-size)));
  max_density+=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());
 }
 CArray<bool>global_point_mask(point_order_list.size(),false);
 CArray<long>point_list;
 float density_step=max_density*0.125f;
 for(float density_threshold=density_step;density_threshold<max_density+density_step*2;density_threshold+=density_step){
  CArray<bool>section_point_mask(point_order_list.size(),false);
  for(long n=0;n<8;++n){
   for(long rp=0;rp<point_order_list.size();++rp){
    long src_point=point_order_list[rp];
    if(!global_point_mask[src_point]){
     long x=src_point%size,y=src_point/size;
     float total_density=0.0f;
     for(long p=0;p<global_point_mask.size();++p){
      if(src_point!=p&&(global_point_mask[p]!=false||section_point_mask[p]!=false)){
       long sx=p%size,sy=p/size;
       C2DVector dist((float)min(min(labs(x-sx),labs(x-sx+size)),labs(x-sx-size)),(float)min(min(labs(y-sy),labs(y-sy+size)),labs(y-sy-size)));
       float density=1.0/(dist.GetSquaredMagnitude()*dist.GetSquaredMagnitude());total_density+=density;
       if(total_density>density_threshold)break;
      }
     }
     if(total_density<density_threshold)section_point_mask[src_point]=true;else section_point_mask[src_point]=false;
    }
   }
  }
  for(long p=0;p<point_order_list.size();++p){long point=point_order_list[p];if(section_point_mask[point]){global_point_mask[point]=true;point_list.push_back(point);}}
 }
 buffer.resize(size*size);
 for(long p=0;p<point_list.size();++p){long point=point_list[p];long sx=point%size,sy=point/size;long alpha=min(p*255/point_list.size()+1,255UL);buffer[sy*size+sx]=(unsigned char)alpha;}
}
