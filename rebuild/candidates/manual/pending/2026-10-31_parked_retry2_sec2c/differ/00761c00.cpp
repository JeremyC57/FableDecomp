// Grade: DIFFER(816v821)/19 lines -- the point index is post-incremented inside the name expression (GFIntToCharString(i++)): string order, inc ebp placement and frame now retail; left: retail hoists the three 0.0f default temporaries' stores into the loop preheader (jmp + lea pad), ours stores each at its call. Tried after the fix: direct 3-arg 0.0f (55), declared default float() (55), named const default in the wrapper (19) (retry wave 2 sec2c)
// Was: DIFFER(810v821) -- prologue/Type/size() sections match; the point loop differs: retail hoists the three 0.0f
//   Transfer defaults into the preheader (stores before the loop jmp) and builds the "]" temporary of the pos[i] name
//   after the name+".pos["+IntToString(i) part (yet destroys it last). Tried: explicit 0.0f, declared default argument,
//   inline 2-arg Transfer overloads (0.0f / float()), one shared value variable.
// NShape::CShape::OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset) -- persist shape `index` as
//   "Shape[<index>]": its type name ("<name>.Type", only for the two known shape types), the point count
//   ("<name>.size()") and every point ("<name>.pos[<i>].X/.Y/.Z", X and Y relative to the integer offset).
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00761C00, 832 bytes (821 + int3 padding), `ret 0Ch` (PDB
//   ?OnSerialise@CShape@NShape@@UAEXAAVCPersistContext@@JABVC2DCoordI@@@Z).
// Callees: CCharString(const char*, long) 0x0099EBF0, GFIntToCharString(long) 0x0099F830 (__fastcall, hidden result in ecx),
//   operator+(const CCharString&, const CCharString&) 0x0099F570 (__fastcall), operator+(const CCharString&, const char*)
//   0x0099F600 (__fastcall), ~CCharString 0x0099EAE0, CCharString() 0x0099E4B0, CCharString(const CCharString&) 0x0099EC30,
//   CCharString::operator= 0x0099EFB0, operator const char* 0x0099E4C0, CPersistContext::Transfer<CCharString> 0x004109A0,
//   Transfer<long> 0x00410BE0, Transfer<float> 0x00410620; the shape's virtual slot 1 (+0x4, its type).
// Data: the two shape-type names are CCharString globals at 0x013BAD90 (type 0) and 0x013BAD8C (type 1).
// Layout: Ego_r.pdb NShape::CShape (40 bytes): Owner +0x4, Points vector<C3DVector> +0x8 (12-byte retail STLport vector).
// Shape: the "Shape[" / "]" literals and ".Type" / ".size()" / ".pos[" are CCharString temporaries joined by the
//   CCharString+CCharString operator (only ".X"/".Y"/".Z" use the const char* one); the Type default is an inline by-value
//   getter (VC7.1 builds the named local, copies it into the argument and destroys it), as the landed
//   CTCHeroCentreDoorMarker::OnSerialise 0x0077F650.
struct CCharStringData;
struct CCharString {
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
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
extern CCharString g_ShapeTypeName0;                         // 0x013BAD90
extern CCharString g_ShapeTypeName1;                         // 0x013BAD8C

struct CPersistContext {
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);   // 0x004109A0
    void Transfer(const char* name, long& value, const long& defaultValue);                 // 0x00410BE0
    void Transfer(const char* name, float& value, const float& defaultValue);               // 0x00410620
    void Transfer(const char* name, CCharString& value) { Transfer(name, value, EmptyCCharString()); }
    void Transfer(const char* name, long& value) { Transfer(name, value, 0L); }
    void Transfer(const char* name, float& value) { Transfer(name, value, 0.0f); }
};

struct C2DCoordI { long X, Y; };
struct C3DVector { float X, Y, Z; };
struct C3DVectorArray {                                      // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};

struct CTCShapeManager;
namespace NShape {
struct CShape {
    virtual void v00();
    virtual long GetType() const;                            // slot 1 (+0x4)
    CTCShapeManager* Owner;                                  // +0x4
    C3DVectorArray   Points;                                 // +0x8
    void OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset);
};

void CShape::OnSerialise(CPersistContext& context, long index, const C2DCoordI& offset)
{
    CCharString name = CCharString("Shape[") + GFIntToCharString(index) + CCharString("]");
    CCharString typeName;
    switch (GetType()) {
    case 0:
        typeName = g_ShapeTypeName0;
        break;
    case 1:
        typeName = g_ShapeTypeName1;
        break;
    default:
        return;
    }
    context.Transfer(name + CCharString(".Type"), typeName);
    long count = Points.size();
    context.Transfer(name + CCharString(".size()"), count);
    long i = 0;
    for (C3DVector* it = Points.begin(); it != Points.end(); ++it) {
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
