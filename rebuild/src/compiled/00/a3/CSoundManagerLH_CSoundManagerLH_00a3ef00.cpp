// CSoundManagerLH::CSoundManagerLH(const CSoundManagerInit& init, bool exclusive) -- Lionhead-audio sound manager: CASoundManager base, empty sound list / name maps, the (0, 0.5, 0) vector; registers the three sound allocators, creates the sound system from a setup record (buffer sizes, 10 MB, a CLHFileManager), configures its voices from the init block's channel mask, then builds the emitter and receiver and enables the player
// Leaf: ??0CSoundManagerLH@@QAE@ABUCSoundManagerInit@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3EF00, 561 bytes, thiscall `ret 8`, returns this; frameless (two ebx/esi/edi/ebp pushes).
// Identity (two sources): (1) the vtable written at +0 is 0x0129D2C4, whose RTTI names CSoundManagerLH; (2) the base-class
//   constructor called first is the landed CASoundManager::CASoundManager(const CSoundManagerInit&, bool) 0x00991910 (this
//   row's parameters pass straight through), the member layout is the landed CSoundManagerLH destructor 0x00A3EDE0's (list
//   +0x44, vector maps +0x48/+0x58/+0x68, the (0, 0.5, 0) vector +0xAC, system +0x7C, emitter +0x80, receiver +0x84) and the
//   only caller is CSystemManager::Initialise 0x009A6610 (`new CSoundManagerLH(init, exclusive)`). No PDB symbol covers
//   CSoundManagerLH (the Ego PDBs stop at CASoundManager); the member spellings are the landed CSoundManagerLH rows'.
// Callees: CASoundManager ctor 0x00991910; operator new 0x00BFEA1A; CBBBSoundAllocator / CBBBSoundPhysicalAllocator
//   constructors (inline vtable stores, RTTI-named, both LHA::CAllocator); the libpk allocator setters 0x00C08CF0 /
//   0x00C08D00 / 0x00C08D10 (each stores its argument into the globals 0x0139C960 / 0x0139C968 / 0x0139C964 through
//   0x00C16690 / 0x00C166D0 / 0x00C166B0, default 0x0143BB34); the CLHFileManager ctor (inline, LHA::CFileManager);
//   the sound system factory 0x00C096E0 (__stdcall, one argument: the 0x30-byte setup record, no `this`); CSoundSystem slot 11
//   SetBufferSize(0x8000), slot 4 Initialise(config), the player (slot 3 / +0x7C) slots 6 (1), 9 SetEnabled(true), 0 Play(0);
//   CSoundSystemConfig ctor 0x00C08800 and SetChannel 0x00C087C0; CSoundEmitterLH ctor 0x00AAC530 (this) and CSoundReceiverLH
//   ctor 0x00A0A8A0 (this).
// Label corrections (NAME_FIXES.tsv): 0x00C096E0 is landed as CWorldManager::InitializeSteeringAfterSave (wrong module: a
//   __stdcall factory of the libpk sound system); 0x00C08CF0 / 0x00C08D00 / 0x00C08D10 are landed under STLport
//   `_Cons_val_pair_morph_*` stand-in names (they are the three allocator setters); 0x00C08800 ("Init") is the config record's
//   constructor and 0x00C087C0 ("CMainGameComponent::BeginInputLoading") its SetChannel; the landed CSoundEmitterLH ctor
//   0x00AAC530 takes a CSoundManagerLH* (it calls GetNextEmitterID 0x00A3E420 on it), not a CGameScriptThing*. The catalog's
//   CMainGameComponent_Initialize is propagated. The vtable 0x012B1E24 is spelled CSoundSystem here (the landed CSoundManagerLH
//   rows) while the landed libpk rows 0x00C094A0 / 0x00C09380 spell it CAudioOwner: coordinator decision.
// Shape: the init list in declaration order (CASoundManager, m_nextEmitterID(0), the list / vector-map members' STLport ctors,
//   the C3DVector); the setup record's fields are assigned in retail order (MaxSounds, MaxVoices, Scale, Flags, MemoryBudget,
//   FileManager) -- the order drives the store sequence; the channel loop's index is `unsigned long` (retail `jb`, a signed
//   index gives `jl`); the LowestSetBit helper is inline (its 128-iteration scan is unrolled into the loop).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include <stddef.h>
void* __cdecl operator new(size_t size);        // 0x00BFEA1A
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const allocator<_Tp>& __a) : _M_node(allocator<_List_node<_Tp> >(), (_List_node_base*)0) {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap { public:
    std::vector<std::pair<K, V> > Data;         // +0x00
    C    Compare;                               // +0x0c
    bool Dirty;                                 // +0x0d
    CVectorMap() : Dirty(false) { }
};
class CSoundBank;
class CSoundBankData;
class CSoundBankRecord { public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
};
struct CSoundManagerInit {                      // descriptive: the derived manager's settings block (the landed 0x00991910 model)
    long          Setting00;                    // +0x00
    long          Setting04;                    // +0x04
    long          Setting08;                    // +0x08
    long          Setting0C;                    // +0x0c
    float         Scale;                        // +0x10
    unsigned long ChannelMask;                  // +0x14 one bit per output channel to enable
    long          Setting18;                    // +0x18
    unsigned long ResourceMemory;               // +0x1c
};
class CResourceBank;
class CASoundManager : public CBaseClassNonCopyable { public:
    long          Setting08;                    // +0x04
    long          Setting0C;                    // +0x08
    float         Scale;                        // +0x0c
    float         InverseScale;                 // +0x10
    long          Setting04;                    // +0x14
    long          Setting00;                    // +0x18
    float         Volume;                       // +0x1c
    bool          Flag;                         // +0x20
    bool          Unknown21[3];                 // +0x21
    bool          Enabled24;                    // +0x24
    bool          Enabled25;                    // +0x25
    bool          Enabled26;                    // +0x26
    CCountedPointer<CResourceBank> ResourceBank;   // +0x28
    CVectorMap<long, CSoundBankRecord> Banks;   // +0x30
    CASoundManager(const CSoundManagerInit& init, bool flag);   // 0x00991910
    virtual ~CASoundManager();                  // 0x00991880
};
class C3DVector { public:
    float x, y, z;
    C3DVector(float X, float Y, float Z) : x(X), y(Y), z(Z) { }
};
// ---- the LHA (Lionhead audio) interfaces the game implements (RTTI: CBBBSoundAllocator / CBBBSoundPhysicalAllocator : LHA::CAllocator,
//      CLHFileManager : LHA::CFileManager) ----
namespace LHA {
class __declspec(novtable) CAllocator { public:
    virtual ~CAllocator();                      // slot 0
    virtual void v01();
    virtual void v02();
};
class __declspec(novtable) CFileManager { public:
    virtual ~CFileManager();                    // slot 0
    virtual void v01();
};
}
class CBBBSoundAllocator : public LHA::CAllocator { public:          // vtable 0x0129D294
    CBBBSoundAllocator() { }
};
class CBBBSoundPhysicalAllocator : public LHA::CAllocator { public:  // vtable 0x0129D2A4
    CBBBSoundPhysicalAllocator() { }
};
class CLHFileManager : public LHA::CFileManager { public:            // vtable 0x0129D2B4
    CLHFileManager() { }
};
// ---- the audio library (libpk, no PDB / RTTI: descriptive names) ----
void __stdcall SetSoundAllocator_00c08cf0(LHA::CAllocator* allocator);            // 0x00C08CF0 (global 0x0139C960, default 0x0143BB34)
void __stdcall SetSoundTempAllocator_00c08d00(LHA::CAllocator* allocator);        // 0x00C08D00 (global 0x0139C968)
void __stdcall SetSoundPhysicalAllocator_00c08d10(LHA::CAllocator* allocator);    // 0x00C08D10 (global 0x0139C964)
struct CSoundSystemSetup {                      // 0x30 bytes, the record 0x00C096E0 builds the sound system from
    long              MaxSounds;                // +0x00 0x40
    long              MaxVoices;                // +0x04 0x200
    long              MemoryBudget;             // +0x08 10 MB
    long              Flags;                    // +0x0c 1
    float             Scale;                    // +0x10 15.0f
    LHA::CFileManager* FileManager;             // +0x14
    float             Float18;                  // +0x18
    float             Float1C;                  // +0x1c 1.0f
    float             Float20;                  // +0x20 30.0f
    long              Long24;                   // +0x24
    long              Long28;                   // +0x28
    long              Long2C;                   // +0x2c
    CSoundSystemSetup() : FileManager(0), Float18(0.0f), Float1C(1.0f), Float20(30.0f), Long24(0), Long28(0), Long2C(0) { }
};
struct CSoundSystemConfig {                     // 0x1c bytes, the voice configuration record (ctor 0x00C08800)
    unsigned short ChannelMask;                 // +0x00
    unsigned short ChannelTouched;              // +0x02
    long           Long04;                      // +0x04 default 4
    long           Long08;                      // +0x08 default 0x1000
    long           Long0C;                      // +0x0c default 2
    long           Long10;                      // +0x10 default 0x800
    long           Long14;                      // +0x14 default 3
    long           Long18;                      // +0x18 default 1
    CSoundSystemConfig();                                       // 0x00C08800
    void SetChannel_00c087c0(unsigned long channel, bool enable);   // 0x00C087C0 (SetMaskBit16 0x00C0AEE0 on the two words)
};
struct CSoundPlayer {                           // the landed CSoundManagerLH rows' player model
    virtual void Play(long sound);              // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual void v06(long value);               // slot 6 (+0x18)
    virtual void v07(); virtual void v08();
    virtual void SetEnabled(bool enabled);      // slot 9 (+0x24)
};
struct CSoundSystem {                           // vtable 0x012B1E24 (the landed CSoundManagerLH rows' name; the libpk ctor 0x00C094A0 is landed as CAudioOwner)
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void v03();                         // slot 3 (+0x0c) GetOutput
    virtual void Initialise(CSoundSystemConfig* config);   // slot 4 (+0x10)
    virtual void v05();
    virtual CSoundPlayer* GetPlayer();          // slot 6 (+0x18)
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void SetBufferSize(long size);      // slot 11 (+0x2c)
};
CSoundSystem* __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup* setup);   // 0x00C096E0 (`ret 4`, no this)
class CSoundManagerLH;
class CSoundObjectLH;
class CSoundBankLH;
class CSoundEmitterLH { public:
    CSoundEmitterLH(CSoundManagerLH* manager);  // 0x00AAC530
    unsigned char _body[0x20];
};
class CSoundReceiverLH { public:
    CSoundReceiverLH(CSoundManagerLH* manager); // 0x00AAD020
    unsigned char _body[0x2c];
};
class CSoundManagerLH : public CASoundManager { public:
    CSoundSystem*               m_pSoundSystem; // +0x40
    CSoundEmitterLH*            m_pMain;        // +0x44
    CSoundReceiverLH*           m_pReceiver;    // +0x48
    std::list<CSoundObjectLH*>  Sounds;         // +0x4c
    long                        m_nextEmitterID;   // +0x50
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>      NameMap;        // +0x64
    C3DVector                   m_vector;       // +0x74
    CSoundManagerLH(const CSoundManagerInit& init, bool exclusive);
    virtual ~CSoundManagerLH();                 // 0x00A3EDE0
};

