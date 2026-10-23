// 00c3b970 CComplexShaderSet_LoadAllShaders
// Flags: none beyond /O2 /Oy /W3
// Leaf: LoadAllShaders
namespace X_00c3b970 {
struct CString{char*Text;CString();~CString();};
// Layout: PKSerialiseCParser.cpp receiver is opaque; helpers transfer in either direction. Transfer helpers are descriptive placeholders identified by byte width and their distinct retail addresses c31b50/60/70/80/90.
struct CArchive{bool TransferUInt(unsigned long&);bool TransferInt(long&);bool TransferShort(unsigned short&);bool TransferFloat(float&);bool TransferString(CString&);};
// Layout: target receiver has fields at0/4/8/c/e/10/14/18/1a/1c/1e/20/22/24/28/2c. Manifest class/method is unverified; no matching active header exists.
struct CComplexShaderSet{unsigned long Field00,Field04;CString Field08;unsigned short Field0c,Field0e;unsigned long Field10;long Field14;unsigned short Field18,Field1a,Field1c,Field1e,Field20,Field22;float Field24,Field28;long Field2c;int LoadAllShaders(CArchive&);};
int CComplexShaderSet::LoadAllShaders(CArchive&r){
 if(r.TransferUInt(Field00))
 if(r.TransferUInt(Field04))
 if(r.TransferString(Field08))
 if(r.TransferShort(Field0c))
 if(r.TransferShort(Field0e))
 if(r.TransferUInt(Field10))
 if(r.TransferInt(Field14))
 if(r.TransferShort(Field18))
 if(r.TransferShort(Field1a))
 if(r.TransferShort(Field1c))
 if(r.TransferShort(Field1e))
 if(r.TransferShort(Field22))
 if(r.TransferShort(Field20))
 if(r.TransferFloat(Field24))
 if(r.TransferFloat(Field28))
 if(r.TransferInt(Field2c))
 return 1;
 return 0;
}
}