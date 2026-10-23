#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_0053c8a9(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_0053c8a9(); if(this->m0) Free1(this->m0); }