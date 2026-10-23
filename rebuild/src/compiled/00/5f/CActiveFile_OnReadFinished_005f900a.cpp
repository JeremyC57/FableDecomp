#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_005f8b7d(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_005f8b7d(); if(this->m0) Free1(this->m0); }