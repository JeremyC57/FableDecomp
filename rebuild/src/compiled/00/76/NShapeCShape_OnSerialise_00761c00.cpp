// NShape::CShape::OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset) -- persist shape `index` as "Shape[<index>]": its type name ("<name>.Type", only for the two known shape types), the point count ("<name>.size()") and every point ("<name>.pos[<i>].X/.Y/.Z", X and Y relative to the integer offset)
// Leaf: ?OnSerialise@CShape@NShape@@UAEXAAVCPersistContext@@JABVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00761C00, 821 bytes (+ int3 padding), thiscall `ret 0Ch`; NShape::CShape vtable 0x0126A208 slot 2
//   (Ego_r.pdb ?OnSerialise@CShape@NShape@@UAEXAAVCPersistContext@@JABVC2DCoordI@@@Z).
// Callees: CCharString(const char*, long) 0x0099EBF0, GFIntToCharString(long) 0x0099F830 (__fastcall, hidden result in ecx),
//   operator+(const CCharString&, const CCharString&) 0x0099F570 / (const CCharString&, const char*) 0x0099F600 (__fastcall),
//   ~CCharString 0x0099EAE0, CCharString() 0x0099E4B0, CCharString(const CCharString&) 0x0099EC30, operator= 0x0099EFB0,
//   operator const char* 0x0099E4C0, CPersistContext::Transfer<CCharString> 0x004109A0 / <long> 0x00410BE0 / <float> 0x00410620
//   (the three-argument member template, ??$Transfer@..@CPersistContext@@QAEXPBDAA..AB..@Z), the shape's GetType (slot 1).
// Data: the type names are the file's anonymous-namespace CCharStrings CLOSED_STRING_NAME 0x013BAD8C ("SHAPE_TYPE_CLOSED",
//   initializer 0x0121EE30) and LINE_STRING_NAME 0x013BAD90 ("SHAPE_TYPE_LINE", 0x0121EE50): Ego_r.pdb's adjacent dynamic
//   initializers for CLOSED_STRING_NAME / LINE_STRING_NAME (same order), the literals, and GetType 0 = CClosedShapeBase.
//   Landed bindings call them g_CCharString_013bad8c / _90 (NAME_FIXES).
// Layout: Ego_r.pdb NShape::CShape: Owner +0x4, Points vector<C3DVector> +0x8 (12-byte STLport vector).
// Shape: CCharString temporaries joined by operator+ (".X/.Y/.Z" use the const char* overload), the point index
//   post-incremented inside the name expression; the two-argument Transfer<T> is the PDB member template forwarding
//   GetDefaultVal<T>() (protected static __fastcall, KI). Transfer<float> carries its real body (the landed 0x00410620 source)
//   as a same-TU noinline specialisation: VC7.1 then sees that the 0.0f default temporaries are only read and hoists their
//   three stores into the loop preheader, as retail does (the declaration-only form left them in the loop: 816v821).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                           // 0x0099E4B0
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
    operator const char*() const;                            // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                              // 0x0099F830
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
namespace {
CCharString CLOSED_STRING_NAME("SHAPE_TYPE_CLOSED");         // 0x013BAD8C (dynamic initializer 0x0121EE30)
CCharString LINE_STRING_NAME("SHAPE_TYPE_LINE");             // 0x013BAD90 (dynamic initializer 0x0121EE50)
}

#include "engine/CMemoryDataInputStream.h"
#define CDataOutputStream CDataOutputStreamLayout
#include "engine/CDataOutputStream.h"
#undef CDataOutputStream

class CDataOutputStream : public CDataOutputStreamLayout { public:
    void WriteEBOOL(bool value);                 // 0x00993E30
    void WriteSLONG(long value);                 // 0x00993EB0
    void WriteUHUGE(unsigned __int64 value);     // 0x00993EC0
    void WriteFloat(float value);                // 0x00993EE0
};
class CStringParser;
class CPersistContext;

template <class T> class CPersistTraits { public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value);   // 0x0040EFB0 for T = float
    void TransferBinaryOut(CDataOutputStream& stream, const T& value) { stream.WriteSLONG(value); }
};
template <> inline void CPersistTraits<float>::TransferBinaryOut(CDataOutputStream& stream, const float& value) { stream.WriteFloat(value); }

