# h20_c687.py: naming wave 20, family c687 -- the chunk 687 NAME_FIXES rows (QUEUE_W20_input.tsv, c687) whose landed own rows / callers carry
# wrong or invented spellings; descriptive libpk names as the chunk proved them (no PDB covers libpk), byte-identical only:
#   00c61c60 / 00c61bc0 CPKArray::GetCount / GetCapacity const (+8 / +0 of the array CPKArray::GetRandom 0x00C61C00 indexes; were the
#            fastcall DrawGetEnvironment stub and a `T::Get` stub); the caller 00c44b50 of GetCapacity (was InputEventSubobject::GetKey);
#   00c44a90 CAmbientEmitter::GetVolume() const ((+4)->GetValue() * (+0)->+0x160; was CMouseCursor_TurnToPointer) and its callers 00c61f60
#            (CPKEmitter::GetValue) / 00c44ab0 (CMouseCursor::TurnToPointer on `this`: the row's class is the emitter);
#   00c08800 CSoundSystemConfig::CSoundSystemConfig() (the 0x1c-byte voice configuration record 0x00A3EF00 fills; was `C::Init`);
#   00c087c0 CSoundSystemConfig::SetChannel(unsigned long, bool) over the landed ?SetMaskBit16@@YIXK_NPAG1@Z 0x00C0AEE0 (was
#            CMainGameComponent::BeginInputLoading over a BeginLoading stand-in) and its caller 00a3ef00 (SetChannel_00c087c0);
#   00c08cf0 / 00c08d00 / 00c08d10 the __stdcall SetSoundAllocator / SetSoundTempAllocator / SetSoundPhysicalAllocator_00c08xxx(LHA::CAllocator*)
#            the landed CSoundManagerLH ctor 0x00A3EF00 binds (were _Cons_val_pair_morph_* stand-ins; the __fastcall store callees
#            0x00C16690 / 0x00C166D0 / 0x00C166B0 named descriptively -- their own rows carry propagated names, DEFERRED);
#   00c36c70 CAmbientScene::Lookup(unsigned long) forwarding to the landed ?GetItem@CSequenceList@@QBEPAUCSequenceItem@@K@Z 0x00C463C0 on the
#            list at +0xc (was `T::Run` over Run_00c463c0);
#   00c46da0 SPosition2D::Add(const CIndexedValue&) (was CQuadric::OperatorPlusEq);
#   00ad7550 std::sort<QuadricOptimiserInternals::COptimisedVertex::CGroup*, bool (__fastcall*)(const CGroup&, const CGroup&)> (the spelling the
#            landed LimitGroupCount 0x00ACA5F0 binds; the callees 0x00AD6F10 / 0x00AD1CA0 are landed under the same instantiation's spellings);
#   00ad4e20 vector<CGroup>::_M_insert_overflow -- the CUnprovenElement_00ad4e20 placeholder is the 8-byte CGroup {BoneIndex, BoneWeight};
#   00c73a00 CString_00c73a00::ClassifyAudioExtension (.ogg -> 1, .wma -> 0; was ClassifyShaderExtension);
#   00ca6620 CBaseRenderer::GetState: WaitDispatchingMessages' third parameter is HWND (?WaitDispatchingMessages@@YGKPAXKPAUHWND__@@I0@Z, the
#            landed chunk-687 leaf of 0x00CB09A0; was void*);
#   00aac530 / 00aac580 CSoundEmitterLH ctors: the manager's 0x00A3E420 is CSoundManagerLH::GetNextEmitterID (chunk 687; were
#            CGameScriptThing::DecrementScriptCounter / TakeNextEmitterNumber).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c687.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def P(a, txt):
    if a not in _X: he.put(a, 'src', txt)

