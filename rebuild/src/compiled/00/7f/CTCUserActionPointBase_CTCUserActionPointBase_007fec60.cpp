// CTCUserActionPointBase::CTCUserActionPointBase(CThing*) -- CTCBase(owner), own vtable, two empty index vectors, zero point count and next-check frame, an empty PThing pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FEC60, length 68 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Vtable 0x0127216C = CTCUserActionPointBase.
// Shape: one `xor eax,eax` zero register for all eight dwords; the vectors are STLport _Vector_base (start/finish
// initialisers, the end pointer an _STLP_alloc_proxy member built from an allocator temporary), the two longs are
// init-list members, then PThing's inline constructor (`lea edi,[esi+2Ch]` scheduled among the zero stores).
// Layout: Ego_r.pdb CTCUserActionPointBase with 12-byte retail vectors: FreeActionPointIndexes +0x0C,
// BlockedActionPointIndexes +0x18, NumberOfActionPoints +0x24, FrameOfNextCheck +0x28, PThing +0x2C. No engine header.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing*       Thing;                    // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* thing);                 // 0x00686800
};

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {          // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CLongAllocator {                     // std::allocator<long> (empty)
    CLongAllocator() { }
};

struct CLongAllocProxy : CLongAllocator {   // STLport _STLP_alloc_proxy<long*, long, allocator<long> >
    long* _M_data;
    CLongAllocProxy(const CLongAllocator& a, long* p) : CLongAllocator(a), _M_data(p) { }
};

struct CLongVectorBase {                    // STLport _Vector_base<long>
    long*           _M_start;
    long*           _M_finish;
    CLongAllocProxy _M_end_of_storage;
    CLongVectorBase(const CLongAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};

struct CLongVector : CLongVectorBase {      // std::vector<long> (retail 12 bytes)
    explicit CLongVector(const CLongAllocator& a = CLongAllocator()) : CLongVectorBase(a) { }
};

struct CTCUserActionPointBase : CTCBase {
    CLongVector         FreeActionPointIndexes;      // +0x0C
    CLongVector         BlockedActionPointIndexes;   // +0x18
    long                NumberOfActionPoints;        // +0x24
    long                FrameOfNextCheck;            // +0x28
    CIntelligentPointer<CThing> PThing;                      // +0x2C
    virtual void v00();
    CTCUserActionPointBase(CThing* thing);
};

CTCUserActionPointBase::CTCUserActionPointBase(CThing* thing)
    : CTCBase(thing), NumberOfActionPoints(0), FrameOfNextCheck(0)
{
}