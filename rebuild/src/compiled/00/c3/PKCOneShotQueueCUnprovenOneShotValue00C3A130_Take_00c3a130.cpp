// PKCOneShotQueue<CUnprovenOneShotValue_00C3A130>::Take(value) -- consume the one-shot value, reporting an empty queue.
// Leaf: ?Take@?$PKCOneShotQueue@VCUnprovenOneShotValue_00C3A130@@@@UAE?AVCUnprovenOneShotValue_00C3A130@@XZ
// Flags: /EHsc
// Identity: probable original template owner from exact assertion filename; Own diagnostic filename PKCOneShotQueue.h line28 and exact Nothing to return! message. Own four-slot table012b54a4 is Take/Put/HasValue/Count with no virtual destructor; constructors initialize full32-bit Filled after4/12-byte trivial Value, not bool. Take returns T by value using native hidden class-return pointer (EAX preserved); it has no explicit output reference. Own EAX tail proves returned buffer identity and explains the copy scheduling. Target reports invalid state using actual CString chain before CDebugBreak then still performs operation; no original element identity recoverable. Distinct descriptive trivial unsigned-word projections preserve own observed copy width without claiming recovered scalar/vector source types.
// Layout: source-proven polymorphic queue Value+4, full32-bit Filled+16; no unsupported base or destructor invented. Actual CString/assert-hook/debug-break family reused, preserving EH lifetime and callback effects. No shared middleware header available. Element projections are explicit original-identity uncertainty allowed by owner rules.
#ifndef _MSC_VER
#define __fastcall
#endif
extern char* _afxPchNil;typedef void(__fastcall* AssertHook)(const char*);extern AssertHook g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(long);};struct CDebugBreak{CDebugBreak();};
class CUnprovenOneShotValue_00C3A130{public:unsigned int Words[3];};
template<class T>class PKCOneShotQueue{public:T Value;int Filled;virtual T Take();virtual void Put(const T&);virtual int HasValue(){return Filled!=0;}virtual int Count(){return Filled;}};
template<class T>T PKCOneShotQueue<T>::Take(){if(!Filled){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCOneShotQueue.h" << "(" << (long)28 << ") : " << "Nothing to return!" << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}Filled=0;return Value;}
template CUnprovenOneShotValue_00C3A130 PKCOneShotQueue<CUnprovenOneShotValue_00C3A130>::Take();