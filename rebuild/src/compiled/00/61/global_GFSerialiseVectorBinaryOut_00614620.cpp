// GFSerialiseVectorBinaryOut(CDataOutputStream& stream, const vector<CTCInventoryMap::CUIMiniMapMarker>& value) -- write the count, then each marker's region, name, position, graphic and angle
// Leaf: GFSerialiseVectorBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614620, length 209 bytes (__fastcall stream, value).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, WriteSLONG 0x00993EB0 (NF197: respelled from the
//   stand-in CDataOutputStreamWriter::WriteLong onto the row's CDataOutputStream class, Ego_r.pdb ?WriteSLONG@CDataOutputStream@@QAEXJ@Z), WriteString 0x009959F0,
//   WriteFloat 0x00993EE0.
// Model: engine/CDataOutputStream.h + writer adapter; local CUIMiniMapMarker (no engine header).
// Shape: speed TU; the marker traits' TransferBinaryOut is inline: Region, the name length, the name, Position.X/Y, GraphicIndex, Angle.
#include "fable_compiler.h"
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x009959F0 ?WriteString@CDataOutputStream@@QAEXABVCCharString@@@Z (was ?WriteString@CDataOutputStreamWriter@@QAEXABVCCharString@@@Z)
class CDataOutputStream : public CDataOutputStreamLayout
{
public:
    void WriteSLONG(long value);                            // 0x00993EB0 ?WriteSLONG@CDataOutputStream@@QAEXJ@Z (Ego_r.pdb)
    void WriteFloat(float);   // 0x00993EE0
 public: void WriteString(CCharString const&); };
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CDataOutputStreamWriter : CDataOutputStream {
};
class C2DVector { public:
    float X, Y;
};
struct CUIMiniMapMarker {
    long        Region;
    CCharString UIDefName;
    C2DVector   Position;
    long        DefNameLength;
    long        GraphicIndex;
    float       Angle;
    CUIMiniMapMarker() : Region(0), UIDefName("", -1)
    {
        Position.X = 0;
        Position.Y = 0;
        DefNameLength = 0;
        GraphicIndex = 0;
        Angle = 0;
    }
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const;
};
template <> FABLE_FORCEINLINE void CPersistTraits<CUIMiniMapMarker>::TransferBinaryOut(CDataOutputStreamWriter& stream, const CUIMiniMapMarker* value) const
{
    stream.WriteSLONG(value->Region);
    stream.WriteSLONG(value->DefNameLength);
    reinterpret_cast<CDataOutputStream*>(&(stream))->WriteString(value->UIDefName);
    stream.WriteFloat(value->Position.X);
    stream.WriteFloat(value->Position.Y);
    stream.WriteSLONG(value->GraphicIndex);
    stream.WriteFloat(value->Angle);
}
template <class T>
void __fastcall GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, std::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}
template void __fastcall GFSerialiseVectorBinaryOut<CUIMiniMapMarker>(CDataOutputStreamWriter& stream, std::vector<CUIMiniMapMarker>& value);