P('00c61c60', '''// CPKArray::GetCount() const -- the number of entries in the libpk array (+0x08)
// Leaf: ?GetCount@CPKArray@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61C60, 4 bytes (thiscall, `mov eax,[ecx+8] ; ret`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the array CPKArray::GetRandom 0x00C61C00 indexes; CAmbientEvent::Trigger
//   0x00C61D10 tests it before picking a sound. Was the propagated DrawGetEnvironment (a __fastcall free stub over a byte pointer).
class CPKArray { public:
    unsigned long m_capacity;                     // +0x00 (GetCapacity 0x00C61BC0)
    unsigned long _pad04;                         // +0x04
    unsigned long m_count;                        // +0x08
    unsigned long GetCount() const;
};
unsigned long CPKArray::GetCount() const { return m_count; }
''')
P('00c61bc0', '''// CPKArray::GetCapacity() const -- the capacity word of the libpk array (+0x00)
// Leaf: ?GetCapacity@CPKArray@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61BC0, 3 bytes (thiscall, `mov eax,[ecx] ; ret`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the array CPKArray::GetRandom 0x00C61C00 indexes; CAmbientEvent::Trigger
//   0x00C61D10 multiplies it by rand() for the empty-slot probability test. Was the propagated CInputEvent::GetKey (a `T::Get` stub).
class CPKArray { public:
    unsigned long m_capacity;                     // +0x00
    unsigned long _pad04;                         // +0x04
    unsigned long m_count;                        // +0x08 (GetCount 0x00C61C60)
    unsigned long GetCapacity() const;
};
unsigned long CPKArray::GetCapacity() const { return m_capacity; }
''')
a = '00c44b50'
if a not in _X:
    he.edit(a, 'src', 'class InputEventSubobject\n{\npublic:\n    unsigned long GetKey();\n    void GetFramesRemaining(unsigned long key);\n};\n',
            '// naming wave 20 (chunk 687): the +0x80 object is a libpk CPKArray (GetCapacity 0x00C61BC0, descriptive); the other member names stay descriptive\nclass CPKArray\n{\npublic:\n    unsigned long GetCapacity() const;             // 0x00C61BC0\n    void GetFramesRemaining(unsigned long key);\n};\n')
    he.edit(a, 'src', '    InputEventSubobject event80;\n', '    CPKArray event80;\n')
    he.edit(a, 'src', '    if (key < event80.GetKey())\n', '    if (key < event80.GetCapacity())\n')
P('00c44a90', '''// CAmbientEmitter::GetVolume() const -- the emitter's ramped volume parameter (+4) times its owner's (+0) +0x160 scale
// Leaf: ?GetVolume@CAmbientEmitter@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44A90, 25 bytes (thiscall, float in st0).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 calls it on the emitter for the volume it hands
//   to the voice parameters (compared with 0.0f); the landed CPKVoiceHandle::UpdateScaledParameter 0x00C61F60 and the emitter's own 0x00C44AB0
//   call it too. Was the propagated CMouseCursor::TurnToPointer (a __fastcall free stub). The ramped parameter's getter is the landed
//   ?GetValue@CRampedParam_00c38510@@QBEMXZ 0x00C38510.
class CAmbientEmitterOwner { public: unsigned char _pad[0x160]; float m_scale; };   // +0x160 (descriptive)
class CRampedParam_00c38510 { public: float GetValue() const; };                    // 0x00C38510 (the landed row's spelling)
class CAmbientEmitter { public:
    CAmbientEmitterOwner*  m_pOwner;              // +0x00
    CRampedParam_00c38510* m_pVolume;             // +0x04
    float GetVolume() const;
};
float CAmbientEmitter::GetVolume() const
{
    float v = m_pVolume->GetValue();
    return v * m_pOwner->m_scale;
}
''')
a = '00c61f60'
if a not in _X:
    he.resub(a, 'src', r'\bCPKEmitter\b', 'CAmbientEmitter')
    he.edit(a, 'src', '    float GetValue();                                  // 0x00C44A90\n', '    float GetVolume() const;                           // 0x00C44A90 ?GetVolume@CAmbientEmitter@@QBEMXZ (chunk 687, descriptive; naming wave 20)\n')
    he.edit(a, 'src', '        float value = emitter->GetValue();\n', '        float value = emitter->GetVolume();\n')
    he.resub(a, 'tst', r'\bCPKEmitter\b', 'CAmbientEmitter')
    he.resub(a, 'tst', r'\bGetValue\b', 'GetVolume')
