extern void __stdcall bil_helper(void* p, int n);
class CMainGameComponent { public: void BeginInputLoading(void* a); };
void CMainGameComponent::BeginInputLoading(void* a){ bil_helper(a, 2); }