class CPersistContext_TransferableComponentGenericType { public:
    CPersistContext_TransferableComponentGenericType() {}
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;      // slot 0
    virtual void TransferIn(CPersistContext& context, CStringParser& parser) = 0;  // slot 1
};
template <class T> class CPersistContext_TransferableComponent : public CPersistContext_TransferableComponentGenericType { public:
    T* Object;
    CPersistContext_TransferableComponent(T& object) : Object(&object) {}
private:
    virtual void TransferOut(CPersistContext& context, CCharString& out);
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};

class CPersistContext {
public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    void*                   PDefinitionManager;   // +0x00
    void*                   PSymbolMap;           // +0x04
    CStringParser*          PParser;              // +0x08
    void*                   ErrorCallback;        // +0x0C
    CCharString*            PStringToClean;       // +0x10
    bool                    UseDefaults;          // +0x14
    bool                    UseCRCCheck;          // +0x15
    bool                    SingleTransfer;       // +0x16
    bool                    Finished;             // +0x17
    EMode                   Mode;                 // +0x18
    bool                    Binary;               // +0x1C
    CCharString*            PStringToSaveTo;      // +0x20
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // ??$Transfer@..@CPersistContext@@QAEXPBDAA..AB..@Z
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
protected:
    template <class T> static T __fastcall GetDefaultVal();                                  // ??$GetDefaultVal@..@CPersistContext@@KI..XZ
    void CheckCRC(const char* name);   // 0x00404500
    void TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BAD60
    void TransferObjectLoadText(const char* name, CPersistContext_TransferableComponentGenericType& item);   // 0x009BBA40
};
template <> inline float __fastcall CPersistContext::GetDefaultVal<float>() { return 0.0f; }
template <> inline long __fastcall CPersistContext::GetDefaultVal<long>() { return 0; }
template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
#pragma auto_inline(off)
template <>
__declspec(noinline) void CPersistContext::Transfer<float>(const char* name, float& value, const float& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_RESET:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY: {
        CPersistTraits<float> traits;
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
    case MODE_LOAD_BINARY: {
        CPersistTraits<float> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<float> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<float> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

#pragma auto_inline(on)

class C2DCoordI { public: long X, Y; };
class C3DVector { public: float X, Y, Z; };
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {      // STLport 4.6 (12 bytes)
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return size_type(_M_finish - _M_start); }
};
}
class CTCShapeManager;
namespace NShape {
enum EShape { };                                             // W4EShape@NShape@@ (enumerators not modelled)
class CShape {                                               // vtable 0x0126A208
public:
    virtual ~CShape();                                       // slot 0 (scalar deleting 0x00764FC0)
    virtual EShape GetType() const = 0;                      // slot 1 (pure; CClosedShapeBase 0x00765040 returns 0)
    virtual void OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset);   // slot 2
    CTCShapeManager*        Owner;                           // +0x04
    std::vector<C3DVector>  Points;                          // +0x08
};

void CShape::OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset)
{
    CCharString name = CCharString("Shape[") + GFIntToCharString(index) + CCharString("]");
    CCharString typeName;
    switch (GetType()) {
    case 0:
        typeName = CLOSED_STRING_NAME;
        break;
    case 1:
        typeName = LINE_STRING_NAME;
        break;
    default:
        return;
    }
    context.Transfer(name + CCharString(".Type"), typeName);
    long count = Points.size();
    context.Transfer(name + CCharString(".size()"), count);
    long i = 0;
    for (std::vector<C3DVector>::iterator it = Points.begin(); it != Points.end(); ++it) {
        CCharString pointName = name + CCharString(".pos[") + GFIntToCharString(i++) + CCharString("]");
        float value = it->X - (float)offset.X;
        context.Transfer(pointName + ".X", value);
        value = it->Y - (float)offset.Y;
        context.Transfer(pointName + ".Y", value);
        value = it->Z;
        context.Transfer(pointName + ".Z", value);
    }
}
}