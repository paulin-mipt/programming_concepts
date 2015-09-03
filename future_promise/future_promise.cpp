#include <Windows.h>
#include <iostream>
#include "myfuture.h"
#include <assert.h>

unsigned __stdcall do_smth(void* p) {
	((PromiseClass<int>*)p)->SetValue(42);
	return 0;
}

unsigned __stdcall throw_exc(void* p) {
	((PromiseClass<int>*)p)->SetException();
	return 0;
}

int main() {	
	PromiseClass<int> promise;
	FutureClass<int> future = promise.GetFuture();

	//test an ordinary case
	int ans = 0;
	assert(!future.TryGet(&ans)); //no answer received yet
	assert(ans == 0);
	HANDLE handle, handle_exc;
	handle = (HANDLE)_beginthreadex(0, 0, &do_smth, (void*)(&promise), 0, 0);
	int x = future.GetValue();
	assert(future.TryGet(&ans)); //answer is ready
	assert(ans == 42);
	WaitForSingleObject(handle, INFINITE);
	CloseHandle(handle);

	//test exceptions
	PromiseClass<int> promise_exc;
	FutureClass<int> future_exc = promise_exc.GetFuture();
	try {
		handle_exc = (HANDLE)_beginthreadex(0, 0, &throw_exc, (void*)(&promise_exc), 0, 0);
		int x_exc = future_exc.GetValue();
	}
	catch (const std::exception &exc) {
		std::cout << exc.what();
	}

	WaitForSingleObject(handle_exc, INFINITE);
	CloseHandle(handle_exc);
	return 0;
}