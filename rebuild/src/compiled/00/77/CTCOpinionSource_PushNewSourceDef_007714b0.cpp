// CTCOpinionSource::PushNewSourceDef(long defIndex) -- set the source up from the def, then push its index on DefStack
// retail keeps it in the parameter slot (both lea point at the argument). The const& form loses the edi cache instead (84v87).
// Leaf: PushNewSourceDef
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

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_long {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(long value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* FindRef_CRetailVector_long_InsertGrow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CRetailVector_long::InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd)
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
    CRetailVector_long DefStack;            // +0x0c
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