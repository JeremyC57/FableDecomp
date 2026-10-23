// Grade: DIFFER(37v1330) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CMeshDataBank::InitialiseEntry(unsigned long id, unsigned long size, const CArray<char>& data) -- parse one mesh bank
//   entry: open a memory stream over the data, read the header fields (inline reads + TransferBinaryIn), the mesh name
//   and the null-terminated sub-names, and register the name -> id pair in the bank's map and lookup vector
// Leaf: InitialiseEntry
// Stub: first-pass skeleton only (the memory stream); the field reads and the map insert are not reconstructed yet.
template <class T> struct CArray { T* Data; long Count; };
struct CMemoryDataInputStream {
    unsigned char _body[0x1c];
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    ~CMemoryDataInputStream();                          // 0x00993B10
};
class CMeshDataBank {
public:
    void InitialiseEntry(unsigned long id, unsigned long size, const CArray<char>& data);
};
void CMeshDataBank::InitialiseEntry(unsigned long id, unsigned long size, const CArray<char>& data)
{
    CMemoryDataInputStream stream(data.Data, data.Count);
}