a = '00c44ab0'
if a not in _X:
    he.resub(a, 'src', r'\bCMouseCursor\b', 'CAmbientEmitter')
    he.edit(a, 'src', '    float TurnToPointer();                  // 0x00C44A90\n', '    float GetVolume() const;                // 0x00C44A90 ?GetVolume@CAmbientEmitter@@QBEMXZ (chunk 687, descriptive; naming wave 20: `this` is the libpk ambient emitter, the member name Initialize stays descriptive)\n')
    he.resub(a, 'src', r'\bTurnToPointer\b', 'GetVolume')
    he.resub(a, 'tst', r'\bCMouseCursor\b', 'CAmbientEmitter')
    he.resub(a, 'tst', r'\bTurnToPointer\b', 'GetVolume')
P('00c08800', '''// CSoundSystemConfig::CSoundSystemConfig() -- the 0x1c-byte voice configuration record: the two channel mask words zeroed, the defaults 4 / 0x1000 / 2 / 0x800 / 3 / 1
// Leaf: ??0CSoundSystemConfig@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08800, 44 bytes (thiscall, returns `this`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the record the landed CSoundManagerLH ctor 0x00A3EF00 builds, fills with
//   SetChannel 0x00C087C0 and hands to CSoundSystem::Initialise (slot 4); its layout is the landed 0x00A3EF00 model. Was a `C::Init` stub.
class CSoundSystemConfig { public:
    unsigned short ChannelMask;                 // +0x00
    unsigned short ChannelTouched;              // +0x02
    long           Long04;                      // +0x04 default 4
    long           Long08;                      // +0x08 default 0x1000
    long           Long0C;                      // +0x0c default 2
    long           Long10;                      // +0x10 default 0x800
    long           Long14;                      // +0x14 default 3
    long           Long18;                      // +0x18 default 1
    CSoundSystemConfig();
};
CSoundSystemConfig::CSoundSystemConfig()
{
    ChannelMask = 0; ChannelTouched = 0; Long04 = 4; Long08 = 0x1000; Long0C = 2; Long10 = 0x800; Long14 = 3; Long18 = 1;
}
''')
P('00c087c0', '''// CSoundSystemConfig::SetChannel(unsigned long channel, bool enable) -- set or clear the channel's bit in the record's two 16-bit mask words through SetMaskBit16 (channel, enable, the words, 0)
// Leaf: ?SetChannel@CSoundSystemConfig@@QAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C087C0, length 19 bytes.
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- called once per set bit of CSoundManagerInit::ChannelMask by the landed
//   CSoundManagerLH ctor 0x00A3EF00 on the config record; was the propagated CMainGameComponent::BeginInputLoading over a BeginLoading stand-in.
// Callees: the landed ?SetMaskBit16@@YIXK_NPAG1@Z 0x00C0AEE0 (`__fastcall`: the channel in ecx, the flag in dl, the mask words and 0 on the stack, `ret 8`).
void __fastcall SetMaskBit16(unsigned long bit, bool set, unsigned short* words, unsigned short* touched);   // 0x00C0AEE0

class CSoundSystemConfig { public:
    unsigned short ChannelMask;                 // +0x00
    unsigned short ChannelTouched;              // +0x02
    void SetChannel(unsigned long channel, bool enable);
};

void CSoundSystemConfig::SetChannel(unsigned long channel, bool enable)
{
    SetMaskBit16(channel, enable, &ChannelMask, 0);
}
''')
a = '00a3ef00'
if a not in _X:
    he.resub(a, 'src', r'\bSetChannel_00c087c0\b', 'SetChannel')
    he.resub(a, 'tst', r'\bSetChannel_00c087c0\b', 'SetChannel')
