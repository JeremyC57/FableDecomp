// Grade: DIFFER(565v565) -- one load order: retail mov edx,[ecx+4]; mov [esp+20h],eax; sub edx,[ecx] (finish first, start from memory), ours stores the CRC first then mov eax,[ecx]; mov edx,[ecx+4]; sub edx,eax (+ loop-head pad length follows)
// NParticleEngine::CPSCSplineBase::CSplinePosParamData::DatabaseSerialise(CPersistContext& context, bool) -- persist MaintainSplineShape; when loading, read the parameter count, size SplinePosParam (at least the spline's point count, filled with the CRC of ""), load each indexed parameter name as its CRC, and clear the array if every CRC is zero
// Leaf: DatabaseSerialise@CSplinePosParamData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21A50, 576 bytes, `ret 8`.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, long&, const long&) 0x00410BE0,
//   Transfer(name, CCharString&, const CCharString&) 0x004109A0, CRC Calc(seed, data, length) 0x004014A0 (__fastcall),
//   CArray<unsigned long>::resize(n, const unsigned long&) 0x00464931, CCharString() 0x0099E4B0, CCharString(const
//   CCharString&) 0x0099EC30, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, GFIntToCharString
//   0x0099F830, operator+ 0x0099F570, operator const char* 0x0099E4C0, memmove 0x00BFEAE6 (inline STLport erase).
// Data: "MaintainSplineShape" 0x012A0BBC, "SplinePosParamSize" 0x012A0BA8, "SplinePosParamSize_" 0x012A0B94,
//   "" 0x0129AAF4, CRC-32 table 0x0129A168.
// Layout: as the landed ReadBinary 0x00B21C90: Spline +0x4, MaintainSplineShape +0x8, SplinePosParam +0xC.
// Shape: the twin of CSplineRandomOffsetData::DatabaseSerialise 0x00B21E90 (SaveValue wrapper builds the name first,
//   `count > size ? count : size`); the empty default is the landed EmptyCCharString() by-value idiom; retail calls the
//   CRC routine out of line for the resize default but inlines its loop for each loaded name (an inline copy here).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

struct C3DVector { float X, Y, Z; };

struct CCharStringData {
    const char* Data;                                   // +0
    long Length;                                        // +4
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetChars() const { return PStringData == 0 ? "" : PStringData->Data; }
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}

unsigned long __fastcall CRCCalc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
extern const unsigned long g_crcTable[256];                                                      // 0x0129A168
__forceinline unsigned long CRCCalcInline(unsigned long seed, const char* data, unsigned long length)
{
    if (length)
        for (unsigned long k = 0; k < length; ++k)
            seed = g_crcTable[(unsigned char)data[k] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}
inline unsigned long GetCRC(const CCharString& s)
{
    return CRCCalc(0, s.GetChars(), s.GetLength());
}
__forceinline unsigned long GetCRCInline(const CCharString& s)
{
    const char* data;
    unsigned long length;
    if (s.PStringData == 0) {
        length = 0;
        data = "";
    } else {
        length = s.PStringData->Length;
        data = s.PStringData->Data;
    }
    return CRCCalcInline(0, data, length);
}

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                   // 0x004045C0
    void Transfer(const char* name, long& value, const long& defaultValue);                   // 0x00410BE0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);     // 0x004109A0
};
__forceinline void LoadValue(CPersistContext& context, const char* name, CCharString& value)
{
    context.Transfer(name, value, EmptyCCharString());
}

__forceinline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}

template <class T> struct CArray {              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned int newSize, const T& value);   // 0x00464931 (T = unsigned long)
    __forceinline T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    __forceinline void clear() { erase(begin(), end()); }
};

struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;    // +0x00
    float             Tension;          // +0x0C
    bool              EndPointsConnected; // +0x10
};
struct CParticleSystem;

namespace NParticleEngine {
struct CPSCSplineBase {
    struct CSplinePosParamData {
        CParticleSystem*      PParentParticleSystem;   // +0x00
        CCardinalSpline*      Spline;                  // +0x04
        bool                  MaintainSplineShape;     // +0x08
        CArray<unsigned long> SplinePosParam;          // +0x0C
        void DatabaseSerialise(CPersistContext& context, bool transfer);
    };
};

void CPSCSplineBase::CSplinePosParamData::DatabaseSerialise(CPersistContext& context, bool)
{
    context.Transfer("MaintainSplineShape", MaintainSplineShape, false);
    if (context.IsSaving())
        return;
    long count;
    context.Transfer("SplinePosParamSize", count, 0L);
    SplinePosParam.resize(count > (long)Spline->ControlPoints.size() ? count : Spline->ControlPoints.size(),
        GetCRC(CCharString()));
    bool anySet = false;
    for (long i = 0; i < count; ++i) {
        CCharString name;
        LoadValue(context, CCharString("SplinePosParamSize_") + GFIntToCharString(i), name);
        SplinePosParam[i] = GetCRCInline(name);
        anySet |= SplinePosParam[i] != 0;
    }
    if (!anySet)
        SplinePosParam.clear();
}
}
