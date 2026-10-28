// PKCAutoDelete<CUnprovenTransferRecord_00C3B850>::~PKCAutoDelete() -- Destroy and release the owned record when nonempty.
// Leaf: ??1?$PKCAutoDelete@VCUnprovenTransferRecord_00C3B850@@@@QAE@XZ
// Identity: Own unwind funclet from00c2f7d0 calls this destructor; sibling Release00c2f070 asserts actual PKCAutoDelete.h filename. Same record transfer00C3B850 and destructor00c3b840, allocation32 bytes. Original record name unknown; shared descriptive record spelling with N200/N201/N203.
// Layout: genuine libpk PKCAutoDelete<T> has one owning pointer0. Unknown record has own allocation size32; destructor is external, no fictitious fields or behavior modeled. No shared headers.
#include <new>
class CUnprovenTransferRecord_00C3B850{public:unsigned char Unmodeled[32];~CUnprovenTransferRecord_00C3B850();};
template<class T>class PKCAutoDelete{public:T* m_pObject;explicit PKCAutoDelete(T* p):m_pObject(p){}~PKCAutoDelete(){if(m_pObject){delete m_pObject;m_pObject=0;}}};
template PKCAutoDelete<CUnprovenTransferRecord_00C3B850>::~PKCAutoDelete();