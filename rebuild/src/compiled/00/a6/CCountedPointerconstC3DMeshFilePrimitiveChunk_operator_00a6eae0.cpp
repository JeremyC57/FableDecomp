// CCountedPointer<const C3DMeshFilePrimitiveChunk>::operator=(const CCountedPointer&) -- Replace ownership only when control blocks differ.
// Leaf: ??4?$CCountedPointer@$$CBVC3DMeshFilePrimitiveChunk@@@@QAEAAV0@ABV0@@Z
// Identity: proven. Own call00a8c33d consumes return of C3DMeshFileSubMeshChunk::FindNextPrimitive const00abaa10; exact original return type CCountedPointer<const C3DMeshFilePrimitiveChunk> matches typed consumer. Own target is the shared-control-block copy assignment; original wrapper/control-block class keys and fields from names/layouts. Fastcall deleter consumes ECX; scalar delete releases control block.
// Layout: wrapper Data0/Info4; CCPPointerInfo RefCount0/DeleteFunc4/Data8. Original template and genuine pointer element type; no concrete owner layout is instantiated.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class C3DMeshFilePrimitiveChunk;class CCPPointerInfo{public:unsigned long RefCount;void (__fastcall* DeleteFunc)(void*);void* Data;};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}__forceinline void ReleaseData(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);::operator delete(Info);}}}__forceinline ~CCountedPointer(){ReleaseData();}CCountedPointer& operator=(const CCountedPointer& other){T* data=other.Data;CCPPointerInfo* info=other.Info;if(Info!=info){ReleaseData();Data=data;Info=info;if(info)++info->RefCount;}return *this;}};template CCountedPointer<const C3DMeshFilePrimitiveChunk>& CCountedPointer<const C3DMeshFilePrimitiveChunk>::operator=(const CCountedPointer<const C3DMeshFilePrimitiveChunk>&);