#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CTVertexPos.h"
static int sequence;
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
struct VertexBufferDouble{CTVertexPos data[8];int initialised,locked,unlocked;VertexBufferDouble():initialised(0),locked(0),unlocked(0){for(int i=0;i<8;++i)data[i].X=data[i].Y=data[i].Z=-9;}bool Initialise(EVertexType type,long count,long stride,unsigned long usage,EVertexBufferPool pool){check(type==VERTEX_POS&&count==8&&stride==12&&usage==0&&pool==VB_POOL_MANAGED,"vertex allocation contract");initialised=++sequence;return true;}unsigned char*Lock(unsigned long a,unsigned long b,unsigned long c){check(a==0&&b==0&&c==0&&initialised,"vertex lock contract");locked=++sequence;return(unsigned char*)data;}void Unlock(){check(locked&&!unlocked,"vertex unlock contract");unlocked=++sequence;}};
struct IndexBufferDouble{unsigned short data[36];int initialised,locked,unlocked;IndexBufferDouble():initialised(0),locked(0),unlocked(0){for(int i=0;i<36;++i)data[i]=99;}void Initialise(unsigned long size,unsigned long usage,EIndexBufferFormat format,EIndexBufferPool pool,EPrimitiveType type){check(size==72&&usage==0&&format==IB_FORMAT_16&&pool==IB_POOL_MANAGED&&type==PT_TRIANGLE_LIST,"index allocation contract");initialised=++sequence;}unsigned char*Lock(unsigned long a,unsigned long b,unsigned long c){check(a==0&&b==0&&c==0&&initialised,"index lock contract");locked=++sequence;return(unsigned char*)data;}void Unlock(){check(locked&&!unlocked,"index unlock contract");unlocked=++sequence;}};
struct CEngineOcclusionManager{VertexBufferDouble UnitBoxVB;IndexBufferDouble UnitBoxIB;void BuildUnitBox();};

void CEngineOcclusionManager::BuildUnitBox()
{
 UnitBoxVB.Initialise(VERTEX_POS,8,sizeof(CTVertexPos),0,VB_POOL_MANAGED);
 CTVertexPos* buffer_verts=(CTVertexPos*)UnitBoxVB.Lock(0,0,0);
 for(long z=0;z<2;++z) {
  for(long y=0;y<2;++y) {
   for(long x=0;x<2;++x) {
    CTVertexPos& vert=buffer_verts[x+2*(y+z*2)];
    vert.X=(float)x;vert.Y=(float)y;vert.Z=(float)z;
   }
  }
 }
 UnitBoxVB.Unlock();
 UnitBoxIB.Initialise(72,0,IB_FORMAT_16,IB_POOL_MANAGED,PT_TRIANGLE_LIST);
 unsigned short ids[36]={2,0,1,2,1,3,4,6,5,7,5,6,1,0,4,1,4,5,2,3,6,7,6,3,0,2,6,6,4,0,3,1,7,1,5,7};
 unsigned short* buffer_ids=(unsigned short*)UnitBoxIB.Lock(0,0,0);
 for(long n=0;n<36;++n)buffer_ids[n]=ids[n];
 UnitBoxIB.Unlock();
}
int main(){CEngineOcclusionManager m;m.BuildUnitBox();check(m.UnitBoxVB.initialised==1&&m.UnitBoxVB.locked==2&&m.UnitBoxVB.unlocked==3&&m.UnitBoxIB.initialised==4&&m.UnitBoxIB.locked==5&&m.UnitBoxIB.unlocked==6,"buffer operation order");
 for(int i=0;i<8;++i){const CTVertexPos&v=m.UnitBoxVB.data[i];check(v.X==(float)(i&1)&&v.Y==(float)((i>>1)&1)&&v.Z==(float)((i>>2)&1),"all eight binary corners and vertex ordering");}
 int faces[6]={0,0,0,0,0,0};
 for(int t=0;t<12;++t){unsigned short a=m.UnitBoxIB.data[3*t],b=m.UnitBoxIB.data[3*t+1],c=m.UnitBoxIB.data[3*t+2];check(a<8&&b<8&&c<8&&a!=b&&b!=c&&a!=c,"triangle indices in range and distinct");const CTVertexPos&p=m.UnitBoxVB.data[a];const CTVertexPos&q=m.UnitBoxVB.data[b];const CTVertexPos&r=m.UnitBoxVB.data[c];float ux=q.X-p.X,uy=q.Y-p.Y,uz=q.Z-p.Z,vx=r.X-p.X,vy=r.Y-p.Y,vz=r.Z-p.Z;float nx=uy*vz-uz*vy,ny=uz*vx-ux*vz,nz=ux*vy-uy*vx;check(nx*nx+ny*ny+nz*nz==1,"half-unit triangle area");check(nx*(0.5f-p.X)+ny*(0.5f-p.Y)+nz*(0.5f-p.Z)>0,"consistent inward winding");if(nx)++faces[p.X==0?0:1];else if(ny)++faces[p.Y==0?2:3];else ++faces[p.Z==0?4:5];}
 for(int f=0;f<6;++f)check(faces[f]==2,"each face has two triangles");check(m.UnitBoxIB.data[33]==1&&m.UnitBoxIB.data[34]==5&&m.UnitBoxIB.data[35]==7,"original final triangle ordinal");puts("00b8d0d0_TEST PASS");return 0;}