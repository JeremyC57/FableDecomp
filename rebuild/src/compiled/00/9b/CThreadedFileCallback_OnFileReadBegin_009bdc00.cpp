// Constant-return member: returns true. __fastcall this=ecx.
struct CThreadedFileCallback { bool OnFileReadBegin(int a0, int a1); };
bool CThreadedFileCallback::OnFileReadBegin(int a0, int a1) { return true; }