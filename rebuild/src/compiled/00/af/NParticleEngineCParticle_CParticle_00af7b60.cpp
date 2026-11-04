// 00af7b60 NParticleEngine::CParticle::~CParticle
// Leaf: ??1CParticle@NParticleEngine@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include <stdlib.h>
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass{CBaseClassNonCopyable(const CBaseClassNonCopyable&);protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
// Layout: handle private counted-pointer storage is unobserved; destruction is external.
class CEnginePrimitiveHandle:public CBaseClass{unsigned char unreadData[4];public:virtual ~CEnginePrimitiveHandle();};
#include "engine/CArray.h"
// Layout: native CArray storage/lifetime is its twelve-byte STLport vector base.
namespace boost{template<class T>class scoped_ptr{T*ptr;scoped_ptr(const scoped_ptr&);public:explicit scoped_ptr(T*p=0):ptr(p){}~scoped_ptr(){delete ptr;}};}
namespace NParticleEngine{
// Layout: random-acceleration state is unread; raw original base and virtual slots retained.
class CRandomAcceleration:public CBaseClassNonCopyable{unsigned char unread[20];public:virtual ~CRandomAcceleration();virtual void Delete();};
class CParticle:public CBaseClassNonCopyable{unsigned char unreadPrefix[124];boost::scoped_ptr<CRandomAcceleration>RandomAcceleration;CArray<CArray<long> >PrimIDList;CArray<CEnginePrimitiveHandle>ParentHandleList;
#ifdef PASS9_TEST
friend int ::main();
#endif
public:virtual ~CParticle();virtual void Delete();};
CParticle::~CParticle(){}
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticle)==156);FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);