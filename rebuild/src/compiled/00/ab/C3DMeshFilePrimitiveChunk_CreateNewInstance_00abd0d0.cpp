// C3DMeshFilePrimitiveChunk::CreateNewInstance() const -- `new C3DMeshFilePrimitiveChunk` (inline constructor, 0x38 bytes: one material index, -1) handed to the virtual InitNewInstance
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABD0D0, 128 bytes. Callees: operator new 0x00BFEA1A, C3DMeshFileChunk(const char*)
// 0x00AA8360 ("PRIM" @ 0x0129DEE4), vector<long>::resize(n, const long& value) 0x004428F0, this->InitNewInstance
// (slot 10, +0x28). Vtable: C3DMeshFilePrimitiveChunk 0x0129ED74.
// Retail layout (the PDB header engine/C3DMeshFilePrimitiveChunk.h sits 8 bytes high): MaterialIndices CArray<long>
// +0x2C (sizeof 0x38). The array is the STLport vector structure (_Vector_base zeroing start/finish, end of
// storage in an _STLP_alloc_proxy) addressed through `lea esi,[edi+2Ch]`; the constructor body resizes it to one
// element (the default value temporary homed in the `push ecx` slot) and sets that element to -1.
// ---- shared retail model for the C3DMeshFile*Chunk family (TLC Fable.exe) -----------------------
// The PDB header rebuild/include/engine/<Class>.h comes from Ego_r.pdb, a later build in which the
// C3DMeshFileChunk base is 0x34 bytes; in retail TLC it is 0x2c (every derived member sits 8 bytes
// lower, and the vtable slots are the same).  The bytes decide, so this file models the retail layout
// with the PDB member names and says so here instead of including the disagreeing header.
struct CCharString {
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

struct CMatrix3x4 { float m_row[3][4]; };               // 48 bytes, copied with rep movsd
struct C3DVector  { float X, Y, Z; C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {} };

struct CChunkedFileSaver;
struct CDataOutputStream {
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};

// CChunkedFileChunk (0x10) -> CChunkedFileSuperChunk -> C3DMeshFileChunk (0x2c in retail).
struct C3DMeshFileChunk {
    virtual ~C3DMeshFileChunk();                                 // slot 0 (scalar deleting dtor)
    virtual void v1();
    virtual CCharString GetChunkInfo() const;                    // slot 2
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0
    virtual C3DMeshFileChunk* CreateNewInstance() const;                                 // slot 9
    virtual C3DMeshFileChunk* InitNewInstance(C3DMeshFileChunk* chunk) const;            // slot 10 (+0x28)
    C3DMeshFileChunk(const char* chunkType);                     // 0x00AA8360
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// ------------------------------------------------------------------------------------------------

template <class T> struct allocator { allocator() {} };

template <class T>
struct _STLP_alloc_proxy {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>&, T* p) : _M_data(p) {}
};

template <class T>
struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
};

template <class T>
struct CArray : _Vector_base<T> {           // retail CArray = STLport vector
    CArray() : _Vector_base<T>(allocator<T>()) {}
    T* begin() { return this->_M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const T& value = T());   // 0x004428F0
};

struct C3DMeshFilePrimitiveChunk : C3DMeshFileChunk {
    CArray<long> MaterialIndices;   // +0x2c (PDB +0x34)   sizeof == 0x38
    C3DMeshFilePrimitiveChunk() : C3DMeshFileChunk("PRIM")
    {
        MaterialIndices.resize(1);
        MaterialIndices[0] = -1;
    }
    virtual C3DMeshFileChunk* CreateNewInstance() const;
};

C3DMeshFileChunk* C3DMeshFilePrimitiveChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFilePrimitiveChunk);
}