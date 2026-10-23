#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_004da1a4(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_004da1a4(); if(this->m0) Free1(this->m0); }