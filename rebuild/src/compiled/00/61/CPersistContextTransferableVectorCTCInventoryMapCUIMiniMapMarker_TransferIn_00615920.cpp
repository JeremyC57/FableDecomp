// CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext* context, CStringParser* parser, long index) -- parses mini-map marker `index` through a default CPersistTraits<CUIMiniMapMarker>
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00615920, 120 bytes (+8 int3 padding), `ret 0Ch`.
// Callees: 0x0099EBF0 CCharString::CCharString(const char*, long) with "" @ 0x0122D70E; 0x006159A0
// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext*, CStringParser*, CUIMiniMapMarker*)
// (thiscall, `ret 0Ch`; manifest label `TransferOut_HeroTraits` is a guess); 0x0099EAE0 CCharString::~CCharString.
// Persist READ hook (vtable 0x01257164 slot 3; slot 2 is TransferOut 0x00614000, slot 1 Resize 0x006193B0, slot 0 the
// count 0x00613FE0) of CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>. The manifest's
// `std::pair<CCharString,...>` is propagated: the 28-byte stride (`imul ecx,ecx,1Ch`) and the traits callee name the
// type (Ego_r.pdb CUIMiniMapMarker 0x1c bytes: Region, CCharString UIDefName, C2DVector Position, DefNameLength,
// GraphicIndex, float Angle). The record's inline default constructor stores Region = 0, builds UIDefName("") in
// place (`lea ecx,[esp+10h]` = traits+4) and then zeroes Position..Angle with immediates; `this` is parked in esi
// across the string constructor. The implicit traits destructor destroys only the string (traits+4).
// The module is built without C++ exceptions (no EH frame around the string).
struct CPersistContext;
struct CStringParser;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class C2DVector {
public:
    float X;
    float Y;
    C2DVector() : X(0.0f), Y(0.0f) { }
};

struct CUIMiniMapMarker {                          // CTCInventoryMap::CUIMiniMapMarker, 0x1c bytes
    long        Region;                            // +0x00
    CCharString UIDefName;                         // +0x04
    C2DVector   Position;                          // +0x08
    long        DefNameLength;                     // +0x10
    long        GraphicIndex;                      // +0x14
    float       Angle;                             // +0x18
    CUIMiniMapMarker() : Region(0), UIDefName(""), Position(), DefNameLength(0), GraphicIndex(0), Angle(0.0f) { }
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);       // 0x00614080 for CUIMiniMapMarker
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);   // 0x006159A0 for CUIMiniMapMarker
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CUIMiniMapMarker>* TransVector;      // +0x4
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CUIMiniMapMarker> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}