for a, nm, cal, g in (('00c08cf0', 'SetSoundAllocator_00c08cf0', 'StoreSoundAllocator_00c16690', '0x0139C960 (default 0x0143BB34)'), ('00c08d00', 'SetSoundTempAllocator_00c08d00', 'StoreSoundTempAllocator_00c166d0', '0x0139C968'), ('00c08d10', 'SetSoundPhysicalAllocator_00c08d10', 'StoreSoundPhysicalAllocator_00c166b0', '0x0139C964')):
    P(a, '''// %s(LHA::CAllocator* allocator) -- install the game's allocator into the libpk sound library (the global %s) through the __fastcall store 0x%s
// Leaf: ?%s@@YGXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, 10 bytes (`__stdcall`, `ret 4`: `mov ecx,[esp+4] ; call store ; ret 4`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- the spelling the landed CSoundManagerLH ctor 0x00A3EF00 binds (it passes a new
//   CBBBSoundAllocator / CBBBSoundPhysicalAllocator, RTTI base LHA::CAllocator). Was a _Cons_val_pair_morph_* STLport stand-in. The store callee's
//   own row carries a propagated name (CHoverable_ProcessEvent): named descriptively here, DEFERRED for its own respell.
namespace LHA { class CAllocator; }
void __fastcall %s(LHA::CAllocator* allocator);   // 0x%s (stores the global %s)
void __stdcall %s(LHA::CAllocator* allocator)
{
    %s(allocator);
}
''' % (nm, g.split()[0], cal[-8:].upper(), nm, a.upper(), cal, cal[-8:].upper(), g.split()[0], nm, cal))
P('00c36c70', '''// CAmbientScene::Lookup(unsigned long id) -- the sequence item of a sound id, from the item list at +0xc (a tail call)
// Leaf: ?Lookup@CAmbientScene@@QAEPAUCSequenceItem@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36C70, 7 bytes (thiscall; VC7.1 tail-calls the forward: `mov ecx,[ecx+0Ch] ; jmp rel32`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 passes CPKArray::GetRandom's id and copies two
//   words from the result (the position record at +4 of the item); the callee is the landed ?GetItem@CSequenceList@@QBEPAUCSequenceItem@@K@Z
//   0x00C463C0 (the model of the landed 0x00C36EA0). Was a `T::Run` forwarder over Run_00c463c0 (the catalog's STLport _Cons_val label is propagated).
#pragma pack(push,1)
struct CSequenceItem;
struct CSequenceList {
    CSequenceItem* GetItem(unsigned long index) const;   // 0x00C463C0
};
class CAmbientScene { public:
    unsigned char  _pad[0xc];
    CSequenceList* Steps;                                // +0x0c
    CSequenceItem* Lookup(unsigned long id);
};
#pragma pack(pop)
CSequenceItem* CAmbientScene::Lookup(unsigned long id) { return Steps->GetItem(id); }
''')
P('00c46da0', '''// SPosition2D::Add(const CIndexedValue& value) -- add the pair's two words to the position
// Leaf: ?Add@SPosition2D@@QAEXABUCIndexedValue@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46DA0, 19 bytes (thiscall, `ret 4`).
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- CAmbientEvent::Trigger 0x00C61D10 adds the scene's +0x40 pair (0x00C25790) to the
//   looked-up sound position before the jitter; was the propagated CQuadric::OperatorPlusEq (a same-type pointer model).
struct CIndexedValue {
    float X;
    float Y;
};
struct SPosition2D {
    float X;
    float Y;
    void Add(const CIndexedValue& value);
};
void SPosition2D::Add(const CIndexedValue& value)
{
    X = value.X + X;
    Y = value.Y + Y;
}
''')
P('00ad7550', '''// std::sort(QuadricOptimiserInternals::COptimisedVertex::CGroup* first, QuadricOptimiserInternals::COptimisedVertex::CGroup* last, bool (__fastcall* comp)(const CGroup&, const CGroup&)) -- introsort loop to depth 2*lg(n), then the final insertion sort (inlined: threshold 16)
// Leaf: ??$sort@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@P6I_NABV123@0@Z@std@@YIXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@0P6I_NABV123@1@Z@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD7550, oracle length 115 bytes.
// Identity (naming wave 20, chunk 687): STLport 4.6 sort (stl/_algo.c) over the 8-byte COptimisedVertex::CGroup {BoneIndex, BoneWeight} with the
//   comparison FUNCTION pointer CGroup::CompareWeight 0x00AC6B40 -- the spelling the landed LimitGroupCount 0x00ACA5F0 (chunk 687, Ego_r.pdb)
//   binds; the callees __introsort_loop 0x00AD6F10 and __unguarded_insertion_sort_aux 0x00AD1CA0 are landed under this instantiation's spellings,
//   __insertion_sort 0x00AD5B50 under a non-template SKeyPair8 stand-in (DEFERRED). Was the descriptive Sort_SElement8_00ad7550 (one of 22 bodies).
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 8) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push [comp] ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; (final insertion sort inlined, threshold 16) ...
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack); `__lg` is the shift-count loop, its result int (the iterator difference).
namespace QuadricOptimiserInternals {
class COptimisedVertex { public:
    class CGroup { public:                              // 8 bytes (Ego_r.pdb nested class)
        unsigned long BoneIndex;
        float         BoneWeight;
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);   // 0x00AC6B40
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupLess)(const CGroup&, const CGroup&);
namespace std {
template <class _Iterator> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
const int __stl_threshold = 16;
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00AD6F10
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                                 // 0x00AD5B50
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);          // 0x00AD1CA0
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Compare>
__forceinline void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)   // inlined by retail here (out of line in the CScriptManager sort 0x00CBDF70)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __lg(__last - __first) * 2, __comp);   // _STLP_VALUE_TYPE
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CGroup*, PFNGroupLess>(CGroup*, CGroup*, PFNGroupLess);
}
''')
a = '00ad4e20'
if a not in _X:
    he.edit(a, 'src', 'class CUnprovenElement_00ad4e20 { public: unsigned long Word0, Word1; };   // descriptive 8-byte element\n',
            'namespace QuadricOptimiserInternals { class COptimisedVertex { public:\n    class CGroup { public: unsigned long BoneIndex; float BoneWeight; };   // the 8-byte {BoneIndex, BoneWeight} record (Ego_r.pdb nested class; chunk 687 LimitGroupCount 0x00ACA5F0, naming wave 20)\n}; }\ntypedef QuadricOptimiserInternals::COptimisedVertex::CGroup CUnprovenElement_00ad4e20;   // (the typedef keeps the row text; the symbol is the CGroup instantiation)\n')
    he.edit(a, 'src', '// Leaf: ?_M_insert_overflow@?$vector@VCUnprovenElement_00ad4e20@@V?$allocator@\n', '// Leaf: ?_M_insert_overflow@?$vector@VCGroup@COptimisedVertex@QuadricOptimiserInternals@@V?$allocator@\n')
    he.resub(a, 'src', r'(?m)^// Identity: element `CUnprovenElement_00ad4e20` -- DESCRIPTIVE element type: `CUnprovenElement_00ad4e20` is a placeholder for an unidentified 8-byte element; evidence: its callers are mes',
             '// Identity (naming wave 20, chunk 687): the element is QuadricOptimiserInternals::COptimisedVertex::CGroup, the 8-byte {BoneIndex, BoneWeight} record of the landed LimitGroupCount 0x00ACA5F0 (which binds this instantiation) and CompareWeight 0x00AC6B40; was the CUnprovenElement_00ad4e20 placeholder. Old note: its callers are mes')
    he.resub(a, 'src', r'(?m)^// std::vector::_M_insert_overflow\(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend\) -- \(vector<CUnprovenElement_00ad4e20>\)', '// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<QuadricOptimiserInternals::COptimisedVertex::CGroup>)')
