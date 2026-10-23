#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_00453630(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_00453630(); if(this->m0) Free1(this->m0); }