// StaticInitialization::Initialize_CUnprovenGlobal_013CB2F8() -- construct the original static object and register its destructor.
// Leaf: _$E1
// Identity: proven original CInkeyBuffer from own vtable/base chain and constructor, global013CB2F8 ->original destructor callback. Compiler-generated initializer performs base construction, genuine derived vtable store and atexit registration. Original global spelling unproven, descriptive name retained.
// Layout: true original polymorphic hierarchy and typed named fields, PCsize416. No opaque base/vptr. Original base constructors/destructors external; derived implicit destructor is genuine. CInkeyBuffer CCharEvent4 donor wchar_t Char/boolPressed has no provable class key/qualified scope; explicit CUnprovenCharEvent projection preserves only verified fields/size without claiming original V/U identity. For shader layout, typed range12 and genuine STLport CArray/vector12 reconcile donor336->retail332; CCharString non-POD by-value PeekName ABI retained. Available shader header aliased. No handwritten initializer/callback wrapper.
#include <stddef.h>
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInitBaseClass:public CBaseClass{public:bool Valid;CInitBaseClass();virtual ~CInitBaseClass();virtual void Validate()const;};
class CUnprovenCharEvent_013CB2F8{public:wchar_t Char;bool Pressed;};class CInkeyBuffer:public CInitBaseClass{public:CUnprovenCharEvent_013CB2F8 Buffer[100];unsigned long FirstChar,LastChar;CInkeyBuffer(){}};CInkeyBuffer CUnprovenGlobal_013CB2F8;
#ifdef _MSC_VER
typedef char OwnerSize[sizeof(CInkeyBuffer)==416?1:-1];
#endif