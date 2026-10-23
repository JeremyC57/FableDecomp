#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_00578fce(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_00578fce(); if(this->m0) Free1(this->m0); }