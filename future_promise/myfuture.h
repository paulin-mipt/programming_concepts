#include <process.h>
#include <memory>

template<class T>
class FutureClass;

template<class T>
class PromiseClass {
friend class FutureClass<T>;
private:
	FutureClass<T> future;
protected:
	std::shared_ptr<T> object;
	std::shared_ptr<bool> exception_occured;
	std::shared_ptr<HANDLE> setValueEvent;
public:
	PromiseClass() {
		setValueEvent = std::shared_ptr<HANDLE>(new HANDLE(CreateEvent(NULL, TRUE, FALSE, NULL)));
		object = std::shared_ptr<T>(new T());
		exception_occured = std::shared_ptr<bool>(new bool(false));
	}

	FutureClass<T> GetFuture() {
		future = FutureClass<T>(this);
		return future;
	}

	void SetValue(T value) {
		*object = value;
		SetEvent(*setValueEvent);
	}

	void SetException() {
		*exception_occured = true;
		SetEvent(*setValueEvent);
	}
};

template<class T>
class FutureClass {
private:
	std::shared_ptr<T> object;
	std::shared_ptr<bool> exception_occured;
	std::shared_ptr<HANDLE> setValueEvent;
public:
	FutureClass() {}

	FutureClass(PromiseClass<T> *_promise) {
		object = std::shared_ptr<T>(_promise->object);
		exception_occured = std::shared_ptr<bool>(_promise->exception_occured);
		setValueEvent = std::shared_ptr<HANDLE>(_promise->setValueEvent);
	}

	T GetValue() {
		WaitForSingleObject(*setValueEvent, INFINITE);
		if (*exception_occured) {
			throw std::exception("Exception occured in future o_o");
		} else {
			T value = *object;
			return value;
		}
	}

	bool TryGet(T *retplace) {
		if (WaitForSingleObject(*setValueEvent, 0) == WAIT_OBJECT_0) {
			*retplace = *object;
			return true;
		}
		else {
			return false;
		}
	}
};