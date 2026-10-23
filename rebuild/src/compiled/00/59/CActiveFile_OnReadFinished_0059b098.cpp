#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_0059af2a(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_0059af2a(); if(this->m0) Free1(this->m0); }