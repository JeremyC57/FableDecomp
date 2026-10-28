// CBankCreatorException::GetText() -- Return a copy of the owned value.
// Leaf: ?GetText@CBankCreatorException@@QBE?AVCCharString@@XZ
// Identity: PDB GetText const; caller009cf328 formats caught bank exception; EntrySymbol+0/ErrorText+4.
class CCharStringData;
class CCharString {public:CCharStringData* PStringData;CCharString(const CCharString&);~CCharString();};
// Layout: no active exception header; original EntrySymbol+0, ErrorText+4.
class CBankCreatorException {public:CCharString EntrySymbol,ErrorText;CCharString GetText() const;};
CCharString CBankCreatorException::GetText() const {return ErrorText;}