#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_004e7749(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_004e7749(); if(this->m0) Free1(this->m0); }