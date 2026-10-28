// CBankFile::GetBankHandle() -- Return a copy of the owned value.
// Leaf: ?GetBankHandle@CBankFile@@QBE?AVCCharString@@XZ
// Identity: Own field+0x88 and corrected active header; PDB exact const getter; callers009f82ac/00a09ddc.
class CCharStringData;
class CCharString {public:CCharStringData* PStringData;CCharString(const CCharString&);~CCharString();};
#define CBankFile CBankFile_HeaderLayout
#include "engine/CBankFile.h"
#undef CBankFile
// Layout: active shared owner layout retained; opaque member viewed as its original composite type.
class CBankFile : public CBankFile_HeaderLayout {public:CCharString GetBankHandle() const;};
CCharString CBankFile::GetBankHandle() const {return *reinterpret_cast<const CCharString*>(&BankHandle_PStringData);}