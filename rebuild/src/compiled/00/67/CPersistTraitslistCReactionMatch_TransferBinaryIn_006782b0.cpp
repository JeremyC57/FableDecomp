// CPersistTraits<list<CReactionMatch*>>::TransferBinaryIn(CMemoryDataInputStream& stream, list<CReactionMatch*>& value) -- binary read of a reaction-match list: the count, clear the list, then per entry the match type, a new default match of that class reading itself, appended
// Leaf: ?TransferBinaryIn@?$CPersistTraits@V?$list@PAVCReactionMatch@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006782B0, 412 bytes of code + the 4-entry case table (0x0067844C), `ret 8`
//   (traits `this` unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, free @ 0x00BFEA14 / malloc @ 0x00BFEA0E (STLport's
//   node allocator), operator new @ 0x00BFEA1A, CReactionMatch::TransferBinaryIn (virtual slot 3, `call [eax+0Ch]`).
// Classes (vtables from rtti-vtables.tsv; slot 3 = the landed TransferBinaryIn overrides): type 0
//   CReactionMatch_SecondaryAxisMRShifted 0x0125B6E8 (0x24 bytes), 1 CReactionMatch_SecondaryAxisCentredMRShifted 0x0125B700
//   (0x24), 2 CReactionMatch_AsymmetricPyramid 0x0125B718 (0x28), 3 CReactionMatch_MRPyramid 0x0125B730 (0x34); any other
//   type creates nothing. Local polymorphic models (the engine headers carry the vptr as a data member).
// Shape: the count through an inline CPersistTraits<long>::TransferBinaryIn (speed TU, `position += size`), STLport's
//   inline list::clear (walk freeing nodes, relink the header), a `count > 0` loop (down-counter in the frame word), the
//   type read into the dead stream slot, an inline factory switch (`new` + default constructor: vtable, ReactionType 0,
//   the null tests merged), the virtual read, then STLport's inline push_back (malloc'd 12-byte node, null-checked
//   placement copy at +8, linked before the header). No frame pointer.
#include "engine/CMemoryDataInputStream.h"

extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
void* __cdecl operator new(unsigned int size);        // 0x00BFEA1A
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CMemoryDataInputStream;
// Method view over the shared engine header (Ego_r.pdb layout: position +4, chunkBase +0xC = current chunk read
// pointer, chunkLength +0x14 = bytes left in the chunk); the overflow reader is CDataInputStream's.
struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, (const void*)chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

enum EOpinionReactionType { OPINION_REACTION_TYPE_NULL = 0, OPINION_REACTION_TYPE_FORCE_DWORD = 0x7fffffff };
enum EReactionMatchType {
    REACTION_MATCH_SECONDARY_AXIS_MR_SHIFTED = 0,
    REACTION_MATCH_SECONDARY_AXIS_CENTRED_MR_SHIFTED = 1,
    REACTION_MATCH_ASYMMETRIC_PYRAMID = 2,
    REACTION_MATCH_MR_PYRAMID = 3,
    REACTION_MATCH_FORCE_DWORD = 0x7fffffff
};

class CReactionMatch {                                 // { vptr, ReactionType +4 }
public:
    EOpinionReactionType ReactionType;                 // +0x04
    CReactionMatch() : ReactionType(OPINION_REACTION_TYPE_NULL) {}
    virtual ~CReactionMatch();                         // slot 0
    virtual float Match(const class COpinionSet& set) = 0;                    // slot 1
    virtual EReactionMatchType GetMatchType() const = 0;                      // slot 2
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream) = 0;        // slot 3
    virtual void TransferBinaryOut(class CDataOutputStream& stream) const = 0; // slot 4
    static CReactionMatch* CreateFromType(EReactionMatchType type);
};

