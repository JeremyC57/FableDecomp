#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_0042c02f(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_0042c02f(); if(this->m0) Free1(this->m0); }