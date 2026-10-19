// CEnginePrimitiveOcclusionManager::CheckVisibilityResult -- accumulate prior-frame occlusion samples, advance invisibility age, and clear completed/stale handles.
// Retail 0x00B2F8E0, 0x106 bytes. Real manager/occlusion headers are retained.
// CEnginePrimitiveOcclusionInfo has no generated header. Its donor vector is16 bytes;
// native STLport vector is12, so Seen is +0x18 and TestFrame +0x1c (donor +0x20).
// GetResult at00B8D0B0 has the matching PDB signature unsigned long(unsigned long,bool);
// its retail body currently returns0. It is still an external call, with global reload per iteration.
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineOcclusionManager.h"
#include <string.h>
inline void* CopyTrivial(const void* first,const void* last,void* result) {
 if(first==last) return result;
 size_t bytes=(const char*)last-(const char*)first;
 return (char*)memmove(result,first,bytes)+bytes; // 0x00BFEAE6
}
struct CHandleVector {
 unsigned long* First; unsigned long* Last; unsigned long* End;
 unsigned long* begin() const{return First;}
 unsigned long* end() const{return Last;}
 bool empty() const{return begin()==end();}
 unsigned long size() const{return end()-begin();}
 unsigned long& operator[](unsigned long i){return *(begin()+i);}
 void Clear(){Last=(unsigned long*)CopyTrivial(Last,Last,First);}
};
struct CEnginePrimitiveOcclusionInfo {
 void* NativeVTable; unsigned long ReferenceCount;
 long InvisibleFrames; CHandleVector OcclusionTestHandles;
 bool Seen; unsigned long TestFrame;
 void ClearHandles(); // 0x00B2F8B0: clears handles only
};
struct CEngineOcclusionManager_Methods:CEngineOcclusionManager {
 unsigned long GetResult(unsigned long,bool); // 0x00B8D0B0
};
extern CEngineOcclusionManager_Methods* g_OcclusionManager; //0x01436E5C
struct CEnginePrimitiveOcclusionManager_Methods:CEnginePrimitiveOcclusionManager {
 void CheckVisibilityResult(CEnginePrimitiveOcclusionInfo&);
};
void CEnginePrimitiveOcclusionManager_Methods::CheckVisibilityResult(CEnginePrimitiveOcclusionInfo& info) {
 if(InvalidateOcclusion) {
  info.InvisibleFrames=0;
  info.OcclusionTestHandles.Clear();
 } else {
  unsigned long frame=g_OcclusionManager->CurrentFrame;
  if(info.TestFrame+1==frame) {
   if(!info.OcclusionTestHandles.empty()) {
    unsigned long count=info.OcclusionTestHandles.size();
    unsigned long samples=0;
    for(unsigned long i=0;i<count;++i) {
     unsigned long handle=info.OcclusionTestHandles[i];
     samples+=g_OcclusionManager->GetResult(handle,true);
    }
    if(samples==0) ++info.InvisibleFrames; else info.InvisibleFrames=0;
    info.OcclusionTestHandles.Clear();
    info.TestFrame=frame;
   }
  } else if(info.TestFrame!=frame) {
   info.ClearHandles();
   info.InvisibleFrames=PrimitiveOccludeDelay+1;
  }
 }
}