class CReactionMatch_SecondaryAxisMRShifted : public CReactionMatch {         // vtable 0x0125B6E8, 0x24 bytes
public:
    long  Axis;                                        // +0x08 EOpinion
    float LowerBoundOnAxis, InvIntervalOnAxis, MShiftZero, MShiftWeight, RShiftZero, RShiftWeight;
    virtual ~CReactionMatch_SecondaryAxisMRShifted();
    virtual float Match(const class COpinionSet& set);
    virtual EReactionMatchType GetMatchType() const;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(class CDataOutputStream& stream) const;
};
class CReactionMatch_SecondaryAxisCentredMRShifted : public CReactionMatch {  // vtable 0x0125B700, 0x24 bytes
public:
    long  Axis;                                        // +0x08 EOpinion
    float CentreOnAxis, InvHalfIntervalOnAxis, MShiftZero, MShiftWeight, RShiftZero, RShiftWeight;
    virtual ~CReactionMatch_SecondaryAxisCentredMRShifted();
    virtual float Match(const class COpinionSet& set);
    virtual EReactionMatchType GetMatchType() const;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(class CDataOutputStream& stream) const;
};
class CReactionMatch_AsymmetricPyramid : public CReactionMatch {              // vtable 0x0125B718, 0x28 bytes
public:
    long  Axis1, Axis2;                                // +0x08, +0x0C EOpinion
    float Values[6];                                   // +0x10
    virtual ~CReactionMatch_AsymmetricPyramid();
    virtual float Match(const class COpinionSet& set);
    virtual EReactionMatchType GetMatchType() const;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(class CDataOutputStream& stream) const;
};
class CReactionMatch_MRPyramid : public CReactionMatch {                      // vtable 0x0125B730, 0x34 bytes
public:
    float Values[8];                                   // +0x08
    float ScarinessShift, AgreeablenessShift, AttractivenessShift;            // +0x28
    virtual ~CReactionMatch_MRPyramid();
    virtual float Match(const class COpinionSet& set);
    virtual EReactionMatchType GetMatchType() const;
    virtual void TransferBinaryIn(CMemoryDataInputStream& stream);
    virtual void TransferBinaryOut(class CDataOutputStream& stream) const;
};

__forceinline CReactionMatch* CReactionMatch::CreateFromType(EReactionMatchType type)
{
    switch (type) {
    case REACTION_MATCH_SECONDARY_AXIS_MR_SHIFTED:
        return new CReactionMatch_SecondaryAxisMRShifted;
    case REACTION_MATCH_SECONDARY_AXIS_CENTRED_MR_SHIFTED:
        return new CReactionMatch_SecondaryAxisCentredMRShifted;
    case REACTION_MATCH_ASYMMETRIC_PYRAMID:
        return new CReactionMatch_AsymmetricPyramid;
    case REACTION_MATCH_MR_PYRAMID:
        return new CReactionMatch_MRPyramid;
    }
    return 0;
}

namespace _STL {
template <class _Tp> class allocator {};
struct _List_node_base {
    _List_node_base* _M_next;                          // +0x00
    _List_node_base* _M_prev;                          // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                       // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;                          // the header node (malloc'd)
    void clear()
    {
        _Node* __cur = (_Node*)_M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    void insert(_List_node_base* __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    void push_back(const _Tp& __x) { insert(_M_node, __x); }
};
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream& stream, long* value)
    {
        long number;
        static_cast<CMemoryDataInputStreamReader&>(stream).Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<EReactionMatchType> {
    __forceinline void TransferBinaryIn(CMemoryDataInputStream& stream, EReactionMatchType* value)
    {
        EReactionMatchType number;
        static_cast<CMemoryDataInputStreamReader&>(stream).Read(&number, sizeof(number));
        *value = number;
    }
};
template <> struct CPersistTraits<_STL::list<CReactionMatch*> > {
    void TransferBinaryIn(CMemoryDataInputStream& stream, _STL::list<CReactionMatch*>& value);
};

void CPersistTraits<_STL::list<CReactionMatch*> >::TransferBinaryIn(CMemoryDataInputStream& stream,
                                                                    _STL::list<CReactionMatch*>& value)
{
    long count;
    CPersistTraits<long>().TransferBinaryIn(stream, &count);
    value.clear();
    for (long i = 0; i < count; ++i) {
        EReactionMatchType type;
        CPersistTraits<EReactionMatchType>().TransferBinaryIn(stream, &type);
        CReactionMatch* match = CReactionMatch::CreateFromType(type);
        if (match != 0) {
            match->TransferBinaryIn(stream);
            value.push_back(match);
        }
    }
}