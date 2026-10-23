#pragma optimize("s",on)
struct CActiveFile { void* m0; void OnReadFinished(); void Base_00592ba7(); };
extern "C" void __cdecl Free1(void* p);
void CActiveFile::OnReadFinished(){ this->Base_00592ba7(); if(this->m0) Free1(this->m0); }