#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_005795c3(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_005795c3(); if(this->m0) Free1(this->m0); }