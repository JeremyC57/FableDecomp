// Grade: DIFFER(668v668) -- register allocation only: retail builds the element pointer through edx and loads UsageIndex into cl ahead of the Type store (ours: ecx/dl, loaded after), counts Streams.size() into ecx at the second loop (ours eax), and orders the two D3DDECL_END dword stores the other way; jump table, frame and switch block order match
// CVertexShaderInput::GenerateD3DDeclaration() -- rebuild the D3DVERTEXELEMENT9 array from the stream/element description: one entry per element (stream, running offset, D3D type, TEXCOORD usage), then D3DDECL_END; an element type with no D3D equivalent clears the declaration
// Leaf: ?GenerateD3DDeclaration@CVertexShaderInput@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB3870, plain `ret`, jump table at 0x00AB3AD4 (14 cases, element byte sizes).
// Callees: STLport vector<D3DVERTEXELEMENT9>::_M_fill_insert 0x00AB4260 (same-TU, out of line); the element-type -> D3DDECLTYPE
//   table 0x0129E9CC {0,1,2,3,4,6,7,6,-1 x6}.
// Layout: from the body -- +0x04 vector<SStream> (0x14-byte entries; Elements vector at +0x08), +0x10 vector<D3DVERTEXELEMENT9>
//   (12-byte STLport vectors); SElement {BYTE UsageIndex +0; long Type +4}. The landed ctor 0x00AB3B10 models the same six
//   dwords descriptively.
struct D3DVERTEXELEMENT9 {
    unsigned short Stream;
    unsigned short Offset;
    unsigned char  Type;
    unsigned char  Method;
    unsigned char  Usage;
    unsigned char  UsageIndex;
};

struct SElement {
    unsigned char UsageIndex;   // +0x00
    long          Type;         // +0x04
};

template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};

struct SStream {
    long                   Pad0;      // +0x00
    long                   Pad4;      // +0x04
    CRetailVector<SElement> Elements; // +0x08
};

struct CDeclarationVector {
    D3DVERTEXELEMENT9* _M_start;
    D3DVERTEXELEMENT9* _M_finish;
    D3DVERTEXELEMENT9* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    D3DVERTEXELEMENT9& operator[](unsigned int n) { return *(_M_start + n); }
    void _M_fill_insert(D3DVERTEXELEMENT9* pos, unsigned int n, const D3DVERTEXELEMENT9& x);   // 0x00AB4260
    static __forceinline D3DVERTEXELEMENT9* copy(D3DVERTEXELEMENT9* first, D3DVERTEXELEMENT9* last, D3DVERTEXELEMENT9* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    __forceinline void erase(D3DVERTEXELEMENT9* first, D3DVERTEXELEMENT9* last)
    {
        D3DVERTEXELEMENT9* i = copy(last, _M_finish, first);
        _M_finish = i;
    }
    __forceinline void resize(unsigned int n, const D3DVERTEXELEMENT9& x)
    {
        if (n < size())
            erase(_M_start + n, _M_finish);
        else
            _M_fill_insert(_M_finish, n - size(), x);
    }
    __forceinline void clear() { erase(_M_start, _M_finish); }
};

extern const long g_VertexTypeToD3DDeclType[14];   // 0x0129E9CC

struct CBase {
    virtual void v00();
};

class CVertexShaderInput : public CBase {
public:
    CRetailVector<SStream> Streams;   // +0x04
    CDeclarationVector     Declaration; // +0x10
    void GenerateD3DDeclaration();
};

static __forceinline long GetVertexTypeSize(long type)
{
    switch (type) {
    case 0: return 4;
    case 4: return 4;
    case 5: return 4;
    case 7: return 4;
    case 9: return 4;
    case 12: return 4;
    case 13: return 4;
    case 1: return 8;
    case 6: return 8;
    case 11: return 8;
    case 2: return 12;
    case 3: return 16;
    case 8: return 2;
    case 10: return 6;
    }
    return 0;
}

void CVertexShaderInput::GenerateD3DDeclaration()
{
    long offset = 0;
    unsigned int count = 0;
    for (unsigned int i = 0; i < Streams.size(); ++i)
        count += Streams[i].Elements.size() + 1;
    D3DVERTEXELEMENT9 el = D3DVERTEXELEMENT9();
    Declaration.resize(count, el);
    unsigned int out = 0;
    for (unsigned int stream = 0; stream < Streams.size(); ++stream) {
        SStream& s = Streams[stream];
        unsigned int n = s.Elements.size();
        for (unsigned int j = 0; j < n; ++j) {
            SElement& e = s.Elements[j];
            long type = e.Type;
            long d3dType = g_VertexTypeToD3DDeclType[type];
            if (d3dType == -1) {
                Declaration.clear();
                return;
            }
            el.Stream = (unsigned short)stream;
            el.Offset = (unsigned short)offset;
            el.Type = (unsigned char)d3dType;
            el.Method = 0;
            el.Usage = 5;
            el.UsageIndex = e.UsageIndex;
            Declaration[out++] = el;
            offset += GetVertexTypeSize(type);
        }
    }
    el.Stream = 0xff;
    el.Offset = 0;
    el.Type = 17;
    el.Method = 0;
    el.Usage = 0;
    el.UsageIndex = 0;
    Declaration[out] = el;
}