a = '00c73a00'
if a not in _X:
    he.resub(a, 'src', r'\bClassifyShaderExtension\b', 'ClassifyAudioExtension')
    he.edit(a, 'src', '// Leaf: ?ClassifyAudioExtension@CString_00c73a00@@\n', '// Leaf: ?ClassifyAudioExtension@CString_00c73a00@@\n// Name (naming wave 20, chunk 687): the extension is an audio one (.ogg -> 1 = COggVorbisStream, .wma -> 0 = the null audio stream in the\n//   caller 0x00C73AF0, CDecompressorInterface.cpp), not a shader; was ClassifyShaderExtension.\n')
    he.resub(a, 'tst', r'\bClassifyShaderExtension\b', 'ClassifyAudioExtension')
a = '00ca6620'
if a not in _X:
    he.edit(a, 'src', 'unsigned long __stdcall WaitDispatchingMessages(void* hObject, unsigned long dwWait, void* hwnd = 0,\n                                                unsigned int uMsg = 0, void* hEvent = 0);   // 0x00CB09A0 (wxutil.cpp)\n',
            'struct HWND__;\nunsigned long __stdcall WaitDispatchingMessages(void* hObject, unsigned long dwWait, HWND__* hwnd = 0,\n                                                unsigned int uMsg = 0, void* hEvent = 0);   // 0x00CB09A0 (wxutil.cpp: ?WaitDispatchingMessages@@YGKPAXKPAUHWND__@@I0@Z, the landed chunk-687 leaf; naming wave 20)\n')
