// CAIStateGroup_JackOfBladesBase::~CAIStateGroup_JackOfBladesBase() -- the compiler-generated (implicit) destructor, kept out of line: ~vector +0x40, ~vector +0x34, ~CIntelligentPointer +0x24, ~CIntelligentPointer +0x1C, ~CIntelligentPointer +0x14, then the base destructor out of line
// Leaf: ??1CAIStateGroup_JackOfBladesBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903046, 51 bytes (+ int3 padding), thiscall, no return value; reached from the scalar deleting destructors listed below.
// Identity (proven): CAIStateGroup_JackOfBladesBase::~CAIStateGroup_JackOfBladesBase(); evidence: its callers are the scalar deleting destructors of CAIStateGroup_JackOfBladesBase 0x00903079 (slot 0 of vtable 0x0128D384, RTTI .?AVCAIStateGroup_JackOfBladesBase@@ : CAIStateGroupBase : CBaseClass), CAIStateGroup_JackOfBladesStage2 0x00903296 (vtable 0x0128D3C4, derived from CAIStateGroup_JackOfBladesBase), CAIStateGroup_JackOfBladesStage3 0x009032E0 (vtable 0x0128D404, derived from CAIStateGroup_JackOfBladesBase), and 0x00903265 (a deleting-destructor-shaped body reached through no vtable); the destroyed offsets +0x14/+0x1C/+0x24/+0x34/+0x40 are Ego_r.pdb CAIStateGroup_JackOfBladesBase's class-typed members (rebased after the 12-byte vector), and the tail jump is ~CAIStateGroupBase 0x008FD288.
// Label: the manifest's "_Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict>" is propagated across byte-identical bodies (family J072: five implicit destructors of
//   five classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the implicit ~CIntelligentPointer<T>); std::vector<CIntelligentPointer<CThing> >::~vector 0x004406D0; CAIStateGroupBase::~CAIStateGroupBase 0x008FD288 (tail jmp; PDB-spelled ??1CAIStateGroupBase@@UAE@XZ).
// Layout: Ego_r.pdb CAIStateGroup_JackOfBladesBase members, rebased after the 12-byte retail STLport vector (Pillars Ego_r +0x44 -> retail +0x40, as the destructor's `lea ecx,[esi+40h]` shows).
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the class-typed
//   members are destroyed in reverse declaration order, then the base destructor CAIStateGroupBase::~CAIStateGroupBase (a tail `jmp`). KillOne's explicit destructor call is a modelling
//   device: it makes VC7.1 emit the implicit destructor, which it keeps out of line.
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {            // STLport 4.6, 12 bytes
public:
    ~vector();                                                  // out of line in retail (0x004406D0 for CIntelligentPointer<CThing>)
};
}   // namespace std
class CBaseClass {                                       // RTTI root, vtable 0x01231710
public:
    CBaseClass();                                        // 0x0099A2F0
    virtual ~CBaseClass();                               // 0x0099A300
};
#pragma pack(push, 1)
class CBaseIntelligentPointer {                          // 8 bytes, 1-aligned in retail
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    void* PData;                                         // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CThing;
class CThingCreatureBase;
class CAIBrain;
enum EScriptAIPriority { };                              // Ego_r.pdb (enumerators not needed)
class CAIStateGroupBase : public CBaseClass {            // Ego_r.pdb 20 bytes
public:
    CAIStateGroupBase();                                 // FableWin.pdb ??0CAIStateGroupBase@@QAE@XZ
    virtual ~CAIStateGroupBase();                        // 0x008FD288 (user-written: stores its vtable)
    CAIBrain*         PBrain;                            // +0x04
    EScriptAIPriority ScriptAIPriority;                  // +0x08
    CCharString       Name;                              // +0x0C
    bool              Terminated;                        // +0x10
    unsigned char     Priority;                          // +0x11
    bool              Enabled;                           // +0x12
};
class CAIStateGroup_JackOfBladesBase : public CAIStateGroupBase {   // vtable 0x0128D384, Ego_r.pdb 0x54 bytes, retail 0x4C (12-byte vectors)
public:
    CAIStateGroup_JackOfBladesBase();                    // FableWin.pdb ??0CAIStateGroup_JackOfBladesBase@@QAE@XZ
    CIntelligentPointer<CThing>             PSwordEmitter;   // +0x14
    CIntelligentPointer<CThing>             PArenaCentre;    // +0x1C
    CIntelligentPointer<CThingCreatureBase> PTargetThing;    // +0x24
    long                                    EnemyKnockDownFrame;   // +0x2C
    long                                    SpeechCounter;   // +0x30
    std::vector<CIntelligentPointer<CThing> > PillarMarkers; // +0x34
    std::vector<CIntelligentPointer<CThing> > Pillars;       // +0x40 (Ego_r +0x44)
};
void KillOne(CAIStateGroup_JackOfBladesBase* p) { p->CAIStateGroup_JackOfBladesBase::~CAIStateGroup_JackOfBladesBase(); }