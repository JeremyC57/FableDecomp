// CWillButtonHelperTimer::~CWillButtonHelperTimer() -- destroy the owned vector then its earlier nontrivial member.
// Leaf: ??1CWillButtonHelperTimer@@QAE@XZ
// Identity: probable function role; PDB unique CWillButtonHelperTimer PCreature+8 and vector<float>+24; body unreferenced in spell code region, destructor-versus-Destroy role uncertain.
// Layout: no record header. Typed real members provide vector start and tracked pointer/string destruction; CProfile uses actual double-aligned72-byte layout and nested record key is struct.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
extern "C" void __cdecl free(void*);
namespace std { template<class T> class allocator {}; template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; }; template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; ~vector() { if(_M_start) free(_M_start); } }; template<class T> void __fastcall _Destroy(T* p) { p->~T(); } }
class CThing; class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; }; template<class T> class CIntelligentPointer:public CBaseIntelligentPointer {};
enum EHeroAbility {}; enum EButtonHeldState {};
class CWillButtonHelperTimer { public: long ButtonHeldDownTimer,WillAccumulatedLevel; CIntelligentPointer<CThing> PCreature; EHeroAbility MyAbility; EButtonHeldState CurrentState; std::vector<float> WillPowerUpAccumulatedTimings; float MaxNumberLevels,CurMaxLevels; bool WillIncrementedFlag,UpdateAccLevel,IsAICasting,IsAIHoldingDown; long LevelToCast; ~CWillButtonHelperTimer(); };
CWillButtonHelperTimer::~CWillButtonHelperTimer() {}