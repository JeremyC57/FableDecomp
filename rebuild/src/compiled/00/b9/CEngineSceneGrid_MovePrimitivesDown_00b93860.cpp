// 00b93860 CEngineSceneGrid::MovePrimitivesDown
// Leaf: ?MovePrimitivesDown@CEngineSceneGrid@@AAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include <new>
#include "rebuild_abi.h"
class CEngineSceneGridPos{public:signed char Level;unsigned char X,Y;CEngineSceneGridPos(){}CEngineSceneGridPos(signed char l,unsigned char x,unsigned char y):Level(l),X(x),Y(y){}CEngineSceneGridPos ConvertToLevel(long level)const{return CEngineSceneGridPos((signed char)level,X>>(Level-level),Y>>(Level-level));}};
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBaseFields
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
// Layout: shared flattened primitive header defines native GridCell20, GridPos24,
// NextPrimitive38. Typed accessors retain its original by-value three-byte GridPos.
class CEngineInternalPrimitiveBase:public CEngineInternalPrimitiveBaseFields{public:void AddToList(CEngineInternalPrimitiveBase**);void RemoveFromList();CEngineSceneGridPos GetGridPos()const{return *reinterpret_cast<const CEngineSceneGridPos*>(&GridPos_Level);}CEngineInternalPrimitiveBase*GetNextPrimitive()const{return reinterpret_cast<CEngineInternalPrimitiveBase*>(NextPrimitive);}};
class CEngineSceneGrid;
class CEngineSceneGridCell{friend class CEngineSceneGrid;private:CEngineSceneGridCell*Parent;CEngineInternalPrimitiveBase*FirstPrimitive;unsigned char unobservedBoundingBoxDrawAndHandles[0x2c];long LocalCount;unsigned long TotalCount,RenderMask,OccludedLayers;unsigned char BoundingBoxValid:1;unsigned char TotalCountValid:1;unsigned char RenderMaskValid:1;unsigned char WasRendered:1;unsigned char HasOcclusionResults:1;unsigned char OccludedFrames,PreloadFrames;
CEngineSceneGridCell(CEngineSceneGridCell*);void InvalidateAll();__forceinline void AddPrimitive(CEngineInternalPrimitiveBase&p){p.AddToList(&FirstPrimitive);++LocalCount;if(BoundingBoxValid||TotalCountValid||RenderMaskValid){BoundingBoxValid=0;TotalCountValid=0;RenderMaskValid=0;if(Parent)Parent->InvalidateAll();}}};
#include "engine/CArray.h"
class CBaseClassNonCopyable{protected:virtual~CBaseClassNonCopyable();};
class CEngineSceneGrid:public CBaseClassNonCopyable{
private:unsigned long Level;CEngineSceneGrid*LevelUp,*LevelDown;unsigned long Size;CArray<CEngineSceneGridCell*>GridCells;
__forceinline void AddPrimitive(unsigned long x,unsigned long y,CEngineInternalPrimitiveBase&p){CEngineSceneGridCell*cell=GridCells[Size*y+x];if(!cell){CEngineSceneGridCell*parent=0;if(LevelUp)parent=LevelUp->GetCellCreate(x>>1,y>>1);cell=new CEngineSceneGridCell(parent);GridCells[Size*y+x]=cell;}p.GridCell=cell;cell->AddPrimitive(p);}
void MovePrimitivesDown();public:CEngineSceneGridCell*GetCellCreate(unsigned long,unsigned long);
};
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGridPos)==3);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGridCell)==0x48);
FABLE_STATIC_ASSERT(sizeof(CEngineSceneGrid)==0x20);
void CEngineSceneGrid::MovePrimitivesDown(){
 for(unsigned long y=0;y<Size;++y)for(unsigned long x=0;x<Size;++x){
  CEngineSceneGridCell*cell=GridCells[Size*y+x];if(!cell)continue;
  CEngineInternalPrimitiveBase*p=cell->FirstPrimitive;
  while(p){CEngineInternalPrimitiveBase*next=p->GetNextPrimitive();if((unsigned long)p->GetGridPos().Level>Level){p->RemoveFromList();CEngineSceneGridPos pos=p->GetGridPos().ConvertToLevel(Level+1);LevelDown->AddPrimitive(pos.X,pos.Y,*p);}p=next;}
 }
}