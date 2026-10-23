#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_0057f58e(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_0057f58e(); if(this->m0) Free1(this->m0); }