// CEngineOcclusionManager::BuildUnitBox -- initialise all eight unit cube corners and its triangle indices.
// Leaf: ?BuildUnitBox@CEngineOcclusionManager@@AAEXXZ
// Flags: /GS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_BUILDUNITBOX_PASS26
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/EVertexType.h"
#include "engine/EVertexBufferPool.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CTVertexPos.h"
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