a = '00aac530'
if a not in _X:
    he.edit(a, 'src', 'class CGameScriptThing { public:\n    long DecrementScriptCounter();                 // 0x00A3E420\n};\n', 'class CSoundManagerLH { public:\n    long GetNextEmitterID();                       // 0x00A3E420 (chunk 687: CSoundManagerLH::GetNextEmitterID, the post-incremented emitter counter at +0x50; naming wave 20)\n};\n')
    he.resub(a, 'src', r'CGameScriptThing\*\s+m_pThing;\s+// \+0x04', 'CSoundManagerLH*     m_pManager;               // +0x04')
    he.resub(a, 'src', r'CSoundEmitterLH\(CGameScriptThing\* thing\)', 'CSoundEmitterLH(CSoundManagerLH* manager)')
    he.edit(a, 'src', '    , m_pThing(thing)\n', '    , m_pManager(manager)\n')
    he.edit(a, 'src', '    , m_scriptCounter(m_pThing->DecrementScriptCounter())\n', '    , m_number(m_pManager->GetNextEmitterID())\n')
    he.resub(a, 'src', r'long\s+m_scriptCounter;\s+// \+0x1c', 'long                 m_number;                 // +0x1c (descriptive)')
    he.edit(a, 'src', '-- CBaseClass, the thing at +4,', '-- CBaseClass, the owning manager at +4,')
    he.edit(a, 'src', "the thing's script counter value at +0x1c", "the manager's next emitter id at +0x1c")
    he.edit(a, 'src', "//   CGameScriptThing::DecrementScriptCounter @ 0x00A3E420 (post-increments the thing's counter; result at +0x1c).", "//   CSoundManagerLH::GetNextEmitterID @ 0x00A3E420 (post-increments the manager's emitter counter; result at +0x1c; chunk 687 -- the landed\n//   CSoundManagerLH ctor 0x00A3EF00 passes `this`; was the propagated CGameScriptThing::DecrementScriptCounter -- naming wave 20).")
    he.resub(a, 'tst', r'\bCGameScriptThing\b', 'CSoundManagerLH')
    he.resub(a, 'tst', r'\bDecrementScriptCounter\b', 'GetNextEmitterID')
a = '00aac580'
if a not in _X:
    he.edit(a, 'src', '    long TakeNextEmitterNumber();                  // 0x00A3E420 (descriptive): return EmitterCounter++ (+0x50)\n', '    long GetNextEmitterID();                       // 0x00A3E420 (chunk 687: CSoundManagerLH::GetNextEmitterID; return EmitterCounter++ (+0x50); naming wave 20)\n')
    he.resub(a, 'src', r'\bTakeNextEmitterNumber\b', 'GetNextEmitterID')
    he.resub(a, 'tst', r'\bTakeNextEmitterNumber\b', 'GetNextEmitterID')
