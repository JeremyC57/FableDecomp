// CTCOpinionSource::PushNewSourceDef(long defIndex) -- set the source up from the def, then push its index on DefStack
// retail keeps it in the parameter slot (both lea point at the argument). The const& form loses the edi cache instead (84v87).
// Leaf: PushNewSourceDef
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CRetailVector_long::InsertGrow(..., const SScalarTag&, unsigned long, bool).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007714B0, length 87 bytes (+9 int3). Manifest label
// "CTCCreatureGroupTag::DeactivateMember" is propagated: the body sits between CTCOpinionSource::SetupFromDef
// (0x007713C0) and AppliesToWitness (0x00771540); its callers are the by-name PushNewSourceDef(const CCharString&)
// @ 0x00771620 (def index looked up through CDefinitionManager::GetDefGlobalIndexFromName) and 0x0070089F. The pop
// twin is 0x00771510 (DefStack.pop_back + SetupFromDef(back)). The (long) overload name is inferred from those callers.
// Callees: CTCOpinionSource::SetupFromDef(long) @ 0x007713C0, the retail vector<long> reallocating insert @ 0x00440990.
// Shape: DefStack (+0x0c, PDB name) is the 12-byte retail vector {first, last, end}; push_back is inlined: placement
// store at `last` (null-tested) and `++last`, else the insert with the empty tag temporary homed in the argument's own
// slot (both `lea` push the parameter's address).
#include <new>


namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* first;
    _Tp* last;
    _Tp* end;
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool atEnd);   // 0x00440990
public:
    void push_back(_Tp value)
    {
        if (last != end) {
            new (last) _Tp(value);
            ++last;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
};
}

void* FindRef_CRetailVector_long_InsertGrow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool atEnd)
{
    void* p = FindRef_CRetailVector_long_InsertGrow_0();
    if (!p)
        return;
    *(void**)&tag = p;
    
}
#pragma auto_inline(on)

class CThing;

class CTCOpinionSource { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    std::vector<long> DefStack;            // +0x0c
    bool SetupFromDef(long defIndex);       // 0x007713C0
    bool PushNewSourceDef(long defIndex);
};

bool CTCOpinionSource::PushNewSourceDef(long defIndex)
{
    if (SetupFromDef(defIndex)) {
        DefStack.push_back(defIndex);
        return true;
    }
    return false;
}