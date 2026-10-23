#pragma optimize("s",on)
// CCountedPointer<T>::ShareData helper-release variant. factors old-ptr release into ReleaseCur (0x4057a0).
// pragma s. family len38 x8 (004190e2).
struct CCPtr { void* info; long* p; void ShareData(void* n, long* np); void ReleaseCur_0059a994(); };
void CCPtr::ShareData(void* n, long* np){ if(this->p!=np){ this->ReleaseCur_0059a994(); this->info=n; this->p=np; if(np) ++*np; } }