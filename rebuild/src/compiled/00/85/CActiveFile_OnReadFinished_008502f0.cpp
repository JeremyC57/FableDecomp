// CActiveFile::OnReadFinished  @ 008502f0
// void __fastcall(CActiveFile* this)
//   push esi; mov esi,ecx; call 0x850280 (this-method);
//   esi = this->field0; if(esi){ operator delete(esi); }

void __cdecl free(void*);   // cdecl free/delete  @ 0xbfea14

struct CActiveFile {
    void* field0;               // +0x00
    void HelperReadFinished();  // @ 0x850280  (__fastcall, void)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->HelperReadFinished();
    void* p = this->field0;
    if (p) {
        free(p);
    }
}