inline long LowestSetBit(unsigned long mask)
{
    for (unsigned long i = 0; i < 128; ++i)
        if (mask & (1 << i))
            return i;
    return -1;
}

CSoundManagerLH::CSoundManagerLH(const CSoundManagerInit& init, bool exclusive)
    : CASoundManager(init, exclusive), m_nextEmitterID(0), m_vector(0.0f, 0.5f, 0.0f)
{
    CBBBSoundAllocator* allocator = new CBBBSoundAllocator;
    CBBBSoundPhysicalAllocator* physical = new CBBBSoundPhysicalAllocator;
    SetSoundAllocator_00c08cf0(allocator);
    SetSoundTempAllocator_00c08d00(allocator);
    SetSoundPhysicalAllocator_00c08d10(physical);

    CSoundSystemSetup setup;
    setup.MaxSounds = 0x40;
    setup.MaxVoices = 0x200;
    setup.Scale = 15.0f;
    setup.Flags = 1;
    setup.MemoryBudget = 0xA00000;
    setup.FileManager = new CLHFileManager;
    m_pSoundSystem = CreateSoundSystem_00c096e0(&setup);
    if (m_pSoundSystem)
    {
        m_pSoundSystem->SetBufferSize(0x8000);
        m_pSoundSystem->GetPlayer()->v06(1);

        CSoundSystemConfig config;
        config.Long04 = 4;
        config.Long0C = 4;
        config.Long18 = 0;
        config.Long14 = 2;
        config.Long10 = 0x6000;
        config.Long08 = 0x800;
        unsigned long mask = init.ChannelMask;
        while (mask != 0)
        {
            long bit = LowestSetBit(mask);
            config.SetChannel_00c087c0(bit, true);
            mask ^= 1 << bit;
        }
        m_pSoundSystem->Initialise(&config);
    }
    m_pMain = new CSoundEmitterLH(this);
    m_pReceiver = new CSoundReceiverLH(this);
    if (m_pSoundSystem)
        m_pSoundSystem->GetPlayer()->SetEnabled(true);
    if (m_pSoundSystem)
        m_pSoundSystem->GetPlayer()->Play(0);
}